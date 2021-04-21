/**********************************************************
 * @Author       dcj
 * @Date         2020-10-28 14:48:28
 * @Description  流程图控件，需要显示流程图，只需要在相应的界面加载此控件即可
 * @Version      V0.0.1
 **********************************************************/
#include "dflowchartwidget.h"
#include "exconfig.h"
#include <QPainter>
#include <QTimerEvent>

const int FONTSIZE = 12; //设置字体大小

const QString toString(float n, int precision = 0)
{
    return QString::number(static_cast<double>(n), 'f', precision);
}

DFlowChartWidget::DFlowChartWidget(QWidget *parent) : QWidget(parent)
{
    loadPixmap();
    initDefaultValue();
    initUI();

    for(int i = 0; i < DEVICE_STATE_NUM; i++)
    {
        m_deviceState[i] = false;
    }
    m_updateTimerID = this->startTimer(1000);
}

//更新器件工作状态
void DFlowChartWidget::updateSwitchInfo()
{
    unsigned int ulTempMaks = DispGetSwitchState(APP_EXE_SWITCHS_MASK);

    m_deviceState[E1_STATE] = ulTempMaks & (1 << APP_EXE_E1_NO) ? true : false;
    m_deviceState[E2_STATE] = ulTempMaks & (1 << APP_EXE_E2_NO) ? true : false;
    m_deviceState[E3_STATE] = ulTempMaks & (1 << APP_EXE_E3_NO) ? true : false;
    m_deviceState[E4_STATE] = ulTempMaks & (1 << APP_EXE_E4_NO) ? true : false;
    m_deviceState[E5_STATE] = ulTempMaks & (1 << APP_EXE_E5_NO) ? true : false;
    m_deviceState[E6_STATE] = ulTempMaks & (1 << APP_EXE_E6_NO) ? true : false;
    m_deviceState[E9_STATE] = ulTempMaks & (1 << APP_EXE_E9_NO) ? true : false;
    m_deviceState[E10_STATE] = ulTempMaks & (1 << APP_EXE_E10_NO) ? true : false;

    m_deviceState[N1_STATE] = ulTempMaks & (1 << APP_EXE_N1_NO) ? true : false;
    m_deviceState[N2_STATE] = ulTempMaks & (1 << APP_EXE_N2_NO) ? true : false;
    m_deviceState[N3_STATE] = ulTempMaks & (1 << APP_EXE_N3_NO) ? true : false;
    m_deviceState[EDI_STATE] = ulTempMaks & (1 << APP_EXE_T1_NO) ? true : false;

    m_deviceState[C1_STATE] = ulTempMaks & (1 << APP_EXE_C1_NO) ? true : false;
    m_deviceState[C2_STATE] = ulTempMaks & (1 << APP_EXE_C2_NO) ? true : false;
    m_deviceState[C3_STATE] = ulTempMaks & (1 << APP_EXE_C3_NO) ? true : false;
    m_deviceState[C4_STATE] = ulTempMaks & (1 << APP_EXE_C4_NO) ? true : false;

    if(ulTempMaks & (1 << APP_EXE_N1_NO))
    {
        DispGetOtherCurrent(APP_EXE_N1_NO,&m_detectionPara[N1_DETECTION].iValueI);
    }
    if(ulTempMaks & (1 << APP_EXE_N2_NO))
    {
        DispGetOtherCurrent(APP_EXE_N2_NO,&m_detectionPara[N2_DETECTION].iValueI);
    }
    if(ulTempMaks & (1 << APP_EXE_N3_NO))
    {
        DispGetOtherCurrent(APP_EXE_N3_NO,&m_detectionPara[N3_DETECTION].iValueI);
    }
    if(ulTempMaks & (1 << APP_EXE_T1_NO))
    {
        DispGetOtherCurrent(APP_EXE_T1_NO,&m_detectionPara[EDI_DETECTION].iValueI);
    }
    if(ulTempMaks & (1 << APP_EXE_C1_NO))
    {
        DispGetRPumpRti(APP_EXE_C1_NO,&m_detectionPara[C1_DETECTION].iValueV, &m_detectionPara[C1_DETECTION].iValueI);
    }
    if(ulTempMaks & (1 << APP_EXE_C2_NO))
    {
        DispGetRPumpRti(APP_EXE_C2_NO,&m_detectionPara[C2_DETECTION].iValueV, &m_detectionPara[C2_DETECTION].iValueI);
    }
    if(ulTempMaks & (1 << APP_EXE_C3_NO))
    {
        DispGetOtherCurrent(APP_EXE_C3_NO,&m_detectionPara[C3_DETECTION].iValueI);
    }
    if(ulTempMaks & (1 << APP_EXE_C4_NO))
    {
        DispGetOtherCurrent(APP_EXE_C4_NO,&m_detectionPara[C4_DETECTION].iValueI);
    }
}

void DFlowChartWidget::updateRpumpInfo(int iChl)
{
    unsigned int ulTempMaks = DispGetSwitchState(APP_EXE_SWITCHS_MASK);
    switch(iChl)
    {
    case 0:
        m_deviceState[C1_STATE] = ulTempMaks & (1 << APP_EXE_C1_NO) ? true : false;
        if(ulTempMaks & (1 << APP_EXE_C1_NO))
        {
            DispGetRPumpRti(APP_EXE_C1_NO,&m_detectionPara[C1_DETECTION].iValueV, &m_detectionPara[C1_DETECTION].iValueI);
        }
        break;
    case 1:
        m_deviceState[C2_STATE] = ulTempMaks & (1 << APP_EXE_C2_NO) ? true : false;
        if(ulTempMaks & (1 << APP_EXE_C2_NO))
        {
            DispGetRPumpRti(APP_EXE_C2_NO,&m_detectionPara[C2_DETECTION].iValueV, &m_detectionPara[C2_DETECTION].iValueI);
        }
        break;
    default:
        break;
    }
}

void DFlowChartWidget::updateGpumpInfo(int iChl)
{
    switch(iChl)
    {
    case 0:
        DispGetOtherCurrent(APP_EXE_C3_NO,&m_detectionPara[C3_DETECTION].iValueI);
        break;
    case 1:
        DispGetOtherCurrent(APP_EXE_C4_NO,&m_detectionPara[C4_DETECTION].iValueI);
        break;
    default:
        break;
    }
}

void DFlowChartWidget::updateRectInfo(int iChl)
{
    switch(iChl)
    {
    case 0:
        DispGetOtherCurrent(APP_EXE_N1_NO,&m_detectionPara[N1_DETECTION].iValueI);
        break;
    case 1:
        DispGetOtherCurrent(APP_EXE_N2_NO,&m_detectionPara[N2_DETECTION].iValueI);
        break;
    case 2:
        DispGetOtherCurrent(APP_EXE_N3_NO,&m_detectionPara[N3_DETECTION].iValueI);
        break;
    default:
        break;
    }
}

void DFlowChartWidget::updateEdiInfo(int iChl)
{
    switch(iChl)
    {
    case 0:
        DispGetOtherCurrent(APP_EXE_T1_NO,&m_detectionPara[EDI_DETECTION].iValueI);
        break;
    default:
        break;
    }
}

void DFlowChartWidget::updTank(int iIndex,float fVolume)
{
    m_iTankLevel = iIndex;
    m_fTankLevel = fVolume;
}

void DFlowChartWidget::updEcoInfo(int iIndex, ECO_INFO_STRU *info)
{
    float fQ;
    float fT;

    switch(iIndex)
    {
    case APP_EXE_I5_NO:
    case APP_EXE_I4_NO:
    case APP_EXE_I3_NO:
        if (CONDUCTIVITY_UINT_OMG == gGlobalParam.MiscParam.iUint4Conductivity)
        {
            fQ = info->fQuality;
        }
        else
        {
            fQ = toConductivity(info->fQuality);
        }
        break;
    default:
        fQ = info->fQuality;
        break;
    }

    if (TEMERATURE_UINT_CELSIUS == gGlobalParam.MiscParam.iUint4Temperature)
    {
        fT = info->fTemperature;
    }
    else
    {
        fT = toFahrenheit(info->fTemperature);
    }

    switch(iIndex)
    {
    case APP_EXE_I5_NO:
        if (DispGetUpQtwFlag() || DispGetUpCirFlag())
        {
            m_waterInfo[I5_VALUE].fQuality = fQ;
            m_waterInfo[I5_VALUE].fTemp = fT;
        }
        break;
    case APP_EXE_I4_NO:
        if (DispGetTocCirFlag())
        {
            m_waterInfo[I4_VALUE].fQuality = fQ;
            m_waterInfo[I4_VALUE].fTemp = fT;
        }
        else if(DispGetUpQtwFlag())
        {
            m_waterInfo[I4_VALUE].fQuality = fQ;
            m_waterInfo[I4_VALUE].fTemp = fT;
        }
        else if(DispGetEdiQtwFlag())
        {
            m_waterInfo[I4_VALUE].fQuality = fQ;
            m_waterInfo[I4_VALUE].fTemp = fT;
        }
        else if (DispGetTankCirFlag())
        {
            m_waterInfo[I4_VALUE].fQuality = fQ;
            m_waterInfo[I4_VALUE].fTemp = fT;
        }
        break;
    case APP_EXE_I3_NO:
        switch (gGlobalParam.iMachineType)
        {
        case MACHINE_RO:
        case MACHINE_UP:
            if (DispGetEdiQtwFlag() || DispGetTankCirFlag())
            {
                m_waterInfo[I3_VALUE].fQuality = fQ;
                m_waterInfo[I3_VALUE].fTemp = fT;
            }
            break;
        default:
            m_waterInfo[I3_VALUE].fQuality = fQ;
            m_waterInfo[I3_VALUE].fTemp = fT;
            break;
        }
        break;
    case APP_EXE_I2_NO: // RO Out
        {
            float fResidue;

            if (DispGetREJ(&fResidue)
                && (MACHINE_PURIST != gGlobalParam.iMachineType))
            {
                m_fResidue = fResidue;
            }

            m_waterInfo[I2_VALUE].fQuality = fQ;
            m_waterInfo[I2_VALUE].fTemp = fT;
        }
        break;
    case APP_EXE_I1_NO: // RO In
        m_waterInfo[I1_VALUE].fQuality = fQ;
        m_waterInfo[I1_VALUE].fTemp = fT;
        break;
    default:
        break;
    }
}

void DFlowChartWidget::updPressure(int iIndex,float fvalue)
{
    if (APP_EXE_PM1_NO == iIndex)
    {
        if (PRESSURE_UINT_BAR == gGlobalParam.MiscParam.iUint4Pressure)
        {
            m_fWorkPressure = fvalue;
        }
        else if (PRESSURE_UINT_MPA == gGlobalParam.MiscParam.iUint4Pressure)
        {
            m_fWorkPressure = toMpa(fvalue);
        }
        else
        {
            m_fWorkPressure = toPsi(fvalue);
        }
    }
}

void DFlowChartWidget::updSwPressure(float fvalue)
{
    if (PRESSURE_UINT_BAR == gGlobalParam.MiscParam.iUint4Pressure)
    {
    	m_fFeedPressure = fvalue;
    }
    else if (PRESSURE_UINT_MPA == gGlobalParam.MiscParam.iUint4Pressure)
    {
		m_fFeedPressure = toMpa(fvalue);
    }
    else
    {
		m_fFeedPressure = toPsi(fvalue);
    }
}

void DFlowChartWidget::updFlowInfo(int iIndex,int iValue)
{
    switch(iIndex)
    {
    case APP_FM_FM1_NO:
        if (DispGetUpQtwFlag() || DispGetUpCirFlag()
            || DispGetEdiQtwFlag() || DispGetTankCirFlag())
        {
            m_flowRate[S1_VALUE] = iValue*1.0/1000;
        }
        else
        {
            m_flowRate[S1_VALUE] = 0;
        }
        break;
   case APP_FM_FM2_NO:
        m_flowRate[S2_VALUE] = (60.0*iValue)/1000;
        break;
   case APP_FM_FM3_NO:
        m_flowRate[S3_VALUE] = (60.0*iValue)/1000;
        break;
   case APP_FM_FM4_NO:
        m_flowRate[S4_VALUE] = (60.0*iValue)/1000;
        break;
   }
}

void DFlowChartWidget::updSourceTank(int iIndex,float fVolume)
{
    m_iSourceTankLevel = iIndex;
    m_fSourceTankLevel = fVolume;
}

void DFlowChartWidget::updTOC(float fToc)
{
    m_fToc = fToc;
}

void DFlowChartWidget::setTitleText(const QString& text)
{
    m_strWorkStatus[0] = text;
}

void DFlowChartWidget::setInfo1(const QString& text)
{
    m_strWorkStatus[1] = text;
}

void DFlowChartWidget::setInfo2(const QString& text)
{
    m_strWorkStatus[2] = text;
}

void DFlowChartWidget::setInfo3(const QString& text)
{
    m_strWorkStatus[3] = text;
}

void DFlowChartWidget::updateUnits()
{
    if (CONDUCTIVITY_UINT_OMG == gGlobalParam.MiscParam.iUint4Conductivity)
    {
        m_strUnit[RES_COND_UNIT] = tr("omg");
    }
    else
    {
        m_strUnit[RES_COND_UNIT] = tr("us");
    }

    if (TEMERATURE_UINT_CELSIUS == gGlobalParam.MiscParam.iUint4Temperature)
    {
        m_strUnit[TEMP_UNIT] = tr("Celsius");
    }
    else
    {
        m_strUnit[TEMP_UNIT] = tr("F");
    }

    if (PRESSURE_UINT_BAR == gGlobalParam.MiscParam.iUint4Pressure)
    {
        m_strUnit[PRESSURE_UNIT] = QString(" bar");
    }
    else if (PRESSURE_UINT_MPA == gGlobalParam.MiscParam.iUint4Pressure)
    {
        m_strUnit[PRESSURE_UNIT] = QString(" MPa");
    }
    else
    {
        m_strUnit[PRESSURE_UNIT] = QString(" psi");
    }
}

//器件状态测试
void DFlowChartWidget::testDevice(int id, bool state)
{
     m_deviceState[id] = state;
}

void DFlowChartWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    paintTapFeed(painter); //绘制自来水进水部分
    paintFeed(painter);    //绘制进水部分，包括限流阀弃水部分
    paintWash(painter);    //绘制清洗部分
    paintReject(painter);  //绘制弃水部分
    paintProduct(painter); //绘制产水部分
    paintPureTank(painter); //绘制纯水箱
    paintDispense(painter); //绘制取水部分

    paintWorkStatus(painter); //绘制工作状态
    paintValue(painter);      //绘制参数值

    return QWidget::paintEvent(event);
}

// 定时刷新控件
void DFlowChartWidget::timerEvent(QTimerEvent *event)
{
    if(m_updateTimerID == event->timerId())
    {
        repaint();
    }
}

//绘制工作状态
void DFlowChartWidget::paintWorkStatus(QPainter &painter)
{
    QPen oldPen = painter.pen();
    QFont oldFont = painter.font();
    painter.setFont(QFont("", FONTSIZE, QFont::Bold));
    painter.setPen(QPen(Qt::gray));

    painter.drawText(20,  5, 150, 40, Qt::AlignLeft, m_strWorkStatus[0] + ":");
    painter.drawText(200, 5, 150, 40, Qt::AlignLeft, m_strWorkStatus[1]);
    painter.drawText(400, 5, 150, 40, Qt::AlignLeft, m_strWorkStatus[2]);
    painter.drawText(600, 5, 150, 40, Qt::AlignLeft, m_strWorkStatus[3]);

    painter.setFont(oldFont);
    painter.setPen(oldPen);
}

