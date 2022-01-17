#ifndef SYSTEMCFGPAGE_H
#define SYSTEMCFGPAGE_H

#include "subpage.h"
#include "Display.h"

class MainWindow;
class QComboBox;
class DLineEdit;
class QCheckBox;
class CBitmapButton;

enum SYSCFGPAGE_BTN_ENUM
{
   SYSCFGPAGE_BTN_SAVE = 0,
   SYSCFGPAGE_BTN_NUM
};

class SystemCfgPage : public CSubPage
{
    Q_OBJECT

public:
    SystemCfgPage(QObject *parent = 0,CBaseWidget *widget = 0 , MainWindow *wndMain = 0);
    
    void creatTitle();
    void switchLanguage();
    void buildTranslation();
    void initUi();
    void update();

private:
    void buildTitles();
    void save();
    void setBackColor();
    void createControl();
    void connectData();

    void setDeviceType();

private:
    QLabel    *m_lbPWTankName;
    QComboBox *m_cmbPWTankVolume;
    DLineEdit *m_lePWTankHeight;
    QLabel    *m_lbPWHUnit;
    DLineEdit *m_lePWTankCap;
    QLabel    *m_lbPWCUnit;

    QLabel    *m_lbSWTankName;
    QComboBox *m_cmbSWTankVolume;
    DLineEdit *m_leSWTankHeight;
    QLabel    *m_lbSWHUnit;
    DLineEdit *m_leSWTankCap;
    QLabel    *m_lbSWCUnit;

    QCheckBox *m_chkPWTankUV;
    QLabel    *m_lbPWTankUVName;
    DLineEdit *m_lePWTankUVValue;
    QLabel    *m_lbPWTankUVUnit;

#ifdef STEPPERMOTOR
    QCheckBox *m_chkStepperMotor;
#endif

    QCheckBox *m_chkDeion;

#ifdef WATERCARDREADER
    QCheckBox *m_chkWaterCard;
#endif

#ifdef CFG_DO_PH
    QCheckBox *m_chkDO;
    QCheckBox *m_chkPH;
#endif

    QLabel    *m_lbPOweronFlushName;
    DLineEdit *m_lePOweronFlushValue;
    QLabel    *m_lbPOweronFlushUnit;

    QLabel    *m_lbLoginLingerName;
    DLineEdit *m_leLoginLingerValue;
    QLabel    *m_lbLoginLingerUnit;

    QLabel    *m_lbMaxDispTime;
    DLineEdit *m_leMaxDispTime;
    QLabel    *m_lbMaxDispTimeUnit;

    QLabel    *m_lbDeviceTypeName;
    QLabel    *m_lbDeviceType;

    QCheckBox *m_chkDeviceTypeTOC;

    CBitmapButton     *m_pBtnSave;   

    bool               m_bHaveToc;

    QLabel *m_pPureRangeLab;
    DLineEdit *m_pPureRangeEdit;
	QLabel *m_pPureRangeUnit;
	
    QLabel *m_pFeedRangeLab;
    DLineEdit *m_pFeedRangeEdit;
	QLabel *m_pFeedRangeUnit;

    QCheckBox *m_aChks[32];

    QLabel      *m_pUpdateLab;
    QPushButton *m_pUpdateBtn;

    struct CHK_ITEM_TYPE {
        int iId;
    }aCHKsIds[32];

    int m_iRealChkNum;
    
public slots:
    void on_btn_clicked(int);
    void on_CmbIndexChange_pw(int index);
    void on_CmbIndexChange_sw(int index);
    void onUpdateBtnClicked();
};

#endif // STERILIZEPAGE_H
