#include "dflowchartpage.h"
#include <QMouseEvent>
#include "mainwindow.h"
#include "titlebar.h"
#include "dflowchartwidget.h"
#include "dtextticker.h"
#include <QPainter>

DFlowChartPage::DFlowChartPage(QObject *parent, CBaseWidget *widget, MainWindow *wndMain)
                                    : CPage(parent, widget, wndMain)
{
    initUi();
    buildTranslation();
    m_iTankLevel = 0;
}

void DFlowChartPage::creatTitle()
{
    CPage::creatTitle();
    buildTitles();
    selectTitle(0);
}

void DFlowChartPage::buildTitles()
{
    QStringList stringList;
    stringList << tr("Flow Chart");
    setTitles(stringList);
}

void DFlowChartPage::on_navi_clicked(int tmp)
{
    switch(tmp)
    {
    case TITLE_BAR_ID_POWER_OFF:
        break;
    case TITLE_BAR_ID_BACK:
        break;
    case TITLE_BAR_ID_HOME:
        show(false);
        m_wndMain->home();
        break;
    }
}

void DFlowChartPage::updateUnits()
{
    m_pFlowChartWidget->updateUnits();
}

void DFlowChartPage::on_updateAlarmMsg(const QString &msg, bool isAdd)
{
    m_pTextTicker->updateShowText(msg, isAdd);
}

void DFlowChartPage::switchLanguage()
{
    buildTranslation();
    buildTitles();
    selectTitle(titleIndex());
}

void DFlowChartPage::buildTranslation()
{
    m_pFlowChartWidget->setTitleText(tr("Runing Status"));
}

void DFlowChartPage::initUi()
{
    QSize size(width(),height());
    QImage image_bg = QImage(size, QImage::Format_ARGB32);
    QPainter p(&image_bg);
    p.fillRect(image_bg.rect(), QColor(228, 231, 240));
    QPalette pal(m_widget->palette());
    pal.setBrush(m_widget->backgroundRole(),QBrush(image_bg));
    m_widget->setAutoFillBackground(true);
    m_widget->setPalette(pal);

    m_pFlowChartWidget = new DFlowChartWidget(m_widget);
    m_pFlowChartWidget->setGeometry(10, 60, 780, 500);

    m_pTextTicker = new DTextTicker(m_widget);
    m_pTextTicker->move(10, 565);

    m_pTitleBar = new titleBar(m_widget);
    m_pTitleBar->setGeometry(0,0,800,55);
    connect(m_pTitleBar, SIGNAL(clicked(int)), this, SLOT(on_navi_clicked(int)));
}

void DFlowChartPage::update()
{
    updateWorkState();
    updateSwitchInfo();

    unsigned char buf[16];

    DISP_CMD_SWITCH_STATE_REPORT_STRU *cmd = (DISP_CMD_SWITCH_STATE_REPORT_STRU *)buf;
    cmd->iRptFlag = 1;
    DispCmdEntry(DISP_CMD_SWITCH_REPORT, buf, sizeof(DISP_CMD_SWITCH_STATE_REPORT_STRU));
}

void DFlowChartPage::fade()
{
    unsigned char buf[16];
    DISP_CMD_SWITCH_STATE_REPORT_STRU *cmd = (DISP_CMD_SWITCH_STATE_REPORT_STRU *)buf;
    cmd->iRptFlag = 0;
    DispCmdEntry(DISP_CMD_SWITCH_REPORT, buf, sizeof(DISP_CMD_SWITCH_STATE_REPORT_STRU));
}

void DFlowChartPage::updateSwitchInfo()
{
    m_pFlowChartWidget->updateSwitchInfo();
}

void DFlowChartPage::updateRpumpInfo(int iChl)
{
    m_pFlowChartWidget->updateRpumpInfo(iChl);
}

void DFlowChartPage::updateGpumpInfo(int iChl)
{
    m_pFlowChartWidget->updateGpumpInfo(iChl);
}

void DFlowChartPage::updateRectInfo(int iChl)
{
    m_pFlowChartWidget->updateRectInfo(iChl);
}

void DFlowChartPage::updateEdiInfo(int iChl)
{
    m_pFlowChartWidget->updateEdiInfo(iChl);
}

