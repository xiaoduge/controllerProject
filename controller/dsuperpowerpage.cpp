#include "dsuperpowerpage.h"
#include "cbitmapbutton.h"
#include "mainwindow.h"
#include "exconfig.h"
#include <QMouseEvent>
#include <QScrollArea>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFormLayout>

#include "dlineedit.h"
#include "duserinfochecker.h"

#define ControlNum 6

#define BACKWIDGET_START_X      10
#define BACKWIDGET_START_Y      80
#define BACKWIDGET_START_HIATUS 80
#define BACKWIDGET_HEIGHT       60
#define BACKWIDGET_WIDTH        (800 - BACKWIDGET_START_X*2)
#define BACKWIDGET_ITEM_LENGTH  120
#define BACKWIDGET_ITEM_HEIGHT  30

static  QRect   sQrectAry[2] = {
        QRect(5,  BACKWIDGET_HEIGHT/2 - BACKWIDGET_ITEM_HEIGHT/2, BACKWIDGET_ITEM_LENGTH + 10, BACKWIDGET_ITEM_HEIGHT) ,
        QRect(60, 2, 110 , 18) ,
    };

DSuperPowerPage::DSuperPowerPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain)
                                   : CSubPage(parent,widget,wndMain)
{
    creatTitle();
    initUi();
    buildTranslation();
}

void DSuperPowerPage::creatTitle()
{
    CSubPage::creatTitle();
    buildTitles();
    selectTitle(0);
}

void DSuperPowerPage::buildTitles()
{
    QStringList stringList;
    stringList << tr("Super Power");
    setTitles(stringList);
}

void DSuperPowerPage::buildTranslation()
{
    m_lbDefaultState->setText(tr("Initialize"));
    m_cmbDefaultState->setItemText(0, tr("Yes"));
    m_cmbDefaultState->setItemText(1, tr("No"));

    m_lbDeviceTypeName->setText(tr("System Type"));

    m_pCompanyLabel->setText(tr("Company"));
    m_pExLabel[SYSCFGPAGE_LB_CATALOGNO]->setText(tr("Catalog No"));
    m_pExLabel[SYSCFGPAGE_LB_SERIALNO]->setText(tr("Serial No"));
    m_pExLabel[SYSCFGPAGE_LB_PRODATE]->setText(tr("Production Date"));
    m_pExLabel[SYSCFGPAGE_LB_INSTALLDATE]->setText(tr("Installation Date"));
    m_pExLabel[SYSCFGPAGE_LB_SOFTVER]->setText(tr("Software Version"));

    m_pBtnDbDel->setText(tr("Delete"));
    m_pLbDbDel->setText(tr("Delete Data"));

    m_pBtnDelCfg->setText(tr("Delete"));
    m_pLbCfgDel->setText(tr("Delete Cfg"));

    m_pBtnSave->setTip(tr("Save"),QColor(228, 231, 240),BITMAPBUTTON_TIP_CENTER);
}

void DSuperPowerPage::switchLanguage()
{
    buildTranslation();
    buildTitles();
    selectTitle(titleIndex());
}

void DSuperPowerPage::setBackColor()
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

#define X_MARGIN (5)
#define X_ITEM_WIDTH (60)
#define X_VALUE_WIDTH (60)

