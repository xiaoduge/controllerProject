/*
 * @Author: dcj
 * @Date: 2020-12-15 15:13:56
 * @LastEditTime: 2020-12-15 15:20:50
 * @Description: RephiLink Service Privacy Policy Dialog
 */
#ifndef DREPHILINKPROTOCOLDLG_H
#define DREPHILINKPROTOCOLDLG_H

#include <QDialog>

class QTextBrowser;
class QPushButton;
class QCheckBox;

class DRephiLinkProtocolDlg : public QDialog
{
    Q_OBJECT
public:
    DRephiLinkProtocolDlg(QWidget *parent = 0, Qt::WindowFlags f = Qt::WindowFlags());
    ~DRephiLinkProtocolDlg();

protected slots:
    void onAgreeBtnClicked();
    void onDisagreeBtnClicked();

private:
    void initUI();
    void initProtocol();

private:
    QTextBrowser *m_pTextBrower;

    QPushButton *m_pAgreeBtn;
    QPushButton *m_pDisagreeBtn;
    QCheckBox *m_pHaveReadChk;
};

#endif // DREPHILINKPROTOCOLDLG_H
