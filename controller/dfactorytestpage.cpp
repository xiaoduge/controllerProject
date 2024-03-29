#include "dfactorytestpage.h"
#include "mainwindow.h"
#include "exconfig.h"
#include "exdisplay.h"
#include "dhintdialog.h"

#include <QGridLayout>
#include <QFormLayout>
#include <QDir>
#include <QTextBrowser>
#include <QTabWidget>
#include <QCheckBox>
#include "dlineedit.h"

DFactoryTestPage::DFactoryTestPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain)
                                   : CSubPage(parent,widget,wndMain)
{
    creatTitle();
    initUi();
    buildTranslation();
    isFlow = false;
    isPressure = false;
	bEngMode = false;
}

void DFactoryTestPage::creatTitle()
{
    CSubPage::creatTitle();
    buildTitles();
    selectTitle(0);
}

void DFactoryTestPage::buildTitles()
{
    QStringList stringList;
    stringList << tr("Factory Test");
    setTitles(stringList);
}

void DFactoryTestPage::setBackColor()
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

void DFactoryTestPage::buildTranslation()
{
    int i;
    m_tabWidget->setTabText(0, tr("RFID"));
    m_tabWidget->setTabText(1, tr("Flow & Pressure"));
    m_tabWidget->setTabText(2, tr("Wifi Test"));
    m_tabWidget->setTabText(3, tr("Maintenance"));
	m_tabWidget->setTabText(4, tr("Tools"));

    m_pConfigLabel[CONFIG_CAT]->setText(tr("Cat No.:"));
    m_pConfigLabel[CONFIG_LOT]->setText(tr("Lot No.:"));
    m_pConfigLabel[CONFIG_INSTALLDATE]->setText(tr("Installation Date:"));
    m_pConfigLabel[CONFIG_VOLUMEOFUSE]->setText(tr("Volume of Use:"));
    m_pIndexLabel->setText(tr("Address:"));

    m_pWriteBtn->setText(tr("Write"));
    m_pReadBtn->setText(tr("Read"));
    m_pClearBtn->setText(tr("Clear"));

    m_pAreaLab[FTESTPAGE_FLOW]->setText(tr("Flow Test"));
    m_pAreaLab[FTESTPAGE_FLOW]->setAlignment(Qt::AlignCenter);
    m_pFlowDisplayLab[S1_DISPLAY]->setText(tr("S1"));
    m_pFlowDisplayLab[S2_DISPLAY]->setText(tr("S2"));
    m_pFlowDisplayLab[S3_DISPLAY]->setText(tr("S3"));
    m_pFlowDisplayLab[S4_DISPLAY]->setText(tr("S4"));
    for(i = 0; i < FLOW_DISPLAY_NUM; i++)
    {
        m_pFlowDisplayLab[i]->setAlignment(Qt::AlignCenter);
    }
    m_pBtn[FTESTPAGE_FLOW]->setText(tr("Start"));


    m_pAreaLab[FTESTPAGE_ILOOP]->setText(tr("Pressure Test"));
    m_pAreaLab[FTESTPAGE_ILOOP]->setAlignment(Qt::AlignCenter);
    m_pPreDisplayLab[P1_DISPLAY]->setText(tr("Work Pres."));
    m_pPreDisplayLab[P2_DISPLAY]->setText(tr("Pure Tank Level"));
    m_pPreDisplayLab[P3_DISPLAY]->setText(tr("Source Tank Level"));
    for(i = 0; i < PRESSURE_DISPLAY_NUM; i++)
    {
        m_pPreDisplayLab[i]->setAlignment(Qt::AlignCenter);
    }

    m_pBtn[FTESTPAGE_ILOOP]->setText(tr("Start"));

    m_pClearWifiMsgBtn->setText(tr("Clear"));

    m_pBtnZigbeeUpd->setText(tr("Zigbee Upd"));

	m_pSwitchAllValve->setText(tr("Open All Valves"));

}

void DFactoryTestPage::switchLanguage()
{
    buildTranslation();
    buildTitles();
    selectTitle(titleIndex());
}

