#include "ConsumableinsPage.h"
#include "mainwindow.h"
#include "consumableinslistwidgtitem.h"
#include "exconfig.h"
#include "dhintdialog.h"
#include <QListWidget>

ConsumableInsPage::ConsumableInsPage(QObject *parent,CBaseWidget *widget ,MainWindow *wndMain, short instanceType)
                                          : CSubPage(parent,widget,wndMain), m_bRfidWork(false)
{
    initTypeMap();

    switch(instanceType)
    {
    case 0:
        initManualItem();
        break;
    default:
        initNormalItem();
        break;
    }  

    creatTitle();

    initUi();

    buildTranslation();

}

void ConsumableInsPage::creatTitle()
{
    CSubPage::creatTitle();

    buildTitles();

    selectTitle(0);
}

void ConsumableInsPage::buildTitles()
{
    QStringList stringList;

    stringList << tr("Components Installation");

    setTitles(stringList);

}

void ConsumableInsPage::buildTranslation()
{
    int iIdx = 0;

    int iMapIdx = 0;

    int iEscape = 0;

    for (iIdx = 0; iIdx < m_iRealItemNum; iIdx++)
    {
        iEscape = 0;
        switch(aIds[iIdx].iId)
        {
        case DISP_PRE_PACK:
            /*
            Ԥ     
            */
            m_aInsListItem[iMapIdx]->setName(tr("Prefilter"));
            m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
            break;
       case DISP_ICP_PACK:
            m_aInsListItem[iMapIdx]->setName(tr("ICP Pack"));
            m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
            break;
        case DISP_AC_PACK:
            /*
            AC PACK
            */
            m_aInsListItem[iMapIdx]->setName(tr("AC Pack"));
            m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
            break;
        case DISP_T_PACK:
            /*
            T Pack
            */
            m_aInsListItem[iMapIdx]->setName(tr("T Pack"));
            m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
            break;
        case DISP_P_PACK:
            /*
            P-Pack  
            */
            m_aInsListItem[iMapIdx]->setName(tr("P Pack"));
            m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
            break;
        case DISP_U_PACK:
             /*
             U-Pack  
             */
            m_aInsListItem[iMapIdx]->setName(tr("U Pack"));
            m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
            break;
        case DISP_AT_PACK:
            /*
            AT-Pack  
            */
            m_aInsListItem[iMapIdx]->setName(tr("AT Pack"));
            m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
            break;
        case DISP_H_PACK:
            /*
            H-Pack  
            */
            m_aInsListItem[iMapIdx]->setName(tr("H Pack"));
            m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
            break;
        case DISP_N1_UV:
            /*
            254UV 
            */
            m_aInsListItem[iMapIdx]->setName(tr("254 UV Lamp"));
            m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
            break;
        case DISP_N2_UV:
            /*
            185UV       
            */
            m_aInsListItem[iMapIdx]->setName(tr("185 UV Lamp"));
            m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
            break;
        case DISP_N3_UV:
            /*
            ˮUV       
            */
            m_aInsListItem[iMapIdx]->setName(tr("Tank UV Lamp"));
            m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
            break;
        case DISP_N4_UV:
            /*
            ·UV       
            */
            m_aInsListItem[iMapIdx]->setName(tr("TUBE UV"));
            m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
            break;
#if 0
        case DISP_N5_UV:
            /*
            TOC UV       
            */
            m_aInsListItem[iMapIdx]->setName(tr("TOC UV"));
            m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
            break;
#endif
        case DISP_W_FILTER:
            /*
                   
            */
            if ((gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveB2)))
            {
                m_aInsListItem[iMapIdx]->setName(tr("Tank Vent Filter"));
                m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
            }
            else
            {
                iEscape = 1;
            }
            break;
        case DISP_T_B_FILTER:
            /*
            ն˹B      
            */
            if(0 == gAdditionalCfgParam.productInfo.iCompany)
            {
                 m_aInsListItem[iMapIdx]->setName(tr("Final Fliter B"));
            }
            else
            {
                m_aInsListItem[iMapIdx]->setName(tr("Bio-filter"));
            }
            m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
            break;
        case DISP_T_A_FILTER:
            /*
            ն˹A      
            */
            m_aInsListItem[iMapIdx]->setName(tr("Final Fliter A"));
            m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
            break;
        case DISP_TUBE_FILTER:
            /*
            ·       
            */
            m_aInsListItem[iMapIdx]->setName(tr("Loop Filter"));
            m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
            break;
        case DISP_TUBE_DI:
            /*
            ·DI     
            */
            m_aInsListItem[iMapIdx]->setName(tr("Loop DI"));
            m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
            break;
        case DISP_MACHINERY_SOURCE_BOOSTER_PUMP:
            /*
            ԭˮѹ
            */
            m_aInsListItem[iMapIdx]->setName(tr("Feed Pump"));
            m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
            break;
        case DISP_MACHINERY_TUBE_CIR_PUMP:
            /*
            ·ѭ       
            */
            m_aInsListItem[iMapIdx]->setName(tr("Dist. Pump"));
            m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
            break;
        case DISP_MACHINERY_CIR_PUMP:
            /*
            ѭ       
            */
            m_aInsListItem[iMapIdx]->setName(tr("Recir. Pump"));
            m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
            break;
        case DISP_MACHINERY_RO_MEMBRANE:
            /*
            ROĤ
            */
            m_aInsListItem[iMapIdx]->setName(tr("RO Membrane"));
            m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
            break;
        case DISP_MACHINERY_RO_BOOSTER_PUMP:
            /*
            ROѹ
            װ
            */
            m_aInsListItem[iMapIdx]->setName(tr("RO Pump"));
            m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
            break;
        case DISP_MACHINERY_EDI:
            /*
            EDI
            */
            m_aInsListItem[iMapIdx]->setName(tr("EDI Module"));
            m_aInsListItem[iMapIdx]->setP2Name(tr("Install"));
            break;           
        }
        if (iEscape)
        {
            continue;
        }
        m_aInsListItem[iMapIdx]->setItemId(iIdx);
        m_aInsListItem[iMapIdx]->setBtnId(iMapIdx);
        m_aInsListItem[iMapIdx]->setCatName(tr("CAT"));
        m_aInsListItem[iMapIdx]->setLotName(tr("LOT"));
        iMapIdx++;
    }

    for (iIdx = 0; iIdx < iMapIdx; iIdx++)
    {
        listWidget->setItemHidden(listWidgetIem[iIdx],false);
    }

    for (iIdx = iMapIdx; iIdx < m_iRealItemNum; iIdx++)
    {
        listWidget->setItemHidden(listWidgetIem[iIdx],true);
    }

    m_iViewItemNum = iMapIdx;
}

