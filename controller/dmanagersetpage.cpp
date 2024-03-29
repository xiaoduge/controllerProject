#include "dmanagersetpage.h"
#include "mainwindow.h"
#include "drephilinkprotocoldlg.h"
#include "dnetworkwidget.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/rtc.h>
#include <unistd.h>
#include <time.h>
#include <QButtonGroup>
#include <QFile>
#include "Display.h"
#include "exconfig.h"
#include "dhintdialog.h"
#include <QRadioButton>
#include "dlineedit.h"
#include <QButtonGroup>
#include <QCalendarWidget>
#include <QSlider>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QSpinBox>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QVBoxLayout>

DManagerSetPage::DManagerSetPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    creatTitle();
    initUi();
    buildTranslation();
}

DManagerSetPage::~DManagerSetPage()
{
	switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_EDI_LOOP:
	case MACHINE_L_RO_LOOP:
        if(gAdditionalCfgParam.machineInfo.iMachineFlow >= 500)
        {
            if(m_pageWidget[MANAGER_PAGE_CALIBRATION])
            {
                m_pageWidget[MANAGER_PAGE_CALIBRATION]->deleteLater();
                m_pageWidget[MANAGER_PAGE_CALIBRATION] = NULL;
            }
            if(m_pageWidget[MANAGER_PAGE_FINALFILTER])
            {
                m_pageWidget[MANAGER_PAGE_FINALFILTER]->deleteLater();
                m_pageWidget[MANAGER_PAGE_FINALFILTER] = NULL;
            }
        }
        break;
    default:
        break;
    }
}

void DManagerSetPage::creatTitle()
{
    CSubPage::creatTitle();
    buildTitles();
    selectTitle(0);
}

void DManagerSetPage::buildTitles()
{
    QStringList stringList;
    stringList << tr("Manager Config");
    setTitles(stringList);
}

void DManagerSetPage::buildTranslation()
{
    //机型流量大于500，没有参数校正和终端过滤器配置
    if(gAdditionalCfgParam.machineInfo.iMachineFlow >= 500)
    {
        m_tabWidget->setTabText(0, tr("Time & Date"));
        m_tabWidget->setTabText(1, tr("LCD"));
        m_tabWidget->setTabText(2, tr("Network"));
        m_tabWidget->setTabText(3, tr("Additional Settings"));
    }
    else
    {
        m_tabWidget->setTabText(0, tr("Time & Date"));
        m_tabWidget->setTabText(1, tr("Calibration"));
        m_tabWidget->setTabText(2, tr("LCD"));
        m_tabWidget->setTabText(3, tr("Final Filter"));
        m_tabWidget->setTabText(4, tr("Network"));
        m_tabWidget->setTabText(5, tr("Additional Settings"));
    }

    //Time
    int iLoop;
    m_astrDateName[0] = tr("Select Date");
    m_astrDateName[1] = tr("Select Time");
    for( iLoop = 0 ; iLoop < DATE_NUM ; iLoop++)
    {
        lbName[iLoop]->setText(m_astrDateName[iLoop]);
    }
    lbTitName->setText(tr("Date & Time"));
    m_pBtns[TIMEPAGE_BTN_CANCEL]->setText(tr("Cancel"));
    m_pBtns[TIMEPAGE_BTN_OK]->setText(tr("OK"));

    //Dispense Rate
    m_pCaliS1Label->setText(tr("Disp. Rate"));
    m_pCaliBtn->setText(tr("Save"));
#ifdef STEPPERMOTOR
    m_pStepperLabel->setText(tr("Motor Valve"));
#endif
    //LCD
    m_DispNames[0] = tr("Brightness");
    m_DispNames[1] = tr("Energy-saving");
    m_sleepLabel -> setText(tr("SleepTime"));
    for( iLoop = 0 ; iLoop < 2 ; iLoop++)
    {
        laName[iLoop]->setText(m_DispNames[iLoop]);
    }
    m_pLcdBtnSave->setText(tr("Save"));

    m_pFilterLabel[0]->setText(tr("Final Fliter A"));
    if(0 == gAdditionalCfgParam.productInfo.iCompany)
    {
        m_pFilterLabel[1]->setText(tr("Final Fliter B"));
    }
    else
    {
        m_pFilterLabel[1]->setText(tr("Bio-filter"));
    }
    m_pFilterSaveBtn->setText(tr("Save"));

    //Network
    m_pWifiLabel->setText(tr("WIFI"));
    m_pNetworkSaveBtn->setText(tr("Save"));

    //Additional Settings
    m_strSounds[0] = tr("Touch-tone");
    m_strSounds[1] = tr("Audio Alarms");
    for(iLoop = 0 ; iLoop < DISPLAY_SOUND_NUM ; iLoop++)
    {
        m_lblNames[iLoop]->setText(m_strSounds[iLoop]);
    }
    
    m_pAdditionalLb[REPHILINK_SETTING]->setText(tr("RephiLink"));
    m_pAdditionalLb[HPCIR_SETTING]->setText(tr("HP Recir."));
    m_pAdditionalLb[COMPENSATION_SETTING]->setText(tr("Temperature Compensation"));
    
    m_pNetworkWidget->buildTranslation();
}

void DManagerSetPage::switchLanguage()
{
    buildTranslation();
    buildTitles();
    selectTitle(titleIndex());
}

void DManagerSetPage::setBackColor()
{
    QSize size(width(),height());
    QImage image_bg = QImage(size, QImage::Format_ARGB32);
    QPainter p(&image_bg);
    p.fillRect(image_bg.rect(), QColor(228, 231, 240));
    QPalette pal(m_widget->palette());
    pal.setBrush(m_widget->backgroundRole(),QBrush(image_bg));
    m_widget->setAutoFillBackground(true);
    m_widget->setPalette(pal);
}

void DManagerSetPage::initUi()
{
    setBackColor();

    m_mainWidget = new QWidget(m_widget);
    m_mainWidget->setGeometry(QRect(0, 55, 800, this->height() - 55));

    QGridLayout *mainLayout = new QGridLayout;
    m_tabWidget = new QTabWidget;
    //add page
    initTimePage();
    initCalibrationPage();

    initLcdPage();
    initFinalFilterPage();
    initNetworkPage();
    initAdditionalSettingsPage();

    if(gAdditionalCfgParam.machineInfo.iMachineFlow >= 500)
    {
        m_tabWidget->removeTab(1);
        m_tabWidget->removeTab(2);
    }

    mainLayout->addWidget(m_tabWidget, 0, 0);
    m_mainWidget->setLayout(mainLayout);

    QFile qss(":/app/tabWidget.qss");
    qss.open(QFile::ReadOnly);
    QString tabWidgetqss = QLatin1String (qss.readAll());
    qss.close();

    m_tabWidget->setStyleSheet(tabWidgetqss);
    m_tabWidget->setFocusPolicy(Qt::NoFocus);
}

