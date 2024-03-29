#include "dhistorypage.h"
#include "mainwindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTime>
#include <QSqlError>
#include <QSqlTableModel>
#include <QTableView>
#include <QHeaderView>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDockWidget>
#include "sendtopage.h"
#include "ToastDlg.h"
#include <QCalendarWidget>

DHistoryPage::DHistoryPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain) : CSubPage(parent,widget,wndMain)
{
    creatTitle();
    initUi();
    buildTranslation();
}

void DHistoryPage::creatTitle()
{
    CSubPage::creatTitle();
    buildTitles();
    selectTitle(0);
}

void DHistoryPage::buildTitles()
{
    QStringList stringList;
    stringList << tr("History");
    setTitles(stringList);

}

void DHistoryPage::initConnect()
{
    connect(m_phBtns[HISTORYPAGE_BTN_QRY_STIME], SIGNAL(clicked()), this, SLOT(on_HISTORYPAGE_BTN_QRY_STIME_clicked()));
    connect(m_phBtns[HISTORYPAGE_BTN_QRY_ETIME], SIGNAL(clicked()), this, SLOT(on_HISTORYPAGE_BTN_QRY_ETIME_clicked()));
    connect(m_phBtns[HISTORYPAGE_BTN_QRY], SIGNAL(clicked()), this, SLOT(on_HISTORYPAGE_BTN_QRY_clicked()));

    connect(m_phBtns[HISTORYPAGE_BTN_SEND], SIGNAL(clicked()), this, SLOT(on_HISTORYPAGE_BTN_SEND_clicked()));
    connect(m_phBtns[HISTORYPAGE_BTN_PREV], SIGNAL(clicked()), this, SLOT(on_HISTORYPAGE_BTN_PREV_clicked()));
    connect(m_phBtns[HISTORYPAGE_BTN_NEXT], SIGNAL(clicked()), this, SLOT(on_HISTORYPAGE_BTN_NEXT_clicked()));

    connect(m_ptBtns[HISTORYPAGE_TIME_BTN_CANCEL], SIGNAL(clicked()), this, SLOT(on_HISTORYPAGE_TIME_BTN_CANCEL_clicked()));
    connect(m_ptBtns[HISTORYPAGE_TIME_BTN_OK], SIGNAL(clicked()), this, SLOT(on_HISTORYPAGE_TIME_BTN_OK_clicked()));
}

void DHistoryPage::buildTranslation()
{
    m_ptBtns[HISTORYPAGE_TIME_BTN_CANCEL]->setText(tr("Cancel"));
    m_ptBtns[HISTORYPAGE_TIME_BTN_OK]->setText(tr("OK"));

    m_phBtns[HISTORYPAGE_BTN_SEND]->setText(tr("Send"));
    m_phBtns[HISTORYPAGE_BTN_QRY]->setText(tr("Search"));
    m_phBtns[HISTORYPAGE_BTN_PREV]->setText(tr("Page Up"));
    m_phBtns[HISTORYPAGE_BTN_NEXT]->setText(tr("Page Down"));

    m_plbStartTime->setText(tr("Start Date"));
    m_plbStopTime->setText(tr("End Date"));

    m_QryObjNameCbox->setItemText(0, tr("Alarm"));
    m_QryObjNameCbox->setItemText(1, tr("GetW"));
    m_QryObjNameCbox->setItemText(2, tr("PWater"));
    m_QryObjNameCbox->setItemText(3, tr("Log"));
    
#ifdef WATERCARDREADER
    m_QryObjNameCbox->setItemText(4, tr("WaterCard"));
#endif
//    m_QryObjNameCbox->setItemText(4, tr("Water"));

    initTabelHeaderData(m_pTableModel, m_QryObjNameCbox->currentIndex() + 1);
}

void DHistoryPage::switchLanguage()
{
    buildTranslation();
    buildTitles();
    selectTitle(titleIndex());
}

void DHistoryPage::setBackColor()
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

