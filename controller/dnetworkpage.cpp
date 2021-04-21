#include "dnetworkpage.h"
#include "cbitmapbutton.h"
#include "mainwindow.h"
#include "exconfig.h"
#include "dnetworkwidget.h"

DNetworkPage::DNetworkPage(QObject *parent, CBaseWidget *widget, MainWindow *wndMain)
    : CSubPage(parent, widget, wndMain)

{
    m_iNetworkMask = gGlobalParam.MiscParam.iNetworkMask;
    creatTitle();
    initUi();
    buildTranslation();
}

DNetworkPage::~DNetworkPage()
{
}

void DNetworkPage::creatTitle()
{
    CSubPage::creatTitle();
    buildTitles();
    selectTitle(0);
}

void DNetworkPage::switchLanguage()
{
    buildTranslation();
    buildTitles();
    selectTitle(titleIndex());
}

void DNetworkPage::buildTranslation()
{
    m_astrNetName[0] = tr("CAN");
    m_astrNetName[1] = tr("Zigbee");
    m_astrNetName[2] = tr("WIFI");

    for (int iLoop = 0; iLoop < DISPLAY_NETWORK_NUM; iLoop++)
    {
        m_laName[iLoop]->setText(m_astrNetName[iLoop]);
    }

    m_pBtnSave->setTip(tr("Save"), QColor(228, 231, 240), BITMAPBUTTON_TIP_CENTER);
    m_pNetworkWidget->buildTranslation();
}

void DNetworkPage::initUi()
{
    m_strQss4Chk = m_wndMain->getQss4Chk();
    setBackColor();

    for (int iLoop = 0; iLoop < DISPLAY_NETWORK_NUM; iLoop++)
    {
        m_pBackWidget[iLoop] = new QWidget(m_widget);
        QPalette pal(m_pBackWidget[iLoop]->palette());
        pal.setColor(QPalette::Background, Qt::gray);
        m_pBackWidget[iLoop]->setAutoFillBackground(true);
        m_pBackWidget[iLoop]->setPalette(pal);
        m_pBackWidget[iLoop]->setGeometry(QRect(124, 80 + 60 * (iLoop - 1), 530, 50));

        m_laName[iLoop] = new QLabel(m_pBackWidget[iLoop]);
        m_laName[iLoop]->setPixmap(NULL);
        m_laName[iLoop]->setGeometry(QRect(25, 15, 180, 20));
        m_laName[iLoop]->show();

        m_chkSwitchs[iLoop] = new QCheckBox(m_pBackWidget[iLoop]);
        m_chkSwitchs[iLoop]->setGeometry(QRect(480, 5, 40, 40));
        m_chkSwitchs[iLoop]->setStyleSheet(m_strQss4Chk);
        m_chkSwitchs[iLoop]->show();

        if (m_iNetworkMask & (1 << iLoop))
        {
            m_chkSwitchs[iLoop]->setChecked(true);
        }
        else
        {
            m_chkSwitchs[iLoop]->setChecked(false);
        }

        connect(m_chkSwitchs[iLoop], SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxChangeState(int)));

        if (DISPLAY_NETWORK_CAN == iLoop)
        {
            m_pBackWidget[iLoop]->hide();
        }
        if (gAdditionalCfgParam.machineInfo.iMachineFlow >= 500)
        {
            if (DISPLAY_NETWORK_ZIGBEE == iLoop)
            {
                m_pBackWidget[iLoop]->hide();
            }
        }
    }
    m_pNetworkWidget = new DNetworkWidget(m_widget);
    m_pNetworkWidget->move(124, 200);
    connect(this, SIGNAL(wifiEnabled(bool)), m_pNetworkWidget, SLOT(setWifiEnabled(bool)));

    m_pBtnSave = new CBitmapButton(m_widget, BITMAPBUTTON_STYLE_PUSH, BITMAPBUTTON_PIC_STYLE_NORMAL, NETWORKPAGE_BTN_SAVE);
    m_pBtnSave->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_PAGE_NAVI_NORMAL]);
    m_pBtnSave->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_PAGE_NAVI_ACTIVE]);
    m_pBtnSave->setGeometry(700, 560, m_pBtnSave->getPicWidth(), m_pBtnSave->getPicHeight());
    m_pBtnSave->setStyleSheet("background-color:transparent");
    connect(m_pBtnSave, SIGNAL(clicked(int)), this, SLOT(onBtnClicked(int)));
    m_pBtnSave->show();

    emit wifiEnabled(Qt::Checked == m_chkSwitchs[DISPLAY_NETWORK_WIFI]->checkState());
}