void ConsumableInsPage::switchLanguage()
{
    buildTranslation();

    buildTitles();

    selectTitle(titleIndex());
}



void ConsumableInsPage::createList()
{
    QColor colors[] = {QColor(200,200,188),QColor(228, 231, 240)};

    
    listWidget = new QListWidget((QWidget *)m_widget);

    listWidget->setStyleSheet("background-color:transparent");
    listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listWidget->setFrameShape(QListWidget::NoFrame);

    listWidget->setGeometry(QRect(10,60 ,780,490));

    m_iCurrentItem = 0;

    for(int iLoop = 0 ; iLoop < m_iRealItemNum ; iLoop++)
    {
        listWidgetIem[iLoop] = new QListWidgetItem;
        listWidgetIem[iLoop]->setSizeHint(QSize(600 , CI_LIST_WIDGET_HEIGHT));
        listWidgetIem[iLoop]->setBackground(colors[iLoop % 2]);

        m_aInsListItem[iLoop] = new CosumableInsListWidgtItem(NULL,this,iLoop);

        listWidget->insertItem(iLoop,listWidgetIem[iLoop]);
        
        listWidget->setItemWidget(listWidgetIem[iLoop] , m_aInsListItem[iLoop]);

    }
    connect(listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(ItemClicked(QListWidgetItem *)));
}