void DManagerSetPage::update()
{
    //Time
    QDateTime sysDateTime;
    sysDateTime = QDateTime::currentDateTime();

    m_pBtns[TIMEPAGE_BTN_DATE_SET]->setText(sysDateTime.toString("yyyy-MM-dd"));
    m_pBtns[TIMEPAGE_BTN_TIME_SET]->setText(sysDateTime.toString("hh:mm:ss"));

    //cali
    if(gGlobalParam.Caliparam.pc[m_caliId].fk > 100)
    {
        m_pCaliS1LineEdit->setText(QString::number(gCaliParam.pc[m_caliId].fk));
    }
    else
    {
        m_pCaliS1LineEdit->setText(QString::number(gCaliParam.pc[m_caliId].fk,'f',3));
    }
#ifdef STEPPERMOTOR
    if(gGlobalParam.SubModSetting.ulAddFlags & (1 << DISP_SM_STEPPERMOTOR))
    {
        if(!m_pStepperWidget->isVisible())
        {
            m_pStepperWidget->show();
        }
        m_pStepperSlider->setValue(gCaliParam.stepperCali.iStart - STEPPER_REFERENCD_POINT);
        m_pStepperValueLB->setText(QString::number(m_pStepperSlider->value()));
    }
    else
    {
        if(m_pStepperWidget->isVisible())
        {
            m_pStepperWidget->hide();
        }
    }
#endif
    //LCD
    m_iEnergySave = gGlobalParam.MiscParam.iEnerySave;
    if(m_iEnergySave)
    {
        m_pCheckEnergySave->setCheckState(Qt::Checked);
        m_pLcdBackWidget[2]->show();
    }
    else
    {
        m_pCheckEnergySave->setCheckState(Qt::Unchecked);
        m_pLcdBackWidget[2]->hide();
    }
    m_iSleepTime = gAdditionalCfgParam.additionalParam.iScreenSleepTime;
    m_comboBox->setCurrentIndex(m_iSleepTime - 1);

    if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_FINALFILTER_A))
    {
        m_pFilterCheck[0]->setChecked(true);
    }
    else
    {
        m_pFilterCheck[0]->setChecked(false);
    }
    if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_FINALFILTER_B))
    {
        m_pFilterCheck[1]->setChecked(true);
    }
    else
    {
        m_pFilterCheck[1]->setChecked(false);
    }

    //Network
    if (gGlobalParam.MiscParam.iNetworkMask & 1 << DISPLAY_NETWORK_WIFI)
    {
        m_pWifiCheckBox->setChecked(true);
    }
    else
    {
        m_pWifiCheckBox->setChecked(false);
    }

    //Additional Settings
    for(int iLoop = 0; iLoop < DISPLAY_SOUND_NUM; ++iLoop)
    {
        if (m_iSoundMask & (1 << iLoop))
        {
            m_chkSwitchs[iLoop]->setChecked(true);
        }
        else
        {
            m_chkSwitchs[iLoop]->setChecked(false);
        }
    }

    if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_REPHILINK))
    {
        m_pAdditionalCheck[REPHILINK_SETTING]->setChecked(true);
    }
    else
    {
        m_pAdditionalCheck[REPHILINK_SETTING]->setChecked(false);
    }

    if(gAdditionalCfgParam.machineInfo.iMachineFlow < 500)
    {
        if (gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir))
        {
            m_pAdditionalCheck[HPCIR_SETTING]->setChecked(true);
        }
        else
        {
            m_pAdditionalCheck[HPCIR_SETTING]->setChecked(false);
        }
    }
    else
    {
        if(m_pAdditionalWidget[HPCIR_SETTING]->isVisible())
        {
            m_pAdditionalWidget[HPCIR_SETTING]->hide();
        }
    }

    if(gGlobalParam.SubModSetting.ulAddFlags & (1 << DISP_SM_COMPENSATION))
    {
         m_pAdditionalCheck[COMPENSATION_SETTING]->setChecked(true);
    }
    else
    {
         m_pAdditionalCheck[COMPENSATION_SETTING]->setChecked(false);
    }
}

void DManagerSetPage::show(bool bShow)
{
    if (bShow)
    {
        m_widget->show() ;

        if (m_wndMain ) m_wndMain->setCurrentPage(this);

        m_secondTimer = startTimer(1000);
    }
    else
    {
        m_widget->hide();
        if(0 != m_secondTimer) killTimer(m_secondTimer);
    }
}

void DManagerSetPage::timerEvent(QTimerEvent *event)
{
    if(m_secondTimer == event->timerId())
    {
        if(0 == m_tabWidget->currentIndex())
        {
            QTime curTime;
            curTime = QTime::currentTime();
            m_pBtns[TIMEPAGE_BTN_TIME_SET]->setText(curTime.toString("hh:mm:ss"));
        }
    }
}

void DManagerSetPage::on_timeDateSet_clicked()
{
    ShowWidget[DATE_SET]->hide();
    ShowWidget[TIME_SET]->hide();

    CalS->show();
    m_pBtns[TIMEPAGE_BTN_CANCEL]->show();
    m_pBtns[TIMEPAGE_BTN_OK]->show();

    m_wndMain->prepareKeyStroke();
}

void DManagerSetPage::on_timeTimeSet_clicked()
{
    QDateTime sysDateTime;
    sysDateTime = QDateTime::currentDateTime();

    int hour = sysDateTime.time().hour();
    int min  = sysDateTime.time().minute();
    int sec  = sysDateTime.time().second();

    TimeHCbox->show();
    TimeHCbox->setCurrentIndex(hour);

    TimeMCbox->show();
    TimeMCbox->setCurrentIndex(min);

    TimeSCbox->show();
    TimeSCbox->setCurrentIndex(sec);

    m_pBtns[TIMEPAGE_BTN_OK]->show();
    m_pBtns[TIMEPAGE_BTN_CANCEL]->show();

    lbTitPic->hide();
    lbTitName->hide();

    ShowWidget[DATE_SET]->hide();
    ShowWidget[TIME_SET]->hide();

    m_wndMain->prepareKeyStroke();
}

