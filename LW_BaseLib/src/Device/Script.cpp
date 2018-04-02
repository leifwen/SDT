//
//  Script.cpp
//  SDT
//
//  Created by Leif Wen on 22/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "Script.h"
#ifdef Script_h
#include "BIF_Expression.h"
//------------------------------------------------------------------------------------------//
void	SCRIPT::SetblIsTerminated	(void){DEVICE_EXE_FRAME::SetblIsTerminated();BIF_BASE::SetblExit(&cgENV);};
void	SCRIPT::ClrblIsTerminated	(void){DEVICE_EXE_FRAME::ClrblIsTerminated();BIF_BASE::ClrblExit(&cgENV);};
void	SCRIPT::PrintB				(CMD_ENV* env,const STDSTR& str){BIF_BASE::PrintB(env,str);};
void	SCRIPT::PrintP				(CMD_ENV* env,const STDSTR& str){BIF_BASE::PrintP(env,str);};
void	SCRIPT::PrintM				(CMD_ENV* env,const STDSTR& strM,const STDSTR& strB){BIF_BASE::PrintM(env,strM,strB);};

void	SCRIPT::SetblPrintSBICinfo	(void){BIF_BASE::SetEnvFlag(&cgENV, BIF_ENV::ENV_blPrintBIFInfo);};
void	SCRIPT::ClrblPrintSBICinfo	(void){BIF_BASE::ClrEnvFlag(&cgENV, BIF_ENV::ENV_blPrintBIFInfo);};
bool32	SCRIPT::CheckPrintSBICinfo	(void){return(BIF_BASE::CheckEnvFlag(&cgENV, BIF_ENV::ENV_blPrintBIFInfo));};

void	SCRIPT::SetblCommandExplain	(void){BIF_BASE::SetEnvFlag(&cgENV, BIF_ENV::ENV_blPrintBIFExplain);};
void	SCRIPT::ClrblCommandExplain	(void){BIF_BASE::ClrEnvFlag(&cgENV, BIF_ENV::ENV_blPrintBIFExplain);};
bool32	SCRIPT::CheckCommandExplain	(void){return(BIF_BASE::CheckEnvFlag(&cgENV, BIF_ENV::ENV_blPrintBIFExplain));};

void	SCRIPT::SetblATResponse		(void){SetSFlag(SRC_blATResponse);};
void	SCRIPT::ClrblATResponse		(void){ClrSFlag(SRC_blATResponse);};
bool32	SCRIPT::CheckATResponse		(void){return(CheckSFlag(SRC_blATResponse));};
//------------------------------------------------------------------------------------------//
SCRIPT::SCRIPT(uint32 size) : DEVICE_EXE_FRAME(size)
#ifdef Commu_AEXE_h
	,cgAExePool(size,nullptr)