void ConsumableInsPage::setBackColor()
{
    QSize size(width(),height());

    QImage image_bg = QImage(size, QImage::Format_ARGB32);

    QPainter p(&image_bg);

    p.fillRect(image_bg.rect(), QColor(228, 231, 240));

    QPalette pal(m_widget->palette());

    pal.setBrush(m_widget->backgroundRole(),QBrush(image_bg));

    m_widget->setAutoFillBackground(true);
    m_widget->setPalette(pal);
}

void ConsumableInsPage::initUi()
{
    setBackColor();

    createList();
}

void ConsumableInsPage::update()
{
    m_wndMain->setWorkMode(APP_WORK_MODE_INSTALL);
   
    buildTranslation();
   
}

void ConsumableInsPage::fade()
{
    m_wndMain->setWorkMode(APP_WORK_MODE_NORMAL);
}

void ConsumableInsPage::updateRfidInfo(int iRfId)
{
	if(!m_bRfidWork)
	{
		return;
	}
	
    int index = m_aInsListItem[m_iCurrentItem]->getItemId();
   
    if (aIds[index].iRfid != iRfId)
    {
         return ;
    }

    if (index < m_iViewItemNum)
    {
        int                 iRet;

        CATNO cn;
        LOTNO ln;

        memset(cn,0,sizeof(CATNO));
        memset(ln,0,sizeof(LOTNO));

        if (m_wndMain->getRfidState(iRfId))
        {
            m_wndMain->getRfidCatNo(iRfId,cn);
            m_wndMain->getRfidLotNo(iRfId,ln);
        }
        else
        {
            iRet = m_wndMain->readRfid(iRfId);
      
            if (iRet)
            {
                return;
            }
      
            m_wndMain->getRfidCatNo(iRfId,cn);
            m_wndMain->getRfidLotNo(iRfId,ln);
        }

        m_aInsListItem[m_iCurrentItem]->setP1(cn);
        m_aInsListItem[m_iCurrentItem]->setP2(ln);
    }       
}


void ConsumableInsPage::ItemClicked(QListWidgetItem * item)
{
    QListWidget *pTmpListWidget = (QListWidget *)this->sender();
    
    m_iCurrentItem = pTmpListWidget->row(item);
}

void ConsumableInsPage::toCurrentItem(int index)
{
    m_iCurrentItem = index;
}

void ConsumableInsPage::initTypeMap()
{
    m_typeMap.insert(PPACK_CATNO, DISP_P_PACK);
    m_typeMap.insert(ACPACK_CATNO, DISP_AC_PACK);
    m_typeMap.insert(UPACK_CATNO, DISP_U_PACK);
    m_typeMap.insert(HPACK_CATNO, DISP_H_PACK);
    m_typeMap.insert(PREPACK_CATNO, DISP_PRE_PACK);
    m_typeMap.insert(ICPPACK_CATNO, DISP_ICP_PACK);
    m_typeMap.insert(CLEANPACK_CATNO, DISP_P_PACK | (1 <<16));
    m_typeMap.insert(ATPACK_CATNO, DISP_AT_PACK);
    m_typeMap.insert(TPACK_CATNO, DISP_T_PACK);
    m_typeMap.insert(ROPACK_CATNO, DISP_MACHINERY_RO_MEMBRANE);
    m_typeMap.insert(UV185_CATNO, DISP_N2_UV);
    m_typeMap.insert(UV254_CATNO, DISP_N1_UV);
    m_typeMap.insert(UVTANK_CATNO, DISP_N3_UV);
    m_typeMap.insert(ROPUMP_CATNO, DISP_MACHINERY_RO_BOOSTER_PUMP);
    m_typeMap.insert(UPPUMP_CATNO, DISP_MACHINERY_CIR_PUMP);
    m_typeMap.insert(FINALFILTER_A_CATNO, DISP_T_A_FILTER);
    m_typeMap.insert(FINALFILTER_B_CATNO, DISP_T_B_FILTER);
    m_typeMap.insert(EDI_CATNO, DISP_MACHINERY_EDI);
    m_typeMap.insert(TANKVENTFILTER_CATNO, DISP_W_FILTER);

    m_typeMap.insert(LOOPFILTER_CATNO, DISP_TUBE_FILTER);
    m_typeMap.insert(LOOPUV_CATNO, DISP_N4_UV);
	m_typeMap.insert(LOOPDI_CATNO, DISP_TUBE_DI);
}