void DManagerSetPage::on_timeOkBtn_clicked()
{
    if(CalS->isVisible())
    {
        QDate date  = CalS->selectedDate();
        QString tmp = date.toString("yyyy-MM-dd");
        m_pBtns[TIMEPAGE_BTN_DATE_SET]->setText(tmp);

        CalS->hide();
        m_pBtns[TIMEPAGE_BTN_OK]->hide();
        m_pBtns[TIMEPAGE_BTN_CANCEL]->hide();
        ShowWidget[DATE_SET]->show();
        ShowWidget[TIME_SET]->show();
    }
    else if(TimeHCbox->isVisible())
    {
        QString TimeStr = TimeHCbox->currentText();
        TimeStr.append(":");
        TimeStr.append(TimeMCbox->currentText());
        TimeStr.append(":");
        TimeStr.append(TimeSCbox->currentText());
        m_pBtns[TIME_SET]->setText(TimeStr);

        //
        TimeHCbox->hide();
        TimeMCbox->hide();
        TimeSCbox->hide();

        m_pBtns[TIMEPAGE_BTN_OK]->hide();
        m_pBtns[TIMEPAGE_BTN_CANCEL]->hide();

        lbTitPic->show();
        lbTitName->show();
        ShowWidget[DATE_SET]->show();
        ShowWidget[TIME_SET]->show();
    }

    changeTime();
    m_wndMain->MainWriteLoginOperationInfo2Db(SETPAGE_SYSTEM_TIME);
    m_wndMain->prepareKeyStroke();
}

void DManagerSetPage::on_timeCancelBtn_clicked()
{
    if(CalS->isVisible())
    {
        CalS->hide();
        m_pBtns[TIMEPAGE_BTN_OK]->hide();
        m_pBtns[TIMEPAGE_BTN_CANCEL]->hide();
        ShowWidget[DATE_SET]->show();
        ShowWidget[TIME_SET]->show();
    }
    else if(TimeHCbox->isVisible())
    {
        TimeHCbox->hide();
        TimeMCbox->hide();
        TimeSCbox->hide();

        m_pBtns[TIMEPAGE_BTN_OK]->hide();
        m_pBtns[TIMEPAGE_BTN_CANCEL]->hide();

        lbTitPic->show();
        lbTitName->show();
        ShowWidget[DATE_SET]->show();
        ShowWidget[TIME_SET]->show();
    }
    m_wndMain->prepareKeyStroke();
}

/**
 * @Author: dcj
 * @Date: 2021-06-21 13:19:37
 * @Description: 保存校准数据
 */
void DManagerSetPage::on_caliSaveBtn_clicked()
{
    QMap<int, DISP_PARAM_CALI_ITEM_STRU> cMap;
    DISP_PARAM_CALI_ITEM_STRU values;

    float fTemp = m_pCaliS1LineEdit->text().toFloat();
    values.fk = fTemp;
    values.fc = 1;
    values.fv = 1;

    cMap.insert(m_caliId, values);

    MainSaveCalibrateParam(gGlobalParam.iMachineType, cMap);

#ifdef STEPPERMOTOR
    // save stepper cali parameter
    gCaliParam.stepperCali.iStart = m_pStepperSlider->value() + STEPPER_REFERENCD_POINT;
    MainSaveStepperCaliParam(gGlobalParam.iMachineType);
#endif

    MainUpdateGlobalParam();

    m_wndMain->MainWriteLoginOperationInfo2Db(SETPAGE_SYSTEM_PARAMETER_CALIBRATE);

    m_wndMain->prepareKeyStroke();
    DHintDialog::getInstance(tr("Successfully saved"));
}

/**
 * @Author: dcj
 * @Date: 2021-06-21 13:20:05
 * @Description: 保存声音设置
 * @param {int} stateL: unused
 */
void DManagerSetPage::on_checkBox_changeState(int state)
{
    Q_UNUSED(state);
    for(int iLoop = 0 ; iLoop < DISPLAY_SOUND_NUM ; iLoop++)
    {
       if ((Qt::Checked == m_chkSwitchs[iLoop]->checkState()))
       {
           m_iSoundMask |= 1 << iLoop;
       }
       else
       {
           m_iSoundMask &= ~(1 << iLoop);
       }
    }

    if (m_iSoundMask != gGlobalParam.MiscParam.iSoundMask)
    {
       DISP_MISC_SETTING_STRU  MiscParam = gGlobalParam.MiscParam;
       MiscParam.iSoundMask = m_iSoundMask;
       MainSaveMiscParam(gGlobalParam.iMachineType,MiscParam);
       MainUpdateSpecificParam(NOT_PARAM_MISC_PARAM);

       m_wndMain->MainWriteLoginOperationInfo2Db(SETPAGE_SYSTEM_SOUND);
    }
}

/**
 * @Author: dcj
 * @Date: 2021-06-21 13:21:38
 * @Description: 保存显示设置
 */
void DManagerSetPage::on_LcdSaveBtn_clicked()
{
    DISP_MISC_SETTING_STRU  MiscParam = gGlobalParam.MiscParam;

    MiscParam.iBrightness = m_iBrightness;
    MiscParam.iEnerySave  = m_iEnergySave;
    MainSaveMiscParam(gGlobalParam.iMachineType,MiscParam);

    gAdditionalCfgParam.additionalParam.iScreenSleepTime = m_iSleepTime;
    MainSaveExConfigParam(gGlobalParam.iMachineType);
    MainRetriveExConfigParam(gGlobalParam.iMachineType);

    MainUpdateSpecificParam(NOT_PARAM_MISC_PARAM);

    m_wndMain->MainWriteLoginOperationInfo2Db(SETPAGE_SYSTEM_DISPLAY);

    m_wndMain->prepareKeyStroke();
    DHintDialog::getInstance(tr("Successfully saved"));
}

/**
 * @Author: dcj
 * @Date: 2021-06-21 13:23:11
 * @Description: 显示节能模式调整（不保存，只当前有效）
 * @param {int} state
 */
void DManagerSetPage::on_CheckEnergySave_stateChanged(int state)
{
    if(Qt::Checked == state)
    {
        m_iEnergySave = 1;
        m_pLcdBackWidget[2]->show();
    }
    else
    {
        m_iEnergySave = 0;
        m_pLcdBackWidget[2]->hide();
    }
    m_wndMain->prepareKeyStroke();
}

/**
 * @Author: dcj
 * @Date: 2021-06-21 13:24:14
 * @Description: 休眠时间调节
 * @param {int} index
 */