void DSuperPowerPage::createControl()
{
    QScrollArea *scrollWidget = new QScrollArea(m_widget);
    QWidget *mainWidget = new QWidget(m_widget);
    mainWidget->setMinimumWidth(740);
    QFormLayout *formLayout = new QFormLayout;

    m_lbDefaultState = new QLabel;
    m_cmbDefaultState = new QComboBox;
    m_cmbDefaultState->addItem(tr("Yes"));
    m_cmbDefaultState->addItem(tr("No"));
    m_cmbDefaultState->setCurrentIndex(1);
    connect(m_cmbDefaultState, SIGNAL(currentIndexChanged(int)), this, SLOT(on_CmbIndexChange_DefaultState(int)));
    formLayout->addRow(m_lbDefaultState, m_cmbDefaultState);

    m_lbDeviceTypeName = new QLabel;
    m_cmbDeviceType = new QComboBox; //系统机型
    for(int iLoop = 0;iLoop < MACHINE_NUM; iLoop++)
    {
        m_cmbDeviceType->addItem(gaMachineType[iLoop].strName);
    }
    m_cmbDeviceType->setCurrentIndex(gGlobalParam.iMachineType);
    connect(m_cmbDeviceType, SIGNAL(currentIndexChanged(int)), this, SLOT(on_CmbIndexChange_deviceType(int)));
    m_cmbDeviceFlow = new QComboBox; //系统流速
    QStringList flowList;
    flowList << "5" << "10" << "12" << "15"<< "24" << "32"
             << "30" << "50" << "60" << "125" << "150" << "250" << "300" << "500" << "600";
    m_cmbDeviceFlow->addItems(flowList);
    updateMachineFlow();
    QHBoxLayout *deviceLayout = new QHBoxLayout;
    deviceLayout->addWidget(m_cmbDeviceType);
    deviceLayout->addWidget(m_cmbDeviceFlow);
    formLayout->addRow(m_lbDeviceTypeName, deviceLayout);

    m_pCompanyLabel = new QLabel;
    m_pCompanyComboBox = new QComboBox;
    QStringList companyList;
    companyList << "Rephile" << "VWR";
    m_pCompanyComboBox->addItems(companyList);
    m_pCompanyComboBox->setCurrentIndex(0);
    formLayout->addRow(m_pCompanyLabel, m_pCompanyComboBox);
	
    m_pExLabel[SYSCFGPAGE_LB_CATALOGNO] = new QLabel;
    m_ExLineEdit[SYSCFGPAGE_LB_CATALOGNO] = new DLineEdit;
    m_pExLabel[SYSCFGPAGE_LB_CATALOGNO]->setBuddy(m_ExLineEdit[SYSCFGPAGE_LB_CATALOGNO]);
    formLayout->addRow(m_pExLabel[SYSCFGPAGE_LB_CATALOGNO], m_ExLineEdit[SYSCFGPAGE_LB_CATALOGNO]);

    m_pExLabel[SYSCFGPAGE_LB_SERIALNO] = new QLabel;
    m_ExLineEdit[SYSCFGPAGE_LB_SERIALNO] = new DLineEdit;
    m_pExLabel[SYSCFGPAGE_LB_SERIALNO]->setBuddy(m_ExLineEdit[SYSCFGPAGE_LB_SERIALNO]);
    formLayout->addRow(m_pExLabel[SYSCFGPAGE_LB_SERIALNO], m_ExLineEdit[SYSCFGPAGE_LB_SERIALNO]);

    m_pExLabel[SYSCFGPAGE_LB_PRODATE] = new QLabel;
    m_ExLineEdit[SYSCFGPAGE_LB_PRODATE] = new DLineEdit;
    m_ExLineEdit[SYSCFGPAGE_LB_PRODATE]->setInputMask("0000-00-00");
    m_pExLabel[SYSCFGPAGE_LB_PRODATE]->setBuddy(m_ExLineEdit[SYSCFGPAGE_LB_PRODATE]);
    formLayout->addRow(m_pExLabel[SYSCFGPAGE_LB_PRODATE], m_ExLineEdit[SYSCFGPAGE_LB_PRODATE]);

    m_pExLabel[SYSCFGPAGE_LB_INSTALLDATE] = new QLabel;
    m_ExLineEdit[SYSCFGPAGE_LB_INSTALLDATE] = new DLineEdit;
    m_ExLineEdit[SYSCFGPAGE_LB_INSTALLDATE]->setInputMask("0000-00-00");
    m_pExLabel[SYSCFGPAGE_LB_INSTALLDATE]->setBuddy(m_ExLineEdit[SYSCFGPAGE_LB_INSTALLDATE]);
    formLayout->addRow(m_pExLabel[SYSCFGPAGE_LB_INSTALLDATE], m_ExLineEdit[SYSCFGPAGE_LB_INSTALLDATE]);

    m_pExLabel[SYSCFGPAGE_LB_SOFTVER] = new QLabel;
    m_ExLineEdit[SYSCFGPAGE_LB_SOFTVER] = new DLineEdit;
    m_ExLineEdit[SYSCFGPAGE_LB_SOFTVER]->setReadOnly(true);
    m_pExLabel[SYSCFGPAGE_LB_SOFTVER]->setBuddy(m_ExLineEdit[SYSCFGPAGE_LB_SOFTVER]);
    formLayout->addRow(m_pExLabel[SYSCFGPAGE_LB_SOFTVER], m_ExLineEdit[SYSCFGPAGE_LB_SOFTVER]);

    //清空数据
    m_pLbDbDel = new QLabel;
    m_pCmDbDel = new QComboBox;
    QStringList cbList;
#ifdef SUB_ACCOUNT
    cbList << tr("All") << tr("Alarm") << tr("GetW") << tr("PWater") << tr("Log") << tr("Consumables") << tr("Sub-account");//  << tr("Water");
#else
    cbList << tr("All") << tr("Alarm") << tr("GetW") << tr("PWater") << tr("Log") << tr("Consumables");//  << tr("Water");
#endif
    m_pCmDbDel->addItems(cbList);
    m_pCmDbDel->setCurrentIndex(0);
    m_pBtnDbDel = new QPushButton;
    connect(m_pBtnDbDel, SIGNAL(clicked()), this, SLOT(on_btnDbDel_clicked()));
    QHBoxLayout *delDataLayout = new QHBoxLayout;
    delDataLayout->addWidget(m_pCmDbDel);
    delDataLayout->addWidget(m_pBtnDbDel);
    formLayout->addRow(m_pLbDbDel, delDataLayout);

    m_pLbCfgDel = new QLabel;
    m_pCmConfigDel = new QComboBox;
    QStringList configFileList;
    configFileList << tr("All") << tr("Consumables Info") << tr("Config Info") << tr("Cailbration Info");
    m_pCmConfigDel->addItems(configFileList);
    m_pCmConfigDel->setCurrentIndex(0);
    m_pBtnDelCfg = new QPushButton;
    connect(m_pBtnDelCfg, SIGNAL(clicked()), this, SLOT(on_btnDelCfg_clicked()));
    QHBoxLayout *delCfgLayout = new QHBoxLayout;
    delCfgLayout->addWidget(m_pCmConfigDel);
    delCfgLayout->addWidget(m_pBtnDelCfg);
    formLayout->addRow(m_pLbCfgDel, delCfgLayout);
    formLayout->setVerticalSpacing(10);
    formLayout->setHorizontalSpacing(10);
    
    QFile qss(":/app/other.qss");
    qss.open(QFile::ReadOnly);
    QString strQss = QLatin1String (qss.readAll());
    qss.close(); 

    mainWidget->setLayout(formLayout);
    mainWidget->setStyleSheet(strQss);
    scrollWidget->setWidget(mainWidget);
    scrollWidget->setGeometry(QRect(5, 60, 780, 490));
    scrollWidget->setStyleSheet("QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{background: transparent;}");

    m_pBtnSave = new CBitmapButton(m_widget,BITMAPBUTTON_STYLE_PUSH,BITMAPBUTTON_PIC_STYLE_NORMAL,SYSCFGPAGE_BTN_SAVE);
    m_pBtnSave->setButtonPicture(gpGlobalPixmaps[GLOBAL_BMP_PAGE_NAVI_NORMAL]);
    m_pBtnSave->setPressPicture(gpGlobalPixmaps[GLOBAL_BMP_PAGE_NAVI_ACTIVE]);
    m_pBtnSave->setGeometry(700,560,m_pBtnSave->getPicWidth(),m_pBtnSave->getPicHeight());
    m_pBtnSave->setStyleSheet("background-color:transparent");
    connect(m_pBtnSave, SIGNAL(clicked(int)), this, SLOT(on_btn_clicked(int)));
    m_pBtnSave->show();
}