//绘制原水进水部分，包括原水箱及原水箱之前的部分
void DFlowChartWidget::paintTapFeed(QPainter &painter)
{
	int offsetX = 80;
    int offsetY = 40;

    QPolygonF points;

    QPen pen(Qt::gray, 3);
    painter.setPen(pen);
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        setStatePen(painter, m_deviceState[E10_STATE]);

        //E10 -> Feed Tank
        points.clear();
        points.append(QPointF(width() - 20, offsetY + 56));
        points.append(QPointF(width() - 95, offsetY + 56));
        points.append(QPointF(width() - 95, offsetY - 4));
        points.append(QPointF(width() - 175, offsetY - 4));
        points.append(QPointF(width() - 175, offsetY + 56));
        points.append(QPointF(width() - 261, offsetY + 56));
        painter.drawPolyline(points);

        if(m_deviceState[E10_STATE])
        {
            //原水阀
            offsetX -= 10;
            painter.drawPixmap(width() - offsetX, offsetY + 42, scaledToWidth(m_devicesPix[VALVE_ON], 28));

            //预处理
            offsetX += 50;
            painter.drawPixmap(width() - offsetX, offsetY - 7, scaledToHeight(m_devicesPix[PRE_ON], 70));
            offsetX += 20;
            painter.drawPixmap(width() - offsetX, offsetY - 7, scaledToHeight(m_devicesPix[PRE_ON], 70));
            offsetX += 20;
            painter.drawPixmap(width() - offsetX, offsetY - 7, scaledToHeight(m_devicesPix[PRE_ON], 70));            
        }
        else
        {
            //原水阀
            offsetX -= 10;
            painter.drawPixmap(width() - offsetX, offsetY + 42, scaledToWidth(m_devicesPix[VALVE_OFF], 28));

            //预处理
            offsetX += 50;
            painter.drawPixmap(width() - offsetX, offsetY - 7, scaledToHeight(m_devicesPix[PRE_OFF], 70));
            offsetX += 20;
            painter.drawPixmap(width() - offsetX, offsetY - 7, scaledToHeight(m_devicesPix[PRE_OFF], 70));
            offsetX += 20;
            painter.drawPixmap(width() - offsetX, offsetY - 7, scaledToHeight(m_devicesPix[PRE_OFF], 70));
        }
		//PRE Pack
        offsetX += 50;
		if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_Pre_Filter))
		{
			if (gCMUsage.ulUsageState & (1 << DISP_PRE_PACKLIFEDAY) 
                || gCMUsage.ulUsageState & (1 << DISP_PRE_PACKLIFEL))
            {
				painter.drawPixmap(width() - offsetX, offsetY + 50, scaledToWidth(m_devicesPix[PREPACK_YELLOW], 20));
            }
			else if(m_deviceState[E10_STATE])
			{
			    painter.drawPixmap(width() - offsetX, offsetY + 50, scaledToWidth(m_devicesPix[PREPACK_ON], 20));
			}
			else
			{
				painter.drawPixmap(width() - offsetX, offsetY + 50, scaledToWidth(m_devicesPix[PREPACK_OFF], 20));
			}
		}

		if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveB3))
		{
			//原水箱
        	offsetX += 50;
        	painter.drawPixmap(width() - offsetX, offsetY - 7, scaledToHeight(m_devicesPix[TANK], 70));
        	paintSourceTankLevel(painter);
		}
		
        break;
    default:
        break;
    }
}

//绘制进水部分，包括进水阀、RO PacK、I2、工作压力表、RO限流阀弃水部分等
void DFlowChartWidget::paintFeed(QPainter &painter)
{
    int offsetX = 220;
    int offsetY = 40;

    QPolygonF points;
    QPen pen(Qt::gray, 3);
    painter.setPen(pen);
    QMatrix matrix;

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        setStatePen(painter, m_deviceState[E1_STATE]);

        //Feed Tank->E1
        points.append(QPointF(width() - 261, offsetY + 56));
        points.append(QPointF(width() - 305, offsetY + 56));
        painter.drawPolyline(points);

        //E1 -> RO弃水
        points.clear();
        points.append(QPointF(width() - 305, offsetY + 56));
        points.append(QPointF(width() - 455, offsetY + 56));
        points.append(QPointF(width() - 455, offsetY + 71));
        points.append(QPointF(width() - 468, offsetY + 71));
        points.append(QPointF(width() - 468, offsetY + 56));
        points.append(QPointF(width() - 714, offsetY + 56));
        points.append(QPointF(width() - 714, offsetY + 274));
        points.append(QPointF(width() - 610, offsetY + 274));
        painter.drawPolyline(points);

        //RO Pack -> RO Pack
        points.clear();
        points.append(QPointF(width() - 730, offsetY + 90));
        points.append(QPointF(width() - 744, offsetY + 90));
        points.append(QPointF(width() - 744, offsetY + 274));
        points.append(QPointF(width() - 715, offsetY + 274));
        painter.drawPolyline(points);

        //RO回水
        points.clear();
        points.append(QPointF(width() - 758, offsetY + 135));
        points.append(QPointF(width() - 765, offsetY + 135));
        points.append(QPointF(width() - 765, offsetY));
        points.append(QPointF(width() - 480, offsetY));
        points.append(QPointF(width() - 480, offsetY + 56));
        painter.drawPolyline(points);

        //RO弃水 限流阀
        points.clear();
        points.append(QPointF(width() - 765, offsetY + 135));
        points.append(QPointF(width() - 765, offsetY + 334));
        points.append(QPointF(width() - 650, offsetY + 334));
        points.append(QPointF(width() - 650, offsetY + 300));
        points.append(QPointF(width() - 520, offsetY + 300));
        points.append(QPointF(width() - 520, offsetY + 430));
        painter.drawPolyline(points);

        //原水泵
        offsetX += 100;
        if(m_deviceState[C3_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 32, scaledToHeight(m_devicesPix[PUMP_ON], 32));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 32, scaledToHeight(m_devicesPix[PUMP_OFF], 32));
        }

        if(m_deviceState[E1_STATE])
        {
            //进水阀
            offsetX += 50;
            painter.drawPixmap(width() - offsetX, offsetY + 42, scaledToWidth(m_devicesPix[VALVE_ON], 28));

            //进水压力传感器
            offsetX += 60;
            painter.drawPixmap(width() - offsetX, offsetY + 42, scaledToWidth(m_devicesPix[P_ON], 28));

            //P Pack
            offsetX += 45;
			if (gCMUsage.ulUsageState & (1 << DISP_P_PACKLIFEDAY) 
                || gCMUsage.ulUsageState & (1 << DISP_P_PACKLIFEL))
			{
				painter.drawPixmap(width() - offsetX, offsetY + 65, scaledToWidth(m_devicesPix[PACK_YELLOW], 28));
			}
			else
			{
				painter.drawPixmap(width() - offsetX, offsetY + 65, scaledToWidth(m_devicesPix[PACK_ON], 28));
			}
            
            //进水流量计
            offsetX += 80;
            painter.drawPixmap(width() - offsetX, offsetY + 51, scaledToWidth(m_devicesPix[FLOWMETER_ON], 28));

            //进水电导率电极
            offsetX += 80;
            painter.drawPixmap(width() - offsetX, offsetY + 42, scaledToWidth(m_devicesPix[I_ON], 28));
        }
        else
        {
            //进水阀
            offsetX += 50;
            painter.drawPixmap(width() - offsetX, offsetY + 42, scaledToWidth(m_devicesPix[VALVE_OFF], 28));

            //进水压力传感器
            offsetX += 60;
            painter.drawPixmap(width() - offsetX, offsetY + 42, scaledToWidth(m_devicesPix[P_OFF], 28));

            //P Pack
            offsetX += 45;
			if (gCMUsage.ulUsageState & (1 << DISP_P_PACKLIFEDAY) 
                || gCMUsage.ulUsageState & (1 << DISP_P_PACKLIFEL))
			{
				painter.drawPixmap(width() - offsetX, offsetY + 65, scaledToWidth(m_devicesPix[PACK_YELLOW], 28));
			}
			else
			{
				painter.drawPixmap(width() - offsetX, offsetY + 65, scaledToWidth(m_devicesPix[PACK_OFF], 28));
			}

            //进水流量计
            offsetX += 80;
            painter.drawPixmap(width() - offsetX, offsetY + 51, scaledToWidth(m_devicesPix[FLOWMETER_OFF], 28));

            //进水电导率电极
            offsetX += 80;
            painter.drawPixmap(width() - offsetX, offsetY + 42, scaledToWidth(m_devicesPix[I_OFF], 28));

        }

        //RO 增压泵
        offsetX += 50;
        if(m_deviceState[C1_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 32, scaledToWidth(m_devicesPix[PUMP_ON], 28));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 32, scaledToWidth(m_devicesPix[PUMP_OFF], 28));
        }

        if(m_deviceState[E1_STATE])
        {
            //RO Pack
            offsetX += 45;
            painter.drawPixmap(width() - offsetX, offsetY + 75, scaledToWidth(m_devicesPix[ROPACK_ON], 28));

            //RO Pack
            offsetX += 30;
            painter.drawPixmap(width() - offsetX, offsetY + 120, scaledToWidth(m_devicesPix[ROPACK_ON], 28));

            //RO电导率电极
            offsetX -= 85;
            painter.drawPixmap(width() - offsetX, offsetY + 260, scaledToWidth(m_devicesPix[I_ON], 28));

            //工作压力传感器
            offsetX += 60;
            painter.drawPixmap(width() - offsetX, offsetY + 320, scaledToWidth(m_devicesPix[P_ON], 28));

            //弃水限流阀
            offsetX -= 150;
            painter.drawPixmap(width() - offsetX - 50, offsetY + 295, scaledToWidth(m_devicesPix[LIMIT_ON], 28));

            //弃水流量计
            offsetX -= 59;
            matrix.rotate(-90);
            painter.drawPixmap(width() - offsetX, offsetY + 340,
                               scaledToWidth(m_devicesPix[FLOWMETER_ON], 28).transformed(matrix, Qt::SmoothTransformation));
        }
        else
        {
            //RO Pack
            offsetX += 45;
            painter.drawPixmap(width() - offsetX, offsetY + 75, scaledToWidth(m_devicesPix[ROPACK_OFF], 28));

            //RO Pack
            offsetX += 30;
            painter.drawPixmap(width() - offsetX, offsetY + 120, scaledToWidth(m_devicesPix[ROPACK_OFF], 28));

            //RO电导率电极
            offsetX -= 85;
            painter.drawPixmap(width() - offsetX, offsetY + 260, scaledToWidth(m_devicesPix[I_OFF], 28));

            //工作压力传感器
            offsetX += 60;
            painter.drawPixmap(width() - offsetX, offsetY + 320, scaledToWidth(m_devicesPix[P_OFF], 28));

            //弃水限流阀
            offsetX -= 150;
            painter.drawPixmap(width() - offsetX - 50, offsetY + 295, scaledToWidth(m_devicesPix[LIMIT_OFF], 28));

            //弃水流量计
            offsetX -= 59;
            matrix.rotate(-90);
            painter.drawPixmap(width() - offsetX, offsetY + 340,
                               scaledToWidth(m_devicesPix[FLOWMETER_OFF], 28).transformed(matrix, Qt::SmoothTransformation));
        }
        break;
    case MACHINE_Genie:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_UP:
    case MACHINE_ADAPT:
        offsetX = 100;
        setStatePen(painter, m_deviceState[E1_STATE]);

        //Feed Tank->E1
        points.append(QPointF(width() - 70, offsetY + 56));
        points.append(QPointF(width() - 300, offsetY + 56));
        painter.drawPolyline(points);

        //E1 -> RO弃水
        points.clear();
        points.append(QPointF(width() - 300, offsetY + 56));
        if(gGlobalParam.iMachineType != MACHINE_ADAPT)
        {
            points.append(QPointF(width() - 300, offsetY + 71));
            points.append(QPointF(width() - 313, offsetY + 71));
            points.append(QPointF(width() - 313, offsetY + 56));

        }
        points.append(QPointF(width() - 340, offsetY + 56));
        points.append(QPointF(width() - 340, offsetY + 71));
        points.append(QPointF(width() - 353, offsetY + 71));
        points.append(QPointF(width() - 353, offsetY + 56));
        points.append(QPointF(width() - 714, offsetY + 56));
        points.append(QPointF(width() - 714, offsetY + 274));
        points.append(QPointF(width() - 610, offsetY + 274));
        painter.drawPolyline(points);

        //RO Pack -> RO Pack
        points.clear();
        points.append(QPointF(width() - 730, offsetY + 90));
        points.append(QPointF(width() - 744, offsetY + 90));
        points.append(QPointF(width() - 744, offsetY + 274));
        points.append(QPointF(width() - 715, offsetY + 274));
        painter.drawPolyline(points);

        //RO回水
        points.clear();
        points.append(QPointF(width() - 758, offsetY + 135));
        points.append(QPointF(width() - 765, offsetY + 135));
        points.append(QPointF(width() - 765, offsetY));
        points.append(QPointF(width() - 480, offsetY));
        points.append(QPointF(width() - 480, offsetY + 56));
        painter.drawPolyline(points);

        //RO弃水 限流阀
        points.clear();
        points.append(QPointF(width() - 765, offsetY + 135));
        points.append(QPointF(width() - 765, offsetY + 334));
        points.append(QPointF(width() - 650, offsetY + 334));
        points.append(QPointF(width() - 650, offsetY + 300));
        points.append(QPointF(width() - 520, offsetY + 300));
        points.append(QPointF(width() - 520, offsetY + 430));
        painter.drawPolyline(points);

		//PRE Pack
        offsetX += 20;
		if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_Pre_Filter))
		{
			if (gCMUsage.ulUsageState & (1 << DISP_PRE_PACKLIFEDAY) 
                || gCMUsage.ulUsageState & (1 << DISP_PRE_PACKLIFEL))
            {
				painter.drawPixmap(width() - offsetX, offsetY + 50, scaledToWidth(m_devicesPix[PREPACK_YELLOW], 20));
            }
			else if(m_deviceState[E1_STATE])
			{
			    painter.drawPixmap(width() - offsetX, offsetY + 50, scaledToWidth(m_devicesPix[PREPACK_ON], 20));
			}
			else
			{
				painter.drawPixmap(width() - offsetX, offsetY + 50, scaledToWidth(m_devicesPix[PREPACK_OFF], 20));
			}
		}

        if(m_deviceState[E1_STATE])
        {
			//进水阀
            offsetX += 60;
            painter.drawPixmap(width() - offsetX, offsetY + 42, scaledToWidth(m_devicesPix[VALVE_ON], 28));

            //进水压力传感器
            offsetX += 80;
            painter.drawPixmap(width() - offsetX, offsetY + 42, scaledToWidth(m_devicesPix[P_ON], 28));

            //AC Pack
            offsetX += 60;
            if(gGlobalParam.iMachineType != MACHINE_ADAPT)
            {
            	if (gCMUsage.ulUsageState & (1 << DISP_AC_PACKLIFEDAY)
                	|| gCMUsage.ulUsageState & (1 << DISP_AC_PACKLIFEL))
            	{
            		painter.drawPixmap(width() - offsetX, offsetY + 65, scaledToWidth(m_devicesPix[PACK_YELLOW], 28));
            	}
				else
				{
                	painter.drawPixmap(width() - offsetX, offsetY + 65, scaledToWidth(m_devicesPix[PACK_ON], 28));
				}
            }

            //P Pack
            offsetX += 40;
			if (gCMUsage.ulUsageState & (1 << DISP_P_PACKLIFEDAY) 
                || gCMUsage.ulUsageState & (1 << DISP_P_PACKLIFEL))
			{
				painter.drawPixmap(width() - offsetX, offsetY + 65, scaledToWidth(m_devicesPix[PACK_YELLOW], 28));
			}
			else
			{
				painter.drawPixmap(width() - offsetX, offsetY + 65, scaledToWidth(m_devicesPix[PACK_ON], 28));
			}

            //进水电导率电极
            offsetX += 245;
            painter.drawPixmap(width() - offsetX, offsetY + 42, scaledToWidth(m_devicesPix[I_ON], 28));
        }
        else
        {
            //进水阀
            offsetX += 60;
            painter.drawPixmap(width() - offsetX, offsetY + 42, scaledToWidth(m_devicesPix[VALVE_OFF], 28));

            //进水压力传感器
            offsetX += 80;
            painter.drawPixmap(width() - offsetX, offsetY + 42, scaledToWidth(m_devicesPix[P_OFF], 28));

            //AC Pack
            offsetX += 60;
            if(gGlobalParam.iMachineType != MACHINE_ADAPT)
            {
            	if (gCMUsage.ulUsageState & (1 << DISP_AC_PACKLIFEDAY)
                	|| gCMUsage.ulUsageState & (1 << DISP_AC_PACKLIFEL))
            	{
            		painter.drawPixmap(width() - offsetX, offsetY + 65, scaledToWidth(m_devicesPix[PACK_YELLOW], 28));
            	}
				else
				{
                	painter.drawPixmap(width() - offsetX, offsetY + 65, scaledToWidth(m_devicesPix[PACK_OFF], 28));
				}
            }

            //P Pack
            offsetX += 40;
			if (gCMUsage.ulUsageState & (1 << DISP_P_PACKLIFEDAY) 
                || gCMUsage.ulUsageState & (1 << DISP_P_PACKLIFEL))
			{
				painter.drawPixmap(width() - offsetX, offsetY + 65, scaledToWidth(m_devicesPix[PACK_YELLOW], 28));
			}
			else
			{
				painter.drawPixmap(width() - offsetX, offsetY + 65, scaledToWidth(m_devicesPix[PACK_OFF], 28));
			}

            //进水电导率电极
            offsetX += 245;
            painter.drawPixmap(width() - offsetX, offsetY + 42, scaledToWidth(m_devicesPix[I_OFF], 28));
        }

        //RO 增压泵
        offsetX += 80;
        if(m_deviceState[C1_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 32, scaledToWidth(m_devicesPix[PUMP_ON], 28));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 32, scaledToWidth(m_devicesPix[PUMP_OFF], 28));
        }

        if(m_deviceState[E1_STATE])
        {
            //RO Pack
            offsetX += 45;
            painter.drawPixmap(width() - offsetX, offsetY + 75, scaledToWidth(m_devicesPix[ROPACK_ON], 28));

            //RO Pack
            offsetX += 30;
            painter.drawPixmap(width() - offsetX, offsetY + 120, scaledToWidth(m_devicesPix[ROPACK_ON], 28));

            //RO电导率电极
            offsetX -= 85;
            painter.drawPixmap(width() - offsetX, offsetY + 260, scaledToWidth(m_devicesPix[I_ON], 28));

            //工作压力传感器
            offsetX += 60;
            painter.drawPixmap(width() - offsetX, offsetY + 320, scaledToWidth(m_devicesPix[P_ON], 28));

            //弃水限流阀
            offsetX -= 150;
            painter.drawPixmap(width() - offsetX - 50, offsetY + 295, scaledToWidth(m_devicesPix[LIMIT_ON], 28));
        }
        else
        {
            //RO Pack
            offsetX += 45;
            painter.drawPixmap(width() - offsetX, offsetY + 75, scaledToWidth(m_devicesPix[ROPACK_OFF], 28));

            //RO Pack
            offsetX += 30;
            painter.drawPixmap(width() - offsetX, offsetY + 120, scaledToWidth(m_devicesPix[ROPACK_OFF], 28));

            //RO电导率电极
            offsetX -= 85;
            painter.drawPixmap(width() - offsetX, offsetY + 260, scaledToWidth(m_devicesPix[I_OFF], 28));

            //工作压力传感器
            offsetX += 60;
            painter.drawPixmap(width() - offsetX, offsetY + 320, scaledToWidth(m_devicesPix[P_OFF], 28));

            //弃水限流阀
            offsetX -= 150;
            painter.drawPixmap(width() - offsetX - 50, offsetY + 295, scaledToWidth(m_devicesPix[LIMIT_OFF], 28));
        }
        break;
    default:
        break;
    }
}