void DFactoryTestPage::initFlowTestPage()
{
    int i;
    m_pageWidget[FACTORY_PAGE_FLOW] = new QWidget(m_widget);
    m_pageWidget[FACTORY_PAGE_FLOW]->setGeometry(0, 55, 800, 545);
    QString qss = ".QWidget{ background-color:rgb(250, 250, 250);}";
    m_pageWidget[FACTORY_PAGE_FLOW]->setStyleSheet(qss);

    for(i = 0; i < FTESTPAGE_NUM; i++)
    {
        m_pFrame[i] = new QFrame(m_pageWidget[FACTORY_PAGE_FLOW]);
        m_pBtn[i] = new QPushButton;
        m_pBtn[i]->setFixedWidth(80);
        m_pAreaLab[i] = new QLabel;
    }

    for(i = 0; i < FLOW_DISPLAY_NUM; i++)
    {
        m_pFlowDisplayLE[i] = new DLineEdit;
        m_pFlowDisplayLE[i]->setReadOnly(true);
        m_pFlowDisplayLab[i] = new QLabel;
    }

    for(i = 0; i <  PRESSURE_DISPLAY_NUM; i++)
    {
        m_pPreDisplayLE[i] = new DLineEdit;
        m_pPreDisplayLE[i]->setReadOnly(true);
        m_pPreDisplayLab[i] = new QLabel;
    }
    m_pFrame[FTESTPAGE_FLOW]->setGeometry(10, 60, 770, 120);
    m_pFrame[FTESTPAGE_ILOOP]->setGeometry(10, 220, 770, 120);
   //    m_pFrame[FTESTPAGE_KEY]->setGeometry(0, 340, 800, 120);

    qss = ".QFrame{ background-color: white;\
                     border: 2px solid; \
                     border-color: rgb(135,206,250);\
                     border-radius: 4px; \
                     padding: 2px;\
                   }";

    m_pFrame[FTESTPAGE_FLOW]->setStyleSheet(qss);
    m_pFrame[FTESTPAGE_ILOOP]->setStyleSheet(qss);

    QGridLayout* gLayout_Flow = new QGridLayout;
    gLayout_Flow->addWidget(m_pAreaLab[FTESTPAGE_FLOW], 0, 0, 1, 1);
    gLayout_Flow->addWidget(m_pFlowDisplayLab[S1_DISPLAY], 0, 1, 1, 1);
    gLayout_Flow->addWidget(m_pFlowDisplayLab[S2_DISPLAY], 0, 2, 1, 1);
    gLayout_Flow->addWidget(m_pFlowDisplayLab[S3_DISPLAY], 0, 3, 1, 1);
    gLayout_Flow->addWidget(m_pFlowDisplayLab[S4_DISPLAY], 0, 4, 1, 1);

    gLayout_Flow->addWidget(m_pBtn[FTESTPAGE_FLOW], 1, 0, 1, 1);
    gLayout_Flow->addWidget(m_pFlowDisplayLE[S1_DISPLAY], 1, 1, 1, 1);
    gLayout_Flow->addWidget(m_pFlowDisplayLE[S2_DISPLAY], 1, 2, 1, 1);
    gLayout_Flow->addWidget(m_pFlowDisplayLE[S3_DISPLAY], 1, 3, 1, 1);
    gLayout_Flow->addWidget(m_pFlowDisplayLE[S4_DISPLAY], 1, 4, 1, 1);

    gLayout_Flow->setHorizontalSpacing(50);
    gLayout_Flow->setVerticalSpacing(30);
    m_pFrame[FTESTPAGE_FLOW]->setLayout(gLayout_Flow);

    QGridLayout* gLayout_ILoop = new QGridLayout;
    gLayout_ILoop->addWidget(m_pAreaLab[FTESTPAGE_ILOOP], 0, 0, 1, 1);
    gLayout_ILoop->addWidget(m_pPreDisplayLab[P1_DISPLAY], 0, 1, 1, 1);
    gLayout_ILoop->addWidget(m_pPreDisplayLab[P2_DISPLAY], 0, 2, 1, 1);
    gLayout_ILoop->addWidget(m_pPreDisplayLab[P3_DISPLAY], 0, 3, 1, 1);


    gLayout_ILoop->addWidget(m_pBtn[FTESTPAGE_ILOOP], 1, 0, 1, 1);
    gLayout_ILoop->addWidget(m_pPreDisplayLE[P1_DISPLAY], 1, 1, 1, 1);
    gLayout_ILoop->addWidget(m_pPreDisplayLE[P2_DISPLAY], 1, 2, 1, 1);
    gLayout_ILoop->addWidget(m_pPreDisplayLE[P3_DISPLAY], 1, 3, 1, 1);

    gLayout_ILoop->setHorizontalSpacing(60);
    gLayout_ILoop->setVerticalSpacing(30);
    m_pFrame[FTESTPAGE_ILOOP]->setLayout(gLayout_ILoop);

    connect(m_pBtn[FTESTPAGE_FLOW], SIGNAL(clicked()), this, SLOT(on_flowBtn_clicked()));
    connect(m_pBtn[FTESTPAGE_ILOOP], SIGNAL(clicked()), this, SLOT(on_iLoopBtn_clicked()));
    //    connect(m_pBtn[FTESTPAGE_KEY], SIGNAL(clicked()), this, SLOT(on_keyBtn_clicked()));

    QIcon icon1(":/pic/unselected.png");
    m_tabWidget->addTab(m_pageWidget[FACTORY_PAGE_FLOW], icon1, tr("Flow & Pressure"));

}