#endif
{
	SetblATResponse();
	SetblPrintSBICinfo();
	
	cgATCondition = TELIT_AT_RESPONSE;
	eGroupListThread.ThreadInit	(this, &SCRIPT::EGroupListThreadFun	,"grouplist");
	eGroupThread.ThreadInit		(this, &SCRIPT::EGroupThreadFun		,"group");
	eCommandThread.ThreadInit	(this, &SCRIPT::ECommandThreadFun	,"command");
	
	BIF_ENV_DEV::InitQuantity(&cgENV);
	BIF_ENV_DEV::Init(&cgENV);
#ifdef Commu_AEXE_h
	BIF_ENV_DEV::SetAExePool(&cgENV, &cgAExePool);
#endif
}
//------------------------------------------------------------------------------------------//
SCRIPT::~SCRIPT(void){
	Stop();
	BIF_ENV_DEV::DeInit(&cgENV);
};
//------------------------------------------------------------------------------------------//
void SCRIPT::Init(const DEVICE* dev){
	DEVICE_EXE_FRAME::Init(dev);
	if (dev != nullptr){
		CMD_ENV::SetSTDOUT(&cgENV, ((DEVICE*)dev)->GetSelSTDOUT());
		BIF_ENV_DEV::SetEDA(&cgENV, (ExpandDeviceAttr*)&dev->cgEDA);
	}
	else{
		CMD_ENV::SetSTDOUT(&cgENV, nullptr);
		BIF_ENV_DEV::SetEDA(&cgENV,nullptr);
	}
}
//------------------------------------------------------------------------------------------//
void SCRIPT::Stop(void){
	CMD_BASE::SetblExit(&cgENV);
#ifdef Commu_AEXE_h
	cgAExePool.Close();
#endif
	DEVICE_EXE_FRAME::Stop();
}
//------------------------------------------------------------------------------------------//
bool32 SCRIPT::Execute(const DEVICE* dev,const GC_LIST* tGroupList){
	if ((tGroupList == nullptr) || IsExecuting(dev) == G_FALSE)
		return G_FALSE;
	GC_LIST::CopyCOMMAND_GROUP_ENABLE(&cgMGroupList,tGroupList);
	return(DEVICE_EXE_FRAME::Execute(dev,&eGroupListThread));
}
//------------------------------------------------------------------------------------------//
bool32 SCRIPT::Execute(const DEVICE* dev,const COMMAND_GROUP* tGroup){
	if ((tGroup == nullptr) || IsExecuting(dev) == G_FALSE)
		return G_FALSE;
	COMMAND_GROUP::CopyCOMMAND_GROUP(&cgMGroup,tGroup);
	return(DEVICE_EXE_FRAME::Execute(dev,&eGroupThread));
}
//------------------------------------------------------------------------------------------//
bool32 SCRIPT::Execute(const DEVICE* dev,const COMMAND_NODE* tCommand){
	if ((tCommand == nullptr) || IsExecuting(dev) == G_FALSE)
		return G_FALSE;
	COMMAND_NODE::CopyCOMMAND_NODE(&cgMCommand,tCommand);
	return(DEVICE_EXE_FRAME::Execute(dev,&eCommandThread));
}
//------------------------------------------------------------------------------------------//
bool32 SCRIPT::EGroupListThreadFun(void* p){
	int32	runTotalTimes;
	DTIME	executeTime,startTime;
	
	startTime.Now();
	runTotalTimes = 0;
	cgMGroupList.ClearTestResult();
	
	do{
		++ runTotalTimes;
		ExecuteGroupList(&cgMGroupList,runTotalTimes);
	}while(IsTerminated() == G_FALSE);
	
	executeTime.Now();
	executeTime -= startTime;
	ShowResult(&cgMGroupList,SYS_FormatTimeABS(DEFAULT_TIMEFORMATE,executeTime));
	cgMGroupList.ClearTestResult();
	cgMGroupList.Empty();
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
bool32 SCRIPT::EGroupThreadFun(void* p){
	int32	runTotalTimes;
	DTIME	executeTime,startTime;
	
	startTime.Now();
	runTotalTimes = 0;
	cgMGroup.ClearResult();
	
	ExecuteGroup(&cgMGroup,runTotalTimes,1);
	
	executeTime.Now();
	executeTime -= startTime;
	ShowResult(&cgMGroup,SYS_FormatTimeABS(DEFAULT_TIMEFORMATE,executeTime));
	cgMGroup.ClearResult();
	CleanChild(&cgMGroup, &cgMGroup);
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
bool32 SCRIPT::ECommandThreadFun(void* p){
	DTIME	executeTime,startTime;

	startTime.Now();
	
	ExecuteCommand(&cgMCommand,100);
	
	executeTime.Now();
	executeTime -= startTime;
	
	*CMD_ENV::GetSTDOUT(&cgENV) << Begin() << NL()
	<< COL_DivLine_Maroon
	<< COL_NormalMessage << "Execute Time: " << SYS_FormatTimeABS(DEFAULT_TIMEFORMATE,executeTime)
	<< "\n\nEnd\n" << Endl();
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
bool32 SCRIPT::ExecuteGroupList(GC_LIST* tGroupList,const int32 &runTotalTimes){
	TREE_LChildRChain_Traversal_LINE_nolock(COMMAND_GROUP,tGroupList,
		if (IsTerminated())
			break;
		if (_opNode->blEnableAutoRun)
			ExecuteGroup(_opNode,runTotalTimes,G_FALSE);
	);
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
bool32 SCRIPT::ExecuteGroup(COMMAND_GROUP* tGroup,const int32& runTotalTimes,bool32 blType){
	int32			blRun,frameTimeout,exitType,cycleTimes,runTimes;
	SYS_TIME_S		timeS;

	blRun = 0;
	exitType = 0;
	runTimes = tGroup->autoRunTimes;
	if ((blType) && (runTimes == 0)){
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
				*CMD_ENV::GetSTDOUT(&cgENV) << Begin() << NL()
				<< COL_DivLine_Maroon
				<< COL_NormalMessage << "Loop: " << Str_ToStr(runTotalTimes) << "::" << tGroup->name << "::Cycle: " << Str_ToStr(cycleTimes)
				<< "\n\nStart\n"
				<< Endl();
			}
		}
		BIF_ENV::RetDefine(&cgENV) = "";
		TREE_LChildRChain_Traversal_LINE_nolock(COMMAND_NODE,tGroup,
			if (IsTerminated())
				break;
				frameTimeout = tGroup->intervalTime;
			if (frameTimeout < 1)
				frameTimeout = 200;
			SYS_Delay_SetTS(&timeS,frameTimeout);
			exitType = ExecuteCommand(_opNode,frameTimeout);
			if (_opNode != BIF_ENV::GetCommandNode(&cgENV))
				_nextNode = BIF_ENV::GetCommandNode(&cgENV);//used for goto cmd
			if (exitType == 6){//stop
				CMD_BASE::DelayMS(&cgENV,50);
				break;
			}
			if (exitType == 3){//break
				CMD_BASE::DelayMS(&cgENV,50);
					break;
			}
			if (exitType == 2)//continue
				SYS_Delay_SetTS(&timeS,frameTimeout);
				if (exitType > 1)
					while((SYS_Delay_CheckTS(&timeS) == 0) && (IsTerminated() == 0));
												);
		if ((runTimes != 1) || ((runTimes == 1) && (blType == G_FALSE))){
			if (CheckPrintSBICinfo() != 0){
				*CMD_ENV::GetSTDOUT(&cgENV) << Begin() << NL()
				<< COL_DivLine_Maroon
				<< COL_NormalMessage << "Loop: " << Str_ToStr(runTotalTimes) << "::" << tGroup->name << "::Cycle: " << Str_ToStr(cycleTimes)
				<< "\n\nEnd\n"
				<< Endl();
			}
		}
		if (exitType == 6)//stop
			return G_TRUE;
	}while((blRun || (cycleTimes < runTimes)) && (IsTerminated() == 0));
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
bool32 SCRIPT::ExecuteCommand(COMMAND_NODE* tCommand,int32 frameTimeout){
	STDSTR		commandStrRet,strOCommand,strTemp;
	int32		eRetCode;
	bool32		blEnResend;	//check meet execute resend contidion
	int32		maxCycle,i,timeout;
	BIF_RETSTR	retStr;
	
	//return(0)--->no execute due to blEnableSend == 0.
	//return(1)--->execute BIC,no need to wait.
	//return(2)--->meet continue condition.
	//return(3)--->meet break condition.
	//return(4)--->meet resend condition.
	//return(5)--->cycle-- to 0
	//return(6)--->meet stop condition.
	
	BIF_ENV::SetCommandNode(&cgENV, tCommand);
	BIF_ENV::RetCMD(&cgENV) = G_TRUE;
	
	if (tCommand->blEnableSend){
		maxCycle = 1;//set cycle times.
		if (tCommand->StrCycle.length() > 0)
			maxCycle = atoi(tCommand->StrCycle.c_str());
		i = 0;
		strOCommand = CMD_BASE::DelComment(tCommand->StrCommand);
		retStr.result = "";
		retStr.forPrint = "";
		retStr.forSend = "";
		if (BIF_ID_COMBINE_REPLACE == cgSubC_Replace.Dispose(&cgENV, strOCommand, &retStr))
			strOCommand = retStr.forSend;
		while(((i++ < maxCycle) || (maxCycle == 0)) && (IsTerminated() == G_FALSE)){
			eRetCode = cgBIFCMD.Dispose(&cgENV,strOCommand,nullptr);
			if (eRetCode != BIF_ID_NO)
				return(BIF_ENV::RetCMD(&cgENV));

			BIF_Transform(&cgENV,_EMPTY(&retStr.forSend),_EMPTY(&retStr.forPrint),strOCommand,tCommand->cmdTail,G_ESCAPE_ON);
			if (retStr.forSend.length() == 0)
				break;
				
			cgSBUF.Empty();
			BIF_ENV_DEV::GetEDA(&cgENV)->device->PrintSendCommand((uint32)retStr.forSend.length(),retStr.forPrint);
			BIF_ENV_DEV::GetEDA(&cgENV)->device->SSend(retStr.forSend);
			
			
			++ tCommand->runTimes;
			timeout = 0;
			SYS_Delay_SetTS(&BIF_ENV::RetDTime(&cgENV),atof(tCommand->StrTimeout.c_str()) * 1000);
			
			blEnResend = G_FALSE;
			BIF_ENV::STDIN(&cgENV) = "";
			while(timeout == 0 && IsTerminated() == 0){
				cgSBUF.Get(nullptr, &BIF_ENV::STDIN(&cgENV), -1);
				if ((tCommand->StrCatch.length() > 0) && (BIF_Expression(&cgENV,tCommand->StrCatch)))
					++ tCommand->catchTimes;
				if ((tCommand->StrContinue.length() > 0) && (BIF_Expression(&cgENV,tCommand->StrContinue)))
					return 2;
				if ((tCommand->StrResend.length() > 0) && (BIF_Expression(&cgENV,tCommand->StrResend))){
					blEnResend = G_TRUE;
					timeout = SYS_Delay_CheckTS(&BIF_ENV::RetDTime(&cgENV));
					break;
				}
				if ((tCommand->StrStop.length() > 0) && (BIF_Expression(&cgENV,tCommand->StrStop)))
					return 3;
				if ((tCommand->StrContinue.length() == 0)
					&& (tCommand->StrResend.length() == 0)
					&& (tCommand->StrStop.length() == 0)
					&& (CheckATResponse())
					&& (BIF_Expression(&cgENV,cgATCondition))){
					return 2;
				}
				timeout = SYS_Delay_CheckTS(&BIF_ENV::RetDTime(&cgENV));
			}
			cgSBUF.Get(nullptr, &BIF_ENV::STDIN(&cgENV), -1);
			if (timeout != 0){
				if (BIF_ENV::STDIN(&cgENV).length() == 0)
					++ tCommand->timeoutTimes;//no response
				if ((tCommand->StrContinue.length() > 0) && (BIF_Expression(&cgENV,tCommand->StrContinue)))
					return 2;
				if ((tCommand->StrResend.length() > 0) && (BIF_Expression(&cgENV,tCommand->StrResend))){
					blEnResend = G_TRUE;
				}
				else if ((tCommand->StrStop.length() > 0) && (BIF_Expression(&cgENV,tCommand->StrStop))){
					return 3;
				}
				if ((BIF_ENV::STDIN(&cgENV).length() > 0) && (tCommand->StrResend.length() > 0) && (blEnResend == G_FALSE))
					return 4;//receive some data, but no match resend condition,do next command
			}
			if ((blEnResend) && (timeout == 0))
				CMD_BASE::DelayMS(&cgENV,frameTimeout);
		}
		return 5;
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
void SCRIPT::PrintGroupResult(OUTPUT_NODE* cOut,COMMAND_GROUP* tGroup,const STDSTR& strGroupName){
	STDSTR		strCommand;
	int32		i;
	bool32		blGT;
	i = 0;
	blGT = G_FALSE;
	TREE_LChildRChain_Traversal_LINE_nolock(COMMAND_NODE,tGroup,
		++ i;
		if (_opNode->catchTimes > 0){
			strCommand = CMD_BASE::DelComment(_opNode->StrCommand);
			if (strCommand.length() > 24){
				strCommand = strCommand.substr(0,20) + " ...";
			}
			else{
				strCommand.insert(strCommand.length(),24 - strCommand.length(),' ');
			}
			*cOut << NL() << COL_clBlue;
			if (blGT == G_FALSE){
				*cOut << strGroupName;
				blGT = G_TRUE;
			}
			*cOut << "  N" << Str_ToStr(i) << "::" << strCommand
			<< COL_clBlue << ",catch(" << _opNode->StrCatch << "):" << Str_ToStr(_opNode->catchTimes) << "/" <<Str_ToStr(_opNode->runTimes);
	
			if (_opNode->timeoutTimes > 0){
				*cOut << COL_clRed << ",no response:" << Str_ToStr(_opNode->timeoutTimes) << "/" << Str_ToStr(_opNode->runTimes);
			}
		}
		else if (_opNode->timeoutTimes > 0){
			strCommand = CMD_BASE::DelComment(_opNode->StrCommand);
			if (strCommand.length() > 24){
				strCommand = strCommand.substr(0,20) + " ...";
			}
			else{
				strCommand.insert(strCommand.length(),24 - strCommand.length(),' ');
			}
			*cOut << NL() << COL_clBlue;
			if (blGT == G_FALSE){
				*cOut << strGroupName;
				blGT = G_TRUE;
			}
			*cOut << "  N" << Str_ToStr(i) << "::" << strCommand
			<< COL_clRed << ",no response:" << Str_ToStr(_opNode->timeoutTimes) << "/" << Str_ToStr(_opNode->runTimes);
		}
	);
}
//------------------------------------------------------------------------------------------//
void SCRIPT::ShowResult(COMMAND_GROUP* tGroup,const STDSTR& tExecuteTime){
	STDSTR	strGroupName;

	*CMD_ENV::GetSTDOUT(&cgENV) << Begin() << NL()
	<< COL_DivLine_Maroon
	<< COL_NormalMessage << "Execute Time: " << tExecuteTime;
	
	if (tGroup->autoRunTimes > 1){
		strGroupName = "G::" + tGroup->name + ":\n";
		PrintGroupResult(CMD_ENV::GetSTDOUT(&cgENV),tGroup,strGroupName);
	}
	*CMD_ENV::GetSTDOUT(&cgENV) << COL_NormalMessage << "\n\nEnd\n" << Endl();
}
//------------------------------------------------------------------------------------------//
void SCRIPT::ShowResult(GC_LIST* tGroupList,const STDSTR& tExecuteTime){
	STDSTR	strGroupName;
	int32	i;

	*CMD_ENV::GetSTDOUT(&cgENV) << Begin() << NL()
	<< COL_DivLine_Maroon
	<< COL_NormalMessage << "Execute Time: " << tExecuteTime;
	i = 0;
	TREE_LChildRChain_Traversal_LINE_nolock(COMMAND_GROUP,tGroupList,
		++ i;
		if (_opNode->blEnableAutoRun){
			strGroupName = "G" + Str_ToStr(i) + "::" + _opNode->name + ":\n";
			PrintGroupResult(CMD_ENV::GetSTDOUT(&cgENV),_opNode,strGroupName);
		}
	);
	*CMD_ENV::GetSTDOUT(&cgENV) << COL_NormalMessage << "\n\nEnd\n" << Endl();
}
//------------------------------------------------------------------------------------------//
void SCRIPT::Help(OUTPUT_NODE* oDevNode){
	CMD_ENV		env;

	if (oDevNode == nullptr)
		return;
	
	env.InitQuantity(CMD_ENV::CMD_VID_NEXT);

	CMD_ENV::SetSTDOUT(&env, oDevNode);

	cgBIFCMD.Help(&env,0);
}
//------------------------------------------------------------------------------------------//
#endif /* Script_h */