void DSuperPowerPage::initUi()
{
    setBackColor();
    createControl();
}

void DSuperPowerPage::update()
{
    connectData();
    m_ExLineEdit[SYSCFGPAGE_LB_CATALOGNO]->setText(gAdditionalCfgParam.productInfo.strCatalogNo);
    m_ExLineEdit[SYSCFGPAGE_LB_SERIALNO]->setText(gAdditionalCfgParam.productInfo.strSerialNo);
    m_ExLineEdit[SYSCFGPAGE_LB_PRODATE]->setText(gAdditionalCfgParam.productInfo.strProductDate);
    m_ExLineEdit[SYSCFGPAGE_LB_INSTALLDATE]->setText(gAdditionalCfgParam.productInfo.strInstallDate);
    m_ExLineEdit[SYSCFGPAGE_LB_SOFTVER]->setText(gApp->applicationVersion());

    m_cmbDeviceType->setCurrentIndex(gGlobalParam.iMachineType);
    checkLoginInfo();
    updateMachineFlow();
}

void DSuperPowerPage::updateMachineFlow()
{
    QString strFlow = QString("%1").arg(gAdditionalCfgParam.machineInfo.iMachineFlow);
    for(int iLoop = 0; iLoop < m_cmbDeviceFlow->count(); ++iLoop)
    {
        if(strFlow == m_cmbDeviceFlow->itemText(iLoop))
        {
            m_cmbDeviceFlow->setCurrentIndex(iLoop);
            break;
        }
    }
}