void ConsumableInsPage::initNormalItem()
{
    int iIdx = 0;
    m_bRfidWork = true;
    if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_Pre_Filter)) //DISP_SM_PreFilterColumn
    {
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_PRE_PACK;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        iIdx++;
    }

    if(gGlobalParam.SubModSetting.ulAddFlags & (1 << DISP_SM_DEION))
    {
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_ICP_PACK;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ICPPACK;
        iIdx++;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        break;
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_AC_PACK;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        iIdx++;
        break;
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        break;
     }

    if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir))
    {
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_T_PACK;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        iIdx++;
    }
    
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_P_PACK;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        iIdx++;
        break;
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_ADAPT:
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_P_PACK;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_PPACK_CLEANPACK;
        iIdx++;
        break;
    case MACHINE_PURIST:
        break;
     }
    
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
        if(gAdditionalCfgParam.machineInfo.iMachineFlow != 250)
        {
            aIds[iIdx].iType = 0;
            aIds[iIdx].iId	 = DISP_AT_PACK;
            aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_HPACK_ATPACK;
            iIdx++;
        }
        break;
    case MACHINE_L_EDI_LOOP:
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_AT_PACK;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_HPACK_ATPACK;
        iIdx++;
        break;
    }
    
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_UP:  
        if(gAdditionalCfgParam.machineInfo.iMachineFlow < 300)
        {
            aIds[iIdx].iType = 0;
            aIds[iIdx].iId   = DISP_H_PACK;
            aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_HPACK_ATPACK;
            iIdx++;
        }
        break;
    case MACHINE_UP:
    case MACHINE_PURIST:
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_H_PACK;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_HPACK_ATPACK;
        iIdx++;
        break;
    case MACHINE_L_Genie:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_ADAPT:
        break;
    default:
        break;
    }
    
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_U_PACK;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_UPACK_HPACK;
        iIdx++;
        break;
    default:
        break;
    }
    
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_EDI:
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_N1_UV; //254
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        iIdx++;
        break;
    case MACHINE_RO:
    case MACHINE_UP:
        break;
    default:
        break;
    }
    
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_N2_UV;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        iIdx++;
        break;
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        break;
    default:
        break;
    }
    
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_PURIST:
        if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_TankUV))
        {
            aIds[iIdx].iType = 0;
            aIds[iIdx].iId   = DISP_N3_UV;
            aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
            iIdx++;
        }
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    {
        if (gGlobalParam.SubModSetting.ulFlags & (1 <<DISP_SM_TubeUV))
        {
            aIds[iIdx].iType = 0;
            aIds[iIdx].iId   = DISP_N4_UV;
            aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
            iIdx++;
        }
        break;
    }
    case MACHINE_Genie:
    case MACHINE_RO:
    case MACHINE_EDI:
    case MACHINE_PURIST:
    case MACHINE_UP:
        break;
    }
     
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_W_FILTER;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        iIdx++;
        break;
    }

    if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_FINALFILTER_B))
    {
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_T_B_FILTER;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        iIdx++;
    }
    if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_FINALFILTER_A))
    {
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_T_A_FILTER;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        iIdx++;
    }

#if 0
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_ADAPT:
	case MACHINE_PURIST:
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_T_B_FILTER;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        iIdx++;
        break;
	default:
		break;
    }

	switch(gGlobalParam.iMachineType)
	{
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_T_A_FILTER;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        iIdx++;
        break;
    }