//绘制RO清洗阀弃水部分
void DFlowChartWidget::paintWash(QPainter &painter)
{
    int offsetX = 585;
    int offsetY = 40;

    QPolygonF points;
    QPen pen(Qt::gray, 3);
    painter.setPen(pen);

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_UP:
    case MACHINE_ADAPT:
        setStatePen(painter, m_deviceState[E3_STATE]);

        points.append(QPointF(width() - 610, offsetY + 274));
        points.append(QPointF(width() - 520, offsetY + 274));
        points.append(QPointF(width() - 520, offsetY + 300));
        painter.drawPolyline(points);

        //清洗阀
        if(m_deviceState[E3_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 260, scaledToWidth(m_devicesPix[VALVE_ON], 28));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 260, scaledToWidth(m_devicesPix[VALVE_OFF], 28));
        }
        break;
    default:
        break;
    }
}

//绘制RO弃水阀弃水部分
void DFlowChartWidget::paintReject(QPainter &painter)
{
    int offsetX = 635;
    int offsetY = 40;

    QPolygonF points;
    QPen pen(Qt::gray, 3);
    painter.setPen(pen);

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_UP:
    case MACHINE_ADAPT:
        setStatePen(painter, m_deviceState[E2_STATE]);

        //RO弃水 弃水阀
        points.clear();
        points.append(QPointF(width() - 650, offsetY + 334));
        points.append(QPointF(width() - 590, offsetY + 334));
        points.append(QPointF(width() - 590, offsetY + 300));
        painter.drawPolyline(points);

        //E2 弃水阀
        if(m_deviceState[E2_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 320, scaledToWidth(m_devicesPix[VALVE_ON], 28));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 320, scaledToWidth(m_devicesPix[VALVE_OFF], 28));

        }
        break;
    default:
        break;
    }
}

//绘制RO产水部分
void DFlowChartWidget::paintProduct(QPainter &painter)
{
    int offsetX = 624;
    int offsetY = 40;

    QPolygonF points;
    QPen pen(Qt::gray, 3);
    painter.setPen(pen);
    QMatrix matrix;

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_EDI_LOOP:
        setStatePen(painter, !m_deviceState[E3_STATE] && m_deviceState[E1_STATE]);

		//RO产水
		if(gGlobalParam.iMachineType == MACHINE_L_Genie 
			&& gAdditionalCfgParam.machineInfo.iMachineFlow == 250)
		{
	        points.clear();
	        points.append(QPointF(width() - 610, offsetY + 274));
	        points.append(QPointF(width() - 610, offsetY + 140));
	        points.append(QPointF(width() - 150, offsetY + 140));
	        points.append(QPointF(width() - 150, offsetY + 170));
	        painter.drawPolyline(points);
		}
		else
		{
	        points.clear();
	        points.append(QPointF(width() - 610, offsetY + 274));
	        points.append(QPointF(width() - 610, offsetY + 140));
	        points.append(QPointF(width() - 533, offsetY + 140));
	        points.append(QPointF(width() - 533, offsetY + 155));
	        points.append(QPointF(width() - 520, offsetY + 155));
	        points.append(QPointF(width() - 520, offsetY + 140));
	        points.append(QPointF(width() - 150, offsetY + 140));
	        points.append(QPointF(width() - 150, offsetY + 170));
	        painter.drawPolyline(points);
		}
        
        //EDI弃水
        points.clear();
        points.append(QPointF(width() - 500, offsetY + 140));
        points.append(QPointF(width() - 500, offsetY + 172));
        points.append(QPointF(width() - 450, offsetY + 172));
        points.append(QPointF(width() - 450, offsetY + 210));
        points.append(QPointF(width() - 500, offsetY + 210));
        points.append(QPointF(width() - 500, offsetY + 430));
        painter.drawPolyline(points);

        if(!m_deviceState[E3_STATE] && m_deviceState[E1_STATE])
        {
            //产水阀
            matrix.rotate(-90);
            painter.drawPixmap(width() - offsetX, offsetY + 190,
                               scaledToWidth(m_devicesPix[VALVE_ON], 28).transformed(matrix, Qt::SmoothTransformation));

            //产水流量计
            offsetX -= 39;
            matrix.rotate(270);
            painter.drawPixmap(width() - offsetX, offsetY + 135,
                               scaledToWidth(m_devicesPix[FLOWMETER_ON], 28).transformed(matrix, Qt::SmoothTransformation));

            //AT Pack
            offsetX -= 45;
			if (gCMUsage.ulUsageState & (1 << DISP_AT_PACKLIFEDAY) 
                || gCMUsage.ulUsageState & (1 << DISP_AT_PACKLIFEL))

			{
				painter.drawPixmap(width() - offsetX, offsetY + 150, scaledToWidth(m_devicesPix[PACK_YELLOW], 28));
			}
			else
			{
				painter.drawPixmap(width() - offsetX, offsetY + 150, scaledToWidth(m_devicesPix[PACK_ON], 28));
			}

            //EDI
            offsetX -= 50;
            if(m_deviceState[EDI_STATE])
            {
                painter.drawPixmap(width() - offsetX, offsetY + 130, scaledToWidth(m_devicesPix[EDI_ON], 28));
            }
            else
            {
                painter.drawPixmap(width() - offsetX, offsetY + 130, scaledToWidth(m_devicesPix[EDI_OFF], 28));
            }

            //EDI电阻率电极
            offsetX -= 80;
            painter.drawPixmap(width() - offsetX, offsetY + 126, scaledToWidth(m_devicesPix[I_ON], 28));
        }
        else
        {
            //产水阀
            matrix.rotate(-90);
            painter.drawPixmap(width() - offsetX, offsetY + 190,
                               scaledToWidth(m_devicesPix[VALVE_OFF], 28).transformed(matrix, Qt::SmoothTransformation));

            //产水流量计
            offsetX -= 39;
            matrix.rotate(270);
            painter.drawPixmap(width() - offsetX, offsetY + 135,
                               scaledToWidth(m_devicesPix[FLOWMETER_OFF], 28).transformed(matrix, Qt::SmoothTransformation));

			//AT Pack
		    offsetX -= 45;
			if(gGlobalParam.iMachineType == MACHINE_L_Genie)
			{
				if(gAdditionalCfgParam.machineInfo.iMachineFlow != 250)
				{
					if (gCMUsage.ulUsageState & (1 << DISP_AT_PACKLIFEDAY) 
		                || gCMUsage.ulUsageState & (1 << DISP_AT_PACKLIFEL))

					{
						painter.drawPixmap(width() - offsetX, offsetY + 150, scaledToWidth(m_devicesPix[PACK_YELLOW], 28));
					}
					else
					{
						painter.drawPixmap(width() - offsetX, offsetY + 150, scaledToWidth(m_devicesPix[PACK_OFF], 28));
					}
				}
			}
			else
			{
				if (gCMUsage.ulUsageState & (1 << DISP_AT_PACKLIFEDAY) 
	                || gCMUsage.ulUsageState & (1 << DISP_AT_PACKLIFEL))

				{
					painter.drawPixmap(width() - offsetX, offsetY + 150, scaledToWidth(m_devicesPix[PACK_YELLOW], 28));
				}
				else
				{
					painter.drawPixmap(width() - offsetX, offsetY + 150, scaledToWidth(m_devicesPix[PACK_OFF], 28));
				}
			}

            //EDI
            offsetX -= 50;
            if(m_deviceState[EDI_STATE])
            {
                painter.drawPixmap(width() - offsetX, offsetY + 130, scaledToWidth(m_devicesPix[EDI_ON], 28));
            }
            else
            {
                painter.drawPixmap(width() - offsetX, offsetY + 130, scaledToWidth(m_devicesPix[EDI_OFF], 28));
            }
            //EDI电阻率电极
            offsetX -= 80;
            painter.drawPixmap(width() - offsetX, offsetY + 126, scaledToWidth(m_devicesPix[I_OFF], 28));

        }
		
		//纯水紫外灯
        offsetX -= 90;
        matrix.rotate(90);
		if (gCMUsage.ulUsageState & (1 << DISP_N1_UVLIFEDAY) 
			|| gCMUsage.ulUsageState & (1 << DISP_N1_UVLIFEHOUR))
		{
			painter.drawPixmap(width() - offsetX, offsetY + 133, 
							   scaledToHeight(m_devicesPix[UV_YELLOW], 64).transformed(matrix, Qt::SmoothTransformation));
		}
		else if(m_deviceState[N1_STATE])
		{
			painter.drawPixmap(width() - offsetX, offsetY + 133,
                               scaledToHeight(m_devicesPix[UV_ON], 64).transformed(matrix, Qt::SmoothTransformation));
		}
		else
		{
			painter.drawPixmap(width() - offsetX, offsetY + 133,
                               scaledToHeight(m_devicesPix[UV_OFF], 64).transformed(matrix, Qt::SmoothTransformation));
		}
        break;
    case MACHINE_L_UP:
    case MACHINE_L_RO_LOOP:
        setStatePen(painter, !m_deviceState[E3_STATE] && m_deviceState[E1_STATE]);

        //RO产水
        points.clear();
        points.append(QPointF(width() - 610, offsetY + 274));
        points.append(QPointF(width() - 610, offsetY + 140));
        points.append(QPointF(width() - 150, offsetY + 140));
        points.append(QPointF(width() - 150, offsetY + 170));
        painter.drawPolyline(points);

        if(!m_deviceState[E3_STATE] && m_deviceState[E1_STATE])
        {
            //产水阀
            matrix.rotate(-90);
            painter.drawPixmap(width() - offsetX, offsetY + 190,
                               scaledToWidth(m_devicesPix[VALVE_ON], 28).transformed(matrix, Qt::SmoothTransformation));

            //产水流量计
            offsetX -= 60;
            matrix.rotate(270);
            painter.drawPixmap(width() - offsetX, offsetY + 135,
                               scaledToWidth(m_devicesPix[FLOWMETER_ON], 28).transformed(matrix, Qt::SmoothTransformation));
        }
        else
        {
            //产水阀
            matrix.rotate(-90);
            painter.drawPixmap(width() - offsetX, offsetY + 190,
                               scaledToWidth(m_devicesPix[VALVE_OFF], 28).transformed(matrix, Qt::SmoothTransformation));

            //产水流量计
            offsetX -= 60;
            matrix.rotate(270);
            painter.drawPixmap(width() - offsetX, offsetY + 135,
                               scaledToWidth(m_devicesPix[FLOWMETER_OFF], 28).transformed(matrix, Qt::SmoothTransformation));
        }

	    //纯水紫外灯
        offsetX -= 245;
        matrix.rotate(90);
		if (gCMUsage.ulUsageState & (1 << DISP_N1_UVLIFEDAY) 
			|| gCMUsage.ulUsageState & (1 << DISP_N1_UVLIFEHOUR))
		{
			painter.drawPixmap(width() - offsetX, offsetY + 133,
							   scaledToHeight(m_devicesPix[UV_YELLOW], 64).transformed(matrix, Qt::SmoothTransformation));

		}
		else if(m_deviceState[N1_STATE])
		{
			painter.drawPixmap(width() - offsetX, offsetY + 133,
							   scaledToHeight(m_devicesPix[UV_ON], 64).transformed(matrix, Qt::SmoothTransformation));

		}
		else
		{
			painter.drawPixmap(width() - offsetX, offsetY + 133,
							   scaledToHeight(m_devicesPix[UV_OFF], 64).transformed(matrix, Qt::SmoothTransformation));

		}
        break;
    case MACHINE_Genie:
    case MACHINE_EDI:
        setStatePen(painter, !m_deviceState[E3_STATE] && m_deviceState[E1_STATE]);

        //RO产水
        points.clear();
        points.append(QPointF(width() - 610, offsetY + 274));
        points.append(QPointF(width() - 610, offsetY + 140));
        points.append(QPointF(width() - 150, offsetY + 140));
        points.append(QPointF(width() - 150, offsetY + 170));
        painter.drawPolyline(points);

        //EDI弃水
        points.clear();
        points.append(QPointF(width() - 560, offsetY + 140));
        points.append(QPointF(width() - 560, offsetY + 172));
        points.append(QPointF(width() - 500, offsetY + 172));
        points.append(QPointF(width() - 500, offsetY + 430));
        painter.drawPolyline(points);

        if(!m_deviceState[E3_STATE] && m_deviceState[E1_STATE])
        {
            //产水阀
            matrix.rotate(-90);
            painter.drawPixmap(width() - offsetX, offsetY + 190,
                               scaledToWidth(m_devicesPix[VALVE_ON], 28).transformed(matrix, Qt::SmoothTransformation));

            //EDI
            offsetX -= 80;
            if(m_deviceState[EDI_STATE])
            {
                painter.drawPixmap(width() - offsetX, offsetY + 130, scaledToWidth(m_devicesPix[EDI_ON], 28));
            }
            else
            {
                painter.drawPixmap(width() - offsetX, offsetY + 130, scaledToWidth(m_devicesPix[EDI_OFF], 28));
            }
            //EDI电阻率电极
            offsetX -= 80;
            painter.drawPixmap(width() - offsetX, offsetY + 126, scaledToWidth(m_devicesPix[I_ON], 28));
        }
        else
        {
            //产水阀
            matrix.rotate(-90);
            painter.drawPixmap(width() - offsetX, offsetY + 190,
                               scaledToWidth(m_devicesPix[VALVE_OFF], 28).transformed(matrix, Qt::SmoothTransformation));

            //EDI
            offsetX -= 80;
            if(m_deviceState[EDI_STATE])
            {
                painter.drawPixmap(width() - offsetX, offsetY + 130, scaledToWidth(m_devicesPix[EDI_ON], 28));
            }
            else
            {
                painter.drawPixmap(width() - offsetX, offsetY + 130, scaledToWidth(m_devicesPix[EDI_OFF], 28));
            }

            //EDI电阻率电极
            offsetX -= 80;
            painter.drawPixmap(width() - offsetX, offsetY + 126, scaledToWidth(m_devicesPix[I_OFF], 28));
        }
		
	    //纯水紫外灯
        offsetX -= 110;		
		if (gCMUsage.ulUsageState & (1 << DISP_N1_UVLIFEDAY) 
			|| gCMUsage.ulUsageState & (1 << DISP_N1_UVLIFEHOUR))
		{
            painter.drawPixmap(width() - offsetX, offsetY + 133,
						       scaledToHeight(m_devicesPix[UV_YELLOW], 64).transformed(matrix, Qt::SmoothTransformation));

		}
		else if(m_deviceState[N1_STATE])
		{
			painter.drawPixmap(width() - offsetX, offsetY + 133,
						   	   scaledToHeight(m_devicesPix[UV_ON], 64).transformed(matrix, Qt::SmoothTransformation));
		}
		else
		{
			painter.drawPixmap(width() - offsetX, offsetY + 133,
						   	   scaledToHeight(m_devicesPix[UV_OFF], 64).transformed(matrix, Qt::SmoothTransformation));
		}
        break;
    case MACHINE_RO:
    case MACHINE_UP:
        setStatePen(painter, !m_deviceState[E3_STATE] && m_deviceState[E1_STATE]);

        //RO产水
        points.clear();
        points.append(QPointF(width() - 610, offsetY + 274));
        points.append(QPointF(width() - 610, offsetY + 140));
        points.append(QPointF(width() - 150, offsetY + 140));
        points.append(QPointF(width() - 150, offsetY + 170));
        painter.drawPolyline(points);

        if(!m_deviceState[E3_STATE] && m_deviceState[E1_STATE])
        {
            //产水阀
            matrix.rotate(-90);
            painter.drawPixmap(width() - offsetX, offsetY + 190,
                               scaledToWidth(m_devicesPix[VALVE_ON], 28).transformed(matrix, Qt::SmoothTransformation));
        }
        else
        {
            //产水阀
            matrix.rotate(-90);
            painter.drawPixmap(width() - offsetX, offsetY + 190,
                               scaledToWidth(m_devicesPix[VALVE_OFF], 28).transformed(matrix, Qt::SmoothTransformation));
        }
        break;
    case MACHINE_ADAPT:
        setStatePen(painter, !m_deviceState[E3_STATE] && m_deviceState[E1_STATE]);

        //RO产水
        points.clear();
        points.append(QPointF(width() - 610, offsetY + 274));
        points.append(QPointF(width() - 610, offsetY + 140));
        points.append(QPointF(width() - 480, offsetY + 140));
        points.append(QPointF(width() - 480, offsetY + 185));
        painter.drawPolyline(points);

        //产水阀
        if(!m_deviceState[E3_STATE] && m_deviceState[E1_STATE])
        {
            matrix.rotate(-90);
            painter.drawPixmap(width() - offsetX, offsetY + 190,
                               scaledToWidth(m_devicesPix[VALVE_ON], 28).transformed(matrix, Qt::SmoothTransformation));
        }
        else
        {
            matrix.rotate(-90);
            painter.drawPixmap(width() - offsetX, offsetY + 190,
                               scaledToWidth(m_devicesPix[VALVE_OFF], 28).transformed(matrix, Qt::SmoothTransformation));
        }
        break;
    default:
        break;
    }
}