void DHistoryPage::initUi()
{
    int iLoop;
    setBackColor();

    m_pOperatingWidget = new QWidget(m_widget);

    QPalette pal(m_pOperatingWidget->palette());
    pal.setColor(QPalette::Background, Qt::white);
    m_pOperatingWidget->setAutoFillBackground(true);
    m_pOperatingWidget->setPalette(pal);

    m_pOperatingWidget->setGeometry(QRect(10 , 60 , 780 ,530));

    for(iLoop = 0; iLoop < HISTORYPAGE_BTN_NUM; iLoop++)
    {
        m_phBtns[iLoop] = new QPushButton(m_pOperatingWidget);
    }

    m_plbStartTime = new QLabel(m_pOperatingWidget);
    m_plbStartTime->setBuddy(m_phBtns[HISTORYPAGE_BTN_QRY_STIME]);
    m_plbStopTime = new QLabel(m_pOperatingWidget);
    m_plbStopTime->setBuddy(m_phBtns[HISTORYPAGE_BTN_QRY_ETIME]);

    m_QryObjNameCbox = new QComboBox(m_pOperatingWidget);
    QStringList cbList;
#ifdef WATERCARDREADER
    cbList << tr("Alarm") << tr("GetW") << tr("PWater") << tr("Log") << tr("WaterCard");  // << tr("Water")
#else
    cbList << tr("Alarm") << tr("GetW") << tr("PWater") << tr("Log");  // << tr("Water")
#endif
    m_QryObjNameCbox->addItems(cbList);

    m_pTableView = new QTableView(m_pOperatingWidget);
    m_pTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_plbStartTime->setGeometry(QRect(140, 10, 80, 25));
    m_phBtns[HISTORYPAGE_BTN_QRY_STIME]->setGeometry(QRect(225, 10, 120, 25));
    m_plbStopTime->setGeometry(QRect(425, 10, 80, 25));
    m_phBtns[HISTORYPAGE_BTN_QRY_ETIME]->setGeometry(QRect(510, 10, 120, 25));

    m_QryObjNameCbox->setGeometry(QRect(225, 50, 120, 25));
    m_phBtns[HISTORYPAGE_BTN_QRY]->setGeometry(QRect(510, 50, 120, 25));

    m_pTableView->setGeometry(10, 80, 760, 400);

    m_phBtns[HISTORYPAGE_BTN_PREV]->setGeometry(QRect(120, 490, 120, 25));
    m_phBtns[HISTORYPAGE_BTN_NEXT]->setGeometry(QRect(310, 490,120, 25));
    m_phBtns[HISTORYPAGE_BTN_SEND]->setGeometry(QRect(500, 490, 120, 25));
    m_phBtns[HISTORYPAGE_BTN_PREV]->hide();
    m_phBtns[HISTORYPAGE_BTN_NEXT]->hide();
    //
    m_dockWidget = new QDockWidget(m_pOperatingWidget);
    m_dockWidget->setGeometry(QRect(0 , 50 , 800 ,400));
    for(iLoop = 0; iLoop < HISTORYPAGE_TIME_BTN_NUM; iLoop++)
    {
        m_ptBtns[iLoop] = new QPushButton(m_dockWidget);
    }
    m_ptBtns[HISTORYPAGE_TIME_BTN_CANCEL]->setGeometry(QRect(230 , 350, 100, 20));
    m_ptBtns[HISTORYPAGE_TIME_BTN_OK]->setGeometry(QRect(430 , 350, 100, 20));

    m_pCalendar = new QCalendarWidget(m_dockWidget);
    switch(gGlobalParam.MiscParam.iLan)
    {
    case APP_LAN_CHN:
        m_pCalendar->setLocale(QLocale::Chinese);
        break;
    case APP_LAN_SPA:
        m_pCalendar->setLocale(QLocale::Spanish);
        break;
    case APP_LAN_FRE:
        m_pCalendar->setLocale(QLocale::French);
        break;
    case APP_LAN_ITA:
        m_pCalendar->setLocale(QLocale::Italian);
        break;
    case APP_LAN_SKR:
        m_pCalendar->setLocale(QLocale::Korean);
        break;
    case APP_LAN_GER:
        m_pCalendar->setLocale(QLocale::German);
        break;
    case APP_LAN_RUS:
        m_pCalendar->setLocale(QLocale::Russian);
        break;
    case APP_LAN_POR:
        m_pCalendar->setLocale(QLocale::Portuguese);
        break;
    default:
        m_pCalendar->setLocale(QLocale::English);
        break;
    }

    m_pCalendar->setGeometry(QRect(100,10,600,300));
    m_dockWidget->hide();
    m_btnID = 0;
   //sendPage
    {
        CBaseWidget *tmpWidget;

        tmpWidget = new CBaseWidget(m_wndMain->getMainWidget());
        tmpWidget->setObjectName("send to");
        tmpWidget->setGeometry(230,120,320,350);
        m_pSendToPage = new SendToPage(this , tmpWidget , m_wndMain);
    }

    QDate initDate  = QDate::currentDate();
    QString strInitDate = initDate.toString("yyyy-MM-dd");
    m_phBtns[HISTORYPAGE_BTN_QRY_ETIME]->setText(strInitDate);
    m_phBtns[HISTORYPAGE_BTN_QRY_STIME]->setText(strInitDate);

    initConnect();
    createDataModelAndView();
}

