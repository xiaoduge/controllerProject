#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QTimer>

#include "DefaultParams.h"
#include "zbcomm.h"
#include "Display.h"
#include "MyParams.h"
#include "cminterface.h"
#include "eco_w.h"

#include "dinithandlecfgpage.h"
#include "dcalcpackflow.h"

#include <QThread>
#include "DNetworkConfig.h"
#include <QProcess>

#define FLOWCHART
#define D_HTTPWORK

//#define TOUCHTEST

#define PAGEID_MARGIN (4)

#define PAGE_X_DIMENSION (40)
#define PAGE_Y_DIMENSION (40)

enum GLOBAL_COMPANY
{
    COMPANY_REPHILE = 0,
    COMPANY_VWR,
    COMPANY_NUM
};


enum GLOBAL_BMP_ENUM
{
    GLOBAL_BMP_HOME = 0,
    GLOBAL_BMP_RUN,
    GLOBAL_BMP_STOP,
    GLOBAL_BMP_POWER_OFF_ACTIVE,
    GLOBAL_BMP_POWER_OFF_INACT,
    GLOBAL_BMP_BAR_SEPERATOR,
    GLOBAL_BMP_NAVIGATOR_BG,
    GLOBAL_BMP_NAVIGATOR_TIMER_BAR,
    GLOBAL_BMP_BTN_GENERAL_ACTIVE,
    GLOBAL_BMP_BTN_GENERAL_INACTIVE,
    GLOBAL_BMP_PAGE_SELECT,
    GLOBAL_BMP_PAGE_UNSELECT,
    GLOBAL_BMP_BACK,
    GLOBAL_BMP_SWITCH_ON,
    GLOBAL_BMP_SWITCH_OFF,
    GLOBAL_BMP_PAGE_NAVI_NORMAL,
    GLOBAL_BMP_PAGE_NAVI_ACTIVE,
    GLOBAL_BMP_CANEL_NORMAL,
    GLOBAL_BMP_CANEL_ACTIVE,
    GLOBAL_BMP_OK_NORMAL,
    GLOBAL_BMP_OK_ACTIVE,
    GLOBAL_BMP_SEND_NORMAL,
    GLOBAL_BMP_SEND_ACTIVE,
    GLOBAL_BMP_BTN_GENERAL_30_ACTIVE,
    GLOBAL_BMP_BTN_GENERAL_30_INACTIVE,
    GLOBAL_BMP_CS_NORMAL,               /*indicator for consumable material state */
    GLOBAL_BMP_CS_NOTIFICATION,         /*indicator for consumable material state */
    GLOBAL_BMP_SHOPPING_CART,
    GLOBAL_BMP_REMIND_ICON,
    GLOBAL_BMP_ALARM_ICON,
    GLOBAL_BMP_DEVICE_ON,
    GLOBAL_BMP_DEVICE_OFF,
    GLOBAL_BMP_NUM
};

enum
{
#ifdef FLOWCHART
    PAGE_FLOWCHART = 0,
    PAGE_MAIN,
#else
    PAGE_MAIN = 0,
#endif
    PAGE_MENU,
    PAGE_SERVICE,
//    PAGE_SET,
    PAGE_NUM
};

enum   //ex_dcj
{
    Ex_Init_Lan = 0,
    Ex_Init_Time,
    Ex_Init_Tankcfg,
    Ex_Init_Syscfg,
    Ex_Init_Network,
    Ex_Init_InstallConsumable,
    Ex_Init_Handlercfg,
    Ex_Init_Num
};

enum GLOBAL_FONT
{
    GLOBAL_FONT_12 = 0,
    GLOBAL_FONT_14 ,
    GLOBAL_FONT_24,
    GLOBAL_FONT_30,
    GLOBAL_FONT_40,
    GLOBAL_FONT_48,
    GLOBAL_FONT_60,
    GLOBAL_FONT_NUM
};