//绘制纯水箱部分
void DFlowChartWidget::paintPureTank(QPainter &painter)
{
    int offsetX = 121;
    int offsetY = 40;

    QPolygonF points;
    QPen pen(Qt::gray, 3);
    painter.setPen(pen);

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_UP:
        setStatePen(painter, m_deviceState[E6_STATE] || (!m_deviceState[E3_STATE] && m_deviceState[E1_STATE]));

        //进纯水箱
        points.clear();
        points.append(QPointF(width() - 150, offsetY + 170));
        //不带水箱循环，则不添加下面四个坐标点
        if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir))
        {
            points.append(QPointF(width() - 114, offsetY + 170));
            points.append(QPointF(width() - 114, offsetY + 185));
            points.append(QPointF(width() - 101, offsetY + 185));
            points.append(QPointF(width() - 101, offsetY + 170));
        }
        points.append(QPointF(width() - 25, offsetY + 170));
        points.append(QPointF(width() - 25, offsetY + 200));
        painter.drawPolyline(points);

        if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir))
        {
            if(m_deviceState[E6_STATE] || (!m_deviceState[E3_STATE] && m_deviceState[E1_STATE]))
            {
                //T Pack
            	if (gCMUsage.ulUsageState & (1 << DISP_T_PACKLIFEDAY)
                	|| gCMUsage.ulUsageState & (1 << DISP_T_PACKLIFEL))
            	{
            		painter.drawPixmap(width() - offsetX, offsetY + 179, scaledToWidth(m_devicesPix[PACK_YELLOW], 28));
            	}
				else
				{
					painter.drawPixmap(width() - offsetX, offsetY + 179, scaledToWidth(m_devicesPix[PACK_ON], 28));
				}
            }
            else
            {
                //T Pack
                if (gCMUsage.ulUsageState & (1 << DISP_T_PACKLIFEDAY)
                	|| gCMUsage.ulUsageState & (1 << DISP_T_PACKLIFEL))
            	{
            		painter.drawPixmap(width() - offsetX, offsetY + 179, scaledToWidth(m_devicesPix[PACK_YELLOW], 28));
            	}
				else
				{
					painter.drawPixmap(width() - offsetX, offsetY + 179, scaledToWidth(m_devicesPix[PACK_OFF], 28));
				}
            }
        }

        //纯水箱
        offsetX -= 71;
        painter.drawPixmap(width() - offsetX, offsetY + 200, scaledToHeight(m_devicesPix[TANK], 80));
        paintPureTankLevel(painter);

        //水箱紫外灯
        offsetX -= 11;
		if (gCMUsage.ulUsageState & (1 << DISP_N3_UVLIFEDAY) 
        	|| gCMUsage.ulUsageState & (1 << DISP_N3_UVLIFEHOUR))
		{
			painter.drawPixmap(width() - offsetX, offsetY + 200, scaledToHeight(m_devicesPix[TANKUV_YELLOW], 48));
		}
        else if(m_deviceState[N3_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 200, scaledToHeight(m_devicesPix[TANKUV_ON], 48));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 200, scaledToHeight(m_devicesPix[TANKUV_OFF], 48));
        }
        break;
    default:
        break;
    }
}