void DHistoryPage::createDataModelAndView()
{
    m_pTableModel = new QSqlTableModel(m_widget);
    dbQuery();
}

void DHistoryPage::dbQuery()
{
    int index = m_QryObjNameCbox->currentIndex();
    switch(index)
    {
    case 0:
        dbQueryAlarm();
        break;
    case 1:
        dbQueryGetWater();
        break;
    case 2:
        dbQueryProduceWater();
        break;
    case 3:
        dbQueryLog();
        break;
    
#ifdef WATERCARDREADER
    case 4:
        dbQueryWaterCardInfo();
        break;
#endif

    default:
        break;
    }
}

void DHistoryPage::dbQueryAll()
{

}

//当天水质记录
void DHistoryPage::dbQueryWater()
{
    m_pTableModel->setTable("Water");

    QString strStartTime = m_phBtns[HISTORYPAGE_BTN_QRY_STIME]->text() + " 00:00:00";
    QString strEndTime = m_phBtns[HISTORYPAGE_BTN_QRY_ETIME]->text() + " 23:59:59";
    m_pTableModel->setFilter(QString("time > '%1' and time < '%2'").arg(strStartTime).arg(strEndTime));

    m_pTableModel->select();
    initTabelHeaderData(m_pTableModel, HISTORYPAGE_DATA_WATER);
    m_pTableView->setModel(m_pTableModel);

    m_pTableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

}

//报警信息记录
void DHistoryPage::dbQueryAlarm()
{
    m_pTableModel->setTable("Alarm");

    QString strStartTime = m_phBtns[HISTORYPAGE_BTN_QRY_STIME]->text() + " 00:00:00";
    QString strEndTime = m_phBtns[HISTORYPAGE_BTN_QRY_ETIME]->text() + " 23:59:59";
    m_pTableModel->setFilter(QString("time > '%1' and time < '%2'").arg(strStartTime).arg(strEndTime));

    m_pTableModel->select();
    initTabelHeaderData(m_pTableModel, HISTORYPAGE_DATA_ALARM);
    m_pTableView->setModel(m_pTableModel);
    m_pTableView->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
    m_pTableView->setColumnWidth(0, 60);
    m_pTableView->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    m_pTableView->horizontalHeader()->setResizeMode(2, QHeaderView::Fixed);
    m_pTableView->setColumnWidth(2, 60);
    m_pTableView->horizontalHeader()->setResizeMode(3, QHeaderView::Fixed);
    m_pTableView->setColumnWidth(3, 180);
}

//取水信息记录
void DHistoryPage::dbQueryGetWater()
{
    m_pTableModel->setTable("GetW");

    QString strStartTime = m_phBtns[HISTORYPAGE_BTN_QRY_STIME]->text() + " 00:00:00";
    QString strEndTime = m_phBtns[HISTORYPAGE_BTN_QRY_ETIME]->text() + " 23:59:59";
    m_pTableModel->setFilter(QString("time > '%1' and time < '%2'").arg(strStartTime).arg(strEndTime));

    m_pTableModel->select();
    initTabelHeaderData(m_pTableModel, HISTORYPAGE_DATA_GETW);
    m_pTableView->setModel(m_pTableModel);


    m_pTableView->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
    m_pTableView->horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);
    m_pTableView->horizontalHeader()->setResizeMode(2, QHeaderView::Fixed);

    m_pTableView->setColumnWidth(1, 120);
    m_pTableView->setColumnWidth(2, 120);

    m_pTableView->horizontalHeader()->setResizeMode(3, QHeaderView::Stretch);
    m_pTableView->horizontalHeader()->setResizeMode(4, QHeaderView::Stretch);
    m_pTableView->horizontalHeader()->setResizeMode(5, QHeaderView::Stretch);
    m_pTableView->horizontalHeader()->setResizeMode(6, QHeaderView::ResizeToContents);

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_EDI:
    case MACHINE_RO:
        m_pTableView->hideColumn(4);
        break;
    case MACHINE_L_Genie:
    case MACHINE_Genie:
    case MACHINE_L_UP:
    case MACHINE_UP:
    case MACHINE_ADAPT:
    case MACHINE_PURIST:
        if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
        {
            m_pTableView->showColumn(4);
        }
        else
        {
            m_pTableView->hideColumn(4);
        }
        break;
    default:
        break;
    }
}