void DSuperPowerPage::connectData()
{
    m_cmbDefaultState->setCurrentIndex(gAdditionalCfgParam.initMachine);
    m_pCompanyComboBox->setCurrentIndex(gAdditionalCfgParam.productInfo.iCompany);
}

void DSuperPowerPage::checkLoginInfo()
{
    DUserInfoChecker userInfo;
    DUserInfo userlog = m_wndMain->getLoginfo();
    bool iSuper = userInfo.checkSuperInfo(userlog.m_strUserName);
    if(userInfo.checkSuperInfo(userlog.m_strUserName))
    {
        m_pCompanyLabel->show();
        m_pCompanyComboBox->show();
        m_pLbDbDel->show();
        m_pCmDbDel->show();
        m_pBtnDbDel->show();
        m_pLbCfgDel->show();
        m_pCmConfigDel->show();
        m_pBtnDelCfg->show();
    }
    else
    {
        
        m_pCompanyLabel->hide();
        m_pCompanyComboBox->hide();
        m_pLbDbDel->hide();
        m_pCmDbDel->hide();
        m_pBtnDbDel->hide();
        m_pLbCfgDel->hide();
        m_pCmConfigDel->hide();
        m_pBtnDelCfg->hide();
    }

    if(userInfo.checkSuperService(userlog.m_strUserName))
    {
        m_pExLabel[SYSCFGPAGE_LB_CATALOGNO]->show();
        m_pExLabel[SYSCFGPAGE_LB_PRODATE]->show();
        m_pExLabel[SYSCFGPAGE_LB_INSTALLDATE]->show();
        m_ExLineEdit[SYSCFGPAGE_LB_CATALOGNO]->show(); 
        m_ExLineEdit[SYSCFGPAGE_LB_PRODATE]->show();
        m_ExLineEdit[SYSCFGPAGE_LB_INSTALLDATE]->show();
    }
    else
    {
        m_pExLabel[SYSCFGPAGE_LB_CATALOGNO]->hide();
        m_pExLabel[SYSCFGPAGE_LB_PRODATE]->hide();
        m_pExLabel[SYSCFGPAGE_LB_INSTALLDATE]->hide();
        m_ExLineEdit[SYSCFGPAGE_LB_CATALOGNO]->hide(); 
        m_ExLineEdit[SYSCFGPAGE_LB_PRODATE]->hide(); 
        m_ExLineEdit[SYSCFGPAGE_LB_INSTALLDATE]->hide(); 
    }
}

bool DSuperPowerPage::deleteDbAll()
{
    //water
    bool ret;
#if 0
    ret = deleteDbWater();
    if(!ret)
    {
        return false;
    }
#endif
    //alarm
    ret = deleteDbAlarm();
    if(!ret)
    {
        return false;
    }
    //getWater
    ret = deleteDbGetWater();
    if(!ret)
    {
        return false;
    }
    //product water
    ret = deleteDbPWater();
    if(!ret)
    {
        return false;
    }
    //log
    ret = deleteDbLog();
    if(!ret)
    {
        return false;
    }
    //consumables
    ret = deleteDbConsumables();
    if(!ret)
    {
        return false;
    }

    ret = deleteDbSubAccount();
    if(!ret)
    {
        return false;
    }

    return true;
}