//绘制取水部分
void DFlowChartWidget::paintDispense(QPainter &painter)
{
    int offsetX = 39;
    int offsetY = 40;

    QPolygonF points;
    QPen pen(Qt::gray, 3);
    painter.setPen(pen);
    QMatrix matrix;
	
    //有终端过滤器到期后，所有手柄均显示为黄色
	bool bYellow = false;
	if(gCMUsage.ulUsageState & (1 << DISP_T_A_FILTERLIFE) 
	   || gCMUsage.ulUsageState & (1 << DISP_T_A_FILTERLIFE)
	   || gCMUsage.ulUsageState & (1 << DISP_T_B_FILTERLIFE) 
	   || gCMUsage.ulUsageState & (1 << DISP_T_B_FILTERLIFE))
	{
		bYellow = true;
	}

    //UP取水时，设置E7阀工作状态为true
	if(DispGetUpQtwFlag())
	{
		m_deviceState[E7_STATE] = true;
	}
	else
	{
		m_deviceState[E7_STATE] = false;
	}

    //HP取水时，设置E8阀工作状态为true
	if(DispGetEdiQtwFlag())
	{
		m_deviceState[E8_STATE] = true;
	}
	else
	{
		m_deviceState[E8_STATE] = false;
	}

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_Genie:
        /****** 绘制水管 ******/
        //水箱->取水进水和循环交叉点  E4工作
        setStatePen(painter, m_deviceState[E4_STATE]);

        points.clear();
        points.append(QPointF(width() - 25, offsetY + 280));
        points.append(QPointF(width() - 25, offsetY + 420));
        points.append(QPointF(width() - 267, offsetY + 420));
        painter.drawPolyline(points);

        //取水流量计->TOC冲洗阀 取水或循环
        setStatePen(painter, m_deviceState[C2_STATE]);

        points.clear();
        points.append(QPointF(width() - 267, offsetY + 420));
        points.append(QPointF(width() - 459, offsetY + 420));
        points.append(QPointF(width() - 459, offsetY + 385));
        painter.drawPolyline(points);

        //TOC冲洗阀->TOC测量水路交点 (旁路)
        setStatePen(painter, !m_deviceState[E9_STATE] && m_deviceState[C2_STATE]);

        points.clear();
        points.append(QPointF(width() - 459, offsetY + 374));
        points.append(QPointF(width() - 435, offsetY + 374));
        points.append(QPointF(width() - 435, offsetY + 260));
        painter.drawPolyline(points);

        //TOC冲洗阀->TOC测量水路交点
        setStatePen(painter, m_deviceState[E9_STATE]);

        points.clear();
        points.append(QPointF(width() - 459, offsetY + 385));
        points.append(QPointF(width() - 459, offsetY + 260));
        points.append(QPointF(width() - 435, offsetY + 260));
        painter.drawPolyline(points);

        //TOC测量水路交点->I4
        setStatePen(painter, m_deviceState[C2_STATE]);

        points.clear();
        points.append(QPointF(width() - 435, offsetY + 260));
        points.append(QPointF(width() - 370, offsetY + 260));
        painter.drawPolyline(points);

        //HP 取水和循环公用管路
        setStatePen(painter, m_deviceState[E8_STATE] || m_deviceState[E6_STATE]);

        points.clear();
        points.append(QPointF(width() - 370, offsetY + 260));
        points.append(QPointF(width() - 340, offsetY + 260));
        points.append(QPointF(width() - 340, offsetY + 230));
        painter.drawPolyline(points);

        //HP 取水
        setStatePen(painter, m_deviceState[E8_STATE]);

        points.clear();
        points.append(QPointF(width() - 340, offsetY + 230));
        points.append(QPointF(width() - 190, offsetY + 230));
        painter.drawPolyline(points);
        painter.drawText(610, 245, "HP");

        //HP循环
        setStatePen(painter, m_deviceState[E6_STATE]);

        points.clear();
        points.append(QPointF(width() - 340, offsetY + 230));
        points.append(QPointF(width() - 340, offsetY + 170));
        points.append(QPointF(width() - 150, offsetY + 170));
        painter.drawPolyline(points);

        //I5->UP取水和循环交叉点
        setStatePen(painter, m_deviceState[E7_STATE] || m_deviceState[E5_STATE]);

        points.clear();
        points.append(QPointF(width() - 370, offsetY + 260));
        points.append(QPointF(width() - 370, offsetY + 300));
        points.append(QPointF(width() - 337, offsetY + 300));
        points.append(QPointF(width() - 337, offsetY + 315));
        points.append(QPointF(width() - 324, offsetY + 315));
        points.append(QPointF(width() - 324, offsetY + 300));
        points.append(QPointF(width() - 267, offsetY + 300));
        painter.drawPolyline(points);

        //UP取水水管
        setStatePen(painter, m_deviceState[E7_STATE]);

        painter.drawLine(QPointF(width() - 267, offsetY + 300), QPointF(width() - 190, offsetY + 300));
        painter.drawText(610, 315, "UP");

        //UP循环阀水管
        setStatePen(painter, m_deviceState[E5_STATE]);

        painter.drawLine(width() - 267, offsetY + 300, width() - 267, offsetY + 420);

        /****** 绘制器件 ******/
        //取水进水阀
        offsetX += 120;
        if(m_deviceState[E4_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 406, scaledToWidth(m_devicesPix[VALVE_ON], 28));

        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 406, scaledToWidth(m_devicesPix[VALVE_OFF], 28));

        }

        //取水流量计
        if(m_deviceState[C2_STATE])
        {
            //取水流量计
            offsetX += 180;
            painter.drawPixmap(width() - offsetX, offsetY + 415, scaledToWidth(m_devicesPix[FLOWMETER_ON], 28));

            //取水增压泵
            offsetX += 70;
            painter.drawPixmap(width() - offsetX, offsetY + 396, scaledToWidth(m_devicesPix[PUMP_ON], 28));

        }
        else
        {
            //取水流量计
            offsetX += 180;
            painter.drawPixmap(width() - offsetX, offsetY + 415, scaledToWidth(m_devicesPix[FLOWMETER_OFF], 28));

            //取水增压泵
            offsetX += 70;
            painter.drawPixmap(width() - offsetX, offsetY + 396, scaledToWidth(m_devicesPix[PUMP_OFF], 28));
        }

        //TOC清洗阀
        offsetX += 64;
        matrix.rotate(-90);
        if(m_deviceState[C2_STATE] && m_deviceState[E9_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 360,
                               scaledToWidth(m_devicesPix[VALVE2_C_ON], 28).transformed(matrix, Qt::SmoothTransformation));
        }
        else if(m_deviceState[C2_STATE] && !m_deviceState[E9_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 360,
                               scaledToWidth(m_devicesPix[VALVE2_L_ON], 28).transformed(matrix, Qt::SmoothTransformation));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 360,
                               scaledToWidth(m_devicesPix[VALVE2_OFF], 28).transformed(matrix, Qt::SmoothTransformation));
        }

		//N2
        offsetX -= 6;
		if (gCMUsage.ulUsageState & (1 << DISP_N2_UVLIFEDAY) 
        	|| gCMUsage.ulUsageState & (1 << DISP_N2_UVLIFEHOUR))
		{
		    painter.drawPixmap(width() - offsetX, offsetY + 280,
                               scaledToHeight(m_devicesPix[UV_YELLOW], 64));
		}
        else if(m_deviceState[N2_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 280,
                               scaledToHeight(m_devicesPix[UV_ON], 64));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 280,
                               scaledToHeight(m_devicesPix[UV_OFF], 64));
        }

        //I4
        offsetX -= 55;
        if(m_deviceState[C2_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 248, scaledToWidth(m_devicesPix[I_ON], 28));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 248, scaledToWidth(m_devicesPix[I_OFF], 28));

        }

        //U Pack
        offsetX -= 68;
        if (gCMUsage.ulUsageState & (1 << DISP_U_PACKLIFEDAY) 
            || gCMUsage.ulUsageState & (1 << DISP_U_PACKLIFEL))
        {
            painter.drawPixmap(width() - offsetX, offsetY + 310, scaledToWidth(m_devicesPix[PACK_YELLOW], 28));
        }
        else if(m_deviceState[E5_STATE] || m_deviceState[E7_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 310, scaledToWidth(m_devicesPix[PACK_ON], 28));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 310, scaledToWidth(m_devicesPix[PACK_OFF], 28));
        }

        //I5
        offsetX -= 40;
        if(m_deviceState[E5_STATE] || m_deviceState[E7_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 286, scaledToWidth(m_devicesPix[I_ON], 28));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 286, scaledToWidth(m_devicesPix[I_OFF], 28));
        }

        //UP 循环阀
        offsetX -= 23;
        if(m_deviceState[E5_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 330,
                               scaledToWidth(m_devicesPix[VALVE_ON], 28).transformed(matrix, Qt::SmoothTransformation));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 330,
                               scaledToWidth(m_devicesPix[VALVE_OFF], 28).transformed(matrix, Qt::SmoothTransformation));
        }

		//UP 取水阀
        offsetX -= 30;
        if(m_deviceState[E7_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 286, scaledToWidth(m_devicesPix[VALVE_ON], 28));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 286, scaledToWidth(m_devicesPix[VALVE_OFF], 28));
        }

		//UP 手柄
        offsetX -= 50;
		if (bYellow)
		{
			painter.drawPixmap(width() - offsetX, offsetY + 249, scaledToHeight(m_devicesPix[DISPER_YELLOW], 54));
		}
		else if(m_deviceState[E7_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 249, scaledToHeight(m_devicesPix[DISPER_ON], 54));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 249, scaledToHeight(m_devicesPix[DISPER], 54));
        }

		//HP 取水阀
        offsetX += 50;
        if(m_deviceState[E8_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 216, scaledToWidth(m_devicesPix[VALVE_ON], 28));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 216, scaledToWidth(m_devicesPix[VALVE_OFF], 28));
        }

		//HP 手柄
        offsetX -= 50;
		if (bYellow)
		{
		    painter.drawPixmap(width() - offsetX, offsetY + 179, scaledToHeight(m_devicesPix[DISPER_YELLOW], 54));
		}
		else if(m_deviceState[E8_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 179, scaledToHeight(m_devicesPix[DISPER_ON], 54));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 179, scaledToHeight(m_devicesPix[DISPER], 54));
        }

        //HP 循环阀
        offsetX += 50;
        if(m_deviceState[E6_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 156, scaledToWidth(m_devicesPix[VALVE_ON], 28));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 156, scaledToWidth(m_devicesPix[VALVE_OFF], 28));
        }
        break;
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_EDI:
    case MACHINE_RO:
        /****** 绘制水管 ******/
        //水箱->取水和循环交叉点  E4工作
        setStatePen(painter, m_deviceState[E4_STATE]);

        points.clear();
        points.append(QPointF(width() - 25, offsetY + 280));
        points.append(QPointF(width() - 25, offsetY + 420));
        points.append(QPointF(width() - 439, offsetY + 420));
        points.append(QPointF(width() - 439, offsetY + 260));
        points.append(QPointF(width() - 340, offsetY + 260));
        painter.drawPolyline(points);

        //HP 取水
        setStatePen(painter, m_deviceState[E8_STATE]);

        points.clear();
        points.append(QPointF(width() - 340, offsetY + 260));
        points.append(QPointF(width() - 190, offsetY + 260));
        painter.drawPolyline(points);

        painter.drawText(610, 275, "HP");

        //HP循环
        setStatePen(painter, m_deviceState[E6_STATE]);

        points.clear();
        points.append(QPointF(width() - 340, offsetY + 260));
        points.append(QPointF(width() - 340, offsetY + 170));
        points.append(QPointF(width() - 150, offsetY + 170));
        painter.drawPolyline(points);

        /****** 绘制器件 ******/
        //取水进水阀
        if(m_deviceState[E4_STATE])
        {
            offsetX += 120;
            painter.drawPixmap(width() - offsetX, offsetY + 406, scaledToWidth(m_devicesPix[VALVE_ON], 28));

            //取水流量计
            offsetX += 80;
            painter.drawPixmap(width() - offsetX, offsetY + 415, scaledToWidth(m_devicesPix[FLOWMETER_ON], 28));
        }
        else
        {
            offsetX += 120;
            painter.drawPixmap(width() - offsetX, offsetY + 406, scaledToWidth(m_devicesPix[VALVE_OFF], 28));

            //取水流量计
            offsetX += 80;
            painter.drawPixmap(width() - offsetX, offsetY + 415, scaledToWidth(m_devicesPix[FLOWMETER_OFF], 28));
        }

        //取水增压泵
        offsetX += 120;
        if(m_deviceState[C2_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 396, scaledToWidth(m_devicesPix[PUMP_ON], 28));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 396, scaledToWidth(m_devicesPix[PUMP_OFF], 28));
        }

        //EDI : I4; RO : I3
        offsetX += 48;
        if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir))
        {
            if(m_deviceState[E4_STATE])
            {
                painter.drawPixmap(width() - offsetX, offsetY + 248, scaledToWidth(m_devicesPix[I_ON], 28));
            }
            else
            {
                painter.drawPixmap(width() - offsetX, offsetY + 248, scaledToWidth(m_devicesPix[I_OFF], 28));
            }
        }

	    //HP 取水阀
        offsetX -= 156;
        if(m_deviceState[E8_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 246, scaledToWidth(m_devicesPix[VALVE_ON], 28));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 246, scaledToWidth(m_devicesPix[VALVE_OFF], 28));
        }
		
		//HP 手柄
        offsetX -= 50;
		if (bYellow)
		{
			painter.drawPixmap(width() - offsetX, offsetY + 209, scaledToHeight(m_devicesPix[DISPER_YELLOW], 54));
		}
		else if(m_deviceState[E8_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 209, scaledToHeight(m_devicesPix[DISPER_ON], 54));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 209, scaledToHeight(m_devicesPix[DISPER], 54));
        }

        //HP 循环阀
        if(m_deviceState[E6_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 156, scaledToWidth(m_devicesPix[VALVE_OFF], 28));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 156, scaledToWidth(m_devicesPix[VALVE_OFF], 28));
        }
        break;
    case MACHINE_L_UP:
    case MACHINE_UP:
        /****** 绘制水管 ******/
        //水箱->取水进水/循环交叉点 E4工作
        setStatePen(painter, m_deviceState[E4_STATE]);
  
        points.clear();
        points.append(QPointF(width() - 25, offsetY + 280));
        points.append(QPointF(width() - 25, offsetY + 420));
        points.append(QPointF(width() - 267 + 64, offsetY + 420));
        painter.drawPolyline(points);

        //取水进水/循环交叉点->HP/UP分支 取水或循环
        setStatePen(painter, m_deviceState[C2_STATE]);

        points.clear();
        points.append(QPointF(width() - 203, offsetY + 420));
        points.append(QPointF(width() - 490, offsetY + 420));
        points.append(QPointF(width() - 490, offsetY + 260));
        painter.drawPolyline(points);

        if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
        {
            //HP/UP分支->UP
            setStatePen(painter, m_deviceState[E5_STATE] || m_deviceState[E7_STATE]);

            points.clear();
            points.append(QPointF(width() - 490, offsetY + 260));
            points.append(QPointF(width() - 467, offsetY + 260));
            points.append(QPointF(width() - 467, offsetY + 275));
            points.append(QPointF(width() - 455, offsetY + 275));
            points.append(QPointF(width() - 455, offsetY + 260));
            points.append(QPointF(width() - 420, offsetY + 260));
            points.append(QPointF(width() - 420, offsetY + 280));
            painter.drawPolyline(points);

            //TOC冲洗阀->TOC测量水路交点
            setStatePen(painter, m_deviceState[E9_STATE]);
  
            points.clear();
            points.append(QPointF(width() - 420, offsetY + 285));
            points.append(QPointF(width() - 420, offsetY + 390));
            points.append(QPointF(width() - 395, offsetY + 390));
            painter.drawPolyline(points);

            //TOC冲洗阀->TOC测量水路交点 (旁路)
            setStatePen(painter, !m_deviceState[E9_STATE] && (m_deviceState[E5_STATE] || m_deviceState[E7_STATE]));

            points.clear();
            points.append(QPointF(width() - 410, offsetY + 288));
            points.append(QPointF(width() - 395, offsetY + 288));
            points.append(QPointF(width() - 395, offsetY + 390));
            painter.drawPolyline(points);

            //TOC测量水路交点 ->UP取水和循环交叉点
            setStatePen(painter, m_deviceState[E5_STATE] || m_deviceState[E7_STATE]);

            points.clear();
            points.append(QPointF(width() - 395, offsetY + 390));
            points.append(QPointF(width() - 370, offsetY + 390));
            points.append(QPointF(width() - 370, offsetY + 300));
            points.append(QPointF(width() - 286, offsetY + 300));
            points.append(QPointF(width() - 285, offsetY + 315));
            points.append(QPointF(width() - 273, offsetY + 315));
            points.append(QPointF(width() - 273, offsetY + 300));
            points.append(QPointF(width() - 203, offsetY + 300));
            painter.drawPolyline(points);
        }
        else
        {
            //HP/UP分支->UP
            setStatePen(painter, m_deviceState[E5_STATE] || m_deviceState[E7_STATE]);

            points.clear();
            points.append(QPointF(width() - 490, offsetY + 260));
            points.append(QPointF(width() - 467, offsetY + 260));
            points.append(QPointF(width() - 467, offsetY + 275));
            points.append(QPointF(width() - 455, offsetY + 275));
            points.append(QPointF(width() - 455, offsetY + 260));
            points.append(QPointF(width() - 420, offsetY + 260));
            points.append(QPointF(width() - 420, offsetY + 390));
            points.append(QPointF(width() - 370, offsetY + 390));
            points.append(QPointF(width() - 370, offsetY + 300));
            points.append(QPointF(width() - 286, offsetY + 300));
            points.append(QPointF(width() - 285, offsetY + 315));
            points.append(QPointF(width() - 273, offsetY + 315));
            points.append(QPointF(width() - 273, offsetY + 300));
            points.append(QPointF(width() - 203, offsetY + 300));
            painter.drawPolyline(points);
        }

        //UP取水水管
        setStatePen(painter, m_deviceState[E7_STATE]);

        painter.drawLine(QPointF(width() - 203, offsetY + 300),
                         QPointF(width() - 126, offsetY + 300));
        painter.drawText(674, 315, "UP");

        //UP循环阀水管
        setStatePen(painter, m_deviceState[E5_STATE]);

        painter.drawLine(QPointF(width() - 203, offsetY + 300),
                         QPointF(width() - 203, offsetY + 420));

        //HP 取水和循环公用管路
        setStatePen(painter, m_deviceState[E6_STATE] || m_deviceState[E8_STATE]);

        points.clear();
        points.append(QPointF(width() - 490, offsetY + 260));
        points.append(QPointF(width() - 490, offsetY + 230));
        points.append(QPointF(width() - 340, offsetY + 230));
        painter.drawPolyline(points);

        //HP 取水
        setStatePen(painter, m_deviceState[E8_STATE]);

        points.clear();
        points.append(QPointF(width() - 340, offsetY + 230));
        points.append(QPointF(width() - 190, offsetY + 230));
        painter.drawPolyline(points);

        painter.drawText(610, 245, "HP");

        //HP循环
        setStatePen(painter, m_deviceState[E6_STATE]);

        points.clear();
        points.append(QPointF(width() - 340, offsetY + 230));
        points.append(QPointF(width() - 340, offsetY + 170));
        points.append(QPointF(width() - 150, offsetY + 170));
        painter.drawPolyline(points);

        /****** 绘制器件 ******/
        //取水进水阀
        offsetX += 56;
        if(m_deviceState[E4_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 406, scaledToWidth(m_devicesPix[VALVE_ON], 28));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 406, scaledToWidth(m_devicesPix[VALVE_OFF], 28));
        }

        if(m_deviceState[C2_STATE])
        {
            //取水流量计
            offsetX += 180;
            painter.drawPixmap(width() - offsetX, offsetY + 415, scaledToWidth(m_devicesPix[FLOWMETER_ON], 28));

            //取水增压泵
            offsetX += 70;
            painter.drawPixmap(width() - offsetX, offsetY + 396, scaledToWidth(m_devicesPix[PUMP_ON], 28));
        }
        else
        {
            //取水流量计
            offsetX += 180;
            painter.drawPixmap(width() - offsetX, offsetY + 415, scaledToWidth(m_devicesPix[FLOWMETER_OFF], 28));

            //取水增压泵
            offsetX += 70;
            painter.drawPixmap(width() - offsetX, offsetY + 396, scaledToWidth(m_devicesPix[PUMP_OFF], 28));
        }

        //H Pack
        offsetX += 130;
        if (gCMUsage.ulUsageState & (1 << DISP_H_PACKLIFEDAY) 
            || gCMUsage.ulUsageState & (1 << DISP_H_PACKLIFEL))
        {
            painter.drawPixmap(width() - offsetX, offsetY + 269, scaledToWidth(m_devicesPix[PACK_YELLOW], 28));
        }
        else if(m_deviceState[E5_STATE] || m_deviceState[E7_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 269, scaledToWidth(m_devicesPix[PACK_ON], 28));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 269, scaledToWidth(m_devicesPix[PACK_OFF], 28));
        }

        //NOTE: I3 配置水箱则配置I3电极用于检测水箱循环水质
        if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir))
        {
            if(m_deviceState[C2_STATE])
            {
                painter.drawPixmap(width() - offsetX + 5, offsetY + 406, scaledToWidth(m_devicesPix[I_ON], 28));
            }
            else
            {
                painter.drawPixmap(width() - offsetX + 5, offsetY + 406, scaledToWidth(m_devicesPix[I_OFF], 28));
            }
        }

        //TOC清洗阀
        offsetX -= 40; //94
        matrix.rotate(-90);
        if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
        {
            if(m_deviceState[E9_STATE])
            {
                painter.drawPixmap(width() - offsetX, offsetY + 275,
                                   scaledToWidth(m_devicesPix[VALVE2_C_ON], 28).transformed(matrix, Qt::SmoothTransformation));
            }
            else if(!m_deviceState[E9_STATE] && (m_deviceState[E5_STATE] || m_deviceState[E7_STATE]))
            {
                painter.drawPixmap(width() - offsetX, offsetY + 275,
                                   scaledToWidth(m_devicesPix[VALVE2_R_ON], 28).transformed(matrix, Qt::SmoothTransformation));
            }
            else
            {
                painter.drawPixmap(width() - offsetX, offsetY + 275,
                                   scaledToWidth(m_devicesPix[VALVE2_OFF], 28).transformed(matrix, Qt::SmoothTransformation));
            }

        }

        //UV
        offsetX -= 7;
		if (gCMUsage.ulUsageState & (1 << DISP_N2_UVLIFEDAY) 
        	|| gCMUsage.ulUsageState & (1 << DISP_N2_UVLIFEHOUR))
		{
		    painter.drawPixmap(width() - offsetX, offsetY + 310,
                               scaledToHeight(m_devicesPix[UV_YELLOW], 64));
		}
        else if(m_deviceState[N2_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 310,
                               scaledToHeight(m_devicesPix[UV_ON], 64));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 310,
                               scaledToHeight(m_devicesPix[UV_OFF], 64));
        }

        //I4
        offsetX -= 85;
        if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
        {
            if(m_deviceState[E5_STATE] || m_deviceState[E7_STATE])
            {
                painter.drawPixmap(width() - offsetX, offsetY + 288, scaledToWidth(m_devicesPix[I_ON], 28));
            }
            else
            {
                painter.drawPixmap(width() - offsetX, offsetY + 288, scaledToWidth(m_devicesPix[I_OFF], 28));
            }
        }

        //U Pack
        offsetX -= 50;
        if (gCMUsage.ulUsageState & (1 << DISP_U_PACKLIFEDAY) 
            || gCMUsage.ulUsageState & (1 << DISP_U_PACKLIFEL))
        {
            painter.drawPixmap(width() - offsetX, offsetY + 310, scaledToWidth(m_devicesPix[PACK_YELLOW], 28));
        }
        else if(m_deviceState[E5_STATE] || m_deviceState[E7_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 310, scaledToWidth(m_devicesPix[PACK_ON], 28));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 310, scaledToWidth(m_devicesPix[PACK_OFF], 28));
        }

        //I5
        offsetX -= 50;
        if(m_deviceState[E5_STATE] || m_deviceState[E7_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 286, scaledToWidth(m_devicesPix[I_ON], 28));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 286, scaledToWidth(m_devicesPix[I_OFF], 28));
        }

        //UP 循环阀
        offsetX -= 26;
        if(m_deviceState[E5_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 330,
                               scaledToWidth(m_devicesPix[VALVE_ON], 28).transformed(matrix, Qt::SmoothTransformation));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 330,
                               scaledToWidth(m_devicesPix[VALVE_OFF], 28).transformed(matrix, Qt::SmoothTransformation));
        }

		//UP 取水阀
        offsetX -= 30;
        if(m_deviceState[E7_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 286, scaledToWidth(m_devicesPix[VALVE_ON], 28));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 286, scaledToWidth(m_devicesPix[VALVE_OFF], 28));
        }

	    //UP 手柄
        offsetX -= 50;
		if (bYellow)
		{
			painter.drawPixmap(width() - offsetX, offsetY + 249, scaledToHeight(m_devicesPix[DISPER_YELLOW], 54));
		}
		else if(m_deviceState[E7_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 249, scaledToHeight(m_devicesPix[DISPER_ON], 54));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 249, scaledToHeight(m_devicesPix[DISPER], 54));
        }

		//HP 取水阀
		offsetX += 114;
        if(m_deviceState[E8_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 216, scaledToWidth(m_devicesPix[VALVE_ON], 28));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 216, scaledToWidth(m_devicesPix[VALVE_OFF], 28));
        }
	
        //HP 手柄
        offsetX -= 50;
		if(bYellow)
		{
			painter.drawPixmap(width() - offsetX, offsetY + 179, scaledToHeight(m_devicesPix[DISPER_YELLOW], 54));
		}
		else if(m_deviceState[E8_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 179, scaledToHeight(m_devicesPix[DISPER_ON], 54));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 179, scaledToHeight(m_devicesPix[DISPER], 54));
        }

        //HP 循环阀
        offsetX += 50;
        if(m_deviceState[E6_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 156, scaledToWidth(m_devicesPix[VALVE_ON], 28));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 156, scaledToWidth(m_devicesPix[VALVE_OFF], 28));
        }
        break;
    case MACHINE_ADAPT:
        /****** 绘制水管 ******/
        setStatePen(painter, m_deviceState[C2_STATE]);

        //RO产水->HP/UP分支
        points.clear();
        points.append(QPointF(width() - 480, offsetY + 185));
        points.append(QPointF(width() - 200, offsetY + 185));
        painter.drawPolyline(points);

        //HP 取水
        setStatePen(painter, m_deviceState[E8_STATE]);

        points.clear();
        points.append(QPointF(width() - 200, offsetY + 185));
        points.append(QPointF(width() - 100, offsetY + 185));
        painter.drawPolyline(points);
        painter.drawText(705, 195, "HP");

        if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
        {
            //HP/UP分支->TOC冲洗阀
            setStatePen(painter, m_deviceState[E5_STATE] || m_deviceState[E7_STATE]);

            points.clear();
            points.append(QPointF(width() - 200, offsetY + 185));
            points.append(QPointF(width() - 200, offsetY + 264));
            points.append(QPointF(width() - 225, offsetY + 264));
            painter.drawPolyline(points);

            //TOC冲洗阀->UV(TOC测量水路交点)
            setStatePen(painter, m_deviceState[E9_STATE]);

            points.clear();
            points.append(QPointF(width() - 225, offsetY + 264));
            points.append(QPointF(width() - 345, offsetY + 264));
            painter.drawPolyline(points);

            //TOC冲洗阀->TOC测量水路交点 (旁路)
            setStatePen(painter, !m_deviceState[E9_STATE] && (m_deviceState[E5_STATE] || m_deviceState[E7_STATE]));

            points.clear();
            points.append(QPointF(width() - 236, offsetY + 280));
            points.append(QPointF(width() - 236, offsetY + 290));
            points.append(QPointF(width() - 345, offsetY + 290));
            points.append(QPointF(width() - 345, offsetY + 264));
            painter.drawPolyline(points);

            //TOC测量水路交点->UP取水和循环交叉点
            setStatePen(painter, m_deviceState[E5_STATE] || m_deviceState[E7_STATE]);

            points.clear();
            points.append(QPointF(width() - 345, offsetY + 264));
            points.append(QPointF(width() - 420, offsetY + 264));
            points.append(QPointF(width() - 420, offsetY + 279));
            points.append(QPointF(width() - 433, offsetY + 279));
            points.append(QPointF(width() - 433, offsetY + 264));
            points.append(QPointF(width() - 460, offsetY + 264));
            points.append(QPointF(width() - 460, offsetY + 366));
            points.append(QPointF(width() - 200, offsetY + 366));
            painter.drawPolyline(points);
        }
        else
        {
            //HP/UP分支->UP取水和循环交叉点
            setStatePen(painter, m_deviceState[E5_STATE] || m_deviceState[E7_STATE]);

            points.clear();
            points.append(QPointF(width() - 200, offsetY + 185));
            points.append(QPointF(width() - 200, offsetY + 264));
            points.append(QPointF(width() - 420, offsetY + 264));
            points.append(QPointF(width() - 420, offsetY + 279));
            points.append(QPointF(width() - 433, offsetY + 279));
            points.append(QPointF(width() - 433, offsetY + 264));
            points.append(QPointF(width() - 460, offsetY + 264));
            points.append(QPointF(width() - 460, offsetY + 366));
            points.append(QPointF(width() - 200, offsetY + 366));
            painter.drawPolyline(points);
        }

        //UP 取水
        setStatePen(painter, m_deviceState[E7_STATE]);
        points.clear();
        points.append(QPointF(width() - 200, offsetY + 366));
        points.append(QPointF(width() - 100, offsetY + 366));
        painter.drawPolyline(points);
        painter.drawText(705, 376, "UP");

        //UP 循环
        setStatePen(painter, m_deviceState[E5_STATE]);

        points.clear();
        points.append(QPointF(width() - 200, offsetY + 366));
        points.append(QPointF(width() - 200, offsetY + 416));
        points.append(QPointF(width() - 480, offsetY + 416));
        points.append(QPointF(width() - 480, offsetY + 185));
        painter.drawPolyline(points);

        /****** 绘制器件 ******/
        if(m_deviceState[C2_STATE])
        {
            offsetX = 430;
            matrix.rotate(180);
            //取水流量计
            painter.drawPixmap(width() - offsetX, offsetY + 180,
                               scaledToWidth(m_devicesPix[FLOWMETER_ON], 28).transformed(matrix, Qt::SmoothTransformation));

            //取水增压泵
            offsetX -= 80;
            painter.drawPixmap(width() - offsetX, offsetY + 161, scaledToWidth(m_devicesPix[PUMP_ON], 28));
        }
        else
        {
            offsetX = 430;
            matrix.rotate(180);
            //取水流量计
            painter.drawPixmap(width() - offsetX, offsetY + 180,
                               scaledToWidth(m_devicesPix[FLOWMETER_OFF], 28).transformed(matrix, Qt::SmoothTransformation));

            //取水增压泵
            offsetX -= 80;
            painter.drawPixmap(width() - offsetX, offsetY + 161, scaledToWidth(m_devicesPix[PUMP_OFF], 28));
        }

		//HP 取水阀
        offsetX -= 180;
        if(m_deviceState[E8_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 171, scaledToWidth(m_devicesPix[VALVE_ON], 28));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 171, scaledToWidth(m_devicesPix[VALVE_OFF], 28));
        }

		//HP 手柄
        offsetX -= 60;
		if(bYellow)
		{
			painter.drawPixmap(width() - offsetX, offsetY + 134, scaledToHeight(m_devicesPix[DISPER_YELLOW], 54));
		}
		else if(m_deviceState[E8_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 134, scaledToHeight(m_devicesPix[DISPER_ON], 54));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 134, scaledToHeight(m_devicesPix[DISPER], 54));
        }

        //TOC清洗阀
        offsetX += 140;
        if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
        {
            matrix.rotate(180);
            if(m_deviceState[E9_STATE])
            {
                painter.drawPixmap(width() - offsetX, offsetY + 250,
                                   scaledToWidth(m_devicesPix[VALVE2_C_ON], 28).transformed(matrix, Qt::SmoothTransformation));
            }
            else if(!m_deviceState[E9_STATE] && (m_deviceState[E5_STATE] || m_deviceState[E7_STATE]))
            {
                painter.drawPixmap(width() - offsetX, offsetY + 250,
                                   scaledToWidth(m_devicesPix[VALVE2_R_ON], 28).transformed(matrix, Qt::SmoothTransformation));
            }
            else
            {
                painter.drawPixmap(width() - offsetX, offsetY + 250,
                                   scaledToWidth(m_devicesPix[VALVE2_OFF], 28).transformed(matrix, Qt::SmoothTransformation));
            }

        }

        //N2 UV
        offsetX += 80;
        matrix.rotate(90);
		if (gCMUsage.ulUsageState & (1 << DISP_N2_UVLIFEDAY) 
        	|| gCMUsage.ulUsageState & (1 << DISP_N2_UVLIFEHOUR))
		{
		    painter.drawPixmap(width() - offsetX, offsetY + 257,
                               scaledToHeight(m_devicesPix[UV_YELLOW], 64).transformed(matrix, Qt::SmoothTransformation));
		}
        else if(m_deviceState[N2_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 257,
                               scaledToHeight(m_devicesPix[UV_ON], 64).transformed(matrix, Qt::SmoothTransformation));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 257,
                               scaledToHeight(m_devicesPix[UV_OFF], 64).transformed(matrix, Qt::SmoothTransformation));
        }

        if(m_deviceState[E5_STATE] || m_deviceState[E7_STATE])
        {
            //I4
            offsetX += 60;
            if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
            {
                painter.drawPixmap(width() - offsetX, offsetY + 248, scaledToWidth(m_devicesPix[I_ON], 28));
            }

            //U Pack
            offsetX += 50;
			if (gCMUsage.ulUsageState & (1 << DISP_U_PACKLIFEDAY) 
                || gCMUsage.ulUsageState & (1 << DISP_U_PACKLIFEL))
			{
				painter.drawPixmap(width() - offsetX, offsetY + 275, scaledToWidth(m_devicesPix[PACK_YELLOW], 28));
			}
			else
			{
				painter.drawPixmap(width() - offsetX, offsetY + 275, scaledToWidth(m_devicesPix[PACK_ON], 28));
			}
            
            //I5
            offsetX -= 80;
            painter.drawPixmap(width() - offsetX, offsetY + 350, scaledToWidth(m_devicesPix[I_ON], 28));
        }
        else
        {
            //I4
            offsetX += 60;
            if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
            {
                painter.drawPixmap(width() - offsetX, offsetY + 248, scaledToWidth(m_devicesPix[I_OFF], 28));
            }

            //U Pack
            offsetX += 50;
			if (gCMUsage.ulUsageState & (1 << DISP_U_PACKLIFEDAY) 
                || gCMUsage.ulUsageState & (1 << DISP_U_PACKLIFEL))
			{
				painter.drawPixmap(width() - offsetX, offsetY + 275, scaledToWidth(m_devicesPix[PACK_YELLOW], 28));
			}
			else
			{
				painter.drawPixmap(width() - offsetX, offsetY + 275, scaledToWidth(m_devicesPix[PACK_OFF], 28));
			}

            //I5
            offsetX -= 80;
            painter.drawPixmap(width() - offsetX, offsetY + 350, scaledToWidth(m_devicesPix[I_OFF], 28));
        }

		//UP 取水阀
        offsetX -= 190;
        if(m_deviceState[E7_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 352, scaledToWidth(m_devicesPix[VALVE_ON], 28));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 352, scaledToWidth(m_devicesPix[VALVE_OFF], 28));
        }

		//UP 手柄
        offsetX -= 60;
		if(bYellow)
		{
		    painter.drawPixmap(width() - offsetX, offsetY + 315, scaledToHeight(m_devicesPix[DISPER_YELLOW], 54));
		}
        else if(m_deviceState[E7_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 315, scaledToHeight(m_devicesPix[DISPER_ON], 54));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 315, scaledToHeight(m_devicesPix[DISPER], 54));
        }

        //UP循环阀
        offsetX += 150;
        if(m_deviceState[E5_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 402, scaledToWidth(m_devicesPix[VALVE_ON], 28));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 402, scaledToWidth(m_devicesPix[VALVE_OFF], 28));
        }
        break;
    case MACHINE_PURIST:
        offsetX = 150;
        offsetY = 120;
        /****** 绘制水管 ******/
        setStatePen(painter, m_deviceState[E4_STATE]);

        //进水->进水循环交叉点
        points.clear();
        points.append(QPointF(width() - 100, offsetY + 264));
        points.append(QPointF(width() - 360, offsetY + 264));
        painter.drawPolyline(points);

        if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
        {
            //进水循环交叉点->TOC冲洗阀
            setStatePen(painter, m_deviceState[C2_STATE]);

            points.clear();
            points.append(QPointF(width() - 360, offsetY + 264));
            points.append(QPointF(width() - 700, offsetY + 264));
            points.append(QPointF(width() - 700, offsetY + 44));
            points.append(QPointF(width() - 649, offsetY + 44));
            points.append(QPointF(width() - 649, offsetY + 59));
            points.append(QPointF(width() - 636, offsetY + 59));
            points.append(QPointF(width() - 636, offsetY + 44));
            points.append(QPointF(width() - 575, offsetY + 44));
            painter.drawPolyline(points);

            //TOC冲洗阀->TOC检测交叉点
            setStatePen(painter, m_deviceState[E9_STATE]);

            points.clear();
            points.append(QPointF(width() - 575, offsetY + 44));
            points.append(QPointF(width() - 425, offsetY + 44));
            painter.drawPolyline(points);

            //TOC冲洗阀->TOC检测交叉点(旁路)
            setStatePen(painter, !m_deviceState[E9_STATE] && m_deviceState[C2_STATE]);

            points.clear();
            points.append(QPointF(width() - 563, offsetY + 57));
            points.append(QPointF(width() - 563, offsetY + 70));
            points.append(QPointF(width() - 425, offsetY + 70));
            points.append(QPointF(width() - 425, offsetY + 44));
            painter.drawPolyline(points);

            //TOC检测交叉点(旁路)->取水循环交点
            setStatePen(painter, m_deviceState[C2_STATE]);
            
            points.clear();
            points.append(QPointF(width() - 425, offsetY + 44));
            points.append(QPointF(width() - 339, offsetY + 44));
            points.append(QPointF(width() - 339, offsetY + 59));
            points.append(QPointF(width() - 326, offsetY + 59));
            points.append(QPointF(width() - 326, offsetY + 44));
            points.append(QPointF(width() - 225, offsetY + 44));
            painter.drawPolyline(points);
        }
        else
        {
            //进水循环交叉点->取水循环交点
            setStatePen(painter, m_deviceState[C2_STATE]);

            points.clear();
            points.append(QPointF(width() - 360, offsetY + 264));
            points.append(QPointF(width() - 700, offsetY + 264));
            points.append(QPointF(width() - 700, offsetY + 44));
            points.append(QPointF(width() - 649, offsetY + 44));
            points.append(QPointF(width() - 649, offsetY + 59));
            points.append(QPointF(width() - 636, offsetY + 59));
            points.append(QPointF(width() - 636, offsetY + 44));
            points.append(QPointF(width() - 339, offsetY + 44));
            points.append(QPointF(width() - 339, offsetY + 59));
            points.append(QPointF(width() - 326, offsetY + 59));
            points.append(QPointF(width() - 326, offsetY + 44));
            points.append(QPointF(width() - 225, offsetY + 44));
            painter.drawPolyline(points);
        }

        //取水循环交点->取水
        setStatePen(painter, m_deviceState[E7_STATE]);

        points.clear();
        points.append(QPointF(width() - 225, offsetY + 44));
        points.append(QPointF(width() - 120, offsetY + 44));
        painter.drawPolyline(points);

        //取水循环交点->循环
        setStatePen(painter, m_deviceState[E5_STATE]);

        points.clear();
        points.append(QPointF(width() - 225, offsetY + 44));
        points.append(QPointF(width() - 225, offsetY + 140));
        points.append(QPointF(width() - 360, offsetY + 140));
        points.append(QPointF(width() - 360, offsetY + 264));
        painter.drawPolyline(points);

        /****** 绘制器件 ******/
        if(m_deviceState[E4_STATE])
        {
            //取水进水阀
            offsetX += 56;
            painter.drawPixmap(width() - offsetX, offsetY + 250, scaledToWidth(m_devicesPix[VALVE_ON], 28));

            //I2 进水流量计
            offsetX += 80;
            painter.drawPixmap(width() - offsetX, offsetY + 250, scaledToWidth(m_devicesPix[I_ON], 28));
        }
        else
        {
            //取水进水阀
            offsetX += 56;
            painter.drawPixmap(width() - offsetX, offsetY + 250, scaledToWidth(m_devicesPix[VALVE_OFF], 28));

            //I2 进水流量计
            offsetX += 80;
            painter.drawPixmap(width() - offsetX, offsetY + 250, scaledToWidth(m_devicesPix[I_OFF], 28));
        }


        if(m_deviceState[C2_STATE])
        {
            //取水流量计
            offsetX += 180;
            painter.drawPixmap(width() - offsetX, offsetY + 259, scaledToWidth(m_devicesPix[FLOWMETER_ON], 28));

            //取水增压泵
            offsetX += 110;
            painter.drawPixmap(width() - offsetX, offsetY + 240, scaledToWidth(m_devicesPix[PUMP_ON], 28));
        }
        else
        {
            //取水流量计
            offsetX += 180;
            painter.drawPixmap(width() - offsetX, offsetY + 259, scaledToWidth(m_devicesPix[FLOWMETER_OFF], 28));

            //取水增压泵
            offsetX += 110;
            painter.drawPixmap(width() - offsetX, offsetY + 240, scaledToWidth(m_devicesPix[PUMP_OFF], 28));  
        }

        //H Pack
        offsetX += 80;
        if (gCMUsage.ulUsageState & (1 << DISP_H_PACKLIFEDAY) 
            || gCMUsage.ulUsageState & (1 << DISP_H_PACKLIFEL))
        {
            painter.drawPixmap(width() - offsetX, offsetY + 55, scaledToWidth(m_devicesPix[PACK_YELLOW], 28));
        }
        else if(m_deviceState[C2_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 55, scaledToWidth(m_devicesPix[PACK_ON], 28));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 55, scaledToWidth(m_devicesPix[PACK_OFF], 28));
        }

        //TOC清洗阀
        offsetX -= 80;
        if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
        {
            if(m_deviceState[E9_STATE])
            {
                painter.drawPixmap(width() - offsetX, offsetY + 30, scaledToWidth(m_devicesPix[VALVE2_C_ON], 28));
            }
            else if(!m_deviceState[E9_STATE] && m_deviceState[C2_STATE])
            {
                painter.drawPixmap(width() - offsetX, offsetY + 30, scaledToWidth(m_devicesPix[VALVE2_L_ON], 28));
            }
            else
            {
                painter.drawPixmap(width() - offsetX, offsetY + 30, scaledToWidth(m_devicesPix[VALVE2_OFF], 28));
            }
        }

        //N2 UV
        offsetX -= 60;
        matrix.rotate(90);
		if (gCMUsage.ulUsageState & (1 << DISP_N2_UVLIFEDAY) 
        	|| gCMUsage.ulUsageState & (1 << DISP_N2_UVLIFEHOUR))
		{
		    painter.drawPixmap(width() - offsetX, offsetY + 37,
                               scaledToHeight(m_devicesPix[UV_YELLOW], 64).transformed(matrix, Qt::SmoothTransformation));
		}
        else if(m_deviceState[N2_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 37,
                               scaledToHeight(m_devicesPix[UV_ON], 64).transformed(matrix, Qt::SmoothTransformation));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 37,
                               scaledToHeight(m_devicesPix[UV_OFF], 64).transformed(matrix, Qt::SmoothTransformation));
        }

        //I4
        offsetX -= 110;
        if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
        {
            if(m_deviceState[C2_STATE])
            {
                painter.drawPixmap(width() - offsetX, offsetY + 30, scaledToWidth(m_devicesPix[I_ON], 28));
            }
            else
            {
                painter.drawPixmap(width() - offsetX, offsetY + 30, scaledToWidth(m_devicesPix[I_OFF], 28));
            }

        }

        //U Pack
        offsetX -= 60;
        if (gCMUsage.ulUsageState & (1 << DISP_U_PACKLIFEDAY) 
            || gCMUsage.ulUsageState & (1 << DISP_U_PACKLIFEL))
        {
            painter.drawPixmap(width() - offsetX, offsetY + 55, scaledToWidth(m_devicesPix[PACK_YELLOW], 28));
        }
        else if(m_deviceState[C2_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 55, scaledToWidth(m_devicesPix[PACK_ON], 28));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 55, scaledToWidth(m_devicesPix[PACK_OFF], 28));
        }

        //I5
        offsetX -= 60;
        if(m_deviceState[C2_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 30, scaledToWidth(m_devicesPix[I_ON], 28));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 30, scaledToWidth(m_devicesPix[I_OFF], 28));
        }

		//UP 取水阀
        offsetX -= 100;
        if(m_deviceState[E7_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 30, scaledToWidth(m_devicesPix[VALVE_ON], 28));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 30, scaledToWidth(m_devicesPix[VALVE_OFF], 28));
        }

		//UP 手柄
        offsetX -= 60;
		if(bYellow)
		{
		    painter.drawPixmap(width() - offsetX, offsetY - 7, scaledToHeight(m_devicesPix[DISPER_YELLOW], 54));
		}
		else if(m_deviceState[E7_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY - 7, scaledToHeight(m_devicesPix[DISPER_ON], 54));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY - 7, scaledToHeight(m_devicesPix[DISPER], 54));
        }

        //UP 循环阀
        offsetX += 241;
        if(m_deviceState[E5_STATE])
        {
            painter.drawPixmap(width() - offsetX, offsetY + 180,
                               scaledToWidth(m_devicesPix[VALVE_ON], 28).transformed(matrix, Qt::SmoothTransformation));
        }
        else
        {
            painter.drawPixmap(width() - offsetX, offsetY + 180,
                               scaledToWidth(m_devicesPix[VALVE_OFF], 28).transformed(matrix, Qt::SmoothTransformation));
        }
        break;
    default:
        break;
    }
}

