#include "dinitnetworkpage.h"
#include "mainwindow.h"
#include <QMouseEvent>
#include "exconfig.h"

DInitNetworkPage::DInitNetworkPage(QObject *parent, CBaseWidget *widget, MainWindow *wndMain): DNetworkPage(parent, widget, wndMain)
{
    initUi();
    buildTranslation();
    this->hideTitleBar();
}

void DInitNetworkPage::buildTranslation()
{
    m_pBackBtn->setText(tr("Back"));
    m_pNextBtn->setText(tr("Next"));
    DNetworkPage::buildTranslation();
}

void DInitNetworkPage::initUi()
{
    m_pBackBtn = new QPushButton(m_widget);
    m_pNextBtn = new QPushButton(m_widget);

    connect(m_pNextBtn, SIGNAL(clicked()), this, SLOT(onNextBtnClicked()));
    connect(m_pBackBtn, SIGNAL(clicked()), this, SLOT(onBackBtnClicked()));

    m_pBackBtn->move(200, 520);
    m_pNextBtn->move(500, 520);
    showSaveBtn(false);
}

void DInitNetworkPage::leaveSubPage()
{
    if (m_iNetworkMask != gGlobalParam.MiscParam.iNetworkMask)
    {
        DISP_MISC_SETTING_STRU  MiscParam = gGlobalParam.MiscParam;
        MiscParam.iNetworkMask = m_iNetworkMask;
        MainSaveMiscParam(gGlobalParam.iMachineType,MiscParam);
        MainUpdateSpecificParam(NOT_PARAM_MISC_PARAM);
    }
    DNetworkPage::leaveSubPage();
}

void DInitNetworkPage::onNextBtnClicked()
{
    save(); //next clicked save

    m_wndMain->naviInitPage(Ex_Init_Network, 0);
    m_wndMain->prepareKeyStroke();
}

void DInitNetworkPage::onBackBtnClicked()
{
    m_wndMain->naviInitPage(Ex_Init_Network, 1);
    m_wndMain->prepareKeyStroke();
}

void DInitNetworkPage::mousePressEvent(QMouseEvent *e)
{
    if (!m_lstFlag)
    {
        m_lstX = e->x();
        m_lstY = e->y();
        m_curX = e->x();
        m_curY = e->y();
        m_lstFlag = 1;
    }
}

void DInitNetworkPage::mouseMoveEvent(QMouseEvent *e)
{
    if (0 == e->x()
        && 0 == e->y())
    {
       return;
    }

    m_curX = e->x();
    m_curY = e->y();
}

void DInitNetworkPage::mouseReleaseEvent(QMouseEvent *e)
{
    if (abs(m_curX - m_lstX) >= PAGE_X_DIMENSION
        && abs(m_curY - m_lstY) <= PAGE_Y_DIMENSION)
    {
        save();
        m_wndMain->naviInitPage(Ex_Init_Network, m_curX - m_lstX > 0 ? 1 : 0);
    }
    m_lstFlag = 0;
}



