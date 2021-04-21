#include "dnetworkwidget.h"
#include "dnetworkwifiinfoitem.h"
#include "mainwindow.h"
#include "dlineedit.h"
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QListWidget>
#include <QProcess>
#include <QNetworkInterface>
#include <QFileSystemWatcher>

/*******************************************************
* DWifiInfo 为单例模式
* 可提供附近可用的Wifi信息、系统当前连接的Wifi信息
*******************************************************/

void deleteWifiInfo(DWifiInfo *obj)
{
    obj->deleteLater();
    obj = NULL;
}

DWifiInfo::DWifiInfo(QObject *parent) : QObject(parent)
{
    init();
}

DWifiInfo::~DWifiInfo()
{
    qDebug() << "~DWifiInfo()";
}

QSharedPointer<DWifiInfo> DWifiInfo::m_pSingleWifiInfo = QSharedPointer<DWifiInfo>(new DWifiInfo, deleteWifiInfo);

QSharedPointer<DWifiInfo> DWifiInfo::getInstance()
{
    return m_pSingleWifiInfo;
}

/**
 * @Description: 执行shell命令获取设备附近可用wifi
 */
void DWifiInfo::getAvailableWifiList()
{
    m_pProcess[Iwlist_Wlan0_Scanning]->start("iwlist wlan0 scanning");
}

/**
 * @Description: 执行shell命令获取设备连接的wifi信息
 */
void DWifiInfo::getConnnectedWifiInfo()
{
    m_pProcess[Iwlist_Wlan0]->start("iwconfig wlan0");
}

const QMap<QString, double>& DWifiInfo::wifiInfoMap() const
{
    return m_wifiInfoMap;
}

const QStringList& DWifiInfo::availableList() const
{
    return m_availableList;
}

const QString& DWifiInfo::connectedWifi() const
{
    return m_connectedWifi;
}

/**
 * @Description: 解析执行“iwconfig wlan0”后返回的信息，解析完成后会发送“connectedWifiReady()”信号
 */
void DWifiInfo::updConnnectedWifiInfo()
{
    QString strAll = m_pProcess[Iwlist_Wlan0]->readAllStandardOutput();
    QRegExp ssidRx("ESSID:\"([^\"]*)\"");

    int pos = 0;
    if((pos = ssidRx.indexIn(strAll, pos)) != -1)
    {
        QString strSSID = ssidRx.cap(0);
        strSSID.remove("ESSID:");
        m_connectedWifi = strSSID;
    }
    else
    {
        m_connectedWifi = "";
    }
    emit connectedWifiReady();
}

/**
 * @Description: 解析执行“iwlist wlan0 scanning”后返回的信息,解析完成后会发送“availableListReady()”信号
 */
void DWifiInfo::updAvailableWifiList()
{
    QString strAll = m_pProcess[Iwlist_Wlan0_Scanning]->readAllStandardOutput();
    QRegExp ssidRx("ESSID:\"([^\"]*)\"");
    QRegExp signalLevelRx("Quality=\\d+/\\d+");

    m_availableList.clear();
    m_wifiInfoMap.clear();

    int pos = 0;
    int previous = pos;
    while((pos = ssidRx.indexIn(strAll, pos)) != -1)
    {
        QString strSSID = ssidRx.cap(0);
        strSSID.remove("ESSID:");

        if(strSSID != "\"\"")
        {
            m_availableList << strSSID;

            QString strTemp = strAll.mid(previous, pos);
            int tempPos = 0;
            if((tempPos = signalLevelRx.indexIn(strTemp, 0)) != -1)
            {
                QString strLevel = signalLevelRx.cap(0);
                strLevel.remove("Quality=");
                QStringList valueList = strLevel.split("/");
                int numerator = valueList[0].toInt();
                int denominator = valueList[1].toInt();
                double level = numerator*1.0 / denominator;
                if(strSSID != "\"\"")
                {
                    setWifiInfoMap(strSSID, level);
                }
            }
        }

        pos += ssidRx.matchedLength();
        previous = pos;
    }
    emit availableListReady();
}

/**
 * Description: 监控系统WIFI配置文件是否修改，如果修改则重新获取当前已连接的wifi信息
 */
void DWifiInfo::onWifiConfigWatcherFileChanged(const QString &fileName)
{
    Q_UNUSED(fileName);
    QTimer::singleShot(3000, this, SLOT(connnectedWifiInfo()));
}

void DWifiInfo::setWifiInfoMap(const QString key, double value)
{
    if(value > m_wifiInfoMap.value(key, 0.0))
    {
        m_wifiInfoMap[key] = value;
    }
}