//绘制原水箱液位
void DFlowChartWidget::paintSourceTankLevel(QPainter &painter)
{
    if(m_iSourceTankLevel <= 0)
    {
        return;
    }

    int iTankLevel = m_iSourceTankLevel > 100 ? 100 : m_iSourceTankLevel;
    QColor levelColor(60,190,235);
    if(iTankLevel < 10)
    {
        levelColor.setRgb(255,86,53);
    }
    QPen oldPen = painter.pen();
    QBrush oldBrush = painter.brush();
    painter.setPen(QPen(levelColor, 1, Qt::DotLine));
    painter.setBrush(QBrush(levelColor));
    painter.setRenderHint(QPainter::Antialiasing, true);

    double offset = 52*(1 - iTankLevel*1.0/100);
    QRectF tankLevelRect(QPointF(522, 39 + offset),
                         QPointF(549, 92)); //Tank Level
    painter.drawRect(tankLevelRect);

    painter.setPen(oldPen);
    painter.setBrush(oldBrush);
}

//绘制纯水箱液位
void DFlowChartWidget::paintPureTankLevel(QPainter &painter)
{
    if(m_iTankLevel <= 0)
    {
        return;
    }
    int iTankLevel = m_iTankLevel > 100 ? 100 : m_iTankLevel;
    QColor levelColor(60,190,235);
    if(iTankLevel < 10)
    {
        levelColor.setRgb(255,86,53);
    }
    QPen oldPen = painter.pen();
    QBrush oldBrush = painter.brush();
    painter.setPen(QPen(levelColor, 1, Qt::DotLine));
    painter.setBrush(QBrush(levelColor));
    painter.setRenderHint(QPainter::Antialiasing, true);

    double offset = 52*(1 - iTankLevel*1.0/100);
    QRectF tankLevelRect(QPointF(732, 247 + offset),
                         QPointF(762, 300)); //Tank Level
    painter.drawRect(tankLevelRect);

    painter.setPen(oldPen);
    painter.setBrush(oldBrush);
}

