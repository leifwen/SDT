/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: BIC_B.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "BIC_B.h"
#include "Comm_Convert.h"
#include "AppLayer.h"
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Unix
#include <fcntl.h>
#include <signal.h>
#include <termios.h>
#endif
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_Node::ExecLC(BICPAR *tBICPAR,const std::string &fullCMD,std::string *ret)const{
	int32	retCode;
	
	retCode = BI_RETCODE_NO;
	
	RTREE_LChildRChain_Traversal_LINE_nolock(BIC_Node,const_cast<BIC_Node *>(this),
		retCode = operateNode_t->Execute(tBICPAR,fullCMD,ret);
		if (retCode != BI_RETCODE_NO)
			break;
	);
	return(retCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_Node::ExecLC_T(BICPAR *tBICPAR, const std::string &exTitle,std::string *ret)const{
	std::string		strCommand,strRet;
	int32			retCode;
	
	PrintStrN(tBICPAR,cgPrintTitle + exTitle + ">",RICH_LIN_clDefault);
	
	if (BI_ReadCommand(tBICPAR) == 0)
		return(BI_RETCODE_NO);
	
	strCommand = Str_Trim(Str_ReadSubItem(&tBICPAR->retCommand,"\r"));
	tBICPAR->retCommand = "";
	retCode = ExecLC(tBICPAR,strCommand,&strRet);
	
	return(retCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_Node::ExecuteLC(BICPAR *tBICPAR,const std::string &par,const std::string &exTitle,std::string *ret)const{
	std::string		strCommand,strRet;
	int32			retCode;
	
	retCode = cgReturnCode;
	if (par.length() > 0){
		retCode = ExecLC(tBICPAR, par, ret);
	}
	else{
		//cgBICList.Help(tBICPAR,0);
		while(tBICPAR->blExit != -1){
			ClrblExit(tBICPAR);
			
			retCode = ExecLC_T(tBICPAR,exTitle,ret);
			if ((retCode == BI_RETCODE_RETURN) || (retCode == BI_RETCODE_EXIT))
				break;
		}
	}
	return(retCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_Node::HelpLC(BICPAR *tBICPAR,int32 blDetail,int32 blPrintSubTitle)const{
	if ((GetcgLChild(this) != nullptr) && (blPrintSubTitle != 0))
		PrintStrN(tBICPAR," Subcommand explain:\n",RICH_LIN_clBrown);
	RTREE_LChildRChain_Traversal_LINE_nolock(BIC_Node,const_cast<BIC_Node *>(this),operateNode_t->Help(tBICPAR,blDetail););
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_Node::Check(BICPAR *tBICPAR,const std::string &fullCMD,std::string *retPar)const{
	std::string		oCommand,oFullCMD,tmpCMD,strT;
	std::string		subCommand;
	
	oFullCMD = Str_LTrim(fullCMD);
	oCommand = cgCommand;
	
	do{
		tmpCMD = oFullCMD;
		subCommand = Str_ReadSubItem(&oCommand,"/");
		strT = Str_RTrim(Str_ReadSubItem(&tmpCMD," "));
		if (strT == subCommand){
			*retPar = Str_Trim(tmpCMD);
			return(cgReturnCode);
		}
		subCommand += '?';
		if (strT == subCommand){
			*retPar = '?';
			return(cgReturnCode);
		}
		subCommand += '?';
		if (strT == subCommand){
			*retPar = "??";
			return(cgReturnCode);
		}
	}while (oCommand.length() > 0);
	*retPar = "";
	return(BI_RETCODE_NO);
}
//------------------------------------------------------------------------------------------//
int32 BIC_Node::Execute(BICPAR *tBICPAR,const std::string &fullCMD,std::string *ret)const{
	std::string	 retPar;
	
	if (Check(tBICPAR,fullCMD,&retPar) == cgReturnCode){
		if ((retPar == "?") || (retPar == "-help"))
			return(Help(tBICPAR));
		if ((retPar == "??") || (retPar == "-shelp"))
			return(HelpLC(tBICPAR,0));
		return(Command(tBICPAR,retPar,ret));
	}
	return(BI_RETCODE_NO);
}
//------------------------------------------------------------------------------------------//
int32 BIC_Node::Help(BICPAR *tBICPAR,int32 blDetail)const{
	
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_Node::Command(BICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	int32			retCode;
	
	*ret = "";
	retCode = ExecuteLC(tBICPAR,par,"",ret);
	if (retCode == BI_RETCODE_RETURN)
		return(cgReturnCode);
	return(retCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_Node::UpdateTitle(const std::string &fTitle){
	
	RTREE_LChildRChain_Traversal_LINE_nolock(BIC_Node,this,operateNode_t->SetSLTitle(fTitle);)
	return 1;
 }
//------------------------------------------------------------------------------------------//
void BIC_Node::SetSLTitle(const std::string &fTitle){
	cgPrintTitle = fTitle + "/" + cgTitle;
	UpdateTitle(cgPrintTitle);
}
//------------------------------------------------------------------------------------------//
uint8 BIC_Node::BI_ReadChar(BICPAR *tBICPAR,int32 blWait){
	uint8	charGet = 0;
	
	while(tBICPAR->blExit == 0){
		if (tBICPAR->charSBUF->Get(&charGet,1) > 0)
			break;
		charGet = 0;
		if (blWait == 0)
			break;
		SYS_SleepMS(10);
	}
	return(charGet);
}
//------------------------------------------------------------------------------------------//
int32 BIC_Node::BI_ReadCommand(BICPAR *tBICPAR){
	uint8	charGet = 0;
	
	while(tBICPAR->blExit == 0){
		charGet = BI_ReadChar(tBICPAR);
		if ((charGet == '\r') || (charGet == '\n')){
			tBICPAR->retCommand += '\r';
			charGet = BI_ReadChar(tBICPAR,0);
			if (charGet > 0){
				if ((charGet == '\r') || (charGet == '\n'))
					return 1;
				tBICPAR->retCommand += charGet;
			}
			return 1;
		}
		tBICPAR->retCommand += charGet;
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
uint8 BIC_Node::PrintPressAnyKey(BICPAR *tBICPAR){
	uint8	retChar = 0;
	if (tBICPAR->blExit == 0){
		tBICPAR->blInPressKeyMode = 1;
		PrintStrN(tBICPAR,"press any key to continue.\n",RICH_LIN_clBrown);
		retChar = BI_ReadChar(tBICPAR);
	}
	tBICPAR->blInPressKeyMode = 0;
	return(retChar);
}
//------------------------------------------------------------------------------------------//
uint8 BIC_Node::PressAnyKey(BICPAR *tBICPAR){
	uint8	retChar;
	retChar = 0;
	while(tBICPAR->blExit == 0){
		tBICPAR->blInPressKeyMode = 1;
		retChar = BI_ReadChar(tBICPAR,0);
		if (retChar > 0)
			break;
		SYS_SleepMS(10);
		if (tBICPAR->sdtApp->m_Device.CheckblConnect() == 0)
			break;
	}
	tBICPAR->blInPressKeyMode = 0;
	return(retChar);
}
//------------------------------------------------------------------------------------------//
void BIC_Node::PrintTitle(BICPAR *tBICPAR,DEVICE::DEVID_TYPE tDevType,int32 blPrintTail){
	PrintStrN(tBICPAR,"",RICH_LIN_clDefault);
	if (tDevType == DEVICE::DEVID_NONE){
		PrintStr(tBICPAR,SWVERSION_SHORTNAME,RICH_LIN_clDefault);
		if (blPrintTail != 0)
			PrintStr(tBICPAR,">",RICH_LIN_clDefault);
		return;
	}
	
	switch(tDevType){
		case DEVICE::DEVID_TCPClient:
			PrintStr(tBICPAR,"TCP",RICH_LIN_clLightBlue);
			PrintStr(tBICPAR,":",RICH_LIN_clDefault);
			PrintStr(tBICPAR,tBICPAR->sdtApp->m_Device.cEDevFlag.tcpIP,RICH_LIN_clBrown);
			PrintStr(tBICPAR,"@",RICH_LIN_clDefault);
			PrintStr(tBICPAR,Str_IntToString(tBICPAR->sdtApp->m_Device.cEDevFlag.tcpPort),RICH_LIN_clBrown);
			break;
		case DEVICE::DEVID_UDPClient:
			PrintStr(tBICPAR,"UDP",RICH_LIN_clLightBlue);
			PrintStr(tBICPAR,":",RICH_LIN_clDefault);
			PrintStr(tBICPAR,tBICPAR->sdtApp->m_Device.cEDevFlag.udpIP,RICH_LIN_clBrown);
			PrintStr(tBICPAR,"@",RICH_LIN_clDefault);
			PrintStr(tBICPAR,Str_IntToString(tBICPAR->sdtApp->m_Device.cEDevFlag.udpPort),RICH_LIN_clBrown);
			break;
		case DEVICE::DEVID_TCPServer:
			PrintStr(tBICPAR,"TCP Server",RICH_LIN_clLightBlue);
			PrintStr(tBICPAR,"@",RICH_LIN_clDefault);
			PrintStr(tBICPAR,Str_IntToString(tBICPAR->sdtApp->m_Device.cEDevFlag.tcpsPort),RICH_LIN_clBrown);
			break;
		case DEVICE::DEVID_UDPServer:
			PrintStr(tBICPAR,"UDP Server",RICH_LIN_clLightBlue);
			PrintStr(tBICPAR,"@",RICH_LIN_clDefault);
			PrintStr(tBICPAR,Str_IntToString(tBICPAR->sdtApp->m_Device.cEDevFlag.udpsPort),RICH_LIN_clBrown);
			break;
		case DEVICE::DEVID_APICOM:
			PrintStr(tBICPAR,"COM",RICH_LIN_clLightBlue);
			PrintStr(tBICPAR,":",RICH_LIN_clDefault);
			PrintStr(tBICPAR,tBICPAR->sdtApp->m_Device.cEDevFlag.com,RICH_LIN_clBrown);
			PrintStr(tBICPAR,"@",RICH_LIN_clDefault);
			PrintStr(tBICPAR,Str_IntToString(tBICPAR->sdtApp->m_Device.cEDevFlag.baudrate),RICH_LIN_clBrown);
			break;
		default:
			break;
	}
	if (blPrintTail != 0)
		PrintStr(tBICPAR,">",RICH_LIN_clDefault);
	return;
}
//------------------------------------------------------------------------------------------//
void BIC_Node::PrintHelpItem(BICPAR *tBICPAR,const std::string &command,const std::string &desp){
	std::string		newCommand;
	newCommand = ' ';
	newCommand += command;
	if (command.length() < 24)
		newCommand.insert(newCommand.length(),24 - command.length(),' ');
	tBICPAR->oDevNode->Spin_InUse_set();
	tBICPAR->oDevNode->WriteToStr(newCommand,RICH_LIN_clLightBlue,COLSTRING::COL_EP_YES,G_LOCK_OFF);
	tBICPAR->oDevNode->WriteToStr(desp + "\n",RICH_LIN_clDefault,COLSTRING::COL_EP_YES,G_LOCK_OFF);
	tBICPAR->oDevNode->Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_Node::BI_SET_ConnectPar(BICPAR *tBICPAR,const std::string &par,DEVICE::DEVID_TYPE tDevType){
	std::string		strPar1,strPar2;
	
	if (tBICPAR->sdtApp->m_Device.CheckblConnect() == 0){
		tBICPAR->sdtApp->m_Device.cgDevType = tDevType;
		if(par.length() > 0){
			strPar2 = Str_Trim(par);
			strPar1 = Str_ReadSubItem(&strPar2," ");
			Str_LTrimSelf(strPar2);
			switch(tDevType){
				case DEVICE::DEVID_TCPClient:
					tBICPAR->sdtApp->m_Device.cEDevFlag.tcpIP = strPar1;
					BI_SET_ConnectPar2(tBICPAR,strPar2);
					break;
				case DEVICE::DEVID_UDPClient:
					tBICPAR->sdtApp->m_Device.cEDevFlag.udpIP = strPar1;
					BI_SET_ConnectPar2(tBICPAR,strPar2);
					break;
				case DEVICE::DEVID_TCPServer:
				case DEVICE::DEVID_UDPServer:
					BI_SET_ConnectPar2(tBICPAR,par);
					break;
				case DEVICE::DEVID_APICOM:
				default:
					tBICPAR->sdtApp->m_Device.cEDevFlag.com = strPar1;
					BI_SET_ConnectPar2(tBICPAR,strPar2);
					break;
			}
		}
		return 1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
int32 BIC_Node::BI_SET_ConnectPar2(BICPAR *tBICPAR,const std::string &par){
	int32 baud;
	
	switch(tBICPAR->sdtApp->m_Device.cgDevType){
		case DEVICE::DEVID_TCPClient:
			baud = tBICPAR->sdtApp->m_Device.cEDevFlag.tcpPort;
			break;
		case DEVICE::DEVID_UDPClient:
			baud = tBICPAR->sdtApp->m_Device.cEDevFlag.udpPort;
			break;
		case DEVICE::DEVID_TCPServer:
			baud = tBICPAR->sdtApp->m_Device.cEDevFlag.tcpsPort;
			break;
		case DEVICE::DEVID_UDPServer:
			baud = tBICPAR->sdtApp->m_Device.cEDevFlag.udpsPort;
			break;
		case DEVICE::DEVID_APICOM:
		default:
			baud = tBICPAR->sdtApp->m_Device.cEDevFlag.baudrate;
			break;
	}
	
	if (par.length() > 0){
		baud = atoi(par.c_str());
		if (baud < 0)
			baud = 0;
	}
	if (tBICPAR->sdtApp->m_Device.CheckblConnect() == 0){
		switch(tBICPAR->sdtApp->m_Device.cgDevType){
			case DEVICE::DEVID_TCPClient:
				tBICPAR->sdtApp->m_Device.cEDevFlag.tcpPort = baud;
				break;
			case DEVICE::DEVID_UDPClient:
				tBICPAR->sdtApp->m_Device.cEDevFlag.udpPort = baud;
				break;
			case DEVICE::DEVID_TCPServer:
				tBICPAR->sdtApp->m_Device.cEDevFlag.tcpsPort = baud;
				break;
			case DEVICE::DEVID_UDPServer:
				tBICPAR->sdtApp->m_Device.cEDevFlag.udpsPort = baud;
				break;
			case DEVICE::DEVID_APICOM:
			default:
				tBICPAR->sdtApp->m_Device.cEDevFlag.baudrate = baud;
				break;
		}
		return 1;
	}
	else if (tBICPAR->sdtApp->m_Device.cgDevType == DEVICE::DEVID_APICOM){
		tBICPAR->sdtApp->m_Device.cgAPIECom->SetBaudrate(baud);
		return 1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_HELP::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Help information.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [-d]"			,"List command detail.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_HELP::Command(BICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	int32			detail;
	BIC_Node		*father;
	
	*ret = "";
	detail = 0;
	
	if (par == "-d")
		detail = 1;
	
	PrintStrN(tBICPAR," Build in command explain:\n",RICH_LIN_clBrown);
	father = (BIC_Node*)GetFather_nolock(const_cast<BIC_HELP *>(this));
	
	if (father != nullptr)
		father->HelpLC(tBICPAR,detail,0);

	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_EXIT::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Exit SDT.");
	return(BI_RETCODE_EXIT_HELP);
}
//------------------------------------------------------------------------------------------//
int32 BIC_EXIT::Command(BICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	*ret = "";
#ifdef CommonDefH_VC
	return(BI_RETCODE_NO);
#endif
#ifdef CommonDefH_Unix
	tBICPAR->blExit = cgReturnCode;
	PrintStrN(tBICPAR,"",RICH_LIN_clDefault);
	return(cgReturnCode);
#endif
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_EXEC::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Execute console command.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_EXEC::Command(BICPAR *tBICPAR,const std::string &par,std::string *ret)const{
#ifdef CommonDefH_Unix
	int			status;
	pid_t		childpid,childpidRet;
	std::string	printData;
	char		chartBuf[1024];
	int			fd_pipeChildOut[2],fd_pipeChildIn[2];
	int			oldf;
	struct termios oldt, newt;
	
	if (par.length() == 0)
		return(cgReturnCode);
	
	status = pipe(fd_pipeChildOut);	//child SDOUT
	status = pipe(fd_pipeChildIn);	//child SDIN
	
	tcgetattr(fd_pipeChildOut[0], &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(fd_pipeChildOut[0], TCSANOW, &newt);
	oldf = fcntl(fd_pipeChildOut[0], F_GETFL, 0);
	fcntl(fd_pipeChildOut[0], F_SETFL, oldf | O_NONBLOCK);
	
	waitpid(-1,nullptr,WNOHANG);
	
	childpid = fork();
	if (childpid == 0){				//enter to child process.
		dup2(fd_pipeChildIn[0],0);	//redirect standard input to read pipe.
		dup2(fd_pipeChildOut[1],1);	//redirect write pipe to standard output.
		
		close(fd_pipeChildIn[0]);	//close read pipe.
		close(fd_pipeChildIn[1]);	//close write pipe.
		close(fd_pipeChildOut[0]);	//close read pipe.
		close(fd_pipeChildOut[1]);	//close write pipe.
		status = ExecuteConsoleCommand(par);
		exit(0);
	}
	else if (childpid > 0){			//enter to fater process.
		close(fd_pipeChildIn[0]);
		close(fd_pipeChildOut[1]);
		do{
			childpidRet = waitpid(childpid,&status,WNOHANG);
			do{
				status = (int32)read(fd_pipeChildOut[0],chartBuf,1024);//read data from child output pipe.
				if (status > 0){
					printData = Str_CharToASCIIStr((uint8*)chartBuf, status, G_ESCAPE_OFF);
					PrintStr(tBICPAR,printData,RICH_LIN_clCyan);
				}
				SYS_SleepMS(0);
			}while((status > 0) && (tBICPAR->blExit == 0));
			
			tBICPAR->blInPressKeyMode = 1;
			if (BI_ReadChar(tBICPAR,0) == 27){
				PrintStrN(tBICPAR,"Pressed ESC, stop child process\r\n",RICH_LIN_clRed);
				kill(childpid,SIGINT);
			}
		}while((tBICPAR->blExit == 0) && (childpid != childpidRet) && (childpidRet != -1));
		tBICPAR->blInPressKeyMode = 0;
		close(fd_pipeChildIn[1]);
		close(fd_pipeChildOut[0]);
		waitpid(childpid,&status,0);
		waitpid(-1,nullptr,WNOHANG);
	}
#endif
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_EXE_BASH::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Execute console command with using bash(default).");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_EXE_BASH::Command(BICPAR *tBICPAR,const std::string &par,std::string *ret)const{
#ifdef CommonDefH_Unix
	int			status;
	pid_t		childpid,childpidRet;
	std::string	printData;
	char		chartBuf[1024];
	int			fd_pipeChildOut[2],fd_pipeChildIn[2];
	int			oldf;
	struct termios oldt, newt;
	
	if (par.length() == 0)
		return(cgReturnCode);
	
	status = pipe(fd_pipeChildOut);	//child SDOUT
	status = pipe(fd_pipeChildIn);	//child SDIN
	
	tcgetattr(fd_pipeChildOut[0], &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(fd_pipeChildOut[0], TCSANOW, &newt);
	oldf = fcntl(fd_pipeChildOut[0], F_GETFL, 0);
	fcntl(fd_pipeChildOut[0], F_SETFL, oldf | O_NONBLOCK);
	
	waitpid(-1,nullptr,WNOHANG);
	
	childpid = fork();
	if (childpid == 0){				//enter to child process.
		dup2(fd_pipeChildIn[0],0);	//redirect standard input to read pipe.
		dup2(fd_pipeChildOut[1],1);	//redirect write pipe to standard output.
		
		close(fd_pipeChildIn[0]);	//close read pipe.
		close(fd_pipeChildIn[1]);	//close write pipe.
		close(fd_pipeChildOut[0]);	//close read pipe.
		close(fd_pipeChildOut[1]);	//close write pipe.
		status = execlp("/bin/bash","/bin/bash","-c",par.c_str(),(char*)nullptr);
		exit(0);
	}
	else if (childpid > 0){			//enter to fater process.
		close(fd_pipeChildIn[0]);
		close(fd_pipeChildOut[1]);
		do{
			childpidRet = waitpid(childpid,&status,WNOHANG);
			do{
				status = (int32)read(fd_pipeChildOut[0],chartBuf,1024);//read data from child output pipe.
				if (status > 0){
					printData = Str_CharToASCIIStr((uint8*)chartBuf, status, G_ESCAPE_OFF);
					PrintStr(tBICPAR,printData,RICH_LIN_clCyan);
				}
				SYS_SleepMS(0);
			}while((status > 0) && (tBICPAR->blExit == 0));
			
			tBICPAR->blInPressKeyMode = 1;
			if (BI_ReadChar(tBICPAR,0) == 27){
				PrintStrN(tBICPAR,"Pressed ESC, stop child process\r\n",RICH_LIN_clRed);
				kill(childpid,SIGINT);
			}
		}while((tBICPAR->blExit == 0) && (childpid != childpidRet) && (childpidRet != -1));
		tBICPAR->blInPressKeyMode = 0;
		close(fd_pipeChildIn[1]);
		close(fd_pipeChildOut[0]);
		waitpid(childpid,&status,0);
		waitpid(-1,nullptr,WNOHANG);
	}
#endif
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_EXE_SH::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Execute console command with using sh.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_EXE_SH::Command(BICPAR *tBICPAR,const std::string &par,std::string *ret)const{
#ifdef CommonDefH_Unix
	int			status;
	pid_t		childpid,childpidRet;
	std::string	printData;
	char		chartBuf[1024];
	int			fd_pipeChildOut[2],fd_pipeChildIn[2];
	int			oldf;
	struct termios oldt, newt;
	
	if (par.length() == 0)
		return(cgReturnCode);
	
	status = pipe(fd_pipeChildOut);	//child SDOUT
	status = pipe(fd_pipeChildIn);	//child SDIN
	
	tcgetattr(fd_pipeChildOut[0], &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(fd_pipeChildOut[0], TCSANOW, &newt);
	oldf = fcntl(fd_pipeChildOut[0], F_GETFL, 0);
	fcntl(fd_pipeChildOut[0], F_SETFL, oldf | O_NONBLOCK);
	
	waitpid(-1,nullptr,WNOHANG);
	
	childpid = fork();
	if (childpid == 0){				//enter to child process.
		dup2(fd_pipeChildIn[0],0);	//redirect standard input to read pipe.
		dup2(fd_pipeChildOut[1],1);	//redirect write pipe to standard output.
		
		close(fd_pipeChildIn[0]);	//close read pipe.
		close(fd_pipeChildIn[1]);	//close write pipe.
		close(fd_pipeChildOut[0]);	//close read pipe.
		close(fd_pipeChildOut[1]);	//close write pipe.
		status = execlp("/bin/sh","/bin/sh","-c",par.c_str(),(char*)nullptr);
		exit(0);
	}
	else if (childpid > 0){			//enter to fater process.
		close(fd_pipeChildIn[0]);
		close(fd_pipeChildOut[1]);
		do{
			childpidRet = waitpid(childpid,&status,WNOHANG);
			do{
				status = (int32)read(fd_pipeChildOut[0],chartBuf,1024);//read data from child output pipe.
				if (status > 0){
					printData = Str_CharToASCIIStr((uint8*)chartBuf, status, G_ESCAPE_OFF);
					PrintStr(tBICPAR,printData,RICH_LIN_clCyan);
				}
				SYS_SleepMS(0);
			}while((status > 0) && (tBICPAR->blExit == 0));
			
			tBICPAR->blInPressKeyMode = 1;
			if (BI_ReadChar(tBICPAR,0) == 27){
				PrintStrN(tBICPAR,"Pressed ESC, stop child process\r\n",RICH_LIN_clRed);
				kill(childpid,SIGINT);
			}
		}while((tBICPAR->blExit == 0) && (childpid != childpidRet) && (childpidRet != -1));
		tBICPAR->blInPressKeyMode = 0;
		close(fd_pipeChildIn[1]);
		close(fd_pipeChildOut[0]);
		waitpid(childpid,&status,0);
		waitpid(-1,nullptr,WNOHANG);
	}
#endif
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_RETURN::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Return to the upper menu.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_RETURN::Command(BICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	*ret = "";
	
	PrintStrN(tBICPAR,"",RICH_LIN_clDefault);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//