#endif


    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    {
        if (gGlobalParam.SubModSetting.ulFlags & DISP_SM_HaveTubeFilter)
        {
            aIds[iIdx].iType = 0;
            aIds[iIdx].iId   = DISP_TUBE_FILTER;
            aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
            iIdx++;
        }

        if (gGlobalParam.SubModSetting.ulFlags & DISP_SM_TubeDI)
        {
            aIds[iIdx].iType = 0;
            aIds[iIdx].iId   = DISP_TUBE_DI;
            aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
            iIdx++;
        }
        break;
    }
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
    case MACHINE_RO:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        aIds[iIdx].iType = 1; //0
        aIds[iIdx].iId   = DISP_MACHINERY_SOURCE_BOOSTER_PUMP;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        iIdx++;
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        aIds[iIdx].iType = 1; //0
        aIds[iIdx].iId   = DISP_MACHINERY_TUBE_CIR_PUMP;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        iIdx++;
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        aIds[iIdx].iType = 1; //0
        aIds[iIdx].iId   = DISP_MACHINERY_CIR_PUMP;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        iIdx++;
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_ADAPT:
        aIds[iIdx].iType = 1; //0
        aIds[iIdx].iId   = DISP_MACHINERY_RO_MEMBRANE;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        iIdx++;
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_ADAPT:
        aIds[iIdx].iType = 1; //0
        aIds[iIdx].iId   = DISP_MACHINERY_RO_BOOSTER_PUMP;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        iIdx++;
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_Genie:
    case MACHINE_EDI:
        aIds[iIdx].iType = 1; //0
        aIds[iIdx].iId   = DISP_MACHINERY_EDI;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        iIdx++;
        break;
    }     
    m_iRealItemNum = iIdx;
}

void ConsumableInsPage::initManualItem()
{
	int iIdx = 0;
	m_bRfidWork = false;
    if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_Pre_Filter)) //DISP_SM_PreFilterColumn
    {
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_PRE_PACK;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        iIdx++;
    }
        
    if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_HP_Water_Cir))
    {
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_T_PACK;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        iIdx++;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_EDI:
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_N1_UV; //254
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        iIdx++;
        break;
    default:
        break;
    }
    
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_N2_UV;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        iIdx++;
        break;
    default:
        break;
    }
    
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_PURIST:
        if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_TankUV))
        {
            aIds[iIdx].iType = 0;
            aIds[iIdx].iId   = DISP_N3_UV;
            aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
            iIdx++;
        }
        break;
    default:
        break;
    }
     
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_W_FILTER;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        iIdx++;
        break;
    default:
        break;
    }

    if(gAdditionalCfgParam.machineInfo.iMachineFlow < 500)
    {
        if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_FINALFILTER_B))
        {
            aIds[iIdx].iType = 0;
            aIds[iIdx].iId   = DISP_T_B_FILTER;
            aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
            iIdx++;
        }
        if(gGlobalParam.MiscParam.ulMisFlags & (1 << DISP_SM_FINALFILTER_A))
        {
            aIds[iIdx].iType = 0;
            aIds[iIdx].iId   = DISP_T_A_FILTER;
            aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
            iIdx++;
        }
    }

#if 0
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_ADAPT:
	case MACHINE_PURIST:
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_T_B_FILTER;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        iIdx++;
        break;
    default:
        break;
    }
	
    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
		if(gAdditionalCfgParam.machineInfo.iMachineFlow < 500)
		{
			aIds[iIdx].iType = 0;
	        aIds[iIdx].iId   = DISP_T_A_FILTER;
	        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
	        iIdx++;
		}
		break;
	case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        aIds[iIdx].iType = 0;
        aIds[iIdx].iId   = DISP_T_A_FILTER;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        iIdx++;
        break;
    default:
        break;
    }