//绘制所有参数
void DFlowChartWidget::paintValue(QPainter &painter)
{
    int offsetX = 5;
    QString strUs  = tr("us");
    int iPrecision = 1;
    double temp;

    QPen oldPen = painter.pen();
    QFont oldFont = painter.font();
    painter.setFont(QFont("", FONTSIZE, QFont::Bold));
    painter.setPen(QPen(QColor(20,146,200))); //60,190,235

    //原水进水参数
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
		if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveB3))
		{
			//原水箱液位
	        if(m_iSourceTankLevel >= 10)
	        {
	            painter.drawText(488, 65, QString("%1%").arg(m_iSourceTankLevel));
	        }
	        else
	        {
	            painter.drawText(496, 65, QString("%1%").arg(m_iSourceTankLevel));
	        }
		}
        break;
    default:
        break;
    }

    //RO进水参数
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        //进水压力
        painter.drawText(353, 75, toString(m_fFeedPressure, 2) + m_strUnit[PRESSURE_UNIT]);

        //进水流量
        painter.drawText(215, 80, toString(m_flowRate[S2_VALUE], 1) + " L/h");

        //进水电导率
        painter.drawText(135, 60,
                         toString(m_waterInfo[I1_VALUE].fQuality) + QString(" ") + strUs);
        painter.drawText(135, 75,
                         toString(m_waterInfo[I1_VALUE].fTemp, 1)  + QString(" ")+ m_strUnit[TEMP_UNIT]);

        //RO电导率
        painter.drawText(80, 280,
                         toString(m_waterInfo[I2_VALUE].fQuality, 1) + QString(" ") + strUs);
        painter.drawText(80, 294,
                         toString(m_waterInfo[I2_VALUE].fTemp, 1)  + QString(" ")+ m_strUnit[TEMP_UNIT]);

        //工作压力
        painter.drawText(47, 355, toString(m_fWorkPressure, 2) + m_strUnit[PRESSURE_UNIT]);
        //弃水流量
        painter.drawText(210, 390, toString(m_flowRate[S4_VALUE], 1));
        painter.drawText(214, 405, "L/h");
        break;
    case MACHINE_Genie:
    case MACHINE_EDI:
    case MACHINE_UP:
    case MACHINE_RO:
    case MACHINE_ADAPT:
        //进水压力
        painter.drawText(510, 75, toString(m_fFeedPressure, 2) + m_strUnit[PRESSURE_UNIT]);

        //进水电导率
        painter.drawText(160, 60,
                         toString(m_waterInfo[I1_VALUE].fQuality) + QString(" ") + strUs);
        painter.drawText(160, 75,
                         toString(m_waterInfo[I1_VALUE].fTemp, 1)  + QString(" ")+ m_strUnit[TEMP_UNIT]);

        //RO电导率
        painter.drawText(80, 280,
                         toString(m_waterInfo[I2_VALUE].fQuality, 1) + QString(" ") + strUs);
        painter.drawText(80, 294,
                         toString(m_waterInfo[I2_VALUE].fTemp, 1)  + QString(" ")+ m_strUnit[TEMP_UNIT]);

        //工作压力
        painter.drawText(47, 355, toString(m_fWorkPressure, 2) + m_strUnit[PRESSURE_UNIT]);
        break;
    default:
        break;
    }

    //产水参数
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_EDI_LOOP:
        //产水水流量
        painter.drawText(176, 162, toString(m_flowRate[S3_VALUE], 1) + " L/h");

        //I3 EDI
        temp = static_cast<double>(m_waterInfo[I3_VALUE].fQuality);
        iPrecision = temp > 1.0 ? 1: 2;
        painter.drawText(360, 145,
                         toString(m_waterInfo[I3_VALUE].fQuality, iPrecision) + QString(" ") + m_strUnit[RES_COND_UNIT]);
        painter.drawText(360, 160,
                         toString(m_waterInfo[I3_VALUE].fTemp, 1)  + QString(" ") + m_strUnit[TEMP_UNIT]);
        break;
    case MACHINE_L_UP:
    case MACHINE_L_RO_LOOP:
        //产水水流量
        painter.drawText(197, 162, toString(m_flowRate[S3_VALUE], 1) + " L/h");
        break;
    case MACHINE_Genie:
    case MACHINE_EDI:
        //I3 EDI
        temp = static_cast<double>(m_waterInfo[I3_VALUE].fQuality);
        iPrecision = temp > 1.0 ? 1: 2;
        painter.drawText(310, 145,
                         toString(m_waterInfo[I3_VALUE].fQuality, iPrecision) + QString(" ") + m_strUnit[RES_COND_UNIT]);
        painter.drawText(310, 160,
                         toString(m_waterInfo[I3_VALUE].fTemp, 1)  + QString(" ") + m_strUnit[TEMP_UNIT]);
        break;
    default:
        break;
    }

    //取水参数
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_Genie:
        //I4
        temp = static_cast<double>(m_waterInfo[I4_VALUE].fQuality);
        iPrecision = temp > 1.0 ? 1: 2;
        painter.drawText(356, 265,
                         toString(m_waterInfo[I4_VALUE].fQuality, iPrecision) + QString(" ") + m_strUnit[RES_COND_UNIT]);
        painter.drawText(356, 280,
                         toString(m_waterInfo[I4_VALUE].fTemp,  1)  + QString(" ") + m_strUnit[TEMP_UNIT]);

        //I5
        temp = static_cast<double>(m_waterInfo[I5_VALUE].fQuality);
        iPrecision = temp > 1.0 ? 1: 2;
        painter.drawText(460, 305,
                         toString(m_waterInfo[I5_VALUE].fQuality, iPrecision) + QString(" ") + m_strUnit[RES_COND_UNIT]);
        painter.drawText(460, 320,
                         toString(m_waterInfo[I5_VALUE].fTemp, 1)  + QString(" ") + m_strUnit[TEMP_UNIT]);

        //取水流量计
        painter.drawText(430, 445, toString(m_flowRate[S1_VALUE], 1) + " L/min");
        break;
    case MACHINE_L_UP:
    case MACHINE_UP:
        if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir))
        {
            //I3
            temp = static_cast<double>(m_waterInfo[I3_VALUE].fQuality);
            iPrecision = temp > 1.0 ? 1: 2;
            painter.drawText(300, 490,
                             toString(m_waterInfo[I3_VALUE].fQuality, iPrecision) + QString(" ") + m_strUnit[RES_COND_UNIT]);
            painter.drawText(300, 440,
                             toString(m_waterInfo[I3_VALUE].fTemp,  1)  + QString(" ") + m_strUnit[TEMP_UNIT]);
        }
        if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
        {
            //I4
            temp = static_cast<double>(m_waterInfo[I4_VALUE].fQuality);
            iPrecision = temp > 1.0 ? 1: 2;
            painter.drawText(420, 305,
                             toString(m_waterInfo[I4_VALUE].fQuality, iPrecision) + QString(" ") + m_strUnit[RES_COND_UNIT]);
            painter.drawText(420, 320,
                             toString(m_waterInfo[I4_VALUE].fTemp,  1)  + QString(" ") + m_strUnit[TEMP_UNIT]);
        }
        //I5
        temp = static_cast<double>(m_waterInfo[I5_VALUE].fQuality);
        iPrecision = temp > 1.0 ? 1: 2;
        painter.drawText(525, 305,
                         toString(m_waterInfo[I5_VALUE].fQuality, iPrecision) + QString(" ") + m_strUnit[RES_COND_UNIT]);
        painter.drawText(525, 320,
                         toString(m_waterInfo[I5_VALUE].fTemp, 1)  + QString(" ") + m_strUnit[TEMP_UNIT]);

        //取水流量计
        painter.drawText(480, 445, toString(m_flowRate[S1_VALUE], 1) + " L/min");
        break;
    case MACHINE_L_EDI_LOOP:
    case MACHINE_EDI:
        if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir))
        {
            //I4
            temp = static_cast<double>(m_waterInfo[I4_VALUE].fQuality);
            iPrecision = temp > 1.0 ? 1: 2;
            painter.drawText(356, 265,
                             toString(m_waterInfo[I4_VALUE].fQuality, iPrecision) + QString(" ") + m_strUnit[RES_COND_UNIT]);
            painter.drawText(356, 280,
                             toString(m_waterInfo[I4_VALUE].fTemp,  1)  + QString(" ") + m_strUnit[TEMP_UNIT]);
        }
        //取水流量计
        painter.drawText(525, 445, toString(m_flowRate[S1_VALUE], 1) + " L/min");
        break;
    case MACHINE_L_RO_LOOP:
    case MACHINE_RO:
        if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir))
        {
            //I3
            temp = static_cast<double>(m_waterInfo[I3_VALUE].fQuality);
            iPrecision = temp > 1.0 ? 1: 2;
            painter.drawText(356, 265,
                             toString(m_waterInfo[I3_VALUE].fQuality, iPrecision) + QString(" ") + m_strUnit[RES_COND_UNIT]);
            painter.drawText(356, 280,
                             toString(m_waterInfo[I3_VALUE].fTemp,  1)  + QString(" ") + m_strUnit[TEMP_UNIT]);
        }
        //取水流量计
        painter.drawText(525, 445, toString(m_flowRate[S1_VALUE], 1) + " L/min");
        break;
    case MACHINE_ADAPT:
        //I4
        if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
        {
            temp = static_cast<double>(m_waterInfo[I4_VALUE].fQuality);
            iPrecision = temp > 1.0 ? 1: 2;
            painter.drawText(375, 265,
                             toString(m_waterInfo[I4_VALUE].fQuality, iPrecision) + QString(" ") + m_strUnit[RES_COND_UNIT]);
            painter.drawText(375, 280,
                             toString(m_waterInfo[I4_VALUE].fTemp,  1)  + QString(" ") + m_strUnit[TEMP_UNIT]);
        }

        //I5
        temp = static_cast<double>(m_waterInfo[I5_VALUE].fQuality);
        iPrecision = temp > 1.0 ? 1: 2;
        painter.drawText(405, 370,
                         toString(m_waterInfo[I5_VALUE].fQuality, iPrecision) + QString(" ") + m_strUnit[RES_COND_UNIT]);
        painter.drawText(405, 385,
                         toString(m_waterInfo[I5_VALUE].fTemp, 1)  + QString(" ") + m_strUnit[TEMP_UNIT]);

        //取水流量计
        painter.drawText(338, 210, toString(m_flowRate[S1_VALUE], 1) + " L/min");
        break;
    case MACHINE_PURIST:
        //I2
        if (gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_UP_IN))
        {
            painter.drawText(480, 350,
                            toString(m_waterInfo[I2_VALUE].fQuality, 1) + QString(" ") + strUs);
            painter.drawText(480, 365,
                            toString(m_waterInfo[I2_VALUE].fTemp, 1)  + QString(" ")+ m_strUnit[TEMP_UNIT]);
        }
        //I4
        if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
        {
            temp = static_cast<double>(m_waterInfo[I4_VALUE].fQuality);
            iPrecision = temp > 1.0 ? 1: 2;
            painter.drawText(355, 130,
                             toString(m_waterInfo[I4_VALUE].fQuality, iPrecision) + QString(" ") + m_strUnit[RES_COND_UNIT]);
            painter.drawText(355, 145,
                             toString(m_waterInfo[I4_VALUE].fTemp,  1)  + QString(" ") + m_strUnit[TEMP_UNIT]);
        }

        //I5
        temp = static_cast<double>(m_waterInfo[I5_VALUE].fQuality);
        iPrecision = temp > 1.0 ? 1: 2;
        painter.drawText(480, 130,
                         toString(m_waterInfo[I5_VALUE].fQuality, iPrecision) + QString(" ") + m_strUnit[RES_COND_UNIT]);
        painter.drawText(480, 145,
                         toString(m_waterInfo[I5_VALUE].fTemp, 1)  + QString(" ") + m_strUnit[TEMP_UNIT]);

        //取水流量计
        painter.drawText(300, 370, toString(m_flowRate[S1_VALUE], 1) + " L/min");
        break;
    default:
        break;
    }

    /****** 左下角显示 ******/
    //电流电压
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_Genie:
        if(MACHINE_Genie == gGlobalParam.iMachineType)
        {
            painter.drawText(offsetX, 405,
                             tr("RO Pump") + QString(":  %1mA %2V").arg(m_detectionPara[C1_DETECTION].iValueI).arg(m_detectionPara[C1_DETECTION].iValueV/1000));
            painter.drawText(offsetX, 420,
                             tr("Recir. Pump") + QString(":  %1mA %2V").arg(m_detectionPara[C2_DETECTION].iValueI).arg(m_detectionPara[C2_DETECTION].iValueV/1000));
        }
        painter.drawText(offsetX, 435,
                         tr("254 UV") + QString(":  %1mA").arg(m_detectionPara[N1_DETECTION].iValueI));
        painter.drawText(offsetX, 450,
                         tr("185 UV") + QString(":  %1mA").arg(m_detectionPara[N2_DETECTION].iValueI));
        painter.drawText(offsetX, 465,
                         tr("TANK UV") + QString(":  %1mA").arg(m_detectionPara[N3_DETECTION].iValueI));
        painter.drawText(offsetX, 480,
                         tr("EDI Module") + QString(":  %1mA").arg(m_detectionPara[EDI_DETECTION].iValueI));
		painter.drawText(offsetX, 495,
                         tr("RO Rejection") + ":  " + toString(m_fResidue, 1) + QString("%"));

		if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
        {
        	QString strToc;
        	if(m_fToc >= 200)
        	{
        		strToc = ">200" + QString("ppb");
        	}
			else if(m_fToc < 10)
			{
				strToc = toString(m_fToc, 1) + QString("ppb");
			}
			else
			{
				strToc = toString(m_fToc) + QString("ppb");
			}
            painter.drawText(170, 495, tr("TOC") + ":  " + strToc);
        }
        break;
    case MACHINE_L_EDI_LOOP:
    case MACHINE_EDI:
        if(MACHINE_EDI == gGlobalParam.iMachineType)
        {
            painter.drawText(offsetX, 420,
                             tr("RO Pump") + QString(":  %1mA %2V").arg(m_detectionPara[C1_DETECTION].iValueI).arg(m_detectionPara[C1_DETECTION].iValueV/1000));
            painter.drawText(offsetX, 435,
                             tr("Recir. Pump") + QString(":  %1mA %2V").arg(m_detectionPara[C2_DETECTION].iValueI).arg(m_detectionPara[C2_DETECTION].iValueV/1000));
        }
        painter.drawText(offsetX, 450,
                         tr("254 UV") + QString(":  %1mA").arg(m_detectionPara[N1_DETECTION].iValueI));
        painter.drawText(offsetX, 465,
                         tr("TANK UV") + QString(":  %1mA").arg(m_detectionPara[N3_DETECTION].iValueI));
        painter.drawText(offsetX, 480,
                         tr("EDI Module") + QString(":  %1mA").arg(m_detectionPara[EDI_DETECTION].iValueI));
        painter.drawText(offsetX, 495,
                         tr("RO Rejection") + ": " + toString(m_fResidue, 1) + QString("%"));
        break;
    case MACHINE_L_UP:
        painter.drawText(offsetX, 450,
                         tr("254 UV") + QString(":  %1mA").arg(m_detectionPara[N1_DETECTION].iValueI));
        painter.drawText(offsetX, 465,
                         tr("185 UV") + QString(":  %1mA").arg(m_detectionPara[N2_DETECTION].iValueI));
        painter.drawText(offsetX, 480,
                         tr("TANK UV") + QString(":  %1mA").arg(m_detectionPara[N3_DETECTION].iValueI));
		painter.drawText(offsetX, 495,
                         tr("RO Rejection") + ":  " + toString(m_fResidue, 1) + QString("%"));
		if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
        {
            QString strToc;
        	if(m_fToc >= 200)
        	{
        		strToc = ">200" + QString("ppb");
        	}
			else if(m_fToc < 10)
			{
				strToc = toString(m_fToc, 1) + QString("ppb");
			}
			else
			{
				strToc = toString(m_fToc) + QString("ppb");
			}
            painter.drawText(170, 475, tr("TOC") + ":  " + strToc);
        }
        break;
    case MACHINE_UP:
        painter.drawText(offsetX, 435,
                         tr("RO Pump") + QString(":  %1mA %2V").arg(m_detectionPara[C1_DETECTION].iValueI).arg(m_detectionPara[C1_DETECTION].iValueV/1000));
        painter.drawText(offsetX, 450,
                         tr("Recir. Pump") + QString(":  %1mA %2V").arg(m_detectionPara[C2_DETECTION].iValueI).arg(m_detectionPara[C2_DETECTION].iValueV/1000));
        painter.drawText(offsetX, 465,
                         tr("185 UV") + QString(":  %1mA").arg(m_detectionPara[N2_DETECTION].iValueI));
        painter.drawText(offsetX, 480,
                         tr("TANK UV") + QString(":  %1mA").arg(m_detectionPara[N3_DETECTION].iValueI));
		painter.drawText(offsetX, 495,
						tr("RO Rejection") + ":  " + toString(m_fResidue, 1) + QString("%"));

	   	if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
       	{
       	    QString strToc;
        	if(m_fToc >= 200)
        	{
        		strToc = ">200" + QString("ppb");
        	}
			else if(m_fToc < 10)
			{
				strToc = toString(m_fToc, 1) + QString("ppb");
			}
			else
			{
				strToc = toString(m_fToc) + QString("ppb");
			}
           	painter.drawText(170, 475, tr("TOC") + ":  " + strToc);
       	}
        break;
    case MACHINE_L_RO_LOOP:
        painter.drawText(offsetX, 465,
                         tr("254 UV") + QString(":  %1mA").arg(m_detectionPara[N1_DETECTION].iValueI));
        painter.drawText(offsetX, 480,
                         tr("TANK UV") + QString(":  %1mA").arg(m_detectionPara[N3_DETECTION].iValueI));
        painter.drawText(offsetX, 495,
                         tr("RO Rejection") + ":  " + toString(m_fResidue, 1) + QString("%"));
        break;
    case MACHINE_RO:
        painter.drawText(offsetX, 450,
                         tr("RO Pump") + QString(":  %1mA %2V").arg(m_detectionPara[C1_DETECTION].iValueI).arg(m_detectionPara[C1_DETECTION].iValueV/1000));
        painter.drawText(offsetX, 465,
                         tr("Recir. Pump") + QString(":  %1mA %2V").arg(m_detectionPara[C2_DETECTION].iValueI).arg(m_detectionPara[C2_DETECTION].iValueV/1000));
        painter.drawText(offsetX, 480,
                         tr("TANK UV") + QString(":  %1mA").arg(m_detectionPara[N3_DETECTION].iValueI));
        painter.drawText(offsetX, 495,
                         tr("RO Rejection") + ":  " + toString(m_fResidue, 1) + QString("%"));
        break;
    case MACHINE_ADAPT:
        painter.drawText(offsetX, 450,
                         tr("RO Pump") + QString(":  %1mA %2V").arg(m_detectionPara[C1_DETECTION].iValueI).arg(m_detectionPara[C1_DETECTION].iValueV/1000));
        painter.drawText(offsetX, 465,
                         tr("Recir. Pump") + QString(":  %1mA %2V").arg(m_detectionPara[C2_DETECTION].iValueI).arg(m_detectionPara[C2_DETECTION].iValueV/1000));
        painter.drawText(offsetX, 480,
                         tr("185 UV") + QString(":  %1mA").arg(m_detectionPara[N2_DETECTION].iValueI));
		painter.drawText(offsetX, 495,
                         tr("RO Rejection") + ":  " + toString(m_fResidue, 1) + QString("%"));
		
		if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
        {
            QString strToc;
        	if(m_fToc >= 200)
        	{
        		strToc = ">200" + QString("ppb");
        	}
			else if(m_fToc < 10)
			{
				strToc =toString(m_fToc, 1) + QString("ppb");
			}
			else
			{
				strToc = toString(m_fToc) + QString("ppb");
			}
            painter.drawText(170, 475, tr("TOC") + ":  " + strToc);
        }
        break;
    case MACHINE_PURIST:
        painter.drawText(offsetX, 480,
                         tr("Recir. Pump") + QString(":  %1mA %2V").arg(m_detectionPara[C2_DETECTION].iValueI).arg(m_detectionPara[C2_DETECTION].iValueV/1000));
        painter.drawText(offsetX, 495,
                         tr("185 UV") + QString(":  %1mA").arg(m_detectionPara[N2_DETECTION].iValueI));
        if(gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTOC))
        {
            QString strToc;
        	if(m_fToc >= 200)
        	{
        		strToc = ">200" + QString("ppb");
        	}
			else if(m_fToc < 10)
			{
				strToc = toString(m_fToc, 1) + QString("ppb");
			}
			else
			{
				strToc = toString(m_fToc) + QString("ppb");
			}
            painter.drawText(170, 490, tr("TOC") + ":  " + strToc);
        }
        break;
    default:
        break;
    }

    //纯水箱液位
    switch (gGlobalParam.iMachineType)
    {
    case MACHINE_ADAPT:
    case MACHINE_PURIST:
        break;
    default:
        //纯水箱液位
        if(m_iTankLevel >= 10)
        {
            painter.drawText(700, 290, QString("%1%").arg(m_iTankLevel));
        }
        else
        {
            painter.drawText(708, 290, QString("%1%").arg(m_iTankLevel));
        }
        break;
    }

    painter.setFont(oldFont);
    painter.setPen(oldPen);
}

