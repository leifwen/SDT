//
//  BIC_SCM.cpp
//  SDT
//
//  Created by Leif Wen on 19/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "BIC_SCM.h"
#ifdef BIC_SCM_h
#include "SYS_File.h"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_SC_EG::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Single command record example");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_SC_EG::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	STDSTR		strData;
	
	PrintALine(env
			   ,COLOR(COL_clDefault,IUD(DEV_LINE_STAR))
			   ,COL_clYellow
			   ,"[scNode]\n"
			   ,"  blEnableCR =\n"
			   ,"  strCommand =\n"
			   ,"[scNode_end]\n"
			   ,COLOR(COL_clDefault,IUD(DEV_LINE_STAR)));
	PrintSuccess(env);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_SC_LOAD::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Load single command from file");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[filename]","Filename. Default is <default.ini>");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_SC_LOAD::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	STDSTR	fileName;
	
	fileName = msg;
	fileName = Str_SplitSubItem(&fileName, ' ');
	
	if (msg.length() == 0){
		fileName += DEFAULT_INI_FILE;
		fileName = CFS_FormatFileName(CFS_GetWorkDIR() + "/" + fileName);
	}

	if (CFS_CheckFile(fileName)){
		BIC_ENV::GetSCList(env)->Empty();
		BIC_ENV::GetSCList(env)->Load(fileName);
		TNF::SetblUpdate(BIC_ENV::GetSCList(env));
		PrintSuccess(env,"Read from <" + fileName + ">");
	}
	else{
		PrintFail(env);
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_SC_SAVE::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Save to file");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[filename]","Filename");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_SC_SAVE::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	STDSTR	fileName;
	fileName = msg;
	fileName = Str_SplitSubItem(&fileName, ' ');

	if (msg.length() > 0){
		BIC_ENV::GetSCList(env)->Save(fileName);
		PrintSuccess(env,"Save to <" + fileName + ">");
	}
	else{
		PrintFail(env);
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_SC_LS::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "List single command");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_SC_LS::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	int32		j;
	STDSTR		strPrintData;
	SC_NODE		*fromNode,*breakNode;
	uint32		fromNodeDRID;
	bool32		blSpace;
	
	PrintALine(env,COLOR(COL_clDefault,IUD(DEV_LINE_STAR)), SC_NODE::GetTitle(&strPrintData));
	
	j = 0;
	
	fromNode =nullptr;
	breakNode = nullptr;
	fromNodeDRID = 0;
	blSpace = G_FALSE;
	do{
		if (breakNode == nullptr){
			fromNode = (SC_NODE*)GetDown(BIC_ENV::GetSCList(env));
		}
		else{
			breakNode = nullptr;
			fromNode = (SC_NODE*)FindInDownChainByDRNodeID(BIC_ENV::GetSCList(env), fromNodeDRID);
		}
		TREE_NextChain_Traversal_LINE(SC_NODE,fromNode,
			PrintStrNL(env,COLOR(COL_clDefault,&_opNode->Compose(&strPrintData)));
			if ((IsExit(env) == G_FALSE) && (++j > 19) && (_nextNode != nullptr)){
				breakNode = _nextNode;
				fromNodeDRID = GetDRNodeID(breakNode);
				break;
			}
		);
		if ((IsExit(env) == G_FALSE) && (blSpace == G_FALSE) && (j > 19) && (breakNode != nullptr)){
			uint8 retChar;
			j = 0;
			retChar = PrintPressAnyKey(env);
			if (retChar == 27){
				break;
			}
			else if (retChar == ' '){
				blSpace = G_TRUE;;
			}
		}
	}while(breakNode != nullptr);
	
	PrintStrNL(env,COLOR(COL_clDefault,IUD(DEV_LINE_STAR)));
	PrintSuccess(env);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_SC_SET::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Set command");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[+cr]"	,"Enable send \"\\r\" in the end");
	PrintHelpSubItem(env,"[-cr]"	,"Disable send \"\\r\" in the end");
	PrintHelpSubItem(env,"<sID>"	,"Command sID, all");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_SC_SET::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	STDSTR		strPar1,strPar2,strPrintData;
	uint32		sID;
	bool32		blCR;
	SC_NODE		*selectNode;
	bool32		blupdate;
	
	blCR = G_FALSE;
	
	if (msg.length() == 0){
		PrintFail(env);
		return(cgCommandID);
	}
	PrintResult(env,COL_clCyan,SC_NODE::GetTitle(&strPrintData));
	
	SplitPar1(strPar1, strPar2, msg);
	blupdate = 0;
	while(strPar1.length() > 0){
		if (strPar1 == "+cr"){
			blCR = G_TRUE;
		}
		else if (strPar1 == "-cr"){
			blCR = G_FALSE;
		}
		else if (strPar1 == "all"){
			TREE_DownChain_Traversal_LINE(SC_NODE,BIC_ENV::GetSCList(env),
				_opNode->rwLock.W_set();
				_opNode->blEnableSendCR = blCR;
				_opNode->rwLock.W_clr();
				PrintResult(env,COLOR(COL_clBlue,_opNode->Compose(&strPrintData)));
			);
			blupdate = G_TRUE;
		}
		else{
			sID = (uint32)strtol(strPar1.c_str(),nullptr,10);
			selectNode = (SC_NODE*)FindInDownChainByDRNodeID(BIC_ENV::GetSCList(env), sID);
			if (selectNode != nullptr){
				selectNode->rwLock.W_set();
				selectNode->blEnableSendCR = blCR;
				selectNode->rwLock.W_clr();
				PrintResult(env,COLOR(COL_clBlue,selectNode->Compose(&strPrintData)));
				blupdate = G_TRUE;
			}
		}
		SplitPar1(&strPar1, &strPar2);
	}
	if ((blupdate == G_TRUE) && (BIC_ENV::GetSCList(env) != nullptr))
		TNF::SetblUpdate(BIC_ENV::GetSCList(env));
	PrintSuccess(env);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_SC_CLONE::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Clone command by sID");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[sID]","Command sID");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_SC_CLONE::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	SC_NODE		*nextNode,*newNode;
	uint32		sDID;
	STDSTR		strPrintData;
	
	if (BIC_ENV::GetSCList(env) != nullptr){
		newNode = (SC_NODE*)BIC_ENV::GetSCList(env)->GetNewNode();
		if (newNode == nullptr){
			PrintFail(env);
			return(cgCommandID);
		}
		
		newNode->StrCommand = "new command";
		if (msg.length() > 0){
			sDID = (uint32)strtol(msg.c_str(),nullptr,10);
			nextNode = (SC_NODE*)FindInDownChainByDRNodeID(BIC_ENV::GetSCList(env), sDID);
			if (nextNode != nullptr)
				SC_NODE::Copy(newNode,nextNode);
		}
		BIC_ENV::GetSCList(env)->AppendDownNode(newNode);
		PrintResult(env,COLOR(COL_clBlue,newNode->Compose(&strPrintData)));
		SetblUpdate(BIC_ENV::GetSCList(env));
		PrintSuccess(env);
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_SC_DEL::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Delete command by sID");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[sID]","Command sID");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_SC_DEL::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	STDSTR		strPar1,strPar2;
	uint32		sDID;
	TNF			*node;
	
	if (BIC_ENV::GetSCList(env) != nullptr){
		if (msg.length() == 0){
			PrintFail(env);
			return(cgCommandID);
		}
		SplitPar1(strPar1, strPar2, msg);
		if (strPar1 == "all"){
			BIC_ENV::GetSCList(env)->Empty();
			SetblUpdate(BIC_ENV::GetSCList(env));
			PrintResult(env,"Delete sID:all");
		}
		else{
			while(strPar1.length() > 0){
				sDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
				node = FindInDownChainByDRNodeID(BIC_ENV::GetSCList(env), sDID);
				BIC_ENV::GetSCList(env)->MoveToTrash(node,node);
				PrintResult(env,"Delete sID:",Str_ToStr(sDID));
				SplitPar1(&strPar1, &strPar2);
			};
			SetblUpdate(BIC_ENV::GetSCList(env));
		}
		PrintSuccess(env);
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_SC_MVUP::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Move command up");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[sID]"	,"Command sID");
	PrintHelpSubItem(env,"[step]"	,"Default value is 1");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_SC_MVUP::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	SC_NODE		*nextNode;
	STDSTR		strPar1,strPar2;
	uint32		sDID,step;
	
	if (BIC_ENV::GetSCList(env) != nullptr){
		if (msg.length() > 0){
			SplitPar1(strPar1, strPar2, msg);
			sDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
			step = (uint32)strtol(strPar2.c_str(),nullptr,10);
			if (step == 0)
				step = 1;
			nextNode = (SC_NODE*)FindInDownChainByDRNodeID(BIC_ENV::GetSCList(env), sDID);
			if (nextNode != nullptr){
				while(step-- > 0)
					MovePrior(nextNode,nextNode);
				SetblUpdate(BIC_ENV::GetSCList(env));
				PrintSuccess(env);
				return(cgCommandID);
			}
		}
		PrintFail(env);
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_SC_MVDN::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Move command down");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[sID]"	,"Command sID");
	PrintHelpSubItem(env,"[step]"	,"Default value is 1");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_SC_MVDN::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	SC_NODE		*nextNode;
	STDSTR		strPar1,strPar2;
	uint32		sDID,step;
	
	if (BIC_ENV::GetSCList(env) != nullptr){
		if (msg.length() > 0){
			SplitPar1(strPar1, strPar2, msg);
			sDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
			step = (uint32)strtol(strPar2.c_str(),nullptr,10);
			if (step == 0)
				step = 1;
			nextNode = (SC_NODE*)FindInDownChainByDRNodeID(BIC_ENV::GetSCList(env), sDID);
			if (nextNode != nullptr){
				while(step-- > 0)
					MoveNext(nextNode,nextNode);
				TNF::SetblUpdate(BIC_ENV::GetSCList(env));
				PrintSuccess(env);
				return(cgCommandID);
			}
		}
		PrintFail(env);
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_SC_COMMAND::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Set command content by sID");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[sID]"		,"Command sID");
	PrintHelpSubItem(env,"<content>"	,"Content");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_SC_COMMAND::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	SC_NODE		*nextNode;
	STDSTR		strPar1,strPar2,strPrintData;
	uint32		sDID;
	
	if (BIC_ENV::GetSCList(env) != nullptr){
		nextNode = nullptr;
		if (msg.length() > 0){
			SplitPar1(strPar1, strPar2, msg);
			sDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
			nextNode = (SC_NODE*)FindInDownChainByDRNodeID(BIC_ENV::GetSCList(env), sDID);
			if (nextNode != nullptr){
				nextNode->rwLock.W_set();
				nextNode->StrCommand = strPar2;
				nextNode->rwLock.W_clr();
				PrintResult(env,COLOR(COL_clBlue,nextNode->Compose(&strPrintData)));
				SetblUpdate(BIC_ENV::GetSCList(env));
				PrintSuccess(env);
				return(cgCommandID);
			}
		}
		PrintFail(env);
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
BIC_SCM::BIC_SCM(void) : BIC_BASE_S(){
	cgCommandID = BIC_ID_CALC;
	cgCommand = "scm";
	cgConsoleName = cgCommand;
	cgHelpName = "SCM";
	
	AppendDown(cgSub_eg)
	< cgSub_load < cgSub_save
	< cgSub_ls < cgSub_set < cgSub_clone < cgSub_del
	< cgSub_mvup < cgSub_mvdn
	< cgSub_command;
};
//------------------------------------------------------------------------------------------//
#endif /* BIC_SCM_h */