bool DSuperPowerPage::deleteDbWater()
{
    QString strSql = "Drop Table Water";
    QSqlQuery query;
    bool ret = query.exec(strSql);
    if(!ret)
    {
        QMessageBox::warning(m_widget, tr("Water"), tr("Deleting table failed: Water"), QMessageBox::Ok);
        return false;
    }

    return true;
}
bool DSuperPowerPage::deleteDbAlarm()
{
    QString strSql = "Drop Table Alarm";
    QSqlQuery query;
    bool ret = query.exec(strSql);
    if(!ret)
    {
        QMessageBox::warning(m_widget, tr("Alarm"), tr("Deleting table failed: Alarm"), QMessageBox::Ok);
        return false;
    }
    return true;
}
bool DSuperPowerPage::deleteDbGetWater()
{
    QString strSql = "Drop Table GetW";
    QSqlQuery query;
    bool ret = query.exec(strSql);
    if(!ret)
    {
        QMessageBox::warning(m_widget, tr("GetWater"), tr("Deleting table failed: GetW"), QMessageBox::Ok);
        return false;
    }
    return true;
}
bool DSuperPowerPage::deleteDbPWater()
{
    QString strSql = "Drop Table PWater";
    QSqlQuery query;
    bool ret = query.exec(strSql);
    if(!ret)
    {
        QMessageBox::warning(m_widget, tr("Product Water"), tr("Deleting table failed: pWater"), QMessageBox::Ok);
        return false;
    }
    return true;
}
bool DSuperPowerPage::deleteDbLog()
{
    QString strSql = "Drop Table Log";
    QSqlQuery query;
    bool ret = query.exec(strSql);
    if(!ret)
    {
        QMessageBox::warning(m_widget, tr("Log"), tr("Deleting table failed: Log"), QMessageBox::Ok);
        return false;
    }
    return true;
}

bool DSuperPowerPage::deleteDbConsumables()
{
    QString strSql = "Delete from Consumable";
    QSqlQuery query;
    bool ret = query.exec(strSql);
    if(!ret)
    {
        QMessageBox::warning(m_widget, tr("Consumable"), tr("Deleting table failed: Consumable"), QMessageBox::Ok);
        return false;
    }
    return true;
}

bool DSuperPowerPage::deleteDbSubAccount()
{
    QString strSql = "Drop Table SubAccount";
    QSqlQuery query;
    bool ret = query.exec(strSql);
    if(!ret)
    {
        QMessageBox::warning(m_widget, tr("SubAccount"), tr("Deleting table failed: SubAccount"), QMessageBox::Ok);
        return false;
    }
    return true;
}

void DSuperPowerPage::save()
{
    gAdditionalCfgParam.productInfo.strCatalogNo = m_ExLineEdit[SYSCFGPAGE_LB_CATALOGNO]->text();
    gAdditionalCfgParam.productInfo.strSerialNo = m_ExLineEdit[SYSCFGPAGE_LB_SERIALNO]->text();
    gAdditionalCfgParam.productInfo.strProductDate = m_ExLineEdit[SYSCFGPAGE_LB_PRODATE]->text();
    gAdditionalCfgParam.productInfo.strInstallDate = m_ExLineEdit[SYSCFGPAGE_LB_INSTALLDATE]->text();

    gAdditionalCfgParam.machineInfo.iMachineFlow = m_cmbDeviceFlow->currentText().toInt();

    gAdditionalCfgParam.productInfo.iCompany = m_pCompanyComboBox->currentIndex();

    MainSaveExMachineMsg(gGlobalParam.iMachineType);
    MainSaveProductMsg(gGlobalParam.iMachineType);
    MainSaveInstallMsg(gGlobalParam.iMachineType);

    gAdditionalCfgParam.initMachine = m_cmbDefaultState->currentIndex();
    MainSaveDefaultState(gGlobalParam.iMachineType);

    MainUpdateGlobalParam();

    show(false);
    m_parent->show(true);
}