#endif

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_TubeUV))
        {
            aIds[iIdx].iType = 0;
            aIds[iIdx].iId   = DISP_N4_UV;
            aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
            iIdx++;
        }
        if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_HaveTubeFilter))
        {
            aIds[iIdx].iType = 0;
            aIds[iIdx].iId   = DISP_TUBE_FILTER;
            aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
            iIdx++;
        }
        if (gGlobalParam.SubModSetting.ulFlags & (1 << DISP_SM_TubeDI))
        {
            aIds[iIdx].iType = 0;
            aIds[iIdx].iId   = DISP_TUBE_DI;
            aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
            iIdx++;
        }
        break;
    default:
        break;
    }

    aIds[iIdx].iType = 1; //0
    aIds[iIdx].iId   = DISP_MACHINERY_SOURCE_BOOSTER_PUMP;
    aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
    iIdx++;

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        aIds[iIdx].iType = 1; //0
        aIds[iIdx].iId   = DISP_MACHINERY_TUBE_CIR_PUMP;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        iIdx++;
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
        if(gAdditionalCfgParam.machineInfo.iMachineFlow < 500)
        {
            aIds[iIdx].iType = 1; //0
            aIds[iIdx].iId   = DISP_MACHINERY_CIR_PUMP;
            aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
            iIdx++;
        }
        break;
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_PURIST:
    case MACHINE_ADAPT:
        aIds[iIdx].iType = 1; //0
        aIds[iIdx].iId   = DISP_MACHINERY_CIR_PUMP;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        iIdx++;
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_ADAPT:
        aIds[iIdx].iType = 1; //0
        aIds[iIdx].iId   = DISP_MACHINERY_RO_MEMBRANE;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        iIdx++;
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_UP:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_L_RO_LOOP:
    case MACHINE_Genie:
    case MACHINE_UP:
    case MACHINE_EDI:
    case MACHINE_RO:
    case MACHINE_ADAPT:
        aIds[iIdx].iType = 1; //0
        aIds[iIdx].iId   = DISP_MACHINERY_RO_BOOSTER_PUMP;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        iIdx++;
        break;
    default:
        break;
    }

    switch(gGlobalParam.iMachineType)
    {
    case MACHINE_L_Genie:
    case MACHINE_L_EDI_LOOP:
    case MACHINE_Genie:
    case MACHINE_EDI:
        aIds[iIdx].iType = 1; //0
        aIds[iIdx].iId   = DISP_MACHINERY_EDI;
        aIds[iIdx].iRfid = APP_RFID_SUB_TYPE_ROPACK_OTHERS;
        iIdx++;
        break;
    default:
        break;
    }     
    m_iRealItemNum = iIdx;
}

void ConsumableInsPage::installFeedback(bool result)
{
    if(result)
    {
        DHintDialog::getInstance(tr("Consumable installed successfully."));
    }
    else
    {
        DHintDialog::getInstance(tr("Consumable installation failed."));
    }

}