enum SETPAGE_NAME
{
    SETPAGE_MAINTAIN_PERIOD = 0,
    SETPAGE_ALARM_SETTING,
    SETPAGE_SYSTEM_TEST,
    SETPAGE_SYSTEM_PARAMETER_CALIBRATE,
    SETPAGE_SYSTEM_PARAM_CONFIG,
    SETPAGE_SYSTEM_DEVICE_CONFIG,
    SETPAGE_SYSTEM_DISPLAY,
    SETPAGE_SYSTEM_NETWORK,
    SETPAGE_SYSTEM_ALLOCATION,
    SETPAGE_SYSTEM_TIME,
    SETPAGE_SYSTEM_LANGUAGE,
    SETPAGE_SYSTEM_SOUND,
    SETPAGE_SYSTEM_UNIT,
    SETPAGE_PERIPHERAL_DEVICE_MANAGER,
    SETPAGE_RFID_CONFIG,
    SETPAGE_INSTALL_PERMISSION,
    SETPAGE_NUMBER
};

enum CONSUMABLE_CATNO
{
    PREPACK_CATNO = 0,
    ICPPACK_CATNO,
    ACPACK_CATNO,
    PPACK_CATNO,
    UPACK_CATNO,
    HPACK_CATNO,
    ATPACK_CATNO,
    CLEANPACK_CATNO,

    TPACK_CATNO,
    ROPACK_CATNO,
    UV185_CATNO,
    UV254_CATNO,
    UVTANK_CATNO,
    ROPUMP_CATNO,
    UPPUMP_CATNO,
    FINALFILTER_A_CATNO,
    FINALFILTER_B_CATNO,
    EDI_CATNO,
    TANKVENTFILTER_CATNO,
    LOOPFILTER_CATNO,
    LOOPUV_CATNO,
    LOOPDI_CATNO,
    CAT_NUM
};

typedef struct config_btn {
    int xPos, yPos;
    char *strBackBitmapPath;
    char *strFrontBitmapPath;
    int enStyle; /* refer BITMAPBUTTON_STYLE_ENUM */
    int enPicStyle; /* refer BITMAPBUTTON_PIC_STYLE_ENUM */
    int attr;
}CONFIG_BTN;

typedef struct config_btn1 {
    int xPos, yPos;
    QPixmap **pBackBitmap;
    QPixmap **pFrontBitmap;
    int enStyle;    /* refer BITMAPBUTTON_STYLE_ENUM     */
    int enPicStyle; /* refer BITMAPBUTTON_PIC_STYLE_ENUM */
    int attr;
}CONFIG_BTN1;

typedef struct  {
    QPixmap **pBackBitmap;
    int enStyle ; /* BITMAPBUTTON_ICON_ENUM */
}CONFIG_BTN_TIP;


typedef struct config_label {
    int xPos, yPos;
    int Width, Height;
    int enFont; /* refer LABEL_FONT */
    QColor colorText;
    char *strBackBitmapPath;
    char * str;
    int  parentBtn;
    int attr;
}CONFIG_LABEL;

namespace Ui {
class MainWindow;
}

class navigatorBar;

class SetDevicePage;
class DCheckConsumaleInstall;
class DConsumableInstallDialog;
class QTcpSocket;
class QNetworkReply;
class DNetworkAccessManager;
class QSslError;
class DWifiConfigDialog;
class QFileSystemWatcher;
class DCardInfoDialog;

class DispenseDataPrint;
class ProductDataPrint;
class AlarmPrint;
class ServiceLogPrint;

typedef struct
{
     unsigned int ulMask4Normlwork;
     unsigned int ulMask4Cleaning;
     int          aiDeviceType4Normal[APP_RF_READER_MAX_NUMBER];
     int          aiDeviceType4Cleaning[APP_RF_READER_MAX_NUMBER];
}MACHINE_RFID_MAP_STRU;

typedef struct
{
    unsigned int aulMask[DISP_ALARM_PART_NUM];
}MACHINE_ALARM_SETTING_STRU;

typedef struct
{
    unsigned int aulMask[1]; /*DISP_CM_NAME_NUM*/
}MACHINE_NOTIFY_SETTING_STRU;