void DWifiInfo::init()
{
    m_pProcess[Iwlist_Wlan0_Scanning] = new QProcess(this);
    connect(m_pProcess[Iwlist_Wlan0_Scanning], SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(updAvailableWifiList()));

    m_pProcess[Iwlist_Wlan0] = new QProcess(this);
    connect(m_pProcess[Iwlist_Wlan0], SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(updConnnectedWifiInfo()));

    QStringList fileList;
    fileList << WIFICONFIG_FILE;
    m_pFileWatcher = new QFileSystemWatcher(fileList, this);
    connect(m_pFileWatcher, SIGNAL(fileChanged (const QString &)),
            this, SLOT(onWifiConfigWatcherFileChanged(const QString &)));
}

/*******************************************************
* DNetworkWidget
* 显示附近可用的Wifi信息、查看系统的IP
*******************************************************/
DNetworkWidget::DNetworkWidget(QWidget * parent, Qt::WindowFlags f)
    : QWidget(parent, f),
      m_bRefreshed(false),
      m_bWifiEnabled(false)
{
    init();
    buildTranslation();
}

void DNetworkWidget::buildTranslation()
{
    m_pSSIDLab->setText(tr("SSID:"));
    m_pAddSSIDBtn->setText(tr("Add"));
    m_pRefreshWifiBtn->setText(tr("Refresh"));
    m_pAddCheckBox->setText(tr("Add network"));
    m_pGetNetInfoBtn->setText(tr("Information"));
}

void DNetworkWidget::setAddCheckBoxChecked(bool bChecked)
{
    m_pAddCheckBox->setChecked(bChecked);
}

void DNetworkWidget::setWifiEnabled(bool enable)
{
    m_bWifiEnabled = enable;
}

/**
 * Description: 刷新附件可用wifi列表视图
 */
void DNetworkWidget::updAvailableWifiListView()
{
    QStringList availableList = m_pWifiInfo->availableList();
    QMap<QString, double> wifiInfoMap = m_pWifiInfo->wifiInfoMap();
    m_connectedWifi = m_pWifiInfo->connectedWifi();

    m_pWifiMsgListWidget->clear();
    QSet<QString> ssidSet = availableList.toSet();
    QSet<QString>::const_iterator it;
    for(it = ssidSet.begin(); it != ssidSet.end(); ++it)
    {
        QListWidgetItem *listItem = new QListWidgetItem;

        listItem ->setSizeHint(QSize(30 , 40));

        if(*it == m_connectedWifi)
        {
            DNetworkWifiInfoItem *listItemWidget  = new DNetworkWifiInfoItem(*it, wifiInfoMap.value(*it), true);
            m_pWifiMsgListWidget->insertItem(0, listItem);
            m_pWifiMsgListWidget->setItemWidget(listItem , listItemWidget);
        }
        else
        {
            DNetworkWifiInfoItem *listItemWidget  = new DNetworkWifiInfoItem(*it, wifiInfoMap.value(*it));
            m_pWifiMsgListWidget->addItem(listItem);
            m_pWifiMsgListWidget->setItemWidget(listItem , listItemWidget);
        }

    }
    m_bRefreshed = true;
}

void DNetworkWidget::updConnectedWifiInfo()
{
    m_connectedWifi = m_pWifiInfo->connectedWifi();
}

void DNetworkWidget::onWifiRefreshBtnClicked()
{
    m_pWifiInfo->getAvailableWifiList();
    m_pWifiMsgListWidget->clear();
    m_pWifiMsgListWidget->addItem("Searching");
}

/**
 * Description: 获取设备各个网口的IP信息
 */
void DNetworkWidget::onGetNetInfoBtnClicked()
{
    m_pWifiMsgListWidget->clear();

    QStringList names;  //设置我们关心的网络设备名称
    names << "eth0";

    if(m_bWifiEnabled)
    {
       names << "wlan0";
    }

    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    QList<QNetworkInterface>::const_iterator iter;
    for(iter = list.constBegin(); iter != list.constEnd(); iter++)
    {
        QNetworkInterface interface = (*iter);
        if(names.contains(interface.name()))
        {
            //获取IP地址条目列表，每个条目中包含一个IP地址，一个子网掩码和一个广播地址
            QList<QNetworkAddressEntry> entryList = interface.addressEntries();

            int listSize = entryList.size();
            if(listSize > 0)
            {
                m_pWifiMsgListWidget->addItem(interface.name() + ":"); //设备名
            }

            foreach(QNetworkAddressEntry entry, entryList) //遍历每一个IP地址条目
            {
                m_pWifiMsgListWidget->addItem("IP Address:" + entry.ip().toString()); //IP地址
                m_pWifiMsgListWidget->addItem("Netmask:" + entry.netmask().toString()); //子网掩码
                m_pWifiMsgListWidget->addItem("Broadcast:" + entry.broadcast().toString()); //广播地址
            }
            if(listSize > 0)
            {
                m_pWifiMsgListWidget->addItem(" ");
            }
        }

    }
    m_bRefreshed = false;
}

