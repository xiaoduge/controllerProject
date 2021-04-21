/****************************************************************************
**
** 网络配置控件
** 查看设备附件可用wifi信息
** 查看设备网络连接信息
**
****************************************************************************/
#ifndef DNETWORKWIDGET_H
#define DNETWORKWIDGET_H

#include <QWidget>
#include <QMap>

class QPushButton;
class QCheckBox;
class QLabel;
class DLineEdit;
class QListWidget;
class QListWidgetItem;
class QProcess;
class QFileSystemWatcher;

class DWifiInfo : public QObject
{
    Q_OBJECT
    enum IwlistProcess
    {
        Iwlist_Wlan0_Scanning,
        Iwlist_Wlan0,
        IwlistProcessNum
    };

public:
    static QSharedPointer<DWifiInfo> getInstance();
    void getAvailableWifiList();
    void getConnnectedWifiInfo();
    const QMap<QString, double>& wifiInfoMap() const;
    const QStringList& availableList() const;
    const QString& connectedWifi() const;

    ~DWifiInfo();

signals:
    void availableListReady();
    void connectedWifiReady();

protected slots:
    void updAvailableWifiList();
    void updConnnectedWifiInfo();
    void onWifiConfigWatcherFileChanged(const QString &fileName);

private:
    explicit DWifiInfo(QObject *parent = 0);
    DWifiInfo(const DWifiInfo&);
    DWifiInfo& operator =(const DWifiInfo&);
    void init();
    void setWifiInfoMap(const QString key, double value);

    QStringList           m_availableList; //附近可用的WiFi信息
    QString               m_connectedWifi; //当前连接的wifi信息
    QMap<QString, double> m_wifiInfoMap;   //附件wifi的信号强度
    QFileSystemWatcher    *m_pFileWatcher;

    QProcess *m_pProcess[IwlistProcessNum];
    static QSharedPointer<DWifiInfo> m_pSingleWifiInfo;
};


class DNetworkWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DNetworkWidget(QWidget * parent = 0, Qt::WindowFlags f = 0 );

    void buildTranslation();
    void setAddCheckBoxChecked(bool bChecked);

signals:

public slots:
    void setWifiEnabled(bool enable);
    void updAvailableWifiListView();
    void updConnectedWifiInfo();

protected slots:
    void onWifiRefreshBtnClicked();
    void onGetNetInfoBtnClicked();
    void onWifiListWidgetItemClicked(QListWidgetItem *item);
    void onAddCheckBoxStateChanged(int state);
    void onAddSSIDBtnClicked();

private:
    void init();
    void initUI();

private:
    QWidget     *m_pWifiConfigWidget;
    QPushButton *m_pRefreshWifiBtn;
    QPushButton *m_pGetNetInfoBtn;
    QCheckBox   *m_pAddCheckBox;

    QPushButton *m_pAddSSIDBtn;
    QLabel      *m_pSSIDLab;
    DLineEdit   *m_pSSIDEdit;

    QListWidget *m_pWifiMsgListWidget;
    QWidget     *m_pWifiSSIDAddWidget;

    bool      m_bRefreshed;
    bool      m_bWifiEnabled;

    QString               m_connectedWifi; //当前连接的wifi信息
    QSharedPointer<DWifiInfo> m_pWifiInfo;
};

void deleteWifiInfo(DWifiInfo *obj);

#endif // DNETWORKWIDGET_H