//运行信息记录
void DHistoryPage::dbQueryProduceWater()
{
    int count;
    int i;
    m_pTableModel->setTable("PWater");

    QString strStartTime = m_phBtns[HISTORYPAGE_BTN_QRY_STIME]->text() + " 00:00:00";
    QString strEndTime = m_phBtns[HISTORYPAGE_BTN_QRY_ETIME]->text() + " 23:59:59";
    m_pTableModel->setFilter(QString("time > '%1' and time < '%2'").arg(strStartTime).arg(strEndTime));

    m_pTableModel->select();

    initTabelHeaderData(m_pTableModel, HISTORYPAGE_DATA_PWATER);
    m_pTableView->setModel(m_pTableModel);
    m_pTableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    count = m_pTableModel->columnCount();
    for(i = 0; i < count - 1; i++)
    {
        m_pTableView->setColumnWidth(i, 100);
    }
    m_pTableView->setColumnWidth(count - 1, 180);

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_Genie:
    case MACHINE_EDI:
        break;
    case MACHINE_L_UP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_UP:
    case MACHINE_RO:
    case MACHINE_ADAPT:
        m_pTableView->hideColumn(7);
        m_pTableView->hideColumn(8);
        break;
    case MACHINE_PURIST:
        break;
    }


}

void DHistoryPage::dbQueryLog()
{
    m_pTableModel->setTable("Log");

    QString strStartTime = m_phBtns[HISTORYPAGE_BTN_QRY_STIME]->text() + " 00:00:00";
    QString strEndTime = m_phBtns[HISTORYPAGE_BTN_QRY_ETIME]->text() + " 23:59:59";
    m_pTableModel->setFilter(QString("time > '%1' and time < '%2'").arg(strStartTime).arg(strEndTime));

    m_pTableModel->select();
    initTabelHeaderData(m_pTableModel, HISTORYPAGE_DATA_LOG);
    m_pTableView->setModel(m_pTableModel);
    m_pTableView->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
    m_pTableView->setColumnWidth(0, 60);
    m_pTableView->horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);
    m_pTableView->setColumnWidth(1, 80);
    m_pTableView->horizontalHeader()->setResizeMode(2, QHeaderView::Stretch);
    m_pTableView->horizontalHeader()->setResizeMode(3, QHeaderView::Stretch);
    m_pTableView->horizontalHeader()->setResizeMode(4, QHeaderView::Fixed);
    m_pTableView->setColumnWidth(4, 180);
}

#ifdef WATERCARDREADER
void DHistoryPage::dbQueryWaterCardInfo()
{
    m_pTableModel->setTable("WaterCards");

    QString strStartTime = m_phBtns[HISTORYPAGE_BTN_QRY_STIME]->text() + " 00:00:00";
    QString strEndTime = m_phBtns[HISTORYPAGE_BTN_QRY_ETIME]->text() + " 23:59:59";
    m_pTableModel->setFilter(QString("time > '%1' and time < '%2'").arg(strStartTime).arg(strEndTime));

    m_pTableModel->select();
    initTabelHeaderData(m_pTableModel, HISTORYPAGE_DATA_WATERCARD);
    m_pTableView->setModel(m_pTableModel);
    m_pTableView->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
    m_pTableView->setColumnWidth(0, 60);
    m_pTableView->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    m_pTableView->horizontalHeader()->setResizeMode(2, QHeaderView::Stretch);
    m_pTableView->horizontalHeader()->setResizeMode(3, QHeaderView::Stretch);
}
#endif