void DFactoryTestPage::initRFIDTestPage()
{
    m_pageWidget[FACTORY_PAGE_RFID] = new QWidget(m_widget);
    m_pageWidget[FACTORY_PAGE_RFID]->setGeometry(0, 55, 800, 545);
    QString qss = ".QWidget{ background-color:rgb(250, 250, 250);}";
    m_pageWidget[FACTORY_PAGE_RFID]->setStyleSheet(qss);

    QFormLayout* fLayout = new QFormLayout;
    fLayout->setAlignment(Qt::AlignCenter);
    fLayout->setVerticalSpacing(8);

    m_pIndexLabel = new QLabel;
    m_pIndexCombo = new QComboBox;
    m_pIndexCombo->addItem(tr("20"));
    m_pIndexCombo->addItem(tr("21"));
    m_pIndexCombo->addItem(tr("22"));
    m_pIndexCombo->addItem(tr("23"));
    m_pIndexCombo->addItem(tr("24"));
    m_pIndexCombo->addItem(tr("25"));
    m_pIndexCombo->setMinimumWidth(100);
    fLayout->addRow(m_pIndexLabel, m_pIndexCombo);

    for(int i = 0; i < CONFIG_NUM; i++)
    {
        m_pConfigLabel[i] = new QLabel;
        m_pConfigLineEdit[i] = new DLineEdit;
//        m_pConfigLineEdit[i]->setMaximumWidth(180);
        fLayout->addRow(m_pConfigLabel[i], m_pConfigLineEdit[i]);
    }

//    m_pConfigLineEdit[CONFIG_INSTALLDATE]->setReadOnly(true);
    m_pConfigLineEdit[CONFIG_INSTALLDATE]->setInputMask("0000-00-00");

    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->setAlignment(Qt::AlignCenter);
    m_pWriteBtn = new QPushButton;
    m_pReadBtn = new QPushButton;
    m_pClearBtn = new QPushButton;
    m_pWriteBtn->setMaximumWidth(100);
    m_pReadBtn->setMaximumWidth(100);
    m_pClearBtn->setMaximumWidth(100);
    hLayout->addWidget(m_pWriteBtn);
    hLayout->addSpacing(50);
    hLayout->addWidget(m_pReadBtn);
    hLayout->addSpacing(50);
    hLayout->addWidget(m_pClearBtn);

    QVBoxLayout* vLayout = new QVBoxLayout;
    vLayout->addLayout(fLayout);
    vLayout->addLayout(hLayout);

    m_pageWidget[FACTORY_PAGE_RFID]->setLayout(vLayout);

    QIcon icon1(":/pic/unselected.png");
    m_tabWidget->addTab(m_pageWidget[FACTORY_PAGE_RFID], icon1, tr("RFID TEST"));

    connect(m_pWriteBtn, SIGNAL(clicked()), this, SLOT(on_writeBtn_clicked()));
    connect(m_pReadBtn, SIGNAL(clicked()), this, SLOT(on_readBtn_clicked()));
    connect(m_pClearBtn, SIGNAL(clicked()), this, SLOT(on_clearBtn_clicked()));
}

