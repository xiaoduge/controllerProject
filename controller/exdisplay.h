#ifndef EXDISPLAY_H
#define EXDISPLAY_H
#include "Display.h"

#ifdef __cplusplus
extern "C"
{
#endif

enum ROWASHSTEP
{
    //RO cl清洗分2步
    ROClWash_FirstStep = 0,
    ROClWash_SecondStep,

    //RO PH清洗小机型分4步
    ROPHWash_FirstStep,
    ROPHWash_SecondStep,
    ROPHWash_ThirdStep,
    ROPHWash_FourthStep,

    //RO PH清洗大机型分3步
    ROPHWash_FirstStep_L,
    ROPHWash_SecondStep_L,
    ROPHWash_ThirdStep_L,
    
    ROWashTimeNum
};

//RO cl清洗和ph清洗时间
extern const unsigned int gROWashDuration[ROWashTimeNum];

typedef struct
{
   float fk;
   float fc;
}Ex_DISP_PARAM_CALI_ITEM_STRU;

typedef struct
{
	float fPureSRange;
	float fFeedSRange;
}SENSOR_RANGE; //Sensor range

typedef struct
{
    short iStart;
}Ex_DISP_PARAM_STEPPER_STRU;

typedef struct
{
    Ex_DISP_PARAM_CALI_ITEM_STRU pc[DISP_PC_COFF_NUM];
	Ex_DISP_PARAM_STEPPER_STRU stepperCali;
}Ex_DISP_PARAM_CALI_STRU;

enum EX_RECT_N
{
    EX_RECT_N1 = 0,
    EX_RECT_N2,
    EX_RECT_N3,
    EX_RECT_NUM
};

typedef struct
{
    int EX_N_NO[EX_RECT_NUM];
}EX_RECT_STATE;

typedef struct
{
    unsigned int ulAlarmNRectTick[EX_RECT_NUM];
    unsigned int ulAlarmNRectDelay[EX_RECT_NUM];
}EX_ALARM_TICK;

typedef struct
{
    unsigned int ulUPAutoCirTick;
}EX_AUTO_CIR_TICK;


typedef struct
{
    unsigned int     bit1AlarmN1       : 1;
    unsigned int     bit1AlarmN2       : 1;
    unsigned int     bit1AlarmN3       : 1;

    unsigned int     bit1AlarmDelayN1  : 1;
    unsigned int     bit1AlarmDelayN2  : 1;
    unsigned int     bit1AlarmDelayN3  : 1;
}EX_ALARM_RECT;

typedef struct
{
    unsigned int     bit1CheckDecPressure       : 1;

}EX_CHECK_STATE;

typedef struct
{
    unsigned int ulDecPressure; //DecPressure;
    int iHandleType;
}EX_DELAY_TICK;

typedef struct
{
    EX_RECT_STATE Ex_Rect_State;
    EX_ALARM_TICK Ex_Alarm_Tick;
    EX_ALARM_RECT Ex_Alarm_Bit;

    EX_DELAY_TICK Ex_Delay_Tick;
    EX_CHECK_STATE EX_Check_State;

    EX_AUTO_CIR_TICK Ex_Auto_Cir_Tick;

}EX_CCB;

typedef struct
{
    unsigned int  bit1NewPPack      : 1;
    unsigned int  bit1FirstQtw      : 1;
}EX_RUNCONDITIONS;

extern unsigned int gMachineFlow;
extern Ex_DISP_PARAM_CALI_STRU gCaliParam;
extern EX_CCB  ex_gCcb;

extern SENSOR_RANGE gSensorRange;

extern unsigned int ex_gulSecond;
extern EX_RUNCONDITIONS g_runConditions;

int Ex_FactoryTest(int select);
unsigned int getKeyState(); //2019.9.16 add
unsigned char getLeakState(); //2020.2.17 add

#ifdef __cplusplus
}
#endif

#endif // EX_DISPLAY_C_H
