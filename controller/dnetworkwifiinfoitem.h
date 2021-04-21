#ifndef DNETWORKWIFIINFOITEM_H
#define DNETWORKWIFIINFOITEM_H

#include <QWidget>

class QLabel;

class DNetworkWifiInfoItem : public QWidget
{
    Q_OBJECT
public:
    explicit DNetworkWifiInfoItem(const QString name, double fLevel, bool connected = false, QWidget *parent = 0);

    enum SignalLevel
    {
        SignalLevel_1,
        SignalLevel_2,
        SignalLevel_3,
        SignalLevel_4,
        SignalLevel_5,
        SignalLevel_Num
    };

    const QString wifiName() const;

signals:

public slots:

private:
    void initUI();

    int level();

private:
    QLabel *m_pSignalLevelLab;
    QLabel *m_pWifiNameLab;
    QLabel *m_pConnectedLab;
    QString m_strWifiName;
    double   m_fLevel;
    bool   bConnected;


    static QString signalLevelPixmap[SignalLevel_Num];
};

#endif // DNETWORKWIFIINFOITEM_H
