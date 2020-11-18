/**********************************************************
 * @Author       dcj
 * @Date         2020-10-28 14:48:28
 * @Description  流程图控件，需要显示流程图，只需要在相应的界面加载此控件即可
 * @Version      V0.0.1
 **********************************************************/
#ifndef DFLOWCHARTWIDGET_H
#define DFLOWCHARTWIDGET_H

#include <QWidget>
#include <QPixmap>
#include "Display.h"

extern DISP_GLOBAL_PARAM_STRU gGlobalParam;

class DFlowChartWidget : public QWidget
{
    Q_OBJECT

    enum MACHINETYPE
    {
        GENIE_G,
        GENIE_G_HPCIR,
        GENIE_E,
        GENIE_E_HPCIR,
        GENIE_U,
        GENIE_U_TOC,
        GENIE_U_HPCIR,
        GENIE_U_TOC_HPCIR,
        GENIE_R,
        GENIE_R_HPCIR,
        GENIE_PURIST,
        GENIE_PURIST_TOC,
        GENIE_A,
        GENIE_A_TOC,
        MACHINETYPE_NUM
    };

    enum DEVICE
    {
        EDI_OFF,
        EDI_ON,
        FLOWMETER_OFF,
        FLOWMETER_ON,
        I_OFF,
        I_ON,
        P_OFF,
        P_ON,
        PACK_OFF,
        PACK_ON,
        PACK_YELLOW,
        PRE_OFF,
        PRE_ON,
        PREPACK_OFF,
        PREPACK_ON,
        PREPACK_YELLOW,
        PUMP_OFF,
        PUMP_ON,
        ROPACK_OFF,
        ROPACK_ON,
        TANK,
        TANKUV_OFF,
        TANKUV_ON,
        TANKUV_YELLOW,
        UV_OFF,
        UV_ON,
        UV_YELLOW,
        VALVE_OFF,
        VALVE_ON,
        VALVE2_OFF,
        VALVE2_C_ON,
        VALVE2_L_ON,
        VALVE2_R_ON,
        DISPER,
        DISPER_ON,
        DISPER_YELLOW,
        LIMIT_OFF,
        LIMIT_ON,
        DEVICE_NUM

    };

    enum UNIT_DISPLAY
    {
        RES_COND_UNIT,
        TEMP_UNIT,
        PRESSURE_UNIT,
        UNIT_NUM
    };

    enum WATER_QUALITY
    {
        I1_VALUE = 0,
        I2_VALUE,
        I3_VALUE,
        I4_VALUE,
        I5_VALUE,
        I_NUM
    };

    enum FLOW_RATE
    {
        S1_VALUE,
        S2_VALUE,
        S3_VALUE,
        S4_VALUE,
        S_NUM
    };

    enum PRESSURE_VALUE
    {
        SOURCE_TANK_VALUE,
        PURE_TANK_VALUE,
        WORK_PRESSURE_VALUE,
        PRESSURE_NUM
    };

    enum I_V_DETECTION
    {
        C1_DETECTION,
        C2_DETECTION,
        C3_DETECTION,
        C4_DETECTION,
        N1_DETECTION,
        N2_DETECTION,
        N3_DETECTION,
        EDI_DETECTION,
        DETECTION_NUM
    };

    enum DEVICE_STATE
    {
        E1_STATE,
        E2_STATE,
        E3_STATE,
        E4_STATE,
        E5_STATE,
        E6_STATE,
        E7_STATE,
        E8_STATE,
        E9_STATE,
        E10_STATE,
        C1_STATE,
        C2_STATE,
        C3_STATE,
        C4_STATE,
        N1_STATE,
        N2_STATE,
        N3_STATE,
        EDI_STATE,
        DEVICE_STATE_NUM
    };
public:
    explicit DFlowChartWidget(QWidget *parent = 0);

    void updateSwitchInfo();
    void updateRpumpInfo(int iChl);
    void updateGpumpInfo(int iChl);
    void updateRectInfo(int iChl);
    void updateEdiInfo(int iChl);

    void updTank(int iIndex,float fVolume);
    void updEcoInfo(int iIndex, ECO_INFO_STRU *info);
    void updPressure(int iIndex,float fvalue);
	void updSwPressure(float fvalue);
    void updFlowInfo(int iIndex,int iValue);
    void updSourceTank(int iIndex,float fVolume);
    void updTOC(float fToc);

    void setTitleText(const QString& text);
    void setInfo1(const QString& text);
    void setInfo2(const QString& text);
    void setInfo3(const QString& text);
    void updateUnits();

    //测试器件状态
    void testDevice(int id, bool state);

public:
    struct WaterInfo
    {
        float fQuality;
        float fTemp;
    };
    struct Detection_Para
    {
        int iValueI;
        int iValueV;
    };

protected:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *);

private:
    void paintWorkStatus(QPainter &painter);
    void paintTapFeed(QPainter &painter); //自来水进水部分
    void paintFeed(QPainter &painter);    //进水部分
    void paintWash(QPainter &painter);    //清洗阀部分
    void paintReject(QPainter &painter);  //弃水电磁阀部分
    void paintProduct(QPainter &painter);  //产水部分
    void paintPureTank(QPainter &painter); //纯水箱部分
    void paintDispense(QPainter &painter); //取水部分

    void paintSourceTankLevel(QPainter &painter); //原水箱液位
    void paintPureTankLevel(QPainter &painter);   //纯水箱液位

    void paintValue(QPainter &painter);

    void initUI();
    void initDefaultValue();
    void loadPixmap();
    QPixmap scaledToWidth(const QPixmap& pixmap, int width);
    QPixmap scaledToHeight(const QPixmap& pixmap, int height);

    void setStatePen(QPainter &painter, bool bWork);

private:
    QPixmap m_devicesPix[DEVICE_NUM];

    WaterInfo    m_waterInfo[I_NUM];
    Detection_Para m_detectionPara[DETECTION_NUM];
    float m_flowRate[S_NUM];
    float m_fFeedPressure;
    float m_fWorkPressure;
    float m_fResidue;
    float m_fToc;

    int   m_iTankLevel;
    float m_fTankLevel;

    int   m_iSourceTankLevel;
    float m_fSourceTankLevel;

    QString m_strUnit[UNIT_NUM];

    bool m_deviceState[DEVICE_STATE_NUM];
    int m_updateTimerID;

    QString m_strWorkStatus[4];
};

#endif // DFLOWCHARTWIDGET_H
