/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: BIC_CMUX.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
 
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "BIC_CMUX.h"
#include "Comm_Convert.h"
#include "AppLayer.h"
//------------------------------------------------------------------------------------------//
int32 BIC_CMUX::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"->CMUX.");
	if (blDetail == 0)
		return(cgReturnCode);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_CMUX_ON::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Start CMUX.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [n]"		,"Start CMUX with external group initialization.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_CMUX_ON::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	*ret = "";
#ifdef	SWVERSION_CMUX
	COMMAND_GROUP	*group;
	int32			retCode;
	uint32			gDID;
	std::string		strPar1;
	
	if (tBICPAR->sdtApp->m_Device.cgDevType != DEVICE::DEVID_APICOM || tBICPAR->sdtApp->m_Device.CheckblConnect() == 0){
		PrintDoRet(tBICPAR,"Not connect to COM device");
		return(cgReturnCode);
	}
	
	strPar1 = Str_Trim(par);
	tBICPAR->oDevNode->Enable();
	if (strPar1.length() > 0){
		gDID = (uint32)strtoll(strPar1.c_str(),nullptr,10);

		tBICPAR->sdtApp->m_GCList.Spin_InUse_set();
		group = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID_nolock(&tBICPAR->sdtApp->m_GCList, gDID);
		if (group != nullptr){
			group->Spin_InUse_set();
			retCode = tBICPAR->sdtApp->m_Script.Execute(&tBICPAR->sdtApp->m_Device,group);
			group->Spin_InUse_clr();
			if (retCode == 0)
				PrintDoRet(tBICPAR,"Script is running!");
			group = nullptr;
		}
		else{
			PrintDoRet(tBICPAR,"No found external group");
		}
		tBICPAR->sdtApp->m_GCList.Spin_InUse_clr();
		if (group == nullptr)
			return(cgReturnCode);
		
		while(tBICPAR->blExit == 0){
			uint8	chkey;
			SYS_SleepMS(10);
			tBICPAR->blInPressKeyMode = 1;
			chkey = BI_ReadChar(tBICPAR,0);
			if (chkey == 27){
				tBICPAR->sdtApp->m_Script.StopRun();
				tBICPAR->blInPressKeyMode = 0;
				return(cgReturnCode);
			}
			else if (chkey == '\r'){
				tBICPAR->oDevNode->Enable();
			}
			if (tBICPAR->sdtApp->m_Script.IsStop() != 0)
				break;
		}
		tBICPAR->blInPressKeyMode = 0;
	}
	tBICPAR->sdtApp->m_CMUXDriver.CMUXStart(0);
	while(tBICPAR->blExit == 0){
		if (PressAnyKey(tBICPAR) == 27)
			break;
	}
	PrintStrN(tBICPAR,"\r\n",RICH_LIN_clDefault);
#endif
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_CMUX_OFF::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Stop CMUX.");
	if (blDetail == 0)
		return(cgReturnCode);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_CMUX_OFF::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	*ret = "";
#ifdef	SWVERSION_CMUX
	tBICPAR->oDevNode->Enable();
	tBICPAR->sdtApp->m_CMUXDriver.CMUXStop();
	SYS_SleepMS(50);
	PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
	PrintStrN(tBICPAR,"CMUX stopped.\n",RICH_LIN_clCyan);
	PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
#endif
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_CMUX_CLD::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Send CLD.");
	if (blDetail == 0)
		return(cgReturnCode);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_CMUX_CLD::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	*ret = "";
#ifdef	SWVERSION_CMUX
	if (tBICPAR->sdtApp->m_CMUXDriver.CheckblStart() == 0){
		PrintDoRet(tBICPAR,"CMUX No Start");
		return(cgReturnCode);
	}
	tBICPAR->oDevNode->Enable();
	tBICPAR->sdtApp->m_CMUXDriver.SendCMUXCLD();
	PressAnyKey(tBICPAR);