void DManagerSetPage::on_comboBox_currentIndexChanged(int index)
{
    m_iSleepTime = index + 1;
}

void DManagerSetPage::setValue(int value)
{
    if(0 == gAdditionalCfgParam.productInfo.iCompany)
    {
        if (value < 50)
        {
            return;
        }
    }
    else
    {
        if (value < 20)
        {
            return;
        }
    }

    m_iBrightness = value;

    Write_sys_int(PWMLCD_FILE,m_iBrightness);
}

/**
 * @Author: dcj
 * @Date: 2021-06-21 13:26:27
 * @Description: 保存终端过滤器配置
 */
void DManagerSetPage::on_FilterSaveBtn_clicked()
{
    DISP_MISC_SETTING_STRU        miscParam = gGlobalParam.MiscParam;

    if(Qt::Checked ==m_pFilterCheck[0]->checkState())
    {
        miscParam.ulMisFlags |= 1 << DISP_SM_FINALFILTER_A;
    }
    else
    {
        miscParam.ulMisFlags &= ~(1 << DISP_SM_FINALFILTER_A);
    }

    if(Qt::Checked ==m_pFilterCheck[1]->checkState())
    {
        miscParam.ulMisFlags |= 1 << DISP_SM_FINALFILTER_B;
    }
    else
    {
        miscParam.ulMisFlags &= ~(1 << DISP_SM_FINALFILTER_B);
    }
    MainSaveMiscParam(gGlobalParam.iMachineType,miscParam);
    MainUpdateGlobalParam();

    m_wndMain->prepareKeyStroke();
    DHintDialog::getInstance(tr("Successfully saved"));
}

void DManagerSetPage::onWifiCheckBoxChangeState(int state)
{
    Q_UNUSED(state);

    if ((Qt::Checked == m_pWifiCheckBox->checkState()))
    {
        m_iNetworkMask |= 1 << DISPLAY_NETWORK_WIFI;
    }
    else
    {
        m_iNetworkMask &= ~(1 << DISPLAY_NETWORK_WIFI);
    }

    emit wifiEnabled(Qt::Checked == m_pWifiCheckBox->checkState());
}

/**
 * @Author: dcj
 * @Date: 2021-06-21 13:28:57
 * @Description: 保存网络配置
 */
void DManagerSetPage::on_NetworkSaveBtn_clicked()
{
    if (m_iNetworkMask != gGlobalParam.MiscParam.iNetworkMask)
    {
        DISP_MISC_SETTING_STRU MiscParam = gGlobalParam.MiscParam;
        MiscParam.iNetworkMask = m_iNetworkMask;
        MainSaveMiscParam(gGlobalParam.iMachineType, MiscParam);
        MainUpdateSpecificParam(NOT_PARAM_MISC_PARAM);
    }
    m_wndMain->MainWriteLoginOperationInfo2Db(SETPAGE_SYSTEM_NETWORK);
    m_wndMain->prepareKeyStroke();
}

/**
 * @Author: dcj
 * @Date: 2021-06-21 13:29:22
 * @Description: 保存HP循环配置
 * @param {int} state ： unused
 */
void DManagerSetPage::on_HPCircheckBox_changeState(int state)
{
    Q_UNUSED(state);
    DISP_MISC_SETTING_STRU        miscParam = gGlobalParam.MiscParam;

    if(gAdditionalCfgParam.machineInfo.iMachineFlow < 500)
    {
        if(Qt::Checked == m_pAdditionalCheck[HPCIR_SETTING]->checkState())
        {
            miscParam.ulMisFlags |= 1 << DISP_SM_HP_Water_Cir;
        }
        else
        {
            miscParam.ulMisFlags &= ~(1 << DISP_SM_HP_Water_Cir);
        }
    }

    MainSaveMiscParam(gGlobalParam.iMachineType,miscParam);
    MainUpdateGlobalParam();
}

/**
 * @Author: dcj
 * @Date: 2021-06-21 13:30:04
 * @Description: 保存RephiLink配置
 */
void DManagerSetPage::on_RephiLinkcheckBox_clicked()
{
    if(Qt::Checked == m_pAdditionalCheck[REPHILINK_SETTING]->checkState())
    {
        DRephiLinkProtocolDlg dlg;
        if(QDialog::Accepted != dlg.exec())
        {
            m_pAdditionalCheck[REPHILINK_SETTING]->setCheckState(Qt::Unchecked);
        }
    }

    DISP_MISC_SETTING_STRU        miscParam = gGlobalParam.MiscParam;
    if(Qt::Checked == m_pAdditionalCheck[REPHILINK_SETTING]->checkState())
    {
        miscParam.ulMisFlags |= 1 << DISP_SM_REPHILINK;
    }
    else
    {
        miscParam.ulMisFlags &= ~(1 << DISP_SM_REPHILINK);
    }

    MainSaveMiscParam(gGlobalParam.iMachineType,miscParam);
    MainUpdateGlobalParam();
}

void DManagerSetPage::on_CompensationcheckBox_changeState(int state)
{
    Q_UNUSED(state);
    DISP_SUB_MODULE_SETTING_STRU  smParam = gGlobalParam.SubModSetting;
    if(Qt::Checked == m_pAdditionalCheck[COMPENSATION_SETTING]->checkState())
    {
        smParam.ulAddFlags |= 1 << DISP_SM_COMPENSATION;
    }
    else
    {
        smParam.ulAddFlags &= ~(1 << DISP_SM_COMPENSATION);
    }
    
    MainSaveSubModuleSetting(gGlobalParam.iMachineType,smParam);
    MainUpdateGlobalParam();
}

#ifdef STEPPERMOTOR
void DManagerSetPage::onStepperSlider_valueChanged(int value)
{
    m_pStepperValueLB->setText(QString::number(value));
}
#endif

