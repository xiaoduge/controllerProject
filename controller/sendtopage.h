#ifndef SENDTOPAGE_H
#define SENDTOPAGE_H

#include "subpage.h"
#include "Display.h"


class MainWindow;
class QCheckBox;
class CBitmapButton;


#define RECEIVER 3

enum SENDPAGE_BTN_ENUM
{
    SENDPAGE_BTN_CANCEL = 0,
    SNEDPAGE_BTN_OK
};

class SendToPage : public CSubPage
{
    Q_OBJECT
public:
    SendToPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);
    virtual void creatTitle();

    virtual void switchLanguage();

    virtual void buildTranslation();

    void buildTitles();

    virtual void initUi();


private:
    void setBackColor();

    void copyAlarmFile();
    void copyGetWater();
    void copyProduceWater();
    void copyLog();

#ifdef WATERCARDREADER
    void copyWaterCardInfo();
#endif
    void copyHistoryToUsb();

private:
    QLabel *lbTitle;

    QLabel *lbPic[RECEIVER];
    QLabel *lbName[RECEIVER];

    QCheckBox     *m_chkSwitchs[RECEIVER];

    CBitmapButton *CancelBtn;
    CBitmapButton *SaveBtn;

    QWidget       *m_pSetWidget[RECEIVER];

public slots:
    void on_checkBox_changeState(int state);
    void on_btn_clicked(int tmp);

};

#endif // SENDTOPAGE_H