#define METHOD_PROLOGUE(theClass, localClass) \
    theClass* pContainer = ((theClass*)((char*)(this) - \
    offsetof(theClass, m_x##localClass)));

struct DUserInfo
{
    QString m_strUserName;
    QString m_strPassword;
};

struct WaterCardInfo
{
    bool         bValid;
    QByteArray   cardID;
    QByteArray   type;
    unsigned int amount;

    unsigned int curVolume;
    unsigned int oldVolumne;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QMainWindow *parent = 0);
    ~MainWindow();
    void clearIdleSecond();
    
#ifdef STEPPERMOTOR
    friend int GetSpeedValue(int iIdx);
#endif
    friend void DispIndicationEntry(unsigned char *pucData,int iLength);
    friend void DispIapIndEntry(IAP_NOTIFY_STRU *pIapNotify);

    void emitDispIndication(unsigned char *pucData,int iLength);
    void emitIapIndication(IAP_NOTIFY_STRU *pIapNotify);

    //2019.10.15 add, for sub-account
    void emitUserLogin();

    void setLokupState(bool state) {m_bLockupDlg = state;}
    void AfDataMsg(IAP_NOTIFY_STRU *pIapNotify);
    void zigbeeDataMsg(IAP_NOTIFY_STRU *pIapNotify);

    void alarmCommProc(bool bAlarm,int iAlarmPart,int iAlarmId);
    
    QWidget * getMainWidget() {return mainWidget;}

    void run(bool bRun);

    void home();

    QFont* getFont(int iType) {return m_pFonts[iType];}
    
    void addPage(CPage *page) {m_pageList << page;}

    void naviPage(int iCurPage,int iDir);
    void naviInitPage(int iCurPage,int iDir);

    void saveLoginfo(const QString& strUserName, const QString& strPassword = "unknow");
    const DUserInfo getLoginfo();

    void MainWriteLoginOperationInfo2Db(int iActId);
    
    void MainWriteCMInstallInfo2Db(int iActId,int iItemIdx,CATNO cn,LOTNO ln);
    void MainWriteMacInstallInfo2Db(int iActId,int iItemIdx,CATNO cn,LOTNO ln);

    void changeWaterQuantity(int iType,bool bAdd,float fValue);
    
    void setWaterQuantity(int iType,float fValue);

    void startCir(int iCirType);

    void changeRPumpValue(int iIdx,int iValue);

    int  getRPumpValue(int iIdx){return m_aiRPumpVoltageLevel[iIdx];}

    float getWaterQuantity(int iType){ return m_afWQuantity[iType];}
    
    void startQtw(int iType,bool bQtw = true);

    
    void updHandler(int iMask,DB_HANDLER_STRU *hdls);
    void delHandler(SN name);

    void updRfReader(int iMask,DB_RFID_STRU *hdls);

    QPixmap    *getBitmap(int id);

    void setCurrentPage(CPage *page){m_pCurPage = page;}
    
    DISPHANDLE startClean(int iType,bool bStart);
    
    SetDevicePage *getDeviceDlg();
    DInitHandleCfgpage *getExInitPage(); //ex-dcj

    QString& getQss4Chk() {return m_strQss4Chk;}

    QString& getQss4Table() {return m_strQss4Tbl;}

    QString& getQss4Cmb() {return m_strQss4Cmb;}

    void switchLanguage();
    void saveAlarmSP();
    void updateTubeCirCfg();

    
    int getAlarmState();

    int getTankLevel() {return m_iLevel;}

    int getEventCount() {return m_periodEvents;}
        
    int getAlarmInfo(int iPart ,int id) {return !!(m_iAlarmRcdMask[0][iPart] & (1 << id));}

    QString& getDbName(int iDbType) {return m_astrDbName[iDbType];}
        
    DB_RFID_STRU * getRfid(SN sn);
    DB_HANDLER_STRU * getHandler(SN sn);
    DB_HANDLER_STRU * getHandler(void);
    DB_RFID_STRU * getRfid(int addr);
    DB_HANDLER_STRU * getHandler(int addr);
    DB_HANDLER_STRU * getDefaultHandler(int iType);
    ECO_INFO_STRU * getEco(int index);

    RF_DATA_LAYOUT_ITEM *getRfRegion(int iRegionId) {return &m_RfDataItems.aItem[iRegionId];}

    void initRfIdLayout(RF_DATA_LAYOUT_ITEMS *pLayout);

    void goAlarm();

    void setWorkMode(int iMode) {m_eWorkMode = iMode;}
    
    int getWorkMode(void) {return m_eWorkMode;}

    bool getRfidState(int iRfId) {return !!m_aRfidInfo[iRfId].ucValid;}

    int  readRfid(int iRfId);

    bool readRfid_Reader(int iRfId);

    void deduction(unsigned int vol);
    void cardReadyBeep();
    void recordCardInfo();
    int checkCardReaderStatus();

    void getRfidCatNo(int iRfId,CATNO sn) {m_aRfidInfo[iRfId].getCatNo(sn);}

    void getRfidLotNo(int iRfId,LOTNO sn) {m_aRfidInfo[iRfId].getLotNo(sn);}

    void updateCMInfoWithRFID(int operate);
    int writeRfid(int iRfId, int dataLayout, QString strData);
    void getRfidInstallDate(int iRfId, QDate* date) {m_aRfidInfo[iRfId].getInstallDate(date);}
    void getRfidVolofUse(int iRfId, int& vol) {m_aRfidInfo[iRfId].getVolUsed(vol);}

    void readCMInfoFromRFID(int iRfId, int type);
    void writeCMInfoToRFID(int iRfId, int type);

    bool updateExConsumableMsg(int iMachineType,CATNO cn,LOTNO ln,int iIndex, int category, QDate& date, int iRfid, bool bRfidWork = false);
    const QDate resetExConsumableMsg(QDate& date, int iRfid, int iType, bool bRfidWork = false);
    const QString& consumableInitDate() const;

    int getActiveExeBrds() { return m_iExeActiveMask ? 1 : 0;}
    int getActiveFmBrds() { return m_iExeActiveMask ? 1 : 0;}

    int getActiveHandlerBrds() ;
    int getHandlerMask();

    int getActiveRfidBrds() ;
    int getMachineAlarmMask(int iMachineType,int iPart);
    int getMachineNotifyMask(int iMachineType,int iPart);

    //Check if the consumable life is zero
    void checkCMParam();

    void prepareKeyStroke();

    int getActiveRfidBrds4Cleaning() ;

    void ClearToc();

    void updateRectState();
    void updateRectAlarmState();

    void initMachineFlow();
    void calcFlow(int state);
    void updatePackFlow();

    void checkConsumableInstall(int iRfId);

    void checkUserLoginStatus();
    void updateRunningFlushTime();
    int runningFlushTime();
    /* for all kinds of state related measurements */
    float        m_fSourceWaterPressure;
    float        m_fSourceWaterConductivity;

    //Static Public Members
    static QStringList consumableCatNo(CONSUMABLE_CATNO iType);

    void emitUnitsChanged();

    void restart();

    void setStartCheckConsumable(bool isStart);

    void stopBuzzing();

    const QString& machineName();
    void initAlarmName();
    const QString alarmName(int iIndex) const;

public slots:
    void retriveLastRunState();
    void onScreenPageHide();

private slots:
    void on_timerEvent();
    void on_timerPeriodEvent();
    void on_timerSecondEvent();
    void on_timerScreenSleepEvent();

    void on_dispIndication(unsigned char *pucData,int iLength);

    //2019.10.15 add, sub-account
    int on_userLogin();

    void on_Stop_clicked();

    void on_Exit_clicked();
    void on_pbRun_clicked();
    void on_AutoLogin(void);
    void on_IapIndication(IAP_NOTIFY_STRU *pIapNotify);

    void on_btn_clicked(int tmp);
    void on_Gif_State_Change();
    void on_timerBuzzerEvent();

    void buildTranslation();

    void on_Ex_ScreenPageHide();

    //void Test();

signals:
    void dispIndication(unsigned char *pucData,int iLength);
    void autoLogin(void);
    void iapIndication(IAP_NOTIFY_STRU *pIapNotify);

    void SleepPageShow(bool); //ex
    void unitsChanged();
    void updateFlowChartAlarm(const QString &msg, bool isAdd);

    //2019.10.15 add, for sub-account
    void userNeedLogin();

protected:
    void mousePressEvent(QMouseEvent *e);

private:
    //Ui::MainWindow *ui;
    class CAlarm 
    {
    public:
        int m_iAlarmPeriod;
        int m_iAlarmDuty;
        int m_iAlarmNum;
        bool m_bDuty;
        bool m_bActive;
    }Alarm;

	void printSystemInfo();
	void initGlobalStyleSheet();
	void initConsumablesInfo();
	void initAlarmCfg();
	void initConsumablesCfg();
	void initRFIDCfg();
    void checkDateTime();
    void initUI();

    
    void Splash();
    
    void mainDisplay();

    void updEcoInfo(int iIdx);

    void updTank();
    void updSourceTank();
    
    void updQtwState(int iType,bool bStart);

    void initHandler();

    void initRfid();

    void saveHandler();
    
    void deleteHandler(SN name);

    void saveRfid();
    
    void setRelay(int iIdx,int iEnable);
    
    void buzzerHandle();
    void startBuzzer();
    void stopBuzzer();
    void prepareAlarmNormal();
    void prepareAlarmAlarm();
    void prepareAlarmFault();
    void stopAlarm();
    void updFlowInfo(int iIdx);
    
    void updPressure(int iIdx);

    void addRfid2DelayList(int iRfId);

    void rmvRfidFromDelayList(int iRfId);

    void saveFmData(int id,unsigned int ulValue);

    //2019.10.15 add, for sub-account
    void doSubAccountWork(double value, int iType);

	void initMachineName();
	void initMachineNameRephile();
    void initMachineNameVWR();
    //
public:
    void showWifiConfigDlg(const QString& name);
#ifdef D_HTTPWORK
    void emitHttpAlarm(const DNetworkAlaramInfo &alarmInfo);
    void checkConsumableAlarm();

signals:
    void sendHttpHeartData(const DNetworkData&);
    void sendHttpAlarm(const DNetworkAlaramInfo&);
	void sendDispenseData(const DDispenseData&);

private:
    void initHttpWorker();
    void initMqtt();
    void publishMqttMessage(const QByteArray& msg);
    bool isDirExist(const QString& fullPath);
    void updateNetworkFlowRate(int iIndex, int iValue);

private slots:
    void on_updateText(const QByteArray&);
    void on_timerNetworkEvent();
    void on_mqttWatcher_fileChanged(const QString& fileName);
    void on_mqttProcess_started();
    void on_mqttProcess_finished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QThread m_workerThread;
    QTimer *m_networkTimer;

    DNetworkData m_uploadNetData;
    bool m_conAlarmMark[HTTP_NOTIFY_NUM];

    QFileSystemWatcher *m_pFileWatcher;
    QProcess m_mqttProcess;
    bool mqttProcessRun;
    int mqttNum;

	int excepCounter; //系统状态异常次数

    unsigned char m_aucContent[256];
    WaterCardInfo m_WaterCardInfo;
    //
#endif
private:
    DWifiConfigDialog *m_pWifiConfigDlg;
    DCardInfoDialog   *m_pCardInfoDlg;

private:
    QTimer* m_timerBuzzer;
    
    QWidget *mainWidget;

    CBaseWidget *m_pSubWidget[PAGE_NUM];
    CPage *m_pSubPages[PAGE_NUM];
    //ex_dcj
    CBaseWidget* m_pExInitWidgets[Ex_Init_Num];
    CPage* m_pExInitPages[Ex_Init_Num];
    CPage* m_curExInitPage;

    CBaseWidget* m_pScreenSleepWidget;
    CPage* m_pScreenSleepPage;
    CPage* m_pPreviousPage;
    //end


    bool m_bSplash;
    int  m_curPageIdx;

    CPage *m_pCurPage;

    QList<CPage *>     m_pageList;
    
    QLabel *m_pLabelGif;
    QMovie *m_pMovieGif;

    int iFirst ;

    QTimer* m_timeTimer;
    QTimer* m_timerPeriodEvent;
    QTimer* m_timeSecondTimer;
    QTimer* m_screenSleepTimer;
    
    int  m_periodEvents;

    int  m_fd4buzzer;

    bool m_bC1Regulator;

    unsigned int m_ulFlowMeter[APP_FM_FLOW_METER_NUM];
    unsigned int m_ulLstFlowMeter[APP_FM_FLOW_METER_NUM];
             int m_iLstFlowMeterTick[APP_FM_FLOW_METER_NUM];
             int m_ulFlowRptTick[APP_FM_FLOW_METER_NUM];

    float m_fPressure[APP_EXE_PRESSURE_METER];
    ECO_INFO_STRU m_EcoInfo[APP_EXE_ECO_NUM];

    float m_fRectifier[APP_EXE_RECT_NUM];

    float m_fEDI[APP_EXE_EDI_NUM];

    
    float m_fPumpV[APP_EXE_G_PUMP_NUM];

    float m_fRPumpV[APP_EXE_R_PUMP_NUM];
    float m_fRPumpI[APP_EXE_R_PUMP_NUM];

    QPalette mQPALARM;
    QFont mQFALARM ;

    ECO_W       m_EcowOfDay[APP_EXE_ECO_NUM];
    ECO_W       m_EcowCurr[APP_EXE_ECO_NUM];
    int         m_curToc;

    int         m_nTimerId;

    bool        m_bLoged;

    QString     m_strLstTime;

    int         m_iLstDay;

    QFont       *m_pFonts[GLOBAL_FONT_NUM];

    int         m_iAlarmRcdMask[2][DISP_ALARM_PART_NUM]; /* array 0 for fired alarm, array 1 for restored alarm */

    MACHINE_ALARM_SETTING_STRU  m_aMas[MACHINE_NUM];
    MACHINE_NOTIFY_SETTING_STRU m_cMas[MACHINE_NUM];
    
    bool        m_bLockupDlg;

    /* for tube circulation */
    bool         m_bTubeCirFlags; /* circulation in progress flag */
    int         m_iStartMinute;
    int         m_iEndMinute;
    int         m_iTubeCirCycle; 
    
    float       m_afWQuantity[APP_DEV_HS_SUB_NUM]; /* unit: 100ml */

    int         m_aiRPumpVoltageLevel[APP_EXE_INPUT_REG_PUMP_NUM];

    DB_HANDLER_STRU m_aHandler[APP_HAND_SET_MAX_NUMBER];
    int             m_iHandlerMask;
    int             m_iHandlerActiveMask;
    
    DB_RFID_STRU m_aRfid[APP_RF_READER_MAX_NUMBER];
    int          m_iRfidMask;
    int          m_iRfidActiveMask;

    int          m_iRfidBufferActiveMask;
    int          m_iRfidDelayedMask;
    int          m_aiRfidDelayedCnt[APP_RF_READER_MAX_NUMBER];

    int          m_iExeActiveMask;
    int          m_iFmActiveMask;

    int          m_eWorkMode;     /* refer APP_WORK_MODE_CLEAN */

    QString      m_strQss4Chk;

    QString      m_strQss4Tbl;

    QString      m_strQss4Cmb;

    QString      m_astrDbName[DB_TBL_NUM];

    int          m_iNotState;   /* for notifying handsets */

    int          m_iLevel; 

    RF_DATA_LAYOUT_ITEMS m_RfDataItems;
    
    DUserInfo  m_userInfo;

    static QStringList m_strConsuamble[CAT_NUM];

    QString m_consuambleInitDate; //Used to determine if it is a new consumable

	QString m_strMachineName;

    QStringList m_AlarmName;
    int m_alarmNameLen;

    class RFIDPackInfo
    {
    public:
        unsigned char         ucValid; /* content valid flag */
        unsigned char         aucContent[256];
        int                   iPackType;
        RF_DATA_LAYOUT_ITEMS *pLayout;
    
        RFIDPackInfo() {ucValid = 0;pLayout = NULL;}
    
        void getCatNo(CATNO sn)
        {
            int iLoop;

            char *src = (char *)&aucContent[pLayout->aItem[RF_DATA_LAYOUT_CATALOGUE_NUM].offset];
            char tsn[(APP_CAT_LENGTH + 3)/4*4];

            int iRu = ((APP_CAT_LENGTH + 3)/4)*4;
            
            for (iLoop = 0; iLoop < iRu; iLoop += 4)
            {
                tsn[iLoop]   = src[iLoop + 3];
                tsn[iLoop+1] = src[iLoop + 2];
                tsn[iLoop+2] = src[iLoop + 1];
                tsn[iLoop+3] = src[iLoop + 0];
            }
            
            memcpy(sn,tsn,APP_CAT_LENGTH);

            sn[APP_CAT_LENGTH] = 0;
        }
        void getLotNo(LOTNO sn)
        {
            int iLoop;

            char *src = (char *)&aucContent[pLayout->aItem[RF_DATA_LAYOUT_LOT_NUMBER].offset];
            char tsn[(APP_LOT_LENGTH + 3)/4*4];

            int iRu = ((APP_LOT_LENGTH + 3)/4)*4;


            for (iLoop = 0; iLoop < iRu; iLoop += 4)
            {
                tsn[iLoop]   = src[iLoop + 3];
                tsn[iLoop+1] = src[iLoop + 2];
                tsn[iLoop+2] = src[iLoop + 1];
                tsn[iLoop+3] = src[iLoop + 0];
            }
            
            memcpy(sn,tsn,APP_LOT_LENGTH);
            
            sn[APP_LOT_LENGTH] = 0;
        }

        void getInstallDate(QDate* date)
        {
            char *src = (char *)&aucContent[pLayout->aItem[RF_DATA_LAYOUT_INSTALL_DATE].offset];
            int day,mon,year;

            day = src[0];
            mon = src[1];
            year = (src[2] << 8)|src[3];

            date->setDate(year, mon, day);
        }

        void getVolUsed(int& vol)
        {
            char *src = (char *)&aucContent[pLayout->aItem[RF_DATA_LAYOUT_UNKNOW_DATA].offset];
            int num1, num2, num3;
            num1 = src[0];
            num2 = src[1];
            num3 = src[2];
            vol = num1 + num2*100 + num3*10000;
        }

        void setLayOut(RF_DATA_LAYOUT_ITEMS *pLayout) {this->pLayout = pLayout;}
    
        void parse()
        {
            CATNO sn;
            getCatNo(sn);
            QString strSn = sn;

            iPackType = DISP_CM_NAME_NUM;

            if (m_strConsuamble[PPACK_CATNO].contains(strSn))
            {
               iPackType = DISP_P_PACK;
            }
            else if(m_strConsuamble[ACPACK_CATNO].contains(strSn))
            {
                iPackType = DISP_AC_PACK;
            }
            else if (m_strConsuamble[UPACK_CATNO].contains(strSn))
            {
               iPackType = DISP_U_PACK;
            }
            else if (m_strConsuamble[ICPPACK_CATNO].contains(strSn))
            {
               iPackType = DISP_ICP_PACK;
            }
            else if (m_strConsuamble[HPACK_CATNO].contains(strSn))
            {
               iPackType = DISP_H_PACK;
            }
            else if (m_strConsuamble[PREPACK_CATNO].contains(strSn))
            {
               iPackType = DISP_PRE_PACK;
            }
            else if (m_strConsuamble[CLEANPACK_CATNO].contains(strSn))
            {
               iPackType = DISP_P_PACK | (1 <<16); /* multiplex flag */
            }
            else if (m_strConsuamble[ATPACK_CATNO].contains(strSn))
            {
               iPackType = DISP_AT_PACK ;
            }
        }
    
        int getPackType()
        {
            return iPackType;
        }
        
    }m_aRfidInfo[APP_RFID_SUB_TYPE_NUM];

    MACHINE_RFID_MAP_STRU MacRfidMap;


private:
    DCheckConsumaleInstall* m_checkConsumaleInstall[APP_RFID_SUB_TYPE_NUM];
    DConsumableInstallDialog* m_consumaleInstallDialog[APP_RFID_SUB_TYPE_NUM];
    bool m_startCheckConsumable;
    //end

    int m_flushMachineFlow;
    int m_productMachineFlow;

    int m_runningFlushTime;
//    DCalcPackFlow m_calcPFlow;

private:
    void initScreenSleep();
    void autoCirPreHour();
    //Determine the I number of HP history data need to save
    int idForHPGetWHistory();

	void startTubeCir();
	void stopTubeCir();
	void checkTubeCir();

    // for printer
    void printWorker(const DispenseDataPrint &data);
    void printWorker(const ProductDataPrint &data);
    void printWorker(const AlarmPrint &data);
    void printWorker(const ServiceLogPrint & data);
    
private slots:
    void on_ScreenSleep(bool sleep);

};

extern MainWindow *gpMainWnd;
extern MACHINE_TYPE_STRU gaMachineType[MACHINE_NUM];

extern QPixmap    *gpGlobalPixmaps[GLOBAL_BMP_NUM];
void Write_sys_int(char *sysfilename,int value);

#define PWMLCD_FILE       "/sys/class/backlight/pwm-backlight/brightness"

// 系统WIFI配置文件
#define WIFICONFIG_FILE   "/etc/wpa_supplicant.conf"

#endif // MAINWINDOW_H
