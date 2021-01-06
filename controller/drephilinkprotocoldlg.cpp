/*
 * @Author: dcj
 * @Date: 2020-12-15 15:13:56
 * @LastEditTime: 2020-12-15 15:18:04
 * @Description: RephiLink Service Privacy Policy Dialog
 */
#include "drephilinkprotocoldlg.h"
#include "Display.h"
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextBrowser>
#include <QPushButton>
#include <QCheckBox>
#include <QFile>
#include <QApplication>
#include <QStyle>
#include <QDebug>

DRephiLinkProtocolDlg::DRephiLinkProtocolDlg(QWidget *parent, Qt::WindowFlags f):QDialog(parent, f)
{
    initUI();
    initProtocol();
}

DRephiLinkProtocolDlg::~DRephiLinkProtocolDlg()
{
    qDebug() << "~DRephiLinkProtocolDlg()";
}

void DRephiLinkProtocolDlg::onAgreeBtnClicked()
{
    if(m_pHaveReadChk->checkState() != Qt::Checked)
    {
        qDebug() << "Please read the above terms carefully";
        return;
    }
    this->accept();
}

void DRephiLinkProtocolDlg::onDisagreeBtnClicked()
{
    this->reject();
}

void DRephiLinkProtocolDlg::initUI()
{
    this->resize(600, 400);
    this->setWindowTitle(tr("RephiLink Service Privacy Policy"));
    this->setWindowIcon(QApplication::style()->standardIcon(QStyle::SP_VistaShield));

    //init Control
    m_pTextBrower = new QTextBrowser;
    m_pTextBrower->setFocusPolicy (Qt::NoFocus);

    m_pHaveReadChk = new QCheckBox(tr("I have read and agree to the terms contained in the Service Privacy Policy"));
    QString strQss4Chk = gpMainWnd->getQss4Chk();
    m_pHaveReadChk->setStyleSheet(strQss4Chk);

    m_pAgreeBtn = new QPushButton(tr("Agree"));
    m_pDisagreeBtn = new QPushButton(tr("Disagree"));
    m_pAgreeBtn->setFixedSize(100, 30);
    m_pDisagreeBtn->setFixedSize(100, 30);

    //init layout
    QHBoxLayout *chkLayout = new QHBoxLayout;
    chkLayout->addWidget(m_pHaveReadChk);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(m_pDisagreeBtn);
    btnLayout->addWidget(m_pAgreeBtn);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(m_pTextBrower);
    vLayout->addLayout(chkLayout);
    vLayout->addLayout(btnLayout);

    this->setLayout(vLayout);

    //init connect
    connect(m_pAgreeBtn, SIGNAL(clicked()), this, SLOT(onAgreeBtnClicked()));
    connect(m_pDisagreeBtn, SIGNAL(clicked()), this, SLOT(onDisagreeBtnClicked()));
}

void DRephiLinkProtocolDlg::initProtocol()
{
    QFile file;
    int code = 1;

    switch(gGlobalParam.MiscParam.iLan)
    {
    case APP_LAN_CHN:
        file.setFileName(":/html/rephilink/rephilink_cn.html");
        break;
    default:
        file.setFileName(":/html/rephilink/rephilink_en.html");
        break;
    }

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray content = file.readAll();
        m_pTextBrower->setHtml(content);
        file.close();
    }
    else
    {
        qDebug() << "Open Protocol File Failed";
    }
}
