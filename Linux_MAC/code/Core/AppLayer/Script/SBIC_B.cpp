/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: SBIC_B.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.08.12
 
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "SBIC_B.h"
#include "Comm_Convert.h"
#include "Device.h"
#include "CommandGList.h"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_Node::ExecuteLC(SBICPAR *tBICPAR,const std::string &fullCMD,std::string *ret)const{
	int32	retCode;
	
	retCode = SBI_RETCODE_NO;
	
	RTREE_LChildRChain_Traversal_LINE_nolock(SBIC_Node,const_cast<SBIC_Node *>(this),
		retCode = operateNode_t->Execute(tBICPAR,fullCMD,ret);
		if (retCode != SBI_RETCODE_NO)
			break;
	);
	return(retCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_Node::HelpLC(SBICPAR *tBICPAR,int32 blDetail)const{
	RTREE_LChildRChain_Traversal_LINE_nolock(SBIC_Node,const_cast<SBIC_Node *>(this),operateNode_t->Help(tBICPAR,blDetail));
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_Node::Check(SBICPAR *tBICPAR,const std::string &fullCMD,std::string *retPar)const{
	std::string		oCommand,oFullCMD,tmpCMD,retT,strT;
	std::string		subCommand,subStart,subEnd;
	oFullCMD = Str_LTrim(fullCMD);
	oCommand = Str_LowerCase(cgCommand);
	
	do{
		tmpCMD = oFullCMD;
		subEnd = Str_ReadSubItem(&oCommand,"/");
		subCommand = "'" + Str_ReadSubItem(&subEnd,",");
		subStart = Str_ReadSubItem(&subEnd,",");
		if (subStart.length() > 0){
			strT = Str_LowerCase(Str_RTrim(Str_ReadSubItem(&tmpCMD,subStart)));
			if (strT == subCommand){
				if (subEnd.length() > 0){
					if (tmpCMD.length() >= subEnd.length()){
						strT = tmpCMD.substr(tmpCMD.length() - subEnd.length());
						if (strT == subEnd){
							*retPar = Str_Trim(tmpCMD.substr(0,tmpCMD.length() - subEnd.length()));
							return(cgReturnCode);
						}
					}
				}
				else{
					*retPar = Str_Trim(tmpCMD);
					return(cgReturnCode);
				}
			}
		}
		else if (Str_LowerCase(tmpCMD) == subCommand){
			*retPar = "";
			return(cgReturnCode);
		}
	}while (oCommand.length() > 0);
	*retPar = "";
	return(SBI_RETCODE_NO);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_Node::Execute(SBICPAR *tBICPAR,const std::string &fullCMD,std::string *ret)const{
	std::string	 retPar;
	
	if (Check(tBICPAR,fullCMD,&retPar) == cgReturnCode)
		return(Command(tBICPAR,retPar,ret));
	return(SBI_RETCODE_NO);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_Node::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_Node::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	return(ExecuteLC(tBICPAR, par, ret));
}
//------------------------------------------------------------------------------------------//
std::string	SBIC_Node::DelComment(const std::string &strInput){
	//  escape \/ ,//
	//  abcd \\//
	std::string::size_type			i,length;
	std::string		strRet;
	uint8			charData,charData1;
	
	length = strInput.length();
	i = 0;
	strRet = "";
	
	if  (length > 0){
		while(i++ < length){
			charData = strInput[i - 1];
			if (i < length){
				charData1 = strInput[i];
				if ((charData == '/') && (charData1 == '/'))
					break;
				if ((charData == '\\') || (charData == '/')){
					strRet += charData;
					strRet += charData1;
					++ i;
					continue;
				}
			}
			strRet += charData;
		};
	}
	return(Str_Trim(strRet));
}
//------------------------------------------------------------------------------------------//
void SBIC_Node::PrintExecute(SBICPAR *tBICPAR,const std::string &strCommand){
	std::string	strTempData;
	
	if ((tBICPAR != nullptr) && (tBICPAR->cgDevice != nullptr) && (tBICPAR->cgDevice->cEDevFlag.blEnablePrintSBICinfo != 0)){
		strTempData = SYS_MakeTimeNow();
		strTempData += " Execute: ";
		strTempData += strCommand;
		strTempData += ".\r\n";
		if (tBICPAR->cgDevice->cgODevList.cgOutput != nullptr){
			tBICPAR->cgDevice->cgODevList.cgOutput->Spin_InUse_set();
			tBICPAR->cgDevice->cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon);
			tBICPAR->cgDevice->cgODevList.cgOutput->WriteStrN(strTempData,RICH_CF_clMaroon);
			tBICPAR->cgDevice->cgODevList.cgOutput->Spin_InUse_clr();
		}
	}
}
//------------------------------------------------------------------------------------------//
int32 SBIC_Node::GetMS(const std::string &par){
	std::string	strUint;
	double	ms;
	
	ms = atof(par.c_str()) * 1000;
	if (par.length() > 3){
		strUint = Str_LowerCase(par.substr(par.length() - 2));
		if (strUint == "ms")
			ms /= 1000;
	}
	return((int32)ms);
}
//------------------------------------------------------------------------------------------//
std::string SBIC_Node::GetMSSTR(int32 ms){
	std::string	ret;
	
	if (ms >= 1000){
		ret = Str_FloatToString((double)ms / 1000.0) + "s";
	}
	else if (ms > 0){
		ret = Str_IntToString(ms) + "ms";
	}
	else{
		ret = "0s";
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//



//------------------------------------------------------------------------------------------//
int32 SBIC_Delay::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = Delay=<PAR> -->Delay PAR time, then goto send next \"Command\". Default unit is second.");
	PrintB(tBICPAR,"  Command = <'Delay=<PAR>>[//COMMENT]");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"     Command = 'Delay=1.2      //delay 1.2s");
	PrintP(tBICPAR,"     Command = 'Delay=1200ms   //delay 1200ms");
	PrintP(tBICPAR,"     Command = 'Delay=1.2s     //delay 1.2s");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_Delay::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	int32 		timeout;
	std::string	strPar1,strPar2;
	*ret = "";
	
	strPar2 = par;
	timeout = GetMS(Str_Trim(Str_ReadSubItem(&strPar2,",")));
	
	PrintExecute(tBICPAR,"Delay " + GetMSSTR(timeout));
	SYS_DelayMS(timeout,&tBICPAR->blExit);
	
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_DTR::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = DTR=<H|L>[,<PAR>] -->Set DTR output high/low and delay PAR time, then goto send next \"Command\".");
	PrintB(tBICPAR,"  Command = <'DTR=<H|L>[,<PAR>]>[//COMMENT]");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"     Command = 'DTR=H,1.2    //Set DTR output high, delay 1.2s");
	PrintP(tBICPAR,"     Command = 'DTR=L,1.2s   //Set DTR output low, delay 1.2s");
	PrintP(tBICPAR,"     Command = 'DTR=L,500ms  //Set DTR output low, delay 500ms");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_DTR::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	int32 		timeout;
	std::string	strPar1,strPar2,strTempData;
	
	*ret = "";
	
	if ((tBICPAR != nullptr) && (tBICPAR->cgDevice != nullptr) && (tBICPAR->cgDevice->cgDevType == DEVICE::DEVID_APICOM)){
		strPar2 = par;
		strPar1 = Str_LowerCase(Str_Trim(Str_ReadSubItem(&strPar2,",")));
		timeout = GetMS(Str_Trim(Str_ReadSubItem(&strPar2,",")));
		
		strTempData = "Set DTR to ";
		if ((strPar1 == "h") || (strPar1 == "1") || (strPar1 == "high")){
			strTempData += "high";
			if (timeout > 0){
				strTempData += ", and delay ";
				strTempData += GetMSSTR(timeout);
			}
			PrintExecute(tBICPAR,strTempData);
			tBICPAR->cgDevice->cgAPIECom->SetDTRToHigh();
			SYS_DelayMS(timeout,&tBICPAR->blExit);
		}else if ((strPar1 == "l") || (strPar1 == "0") || (strPar1 == "low")){
			strTempData += "low";
			if (timeout > 0){
				strTempData += ", and delay ";
				strTempData += GetMSSTR(timeout);
			}
			PrintExecute(tBICPAR,strTempData);
			tBICPAR->cgDevice->cgAPIECom->SetDTRToLow();
			SYS_DelayMS(timeout,&tBICPAR->blExit);
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_RTS::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = RTS=<H|L>[,<PAR>] -->Set RTS output high/low and delay PAR time, then goto send next \"Command\".");
	PrintB(tBICPAR,"  Command = <'RTS=<H|L>[,<PAR>]>[//COMMENT]");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"     Command = 'RTS=H,1.2    //Set RTS output high, delay 1.2s");
	PrintP(tBICPAR,"     Command = 'RTS=L,1.2s   //Set RTS output low, delay 1.2s");
	PrintP(tBICPAR,"     Command = 'RTS=L,500ms  //Set RTS output low, delay 500ms");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_RTS::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	int32 		timeout;
	std::string	strPar1,strPar2,strTempData;
	
	*ret = "";
	if ((tBICPAR != nullptr) && (tBICPAR->cgDevice != nullptr) && (tBICPAR->cgDevice->cgDevType == DEVICE::DEVID_APICOM)){
		strPar2 = par;
		strPar1 = Str_LowerCase(Str_Trim(Str_ReadSubItem(&strPar2,",")));
		timeout = GetMS(Str_Trim(Str_ReadSubItem(&strPar2,",")));
		
		strTempData = "Set RTS to ";
		if ((strPar1 == "h") || (strPar1 == "1") || (strPar1 == "high")){
			strTempData += "high";
			if (timeout > 0){
				strTempData += ", and delay ";
				strTempData += GetMSSTR(timeout);
			}
			PrintExecute(tBICPAR,strTempData);
			tBICPAR->cgDevice->cgAPIECom->SetRTSToHigh();
			SYS_DelayMS(timeout,&tBICPAR->blExit);
		}else if ((strPar1 == "l") || (strPar1 == "0") || (strPar1 == "low")){
			strTempData += "low";
			if (timeout > 0){
				strTempData += ", and delay ";
				strTempData += GetMSSTR(timeout);
			}
			PrintExecute(tBICPAR,strTempData);
			tBICPAR->cgDevice->cgAPIECom->SetRTSToLow();
			SYS_DelayMS(timeout,&tBICPAR->blExit);
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_ChangeCom::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = ChangeCom=<PAR1>,<PAR2> -->Change COM to PAR1, baud rate to PAR2.");
	PrintB(tBICPAR,"  Command = <'ChangeCom=<PAR1>,<PAR2>>[//COMMENT]");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"     Command = 'ChangeCom=COM3,115200  //set COM is COM3, baud rate is 115200");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_ChangeCom::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	int32 		baudrate;
	std::string	port,strTempData;
	
	*ret = 'F';
	
	if ((tBICPAR != nullptr) && (tBICPAR->cgDevice != nullptr)){
		tBICPAR->cgDevice->Close(0);
		strTempData = par;
		port = Str_Trim(Str_ReadSubItem(&strTempData, ","));
		baudrate = atoi(Str_Trim(strTempData).c_str());
		
		strTempData = "Change to ";
		strTempData += port;
		strTempData += ',';
		strTempData += Str_IntToString(baudrate);
		PrintExecute(tBICPAR,strTempData);
		if (tBICPAR->cgDevice->Open(DEVICE::OP_COM,port,baudrate,tBICPAR->cgDevice->cEDevFlag.blEnableEcho) != 0){
			*ret = 'T';
		}
		else if (tBICPAR->cgCommand != nullptr){
			tBICPAR->cgCommand->StrCatch = "Fail";
			++ tBICPAR->cgCommand->catchTimes;
		}
		tBICPAR->cgFunRet = *ret;
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_ChangeBR::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = ChangeBR=<PAR> -->Set baud rate.");
	PrintB(tBICPAR,"   <PAR> = 1200,2400,9600,14400,19200,38400,56000,57600");
	PrintB(tBICPAR,"          ,115200,128000,230400,256000,460800,912600.");
	PrintB(tBICPAR,"  Command = <'ChangeBR=<PAR>>[//COMMENT]");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"     Command = 'ChangeBR=115200  //set baud rate 115200");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_ChangeBR::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	int32 		baudrate;
	std::string	strTempData;
	
	*ret = "";
	
	if ((tBICPAR != nullptr) && (tBICPAR->cgDevice != nullptr) && (tBICPAR->cgDevice->cgDevType == DEVICE::DEVID_APICOM)){
		baudrate = atoi(Str_Trim(par).c_str());
		strTempData = "Change Baudrate to " + Str_IntToString(baudrate);
		PrintExecute(tBICPAR,strTempData);
		tBICPAR->cgDevice->cgAPIECom->SetBaudrate(baudrate);
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_OpenTCPSocket::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = OpenTCPSocket/OTCP=<PAR1>,<PAR2> -->Open TCP socket, PAR1 is IP, PAR2 is port.");
	PrintB(tBICPAR,"  Command = <'OpenTCPSocket=<PAR1>,<PAR2>>[//COMMENT]");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"     Command = 'OpenTCPSocket=127.0.0.1,9527  //Open TCP socket");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_OpenTCPSocket::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	int32 		port;
	std::string	IP,strTempData;
	
	*ret = 'F';
	if ((tBICPAR != nullptr) && (tBICPAR->cgDevice != nullptr)){
		tBICPAR->cgDevice->Close(0);
		tBICPAR->blRSC = 1;
		strTempData = par;
		IP = Str_Trim(Str_ReadSubItem(&strTempData, ","));
		port = atoi(Str_Trim(strTempData).c_str());
		
		strTempData = "Open ";
		strTempData += IP;
		strTempData += ',';
		strTempData += Str_IntToString(port);
		PrintExecute(tBICPAR,strTempData);
		
		if (tBICPAR->cgDevice->Open(DEVICE::OP_TCPClient,IP,port,tBICPAR->cgDevice->cEDevFlag.blEnableEcho) != 0){
			*ret = 'T';
		}
		else if (tBICPAR->cgCommand != nullptr){
			tBICPAR->cgCommand->StrCatch = "Fail";
			++ tBICPAR->cgCommand->catchTimes;
		}
		tBICPAR->cgFunRet = *ret;
	}
	
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_CloseTCPSocket::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = CloseTCPSocket/CTCP -->Close socket.");
	PrintB(tBICPAR,"  Command = <'CloseTCPSocket>[//COMMENT]");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"     Command = 'CloseTCPSocket  //Close socket");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_CloseTCPSocket::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	std::string	strTempData;
	
	*ret = 'F';
	if ((tBICPAR != nullptr) && (tBICPAR->cgDevice != nullptr) && (tBICPAR->cgDevice->cgDevType == DEVICE::DEVID_TCPClient)){
		tBICPAR->blRSC = 1;
		strTempData = "Close TCP Socket";
		PrintExecute(tBICPAR,strTempData);
		if (tBICPAR->cgDevice->cgAPISocket->CloseD(1) != 0){
			strTempData = "Close TCP Socket, success";
			*ret = 'T';
			tBICPAR->blRSC = 1;
		}
		else{
			strTempData = "Close TCP Socket, fail";
			if (tBICPAR->cgCommand != nullptr){
				tBICPAR->cgCommand->StrCatch = "Fail";
				++ tBICPAR->cgCommand->catchTimes;
			}
		}
		PrintExecute(tBICPAR,strTempData);
		tBICPAR->cgFunRet = *ret;
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_OpenUDPSocket::Help(SBICPAR *tBICPAR, int32 blDetail)const{
	PrintB(tBICPAR, ".CMD = OpenUDPSocket/OUDP=<PAR1>,<PAR2> -->Open UDP socket, PAR1 is IP, PAR2 is port.");
	PrintB(tBICPAR, "  Command = <'OpenUDPSocket=<PAR1>,<PAR2>>[//COMMENT]");
	PrintP(tBICPAR, "   eg:");
	PrintP(tBICPAR, "     Command = 'OpenUDPSocket=127.0.0.1,9527  //Open UDP socket");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_OpenUDPSocket::Command(SBICPAR *tBICPAR, const std::string &par, std::string *ret)const{
	int32 		port;
	std::string	IP, strTempData;
	
	*ret = 'F';
	if ((tBICPAR != nullptr) && (tBICPAR->cgDevice != nullptr)){
		tBICPAR->cgDevice->Close(0);
		tBICPAR->blRSC = 1;
		strTempData = par;
		IP = Str_Trim(Str_ReadSubItem(&strTempData, ","));
		port = atoi(Str_Trim(strTempData).c_str());
		
		strTempData = "Open ";
		strTempData += IP;
		strTempData += ',';
		strTempData += Str_IntToString(port);
		PrintExecute(tBICPAR, strTempData);
		
		if (tBICPAR->cgDevice->Open(DEVICE::OP_UDPClient, IP, port, tBICPAR->cgDevice->cEDevFlag.blEnableEcho) != 0){
			*ret = 'T';
		}
		else if (tBICPAR->cgCommand != nullptr){
			tBICPAR->cgCommand->StrCatch = "Fail";
			++tBICPAR->cgCommand->catchTimes;
		}
		tBICPAR->cgFunRet = *ret;
	}
	
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_CloseUDPSocket::Help(SBICPAR *tBICPAR, int32 blDetail)const{
	PrintB(tBICPAR, ".CMD = CloseUDPSocket/CUDP -->Close socket.");
	PrintB(tBICPAR, "  Command = <'CloseUDPSocket>[//COMMENT]");
	PrintP(tBICPAR, "   eg:");
	PrintP(tBICPAR, "     Command = 'CloseUDPSocket  //Close socket");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_CloseUDPSocket::Command(SBICPAR *tBICPAR, const std::string &par, std::string *ret)const{
	std::string	strTempData;
	
	*ret = 'F';
	if ((tBICPAR != nullptr) && (tBICPAR->cgDevice != nullptr) && (tBICPAR->cgDevice->cgDevType == DEVICE::DEVID_UDPClient)){
		tBICPAR->blRSC = 1;
		strTempData = "Close UDP Socket";
		PrintExecute(tBICPAR,strTempData);
		if (tBICPAR->cgDevice->cgAPISocket->CloseD(1) != 0){
			strTempData = "Close UDP Socket, success";
			*ret = 'T';
			tBICPAR->blRSC = 1;
		}
		else{
			strTempData = "Close UDP Socket, fail";
			if (tBICPAR->cgCommand != nullptr){
				tBICPAR->cgCommand->StrCatch = "Fail";
				++tBICPAR->cgCommand->catchTimes;
			}
		}
		PrintExecute(tBICPAR, strTempData);
		tBICPAR->cgFunRet = *ret;
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_SetRecvDataMode::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = SetRecvDataMode=<HEX|ASCII> -->Change receive data print mode, HEE or ASCII.");
	PrintB(tBICPAR,"  Command = <'SetRecvDataMode=<HEX|ASCII>>[//COMMENT]");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"     Command = 'SetRecvDataMode=HEX    //change to HEX mode");
	PrintP(tBICPAR,"     Command = 'SetRecvDataMode=ASCII  //change to ASCII mode");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_SetRecvDataMode::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	std::string		strTempData,strCMDP1;
	
	*ret = "";
	if ((tBICPAR != nullptr) && (tBICPAR->cgDevice != nullptr)){
		if (Str_UpperCase(Str_Trim(par)) == "HEX"){
			B_SetFLAG64(tBICPAR->cgDevice->cgODevList.cgODevFlagU64, ODEV_FLAG_EnHEXViewMode);
		}else if (Str_UpperCase(Str_Trim(par)) == "ASCII"){
			B_ClrFLAG64(tBICPAR->cgDevice->cgODevList.cgODevFlagU64, ODEV_FLAG_EnHEXViewMode);
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_SetRecMsgReport::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = SetRecMsgReport=<Enable|Disable> -->Enable/Disable receive report.");
	PrintB(tBICPAR,"  Command = <'SetRecMsgReport=<Enable|Disable>>[//COMMENT]");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"     Command = 'SetRecMsgReport=Enable    //enable report");
	PrintP(tBICPAR,"     Command = 'SetRecMsgReport=Disable   //disable report");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_SetRecMsgReport::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	std::string		strTempData,strCMDP1;
	*ret = "";
	
	if ((tBICPAR != nullptr) && (tBICPAR->cgDevice != nullptr)){
		if (Str_UpperCase(Str_Trim(par)) == "ENABLE"){
			B_SetFLAG64(tBICPAR->cgDevice->cgODevList.cgODevFlagU64, ODEV_FLAG_EnRecMsg);
		}else if (Str_UpperCase(Str_Trim(par)) == "DISABLE"){
			B_ClrFLAG64(tBICPAR->cgDevice->cgODevList.cgODevFlagU64, ODEV_FLAG_EnRecMsg);
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_Synchronous::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = Synchronous=<PAR1>,<PAR2> -->Time synchronization, PAR1 is offset of zone second, PAR2 is synchronization interval, uint is second.");
	PrintB(tBICPAR,"  Command = <'Synchronous=<PAR1>,<PAR2>>[//COMMENT]");
	PrintP(tBICPAR,"  Notes:");
	PrintP(tBICPAR,"     1.If this is the first implementation, T0=TIME(HOUR(Tnow),MINUTE(Tnow),0)+PAR1.");
	PrintP(tBICPAR,"        eg:Time is 10:12:45, record T0=10:12:00 + PAR1.");
	PrintP(tBICPAR,"     2.According to PAR2 to calculate the execution time of next \"Command\". Tnext=T0+INT((Tnow-T0)/Timeout(ms)+1)*Timeout(ms).");
	PrintP(tBICPAR,"     3.If this is not the first time implementation, T0 has no changed, re-calculate Tnext.");
	PrintP(tBICPAR,"   eg:Time is 10:12:14");
	PrintP(tBICPAR,"     Command = 'Synchronous=0,10      //T0=10:12:00, the execution time of next \"Command\" is 10:12:20, 10:12:30, 10:12:40,...");
	PrintP(tBICPAR,"     Command = 'Synchronous=5,10      //T0=10:12:05, the execution time of next \"Command\" is 10:12:15, 10:12:25, 10:12:35,...");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_Synchronous::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	double 			T1,T2,synchronousDelay;
	SYS_DateTime	currentTime,dT;
	int32			hour, min, sec, msec;
	uint64			msecT2,dTime;
	
	std::string		strTempData,strTime;
	std::string		strPar1,strPar2;
	
	*ret = "";
	if ((tBICPAR != nullptr) && (tBICPAR->cgCommand != nullptr)){
		strPar2 = par;
		strPar1 = Str_LTrim(Str_ReadSubItem(&strPar2,","));
		strPar2 = Str_RTrim(strPar2);
		
		T1 = atof(strPar1.c_str());
		T2 = atof(strPar2.c_str());
		
		currentTime.Now();
		if (tBICPAR->cgCommand->blFirstSynchronous == 0){
			tBICPAR->cgCommand->blFirstSynchronous = 1;
			currentTime.DecodeTime(&hour,&min,&sec,&msec);
			SYS_EncodeTimeABS(&dT,0,0,sec,msec);
			tBICPAR->cgCommand->timeST0 = currentTime - dT;
			msec = (int32)(T1 * 1000) % 1000;
			SYS_EncodeTimeABS(&dT,0,0,(int32)T1,msec);
			tBICPAR->cgCommand->timeST0 += dT;
			tBICPAR->cgCommand->timeST0 -= 360000;
		}
		dT = currentTime - tBICPAR->cgCommand->timeST0;
		
		dTime = (uint64)(dT.GetSec() * 1000 * 1000);
		msecT2 = (uint64)(T2 * 1000 * 1000);
		dTime %= msecT2;
		synchronousDelay = (double)(msecT2 - dTime) / 1000;
		
		if ((tBICPAR->cgDevice != nullptr) && (tBICPAR->cgDevice->cEDevFlag.blEnablePrintSBICinfo != 0)){
			if (tBICPAR->cgDevice->cgODevList.cgOutput != nullptr){
				tBICPAR->cgDevice->cgODevList.cgOutput->Spin_InUse_set();
				tBICPAR->cgDevice->cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon);
				tBICPAR->cgDevice->cgODevList.cgOutput->WriteStrN(SYS_MakeTime(currentTime) + " Synchronous from ",RICH_CF_clMaroon);
				tBICPAR->cgDevice->cgODevList.cgOutput->WriteStr(tBICPAR->cgCommand->timeST0.FormatDateTime(DEFAULT_TIMEFORMATE),RICH_CF_clRed);
				tBICPAR->cgDevice->cgODevList.cgOutput->WriteStr(" per ",RICH_CF_clMaroon);
				tBICPAR->cgDevice->cgODevList.cgOutput->WriteStr(Str_FloatToString(T2) + "s",RICH_CF_clBlue);
				tBICPAR->cgDevice->cgODevList.cgOutput->WriteStr(". \r\n Next command will be send after ",RICH_CF_clMaroon);
				tBICPAR->cgDevice->cgODevList.cgOutput->WriteStr(Str_FloatToString(synchronousDelay / 1000) + "s\r\n",RICH_CF_clRed);
				tBICPAR->cgDevice->cgODevList.cgOutput->Spin_InUse_clr();
			}
		}
		SYS_DelayMS(synchronousDelay,&tBICPAR->blExit);
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
