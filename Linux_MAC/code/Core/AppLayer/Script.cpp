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
#include "SMC.h"
#ifdef ScriptH
//------------------------------------------------------------------------------------------//
Script::Script(void) : DEVICE_EXE_FRAME()
		#ifdef SBIC_RunEXEH
		,cgAExePool(BUF_MAX_SIZE,nullptr)
		#endif
{
	SetblATResponse();
	SetblPrintSBICinfo();

	cgATCondition = TELIT_AT_RESPONSE;
	eGroupListThread.ThreadInit	(this, &Script::EGroupListThreadFun	,"grouplist");
	eGroupThread.ThreadInit		(this, &Script::EGroupThreadFun		,"group");
	eCommandThread.ThreadInit	(this, &Script::ECommandThreadFun	,"command");
}
//------------------------------------------------------------------------------------------//
void Script::Init(const DEVICE *tDevice){
	SetDefAttr(&cgSBICENV,tDevice,&cgSBUF);
	#ifdef SBIC_RunEXEH
	cgSBICENV.cgAExePool = &cgAExePool;
	#endif
	DEVICE_EXE_FRAME::Init(tDevice);
}
//------------------------------------------------------------------------------------------//
void Script::Stop(void){
#ifdef SBIC_RunEXEH
	cgAExePool.CloseD();
#endif
	DEVICE_EXE_FRAME::Stop();
}
//------------------------------------------------------------------------------------------//
int32 Script::Execute(const DEVICE *tDevice,const GC_LIST *tGroupList){
	if ((tGroupList == nullptr) || CheckExecute(tDevice) == 0)
		return 0;
	GC_LIST::CopyCOMMAND_GROUP_ENABLE(&cgMGroupList,tGroupList);
	return(DEVICE_EXE_FRAME::Execute(tDevice,&eGroupListThread));
}
//------------------------------------------------------------------------------------------//
int32 Script::Execute(const DEVICE *tDevice,const COMMAND_GROUP *tGroup){
	if ((tGroup == nullptr) || CheckExecute(tDevice) == 0)
		return 0;
	COMMAND_GROUP::CopyCOMMAND_GROUP(&cgMGroup,tGroup);
	return(DEVICE_EXE_FRAME::Execute(tDevice,&eGroupThread));
}
//------------------------------------------------------------------------------------------//
int32 Script::Execute(const DEVICE *tDevice,const COMMAND_NODE *tCommand){
	if ((tCommand == nullptr) || CheckExecute(tDevice) == 0)
		return 0;
	COMMAND_NODE::CopyCOMMAND_NODE(&cgMCommand,tCommand);
	return(DEVICE_EXE_FRAME::Execute(tDevice,&eCommandThread));
}
//------------------------------------------------------------------------------------------//
int32 Script::EGroupListThreadFun(void *p){
	int32	runTotalTimes;
	STDSTR	strETime;
	TIME	executeTime,nowTime;
	
	executeTime.Now();
	runTotalTimes = 0;
	cgMGroupList.ClearTestResult();
	
	do{
		++ runTotalTimes;
		ExecuteGroupList(&cgMGroupList,runTotalTimes);
	}while(IsTerminated() == 0);
	
	nowTime.Now();
	executeTime.MinusDateTime(nowTime,executeTime);
	ShowResult(&cgMGroupList,SYS_FormatTimeABS(DEFAULT_TIMEFORMATE,executeTime));
	cgMGroupList.ClearTestResult();
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 Script::EGroupThreadFun(void *p){
	int32	runTotalTimes;
	STDSTR	strETime;
	TIME	executeTime,nowTime;
	
	executeTime.Now();
	runTotalTimes = 0;
	cgMGroup.ClearResult();
	
	ExecuteGroup(&cgMGroup,runTotalTimes,1);
	
	nowTime.Now();
	executeTime.MinusDateTime(nowTime,executeTime);
	ShowResult(&cgMGroup,SYS_FormatTimeABS(DEFAULT_TIMEFORMATE,executeTime));
	cgMGroup.ClearResult();
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 Script::ECommandThreadFun(void *p){
	TIME		executeTime,nowTime;
	STDSTR		strRecData,strRET;
	OUTPUT_NODE	*cOut;
	
	executeTime.Now();
	
	ExecuteCommand(&cgMCommand,100);
	
	nowTime.Now();
	executeTime.MinusDateTime(nowTime,executeTime);
	
	SBIC_CMD::DeviceLock(&cgSBICENV);
	cOut = SBIC_CMD::GetDevOut(&cgSBICENV);
	SBIC_CMD::PrintNL(cOut)
	<< COL_DivLine_Maroon
	<< COL_NormalMessage << "Execute Time: " << SYS_FormatTimeABS(DEFAULT_TIMEFORMATE,executeTime)
	<< "\n\nEnd\n" << Endl;
	SBIC_CMD::DeviceUnlock(&cgSBICENV);
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 Script::ExecuteGroupList(GC_LIST *tGroupList,const int32 &runTotalTimes){
	TREE_LChildRChain_Traversal_LINE_nolock(COMMAND_GROUP,tGroupList,
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
	STDSTR			strRecData,strRET,strTemp,stringCommand,resultLable,retCMDLable,strTempData;
	SYS_TIME_S		timeS;
	OUTPUT_NODE		*cOut;
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
			if (CheckPrintSBICinfo() != 0){
				SBIC_CMD::DeviceLock(&cgSBICENV);
				cOut = SBIC_CMD::GetDevOut(&cgSBICENV);
				SBIC_CMD::PrintNL(cOut)
				<< COL_DivLine_Maroon
				<< COL_NormalMessage << "Loop: " << Str_ToString(runTotalTimes) << "::" << tGroup->name << "::Cycle: " << Str_ToString(cycleTimes)
				<< "\n\nStart\n"
				<< Endl;
				SBIC_CMD::DeviceUnlock(&cgSBICENV);
			}
		}
		
		TREE_LChildRChain_Traversal_LINE_nolock(COMMAND_NODE,tGroup,
			if (IsTerminated() != 0)
				break;
			frameTimeout = tGroup->intervalTime;
			if (frameTimeout < 1)
				frameTimeout = 200;
			SYS_Delay_SetTS(&timeS,frameTimeout);
			exitType = ExecuteCommand(operateNode_t,frameTimeout);
			if (operateNode_t != cgSBICENV.cgCommand)
				nextNode_t = cgSBICENV.cgCommand;//used for goto cmd
			if (exitType == 6){//stop
				SYS_DelayMS(50,&cgSBICENV.blExit);
				break;
			}
			if (exitType == 3){//break
				SYS_DelayMS(50,&cgSBICENV.blExit);
				break;
			}
			if (exitType == 2)//continue
				SYS_Delay_SetTS(&timeS,frameTimeout);
				if (exitType > 1)
					while((SYS_Delay_CheckTS(&timeS) == 0) && (IsTerminated() == 0));
			);
		if ((runTimes != 1) || ((runTimes == 1) && (blType == 0))){
			if (CheckPrintSBICinfo() != 0){
				SBIC_CMD::DeviceLock(&cgSBICENV);
				cOut = SBIC_CMD::GetDevOut(&cgSBICENV);
				SBIC_CMD::PrintNL(cOut)
				<< COL_DivLine_Maroon
				<< COL_NormalMessage << "Loop: " << Str_ToString(runTotalTimes) << "::" << tGroup->name << "::Cycle: " << Str_ToString(cycleTimes)
				<< "\n\nEnd\n"
				<< Endl;
				SBIC_CMD::DeviceUnlock(&cgSBICENV);
			}
		}
		if (exitType == 6)//stop
			return 1;
	}while(((blRun != 0) || (cycleTimes < runTimes)) && (IsTerminated() == 0));
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 Script::ExecuteCommand(COMMAND_NODE *tCommand,int32 frameTimeout){
	STDSTR		commandStrRet,strPrintData,strForSend,strOCommand,strTemp;
	int32		eRetCode;
	int32		blEnResend;	//check meet execute resend contidion
	int32		maxCycle,i,timeout,ByteNum;
	
	//return(0)--->no execute due to blEnableSend == 0.
	//return(1)--->execute BIC,no need to wait.
	//return(2)--->meet continue condition.
	//return(3)--->meet break condition.
	//return(4)--->meet resend condition.
	//return(5)--->cycle-- to 0
	//return(6)--->meet stop condition.
	
	cgSBICENV.cgCommand = tCommand;
	cgSBICENV.blCMDRet = 1;
	
	if (tCommand->blEnableSend != 0){
		maxCycle = 1;//set cycle times.
		if (tCommand->StrCycle.length() > 0)
			maxCycle = atoi(tCommand->StrCycle.c_str());
		i = 0;
		strOCommand = SBIC_CMD::DelComment(tCommand->StrCommand);
		while(((i++ < maxCycle) || (maxCycle == 0)) && (IsTerminated() == 0)){
			eRetCode = cgSBICCMD.ExecuteTraversalChild(&cgSBICENV,strOCommand,nullptr);
			if (eRetCode != SBI_RETCODE_NO)
				return(cgSBICENV.blCMDRet);
			cgSBICCMD.cgSubC_PRINT.CreateCommand(&cgSBICENV,&strForSend,&strPrintData,strOCommand,tCommand->cmdTail,G_ESCAPE_ON);
			
			cgSBUF.Clean();
			ByteNum = cgSBICENV.eda->device->SSend(strForSend,G_ESCAPE_OFF);
			cgSBICENV.eda->device->PrintSendCommand(strPrintData,ByteNum);
			
			++ tCommand->runTimes;
			timeout = 0;
			SYS_Delay_SetTS(&cgSBICENV.cgTimeS,atof(tCommand->StrTimeout.c_str()) * 1000);
			
			blEnResend = 0;
			cgSBICENV.cstdin_Str = "";
			while(timeout == 0 && IsTerminated() == 0){
				if ((tCommand->StrCatch.length() > 0) && (cgSBICCMD.cgSubC_Expression.Expression(&cgSBICENV,tCommand->StrCatch) != 0))
					++ tCommand->catchTimes;
				if ((tCommand->StrContinue.length() > 0) && (cgSBICCMD.cgSubC_Expression.Expression(&cgSBICENV,tCommand->StrContinue) != 0))
					return 2;
				if ((tCommand->StrResend.length() > 0) && (cgSBICCMD.cgSubC_Expression.Expression(&cgSBICENV,tCommand->StrResend) != 0)){
					blEnResend = 1;
					timeout = SYS_Delay_CheckTS(&cgSBICENV.cgTimeS);
					break;
				}
				if ((tCommand->StrStop.length() > 0) && (cgSBICCMD.cgSubC_Expression.Expression(&cgSBICENV,tCommand->StrStop) != 0))
					return 3;
				if ((tCommand->StrContinue.length() == 0)
					&& (tCommand->StrResend.length() == 0)
					&& (tCommand->StrStop.length() == 0)
					&& (CheckATResponse() != 0)
					&& (cgSBICCMD.cgSubC_Expression.Expression(&cgSBICENV,cgATCondition) != 0)){
					return 2;
				}
				timeout = SYS_Delay_CheckTS(&cgSBICENV.cgTimeS);
			}
			if (timeout != 0){
				if (cgSBICENV.cstdin_Str.length() == 0)
					++ tCommand->timeoutTimes;//no response
				if ((tCommand->StrContinue.length() > 0) && (cgSBICCMD.cgSubC_Expression.Expression(&cgSBICENV,tCommand->StrContinue) != 0))
					return 2;
				if ((tCommand->StrResend.length() > 0) && (cgSBICCMD.cgSubC_Expression.Expression(&cgSBICENV,tCommand->StrResend) != 0)){
					blEnResend = 1;
				}
				else if ((tCommand->StrStop.length() > 0) && (cgSBICCMD.cgSubC_Expression.Expression(&cgSBICENV,tCommand->StrStop) != 0)){
					return 3;
				}
				if ((cgSBICENV.cstdin_Str.length() > 0) && (tCommand->StrResend.length() > 0) && (blEnResend == 0))
					return 4;//receive some data, but no match resend condition,do next command
			}
			if ((blEnResend != 0) && (timeout == 0))
				SYS_DelayMS(frameTimeout,&cgSBICENV.blExit);
		}
		return 5;
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
void Script::PrintGroupResult(OUTPUT_NODE* cOut,COMMAND_GROUP *tGroup,const STDSTR &strGroupName){
	STDSTR		strCommand;
	int32		i,blGT;
	i = 0;
	blGT = 0;
	TREE_LChildRChain_Traversal_LINE_nolock(COMMAND_NODE,tGroup,
		++ i;
		if (operateNode_t->catchTimes > 0){
			strCommand = SBIC_Node::DelComment(operateNode_t->StrCommand);
			if (strCommand.length() > 24){
				strCommand = strCommand.substr(0,20) + " ...";
			}
			else{
				strCommand.insert(strCommand.length(),24 - strCommand.length(),' ');
			}
			*cOut << NL << COL_clBlue;
			if (blGT == 0){
				*cOut << strGroupName;
				blGT = 1;
			}
			*cOut << "  N" << Str_ToString(i) << "::" << strCommand
			<< COL_clBlue << ",catch(" << operateNode_t->StrCatch << "):" << Str_ToString(operateNode_t->catchTimes) << "/" << Str_ToString(operateNode_t->runTimes);
			
			if (operateNode_t->timeoutTimes > 0){
				*cOut << COL_clRed << ",no response:" << Str_ToString(operateNode_t->timeoutTimes) << "/" << Str_ToString(operateNode_t->runTimes);
			}
		}
		else if (operateNode_t->timeoutTimes > 0){
			strCommand = SBIC_Node::DelComment(operateNode_t->StrCommand);
			if (strCommand.length() > 24){
				strCommand = strCommand.substr(0,20) + " ...";
			}
			else{
				strCommand.insert(strCommand.length(),24 - strCommand.length(),' ');
			}
			*cOut << NL << COL_clBlue;
			if (blGT == 0){
				*cOut << strGroupName;
				blGT = 1;
			}
			*cOut << "  N" << Str_ToString(i) << "::" << strCommand
			<< COL_clRed << ",no response:" << Str_ToString(operateNode_t->timeoutTimes) << "/" << Str_ToString(operateNode_t->runTimes);
		}
	);
}
//------------------------------------------------------------------------------------------//
void Script::ShowResult(COMMAND_GROUP *tGroup,const STDSTR &tExecuteTime){
	STDSTR	strGroupName;
	OUTPUT_NODE	*cOut;
	SBIC_CMD::DeviceLock(&cgSBICENV);
	cOut = SBIC_CMD::GetDevOut(&cgSBICENV);
	SBIC_CMD::PrintNL(cOut)
	<< COL_DivLine_Maroon
	<< COL_NormalMessage << "Execute Time: " << tExecuteTime;
	
	if (tGroup->autoRunTimes > 1){
		strGroupName = "G::" + tGroup->name + ":\n";
		PrintGroupResult(cOut,tGroup,strGroupName);
	}
	*cOut << COL_NormalMessage << "\n\nEnd\n" << Endl;
	SBIC_CMD::DeviceUnlock(&cgSBICENV);
}
//------------------------------------------------------------------------------------------//
void Script::ShowResult(GC_LIST *tGroupList,const STDSTR &tExecuteTime){
	STDSTR	strGroupName;
	int32	i;
	OUTPUT_NODE	*cOut;
	SBIC_CMD::DeviceLock(&cgSBICENV);
	cOut = SBIC_CMD::GetDevOut(&cgSBICENV);
	
	SBIC_CMD::PrintNL(cOut)
	<< COL_DivLine_Maroon
	<< COL_NormalMessage << "Execute Time: " << tExecuteTime;
	i = 0;
	TREE_LChildRChain_Traversal_LINE_nolock(COMMAND_GROUP,tGroupList,
		++ i;
		if (operateNode_t->blEnableAutoRun != 0){
			strGroupName = "G" + Str_ToString(i) + "::" + operateNode_t->name + ":\n";
			PrintGroupResult(cOut,operateNode_t,strGroupName);
		}
	);
	*cOut << COL_NormalMessage << "\n\nEnd\n" << Endl;
	SBIC_CMD::DeviceUnlock(&cgSBICENV);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void Script::Help(OUTPUT_NODE *oDevNode){
	SBIC_ENV	tSBICENV;
	STDSTR		strTemp;
	
	if (oDevNode == nullptr)
		return;
	tSBICENV.cstdout = oDevNode;
	
	strTemp = "Written by Leif Wen,Began in 2012-02-09\n\n";
	strTemp += SWVERSION_SHORTNAME;
	strTemp += " Help ";
	strTemp += SWVERSION_VER;
	PrintM(&tSBICENV,strTemp);
	
	PrintP(&tSBICENV,"----------------------------------------------------------------------");
	PrintM(&tSBICENV,"Script Introduction:"													  );
	PrintM(&tSBICENV,"Command            : ","Data or build-in commands."					  );
	PrintM(&tSBICENV,"Timeout(s)         : ","Receive timeout."								  );
	PrintM(&tSBICENV,"Cycle              : ","Cycle times,if blank,Cycle = 1 as default."	  );
	PrintM(&tSBICENV,"Send<CR>           : ","Add \\r(0x0D) in the end of command."			  );
	PrintM(&tSBICENV,"HEX                : ","Command is HEX format."						  );
	PrintM(&tSBICENV,"Continue Condition : ","Continue expression. If blank, do not check it.");
	PrintM(&tSBICENV,"Stop Condition     : ","Stop expression. If blank, do not check it."	  );
	PrintM(&tSBICENV,"Resend Condition   : ","Resend expression. If blank, do not check it."  );
	PrintM(&tSBICENV,"Catch Condition    : ","Log expression. If blank, do not check it."	  );
	PrintP(&tSBICENV,"----------------------------------------------------------------------");
	
	PrintM(&tSBICENV,"How does \"Condition\" work:");
	PrintM(&tSBICENV," A. After send \"Command\",");
	PrintM(&tSBICENV,"   1. If no receive timeout:");
	PrintP(&tSBICENV,"     1.1  If recevie some data:");
	PrintB(&tSBICENV,"        1.1.1  Check \"Catch Condition\".");
	PrintB(&tSBICENV,"        1.1.2  If received data contains \"Catch Condition\", the build-in \"catch counter\" is incremented by one and report the result when the scprit stop. It's valid in \"Execute Automatically Command Group List\" test only.");
	PrintB(&tSBICENV,"        1.1.3  Check \"Continue Condition\".");
	PrintB(&tSBICENV,"        1.1.4  If received data contains \"Continue Condition\", then goto send next \"Command\".");
	PrintB(&tSBICENV,"        1.1.5  Else check \"Resend Condition\".");
	PrintB(&tSBICENV,"        1.1.6  If received data contains \"Resend Condition\", then goto re-send this \"Command\".");
	PrintB(&tSBICENV,"        1.1.7  Else check \"Stop Condition\".");
	PrintB(&tSBICENV,"        1.1.8  If received data contains \"Stop Condition\", the script execution will be stopped.");
	PrintB(&tSBICENV,"        1.1.9  Else \"Stop Condition\", check whether is timeout.");
	PrintB(&tSBICENV,"        1.1.10 If timeout, \"Cycle\" minus one, and goto re-send this \"Command\" until \"Cycle\" = 0, then goto send next \"Command\".");
	PrintB(&tSBICENV,"        1.1.11 Else no timeout, waitting to receive data.");
	PrintP(&tSBICENV,"     1.2  Else waitting to receive data.");
	PrintM(&tSBICENV,"   2. If timeout:");
	PrintP(&tSBICENV,"     2.1  If no recevie any data, the build-in \"no response counter\" is incremented by one and report the result when the scprit stop. It's valid in \"Execute Automatically Command Group List\" test only.");
	PrintP(&tSBICENV,"     2.2  \"Cycle\" minus one, and goto re-send this \"Command\" until \"Cycle\" = 0, then goto send next \"Command\".");
	
	PrintM(&tSBICENV," B. \"Condition\"'s arithmetic expression:");
	PrintM(&tSBICENV,"   1. Support AND, OR, NOR, [, ] operators.");
	PrintP(&tSBICENV,"     1.1  AND operators is : && or &.");
	PrintP(&tSBICENV,"     1.2  OR operators is  : || or |.");
	PrintP(&tSBICENV,"     1.3  NOR operators is  :!.");
	PrintM(&tSBICENV,"   2. Automatic delete spaces character(0x20) before and after the \"Condition\" expression.");
	//	::PrintP(&tSBICENV,"     2.1  Eg. The expression is ABCD && DEF, if the received data contains the ABCD and DEF, the expression is true.");
	//	::PrintP(&tSBICENV,"     2.2  Eg. The expression is \"ABCD \" && DEF, if the received data contains 'ABCD '(5bytes) and DEF, the expression is true.");
	PrintM(&tSBICENV,"   3. if use &, |,! , [, ], \\, need use \\&, \\|, \\!, \\[, \\], \\\\ to escape.");
	PrintM(&tSBICENV,"   4. Support hexadecimal escape input,Format:\\0xhh or \\0Xhh.");
	PrintM(&tSBICENV,"   5. Support these escape character: \\a, \\b, \\f, \\n, \\r, \\t, \\v, \\\\, \\', \\\", \\0, \\/, \\*, \\?.");
	PrintM(&tSBICENV,"   6. Support build-in command: 'Receive, 'CTS, 'DSR, 'RING, 'DCD, 'Timeout, 'nullptr, 'RX.");
	PrintM(&tSBICENV,"   7. Support * and ? compare.");
	
	PrintP(&tSBICENV,"----------------------------------------------------------------------");
	PrintM(&tSBICENV,"Build-in Command Format:");
	PrintM(&tSBICENV,"   A.Command = <'CMD>[//COMMENT].");
	PrintM(&tSBICENV,"   B.Command = [<data>][//COMMENT].");
	PrintP(&tSBICENV, "Notes:");
	PrintP(&tSBICENV,"   1.Only accept visible characters(invisible characters instead of the use of escape character).");
	PrintP(&tSBICENV,"   2.<> means: delete spaces character(0x20) before and after CMD. [] means: options.");
	PrintP(&tSBICENV,"   3.Support hexadecimal escape input,Format:\\0xhh or \\0Xhh.");
	PrintP(&tSBICENV,"   4.Support these escape character: \\a, \\b, \\f, \\n, \\r, \\t, \\v, \\\\, \\', \\\", \\0, \\/, \\*, \\?.");
	PrintP(&tSBICENV,"   5.\"Single Command\" support '', 'HEX, 'TIME, 'Build/Send.");
	PrintP(&tSBICENV,"   6.\"Data Transmission\" support '', 'HEX, 'TIME, 'Build/Send.");
	PrintP(&tSBICENV,"----------------------------------------------------------------------");
	PrintM(&tSBICENV,"CMD List:(CMD Ignore case)");
	
	SBIC_Combine	tSBIC_Combine;
	SBIC_Conditon	tSBIC_Conditon;
	cgSBICCMD.HelpTraversalChild(&tSBICENV,1);
	tSBIC_Combine.Help(&tSBICENV,1);
	tSBIC_Conditon.Help(&tSBICENV,1);
	
	PrintP(&tSBICENV,"----------------------------------------------------------------------");
	PrintM(&tSBICENV,"End");
}
//------------------------------------------------------------------------------------------//
#endif