void DManagerSetPage::initTimePage()
{
    m_pageWidget[MANAGER_PAGE_TIME] = new QWidget;

    //createHeads
    QPixmap PixTit;
    PixTit.load(":/pic/time.png");

    lbTitPic = new QLabel(m_pageWidget[MANAGER_PAGE_TIME]);
    lbTitPic->setGeometry(QRect(150, 100 , 28 , 28));
    lbTitPic->setPixmap(PixTit);

    lbTitName = new QLabel(m_pageWidget[MANAGER_PAGE_TIME]);
    lbTitName->setGeometry(QRect(210, 105 , 200 , 28));
    lbTitName->setStyleSheet(" font-size:24pt;color:#000000;font-family:Arial;QFont::Bold");

    //Create_Cal_Ok
    m_pBtns[TIMEPAGE_BTN_CANCEL] = new QPushButton(m_pageWidget[MANAGER_PAGE_TIME]);
    m_pBtns[TIMEPAGE_BTN_CANCEL]->setGeometry(QRect(230, 400, 76, 42));
    m_pBtns[TIMEPAGE_BTN_CANCEL]->hide();

    connect(m_pBtns[TIMEPAGE_BTN_CANCEL], SIGNAL(clicked()), this, SLOT(on_timeCancelBtn_clicked()));

    m_pBtns[TIMEPAGE_BTN_OK]     = new QPushButton(m_pageWidget[MANAGER_PAGE_TIME]);
    m_pBtns[TIMEPAGE_BTN_OK]->setGeometry(QRect(430, 400, 76, 42));
    m_pBtns[TIMEPAGE_BTN_OK]->hide();

    connect(m_pBtns[TIMEPAGE_BTN_OK], SIGNAL(clicked()), this, SLOT(on_timeOkBtn_clicked()));
    //Create_Date_Time
    int iLoop;
    int tmp;

    for(iLoop = 0 ; iLoop < DATE_NUM ; iLoop++)
    {
        ShowWidget[iLoop] = new QWidget(m_pageWidget[MANAGER_PAGE_TIME]);

        QPalette pal(ShowWidget[iLoop]->palette());

        pal.setColor(QPalette::Background, Qt::white);

        ShowWidget[iLoop]->setAutoFillBackground(true);
        ShowWidget[iLoop]->setPalette(pal);

        ShowWidget[iLoop]->setGeometry(QRect(134 , 160 + 90 * iLoop , 530 ,60));

        lbName[iLoop] = new QLabel(ShowWidget[iLoop]);
        lbName[iLoop]->setGeometry(QRect(25, 20 , 120 , 20));
        lbName[iLoop]->setAlignment(Qt::AlignLeft);
        m_pBtns[iLoop] = new QPushButton(ShowWidget[iLoop]);
        m_pBtns[iLoop]->setGeometry(QRect(300 ,20 ,200,20));
        m_pBtns[iLoop]->setStyleSheet("background-color:transparent");

        if(DATE_SET == iLoop)
        {
            connect(m_pBtns[iLoop], SIGNAL(clicked()), this, SLOT(on_timeDateSet_clicked()));
            CalS = new QCalendarWidget(m_pageWidget[MANAGER_PAGE_TIME]);

            switch(gGlobalParam.MiscParam.iLan)
            {
            case APP_LAN_CHN:
                CalS->setLocale(QLocale::Chinese);
                break;
            case APP_LAN_SPA:
                CalS->setLocale(QLocale::Spanish);
                break;
            case APP_LAN_FRE:
                CalS->setLocale(QLocale::French);
                break;
            case APP_LAN_ITA:
                CalS->setLocale(QLocale::Italian);
                break;
            case APP_LAN_SKR:
                CalS->setLocale(QLocale::Korean);
                break;
            case APP_LAN_GER:
                CalS->setLocale(QLocale::German);
                break;
            case APP_LAN_RUS:
                CalS->setLocale(QLocale::Russian);
                break;
            case APP_LAN_POR:
                CalS->setLocale(QLocale::Portuguese);
                break;
            default:
                CalS->setLocale(QLocale::English);
                break;
            }

            CalS->setGeometry(QRect(130,70,500,300));
            CalS->hide();

        }
        else if(TIME_SET == iLoop)
        {
            connect(m_pBtns[iLoop], SIGNAL(clicked()), this, SLOT(on_timeTimeSet_clicked()));

            TimeHCbox = new QComboBox(m_pageWidget[MANAGER_PAGE_TIME]);
            TimeHCbox->setGeometry(230,200,80,30);
            TimeHCbox->hide();

            for(tmp = 0 ; tmp < 24 ; tmp++ )
            {
                TimeHCbox->addItem(QString::number(tmp,10));
            }

            TimeMCbox = new QComboBox(m_pageWidget[MANAGER_PAGE_TIME]);
            TimeMCbox->setGeometry(330,200,80,30);
            TimeMCbox->hide();

            for(tmp = 0 ; tmp < 60 ; tmp++ )
            {
                TimeMCbox->addItem(QString::number(tmp,10));
            }

            TimeSCbox = new QComboBox(m_pageWidget[MANAGER_PAGE_TIME]);
            TimeSCbox->setGeometry(430,200,80,30);
            TimeSCbox->hide();

            for(tmp = 0 ; tmp < 60 ; tmp++ )
            {
                TimeSCbox->addItem(QString::number(tmp,10));
            }
        }
    }
    QIcon icon1(":/pic/unselected.png");
    m_tabWidget->addTab(m_pageWidget[MANAGER_PAGE_TIME], icon1, tr("Time & Date"));
}

void DManagerSetPage::initCalibrationPage()
{
    m_caliId = DISP_PC_COFF_S1;
    setBackColor();
    m_pageWidget[MANAGER_PAGE_CALIBRATION] = new QWidget;

    QFile qss(":/app/slider.qss");
    qss.open(QFile::ReadOnly);
    QString qsss = QLatin1String (qss.readAll());
    qss.close();

    QPixmap back(":/pic/SubPageBack.png");
    QSize size(back.width(), back.height());
    QImage image_bg = QImage(size, QImage::Format_ARGB32);
    QPainter p(&image_bg);
    p.fillRect(image_bg.rect(), QColor(255, 255, 255));

    m_pCaliS1Widget = new QWidget(m_pageWidget[MANAGER_PAGE_CALIBRATION]);

    QPalette pal(m_pCaliS1Widget->palette());
    pal.setColor(QPalette::Background, Qt::white);

    m_pCaliS1Widget->setAutoFillBackground(true);
    m_pCaliS1Widget->setPalette(pal);

    m_pCaliS1Widget->setGeometry(QRect(120 , 150 , 530 ,60));

    m_pCaliS1Label = new QLabel(m_pCaliS1Widget);
    m_pCaliS1Label->setGeometry(10, 10, 220, 40);
    m_pCaliS1Label->setStyleSheet(" font-size:18px;color:#16181e;font-family:Arial;QFont::Bold");
    m_pCaliS1Label->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);

    m_pCaliS1LineEdit = new DLineEdit(m_pCaliS1Widget);
    m_pCaliS1LineEdit->setGeometry(290, 8, 120, 40);

    QDoubleValidator *doubleValidator = new QDoubleValidator(0.0, 99.9, 3, m_pCaliS1Widget);
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);
    m_pCaliS1LineEdit->setValidator(doubleValidator);

