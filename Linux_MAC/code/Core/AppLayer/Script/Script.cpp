/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: Script.cpp
 * File ID	:
 * Remark	:
 * Writer   : Leif Wen
 * Date     : 2012.12.15
 * @2013.6.30 :	change to Liunx style
 use int32 to replace int32
 use int32 to replace int32
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Script.h"
#include "Comm_Convert.h"
#include "Device.h"
#include "SMC.h"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
Script::Script(void) : SBIC_Node(){
#ifdef	SWVERSION_SCRIPT
	cgBufMaxSize = BUF_MAX_SIZE;
	cgBuffer.Init(BUF_MAX_SIZE);
	
	cgATCondition = TELIT_GPRS_RESPONSE;
	eGroupListThread.ThreadInit(this, &Script::EGroupListThreadFun);
	eGroupThread.ThreadInit(this, &Script::EGroupThreadFun);
	eCommandThread.ThreadInit(this, &Script::ECommandThreadFun);
	
	AddNode(&cgSubC_Delay);
	AddNode(&cgSubC_Wait);
	AddNode(&cgSubC_DTR);
	AddNode(&cgSubC_RTS);
	AddNode(&cgSubC_ChangeCom);
	AddNode(&cgSubC_ChangeBR);
	AddNode(&cgSubC_OpenTCPSocket);
	AddNode(&cgSubC_CloseTCPSocket);
	AddNode(&cgSubC_OpenUDPSocket);
	AddNode(&cgSubC_CloseUDPSocket);
	AddNode(&cgSubC_SetRecvDataMode);
	AddNode(&cgSubC_SetRecMsgReport);
	AddNode(&cgSubC_PRINT);
	AddNode(&cgSubC_Synchronous);
	AddNode(&cgSubC_Lable);
	AddNode(&cgSubC_GOTO);
	AddNode(&cgSubC_STOP);
	AddNode(&cgSubC_BREAK);
#ifdef CommonDefH_Unix
	AddNode(&cgSubC_RunExternal);
	AddNode(&cgSubC_StopExternal);
#endif
	AddNode(&cgSubC_Search);
	StopRun();
	Init();
#endif
}
//------------------------------------------------------------------------------------------//
Script::~Script(void){
	StopRun();
}
//------------------------------------------------------------------------------------------//
int32 Script::Execute(const DEVICE *tDevice,const COMMAND_NODE *tCommand){
#ifdef	SWVERSION_SCRIPT
	SMC_EncryptI(0)
	if ((tDevice == nullptr) || (tCommand == nullptr))
		return 0;
	if (InDoing_try() == 0)
		return 0;
	SMC_EncryptS(0)
	StopRun();
	Init();
	Enable();
	cgDevice = (DEVICE*)tDevice;
	cgSBICPAR.cgDevice = (DEVICE*)tDevice;
	COMMAND_NODE::CopyCOMMAND_NODE(tCommand, &cgMCommand);
	eCommandThread.ThreadRun();
	SMC_EncryptE(0)
#endif
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 Script::Execute(const DEVICE *tDevice,const COMMAND_GROUP *tGroup){
#ifdef	SWVERSION_SCRIPT
	SMC_EncryptI(0)
	if ((tDevice == nullptr) || (tGroup == nullptr))
		return 0;
	if (InDoing_try() == 0)
		return 0;
	SMC_EncryptS(0)
	StopRun();
	Init();
	Enable();
	cgDevice = (DEVICE*)tDevice;
	cgSBICPAR.cgDevice = (DEVICE*)tDevice;
	COMMAND_GROUP::CopyCOMMAND_GROUP(tGroup, &cgMGroup);
	eGroupThread.ThreadRun();
	SMC_EncryptE(0)
#endif
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 Script::Execute(const DEVICE *tDevice,const GC_LIST *tGroupList){
#ifdef	SWVERSION_SCRIPT
	SMC_EncryptI(0)
	if ((tDevice == nullptr) || (tGroupList == nullptr))
		return 0;
	if (InDoing_try() == 0)
		return 0;
	SMC_EncryptS(0)
	StopRun();
	Init();
	Enable();
	cgDevice = (DEVICE*)tDevice;
	cgSBICPAR.cgDevice = (DEVICE*)tDevice;
	GC_LIST::CopyCOMMAND_GROUP_ENABLE(tGroupList,&cgMGroupList);
	eGroupListThread.ThreadRun();
	SMC_EncryptE(0)
#endif
	return 1;
}
//------------------------------------------------------------------------------------------//
void Script::Init(void){
	cgDevice = nullptr;
	cgSBICPAR.cgDevice = nullptr;
	cgSBICPAR.cgBuffer = &cgBuffer;
	cgSBICPAR.cgCommand = nullptr;
	cgSBICPAR.cgRecvbuf = "";
	cgSBICPAR.cgSearchRet = "";
	cgSBICPAR.cgFunRet = "";
	cgSBICPAR.blExit = 0;
	cgSBICPAR.blRSC = 0;
	cgSBICPAR.cgODevNode = nullptr;
	cgSBICPAR.cgTimeS = nullptr;
#ifdef CommonDefH_Unix
	cgSBICPAR.cgSBIC_RE_LIST = &cgSBIC_RE_LIST;
#endif
	cgBuffer.Clean();
	ClrblRSC();
	ClrblIsTerminated();
	Disable();
}
//------------------------------------------------------------------------------------------//
void Script::StopRun(void){
	SetblIsTerminated();
	eGroupListThread.ThreadStop();
	eGroupThread.ThreadStop();
	eCommandThread.ThreadStop();
#ifdef CommonDefH_Unix
	cgSBIC_RE_LIST.Stop();
#endif
	Disable();
}
//------------------------------------------------------------------------------------------//
int32 Script::EGroupListThreadFun(void){
	int32				runTotalTimes;
	std::string			strETime;
	SYS_DateTime		executeTime,nowTime;
	
	executeTime.Now();
	cgDevice->rxBufferList.AddNode(&cgBuffer);
	
	runTotalTimes = 0;
	cgMGroupList.ClearTestResult();
	do{
		++ runTotalTimes;
		ExecuteGroupList(&cgMGroupList,runTotalTimes);
	}while(IsTerminated() == 0);
	
	nowTime.Now();
	executeTime.MinusDateTime(nowTime,executeTime);
	strETime = SYS_FormatTimeABS(DEFAULT_TIMEFORMATE,executeTime);
	ShowResult(&cgMGroupList,strETime);
	cgMGroupList.ClearTestResult();
	
	SYS_SleepMS(10);
	cgBuffer.RemoveSelf();
	Init();
	InDoing_clr();
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 Script::EGroupThreadFun(void){
	int32				runTotalTimes;
	std::string			strETime;
	SYS_DateTime		executeTime,nowTime;
	
	executeTime.Now();
	cgDevice->rxBufferList.AddNode(&cgBuffer);
	
	runTotalTimes = 0;
	cgMGroup.ClearResult();
	ExecuteGroup(&cgMGroup,runTotalTimes,1);
	nowTime.Now();
	executeTime.MinusDateTime(nowTime,executeTime);
	strETime = SYS_FormatTimeABS(DEFAULT_TIMEFORMATE,executeTime);
	ShowResult(&cgMGroup,strETime);
	cgMGroup.ClearResult();
	
	SYS_SleepMS(10);
	cgBuffer.RemoveSelf();
	Init();
	InDoing_clr();
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 Script::ECommandThreadFun(void){
	SYS_DateTime		executeTime,nowTime;
	std::string			strRecData,strRET;
	
	executeTime.Now();
	cgDevice->rxBufferList.AddNode(&cgBuffer);
	
	ExecuteCommand(&cgMCommand,100);
	
	nowTime.Now();
	executeTime.MinusDateTime(nowTime,executeTime);
	if (cgDevice->cgODevList.cgOutput != nullptr){
		cgDevice->cgODevList.cgOutput->Spin_InUse_set();
		cgDevice->cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon);
		cgDevice->cgODevList.cgOutput->WriteStrN("Execute Time: ",RICH_CF_clPurple);
		cgDevice->cgODevList.cgOutput->WriteStr(SYS_FormatTimeABS(DEFAULT_TIMEFORMATE,executeTime),RICH_CF_clPurple);
		cgDevice->cgODevList.cgOutput->WriteStr("\r\nEnd\r\n",RICH_CF_clPurple);
		cgDevice->cgODevList.cgOutput->Spin_InUse_clr();
	}
	SYS_SleepMS(10);
	cgBuffer.RemoveSelf();
	Init();
	InDoing_clr();
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 Script::ExecuteGroupList(GC_LIST *tGroupList,const int32 &runTotalTimes){
	RTREE_LChildRChain_Traversal_LINE_nolock(COMMAND_GROUP,tGroupList,
		if (IsTerminated() != 0)
			break;
		if (operateNode_t->blEnableAutoRun != 0)
		ExecuteGroup(operateNode_t,runTotalTimes,0);
	);
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 Script::ExecuteGroup(COMMAND_GROUP *tGroup,const int32 &runTotalTimes,int32 blType){
	int32			blRun,frameTimeout,exitType,cycleTimes,runTimes;
	std::string		strRecData,strRET,strTemp,stringCommand,resultLable,retCMDLable,strTempData;
	SYS_TIME_S		timeS;
	strRecData = "";
	blRun = 0;
	exitType = 0;
	runTimes = tGroup->autoRunTimes;
	if ((blType != 0) && (runTimes == 0)){
		runTimes = 1;
	}
	else if (runTimes == 0){
		blRun = 1;
	}
	
	cycleTimes = 0;
	do{
		++ cycleTimes;
		if ((runTimes != 1) || ((runTimes == 1) && (blType == 0))){
			if (cgDevice->cEDevFlag.blEnablePrintSBICinfo != 0){
				strTemp = "Loop: ";
				strTemp += Str_IntToString(runTotalTimes);
				strTemp += "::";
				strTemp += tGroup->name;
				strTemp += "::Cycle: ";
				strTemp += Str_IntToString(cycleTimes);
				strTemp += "\r\nStart\r\n";
				if (cgDevice->cgODevList.cgOutput != nullptr){
					cgDevice->cgODevList.cgOutput->Spin_InUse_set();
					cgDevice->cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon);
					cgDevice->cgODevList.cgOutput->WriteStrN(strTemp, RICH_CF_clPurple);
					cgDevice->cgODevList.cgOutput->Spin_InUse_clr();
				}
			}
		}
		
		RTREE_LChildRChain_Traversal_LINE_nolock(COMMAND_NODE,tGroup,
			if (IsTerminated() != 0)
				break;
			frameTimeout = tGroup->intervalTime;
			if (frameTimeout < 1)
				frameTimeout = 200;
			SYS_Delay_SetTS(&timeS,frameTimeout);
			exitType = ExecuteCommand(operateNode_t,frameTimeout);
			if (operateNode_t != cgSBICPAR.cgCommand)
				nextNode_t = cgSBICPAR.cgCommand;//used for goto cmd
			if (exitType == 6){//stop
				SYS_DelayMS(50,&cgSBICPAR.blExit);
				break;
			}
			if (exitType == 3){//break
				SYS_DelayMS(50,&cgSBICPAR.blExit);
				break;
			}
			if (exitType == 2)//continue
				SYS_Delay_SetTS(&timeS,frameTimeout);
			if (exitType > 1)
				while((SYS_Delay_CheckTS(&timeS) == 0) && (IsTerminated() == 0));
		);
		if ((runTimes != 1) || ((runTimes == 1) && (blType == 0))){
			if (cgDevice->cEDevFlag.blEnablePrintSBICinfo != 0){
				strTemp = "Loop: ";
				strTemp += Str_IntToString(runTotalTimes);
				strTemp += "::";
				strTemp += tGroup->name;
				strTemp += "::Cycle: ";
				strTemp += Str_IntToString(cycleTimes);
				strTemp += "\r\nEnd\r\n";
				if (cgDevice->cgODevList.cgOutput != nullptr){
					cgDevice->cgODevList.cgOutput->Spin_InUse_set();
					cgDevice->cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon);
					cgDevice->cgODevList.cgOutput->WriteStrN(strTemp, RICH_CF_clPurple);
					cgDevice->cgODevList.cgOutput->Spin_InUse_clr();
				}
			}
		}
		if (exitType == 6)//stop
			return 1;
	}while(((blRun != 0) || (cycleTimes < runTimes)) && (IsTerminated() == 0));
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 Script::ExecuteCommand(COMMAND_NODE *tCommand,int32 frameTimeout){
	std::string	commandStrRet,strPrintData,strHexCommand,strOCommand,strTemp;
	int32		eRetCode;
	int32		blEnResend;	//check meet execute resend contidion
	int32		maxCycle,i,timeout,ByteNum;
	SYS_TIME_S	timeS;
	//return(0)--->no execute due to blEnableSend == 0.
	//return(1)--->execute BIC,no need to wait.
	//return(2)--->meet continue condition.
	//return(3)--->meet break condition.
	//return(4)--->meet resend condition.
	//return(5)--->cycle-- to 0
	//return(6)--->meet stop condition.
	
	cgSBICPAR.cgCommand = tCommand;
	cgSBICPAR.blCMDRet = 1;
	
	if (tCommand->blEnableSend != 0){
		maxCycle = 1;//set cycle times.
		if (tCommand->StrCycle.length() > 0)
			maxCycle = atoi(tCommand->StrCycle.c_str());
		i = 0;
		strOCommand = DelComment(tCommand->StrCommand);
		while(((i++ < maxCycle) || (maxCycle == 0)) && (IsTerminated() == 0)){
			eRetCode = ExecuteLC(&cgSBICPAR,strOCommand,&strPrintData);
			if (eRetCode != SBI_RETCODE_NO)
				return(cgSBICPAR.blCMDRet);
			cgSubC_PRINT.CreateHexCommand(&cgSBICPAR,strOCommand,tCommand->blEnableHEX,tCommand->blEnableSendCR,G_ESCAPE_ON,&strHexCommand,&strPrintData);
			
			cgBuffer.Clean();
			ByteNum = cgDevice->WriteInHEX(strHexCommand);
			cgDevice->PrintSendCommand(strPrintData,ByteNum);
			
			++ tCommand->runTimes;
			timeout = 0;
			SYS_Delay_SetTS(&timeS,atof(tCommand->StrTimeout.c_str()) * 1000);
			
			cgSBICPAR.cgTimeS = &timeS;
			blEnResend = 0;
			cgSBICPAR.cgRecvbuf = "";
			while(timeout == 0 && IsTerminated() == 0){
				cgBuffer.GetInHEX(&cgSBICPAR.cgRecvbuf,G_SPACE_OFF);
				if ((tCommand->StrCatch.length() > 0) && (cgSubC_Expression.Expression(&cgSBICPAR,tCommand->StrCatch) != 0))
					++ tCommand->catchTimes;
				if ((tCommand->StrContinue.length() > 0) && (cgSubC_Expression.Expression(&cgSBICPAR,tCommand->StrContinue) != 0))
					return 2;
				if ((tCommand->StrResend.length() > 0) && (cgSubC_Expression.Expression(&cgSBICPAR,tCommand->StrResend) != 0)){
					blEnResend = 1;
					timeout = SYS_Delay_CheckTS(&timeS);
					break;
				}
				if ((tCommand->StrStop.length() > 0) && (cgSubC_Expression.Expression(&cgSBICPAR,tCommand->StrStop) != 0))
					return 3;
				if ((tCommand->StrContinue.length() == 0) && (tCommand->StrResend.length() == 0)
					&& (tCommand->StrStop.length() == 0) && (cgDevice->cEDevFlag.blScriptAT != 0)
					&& (cgSubC_Expression.Expression(&cgSBICPAR,cgATCondition) != 0)){
					return 2;
				}
				timeout = SYS_Delay_CheckTS(&timeS);
			}
			if (timeout != 0){
				if (cgSBICPAR.cgRecvbuf.length() == 0)
					++ tCommand->timeoutTimes;//no response
				if ((tCommand->StrContinue.length() > 0) && (cgSubC_Expression.Expression(&cgSBICPAR,tCommand->StrContinue) != 0))
					return 2;
				if ((tCommand->StrResend.length() > 0) && (cgSubC_Expression.Expression(&cgSBICPAR,tCommand->StrResend) != 0)){
					blEnResend = 1;
				}
				else if ((tCommand->StrStop.length() > 0) && (cgSubC_Expression.Expression(&cgSBICPAR,tCommand->StrStop) != 0)){
					return 3;
				}
				if ((cgSBICPAR.cgRecvbuf.length() > 0) && (tCommand->StrResend.length() > 0) && (blEnResend == 0))
					return 4;//receive some data, but no match resend condition,do next command
			}
			if ((blEnResend != 0) && (timeout == 0))
				SYS_DelayMS(frameTimeout,&cgSBICPAR.blExit);
		}
		return 5;
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
void Script::PrintGroupResult(COMMAND_GROUP *tGroup,const std::string &strGroupName){
	std::string			strTemp,strCommand;
	int32				i,blGT;
	
	i = 0;
	blGT = 0;
	RTREE_LChildRChain_Traversal_LINE_nolock(COMMAND_NODE,tGroup,
		++ i;
		if (operateNode_t->catchTimes > 0){
			strCommand = SBIC_Node::DelComment(operateNode_t->StrCommand);
			if (strCommand.length() > 24){
				strCommand = strCommand.substr(0,20) + " ...";
			}
			else{
				strCommand.insert(strCommand.length(),24 - strCommand.length(),' ');
			}
			strTemp = "\r\n";
			if (blGT == 0){
				strTemp += strGroupName;
				blGT = 1;
			}
			strTemp += "  N" + Str_IntToString(i) + "::" + strCommand;
			cgDevice->cgODevList.cgOutput->WriteStr(strTemp,RICH_CF_clBlue);
			strTemp = ",catch(" + operateNode_t->StrCatch + "):" + Str_IntToString(operateNode_t->catchTimes) + "/" + Str_IntToString(operateNode_t->runTimes);
			if (operateNode_t->timeoutTimes > 0)
				strTemp += ",no response:" + Str_IntToString(operateNode_t->timeoutTimes) + "/" + Str_IntToString(operateNode_t->runTimes);
			cgDevice->cgODevList.cgOutput->WriteStr(strTemp,RICH_CF_clRed);
		}
		else if (operateNode_t->timeoutTimes > 0){
			strCommand = SBIC_Node::DelComment(operateNode_t->StrCommand);
			if (strCommand.length() > 24){
				strCommand = strCommand.substr(0,20) + " ...";
			}
			else{
				strCommand.insert(strCommand.length(),24 - strCommand.length(),' ');
			}
			strTemp = "\r\n";
			if (blGT == 0){
				strTemp += strGroupName;
				blGT = 1;
			}
			strTemp += "  N" + Str_IntToString(i) + "::" + strCommand;
			cgDevice->cgODevList.cgOutput->WriteStr(strTemp,RICH_CF_clBlue);
			strTemp = ",no response:" + Str_IntToString(operateNode_t->timeoutTimes) + "/" + Str_IntToString(operateNode_t->runTimes);
			cgDevice->cgODevList.cgOutput->WriteStr(strTemp,RICH_CF_clRed);
		}
	);
}
//------------------------------------------------------------------------------------------//
void Script::ShowResult(COMMAND_GROUP *tGroup,const std::string &tExecuteTime){
	std::string	strGroupName;

	if (cgDevice->cgODevList.cgOutput != nullptr){
		cgDevice->cgODevList.cgOutput->Spin_InUse_set();
		cgDevice->cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon);
		cgDevice->cgODevList.cgOutput->WriteStrN("Execute Time: ",RICH_CF_clPurple);
		cgDevice->cgODevList.cgOutput->WriteStr(tExecuteTime,RICH_CF_clPurple);
		
		if (tGroup->autoRunTimes > 1){
			strGroupName = "G::" + tGroup->name + ":\r\n";
			PrintGroupResult(tGroup,strGroupName);
		}
		cgDevice->cgODevList.cgOutput->WriteStr("\r\nEnd\r\n",RICH_CF_clPurple);
		cgDevice->cgODevList.cgOutput->Spin_InUse_clr();
	}
}
//------------------------------------------------------------------------------------------//
void Script::ShowResult(GC_LIST *tGroupList,const std::string &tExecuteTime){
	std::string	strGroupName;
	int32		i;
	
	if (cgDevice->cgODevList.cgOutput != nullptr){
		cgDevice->cgODevList.cgOutput->Spin_InUse_set();
		cgDevice->cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon);
		cgDevice->cgODevList.cgOutput->WriteStrN("Execute Time: ",RICH_CF_clPurple);
		cgDevice->cgODevList.cgOutput->WriteStr(tExecuteTime,RICH_CF_clPurple);
		
		i = 0;
		RTREE_LChildRChain_Traversal_LINE_nolock(COMMAND_GROUP,tGroupList,
			++ i;
			if (operateNode_t->blEnableAutoRun != 0){
				strGroupName = "G" + Str_IntToString(i) + "::" + operateNode_t->name + ":\r\n";
				PrintGroupResult(operateNode_t,strGroupName);
			}
		);
		cgDevice->cgODevList.cgOutput->WriteStr("\r\nEnd\r\n",RICH_CF_clPurple);
		cgDevice->cgODevList.cgOutput->Spin_InUse_clr();
	}
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void Script::Help(ODEV_NODE *tODevNode){
	SBICPAR	tSBICPAR;
	std::string strTemp;
	if (tODevNode == nullptr)
		return;
	tSBICPAR.cgODevNode = tODevNode;
	
	strTemp = "Written by Leif Wen,Began in 2012-02-09\r\n\r\n";
	strTemp += SWVERSION_SHORTNAME;
	strTemp += " Help ";
	strTemp += SWVERSION_VER;
	PrintM(&tSBICPAR,strTemp);
	
	PrintP(&tSBICPAR,"----------------------------------------------------------------------");
	PrintM(&tSBICPAR,"Script Introduction:"													  );
	PrintM(&tSBICPAR,"Command            : ","Data or build-in commands."					  );
	PrintM(&tSBICPAR,"Timeout(s)         : ","Receive timeout."								  );
	PrintM(&tSBICPAR,"Cycle              : ","Cycle times,if blank,Cycle = 1 as default."	  );
	PrintM(&tSBICPAR,"Send<CR>           : ","Add \\r(0x0D) in the end of command."			  );
	PrintM(&tSBICPAR,"HEX                : ","Command is HEX format."						  );
	PrintM(&tSBICPAR,"Continue Condition : ","Continue expression. If blank, do not check it." );
	PrintM(&tSBICPAR,"Stop Condition     : ","Stop expression. If blank, do not check it."	  );
	PrintM(&tSBICPAR,"Resend Condition   : ","Resend expression. If blank, do not check it."	  );
	PrintM(&tSBICPAR,"Catch Condition    : ","Log expression. If blank, do not check it."	  );
	PrintP(&tSBICPAR,"----------------------------------------------------------------------");
	
	PrintM(&tSBICPAR,"How does \"Condition\" work:");
	PrintM(&tSBICPAR," A. After send \"Command\",");
	PrintM(&tSBICPAR,"   1. If no receive timeout:");
	PrintP(&tSBICPAR,"     1.1  If recevie some data:");
	PrintB(&tSBICPAR,"        1.1.1  Check \"Catch Condition\".");
	PrintB(&tSBICPAR,"        1.1.2  If received data contains \"Catch Condition\", the build-in \"catch counter\" is incremented by one and report the result when the scprit stop. It's valid in \"Execute Automatically Command Group List\" test only.");
	PrintB(&tSBICPAR,"        1.1.3  Check \"Continue Condition\".");
	PrintB(&tSBICPAR,"        1.1.4  If received data contains \"Continue Condition\", then goto send next \"Command\".");
	PrintB(&tSBICPAR,"        1.1.5  Else check \"Resend Condition\".");
	PrintB(&tSBICPAR,"        1.1.6  If received data contains \"Resend Condition\", then goto re-send this \"Command\".");
	PrintB(&tSBICPAR,"        1.1.7  Else check \"Stop Condition\".");
	PrintB(&tSBICPAR,"        1.1.8  If received data contains \"Stop Condition\", the script execution will be stopped.");
	PrintB(&tSBICPAR,"        1.1.9  Else \"Stop Condition\", check whether is timeout.");
	PrintB(&tSBICPAR,"        1.1.10 If timeout, \"Cycle\" minus one, and goto re-send this \"Command\" until \"Cycle\" = 0, then goto send next \"Command\".");
	PrintB(&tSBICPAR,"        1.1.11 Else no timeout, waitting to receive data.");
	PrintP(&tSBICPAR,"     1.2  Else waitting to receive data.");
	PrintM(&tSBICPAR,"   2. If timeout:");
	PrintP(&tSBICPAR,"     2.1  If no recevie any data, the build-in \"no response counter\" is incremented by one and report the result when the scprit stop. It's valid in \"Execute Automatically Command Group List\" test only.");
	PrintP(&tSBICPAR,"     2.2  \"Cycle\" minus one, and goto re-send this \"Command\" until \"Cycle\" = 0, then goto send next \"Command\".");
	
	PrintM(&tSBICPAR," B. \"Condition\"'s arithmetic expression:");
	PrintM(&tSBICPAR,"   1. Support AND, OR, NOR, [, ] operators.");
	PrintP(&tSBICPAR,"     1.1  AND operators is : && or &.");
	PrintP(&tSBICPAR,"     1.2  OR operators is  : || or |.");
	PrintP(&tSBICPAR,"     1.3  NOR operators is  :!.");
	PrintM(&tSBICPAR,"   2. Automatic delete spaces character(0x20) before and after the \"Condition\" expression.");
	//	::PrintP(&tSBICPAR,"     2.1  Eg. The expression is ABCD && DEF, if the received data contains the ABCD and DEF, the expression is true.");
	//	::PrintP(&tSBICPAR,"     2.2  Eg. The expression is \"ABCD \" && DEF, if the received data contains 'ABCD '(5bytes) and DEF, the expression is true.");
	PrintM(&tSBICPAR,"   3. if use &, |,! , [, ], \\, need use \\&, \\|, \\!, \\[, \\], \\\\ to escape.");
	PrintM(&tSBICPAR,"   4. Support hexadecimal escape input,Format:\\0xhh or \\0Xhh.");
	PrintM(&tSBICPAR,"   5. Support these escape character: \\a, \\b, \\f, \\n, \\r, \\t, \\v, \\\\, \\', \\\", \\0, \\/, \\*, \\?.");
	PrintM(&tSBICPAR,"   6. Support build-in command: 'Receive, 'CTS, 'DSR, 'RING, 'DCD, 'Timeout, 'nullptr, 'RX.");
	PrintM(&tSBICPAR,"   7. Support * and ? compare.");
	
	PrintP(&tSBICPAR,"----------------------------------------------------------------------");
	PrintM(&tSBICPAR,"Build-in Command Format:");
	PrintM(&tSBICPAR,"   A.Command = <'CMD>[//COMMENT].");
	PrintM(&tSBICPAR,"   B.Command = [<data>][//COMMENT].");
	PrintP(&tSBICPAR, "Notes:");
	PrintP(&tSBICPAR,"   1.Only accept visible characters(invisible characters instead of the use of escape character).");
	PrintP(&tSBICPAR,"   2.<> means: delete spaces character(0x20) before and after CMD. [] means: options.");
	PrintP(&tSBICPAR,"   3.Support hexadecimal escape input,Format:\\0xhh or \\0Xhh.");
	PrintP(&tSBICPAR,"   4.Support these escape character: \\a, \\b, \\f, \\n, \\r, \\t, \\v, \\\\, \\', \\\", \\0, \\/, \\*, \\?.");
	PrintP(&tSBICPAR,"   5.\"Single Command\" support '', 'HEX, 'TIME, 'Build/Send.");
	PrintP(&tSBICPAR,"   6.\"Data Transmission\" support '', 'HEX, 'TIME, 'Build/Send.");
	PrintP(&tSBICPAR,"----------------------------------------------------------------------");
	PrintM(&tSBICPAR,"CMD List:(CMD Ignore case)");
	
	SBIC_Combine	tSBIC_Combine;
	SBIC_Conditon	tSBIC_Conditon;
	HelpLC(&tSBICPAR,1);
	tSBIC_Combine.Help(&tSBICPAR,1);
	tSBIC_Conditon.Help(&tSBICPAR,1);
	
	PrintP(&tSBICPAR,"----------------------------------------------------------------------");
	PrintM(&tSBICPAR,"End");
}
//------------------------------------------------------------------------------------------//