void DFactoryTestPage::initUpdateWifiPage()
{
    m_pageWidget[FACTORY_PAGE_UPDWIFI] = new QWidget(m_widget);
    m_pageWidget[FACTORY_PAGE_UPDWIFI]->setGeometry(0, 55, 800, 545);
    QString qss = ".QWidget{ background-color:rgb(250, 250, 250);}";
    m_pageWidget[FACTORY_PAGE_UPDWIFI]->setStyleSheet(qss);

    m_pClearWifiMsgBtn = new QPushButton(m_pageWidget[FACTORY_PAGE_UPDWIFI]);
    m_pClearWifiMsgBtn->setGeometry(550, 50, 80, 30);
    connect(m_pClearWifiMsgBtn, SIGNAL(clicked()), this, SLOT(on_clearWifiMsgBtn_clicked()));

    m_pWifiMsgTBrowser = new QTextBrowser(m_pageWidget[FACTORY_PAGE_UPDWIFI]);
	m_pWifiMsgTBrowser->document()->setMaximumBlockCount(200);
    m_pWifiMsgTBrowser->setGeometry(50, 85, 700, 400);

    QIcon icon1(":/pic/unselected.png");
    m_tabWidget->addTab(m_pageWidget[FACTORY_PAGE_UPDWIFI], icon1, tr("Update Wifi"));
}

void DFactoryTestPage::initzigbeePage()
{
    m_pageWidget[FACTORY_PAGE_UPDZIGBEE] = new QWidget(m_widget);
    m_pageWidget[FACTORY_PAGE_UPDZIGBEE]->setGeometry(0, 55, 800, 545);
    QString qss = ".QWidget{ background-color:rgb(250, 250, 250);}";
    m_pageWidget[FACTORY_PAGE_UPDZIGBEE]->setStyleSheet(qss);

    QHBoxLayout  *hLayout =  new QHBoxLayout;

    m_plbZigbeeUpd = new QLabel;
    m_plbZigbeeUpd->setAlignment(Qt::AlignCenter);
    m_plbZigbeeUpd->setText("00");
    hLayout->addWidget(m_plbZigbeeUpd);

    m_pBtnZigbeeUpd  = new QPushButton;
    m_pBtnZigbeeUpd->setText(tr("Zigbee Upd"));
    hLayout->addWidget(m_pBtnZigbeeUpd);

    m_pageWidget[FACTORY_PAGE_UPDZIGBEE]->setLayout(hLayout);

    QIcon icon1(":/pic/unselected.png");
    m_tabWidget->addTab(m_pageWidget[FACTORY_PAGE_UPDZIGBEE], icon1, tr("Maintenance"));

    connect(m_pBtnZigbeeUpd, SIGNAL(clicked()), this, SLOT(on_updZigbeeBtn_clicked()));
}

void DFactoryTestPage::initToolsPage()
{
	QString strQss4Chk = m_wndMain->getQss4Chk();

	m_pageWidget[FACTORY_PAGE_TOOLS] = new QWidget(m_widget);
    m_pageWidget[FACTORY_PAGE_TOOLS]->setGeometry(0, 55, 800, 545);
    QString qss = ".QWidget{ background-color:rgb(250, 250, 250);}";
    m_pageWidget[FACTORY_PAGE_TOOLS]->setStyleSheet(qss);

    QHBoxLayout  *hLayout =  new QHBoxLayout;

	m_pSwitchAllValve = new QCheckBox;
	m_pSwitchAllValve->setStyleSheet(strQss4Chk);
    hLayout->addWidget(m_pSwitchAllValve);
	hLayout->setAlignment(Qt::AlignLeft);

	QVBoxLayout *vLayout = new QVBoxLayout;
	vLayout->addLayout(hLayout);
	vLayout->setAlignment(Qt::AlignTop);
	vLayout->setContentsMargins(50, 50, 10, 10);
    m_pageWidget[FACTORY_PAGE_TOOLS]->setLayout(vLayout);

    QIcon icon1(":/pic/unselected.png");
    m_tabWidget->addTab(m_pageWidget[FACTORY_PAGE_TOOLS], icon1, tr("Tools"));

    connect(m_pSwitchAllValve, SIGNAL(stateChanged(int)), this, SLOT(on_switchAllValve_stateChanged(int)));
}

void DFactoryTestPage::on_flowBtn_clicked()
{
    if(isFlow)
    {
        return;
    }
    isFlow = true;

    int iRet;
    iRet = Ex_FactoryTest(0);
    if(iRet)
    {
        QMessageBox::warning(m_widget, tr("Fail: Flow"), tr("Can not enter test Mode"), QMessageBox::Ok);
    }
}

void DFactoryTestPage::on_iLoopBtn_clicked()
{
    if(isPressure)
    {
        return;
    }
    isPressure = true;
    int iRet;
    iRet = Ex_FactoryTest(1);
    if(iRet)
    {
        if(iRet)
        {
            QMessageBox::warning(m_widget, tr("Fail: Pressure"), tr("Can not enter test Mode"), QMessageBox::Ok);
        }
    }
}