void DFlowChartWidget::initUI()
{
    setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(228, 231, 240));
    this->setPalette(palette);
}

//初始化参数默认值
void DFlowChartWidget::initDefaultValue()
{
	for(int i = 0; i < I_NUM; i++)
	{
		m_waterInfo[i].fQuality = 0.0;
    	m_waterInfo[i].fTemp = 0.0;
	}

    m_fResidue = 0.0;
    m_fToc = 0;

    for(int i = 0; i < DETECTION_NUM; i++)
    {
        m_detectionPara[i].iValueI = 0;
        m_detectionPara[i].iValueV = 0;
    }
    for(int i = 0; i < S_NUM; i++)
    {
        m_flowRate[i] = 0.0;
    }

    m_fFeedPressure = 0;
    m_fWorkPressure = 0;
    m_iTankLevel = 0;
    m_fTankLevel = 0;

    m_iSourceTankLevel = 0;
    m_fSourceTankLevel = 0;

    updateUnits();
}

//加载绘图图片素材
void DFlowChartWidget::loadPixmap()
{
    QString strDImage[DEVICE_NUM] =
    {
        ":/flowchartdevices/EDI_OFF.png",
        ":/flowchartdevices/EDI_ON.png",
        ":/flowchartdevices/FLOWMETER_OFF.png",
        ":/flowchartdevices/FLOWMETER_ON.png",
        ":/flowchartdevices/I_OFF.png",
        ":/flowchartdevices/I_ON.png",
        ":/flowchartdevices/P_OFF.png",
        ":/flowchartdevices/P_ON.png",
        ":/flowchartdevices/PACK_OFF.png",
        ":/flowchartdevices/PACK_ON.png",
        ":/flowchartdevices/PACK_YELLOW.png", 
        ":/flowchartdevices/PRE_OFF.png",
        ":/flowchartdevices/PRE_ON.png",
        ":/flowchartdevices/PREPACK_OFF.png",
        ":/flowchartdevices/PREPACK_ON.png",
        ":/flowchartdevices/PREPACK_YELLOW.png",
        ":/flowchartdevices/PUMP_OFF.png",
        ":/flowchartdevices/PUMP_ON.png",
        ":/flowchartdevices/ROPACK_OFF.png",
        ":/flowchartdevices/ROPACK_ON.png",
        ":/flowchartdevices/TANK.png",
        ":/flowchartdevices/TANKUV_OFF.png",
        ":/flowchartdevices/TANKUV_ON.png",
        ":/flowchartdevices/TANKUV_YELLOW.png", 
        ":/flowchartdevices/UV_OFF.png",
        ":/flowchartdevices/UV_ON.png",
        ":/flowchartdevices/UV_YELLOW.png",
        ":/flowchartdevices/VALVE_OFF.png",
        ":/flowchartdevices/VALVE_ON.png",
        ":/flowchartdevices/VALVE2_OFF.png",
        ":/flowchartdevices/VALVE2_C_ON.png",
        ":/flowchartdevices/VALVE2_L_ON.png",
        ":/flowchartdevices/VALVE2_R_ON.png",
        ":/flowchartdevices/DISPER.png",
        ":/flowchartdevices/DISPER_ON.png",
        ":/flowchartdevices/DISPER_YELLOW.png",
        ":/flowchartdevices/LIMIT_OFF.png",
        ":/flowchartdevices/LIMIT_ON.png"
    };

    for(int i = 0; i < DEVICE_NUM; i++)
    {
        m_devicesPix[i] = QPixmap(strDImage[i]);
    }
}

QPixmap DFlowChartWidget::scaledToWidth(const QPixmap &pixmap, int width)
{
    return pixmap.scaledToWidth(width, Qt::SmoothTransformation);
}

QPixmap DFlowChartWidget::scaledToHeight(const QPixmap &pixmap, int height)
{
    return pixmap.scaledToHeight(height, Qt::SmoothTransformation);
}

//根据负载的工作状态为painter 设置不同颜色的pen
void DFlowChartWidget::setStatePen(QPainter &painter, bool bWork)
{
    if(bWork)
    {
        painter.setPen(QPen(QColor(60,190,235), 3));
    }
    else
    {
        painter.setPen(QPen(Qt::gray, 3));
    }
}