void DFlowChartPage::updateWorkState()
{
    QString strInfo1 = "UP:  ";
    QString strInfo2 = "HP:  ";
    QString strInfo3;

    if((m_iTankLevel >= B2_FULL)
        && (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveB2)))
    {
        strInfo3 = tr("Tank Full");
    }

    switch(DispGetWorkState())
    {
    case DISP_WORK_STATE_IDLE:
         if (DispGetFwFlag())
         {
             strInfo3 = tr("Fill Feed Tank");
         }
         break;
    case DISP_WORK_STATE_PREPARE:
        if(NOT_RUNING_STATE_FLUSH == DispGetRunningStateFlag())
        {
            strInfo2 += tr("Flush");
        }
        break;
    case DISP_WORK_STATE_RUN:
        switch(gGlobalParam.iMachineType)
        {
        case MACHINE_ADAPT:
            if(CcbGetTwFlag() || CcbGetTwPendingFlag())
            {
                if(NOT_RUNING_STATE_FLUSH == DispGetRunningStateFlag())
                {
                    strInfo2 += tr("Flush");
                }
                else if(NOT_RUNING_STATE_CLEAN == DispGetRunningStateFlag())
                {
                    strInfo2 += tr("Rinsing");
                }
            }
            break;
        default:
            if(NOT_RUNING_STATE_CLEAN == DispGetRunningStateFlag())
            {
                strInfo2 += tr("Rinsing");
            }
            if (DispGetPwFlag() && (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveB2)))
            {
                strInfo3 = tr("Filling");
            }
            break;
        }
        break;
    case DISP_WORK_STATE_LPP:
         strInfo3 = tr("");
         break;
    case DISP_WORK_STATE_KP:
         strInfo3 = tr("");
         break;
    }

    switch(DispGetWorkState4Pw())
    {
    case DISP_WORK_STATE_IDLE:
         strInfo1 +=  tr("Standby");
         strInfo2 +=  tr("Standby");
         break;
    case DISP_WORK_STATE_RUN:
        if (DispGetUpQtwFlag())
        {
             strInfo1 +=  tr("Dispense");
        }
        else if (DispGetUpCirFlag())
        {
            strInfo1 +=  tr("Recirculation");
        }
        else
        {
            strInfo1 +=  tr("Ready");
        }

        if(NOT_RUNING_STATE_NONE == DispGetRunningStateFlag())
        {
            if (MACHINE_FUNCTION_EDI == (MACHINE_FUNCTION_EDI & gaMachineType[gGlobalParam.iMachineType].iFunctions))
            {
                if (DispGetEdiQtwFlag())
                {
                    strInfo2 +=  tr("Dispense");
                }
                else
                {
                    strInfo2 +=  tr("Ready");
                }
             }
        }

        if (DispGetTankCirFlag())
        {
            strInfo3 = tr("Recirculation");
        }
        break;
    }

    if (MACHINE_FUNCTION_UP == (MACHINE_FUNCTION_UP & gaMachineType[gGlobalParam.iMachineType].iFunctions))
    {
        m_pFlowChartWidget->setInfo1(strInfo1);
    }
    if (MACHINE_FUNCTION_EDI == (MACHINE_FUNCTION_EDI & gaMachineType[gGlobalParam.iMachineType].iFunctions))
    {
        m_pFlowChartWidget->setInfo2(strInfo2);
    }
    m_pFlowChartWidget->setInfo3(strInfo3);
}

void DFlowChartPage::updTank(int iIndex, float fVolume)
{
    m_iTankLevel = iIndex;
    m_fTankLevel = fVolume;
    m_pFlowChartWidget->updTank(iIndex, fVolume);
}

void DFlowChartPage::updEcoInfo(int iIndex, ECO_INFO_STRU *info)
{
    m_pFlowChartWidget->updEcoInfo(iIndex, info);
}

void DFlowChartPage::updPressure(int iIndex,float fvalue)
{
    m_pFlowChartWidget->updPressure(iIndex, fvalue);
}

void DFlowChartPage::updSwPressure(float fvalue)
{
	m_pFlowChartWidget->updSwPressure(fvalue);
}

void DFlowChartPage::updFlowInfo(int iIndex,int Value)
{
    m_pFlowChartWidget->updFlowInfo(iIndex, Value);
}

void DFlowChartPage::updSourceTank(int iIndex, float fVolume)
{
    m_pFlowChartWidget->updSourceTank(iIndex, fVolume);
}

void DFlowChartPage::updTOC(float fToc)
{
    m_pFlowChartWidget->updTOC(fToc);
}

void DFlowChartPage::mousePressEvent(QMouseEvent *e)
{
    if (!m_lstFlag)
    {
        m_lstX = e->x();
        m_lstY = e->y();
        m_curX = e->x();
        m_curY = e->y();
        m_lstFlag = 1;
    }
}

void DFlowChartPage::mouseReleaseEvent(QMouseEvent *e)
{
    if (abs(m_curX - m_lstX) >= PAGE_X_DIMENSION
        && abs(m_curY - m_lstY) <= PAGE_Y_DIMENSION)
    {

        m_wndMain->naviPage(0,m_curX - m_lstX > 0 ? 1 : 0);
    }
    m_lstFlag = 0;
}

void DFlowChartPage::mouseMoveEvent(QMouseEvent *e)
{
    if (0 == e->x() && 0 == e->y())
    {
       return;
    }
    m_curX = e->x();
    m_curY = e->y();
}



