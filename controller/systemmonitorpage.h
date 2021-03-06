#ifndef SYSTEM_MONITOR_PAGE_H
#define SYSTEM_MONITOR_PAGE_H


#include "subpage.h"
#include "systemmonitorlistwidgtitem.h"

class MainWindow;
class QListWidget;
class QListWidgetItem;
class SystemMonitorListWidgtItem;

class SystemMonitorPage : public CSubPage
{
    Q_OBJECT

public:
    SystemMonitorPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    virtual void initUi();

    virtual void update();
    virtual void fade();

    void updateSwitchInfo();

    void updateRpumpInfo(int iChl);

    void updateGpumpInfo(int iChl);

    void updateRectInfo(int iChl);

    void updateEdiInfo(int iChl);

    void updateWorkState();
    
    void updateDeviceState();

    void updateTankLevel(int level); //ex

private:

    void createList();

    void setBackColor();

    void buildTitles();

    struct SYS_MONITOR_ITEM_TYPE {

        int iType;
        int iId;
        int num;
        
    }aIds[SYSTEMPAGE_ITEM_NAME_NUM];

    QListWidget *m_pListWidget;

    QListWidgetItem *m_apListWidgetIem[SYSTEMPAGE_ITEM_NAME_NUM];

    SystemMonitorListWidgtItem *m_apSysMonitorListItem[SYSTEMPAGE_ITEM_NAME_NUM];

    unsigned int m_ulSwitchMask;

    int m_iRealNum;

    int m_tankLevel; //ex_level


public slots:
    void ItemClicked(QListWidgetItem *item);
};

#endif // SYSTEMWARNI_H
