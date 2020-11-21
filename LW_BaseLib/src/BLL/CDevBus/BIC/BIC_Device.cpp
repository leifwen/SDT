//
//  BIC_Device.cpp
//  SDT
//
//  Created by Leif Wen on 14/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "BIC_Device.h"
#include "SYS_File.h"
#include "ALG_Math.h"
#ifdef BIC_Device_h
//------------------------------------------------------------------------------------------//
#ifdef SList_h
//------------------------------------------------------------------------------------------//
CMDID BIC_SC_SEND::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Send command by sID if connected");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[sID]"		,"Command sID");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_SC_SEND::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	ExpandDevBusAttr* attr = BIC_ENV_DEV::GetEDA(env);
	SC_NODE		*nextNode;
	STDSTR		StrCommand;
	int32		blEnableSendCR;
	uint32		sDID;
	
	StrCommand = "";
	nextNode = nullptr;
	if (attr->IsOpened()){
		if (msg.length() > 0){
			sDID = (uint32)strtol(msg.c_str(),nullptr,10);
			nextNode = (SC_NODE*)FindInDownChainByDRNodeID(BIC_ENV::GetSCList(env), sDID);
			if (nextNode != nullptr){
				nextNode->rwLock.R_set();
				StrCommand = nextNode->StrCommand;
				blEnableSendCR = nextNode->blEnableSendCR;
				nextNode->rwLock.R_clr();
				PrintEnable(env);
				attr->cdevbus->SendCommandWithPrint(StrCommand,(CMD_TAIL)blEnableSendCR,G_ESCAPE_ON);
				BIC_CONNECT::PressAnyKey(env,attr,nullptr);
				PrintSuccess(env);
			}
			else{
				PrintFail(env);
			}
		}
	}
	else{
		PrintFail(env,"no connect");
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
#endif
#if defined BIC_GCM_h && defined Script_h
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_GROUP_SEND::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Send command by cID if connected");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[cID]"		,"Command cID");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_GROUP_SEND::Command(CMD_ENV* env,const STDSTR& msg,void* cg)const{
	COMMAND_GROUP*	group = static_cast<COMMAND_GROUP*>(cg);
	COMMAND_NODE	command,*node;
	uint32			cDID;
	
	command.Init();
	
	if (msg.length() > 0){
		cDID = (uint32)strtol(msg.c_str(),nullptr,10);
		node = (COMMAND_NODE*)FindInDownChainByDRNodeID(group, cDID);
		if (node != nullptr){
			COMMAND_NODE::CopyCommandNode(&command,node);
			command.blEnableSend = 1;
			PrintEnable(env);
			if (BIC_ENV_DEV::GetScript(env)->Execute(BIC_ENV_DEV::GetEDA(env)->cdevbus,&command) == G_FALSE){
				PrintFail(env,"script is running");
				return(cgCommandID);
			}
			if (IntoPressKeyMode(env) > 0){
				BIC_ENV_DEV::GetScript(env)->Stop();
				PrintSuccess(env,"Pressed ESC key, stopped script");
			}
			return(cgCommandID);
		}
	}
	PrintFail(env);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
bool32 BIC_GC_GROUP_SEND::IsExitPressKeyMode(CMD_ENV* env)const{
	return(BIC_ENV_DEV::GetScript(env)->IsStop());
};
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
CMDID BIC_SEND::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Send data if connected");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
#ifdef Script_h
	PrintHelpSubItem(env,"[-c <n>]"		,"Cycle times,default is endless");
	PrintHelpSubItem(env,"[-t <s>]"		,"Time interval,uint is s, default is 1s");
#endif
	PrintHelpSubItem(env,"[-e]"			,"Escape disable");
	PrintHelpSubItem(env,"[-d]"			,"Data");
	PrintHelpSubItem(env,"<data>"		,"Data");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_SEND::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	ExpandDevBusAttr* attr = BIC_ENV_DEV::GetEDA(env);
	
	STDSTR			strData,strPar1, strPar2;
	G_ESCAPE		escMode = G_ESCAPE_ON;
	bool32			blUseScript = G_FALSE;
#ifdef Script_h
	COMMAND_NODE	commandNode;
#endif
	
	do{
		if (msg.length() == 0)
			break;
		if (!attr->IsOpened()){
			PrintFail(env,"no connect");
			break;
		}
		
		strData = msg;
#ifdef Script_h
		commandNode.Init();
		commandNode.StrTimeout = '1';
		commandNode.StrCycle = '0';
		commandNode.StrResend = "'timeout";
		commandNode.cmdTail = CMD_NONE;
#endif
		SplitPar1(strPar1,strPar2,msg);

		do{
			if (strPar1 == "-e"){
				escMode = G_ESCAPE_OFF;
			}
			else if (strPar1 == "-d"){
				strData = strPar2;
				break;
			}
#ifdef Script_h
			else if (strPar1 == "-c"){
				SplitPar1(&strPar1,&strPar2);
				commandNode.StrCycle = strPar1;
				blUseScript = G_TRUE;
			}
			else if (strPar1 == "-t"){
				SplitPar1(&strPar1,&strPar2);
				commandNode.StrTimeout = strPar1;
				blUseScript = G_TRUE;
			}
#endif
			else{
				break;
			}
			strData = strPar2;
			SplitPar1(&strPar1,&strPar2);
		}while(strPar1.length() > 0);
		
		PrintEnable(env);
		if (escMode == G_ESCAPE_OFF)
			strData = Str_Replace(strData, "\\", "\\\\");
		if (blUseScript == G_FALSE){
			attr->cdevbus->SendCommandWithPrint(strData,CMD_NONE,G_ESCAPE_ON);
			SYS_SleepMS(100);
		}
#ifdef Script_h
		else{
			commandNode.StrCommand = strData;
			if (BIC_ENV_DEV::GetScript(env)->Execute(attr->cdevbus,&commandNode) == G_FALSE){
				PrintFail(env,"script is running");
				break;
			}
			if (IntoPressKeyMode(env) > 0){
				BIC_ENV_DEV::GetScript(env)->Stop();
				PrintSuccess(env,"Pressed ESC key, stopped script");
			}
#endif
		}
	}while(0);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
bool32 BIC_SEND::IsExitPressKeyMode(CMD_ENV* env)const{
#ifdef Script_h
	return(BIC_ENV_DEV::GetScript(env)->IsStop());
#else
	return G_TRUE;
#endif
};
//------------------------------------------------------------------------------------------//
#ifdef SendFile_h
//------------------------------------------------------------------------------------------//
CMDID BIC_SENDFILE::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Send file if connected");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[-b|-k <n>]"	,"Bps/KBps, default is 100KBps");
	PrintHelpSubItem(env,"[-p <n>]"		,"PackageSize, max and default is 2048");
	PrintHelpSubItem(env,"<filename>"	,"File Name");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_SENDFILE::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	ExpandDevBusAttr* attr = BIC_ENV_DEV::GetEDA(env);
	
	STDSTR	strPar1,strPar2,fn;
	uint32	bps = TFileSend::FS_BPS_DEF;
	uint32	packagsSize = TFileSend::PACKAGE_MAX_SIZE;
	
	do{
		if (msg.length() == 0)
			break;

		fn = msg;
		SplitPar1(strPar1,strPar2,msg);
		do{
			if (strPar1 == "-p"){
				SplitPar1(&strPar1,&strPar2);
				packagsSize = ALG_FAOCalc_UINT32(nullptr,strPar1);
			}
			else if (strPar1 == "-b"){
				SplitPar1(&strPar1,&strPar2);
				bps = ALG_FAOCalc_DOUBLE(nullptr,strPar1);
			}
			else if (strPar1 == "-k"){
				SplitPar1(&strPar1,&strPar2);
				bps = ALG_FAOCalc_DOUBLE(nullptr,strPar1) * 1000;
			}
			else{
				break;
			}
			fn = strPar2;
			SplitPar1(&strPar1,&strPar2);
		}while(strPar1.length() > 0);
		fn = Str_SplitSubItem(&fn, ' ');
		
		PrintEnable(env);
		if (CFS_CheckFile(fn) == G_FALSE){
			PrintFail(env,"file does not exist");
		}
		else{
			if (BIC_ENV_DEV::GetFileSend(env)->Execute(attr->cdevbus,fn,CMD_ENV::GetSTDOUT(env),bps,packagsSize) == G_FALSE){
				PrintFail(env,"file is sending");
				return(cgCommandID);
			}
			IntoPressKeyMode(env);
		}
	}while(0);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