void DFactoryTestPage::on_keyBtn_clicked()
{
}

void DFactoryTestPage::on_writeBtn_clicked()
{
    m_wndMain->prepareKeyStroke();
    int index = m_pIndexCombo->currentIndex();
    QString catData = m_pConfigLineEdit[CONFIG_CAT]->text();
    QString lotData = m_pConfigLineEdit[CONFIG_LOT]->text();
    QString installDate = m_pConfigLineEdit[CONFIG_INSTALLDATE]->text();
    QString volData = m_pConfigLineEdit[CONFIG_VOLUMEOFUSE]->text();
    int iRet = 0;

    iRet = m_wndMain->writeRfid(index, RF_DATA_LAYOUT_CATALOGUE_NUM, catData);
    if(iRet != 0)
    {
        QMessageBox::warning(NULL, tr("Warning"), tr("write cat error"), QMessageBox::Ok);
    }
    iRet = m_wndMain->writeRfid(index, RF_DATA_LAYOUT_LOT_NUMBER, lotData);
    if(iRet != 0)
    {
        QMessageBox::warning(NULL, tr("Warning"), tr("write lot error"), QMessageBox::Ok);
    }

    iRet = m_wndMain->writeRfid(index, RF_DATA_LAYOUT_INSTALL_DATE, installDate);
    if(iRet != 0)
    {
        QMessageBox::warning(NULL, tr("Warning"), tr("write install date error"), QMessageBox::Ok);
    }
    iRet = m_wndMain->writeRfid(index, RF_DATA_LAYOUT_UNKNOW_DATA, volData);
    if(iRet != 0)
    {
        QMessageBox::warning(NULL, tr("Warning"), tr("write vol data error"), QMessageBox::Ok);
    }

    DHintDialog::getInstance(tr("Write finished"));
    m_wndMain->updateCMInfoWithRFID(0);
}

void DFactoryTestPage::on_readBtn_clicked()
{
    m_wndMain->prepareKeyStroke();
    updateRFIDInfo(m_pIndexCombo->currentIndex());
}

void DFactoryTestPage::on_clearBtn_clicked()
{
    m_pConfigLineEdit[CONFIG_CAT]->clear();
    m_pConfigLineEdit[CONFIG_LOT]->clear();
    m_pConfigLineEdit[CONFIG_INSTALLDATE]->clear();
    m_pConfigLineEdit[CONFIG_VOLUMEOFUSE]->clear();
}

void DFactoryTestPage::on_clearWifiMsgBtn_clicked()
{
    m_pWifiMsgTBrowser->clear();
}

void DFactoryTestPage::on_updZigbeeBtn_clicked()
{
    int iLength;

    QString BIN_FILE =  ":/other/shznApp.bin";

    QFileInfo info(BIN_FILE);
    if (info.exists())
    {
        iLength = info.size();
    }

    if (iLength <= MAX_FILE_SIZE)
    {
        QFile tmpfile(BIN_FILE);
        tmpfile.open(QIODevice::ReadOnly);
        tmpfile.read((char*)gFileMem, iLength);
        tmpfile.close();

        DispStartZigbeeUpd(iLength,NULL);
    }
    m_wndMain->prepareKeyStroke();
}

void DFactoryTestPage::on_switchAllValve_stateChanged(int state)
{
    //如果当前不是待机状态，则不执行打开所有电磁阀的操作，直接返回
    if (DISP_WORK_STATE_IDLE != DispGetWorkState())
    {
        //保证把CheckBox状态切换到Unchecked状态
        if(state == Qt::Checked)
        {
            m_pSwitchAllValve->setCheckState(Qt::Unchecked);
            QMessageBox::about(NULL, tr("Notice"), tr("Please Stop System First!"));
        }
        
        return;
    }

    unsigned char buf[1];
    DISPHANDLE hdl = NULL;
    buf[0] = state == Qt::Checked ? 1 : 0;
    hdl = DispCmdEntry(DISP_CMD_OPENALLVALVES, buf, 1);
    if(!hdl)
    {
        //命令返回错误时，保证把CheckBox状态切换到Unchecked状态
        if(state == Qt::Checked)
        {
            m_pSwitchAllValve->setCheckState(Qt::Unchecked);
        }
        qDebug() << "DISP_CMD_ENG_CMD: " << hdl;
    }
}

