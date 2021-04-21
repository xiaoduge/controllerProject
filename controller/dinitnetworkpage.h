/****************************************************************************
**
** 初始化界面设置网络
**
****************************************************************************/
#ifndef DINITNETWORKPAGE_H
#define DINITNETWORKPAGE_H

#include "dnetworkpage.h"

class DInitNetworkPage : public DNetworkPage
{
    Q_OBJECT
public:
    DInitNetworkPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);

    void buildTranslation();
    void initUi();
    void leaveSubPage();

private slots:
    void onNextBtnClicked(); 
    void onBackBtnClicked(); 

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    QPushButton* m_pNextBtn; 
    QPushButton* m_pBackBtn; 

};

#endif // EX_INIT_NETWORKPAGE_H