bool32 BIC_SENDFILE::IsExitPressKeyMode(CMD_ENV* env)const{
	return(BIC_ENV_DEV::GetFileSend(env)->IsStop());
};
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
CMDID BIC_RECFILE::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Receive file if connected");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[-w]"			,"Overwrite");
	PrintHelpSubItem(env,"[-b]"			,"Receive file size");
	PrintHelpSubItem(env,"<filename>"	,"File Name");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_RECFILE::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	ExpandDevBusAttr* attr = BIC_ENV_DEV::GetEDA(env);
	
	STDSTR	strPar1,strPar2,fn,dir_self,fn_dir;
	std::fstream 	fileStream;
	bool32	blOverwrite = G_FALSE,blprint = G_FALSE,blStartRec = G_FALSE,blreset;
	SBUF	rxSBUF;
	uint32	num;
	uint64	rxBytes = 0,recBytes = 0;
	uint8	chKey;
	SYS_TIME_S	tm100ms,bpsTimeMS;
	double	kBps = 0.0;
	
	rxSBUF.array.InitSize(1024 * 2);
	
	do{
		if (msg.length() == 0)
			break;

		fn = msg;
		SplitPar1(strPar1,strPar2,msg);
		do{
			if (strPar1 == "-w"){
				blOverwrite = G_TRUE;
			}
			else if (strPar1 == "-b"){
				SplitPar1(&strPar1,&strPar2);
				recBytes = ALG_FAOCalc_DOUBLE(nullptr,strPar1);
			}
			else{
				break;
			}
			fn = strPar2;
			SplitPar1(&strPar1,&strPar2);
		}while(strPar1.length() > 0);
		fn = Str_SplitSubItem(&fn, ' ');

		dir_self = CFS_FormatFileName(CFS_GetSelfDIR());
		fn_dir = MSG_DN_FDIR;
		fn = CFS_FormatFileName(fn_dir + "/" + fn);
		fn_dir = CFS_GetDIR(fn);
		fn = CFS_FormatFileName(dir_self + "/" + fn);
	
		do{
			if (CFS_CreateDIRLoop(dir_self, fn_dir) == G_FALSE){
				PrintEnable(env);
				PrintFail(env,"file name is directory");
				break;
			}
			
			if (CFS_CheckFile(fn)){
				if (CFS_CheckDIR(fn)){
					PrintEnable(env);
					PrintFail(env,"file name is directory");
					break;
				}
				if (blOverwrite == G_FALSE){
					PrintEnable(env);
					PrintFail(env,"file exist");
					break;
				}
			}
		
			PrintALine(env,COLOR(COL_clCyan,IUD("In receive file mode:")));
			SetInOnlineMode(env);
			PrintDisable(env);
			do{
				PrintWithTime_noNL(env, "Receiving", COL_clBlue, "0", COL_NormalMessage, "Bytes.");
				
				rxSBUF.array.Reset();
				attr->cdevbus->RxDataShareTo(&rxSBUF);
				fileStream.open(fn.c_str(),std::ios::out|std::ios::trunc|std::ios::binary);
				SYS_StopWatch_Start(&tm100ms);
				blStartRec = G_FALSE;
				blreset = G_FALSE;
				rxBytes = 0;
				do{
					SYS_SleepMS(1);
					chKey = ReadChar(env,G_FALSE);
					if (chKey == 27)
						break;
					if (chKey == 'r'){
						blreset = G_TRUE;
						CleanLastLine(env);
						break;
					}
					
					num = rxSBUF.array.Used();
					if ((recBytes > 0) && (rxBytes + num > recBytes))
						num = uint32(recBytes - rxBytes);

					if (num > 0){
						if (blStartRec == G_FALSE)
							SYS_StopWatch_Start(&bpsTimeMS);
						blStartRec = G_TRUE;
						strPar1 = "";
						rxSBUF.array.Read(&strPar1, num);
						fileStream << strPar1;
						rxSBUF.array.Out(num);
						rxBytes += num;
						blprint = G_TRUE;
					}
					if (blprint != G_FALSE){
						SYS_StopWatch_Stop(&tm100ms);
						if (tm100ms.timeMS > 100){
							CleanLastLine(env);
							kBps = SYS_StopWatch_Stop(&bpsTimeMS) + 1;
							kBps = rxBytes / kBps;
							PrintWithTime_noNL(env, "Receiving", COL_clBlue, Str_ToStr(rxBytes), COL_NormalMessage, "Bytes,"
											   ,Str_FloatToStr(kBps),"KBps.");
							SYS_StopWatch_Start(&tm100ms);
							blprint = G_FALSE;
						}
					}
					if ((recBytes > 0) && (rxBytes >= recBytes))
						break;
				}while((IsExit(env) == G_FALSE) && attr->IsOpened());
				fileStream.flush();
				fileStream.close();
				rxSBUF.RemoveSelf();
			}while(blreset == G_TRUE);
			PrintALine(env,COL_clBlue, "MD5:" , COL_clCyan, Str_ASCIIToHEX(ALG_Digest_MD5(IUD_FILE(fn)),G_ESCAPE_OFF));
			PrintSuccess(env);
			SYS_SleepMS(10);
			PrintEnable(env);
			PrintStr(env,"\n");
			ClrInOnlineMode(env);
		}while(0);
	}while(0);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_STOP::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Stop script execution");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_STOP::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	if (msg.length() == 0){
		PrintEnable(env);