#ifdef STEPPERMOTOR
    m_pStepperWidget = new QWidget(m_pageWidget[MANAGER_PAGE_CALIBRATION]);

    QPalette pal2(m_pStepperWidget->palette());
    pal2.setColor(QPalette::Background, Qt::white);
    m_pStepperWidget->setAutoFillBackground(true);
    m_pStepperWidget->setPalette(pal2);
    m_pStepperWidget->setGeometry(QRect(120, 250, 530, 60));

    m_pStepperLabel = new QLabel(m_pStepperWidget);
    m_pStepperLabel->setGeometry(10, 10, 220, 40);
    m_pStepperLabel->setStyleSheet(" font-size:18px;color:#16181e;font-family:Arial;QFont::Bold");
    m_pStepperLabel->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);

    m_pStepperSlider = new QSlider(m_pStepperWidget);
    m_pStepperSlider->setOrientation(Qt::Horizontal);
    m_pStepperSlider->setStyleSheet(qsss);
    m_pStepperSlider->setGeometry(QRect(140 , 10 ,350, 40));
    m_pStepperSlider->setRange(-100, 100);
    m_pStepperSlider->setSingleStep(5);
    m_pStepperSlider->setTickPosition(QSlider::TicksAbove); 

    m_pStepperValueLB = new QLabel(m_pStepperWidget);
    m_pStepperValueLB ->setGeometry(495, 12, 60, 40);
    m_pStepperValueLB->setStyleSheet("font-size:14px;color:#0095F4;font-family:Arial;QFont::Bold");
    m_pStepperValueLB->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);

    connect(m_pStepperSlider, SIGNAL(valueChanged(int)), this, SLOT(onStepperSlider_valueChanged(int)));
    if(!(gGlobalParam.SubModSetting.ulAddFlags & (1 << DISP_SM_STEPPERMOTOR)))
    {
        m_pStepperWidget->hide();
    }
    
#endif

    m_pCaliBtn = new QPushButton(m_pageWidget[MANAGER_PAGE_CALIBRATION]);
    m_pCaliBtn->move(580, 420);
    connect(m_pCaliBtn, SIGNAL(clicked()), this, SLOT(on_caliSaveBtn_clicked()));

    QIcon icon1(":/pic/unselected.png");
    m_tabWidget->addTab(m_pageWidget[MANAGER_PAGE_CALIBRATION], icon1, tr("Calibration"));
}

void DManagerSetPage::initLcdPage()
{
    m_pageWidget[MANAGER_PAGE_LCD] = new QWidget;

    int iLoop = 0;

    QFile qss(":/app/slider.qss");
    qss.open(QFile::ReadOnly);
    QString qsss = QLatin1String (qss.readAll());
    qss.close();

    m_iEnergySave = gGlobalParam.MiscParam.iEnerySave;
    m_iBrightness = gGlobalParam.MiscParam.iBrightness;
    m_iSleepTime = gAdditionalCfgParam.additionalParam.iScreenSleepTime;

    for( iLoop = 0 ; iLoop < 3 ; iLoop++)
    {
        m_pLcdBackWidget[iLoop] = new QWidget(m_pageWidget[MANAGER_PAGE_LCD]);
        QPalette pal(m_pLcdBackWidget[iLoop]->palette());
        pal.setColor(QPalette::Background, Qt::white);
        m_pLcdBackWidget[iLoop]->setAutoFillBackground(true);
        m_pLcdBackWidget[iLoop]->setPalette(pal);

        m_pLcdBackWidget[iLoop]->setGeometry(QRect(120 , 100 + 70 * iLoop , 530 ,60));

        laName[iLoop]      = new QLabel(m_pLcdBackWidget[iLoop]);
        laName[iLoop]->setPixmap(NULL);
        laName[iLoop]->setGeometry(QRect(20, 15 , 150 , 20));
        laName[iLoop]->setAlignment(Qt::AlignLeft);
        laName[iLoop]->show();
    }

    pSlider[0] = new QSlider(m_pLcdBackWidget[0]);
    pSlider[0]->setOrientation(Qt::Horizontal);
    pSlider[0]->setStyleSheet(qsss);
    pSlider[0]->setGeometry(QRect(140 , 10 ,370,40));
    pSlider[0]->setMinimum(0);

    if(0 == gAdditionalCfgParam.productInfo.iCompany)
    {
        pSlider[0]->setMaximum(255);
    }
    else
    {
        pSlider[0]->setMaximum(100);
    }

    pSlider[0]->setValue(m_iBrightness);

    connect(pSlider[0], SIGNAL(valueChanged(int)), this, SLOT(setValue(int)));

    m_pCheckEnergySave = new QCheckBox(m_pLcdBackWidget[1]);
    QString strQss4Chk = m_wndMain->getQss4Chk();
    m_pCheckEnergySave->setStyleSheet(strQss4Chk);

    m_pCheckEnergySave->setGeometry(QRect(450, 15, 120, 30));

    connect(m_pCheckEnergySave, SIGNAL(stateChanged(int)), this, SLOT(on_CheckEnergySave_stateChanged(int)));

    //ex
    m_sleepLabel = new QLabel(m_pLcdBackWidget[2]);
    m_comboBox = new QComboBox(m_pLcdBackWidget[2]);
    for(iLoop = 1; iLoop < 61; iLoop++)
    {
        m_comboBox->addItem(QString("%1").arg(iLoop));
    }
    m_comboBox->setCurrentIndex(m_iSleepTime - 1);
    connect(m_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_comboBox_currentIndexChanged(int)));

    m_sleepLabel->setGeometry(QRect(25, 15, 140 , 20));
    m_comboBox->setGeometry(QRect(200 , 10, 80, 30));
    if(m_iEnergySave)
    {
        m_pLcdBackWidget[2]->show();
    }
    else
    {
        m_pLcdBackWidget[2]->hide();
    }

    //end

    m_pLcdBtnSave = new QPushButton(m_pageWidget[MANAGER_PAGE_LCD]);
    m_pLcdBtnSave->move(580,450);

    connect(m_pLcdBtnSave, SIGNAL(clicked()), this, SLOT(on_LcdSaveBtn_clicked()));

    QIcon icon1(":/pic/unselected.png");
    m_tabWidget->addTab(m_pageWidget[MANAGER_PAGE_LCD], icon1, tr("LCD"));
}