void DNetworkWidget::onWifiListWidgetItemClicked(QListWidgetItem *item)
{
    if(m_bRefreshed)
    {
        DNetworkWifiInfoItem *itemWidget = qobject_cast<DNetworkWifiInfoItem*>(m_pWifiMsgListWidget->itemWidget(item));
        if(itemWidget)
        {
            QString strName = itemWidget->wifiName();
            strName = strName.remove("\"");
            gpMainWnd->showWifiConfigDlg(strName);
        }

    }
}

void DNetworkWidget::onAddCheckBoxStateChanged(int state)
{
    if(state == Qt::Checked)
    {
        m_pSSIDEdit->clear();
        m_pWifiSSIDAddWidget->show();
    }
    else
    {
        m_pWifiSSIDAddWidget->hide();
    }
}

void DNetworkWidget::onAddSSIDBtnClicked()
{
    QString strSSID = QString("ESSID:\"%1\"").arg(m_pSSIDEdit->text());
    m_pWifiMsgListWidget->addItem(strSSID);
}

void DNetworkWidget::initUI()
{
    this->setFixedSize(530, 300);

    m_pWifiConfigWidget = new QWidget(this);
    QPalette pal(m_pWifiConfigWidget->palette());
    pal.setColor(QPalette::Background, Qt::gray);
    m_pWifiConfigWidget->setAutoFillBackground(true);
    m_pWifiConfigWidget->setPalette(pal);
    m_pWifiConfigWidget->setGeometry(QRect(0 , 0 , 530 ,242));

    m_pWifiMsgListWidget = new QListWidget(m_pWifiConfigWidget);
    m_pWifiMsgListWidget->setGeometry(5, 5, 520, 200);

    m_pRefreshWifiBtn = new QPushButton(m_pWifiConfigWidget);
    m_pRefreshWifiBtn->setGeometry(210, 208, 100, 30);

    m_pGetNetInfoBtn = new QPushButton(m_pWifiConfigWidget);
    m_pGetNetInfoBtn->setGeometry(50, 208, 100, 30);

    m_pAddCheckBox = new QCheckBox(m_pWifiConfigWidget);
    m_pAddCheckBox->setGeometry(390, 208, 120, 30);

    QString strQss4Chk = gpMainWnd->getQss4Chk();
    m_pAddCheckBox->setStyleSheet(strQss4Chk);

    m_pWifiSSIDAddWidget = new QWidget(this);
    pal = m_pWifiSSIDAddWidget->palette();
    pal.setColor(QPalette::Background, Qt::gray);
    m_pWifiSSIDAddWidget->setAutoFillBackground(true);
    m_pWifiSSIDAddWidget->setPalette(pal);
    m_pWifiSSIDAddWidget->setGeometry(QRect(0 , 250, 530 ,50));

    m_pSSIDLab = new QLabel(m_pWifiSSIDAddWidget);
    m_pSSIDLab->setGeometry(QRect(5, 10 , 50 , 30));

    m_pSSIDEdit = new DLineEdit(m_pWifiSSIDAddWidget);
    m_pSSIDEdit->setGeometry(QRect(60, 10 , 190 , 30));

    m_pAddSSIDBtn = new QPushButton(m_pWifiSSIDAddWidget);
    m_pAddSSIDBtn->setGeometry(400, 10, 100, 30);

    if(!(Qt::Checked == m_pAddCheckBox->checkState()))
    {
        m_pWifiSSIDAddWidget->hide();
    }

    connect(m_pRefreshWifiBtn, SIGNAL(clicked()), this, SLOT(onWifiRefreshBtnClicked()));
    connect(m_pGetNetInfoBtn, SIGNAL(clicked()), this, SLOT(onGetNetInfoBtnClicked()));
    connect(m_pAddCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onAddCheckBoxStateChanged(int)));
    connect(m_pWifiMsgListWidget, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(onWifiListWidgetItemClicked(QListWidgetItem*)));
    connect(m_pAddSSIDBtn, SIGNAL(clicked()), this, SLOT(onAddSSIDBtnClicked()));
}

void DNetworkWidget::init()
{
    initUI();
    m_pWifiInfo = DWifiInfo::getInstance();
    connect(m_pWifiInfo.data(), SIGNAL(availableListReady()), this, SLOT(updAvailableWifiListView()));
    connect(m_pWifiInfo.data(), SIGNAL(connectedWifiReady()), this, SLOT(updConnectedWifiInfo()));

    m_pWifiInfo->getConnnectedWifiInfo();
}