#ifdef Script_h
		BIC_ENV_DEV::GetScript(env)->Stop();
#endif
#ifdef SendFile_h
		BIC_ENV_DEV::GetFileSend(env)->Stop();
#endif
		PrintSuccess(env,"Script is stopped");
		return(cgCommandID);
	}
	return(Help(env,0));
};
//------------------------------------------------------------------------------------------//
#ifdef Script_h
//------------------------------------------------------------------------------------------//
CMDID BIC_RUN::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Execute group command script");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[-p"		,"Prevent automatic stop");
	PrintHelpSubItem(env,"[gID"		,"Execute group by gID");
	PrintHelpSubItem(env," [ct]]"	,"ct: Cycle times");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_RUN::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	ExpandDevBusAttr* attr = BIC_ENV_DEV::GetEDA(env);
	
	uint32			num;
	STDSTR			strPar1,strPar2;
	COMMAND_GROUP	*group;
	bool32			pas = G_FALSE;
	
	if (msg.length() == 0){
		PrintEnable(env);
		if (BIC_ENV_DEV::GetScript(env)->Execute(attr->cdevbus,BIC_ENV::GetGCList(env)) == G_FALSE){
			PrintFail(env,"script is running");
			return(cgCommandID);
		}
	}
	else{
		SplitPar1(strPar1,strPar2,msg);
		if (strPar1 == "-p"){
			SplitPar1(&strPar1,&strPar2);
			pas = G_TRUE;
		}
		num = (uint32)strtol(strPar1.c_str(),nullptr,10);
		
		group = (COMMAND_GROUP*)FindInDownChainByDRNodeID(BIC_ENV::GetGCList(env), num);
		if (group != nullptr){
			group->autoRunTimes = 1;
			if (strPar2.length() > 0)
				group->autoRunTimes = atoi(strPar2.c_str());
			PrintEnable(env);
			
			if (BIC_ENV_DEV::GetScript(env)->Execute(attr->cdevbus,group) == G_FALSE){
				PrintFail(env,"script is running");
				return(cgCommandID);
			}
			if (pas != G_FALSE)
				BIC_ENV_DEV::GetScript(env)->ClrblStopSelf();
		}
		else{
			PrintFail(env);
			return(cgCommandID);
		}
	}
	if (pas == G_TRUE){
		while(BIC_ENV_DEV::GetScript(env)->IsStop() == G_FALSE)
			SYS_SleepMS(10);
	}
	IntoPressKeyMode(env);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
