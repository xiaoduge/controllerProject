#ifndef DNETWORKPAGE_H
#define DNETWORKPAGE_H

#include "subpage.h"
#include "Display.h"

class MainWindow;
class QCheckBox;
class CBitmapButton;
class DNetworkWidget;

class DNetworkPage : public CSubPage
{
    Q_OBJECT

public:
    enum NETWORKPAGE_BTN_ENUM
    {
       NETWORKPAGE_BTN_SAVE = 0,
       NETWORKPAGE_BTN_NUM
    };

public:
    explicit DNetworkPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);
    virtual ~DNetworkPage();

    virtual void creatTitle();
    virtual void switchLanguage();
    virtual void buildTranslation();
    virtual void initUi();
    virtual void leaveSubPage();
    virtual void update();

    bool wifiCheckState();
    void showSaveBtn(bool iShow);

    void buildTitles();
    void save();
    void setBackColor();

signals:
    void wifiEnabled(bool);

protected slots:
    void onBtnClicked(int iIndex);
    void onCheckBoxChangeState(int state);

protected:
    int             m_iNetworkMask;

private:
    QLabel         *m_laName[DISPLAY_NETWORK_NUM];
    QCheckBox      *m_chkSwitchs[DISPLAY_NETWORK_NUM];
    QWidget        *m_pBackWidget[DISPLAY_NETWORK_NUM];
    QString         m_astrNetName[DISPLAY_NETWORK_NUM];
    QString         m_strQss4Chk;
    DNetworkWidget *m_pNetworkWidget;
    CBitmapButton  *m_pBtnSave;
};


#endif // DNETWORKPAGE_H