void DNetworkPage::leaveSubPage()
{
    CSubPage::leaveSubPage();
}

void DNetworkPage::update()
{
    m_pNetworkWidget->setAddCheckBoxChecked(false);

    if (gGlobalParam.MiscParam.iNetworkMask & 1 << DISPLAY_NETWORK_ZIGBEE)
    {
        m_chkSwitchs[DISPLAY_NETWORK_ZIGBEE]->setChecked(true);
    }
    else
    {
        m_chkSwitchs[DISPLAY_NETWORK_ZIGBEE]->setChecked(false);
    }

    if (gGlobalParam.MiscParam.iNetworkMask & 1 << DISPLAY_NETWORK_WIFI)
    {
        m_chkSwitchs[DISPLAY_NETWORK_WIFI]->setChecked(true);
    }
    else
    {
        m_chkSwitchs[DISPLAY_NETWORK_WIFI]->setChecked(false);
    }
}

bool DNetworkPage::wifiCheckState()
{
    if(Qt::Checked == m_chkSwitchs[DISPLAY_NETWORK_WIFI]->checkState())
    {
        return true;
    }
    return false;
}

void DNetworkPage::showSaveBtn(bool iShow)
{
    if(iShow)
    {
        m_pBtnSave->show();
    }
    else
    {
        m_pBtnSave->hide();
    }
}

void DNetworkPage::onBtnClicked(int iIndex)
{
    switch(iIndex)
    {
    case NETWORKPAGE_BTN_SAVE:
        save();
        show(false);
        m_parent->show(true);
        break;
    default:
        break;
    }

    m_wndMain->prepareKeyStroke();
}

void DNetworkPage::onCheckBoxChangeState(int state)
{
    Q_UNUSED(state);

    for (int iLoop = 0; iLoop < DISPLAY_NETWORK_NUM; iLoop++)
    {
        if ((Qt::Checked == m_chkSwitchs[iLoop]->checkState()))
        {
            m_iNetworkMask |= 1 << iLoop;
        }
        else
        {
            m_iNetworkMask &= ~(1 << iLoop);
        }
    }
    emit wifiEnabled(Qt::Checked == m_chkSwitchs[DISPLAY_NETWORK_WIFI]->checkState());
}

void DNetworkPage::buildTitles()
{
    QStringList stringList;
    stringList << tr("Connectivity");
    setTitles(stringList);
}

void DNetworkPage::save()
{
    if (m_iNetworkMask != gGlobalParam.MiscParam.iNetworkMask)
    {
        DISP_MISC_SETTING_STRU MiscParam = gGlobalParam.MiscParam;
        MiscParam.iNetworkMask = m_iNetworkMask;
        MainSaveMiscParam(gGlobalParam.iMachineType, MiscParam);
        MainUpdateSpecificParam(NOT_PARAM_MISC_PARAM);
    }
    m_wndMain->MainWriteLoginOperationInfo2Db(SETPAGE_SYSTEM_NETWORK);
}

void DNetworkPage::setBackColor()
{
    QSize size(width(), height());
    QImage image_bg = QImage(size, QImage::Format_ARGB32);
    QPainter p(&image_bg);
    p.fillRect(image_bg.rect(), QColor(228, 231, 240));
    QPalette pal(m_widget->palette());
    pal.setBrush(m_widget->backgroundRole(), QBrush(image_bg));
    m_widget->setAutoFillBackground(true);
    m_widget->setPalette(pal);
}