void DSuperPowerPage :: on_CmbIndexChange_DefaultState(int index)
{
    int iIdx = m_cmbDefaultState->currentIndex();
    if (iIdx == 0)
    {
         QMessageBox::StandardButton rb = QMessageBox::question(NULL,
                                                                tr("NOTIFY"),
                                                                tr("Do you want to restart the system immediately\n to enter the initialization interface?"),
                                                                QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

         if(rb != QMessageBox::Yes)
         {
             return;
         }
         gAdditionalCfgParam.machineInfo.iMachineFlow = m_cmbDeviceFlow->currentText().toInt();
         MainSaveExMachineMsg(gGlobalParam.iMachineType);

         gAdditionalCfgParam.initMachine = m_cmbDefaultState->currentIndex();
         MainSaveDefaultState(gGlobalParam.iMachineType);

         MainUpdateGlobalParam();
         update();
         /**/

         m_wndMain->restart();
     }
}

void DSuperPowerPage::on_CmbIndexChange_deviceType(int index)
{
    if (index != gGlobalParam.iMachineType)
    {
        QMessageBox::StandardButton rb = QMessageBox::question(NULL, tr("NOTIFY"), tr("Change Device Type?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if(rb != QMessageBox::Yes)
        {
            return;
        }
        MainSaveMachineType(index);
//        MainUpdateGlobalParam();
        update();
         /**/
        m_wndMain->restart();
    }
}

void DSuperPowerPage::on_btnDbDel_clicked()
{
    int index = m_pCmDbDel->currentIndex();
    switch(index)
    {
    case 0:
        deleteDbAll();
        break;
    case 1:
        deleteDbAlarm();
        break;
    case 2:
        deleteDbGetWater();
        break;
    case 3:
        deleteDbPWater();
        break;
    case 4:
        deleteDbLog();
        break;
    case 5:
        deleteDbConsumables();
        break;
    case 6:
        deleteDbSubAccount();
        break;
    default:
        break;
    }
}

bool DSuperPowerPage::deleteInfoConfig()
{
    QString strCfgName = gaMachineType[gGlobalParam.iMachineType].strName;
    strCfgName += "_info.ini"; //耗材信息配置文件

    QFile infoFile(strCfgName);
    if(!infoFile.exists())
    {
        return true;
    }
    else
    {
        if(infoFile.remove())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool DSuperPowerPage::deleteConfig()
{
    QString strCfgName = gaMachineType[gGlobalParam.iMachineType].strName;
    strCfgName += ".ini"; //系统参数配置文件

    QFile cfgFile(strCfgName);
    if(!cfgFile.exists())
    {
        return true;
    }
    else
    {
        if(cfgFile.remove())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool DSuperPowerPage::deleteCaliParamConfig()
{
    QString strCfgName = gaMachineType[gGlobalParam.iMachineType].strName;
    strCfgName += "_CaliParam.ini";  //参数校正配置文件

    QFile cailFile(strCfgName);
    if(!cailFile.exists())
    {
        return true;
    }
    else
    {
        if(cailFile.remove())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

void DSuperPowerPage::on_btnDelCfg_clicked()
{
    int index = m_pCmConfigDel->currentIndex();
    bool ret;
    switch(index)
    {
    case 0:
        ret = deleteInfoConfig();
        if(!ret)
        {
            QMessageBox::warning(NULL, tr("Warning"), tr("Delete Consumables info file failed"), QMessageBox::Ok);
        }
        ret = deleteConfig();
        if(!ret)
        {
            QMessageBox::warning(NULL, tr("Warning"), tr("Delete Config info file failed"), QMessageBox::Ok);
        }
        ret = deleteCaliParamConfig();
        if(!ret)
        {
            QMessageBox::warning(NULL, tr("Warning"), tr("Delete Caliration info file failed"), QMessageBox::Ok);
        }
        break;
    case 1:
        ret = deleteInfoConfig();
        if(!ret)
        {
            QMessageBox::warning(NULL, tr("Warning"), tr("Delete Consumables info file failed"), QMessageBox::Ok);
        }
        break;
    case 2:
        ret = deleteConfig();
        if(!ret)
        {
            QMessageBox::warning(NULL, tr("Warning"), tr("Delete Config info file failed"), QMessageBox::Ok);
        }
        break;
    case 3:
        ret = deleteCaliParamConfig();
        if(!ret)
        {
            QMessageBox::warning(NULL, tr("Warning"), tr("Delete Caliration info file failed"), QMessageBox::Ok);
        }
        break;
    default:
        break;
    }
}

void DSuperPowerPage::on_btn_clicked(int index)
{

   switch(index)
   {
   case SYSCFGPAGE_BTN_SAVE:
       save();
       break;
   default:
       break;
   }

   m_wndMain->prepareKeyStroke();//蜂鸣器报警
}




