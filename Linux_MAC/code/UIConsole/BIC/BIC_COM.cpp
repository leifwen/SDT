/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: BIC_COM.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
 
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "BIC_COM.h"
#include "Comm_Convert.h"
#include "AppLayer.h"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_COM::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Set as COM mode.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [name][br]"		,"COM name and baudrate.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_COM::Command(BICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	std::string		strPar1,strPar2;
	
	*ret = "";
	
	strPar2 = Str_Trim(par);
	strPar1 = Str_ReadSubItem(&strPar2," ");
	Str_LTrimSelf(strPar2);
	
	if (BI_SET_ConnectPar(tBICPAR,par,DEVICE::DEVID_APICOM) == 0)
		PrintDoRet(tBICPAR,"Set fail due to already connected");
	
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_COM_BR::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Set COM baudrate.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     <n>","Baudrate.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_COM_BR::Command(BICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	*ret = "";
	if (par.length() == 0)
		return(Help(tBICPAR));
	
	if (tBICPAR->sdtApp->m_Device.cgDevType == DEVICE::DEVID_APICOM){
		BI_SET_ConnectPar2(tBICPAR,par);
	}
	else{
		PrintDoRet(tBICPAR,"Set fail due to no COM device");
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_COM_DTR::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Set DTR.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [-H[t]]"	,"Set to High, t is time, unit is S.");
	PrintHelpItem(tBICPAR,"     [-L[t]]"	,"Set to Low, t is time, unit is S.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_COM_DTR::Command(BICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	std::string		strPrintData,strPar1,strPar2,strSub;
	double			timeMs;
	SYS_TIME_S		timeS;
	
	*ret = "";
	if ((tBICPAR->sdtApp->m_Device.cgDevType == DEVICE::DEVID_APICOM) && (tBICPAR->sdtApp->m_Device.CheckblConnect() != 0)){
		if (par.length() == 0){
			if (tBICPAR->sdtApp->m_Device.cgAPIECom->GetDTRStatus() == 0){
				PrintStrN(tBICPAR," DTR is high.\n",RICH_LIN_clDefault);
			}
			else{
				PrintStrN(tBICPAR," DTR is low.\n",RICH_LIN_clDefault);
			}
		}
		else{
			strPar2 = par;
			while(strPar2.length() > 0){
				strPar1 = Str_Trim(Str_ReadSubItem(&strPar2, " "));
				if (strPar1 == "-H"){
					tBICPAR->sdtApp->m_Device.cgAPIECom->SetDTRToHigh();
					strPrintData = SYS_MakeTimeNow();
					strPrintData += " Set DTR to high\r\n";
					if (tBICPAR->sdtApp->m_Device.cgODevList.cgOutput != nullptr){
						tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->Spin_InUse_set();
						tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
						tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->WriteStrN(strPrintData,RICH_CF_clPurple,COLSTRING::COL_EP_YES);
						tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
						tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->Spin_InUse_clr();
					}
					return(cgReturnCode);
				}
				else if (strPar1 == "-L"){
					tBICPAR->sdtApp->m_Device.cgAPIECom->SetDTRToLow();
					strPrintData = SYS_MakeTimeNow();
					strPrintData += " Set DTR to Low\r\n";
					if (tBICPAR->sdtApp->m_Device.cgODevList.cgOutput != nullptr){
						tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->Spin_InUse_set();
						tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
						tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->WriteStrN(strPrintData,RICH_CF_clPurple,COLSTRING::COL_EP_YES);
						tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
						tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->Spin_InUse_clr();
					}
					return(cgReturnCode);
				}
				else if(strPar1.length() > 2){
					strSub = strPar1.substr(0,2);
					if (strSub == "-H"){
						strSub = strPar1.substr(2);
						timeMs = atof(strSub.c_str());
						tBICPAR->sdtApp->m_Device.cgAPIECom->SetDTRToHigh();
						strPrintData = SYS_MakeTimeNow();
						SYS_Delay_SetTS(&timeS,timeMs * 1000);
						strPrintData += " Set DTR to high\r\n";
						if (tBICPAR->sdtApp->m_Device.cgODevList.cgOutput != nullptr){
							tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->Spin_InUse_set();
							tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
							tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->WriteStrN(strPrintData,RICH_CF_clPurple,COLSTRING::COL_EP_YES);
							tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
							tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->Spin_InUse_clr();
						}
						
						while ((tBICPAR->blExit == 0) && (SYS_Delay_CheckTS(&timeS) == 0)){
							tBICPAR->blInPressKeyMode = 1;
							if (BI_ReadChar(tBICPAR,0) == 27){
								PrintStrN(tBICPAR,"Pressed ESC, stop execute command.\r\n",RICH_LIN_clRed);
								tBICPAR->blInPressKeyMode = 0;
								return(cgReturnCode);
							}
						}
						tBICPAR->blInPressKeyMode = 0;
						continue;
					}
					else if (strSub == "-L"){
						strSub = strPar1.substr(2);
						timeMs = atof(strSub.c_str());
						tBICPAR->sdtApp->m_Device.cgAPIECom->SetDTRToLow();
						strPrintData = SYS_MakeTimeNow();
						SYS_Delay_SetTS(&timeS,timeMs * 1000);
						strPrintData += " Set DTR to Low\r\n";
						if (tBICPAR->sdtApp->m_Device.cgODevList.cgOutput != nullptr){
							tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->Spin_InUse_set();
							tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
							tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->WriteStrN(strPrintData,RICH_CF_clPurple,COLSTRING::COL_EP_YES);
							tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
							tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->Spin_InUse_clr();
						}
						
						while ((tBICPAR->blExit == 0) && (SYS_Delay_CheckTS(&timeS) == 0)){
							tBICPAR->blInPressKeyMode = 1;
							if (BI_ReadChar(tBICPAR,0) == 27){
								PrintStrN(tBICPAR,"Pressed ESC, stop execute command.\r\n",RICH_LIN_clRed);
								tBICPAR->blInPressKeyMode = 0;
								return(cgReturnCode);
							}
						}
						tBICPAR->blInPressKeyMode = 0;
						continue;
					}
				}
				break;
			}
		}
	}
	else{
		PrintDoRet(tBICPAR,"Set fail due to no COM connected");
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_COM_RTS::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Set RTS.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [-H[t]]"	,"Set to High, t is time, unit is S.");
	PrintHelpItem(tBICPAR,"     [-L[t]]"	,"Set to Low, t is time, unit is S.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_COM_RTS::Command(BICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	std::string		strPrintData,strPar1,strPar2,strSub;
	double			timeMs;
	SYS_TIME_S		timeS;
	
	*ret = "";
	if ((tBICPAR->sdtApp->m_Device.cgDevType == DEVICE::DEVID_APICOM) && (tBICPAR->sdtApp->m_Device.CheckblConnect() != 0)){
		if (par.length() == 0){
			if (tBICPAR->sdtApp->m_Device.cgAPIECom->GetRTSStatus() == 0){
				PrintStrN(tBICPAR," RTS is high.\n",RICH_LIN_clDefault);
			}
			else{
				PrintStrN(tBICPAR," RTS is low.\n",RICH_LIN_clDefault);
			}
		}
		else{
			strPar2 = par;
			while(strPar2.length() > 0){
				strPar1 = Str_Trim(Str_ReadSubItem(&strPar2, " "));
				if (strPar1 == "-H"){
					tBICPAR->sdtApp->m_Device.cgAPIECom->SetRTSToHigh();
					strPrintData = SYS_MakeTimeNow();
					strPrintData += " Set RTS to high\r\n";
					if (tBICPAR->sdtApp->m_Device.cgODevList.cgOutput != nullptr){
						tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->Spin_InUse_set();
						tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
						tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->WriteStrN(strPrintData,RICH_CF_clPurple,COLSTRING::COL_EP_YES);
						tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
						tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->Spin_InUse_clr();
					}
					return(cgReturnCode);
				}
				else if (strPar1 == "-L"){
					tBICPAR->sdtApp->m_Device.cgAPIECom->SetRTSToLow();
					strPrintData = SYS_MakeTimeNow();
					strPrintData += " Set RTS to Low\r\n";
					if (tBICPAR->sdtApp->m_Device.cgODevList.cgOutput != nullptr){
						tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->Spin_InUse_set();
						tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
						tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->WriteStrN(strPrintData,RICH_CF_clPurple,COLSTRING::COL_EP_YES);
						tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
						tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->Spin_InUse_clr();
					}
					return(cgReturnCode);
				}
				else if(strPar1.length() > 2){
					strSub = strPar1.substr(0,2);
					if (strSub == "-H"){
						strSub = strPar1.substr(2);
						timeMs = atof(strSub.c_str());
						tBICPAR->sdtApp->m_Device.cgAPIECom->SetRTSToHigh();
						strPrintData = SYS_MakeTimeNow();
						SYS_Delay_SetTS(&timeS,timeMs * 1000);
						strPrintData += " Set RTS to high\r\n";
						if (tBICPAR->sdtApp->m_Device.cgODevList.cgOutput != nullptr){
							tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->Spin_InUse_set();
							tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
							tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->WriteStrN(strPrintData,RICH_CF_clPurple,COLSTRING::COL_EP_YES);
							tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
							tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->Spin_InUse_clr();
						}
						while ((tBICPAR->blExit == 0) && (SYS_Delay_CheckTS(&timeS) == 0)){
							tBICPAR->blInPressKeyMode = 1;
							if (BI_ReadChar(tBICPAR,0) == 27){
								PrintStrN(tBICPAR,"Pressed ESC, stop execute command.\r\n",RICH_LIN_clRed);
								tBICPAR->blInPressKeyMode = 0;
								return(cgReturnCode);
							}
						}
						tBICPAR->blInPressKeyMode = 0;
						continue;
					}
					else if (strSub == "-L"){
						strSub = strPar1.substr(2);
						timeMs = atof(strSub.c_str());
						tBICPAR->sdtApp->m_Device.cgAPIECom->SetRTSToLow();
						strPrintData = SYS_MakeTimeNow();
						SYS_Delay_SetTS(&timeS,timeMs * 1000);
						strPrintData += " Set RTS to Low\r\n";
						if (tBICPAR->sdtApp->m_Device.cgODevList.cgOutput != nullptr){
							tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->Spin_InUse_set();
							tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
							tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->WriteStrN(strPrintData,RICH_CF_clPurple,COLSTRING::COL_EP_YES);
							tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon,COLSTRING::COL_EP_YES);
							tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->Spin_InUse_clr();
						}
						
						while ((tBICPAR->blExit == 0) && (SYS_Delay_CheckTS(&timeS) == 0)){
							tBICPAR->blInPressKeyMode = 1;
							if (BI_ReadChar(tBICPAR,0) == 27){
								PrintStrN(tBICPAR,"Pressed ESC, stop execute command.\r\n",RICH_LIN_clRed);
								tBICPAR->blInPressKeyMode = 0;
								return(cgReturnCode);
							}
						}
						tBICPAR->blInPressKeyMode = 0;
						continue;
					}
				}
				break;
			}
		}
	}
	else{
		PrintDoRet(tBICPAR,"Set fail due to no COM connected");
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_COM_MS::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Modem status report");
	
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [on|off]"	,"Enable/disable modem status report.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_COM_MS::Command(BICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	std::string		strPrintData;
	*ret = "";
	
	if ((tBICPAR->sdtApp->m_Device.cgDevType == DEVICE::DEVID_APICOM) && (tBICPAR->sdtApp->m_Device.CheckblConnect() != 0)){
		if (par == "on"){
			B_SetFLAG64(tBICPAR->sdtApp->m_Device.cgODevList.cgODevFlagU64, ODEV_FLAG_EnMSReport);
			PrintStrN(tBICPAR," enable modem status report.\n",RICH_LIN_clDefault);
		}
		else if (par == "off"){
			B_ClrFLAG64(tBICPAR->sdtApp->m_Device.cgODevList.cgODevFlagU64, ODEV_FLAG_EnMSReport);
			PrintStrN(tBICPAR," disable modem status report.\n",RICH_LIN_clDefault);
		}
		else if (par.length() == 0){
			if (tBICPAR->sdtApp->m_Device.cgAPIECom->GetDTRStatus() == 0){
				strPrintData = " DTR=L,";
			}
			else{
				strPrintData += " DTR=H,";
			}
			if (tBICPAR->sdtApp->m_Device.cgAPIECom->GetRTSStatus() == 0){
				strPrintData += " RTS=L,";
			}
			else{
				strPrintData += " RTS=H,";
			}
			strPrintData += (" CTS=" + tBICPAR->sdtApp->m_Device.cgAPIECom->GetCTSStatus() + ",");
			strPrintData += (" DSR=" + tBICPAR->sdtApp->m_Device.cgAPIECom->GetDSRStatus() + ",");
			strPrintData += (" RING=" + tBICPAR->sdtApp->m_Device.cgAPIECom->GetRINGStatus() + ",");
			strPrintData += (" DCD=" + tBICPAR->sdtApp->m_Device.cgAPIECom->GetDCDStatus() + ",");
			PrintStrN(tBICPAR,strPrintData + "\n",RICH_LIN_clDefault);
		}
		else{
			Help(tBICPAR);
		}
	}
	else{
		PrintDoRet(tBICPAR,"Set fail due to no COM connected");
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_COM_LS::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"List valid COM.");
	if (blDetail == 0)
		return(cgReturnCode);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_COM_LS::Command(BICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	tBICPAR->validComList->Refresh();
	PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);

	tBICPAR->validComList->Spin_InUse_set();
	RTREE_LChildRChain_Traversal_LINE_nolock(IPCOMNAME,tBICPAR->validComList,
		if ((operateNode_t->typeID == PublicDevice_DEVID_APICOM) && (operateNode_t->blAvailable != 0))
			PrintStrN(tBICPAR,operateNode_t->strShowName + "\n",RICH_LIN_clCyan);
	);
	tBICPAR->validComList->Spin_InUse_clr();
	PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//