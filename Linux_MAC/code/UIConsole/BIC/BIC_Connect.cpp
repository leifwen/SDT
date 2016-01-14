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
#include "BIC_Connect.h"
#include "Comm_Convert.h"
#include "AppLayer.h"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_DISPLAY::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Enable/disable view.");
	if (blDetail == 0)
		return(BI_RETCODE_DISPLAY);
	PrintHelpItem(tBICPAR,"     [on"		,"Always enable.");
	PrintHelpItem(tBICPAR,"     |off"		,"Always disable.");
	PrintHelpItem(tBICPAR,"     |auto]"		,"Set in auto mode.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_DISPLAY::Command(BICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	*ret = "";
	if (par == "on"){
		tBICPAR->blDisplayAuto = 0;
		tBICPAR->oDevNode->Enable();
	}
	else if (par == "off"){
		tBICPAR->blDisplayAuto = 0;
		tBICPAR->oDevNode->Disable();
	}
	else if (par == "auto"){
		tBICPAR->blDisplayAuto = 1;
	}
	else if (par.length() > 0){
		return(Help(tBICPAR));
	}
	if (tBICPAR->blDisplayAuto != 0){
		PrintStrN(tBICPAR," Display is in auto mode.\n",RICH_LIN_clDefault);
	}
	else if (tBICPAR->oDevNode->CheckblEnabled() == 0){
		PrintStrN(tBICPAR," Display is disable.\n",RICH_LIN_clDefault);
	}
	else{
		PrintStrN(tBICPAR," Display is enable.\n",RICH_LIN_clDefault);
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_CONNECT::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Open COM, TCP, UDP, TCP server, UDP server.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"","Goto online mode after connected.");
	PrintHelpItem(tBICPAR,"","Double press ESC + ESC to exit online mode.");
	PrintHelpItem(tBICPAR,"","Double press ESC + A~Z to send 01~1A.");
	PrintHelpItem(tBICPAR,"","Double press ESC + [ to send 1B.");
	PrintHelpItem(tBICPAR,"","Double press ESC + \\ to send 1C.");
	PrintHelpItem(tBICPAR,"","Double press ESC + ] to send 1D.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_CONNECT::Command(BICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	std::string		sendData;
	uint8			chKey;
	
	*ret = "";
	if (par.length() > 0 )
		return(Help(tBICPAR));
	
	if (tBICPAR->sdtApp->m_Device.CheckblConnect() == 0){
		switch(tBICPAR->sdtApp->m_Device.cgDevType){
#ifdef SWVERSION_TCPClient
			case DEVICE::DEVID_TCPClient:
				tBICPAR->sdtApp->m_Device.Open(DEVICE::OP_TCPClient,tBICPAR->sdtApp->m_Device.cEDevFlag.tcpIP,tBICPAR->sdtApp->m_Device.cEDevFlag.tcpPort,tBICPAR->sdtApp->m_Device.cEDevFlag.blEnableEcho);
				break;
#endif
#ifdef SWVERSION_UDPCLient
			case  DEVICE::DEVID_UDPClient:
				tBICPAR->sdtApp->m_Device.Open(DEVICE::OP_UDPClient,tBICPAR->sdtApp->m_Device.cEDevFlag.udpIP,tBICPAR->sdtApp->m_Device.cEDevFlag.udpPort,tBICPAR->sdtApp->m_Device.cEDevFlag.blEnableEcho);
				break;
#endif
#ifdef SWVERSION_TCPServer
			case  DEVICE::DEVID_TCPServer:
				tBICPAR->sdtApp->m_Device.Open(DEVICE::OP_TCPServer,"",tBICPAR->sdtApp->m_Device.cEDevFlag.tcpsPort,tBICPAR->sdtApp->m_Device.cEDevFlag.blEnableEcho);
				break;
#endif
#ifdef SWVERSION_UDPServer
			case  DEVICE::DEVID_UDPServer:
				tBICPAR->sdtApp->m_Device.Open(DEVICE::OP_UDPServer,"",tBICPAR->sdtApp->m_Device.cEDevFlag.udpsPort,tBICPAR->sdtApp->m_Device.cEDevFlag.blEnableEcho);
				break;
#endif
#ifdef SWVERSION_COM
			case  DEVICE::DEVID_APICOM:
				tBICPAR->sdtApp->m_Device.Open(DEVICE::OP_COM,tBICPAR->sdtApp->m_Device.cEDevFlag.com,tBICPAR->sdtApp->m_Device.cEDevFlag.baudrate,tBICPAR->sdtApp->m_Device.cEDevFlag.blEnableEcho);
				break;
#endif
			default:
				break;
		}
	}
	//define ctrl^A~Z is 1~26,HEX is 01~1A. Escape ctrl^~ to 128
	if (tBICPAR->sdtApp->m_Device.CheckblConnect() != 0){
		tBICPAR->blInOnlineMode = 1;
		tBICPAR->blUseSecondLH = 1;
		if (tBICPAR->blDisplayAuto != 0)
			tBICPAR->oDevNode->Enable();
		sendData = "";
		chKey = 0;
		while((tBICPAR->blExit == 0) && (tBICPAR->sdtApp->m_Device.CheckblConnect() != 0)){
			SYS_SleepMS(10);
			chKey = BI_ReadChar(tBICPAR,0);
			if ((chKey >= 32) && (chKey <= 126)){
				sendData += chKey;
			}
			else if (chKey == '\r'){
				sendData += chKey;
				if (tBICPAR->sdtApp->m_Device.cgODevList.cgOutput != nullptr){
					tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->Spin_InUse_set();
					if (tBICPAR->oDevNode->CheckblEnabled() != 0){
						tBICPAR->oDevNode->Disable();
						tBICPAR->sdtApp->m_Device.PrintSendStr(sendData,RICH_CF_clBlack);
						tBICPAR->oDevNode->Enable();
					}
					else{
						tBICPAR->sdtApp->m_Device.PrintSendStr(sendData,RICH_CF_clBlack);
						if (tBICPAR->blDisplayAuto != 0)
							tBICPAR->oDevNode->Enable();
					}
					tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->Spin_InUse_clr();
				}
				tBICPAR->sdtApp->m_Device.WriteInASCII(sendData, G_ESCAPE_OFF);
				sendData = "";
			}
			else if (chKey == 27){
				break;
			}
			else{
				if (chKey == 0x80)
					chKey = 27;
				if ((chKey > 0) && (chKey <= 31)){
					std::string strT;
					strT = '^';
					strT += (chKey + 0x40);
					if (tBICPAR->sdtApp->m_Device.cgODevList.cgOutput != nullptr){
						tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->Spin_InUse_set();
						if (tBICPAR->oDevNode->CheckblEnabled() != 0){
							tBICPAR->oDevNode->Disable();
							tBICPAR->sdtApp->m_Device.PrintSendStr(strT,RICH_CF_clBlack);
							tBICPAR->oDevNode->Enable();
						}
						else{
							tBICPAR->sdtApp->m_Device.PrintSendStr(strT,RICH_CF_clBlack);
							if ((tBICPAR->blDisplayAuto != 0))
								tBICPAR->oDevNode->Enable();
						}
						tBICPAR->sdtApp->m_Device.cgODevList.cgOutput->Spin_InUse_clr();
					}
					tBICPAR->sdtApp->m_Device.Write((uint8*)&chKey,1);
				}
			}
		}
		PrintStrN(tBICPAR, "\r\n", RICH_CF_clBlack);
	}
	tBICPAR->blInOnlineMode = 0;
	tBICPAR->blUseSecondLH = 0;
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_DISCONNECT::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Close COM, TCP, UDP, TCP server, UDP server.");
	return(cgReturnCode);
}//------------------------------------------------------------------------------------------//
int32 BIC_DISCONNECT::Command(BICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	*ret = "";
	if (par.length() > 0)
		return(Help(tBICPAR));
	
	tBICPAR->sdtApp->m_Script.StopRun();
	tBICPAR->sdtApp->m_Device.Close(tBICPAR->sdtApp->m_Device.CheckblConnect());
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_CR::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Connection status report.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [-c]","Clear report.");
	return(cgReturnCode);
}//------------------------------------------------------------------------------------------//
int32 BIC_CR::Command(BICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	*ret = "";
	if (tBICPAR->sdtApp->m_Device.CheckblConnect() == 0){
		PrintDoRet(tBICPAR,"Fail due to no connected");
		return(cgReturnCode);
	}
	if (par == "-c"){
#ifdef CommonDefH_VC
		CMDIFrameWndEx *mainFrame;
		mainFrame = (CMDIFrameWndEx*)theApp.GetMainWnd();
		if (mainFrame != nullptr){
			mainFrame->PostMessageW(WM_ON_PRINTOUT,1,0);
			SYS_SleepMS(100);
		}
#endif
#ifdef CommonDefH_Unix
		tBICPAR->sdtApp->m_Device.Clean();
		tBICPAR->sdtApp->m_Device2.Clean();
#endif
	}
	
	PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
	PrintTitle(tBICPAR,tBICPAR->sdtApp->m_Device.cgDevType);
	PrintStrN(tBICPAR,"Received : " + Str_UInt64ToString(tBICPAR->sdtApp->m_Device.RxBytes()) + " bytes\n",RICH_LIN_clCyan);
	PrintStrN(tBICPAR,"Echo     : " + Str_UInt64ToString(tBICPAR->sdtApp->m_Device.FwBytes()) + " bytes\n",RICH_LIN_clCyan);
	PrintStrN(tBICPAR,"Sent     : " + Str_UInt64ToString(tBICPAR->sdtApp->m_Device.TxBytes()) + " bytes\n",RICH_LIN_clCyan);
	PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_ECHO::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Enable/disable echo.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [on|off]","");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_ECHO::Command(BICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	*ret = "";
	if (par == "on"){
		tBICPAR->sdtApp->m_Device.cEDevFlag.blEnableEcho = 1;
		tBICPAR->sdtApp->m_Device.EnableEcho();
	}
	else if (par == "off"){
		tBICPAR->sdtApp->m_Device.cEDevFlag.blEnableEcho = 0;
		tBICPAR->sdtApp->m_Device.DisableEcho();
	}
	else if(par.length() > 0){
		return(Help(tBICPAR));
	}
	
	if (tBICPAR->sdtApp->m_Device.cEDevFlag.blEnableEcho == 0){
		PrintStrN(tBICPAR," ECHO is disable.\n",RICH_LIN_clDefault);
	}
	else{
		PrintStrN(tBICPAR," ECHO is enable.\n",RICH_LIN_clDefault);
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_REC::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Set receive data report parameter.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [-T<on|off>]"	,"Enable/disable receive tip.");
	PrintHelpItem(tBICPAR,"     [-m<H|A>]"		,"Receive mode: Hex/ASCII.");
	PrintHelpItem(tBICPAR,"     [-O<on|off>]"	,"Enable/disable OSP message delimiter.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_REC::Command(BICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	int32 blcheck;
	std::string		strCommand,strPar1,strPar2;
	*ret = "";
	strCommand = Str_Trim(par);
	
	blcheck = 0;
	do{
		strPar1 = Str_ReadSubItem(&strCommand," ");
		Str_LTrimSelf(strCommand);
		if (strPar1 == "-Ton"){
			B_SetFLAG64(tBICPAR->sdtApp->m_Device.cgODevList.cgODevFlagU64,ODEV_FLAG_EnRecMsg);
			blcheck = 1;
		}
		else if (strPar1 == "-Toff"){
			B_ClrFLAG64(tBICPAR->sdtApp->m_Device.cgODevList.cgODevFlagU64,ODEV_FLAG_EnRecMsg);
			blcheck = 1;
		}
		else if (strPar1 == "-mH"){
			B_SetFLAG64(tBICPAR->sdtApp->m_Device.cgODevList.cgODevFlagU64,ODEV_FLAG_EnHEXViewMode);
			blcheck = 1;
		}
		else if (strPar1 == "-mA"){
			B_ClrFLAG64(tBICPAR->sdtApp->m_Device.cgODevList.cgODevFlagU64,ODEV_FLAG_EnHEXViewMode);
			blcheck = 1;
		}
		
		else if (strPar1 == "-Oon"){
			B_SetFLAG64(tBICPAR->sdtApp->m_Device.cgODevList.cgODevFlagU64,ODEV_FLAG_EnOSPMsgLine);
			blcheck = 1;
		}
		else if (strPar1 == "-Ooff"){
			B_ClrFLAG64(tBICPAR->sdtApp->m_Device.cgODevList.cgODevFlagU64,ODEV_FLAG_EnOSPMsgLine);
			blcheck = 1;
		}
	}while(strCommand.length() > 0);
	
	if ((par.length() > 0) && (blcheck == 0))
		return(Help(tBICPAR));
	
	if (B_ChkFLAG64(tBICPAR->sdtApp->m_Device.cgODevList.cgODevFlagU64,ODEV_FLAG_EnRecMsg) == 0){
		PrintStrN(tBICPAR," disable receive tip,",RICH_LIN_clDefault);
	}
	else{
		PrintStrN(tBICPAR," enable receive tip,",RICH_LIN_clDefault);
	}
	if (B_ChkFLAG64(tBICPAR->sdtApp->m_Device.cgODevList.cgODevFlagU64,ODEV_FLAG_EnHEXViewMode) == 0){
		PrintStr(tBICPAR," ASCII mode,",RICH_LIN_clDefault);
	}
	else{
		PrintStr(tBICPAR," HEX mode,",RICH_LIN_clDefault);
	}
	if (B_ChkFLAG64(tBICPAR->sdtApp->m_Device.cgODevList.cgODevFlagU64,ODEV_FLAG_EnOSPMsgLine) == 0){
		PrintStr(tBICPAR," disable OSP message delimiter.\n",RICH_LIN_clDefault);
	}
	else{
		PrintStr(tBICPAR," enable OSP message delimiter.\n",RICH_LIN_clDefault);
	}
	
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_DEV::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"List COM, TCP, UDP, TCP server, UDP server parameter.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_DEV::Command(BICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	*ret = "";
	if (par.length() > 0 )
		return(Help(tBICPAR));
	
	PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
	
	PrintStr(tBICPAR,"COM",RICH_LIN_clLightBlue);
	PrintStr(tBICPAR,":",RICH_LIN_clDefault);
	PrintStr(tBICPAR,tBICPAR->sdtApp->m_Device.cEDevFlag.com,RICH_LIN_clBrown);
	PrintStr(tBICPAR,"@",RICH_LIN_clDefault);
	PrintStr(tBICPAR,Str_IntToString(tBICPAR->sdtApp->m_Device.cEDevFlag.baudrate),RICH_LIN_clBrown);
	PrintStr(tBICPAR,"\n",RICH_LIN_clDefault);
	
	PrintStr(tBICPAR,"TCP",RICH_LIN_clLightBlue);
	PrintStr(tBICPAR,":",RICH_LIN_clDefault);
	PrintStr(tBICPAR,tBICPAR->sdtApp->m_Device.cEDevFlag.tcpIP,RICH_LIN_clBrown);
	PrintStr(tBICPAR,"@",RICH_LIN_clDefault);
	PrintStr(tBICPAR,Str_IntToString(tBICPAR->sdtApp->m_Device.cEDevFlag.tcpPort),RICH_LIN_clBrown);
	PrintStr(tBICPAR,"\n",RICH_LIN_clDefault);
	
	PrintStr(tBICPAR,"UDP",RICH_LIN_clLightBlue);
	PrintStr(tBICPAR,":",RICH_LIN_clDefault);
	PrintStr(tBICPAR,tBICPAR->sdtApp->m_Device.cEDevFlag.udpIP,RICH_LIN_clBrown);
	PrintStr(tBICPAR,"@",RICH_LIN_clDefault);
	PrintStr(tBICPAR,Str_IntToString(tBICPAR->sdtApp->m_Device.cEDevFlag.udpPort),RICH_LIN_clBrown);
	PrintStr(tBICPAR,"\n",RICH_LIN_clDefault);
	
	PrintStr(tBICPAR,"TCP Server",RICH_LIN_clLightBlue);
	PrintStr(tBICPAR,"@",RICH_LIN_clDefault);
	PrintStr(tBICPAR,Str_IntToString(tBICPAR->sdtApp->m_Device.cEDevFlag.tcpsPort),RICH_LIN_clBrown);
	PrintStr(tBICPAR,"\n",RICH_LIN_clDefault);
	
	PrintStr(tBICPAR,"UDP Server",RICH_LIN_clLightBlue);
	PrintStr(tBICPAR,"@",RICH_LIN_clDefault);
	PrintStr(tBICPAR,Str_IntToString(tBICPAR->sdtApp->m_Device.cEDevFlag.udpsPort),RICH_LIN_clBrown);
	PrintStr(tBICPAR,"\n",RICH_LIN_clDefault);
	
	PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//