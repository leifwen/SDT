/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: SBIC_RunExternal.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.8.15
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "SBIC_RunExternal.h"
#include "SYS_Time.h"
#include "Comm_Convert.h"
#include "Device.h"
#include "CommandGList.h"
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Unix
#include <fcntl.h>
#include <signal.h>
#include <termios.h>
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_RE_Node::Run(const std::string &lable,const std::string &cmd){
	int32			oldf;
	struct termios	oldt, newt;
	
	cgLable = lable;
	cgCommand = cmd;
	pipe(fd_pipeChildOut);	//child SDOUT
	pipe(fd_pipeChildIn);	//child SDIN
	
	tcgetattr(fd_pipeChildOut[0], &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(fd_pipeChildOut[0], TCSANOW, &newt);
	oldf = fcntl(fd_pipeChildOut[0], F_GETFL, 0);
	fcntl(fd_pipeChildOut[0], F_SETFL, oldf | O_NONBLOCK);
	
	childpid = fork();
	if (childpid == 0){				//enter to child process.
		dup2(fd_pipeChildIn[0],0);	//redirect standard input to read pipe.
		dup2(fd_pipeChildOut[1],1);	//redirect write pipe to standard output.
		
		close(fd_pipeChildIn[0]);	//close read pipe.
		close(fd_pipeChildIn[1]);	//close write pipe.
		close(fd_pipeChildOut[0]);	//close read pipe.
		close(fd_pipeChildOut[1]);	//close write pipe.
		ExecuteConsoleCommand(cgCommand);
		exit(0);
	}
	else if (childpid > 0){			//enter to fater process.
		close(fd_pipeChildIn[0]);
		close(fd_pipeChildOut[1]);
		return 1;
	}
	cgCommand = "";
	close(fd_pipeChildIn[0]);	//close read pipe.
	close(fd_pipeChildIn[1]);	//close write pipe.
	close(fd_pipeChildOut[0]);	//close read pipe.
	close(fd_pipeChildOut[1]);	//close write pipe.
	return 0;
}
//------------------------------------------------------------------------------------------//
int32 SBIC_RE_Node::Kill(void)const{
	if (childpid > 0)
		return(kill(childpid,SIGINT));
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 SBIC_RE_Node::ClosePIPE(void)const{
	close(fd_pipeChildIn[1]);
	close(fd_pipeChildOut[0]);
	return 1;
}
//------------------------------------------------------------------------------------------//




//------------------------------------------------------------------------------------------//
SBIC_RE_LIST::SBIC_RE_LIST(void) : RTREE_NODE(){
	cgDevice = nullptr;
	eECPThread.ThreadInit(this, &SBIC_RE_LIST::ECPThreadFun);
}
//------------------------------------------------------------------------------------------//
SBIC_RE_LIST::~SBIC_RE_LIST(void){
	eECPThread.ThreadStop();
	Stop();
}
//------------------------------------------------------------------------------------------//
void SBIC_RE_LIST::WriteToPrintBuffer(SBIC_RE_Node *tNode,uint8 *buffer,int32 size){
	int32	num;
	uint8	charData;
	
	do{
		num = tNode->GetPIPEData(buffer,size);
		if (num > 0)
			cgPrintBuffer.Put(buffer,num);
	}while(num > 0);
	charData = '\n';
	cgPrintBuffer.Put(&charData,1);
}
//------------------------------------------------------------------------------------------//
void SBIC_RE_LIST::PrintBuffer(void){
	int32			num;
	std::string		printData;
	SYS_DateTime	dT;
	
	printData = "";
	while(cgPrintBuffer.IsEmpty() == 0){
		dT.Now();
		num = cgPrintBuffer.GetInLine(&printData);
		if ((num > 0) && (printData != "\n") && (printData != "\r") && (cgDevice != nullptr)){
			if (printData[printData.length() - 1] != '\n')
				printData += '\n';
			if (cgDevice->cgODevList.cgOutput != nullptr){
				cgDevice->cgODevList.cgOutput->Spin_InUse_set();
				cgDevice->cgODevList.cgOutput->WriteStrN(dT.FormatDateTime(DEFAULT_TIMEFORMATE) + " ", RICH_LIN_clRed);
				cgDevice->cgODevList.cgOutput->WriteStr(printData, RICH_LIN_clDefault);
				cgDevice->cgODevList.cgOutput->Spin_InUse_clr();
			}
		}
	};
}
//------------------------------------------------------------------------------------------//
std::string	SBIC_RE_LIST::PrintBuffer(SBIC_RE_Node *tNode,uint8 *buffer,int32 size){
	int32			num;
	std::string		printData,recData,srtRet;
	SYS_DateTime	dT;
	
	recData = "";
	do{
		num = tNode->GetPIPEData(buffer,size);
		if (num > 0)
			recData += Str_CharToASCIIStr(buffer, num, G_ESCAPE_OFF);
	}while(num > 0);
	
	srtRet = recData;
	
	printData = "";
	while(recData.length() > 0){
		dT.Now();
		printData = Str_ReadSubItem(&recData, "\n");
		if ((printData.length() > 0) && (cgDevice != nullptr)){
			printData += '\n';
			if (cgDevice->cgODevList.cgOutput != nullptr){
				cgDevice->cgODevList.cgOutput->Spin_InUse_set();
				cgDevice->cgODevList.cgOutput->WriteStr(dT.FormatDateTime(DEFAULT_TIMEFORMATE) + " ", RICH_LIN_clRed);
				cgDevice->cgODevList.cgOutput->WriteStr(printData, RICH_LIN_clDefault);
				cgDevice->cgODevList.cgOutput->Spin_InUse_clr();
			}
		}
	};
	return(srtRet);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_RE_LIST::ECPThreadFun(void){
	SBIC_RE_Node	*delNode,*fromNode;
	int				childPID,status,blupdate;
	uint8			buffer[1024 * 4];
	
	while(eECPThread.IsTerminated() == 0){
		waitpid(-1,nullptr,WNOHANG);
		Spin_InUse_set();
		
		fromNode = (SBIC_RE_Node*)GetcgLChild(this);
		blupdate = 0;
		do{
			delNode = nullptr;
			RTREE_RChain_Traversal_LINE_nolock(SBIC_RE_Node,fromNode,
				if (operateNode_t->CheckblKilled() != 0)
					operateNode_t->Kill();
				childPID = operateNode_t->GetChildStatus(&status);
				WriteToPrintBuffer(operateNode_t,buffer,sizeof(buffer));
				if (childPID == operateNode_t->GetChildPID() || childPID == -1){
					delNode = operateNode_t;
					break;
				}
			);
			fromNode = nullptr;
			if (delNode != nullptr){
				fromNode = (SBIC_RE_Node*)GetcgRChild(delNode);
				delNode->ClosePIPE();
				waitpid(delNode->GetChildPID(),&status,0);
				waitpid(-1,nullptr,WNOHANG);
				RemoveNodesInRChain(delNode,nullptr,G_LOCK_OFF);
				InsertLChild(GetcgTrash(this), delNode);
				blupdate = 1;
			}
		}while(fromNode != nullptr);
		if (blupdate != 0)
			CleanTrash(this);
		Spin_InUse_clr();
		PrintBuffer();
		SYS_SleepMS(2);
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 SBIC_RE_LIST::Kill(const std::string &lable,const std::string &cmd,int32 timeout){
	SBIC_RE_Node	*delNode;
	int				childRet,status;
	SYS_TIME_S		timeS;
	uint8			buffer[1024 * 4];
	
	delNode = nullptr;
	Spin_InUse_set();
	RTREE_LChildRChain_Traversal_LINE_nolock(SBIC_RE_Node,this,
		if ((operateNode_t->CheckblKilled() == 0) && (operateNode_t->GetLable() == lable) && (operateNode_t->GetCommand() == cmd)){
			delNode = operateNode_t;
			RemoveNodesInRChain(delNode,nullptr,G_LOCK_OFF);
			break;
		}
	);
	Spin_InUse_clr();
	if (delNode != nullptr){
		SYS_Delay_SetTS(&timeS,timeout);
		delNode->Kill();
		do{
			childRet = delNode->GetChildStatus(&status);
			timeout = SYS_Delay_CheckTS(&timeS);
			PrintBuffer(delNode,buffer,sizeof(buffer));
			if (childRet == delNode->GetChildPID() || childRet == -1){//child is exit
				delNode->ClosePIPE();
				waitpid(delNode->GetChildPID(),&status,0);
				waitpid(-1,nullptr,WNOHANG);
				try{
					delete delNode;
				}
				catch(...){}
				return 1;
			}
		}while(timeout == 0);
		delNode->SetblKilled();
		AddNode(delNode);
		eECPThread.ThreadRun();
		return 0;
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
void SBIC_RE_LIST::Start(DEVICE *tDevice){
	Spin_InUse_set();
	cgDevice = tDevice;
	Spin_InUse_clr();
	eECPThread.ThreadRun();
}
//------------------------------------------------------------------------------------------//
void SBIC_RE_LIST::Stop(void){
	Spin_InUse_set();
	RTREE_LChildRChain_Traversal_LINE_nolock(SBIC_RE_Node,this,
		operateNode_t->SetblKilled();
		operateNode_t->Kill();
	);
	Spin_InUse_clr();
	eECPThread.ThreadStop();
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_RunExternal::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = RunExternal=<lable>,<timeout>,<command> -->Execute external command,only use in Linux.");
	PrintB(tBICPAR,"  Command = <'RunExternal=<lable>,<timeout>,<command>>[//COMMENT]");
	PrintP(tBICPAR,"  Notes:1.Can use 'lsc check the execution result.");
	PrintP(tBICPAR,"        2.The lable is used for distinguishing the same command.");
	PrintP(tBICPAR,"        3.Condition is used for checking whether matched EC output data.");
	PrintP(tBICPAR,"     eg:");
	PrintP(tBICPAR,"     Command = 'RunExternal= 001,10s,pppd file ./script");
	PrintP(tBICPAR,"     Condition = \"local  IP address\"");
	PrintP(tBICPAR,"     Execute pppd file ./script, and timeout is 10s.");
	PrintP(tBICPAR,"     if the command output contain \"local  IP address\", the execution result is true.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_RunExternal::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	std::string		strPar1,strPar3,strContinue,strPrintData;
	SBIC_RE_Node	*NewRENode;
	int32			timeout;
	SYS_TIME_S		timeS;
	uint8			buffer[1024 * 4];
	int				childPID,status;
	
	*ret = 'F';
	
	if ((tBICPAR != nullptr) && (tBICPAR->cgDevice != nullptr) && (tBICPAR->cgSBIC_RE_LIST != nullptr)){
		
		strPar3 = Str_Trim(par);
		strPar1 = Str_RTrim(Str_ReadSubItem(&strPar3,","));
		timeout = GetMS(Str_Trim(Str_ReadSubItem(&strPar3,",")));
		Str_TrimSelf(strPar3);
		strContinue = "";
		if (tBICPAR->cgCommand != nullptr)
			strContinue = tBICPAR->cgCommand->StrContinue;
		
		strPrintData = "Execute external command: \r\n";
		strPrintData += (" Command is " + strPar1 + ":" + strPar3);
		strPrintData += ("\r\n Timeout is " + GetMSSTR(timeout));
		if (strContinue.length() > 0)
			strPrintData += ("\r\n True condition is " + strContinue);
		PrintExecute(tBICPAR,strPrintData);
		
		SYS_Delay_SetTS(&timeS,timeout);
		
		if (strPar3.length() > 0){
			tBICPAR->cgSBIC_RE_LIST->Start(tBICPAR->cgDevice);
			NewRENode = new SBIC_RE_Node;
			if ((NewRENode != nullptr) && (NewRENode->Run(strPar1,strPar3) != 0)){
				tBICPAR->cgRecvbuf = "";
				do{
					timeout = SYS_Delay_CheckTS(&timeS);
					childPID = NewRENode->GetChildStatus(&status);
					strPrintData = tBICPAR->cgSBIC_RE_LIST->PrintBuffer(NewRENode,buffer,sizeof(buffer));
					tBICPAR->cgRecvbuf += Str_ASCIIToHEX(strPrintData, G_ESCAPE_OFF);
					if (cgSubC_Expression.Expression(tBICPAR,strContinue) != 0)
						*ret = 'T';
					if (childPID == NewRENode->GetChildPID() || childPID == -1){
						if (strContinue.length() == 0)
							*ret = 'T';
						break;
					}
				}while(tBICPAR->blExit == 0 && timeout == 0 && (*ret == "F"));
				tBICPAR->cgSBIC_RE_LIST->Spin_InUse_set();
				tBICPAR->cgSBIC_RE_LIST->AddNode(NewRENode);
				tBICPAR->cgSBIC_RE_LIST->Spin_InUse_clr();
				tBICPAR->cgSBIC_RE_LIST->Start(tBICPAR->cgDevice);
			}
		}
		if ((tBICPAR->cgCommand != nullptr) && (*ret == "F")){
			tBICPAR->cgCommand->StrCatch = "Fail";
			++ tBICPAR->cgCommand->catchTimes;
		}
		tBICPAR->cgFunRet = *ret;
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_StopExternal::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = StopExternal=<lable>,<timeout>,<command> -->Stop external command,only use in Linux.");
	PrintB(tBICPAR,"  Command = <'StopExternal=<lable>,<timeout>,<command>>[//COMMENT]");
	PrintP(tBICPAR,"  Notes:1.Can use 'lsc check the execution result.");
	PrintP(tBICPAR,"        2.The lable is uesd for distinguishing the same command.");
	PrintP(tBICPAR,"     eg:");
	PrintP(tBICPAR,"     Command = 'StopExternal=001,5s,pppd file ./script  // stop EC pppd file ./script which lable is 001");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_StopExternal::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	std::string		strPrintData,strPar1,strPar3;
	int32			timeout;
	*ret = "F";
	
	waitpid(-1,nullptr,WNOHANG);
	
	if ((tBICPAR != nullptr) && (tBICPAR->cgDevice != nullptr) && (tBICPAR->cgSBIC_RE_LIST != nullptr)){
		strPar3 = Str_Trim(par);
		strPar1 = Str_RTrim(Str_ReadSubItem(&strPar3,","));
		timeout = GetMS(Str_Trim(Str_ReadSubItem(&strPar3,",")));
		Str_TrimSelf(strPar3);
		
		strPrintData = "Stop external command: " + strPar1 + ":" + strPar3;
		PrintExecute(tBICPAR,strPrintData);
		if (tBICPAR->cgSBIC_RE_LIST->Kill(strPar1,strPar3,timeout) != 0)
			*ret = 'T';
		tBICPAR->cgFunRet = *ret;
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
#endif