#endif
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_CMUX_PSC::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Send PSC.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [n]"		,"num.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_CMUX_PSC::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	*ret = "";
#ifdef	SWVERSION_CMUX
	int32	num;
	tBICPAR->oDevNode->Enable();
	if (tBICPAR->sdtApp->m_CMUXDriver.CheckblStart() == 0){
		PrintDoRet(tBICPAR,"CMUX No Start");
		return(cgReturnCode);
	}
	if (par.length() == 0){
		tBICPAR->sdtApp->m_CMUXDriver.SendCMUXStdPSC(0xff);
	}
	else{
		num = atoi(par.c_str());
		tBICPAR->sdtApp->m_CMUXDriver.SendCMUXStdPSC(num);
	}
	PressAnyKey(tBICPAR);
#endif
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_CMUX_FC::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Send FC.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     <on|off>","On/Off.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_CMUX_FC::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	*ret = "";
#ifdef	SWVERSION_CMUX
	if (tBICPAR->sdtApp->m_CMUXDriver.CheckblStart() == 0){
		PrintDoRet(tBICPAR,"CMUX No Start");
		return(cgReturnCode);
	}
	tBICPAR->oDevNode->Enable();
	if (par.length() > 0){
		if (par == "on"){
			tBICPAR->sdtApp->m_CMUXDriver.SendFCC(1);
			PressAnyKey(tBICPAR);
			return(cgReturnCode);
		}
		else if (par == "off"){
			tBICPAR->sdtApp->m_CMUXDriver.SendFCC(0);
			PressAnyKey(tBICPAR);
			return(cgReturnCode);
		}
	}
	PrintDoRet(tBICPAR,"ERROR");