bool32 BIC_RUN::IsExitPressKeyMode(CMD_ENV* env)const{
	return(BIC_ENV_DEV::GetScript(env)->IsStop());
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_SCRIPT::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Set script parameter");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env, "[-l"					, "List Script Build-in Command");
	PrintHelpSubItem(env, " |[-A <on|off>]"		, "Check standard AT response");
	PrintHelpSubItem(env, "  [-E <on|off>]"		, "Display script BIC execution");
	PrintHelpSubItem(env, "  [-CE <on|off>]] "	, "Display script BIC explain report");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_SCRIPT::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	bool32	blcheck;
	STDSTR	strCommand, strPar1, strPar2;
	
	strCommand = Str_Trim(msg);
	
	blcheck = G_FALSE;
	if (msg == "-l"){
		BIC_ENV_DEV::GetScript(env)->Help(GetSTDOUT(env));
		return(cgCommandID);
	}
	SplitPar1(strPar1, strPar2, msg);
	do{
		if (strPar1 == "-A"){
			SplitPar1(&strPar1,&strPar2);
			if (strPar1 == "on"){
				BIC_ENV_DEV::GetScript(env)->SetblATResponse();
				blcheck = G_TRUE;
			}
			else if (strPar1 == "off"){
				BIC_ENV_DEV::GetScript(env)->ClrblATResponse();
				blcheck = G_TRUE;
			}
		}
		else if (strPar1 == "-E"){
			SplitPar1(&strPar1,&strPar2);
			if (strPar1 == "on"){
				BIC_ENV_DEV::GetScript(env)->SetblPrintSBICinfo();
				blcheck = G_TRUE;
			}
			else if (strPar1 == "off"){
				BIC_ENV_DEV::GetScript(env)->ClrblPrintSBICinfo();
				blcheck = G_TRUE;
			}
		}
		else if (strPar1 == "-CE"){
			SplitPar1(&strPar1,&strPar2);
			if (strPar1 == "on"){
				BIC_ENV_DEV::GetScript(env)->SetblCommandExplain();
				blcheck = G_TRUE;
			}
			else if (strPar1 == "off"){
				BIC_ENV_DEV::GetScript(env)->ClrblCommandExplain();
				blcheck = G_TRUE;
			}
		}
		SplitPar1(&strPar1,&strPar2);
	}while(strPar1.length() > 0);
	
	if ((msg.length() > 0) && (blcheck == G_FALSE))
		return(Help(env,0));
	
	PrintResult(env
				,((BIC_ENV_DEV::GetScript(env)->CheckATResponse() == G_FALSE)?"Disable":"Enable"),"check AT,"
				,((BIC_ENV_DEV::GetScript(env)->CheckPrintSBICinfo() == G_FALSE)?"disable":"enable"),"show script BIC excution,"
				,((BIC_ENV_DEV::GetScript(env)->CheckCommandExplain() == G_FALSE)?"disable":"enable"),"show script BIC explain");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
#endif










//------------------------------------------------------------------------------------------//
BIC_CONN::BIC_CONN(void) : BIC_BASE_S() {
	SetSFlag(CMD_blTrySubCMD);
	cgCommandID = BIC_ID_CONN;
	cgCommand = "conn";
	cgConsoleName = cgCommand;
	cgHelpName = "Connection";
	
	AppendDown(cgC_MAIN) < cgC_SECOND
#ifdef SWVERSION_AUXDEVICE
	< cgC_AUX
#endif
	< cgC_CONNECT < cgC_ONLINE < cgC_DISCONNECT < cgC_CR < cgC_ECHO < cgC_REC < cgC_DEV
#ifdef SWVERSION_COM
	< cgC_ACOM
#endif
#if (defined SWVERSION_PORT) || (defined SWVERSION_Server)
	< cgC_ASOCKET
#endif
#ifdef SWVERSION_BT
	< cgC_BT
#endif
#ifdef SWVERSION_BLE
	< cgC_BLE
#endif
	< cgC_SEND
#ifdef SendFile_h
	< cgC_SENDFILE
#endif
	< cgC_RECFILE
#ifdef SWVERSION_SCRIPT
	< cgC_RUN
#endif
#if defined SendFile_h || defined SWVERSION_SCRIPT
	 < cgC_STOP
#endif
#ifdef SWVERSION_SCRIPT
	< cgC_SCRIPT
#endif
	;
};
//------------------------------------------------------------------------------------------//
CMDID BIC_CONN::MessageProcessing(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* p)const{
	return(BIC_BASE_S::MessageProcessing(env,mID,msg,BIC_ENV_DEV::GetEDA(env)));
};
//------------------------------------------------------------------------------------------//
#endif /* BIC_Device_h */