void DFactoryTestPage::initUi()
{
    setBackColor();

    m_mainWidget = new QWidget(m_widget);
    m_mainWidget->setGeometry(QRect(0, 55, 800, this->height() - 55));

    QGridLayout *mainLayout = new QGridLayout;
    m_tabWidget = new QTabWidget;
    //add page
    initRFIDTestPage();
    initFlowTestPage();
    initUpdateWifiPage();
    initzigbeePage();
	initToolsPage();

    mainLayout->addWidget(m_tabWidget, 0, 0);
    m_mainWidget->setLayout(mainLayout);

    QFile qss(":/app/tabWidget.qss");
    qss.open(QFile::ReadOnly);
    QString tabWidgetqss = QLatin1String (qss.readAll());
    qss.close();
		
    m_tabWidget->setStyleSheet(tabWidgetqss);
    m_tabWidget->setFocusPolicy(Qt::NoFocus);

}

void DFactoryTestPage::show(bool bShow)
{
    CSubPage::show(bShow);
}

void DFactoryTestPage::update()
{
    m_wndMain->setWorkMode(APP_WORK_MODE_INSTALL);
    CSubPage::update();
}

void DFactoryTestPage::fade()
{
    //离开当前页时，保证把CheckBox状态切换到Unchecked状态
    if(m_pSwitchAllValve->checkState() == Qt::Checked)
    {
        m_pSwitchAllValve->setCheckState(Qt::Unchecked);
    }

    m_wndMain->setWorkMode(APP_WORK_MODE_NORMAL);
    CSubPage::fade();
}

void DFactoryTestPage::updateFlow(int iIndex, int value)
{
    double temp = (value*1.0)/1000;
    switch(iIndex)
    {
    case APP_FM_FM1_NO:
        m_pFlowDisplayLE[S1_DISPLAY]->setText(QString("%1").arg(temp));
        break;
    case APP_FM_FM2_NO:
        m_pFlowDisplayLE[S2_DISPLAY]->setText(QString("%1").arg(temp));
        break;
    case APP_FM_FM3_NO:
        m_pFlowDisplayLE[S3_DISPLAY]->setText(QString("%1").arg(temp));
        break;
    case APP_FM_FM4_NO:
        m_pFlowDisplayLE[S4_DISPLAY]->setText(QString("%1").arg(temp));
        break;
    }
}

void DFactoryTestPage::updatePressure(int iIndex, float fvalue)
{
    if(APP_EXE_PM1_NO == iIndex)
    {
        m_pPreDisplayLE[P1_DISPLAY]->setText(QString("%1").arg(fvalue));
    }
}

void DFactoryTestPage::updTank(int iLevel, float fVolume)
{
    Q_UNUSED(fVolume);
    m_pPreDisplayLE[P2_DISPLAY]->setText(QString("%1").arg(iLevel));
}

void DFactoryTestPage::updSourceTank(int iLevel, float fVolume)
{
    Q_UNUSED(fVolume);
    m_pPreDisplayLE[P3_DISPLAY]->setText(QString("%1").arg(iLevel));
}

void DFactoryTestPage::updateRFIDInfo(int iRfId)
{
    int iRet;

    CATNO cn;
    LOTNO ln;
    QDate installDate;
    int volUsed;

    memset(cn,0,sizeof(CATNO));
    memset(ln,0,sizeof(LOTNO));
	
    {
        iRet = m_wndMain->readRfid(iRfId);
        if (iRet)
        {
            return;
        }

        m_wndMain->getRfidCatNo(iRfId,cn);
        m_wndMain->getRfidLotNo(iRfId,ln);
        m_wndMain->getRfidInstallDate(iRfId, &installDate);
        m_wndMain->getRfidVolofUse(iRfId, volUsed);
    }

    m_pConfigLineEdit[CONFIG_CAT]->setText(cn);
    m_pConfigLineEdit[CONFIG_LOT]->setText(ln);
    m_pConfigLineEdit[CONFIG_INSTALLDATE]->setText(installDate.toString("yyyy-MM-dd"));
    m_pConfigLineEdit[CONFIG_VOLUMEOFUSE]->setText(QString("%1").arg(volUsed));
}

void DFactoryTestPage::updateWifiTestMsg(const QString &msg)
{
	QString curTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
	m_pWifiMsgTBrowser->append(curTime + " : " + msg);
}

void DFactoryTestPage::zigbeeUpdResult(int iResult, int iPercent)
{
    if (iResult)
    {
        m_plbZigbeeUpd->setText(tr("FAIL"));
    }
    else
    {
        m_plbZigbeeUpd->setText(QString::number(iPercent));
    }
}