#endif
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_CMUX_DTR::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Set DTR.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     <num>"		,"CUX virtual COM num(1-4).");
	PrintHelpItem(tBICPAR,"     <[-H[t]]"	,"Set to High, t is time, unit is S.");
	PrintHelpItem(tBICPAR,"      [-L[t]]>"	,"Set to Low, t is time, unit is S.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_CMUX_DTR::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
#ifdef	SWVERSION_CMUX
	std::string		strSub,strPar1,strPar2;
	double			timeMs;
	SYS_TIME_S		timeS;
	int32			num;
	CMUXCOM			*tCMUXCOM;
	*ret = "";
	if (tBICPAR->sdtApp->m_CMUXDriver.CheckblStart() == 0){
		PrintDoRet(tBICPAR,"CMUX No Start");
		return(cgReturnCode);
	}
	strPar2 = Str_Trim(par);
	strPar1 = Str_ReadSubItem(&strPar2," ");
	Str_LTrimSelf(strPar2);
	
	num = atoi(strPar1.c_str());
	if ((num == 0) || (num > CMUX_MAX_DLCI_INDEX)){
		PrintDoRet(tBICPAR,"ERROR");
		return(cgReturnCode);
	}
	tBICPAR->sdtApp->m_CMUXDriver.Spin_InUse_set();
	tCMUXCOM = tBICPAR->sdtApp->m_CMUXDriver.Find_nolock(num);
	while(strPar2.length() > 0){
		strPar1 = Str_Trim(Str_ReadSubItem(&strPar2, " "));
		if (strPar1 == "-H"){
			tCMUXCOM->vPortDTR = 1;
			tBICPAR->sdtApp->m_CMUXDriver.SendMSC(num,tCMUXCOM->vPortDTR,tCMUXCOM->vPortRTS);
		}
		else if (strPar1 == "-L"){
			tCMUXCOM->vPortDTR = 0;
			tBICPAR->sdtApp->m_CMUXDriver.SendMSC(num,tCMUXCOM->vPortDTR,tCMUXCOM->vPortRTS);
		}
		else if(strPar1.length() > 2){
			strSub = strPar1.substr(0,2);
			strSub = strPar1.substr(2);
			timeMs = atof(strSub.c_str());

			if (strSub == "-H"){
				tCMUXCOM->vPortDTR = 1;
			}
			else if (strSub == "-L"){
				tCMUXCOM->vPortDTR = 0;
			}
			tBICPAR->sdtApp->m_CMUXDriver.SendMSC(num,tCMUXCOM->vPortDTR,tCMUXCOM->vPortRTS);
			SYS_Delay_SetTS(&timeS,timeMs * 1000);
			
			while ((tBICPAR->blExit == 0) && (SYS_Delay_CheckTS(&timeS) == 0)){
				tBICPAR->blInPressKeyMode = 1;
				if (BI_ReadChar(tBICPAR,0) == 27){
					PrintStrN(tBICPAR,"Pressed ESC, stop execute command.\r\n",RICH_LIN_clRed);
					tBICPAR->blInPressKeyMode = 0;
					break;
				}
			}
			tBICPAR->blInPressKeyMode = 0;
			continue;
		}
		break;
	}
	tBICPAR->sdtApp->m_CMUXDriver.Spin_InUse_clr();
#endif
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_CMUX_RTS::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Set RTS.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     <num>"		,"CUX virtual COM num(1-4).");
	PrintHelpItem(tBICPAR,"     <[-H[t]]"	,"Set to High, t is time, unit is S.");
	PrintHelpItem(tBICPAR,"      [-L[t]]>"	,"Set to Low, t is time, unit is S.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_CMUX_RTS::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
#ifdef	SWVERSION_CMUX
	std::string		strSub,strPar1,strPar2;
	double			timeMs;
	SYS_TIME_S		timeS;
	int32			num;
	CMUXCOM			*tCMUXCOM;
	*ret = "";
	if (tBICPAR->sdtApp->m_CMUXDriver.CheckblStart() == 0){
		PrintDoRet(tBICPAR,"CMUX No Start");
		return(cgReturnCode);
	}
	strPar2 = Str_Trim(par);
	strPar1 = Str_ReadSubItem(&strPar2," ");
	Str_LTrimSelf(strPar2);
	
	num = atoi(strPar1.c_str());
	if ((num == 0) || (num > CMUX_MAX_DLCI_INDEX)){
		PrintDoRet(tBICPAR,"ERROR");
		return(cgReturnCode);
	}
	tBICPAR->sdtApp->m_CMUXDriver.Spin_InUse_set();
	tCMUXCOM = tBICPAR->sdtApp->m_CMUXDriver.Find_nolock(num);
	while(strPar2.length() > 0){
		strPar1 = Str_Trim(Str_ReadSubItem(&strPar2, " "));
		if (strPar1 == "-H"){
			tCMUXCOM->vPortRTS = 1;
			tBICPAR->sdtApp->m_CMUXDriver.SendMSC(num,tCMUXCOM->vPortDTR,tCMUXCOM->vPortRTS);
		}
		else if (strPar1 == "-L"){
			tCMUXCOM->vPortRTS = 0;
			tBICPAR->sdtApp->m_CMUXDriver.SendMSC(num,tCMUXCOM->vPortDTR,tCMUXCOM->vPortRTS);
		}
		else if(strPar1.length() > 2){
			strSub = strPar1.substr(0,2);
			strSub = strPar1.substr(2);
			timeMs = atof(strSub.c_str());
			
			if (strSub == "-H"){
				tCMUXCOM->vPortRTS = 1;
			}
			else if (strSub == "-L"){
				tCMUXCOM->vPortRTS = 0;
			}
			tBICPAR->sdtApp->m_CMUXDriver.SendMSC(num,tCMUXCOM->vPortDTR,tCMUXCOM->vPortRTS);
			SYS_Delay_SetTS(&timeS,timeMs * 1000);
			
			while ((tBICPAR->blExit == 0) && (SYS_Delay_CheckTS(&timeS) == 0)){
				tBICPAR->blInPressKeyMode = 1;
				if (BI_ReadChar(tBICPAR,0) == 27){
					PrintStrN(tBICPAR,"Pressed ESC, stop execute command.\r\n",RICH_LIN_clRed);
					tBICPAR->blInPressKeyMode = 0;
					break;
				}
			}
			tBICPAR->blInPressKeyMode = 0;
			continue;
		}
		break;
	}
	tBICPAR->sdtApp->m_CMUXDriver.Spin_InUse_clr();
#endif
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_DLCI::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Set/list CMUX virtual COM status.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [num"			,"CUX virtual COM num(1-4).");
	PrintHelpItem(tBICPAR,"      <-m<H|A>"		,"Receive mode: Hex/ASCII.");
	PrintHelpItem(tBICPAR,"      |-E<on|off>>]"	,"Enable/disable escape.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_DLCI::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
#ifdef	SWVERSION_CMUX
	std::string		strPar1,strPar2,strPrintData;
	int32			num,retCode;
	CMUXCOM			*tCMUXCOM;
	*ret = "";
	if (tBICPAR->sdtApp->m_CMUXDriver.CheckblStart() == 0){
		PrintDoRet(tBICPAR,"CMUX No Start");
		return(cgReturnCode);
	}
	
	if (par.length() == 0){
		num = 0;
		PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
		tBICPAR->sdtApp->m_CMUXDriver.Spin_InUse_set();
		while(num++ < CMUX_MAX_DLCI_INDEX){
			tCMUXCOM = tBICPAR->sdtApp->m_CMUXDriver.Find_nolock(num);
			if (tCMUXCOM == nullptr)
				continue;
			strPrintData = " DLCI " + Str_IntToString(num) + " :";
			if (tCMUXCOM->vPortDTR == 0){
				strPrintData += " DTR=L,";
			}
			else{
				strPrintData += " DTR=H,";
			}
			if (tCMUXCOM->vPortRTS == 0){
				strPrintData += " RTS=L,";
			}
			else{
				strPrintData += " RTS=H,";
			}
			if (tCMUXCOM->vPortCTS == 0){
				strPrintData += " CTS=L,";
			}
			else{
				strPrintData += " CTS=H,";
			}
			if (tCMUXCOM->vPortDSR == 0){
				strPrintData += " DSR=L,";
			}
			else{
				strPrintData += " DSR=H,";
			}
			if (tCMUXCOM->vPortRING == 0){
				strPrintData += " RING=L,";
			}
			else{
				strPrintData += " RING=H,";
			}
			if (tCMUXCOM->vPortDCD == 0){
				strPrintData += " DCD=L,";
			}
			else{
				strPrintData += " DCD=H,";
			}
			if (tCMUXCOM->vPortHEX == 0){
				strPrintData += " ASCII mode,";
			}
			else{
				strPrintData += " HEX mode,";
			}
			if (tCMUXCOM->vPortEscape == 0){
				strPrintData += " disable escape.\n";
			}
			else{
				strPrintData += " enable escape.\n";
			}
			PrintStrN(tBICPAR,strPrintData,RICH_LIN_clCyan);
		}
		tBICPAR->sdtApp->m_CMUXDriver.Spin_InUse_clr();
		PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
		return(cgReturnCode);
	}
	
	strPar2 = Str_Trim(par);
	strPar1 = Str_ReadSubItem(&strPar2," ");
	Str_LTrimSelf(strPar2);
	
	num = atoi(strPar1.c_str());
	if ((num == 0) || (num > CMUX_MAX_DLCI_INDEX)){
		PrintDoRet(tBICPAR,"ERROR");
		return(cgReturnCode);
	}
	tBICPAR->sdtApp->m_CMUXDriver.Spin_InUse_set();
	tCMUXCOM = tBICPAR->sdtApp->m_CMUXDriver.Find_nolock(num);
	do{
		Str_LTrimSelf(strPar2);
		
		if (strPar2.length() == 0){
			PrintDoRet(tBICPAR,"ERROR");
			break;
		}
		
		retCode = 0;
		if (strPar2 == "-mH"){
			tCMUXCOM->vPortHEX = 1;
			tCMUXCOM->vPortEscape = 0;
			retCode = 1;
		}
		else if (strPar2 == "-mA"){
			tCMUXCOM->vPortHEX = 0;
			retCode = 1;
		}
		else if (strPar2 == "-Eon"){
			tCMUXCOM->vPortEscape = 1;
			tCMUXCOM->vPortHEX = 0;
			retCode = 1;
		}
		else if (strPar2 == "-Eoff"){
			tCMUXCOM->vPortEscape = 0;
			retCode = 1;
		}
		if (retCode == 1){
			strPrintData = " DLCI " + Str_IntToString(num) + " :";
			if (tCMUXCOM->vPortHEX == 0){
				strPrintData += " ASCII mode,";
			}
			else{
				strPrintData += " HEX mode,";
			}
			if (tCMUXCOM->vPortEscape == 0){
				strPrintData += " disable escape.\n";
			}
			else{
				strPrintData += " enable escape.\n";
			}
			PrintStrN(tBICPAR,strPrintData,RICH_LIN_clDefault);
		}
	}while(0);
	tBICPAR->sdtApp->m_CMUXDriver.Spin_InUse_clr();
#endif
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//