void DManagerSetPage::initFinalFilterPage()
{
    m_pageWidget[MANAGER_PAGE_FINALFILTER] = new QWidget;

    for(int i = 0; i < 2; i++)
    {
        m_pFilterBackWidget[i] = new QWidget(m_pageWidget[MANAGER_PAGE_FINALFILTER]);

        QPalette pal(m_pFilterBackWidget[i]->palette());
        pal.setColor(QPalette::Background, Qt::white);
        m_pFilterBackWidget[i]->setAutoFillBackground(true);
        m_pFilterBackWidget[i]->setPalette(pal);
        m_pFilterBackWidget[i]->setGeometry(QRect(120 , 120 + 70 * i , 530 ,60));

        m_pFilterLabel[i] = new QLabel(m_pFilterBackWidget[i]);
        m_pFilterLabel[i]->setPixmap(NULL);
        m_pFilterLabel[i]->setGeometry(QRect(25, 25 , 260 , 20));
        m_pFilterLabel[i]->setStyleSheet(" font-size:18px;color:#16181e;font-family:Arial;QFont::Bold");
        m_pFilterLabel[i]->show();
        m_pFilterLabel[i]->setAlignment(Qt::AlignLeft);

        m_pFilterCheck[i] = new QCheckBox(m_pFilterBackWidget[i]);
        m_pFilterCheck[i]->setGeometry(QRect(480 , 9 ,40,40));

        QString strQss4Chk = m_wndMain->getQss4Chk();
        m_pFilterCheck[i]->setStyleSheet(strQss4Chk);
    }

    m_pFilterSaveBtn = new QPushButton(m_pageWidget[MANAGER_PAGE_FINALFILTER]);
    m_pFilterSaveBtn->move(580, 420);
    connect(m_pFilterSaveBtn, SIGNAL(clicked()), this, SLOT(on_FilterSaveBtn_clicked()));

    QIcon icon1(":/pic/unselected.png");
    m_tabWidget->addTab(m_pageWidget[MANAGER_PAGE_FINALFILTER], icon1, tr("Final Filter"));
}

void DManagerSetPage::initNetworkPage()
{
    m_iNetworkMask = gGlobalParam.MiscParam.iNetworkMask;
    m_pageWidget[MANAGER_PAGE_NETWORK] = new QWidget;

    m_pWifiBackWidget = new QWidget(m_pageWidget[MANAGER_PAGE_NETWORK]);

    QPalette pal(m_pWifiBackWidget->palette());
    pal.setColor(QPalette::Background, Qt::white);
    m_pWifiBackWidget->setAutoFillBackground(true);
    m_pWifiBackWidget->setPalette(pal);
    m_pWifiBackWidget->setGeometry(QRect(120, 40, 530, 50));

    m_pWifiLabel = new QLabel(m_pWifiBackWidget);
    m_pWifiLabel->setGeometry(QRect(25, 15, 260 , 20));
    m_pWifiLabel->setStyleSheet(" font-size:18px;color:#16181e;font-family:Arial;QFont::Bold");
    m_pWifiLabel->show();
    m_pWifiLabel->setAlignment(Qt::AlignLeft);

    m_pWifiCheckBox = new QCheckBox(m_pWifiBackWidget);
    m_pWifiCheckBox->setGeometry(QRect(480, 5, 40, 40));
    if (m_iNetworkMask & (1 << DISPLAY_NETWORK_WIFI))
    {
        m_pWifiCheckBox->setChecked(true);
    }
    else
    {
        m_pWifiCheckBox->setChecked(false);
    }
    connect(m_pWifiCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onWifiCheckBoxChangeState(int)));

    QString strQss4Chk = m_wndMain->getQss4Chk();
    m_pWifiCheckBox->setStyleSheet(strQss4Chk);

    m_pNetworkWidget = new DNetworkWidget(m_pageWidget[MANAGER_PAGE_NETWORK]);
    m_pNetworkWidget->move(120, 100);
    connect(this, SIGNAL(wifiEnabled(bool)), m_pNetworkWidget, SLOT(setWifiEnabled(bool)));

    m_pNetworkSaveBtn = new QPushButton(m_pageWidget[MANAGER_PAGE_NETWORK]);
    m_pNetworkSaveBtn->move(580, 420);
    connect(m_pNetworkSaveBtn, SIGNAL(clicked()), this, SLOT(on_NetworkSaveBtn_clicked()));

    QIcon icon1(":/pic/unselected.png");
    m_tabWidget->addTab( m_pageWidget[MANAGER_PAGE_NETWORK], icon1, tr("Network"));

    emit wifiEnabled(Qt::Checked == m_pWifiCheckBox->checkState());
}