void ConsumableInsPage::on_btn_clicked(int index)
{  
    int iOffset = 0;
    int iLen;

    QDate installDate = QDate::currentDate();
   
   /* check SN number */
    if (index < m_iViewItemNum)
    {
        CATNO cn;
        LOTNO ln;
        QString strCn = m_aInsListItem[index]->getP1();
        QString strLn = m_aInsListItem[index]->getP2();

        int iMapIdx = m_aInsListItem[index]->getItemId();

        memset(cn,0,sizeof(CATNO));
        memset(ln,0,sizeof(LOTNO));

        toCurrentItem(index);

        if((strCn.isEmpty() || strLn.isEmpty()) && m_bRfidWork)
        {
            /* try to read info by RFID */
            if ( aIds[iMapIdx].iRfid < APP_RF_READER_MAX_NUMBER)
            {
                int  iRet;

                if (m_wndMain->getRfidState(aIds[iMapIdx].iRfid))
                {
                    m_wndMain->getRfidCatNo(aIds[iMapIdx].iRfid,cn);
                    m_wndMain->getRfidLotNo(aIds[iMapIdx].iRfid,ln);
                    m_wndMain->getRfidInstallDate(aIds[iMapIdx].iRfid, &installDate);
                }
                else
                {
                    iRet = m_wndMain->readRfid(aIds[iMapIdx].iRfid);

                    if (iRet)
                    {
                        QMessageBox::about(NULL, tr("Alarm"), "<font color='red'>"+tr("Failed to read RFID,please make sure the part is installed correctly.")+"</font>");
                        return;
                    }
                    
                    m_wndMain->getRfidCatNo(aIds[iMapIdx].iRfid,cn);
                    m_wndMain->getRfidLotNo(aIds[iMapIdx].iRfid,ln);
                    m_wndMain->getRfidInstallDate(aIds[iMapIdx].iRfid, &installDate);
                }

                m_aInsListItem[index]->setP1(cn);
                m_aInsListItem[index]->setP2(ln);
            }
            else
            {
                QMessageBox::about(NULL, tr("About"), tr("Please Input Serial Number !"));
                return;
            }
        }
       
        strCn = m_aInsListItem[index]->getP1();
        strLn = m_aInsListItem[index]->getP2();

        if (strCn.isEmpty()
            || strLn.isEmpty())
        {
            QMessageBox::about(NULL, tr("About"), tr("Please Input Serial Number !"));
            return;
        }
        switch(aIds[iMapIdx].iType)
        {
        case 0:
        {
            int cmIdx = aIds[iMapIdx].iId;

            int value = m_typeMap.key(cmIdx);
            bool verify = MainWindow::consumableCatNo(static_cast<CONSUMABLE_CATNO>(value)).contains(strCn);
            if(!verify)
            {
                QMessageBox::warning(NULL,
                                     tr("Warning"),
                                     tr("The type of consumable is wrong, failed installation!"),
                                     QMessageBox::Ok
                                     );
                return;
            }

              /* install */
            strncpy(cn,strCn.toAscii(),APP_CAT_LENGTH);
            strncpy(ln,strLn.toAscii(),APP_LOT_LENGTH);
			bool ret;
   			if(m_bRfidWork)
   			{
            	ret = m_wndMain->updateExConsumableMsg(gGlobalParam.iMachineType, cn, ln, cmIdx, 0, installDate, aIds[iMapIdx].iRfid, true);
   			}
			else
			{
				ret = m_wndMain->updateExConsumableMsg(gGlobalParam.iMachineType, cn, ln, cmIdx, 0, installDate, aIds[iMapIdx].iRfid, false);
			}
              
            strncpy(gGlobalParam.cmSn.aCn[cmIdx], cn, APP_CAT_LENGTH);
            strncpy(gGlobalParam.cmSn.aLn[cmIdx], ln, APP_LOT_LENGTH);
   
            MainResetCmInfo(cmIdx);
   
            m_wndMain->MainWriteCMInstallInfo2Db(cmIdx,0,cn,ln);

            installFeedback(ret);
            
            break;
        }
            
        case 1:
        {
            int macIdx = aIds[iMapIdx].iId - DISP_MACHINERY_SOURCE_BOOSTER_PUMP;

            int value = m_typeMap.key(aIds[iMapIdx].iId);
            bool verify = MainWindow::consumableCatNo(static_cast<CONSUMABLE_CATNO>(value)).contains(strCn);
            if(!verify)
            {
                QMessageBox::warning(NULL,
                                     tr("Warning"),
                                     tr("The type of consumable is wrong, failed installation!"),
                                     QMessageBox::Ok
                                     );
                return;
            }
            if (0 == strcmp(strCn.toAscii(),gGlobalParam.macSn.aCn[macIdx])
               || 0 == strcmp(strLn.toAscii(),gGlobalParam.macSn.aLn[macIdx]))
            {
               return ;
            }
             
             /* install */
            strncpy(cn,strCn.toAscii(),APP_CAT_LENGTH);
            strncpy(ln,strLn.toAscii(),APP_LOT_LENGTH);

            bool ret;
			if(m_bRfidWork)
			{
                ret = m_wndMain->updateExConsumableMsg(gGlobalParam.iMachineType, cn, ln, macIdx, 1, installDate, aIds[iMapIdx].iRfid, true);
			}
            else
            {
                ret = m_wndMain->updateExConsumableMsg(gGlobalParam.iMachineType, cn, ln, macIdx, 1, installDate, aIds[iMapIdx].iRfid, false);
            }
             /* reset state info */
            m_wndMain->MainWriteMacInstallInfo2Db(macIdx,0,cn,ln);

            installFeedback(ret);

            break;  
        }        
                 
        }
    }
   
    m_wndMain->prepareKeyStroke();
}

