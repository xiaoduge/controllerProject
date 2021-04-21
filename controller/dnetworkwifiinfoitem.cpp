#include "dnetworkwifiinfoitem.h"
#include <QLabel>
#include <QHBoxLayout>

QString DNetworkWifiInfoItem::signalLevelPixmap[SignalLevel_Num] =
{
    ":/wifiinfo/Signal_Level_1.png",
    ":/wifiinfo/Signal_Level_2.png",
    ":/wifiinfo/Signal_Level_3.png",
    ":/wifiinfo/Signal_Level_4.png",
    ":/wifiinfo/Signal_Level_5.png",
};

DNetworkWifiInfoItem::DNetworkWifiInfoItem(const QString name, double fLevel, bool connected, QWidget *parent)
    : QWidget(parent),
      m_strWifiName(name),
      m_fLevel(fLevel),
      bConnected(connected)
{
    initUI();
}

const QString DNetworkWifiInfoItem::wifiName() const
{
    return m_pWifiNameLab->text();
}

void DNetworkWifiInfoItem::initUI()
{
    m_pSignalLevelLab = new QLabel(this);
    QPixmap pixmap(signalLevelPixmap[level() - 1]);
    m_pSignalLevelLab->setPixmap(pixmap.scaled(24, 21, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    m_pWifiNameLab = new QLabel(m_strWifiName, this);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(120);
    layout->addWidget(m_pSignalLevelLab);
    layout->addWidget(m_pWifiNameLab);
    if(bConnected)
    {
        m_pConnectedLab = new QLabel(this);
        m_pConnectedLab->setPixmap(QPixmap(":/wifiinfo/connected.png").scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        layout->addWidget(m_pConnectedLab);
    }
    layout->addStretch();
    this->setLayout(layout);


}

int DNetworkWifiInfoItem::level()
{
    int iLevel = static_cast<int>(m_fLevel * static_cast<int>(SignalLevel_Num) + 0.5);

    if(iLevel > 5)
    {
        iLevel = 5;
    }
    if(iLevel < 1)
    {
        iLevel = 1;
    }

    return  iLevel;
}