void DManagerSetPage::initAdditionalSettingsPage()
{
    setBackColor();
    m_pageWidget[MANAGER_PAGE_ADDSETTINGS] = new QWidget;
    m_pageWidget[MANAGER_PAGE_ADDSETTINGS]->setMinimumWidth(740);
    QScrollArea *scrollWidget = new QScrollArea(m_widget);

    QVBoxLayout *vLayout = new QVBoxLayout;
    QMargins vLayoutMargins = vLayout->contentsMargins();
    vLayoutMargins.setTop(20);   //设置VBoxLayout 上边界为20
    vLayout->setContentsMargins(vLayoutMargins);

    QPixmap back(":/pic/SubPageBack.png");
    QSize size(back.width(), back.height());
    QImage image_bg = QImage(size, QImage::Format_ARGB32);
    QPainter p(&image_bg);
    p.fillRect(image_bg.rect(), QColor(255, 255, 255));
    
    QString strQss4Chk = m_wndMain->getQss4Chk();

    //声音设置选项，单独构建
    m_iSoundMask = gGlobalParam.MiscParam.iSoundMask;
    for(int iLoop = 0 ; iLoop < DISPLAY_SOUND_NUM ; iLoop++)
    {
        m_pAudioBackWidget[iLoop] = new QWidget;

        QPalette pal(m_pAudioBackWidget[iLoop]->palette());
        pal.setColor(QPalette::Background, Qt::white);
        m_pAudioBackWidget[iLoop]->setAutoFillBackground(true);
        m_pAudioBackWidget[iLoop]->setPalette(pal);

        m_pAudioBackWidget[iLoop]->setMinimumSize(530, 60);

        m_lblNames[iLoop] = new QLabel(m_pAudioBackWidget[iLoop]);
        m_lblNames[iLoop]->setPixmap(NULL);
        m_lblNames[iLoop]->setGeometry(QRect(25, 25 , 140 , 20));
        m_lblNames[iLoop]->setStyleSheet(" font-size:18px;color:#16181e;font-family:Arial;QFont::Bold");
        m_lblNames[iLoop]->setAlignment(Qt::AlignLeft);

        m_chkSwitchs[iLoop] = new QCheckBox(m_pAudioBackWidget[iLoop]);
        m_chkSwitchs[iLoop]->setGeometry(QRect(480 , 9 ,40,40));
        m_chkSwitchs[iLoop]->setStyleSheet(strQss4Chk);

        if (m_iSoundMask & (1 << iLoop))
        {
            m_chkSwitchs[iLoop]->setChecked(true);
        }
        else
        {
            m_chkSwitchs[iLoop]->setChecked(false);
        }

        connect(m_chkSwitchs[iLoop], SIGNAL(stateChanged(int)), this, SLOT(on_checkBox_changeState(int)));

        vLayout->addWidget(m_pAudioBackWidget[iLoop]);
    }

    //其它配置项，若新增配置项，则在此处增加
    for(int iLoop = 0 ; iLoop < ADDITIONAL_NUM ; iLoop++)
    {
        m_pAdditionalWidget[iLoop] = new QWidget;

        QPalette pal(m_pAdditionalWidget[iLoop]->palette());
        pal.setColor(QPalette::Background, Qt::white);
        m_pAdditionalWidget[iLoop]->setAutoFillBackground(true);
        m_pAdditionalWidget[iLoop]->setPalette(pal);
        m_pAdditionalWidget[iLoop]->setMinimumSize(530, 60);

        m_pAdditionalLb[iLoop] = new QLabel(m_pAdditionalWidget[iLoop]);
        m_pAdditionalLb[iLoop]->setPixmap(NULL);
        m_pAdditionalLb[iLoop]->setGeometry(QRect(25, 25 , 260 , 20));
        m_pAdditionalLb[iLoop]->setStyleSheet(" font-size:18px;color:#16181e;font-family:Arial;QFont::Bold");
        m_pAdditionalLb[iLoop]->show();
        m_pAdditionalLb[iLoop]->setAlignment(Qt::AlignLeft);

        m_pAdditionalCheck[iLoop] = new QCheckBox(m_pAdditionalWidget[iLoop]);
        m_pAdditionalCheck[iLoop]->setGeometry(QRect(480 , 9 ,40,40));
        m_pAdditionalCheck[iLoop]->setStyleSheet(strQss4Chk);

        vLayout->addWidget(m_pAdditionalWidget[iLoop]);
    }

    connect(m_pAdditionalCheck[HPCIR_SETTING], SIGNAL(stateChanged(int)), this, SLOT(on_HPCircheckBox_changeState(int)));
    connect(m_pAdditionalCheck[REPHILINK_SETTING], SIGNAL(clicked()), this, SLOT(on_RephiLinkcheckBox_clicked()));
    connect(m_pAdditionalCheck[COMPENSATION_SETTING], SIGNAL(stateChanged(int)), this, SLOT(on_CompensationcheckBox_changeState(int)));
    
    QFile qss(":/app/other.qss");
    qss.open(QFile::ReadOnly);
    QString strQss = QLatin1String (qss.readAll());
    qss.close(); 

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addStretch(1);
    hLayout->addLayout(vLayout);
    hLayout->addStretch(1);
    m_pageWidget[MANAGER_PAGE_ADDSETTINGS]->setLayout(hLayout);
    m_pageWidget[MANAGER_PAGE_ADDSETTINGS]->setStyleSheet(strQss);
    scrollWidget->setWidget(m_pageWidget[MANAGER_PAGE_ADDSETTINGS]);
    scrollWidget->setGeometry(QRect(5, 60, 780, 490));
    scrollWidget->setStyleSheet("QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{background: transparent;}");

    if(0 != gAdditionalCfgParam.productInfo.iCompany)
    {
        m_pAdditionalWidget[REPHILINK_SETTING]->hide();
    }

    if(gAdditionalCfgParam.machineInfo.iMachineFlow >= 500 || MACHINE_ADAPT == gGlobalParam.iMachineType)
    {
        m_pAdditionalWidget[HPCIR_SETTING]->hide();
    }

    QIcon icon1(":/pic/unselected.png");
    m_tabWidget->addTab(scrollWidget, icon1, tr("Additional Settings"));
}

void DManagerSetPage::changeTime()
{
    //QDateTime time;
    time_t    Time_t;
    struct tm curtime;
    struct rtc_time rtctime ;

    int fb;

    QStringList timeStrList = m_pBtns[TIMEPAGE_BTN_TIME_SET]->text().split(QRegExp("[:]"));

    QStringList dateStrList = m_pBtns[TIMEPAGE_BTN_DATE_SET]->text().split(QRegExp("[-]"));

    int iYear,iMon,iDay,iHour,iMin,iSec;

    iSec  = QString(timeStrList[2]).toInt();
    iMin  = QString(timeStrList[1]).toInt();
    iHour = QString(timeStrList[0]).toInt();
    iDay  = QString(dateStrList[2]).toInt();
    iMon  = QString(dateStrList[1]).toInt();
    iYear = QString(dateStrList[0]).toInt();

    curtime.tm_sec  = iSec;
    curtime.tm_min  = iMin;
    curtime.tm_hour = iHour;
    curtime.tm_mday = iDay;
    curtime.tm_mon  = iMon  - 1;
    curtime.tm_year = iYear - 1900;
    curtime.tm_isdst= -1;

    Time_t = mktime(&curtime);

    stime(&Time_t);

    struct tm *tmp_ptr = NULL;

    tmp_ptr = gmtime(&Time_t);

    rtctime.tm_sec  = tmp_ptr->tm_sec;
    rtctime.tm_min  = tmp_ptr->tm_min;
    rtctime.tm_hour = tmp_ptr->tm_hour;
    rtctime.tm_mday = tmp_ptr->tm_mday;
    rtctime.tm_mon  = tmp_ptr->tm_mon;
    rtctime.tm_year = tmp_ptr->tm_year;
    rtctime.tm_isdst=-1;

    fb = open("/dev/rtc0", QIODevice::ReadWrite);
    if (fb != -1)
    {
        int ret = ioctl(fb , RTC_SET_TIME , &rtctime);

        if (ret)
        {
            qDebug() << " ioctl : " << ret;
        }

        ::close(fb);
    }
}