void DHistoryPage::initTabelHeaderData(QSqlTableModel *tableModel, int tableID)
{
    switch(tableID)
    {
    case HISTORYPAGE_DATA_WATER:
        tableModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
        tableModel->setHeaderData(1, Qt::Horizontal, tr("Ecoid"));  //电极
        tableModel->setHeaderData(2, Qt::Horizontal, tr("Quality")); //水质
        tableModel->setHeaderData(3, Qt::Horizontal, tr("Time"));  //时间
        break;
    case HISTORYPAGE_DATA_ALARM:
        tableModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
        tableModel->setHeaderData(1, Qt::Horizontal, tr("type"));//
        tableModel->setHeaderData(2, Qt::Horizontal, tr("Status"));//
        tableModel->setHeaderData(3, Qt::Horizontal, tr("Time"));
        break;
    case HISTORYPAGE_DATA_GETW:
        tableModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
        tableModel->setHeaderData(1, Qt::Horizontal, tr("WName"));  //
        tableModel->setHeaderData(2, Qt::Horizontal, tr("Quantity")); //取水量
        tableModel->setHeaderData(3, Qt::Horizontal, tr("Quality")); //水质
        tableModel->setHeaderData(4, Qt::Horizontal, tr("TOC"));  //
        tableModel->setHeaderData(5, Qt::Horizontal, tr("Tmp"));    //温度
        tableModel->setHeaderData(6, Qt::Horizontal, tr("Time"));    //
        break;
    case HISTORYPAGE_DATA_PWATER:
        tableModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
        tableModel->setHeaderData(1, Qt::Horizontal, tr("Duration"));//持续时间
        tableModel->setHeaderData(2, Qt::Horizontal, tr("Ecoroin"));//进水水质
        tableModel->setHeaderData(3, Qt::Horizontal, tr("Tmproin"));//进水温度
        tableModel->setHeaderData(4, Qt::Horizontal, tr("Ecorores"));//截留率
        tableModel->setHeaderData(5, Qt::Horizontal, tr("eEcoropw"));//RO水质
        tableModel->setHeaderData(6, Qt::Horizontal, tr("Tmpropw"));//RO温度
        tableModel->setHeaderData(7, Qt::Horizontal, tr("Ecoedi"));//EDI水质
        tableModel->setHeaderData(8, Qt::Horizontal, tr("Tmpedi"));//EDI温度
        tableModel->setHeaderData(9, Qt::Horizontal, tr("Time"));//开始时间
        break;
    case HISTORYPAGE_DATA_LOG:
        tableModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
        tableModel->setHeaderData(1, Qt::Horizontal, tr("Name"));//用户名
        tableModel->setHeaderData(2, Qt::Horizontal, tr("Action"));//
        tableModel->setHeaderData(3, Qt::Horizontal, tr("Info"));  //
        tableModel->setHeaderData(4, Qt::Horizontal, tr("Time"));
        break;
#ifdef WATERCARDREADER
    case HISTORYPAGE_DATA_WATERCARD:
        tableModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
        tableModel->setHeaderData(1, Qt::Horizontal, tr("CardID"));//用户名
        tableModel->setHeaderData(2, Qt::Horizontal, tr("Balance"));//
        tableModel->setHeaderData(3, Qt::Horizontal, tr("Time"));  //
        break;
#endif
    default:
        break;
    }
}


//    m_wndMain->prepareKeyStroke();


void DHistoryPage::update()
{
//    QDate date  = m_pCalendar->selectedDate();
//    QString tmp = date.toString("yyyy-MM-dd");

//    m_phBtns[HISTORYPAGE_BTN_QRY_ETIME]->setText(tmp);
    //    m_phBtns[HISTORYPAGE_BTN_QRY_STIME]->setText(tmp);
}

void DHistoryPage::leaveSubPage()
{
    m_pTableModel->clear();
    qDebug() << "QSqlTableModel clear()";
    CSubPage::leaveSubPage();
}

void DHistoryPage::on_HISTORYPAGE_BTN_QRY_STIME_clicked()
{
    m_dockWidget->show();
    m_btnID = 1;
    m_wndMain->prepareKeyStroke();
}

void DHistoryPage::on_HISTORYPAGE_BTN_QRY_ETIME_clicked()
{
    m_dockWidget->show();
    m_btnID = 2;
    m_wndMain->prepareKeyStroke();
}

void DHistoryPage::on_HISTORYPAGE_BTN_QRY_clicked()
{
    dbQuery();
    m_wndMain->prepareKeyStroke();
}

void DHistoryPage::on_HISTORYPAGE_BTN_PREV_clicked()
{
    m_wndMain->prepareKeyStroke();
}

void DHistoryPage::on_HISTORYPAGE_BTN_NEXT_clicked()
{
    m_wndMain->prepareKeyStroke();
}

void DHistoryPage::on_HISTORYPAGE_BTN_SEND_clicked()
{
    m_widget->setWindowOpacity(0.0);

    m_pSendToPage->show(true);

    //msg_box->exec();
    m_wndMain->prepareKeyStroke();
}

void DHistoryPage::on_m_QryObjNameCbox_currentIndexChanged(int index)
{
}

void DHistoryPage::on_HISTORYPAGE_TIME_BTN_CANCEL_clicked()
{
    m_dockWidget->hide();
    m_btnID = 0;
}

void DHistoryPage::on_HISTORYPAGE_TIME_BTN_OK_clicked()
{
    QDate selectDate = m_pCalendar->selectedDate();
    QString strDate = selectDate.toString("yyyy-MM-dd");
    if(1 == m_btnID)
    {
        m_phBtns[HISTORYPAGE_BTN_QRY_STIME]->setText(strDate);
    }
    else if(2 == m_btnID)
    {
        m_phBtns[HISTORYPAGE_BTN_QRY_ETIME]->setText(strDate);
    }
    m_dockWidget->hide();
}



