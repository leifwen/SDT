//
//  BIC_GCM.cpp
//  SDT
//
//  Created by Leif Wen on 19/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "BIC_GCM.h"
#ifdef BIC_GCM_h
#include "SYS_File.h"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_EG::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Group record example");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_EG::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	STDSTR		strData;
	
	PrintALine(env
			   ,COLOR(COL_clDefault,IUD(DEV_LINE_STAR))
			   ,COL_clYellow
			   ,"[grouplist]\n"
			   ,"  [group]\n"
			   ,"    groupName =\n"
			   ,"    intervalTime =\n"
			   ,"    [node]\n"
			   ,"      StrCommand =\n"
			   ,"      blEnableSend =\n"
			   ,"      cmdTail =\n"
			   ,"      StrShowName =\n"
			   ,"      StrTimeout =\n"
			   ,"      StrCycle =\n"
			   ,"      StrContinue =\n"
			   ,"      StrResend =\n"
			   ,"      StrStop =\n"
			   ,"      StrCatch =\n"
			   ,"    [node_end]\n"
			   ,"    [node]\n"
			   ,"    [node_end]\n"
			   ,"  [group_end]\n"
			   ,"  [group]\n"
			   ,"  [group_end]\n"
			   ,"[grouplist_end]\n"
			   ,COLOR(COL_clDefault,IUD(DEV_LINE_STAR)));
	PrintSuccess(env);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_LOAD::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Load group command from file");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[filename]","Filename. Default is <default.ini>");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_LOAD::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	STDSTR	fileName;
	
	fileName = msg;
	fileName = Str_SplitSubItem(&fileName, ' ');
	
	if (msg.length() == 0)
		fileName = "Default.ini";
	
	if (CFS_CheckFile(fileName)){
		BIC_ENV::GetGCList(env)->Empty();
		BIC_ENV::GetGCList(env)->Load(fileName);
		SetblUpdate(BIC_ENV::GetGCList(env));
		PrintSuccess(env,"Read from <" + fileName + ">");
	}
	else{
		PrintFail(env);
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_SAVE::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Save to file");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[filename]","Filename");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_SAVE::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	STDSTR	fileName;
	fileName = msg;
	fileName = Str_SplitSubItem(&fileName, ' ');
	
	if (msg.length() > 0){
		BIC_ENV::GetGCList(env)->Save(fileName);
		PrintSuccess(env,"Save to <" + fileName + ">");
	}
	else{
		PrintFail(env);
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
namespace BIC_ENV {
	static	bool32	GC_LS_PrintGroupList	(CMD_ENV* env,GC_LIST *tGroupList,uint32 flag);
	static	bool32	GC_LS_PringGroup		(CMD_ENV* env,COMMAND_GROUP *tGroup,uint32 flag);
	static	bool32	GC_LS_PringCommand		(CMD_ENV* env,COMMAND_NODE *tCommand,uint32 flag);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_LS::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "List group");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[-s]"		,"Show selected item");
	PrintHelpSubItem(env,"[-c]"		,"Show Cycle column");
	PrintHelpSubItem(env,"[-t]"		,"Show Timeout/interval column");
	PrintHelpSubItem(env,"[-d]"		,"Show condition");
	PrintHelpSubItem(env,"[-a]"		,"Show all");
	PrintHelpSubItem(env,"[gID]"	,"Group gID, s/uns");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_LS::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	STDSTR		strPar1,strPar2,strGroupDID;
	uint32		flag_gl,groupNum;
	
	flag_gl = 0;
	strGroupDID = "";
	
	if (msg.length() > 0){
		SplitPar1(strPar1, strPar2, msg);
		while(strPar1.length() > 0){
			if (strPar1 == "-s"){
				flag_gl |= GL_showSelect;
				flag_gl |= CL_showSelect;
			}
			else if (strPar1 == "-c"){
				flag_gl |= GL_showCycle;
				flag_gl |= CL_showCycle;
			}
			else if (strPar1 == "-t"){
				flag_gl |= GL_showInterval;
				flag_gl |= CL_showTimeout;
			}
			else if (strPar1 == "-d"){
				flag_gl |= CL_showDetail;
			}
			else if (strPar1 == "-a"){
				flag_gl |= GL_showCycle;
				flag_gl |= CL_showCycle;
				flag_gl |= GL_showInterval;
				flag_gl |= CL_showTimeout;
				flag_gl |= CL_showDetail;
			}
			else{
				strGroupDID += strPar1;
				strGroupDID += ' ';
			}
			SplitPar1(&strPar1, &strPar2);
		}
	}
	
	if (strGroupDID.length() == 0){
		BIC_ENV::GC_LS_PrintGroupList(env,BIC_ENV::GetGCList(env),flag_gl);
	}
	else{
		while(strGroupDID.length() > 0){
			strPar1 = Str_ReadSubItem(&strGroupDID," ");
			if (strPar1 == "s"){
				flag_gl |= GL_showSelect;
				flag_gl |= GL_showCL;
				BIC_ENV::GC_LS_PrintGroupList(env,BIC_ENV::GetGCList(env),flag_gl);
			}
			else if (strPar1 == "uns"){
				flag_gl &= (~GL_showSelect);
				flag_gl |= GL_showCL;
				BIC_ENV::GC_LS_PrintGroupList(env,BIC_ENV::GetGCList(env),flag_gl);
			}
			else {
				groupNum = (uint32)strtol(strPar1.c_str(),nullptr,10);
				BIC_ENV::GC_LS_PringGroup(env,(COMMAND_GROUP*)FindInDownChainByDRNodeID(BIC_ENV::GetGCList(env), groupNum),flag_gl);
			}
		}
	}
	PrintSuccess(env);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
bool32 BIC_ENV::GC_LS_PrintGroupList(CMD_ENV* env,GC_LIST* tGroupList,uint32 flag){
	COMMAND_GROUP	*fromNode,*breakNode;
	uint32			fromNodeDRID;
	int32 			j;
	bool32			blSpace;
	STDSTR			strPrintData,strTemp;
	
	if (tGroupList == nullptr)
		return G_FALSE;
	
	BIC_BASE::PrintStrNL(env,COLOR(COL_clDefault,IUD(DEV_LINE_STAR)));
	
	if ((flag & GL_showCL) == G_FALSE)
		BIC_BASE::PrintStrNL(env,COL_clDefault,COMMAND_GROUP::GetTitle(&strPrintData, flag));
	
	j = 0;
	fromNodeDRID = 0;
	fromNode = nullptr;
	breakNode = nullptr;
	blSpace = G_FALSE;
	
	do{
		if (breakNode == nullptr){
			fromNode = (COMMAND_GROUP*)TNF::GetDown(tGroupList);
		}
		else{
			breakNode = nullptr;
			fromNode = (COMMAND_GROUP*)TNF::FindInDownChainByDRNodeID(tGroupList, fromNodeDRID);
		}
		TREE_NextChain_Traversal_LINE(COMMAND_GROUP,fromNode,
			do{
				if (flag & GL_showCL){
					j = 0;
					if (((flag & GL_showSelect) && _opNode->blEnableAutoRun)
						|| (((flag & GL_showSelect) == G_FALSE) && (_opNode->blEnableAutoRun == G_FALSE))){
						GC_LS_PringGroup(env,_opNode,flag);
						j = 20;
					}
					break;
				}
				if ((flag & GL_showSelect) && (_opNode->blEnableAutoRun == G_FALSE))
					break;
									   
				BIC_BASE::PrintStrNL(env,COLOR(COL_clDefault,_opNode->Compose(&strPrintData,flag)));
				++j;
			}while(0);
								   
			if ((BIC_BASE::IsExit(env) == G_FALSE) && (j > 19) && (_nextNode != nullptr)){
				breakNode = _nextNode;
				fromNodeDRID = TNF::GetDRNodeID(_nextNode);
				break;
			}
		);
		if ((BIC_BASE::IsExit(env) == G_FALSE) && (blSpace == G_FALSE)&& (j > 19) && (breakNode != nullptr)){
			uint8 retChar;
			j = 0;
			retChar = BIC_BASE::PrintPressAnyKey(env);
			if (retChar == 27){
				break;
			}
			else if (retChar == ' '){
				blSpace = G_TRUE;
			}
		}
	}while(breakNode != nullptr);
	BIC_BASE::PrintStrNL(env,COLOR(COL_clDefault,IUD(DEV_LINE_STAR)));
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
bool32 BIC_ENV::GC_LS_PringGroup(CMD_ENV* env,COMMAND_GROUP* tGroup,uint32 flag){
	STDSTR	strPrintData;
	if (tGroup != nullptr){
		tGroup->rwLock.R_set();
#ifdef Output_h
		*CMD_ENV::GetSTDOUT(env) << Begin() << NL()
		<< COL_DivStar_Default
		<< COL_clDYellow << " gID      : " << COL_clBlue << Str_ToStr(TNF::GetDRNodeID(tGroup)) << "\n"
		<< COL_clDYellow << " Name     : " << COL_clBlue << tGroup->name << "\n"
		<< COL_clDYellow << " Interval : " << COL_clBlue << Str_ToStr(tGroup->intervalTime) << "ms\n"
		<< COL_DivStar_Default
		<< " " << COMMAND_NODE::GetTitle(&strPrintData, flag)
		<< Endl();
#endif
		tGroup->rwLock.R_clr();
		TREE_DownChain_Traversal_LINE(COMMAND_NODE, tGroup,GC_LS_PringCommand(env,_opNode,flag););
		BIC_BASE::PrintStrNL(env,COLOR(COL_clDefault,IUD(DEV_LINE_STAR)));
	}
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
bool32 BIC_ENV::GC_LS_PringCommand(CMD_ENV* env,COMMAND_NODE* command,uint32 flag){
	STDSTR	strPrintData;
	
	if (command != nullptr){
		if (((flag & CL_showSelect)) && (command->blEnableSend == G_FALSE))
			return G_TRUE;
		BIC_BASE::PrintStrNL(env,COLOR((command->blEnableSend == G_FALSE) ? COL_clDefault : COL_clBlue,command->Compose(&strPrintData, flag)));
		if ((flag & CL_showDetail))
			BIC_BASE::PrintStrNL(env,COLOR(COL_clCyan,command->ComposeDetail(&strPrintData)));
	}
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
struct BIC_GCM_SET_PAR{
	bool32	c,t,s,blnum;
	uint32	flag;
};
namespace BIC_ENV {
	static	int32	SetPar		(CMD_ENV* env, COMMAND_GROUP* tGroup, BIC_GCM_SET_PAR* tPar);
	static	int32	SetPar		(CMD_ENV* env, GC_LIST* tGroupList, BIC_GCM_SET_PAR* tPar);
	static	void	ClearSetPar	(BIC_GCM_SET_PAR* tPar);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_SET::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Set group");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[+]"		,"Set group selected");
	PrintHelpSubItem(env,"[-]"		,"Set group disselected");
	PrintHelpSubItem(env,"[-cN]"	,"Set cycle, N is value");
	PrintHelpSubItem(env,"[-tN]"	,"Set interval, N is value");
	PrintHelpSubItem(env,"<gID>"	,"Group gID, s/uns");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_SET::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	//set +/- num num
	//set c3 num num
	//set t200 num num
	//set + c3 t300 num num - num num c4 num
	STDSTR			strPar1,strPar2,strPrintData;
	uint32			groupDID;
	BIC_GCM_SET_PAR	tSetPar;
	COMMAND_GROUP	*group;
	int32			blupdate;
	
	if (msg.length() == 0){
		PrintFail(env);
		return(cgCommandID);
	}
	
	PrintResult(env,COL_clCyan,COMMAND_GROUP::GetTitle(&strPrintData,GL_showCycle | GL_showInterval));
	
	group = nullptr;
	tSetPar.blnum = G_TRUE;
	BIC_ENV::ClearSetPar(&tSetPar);
	SplitPar1(strPar1, strPar2, msg);
	blupdate = G_FALSE;
	while(strPar1.length() > 0){
		if (strPar1 == "+"){
			BIC_ENV::ClearSetPar(&tSetPar);
			tSetPar.flag |= GL_setSelect;
			tSetPar.s = G_TRUE;
		}
		else if (strPar1 == "-"){
			BIC_ENV::ClearSetPar(&tSetPar);
			tSetPar.flag |= GL_setSelect;
			tSetPar.s = G_FALSE;
		}
		else if ((strPar1.length() > 1) && (strPar1.substr(0,2) == "-c")){
			BIC_ENV::ClearSetPar(&tSetPar);
			strPar1 = strPar1.substr(2);
			tSetPar.flag |= GL_setCycle;
			tSetPar.c = atoi(strPar1.c_str());
		}
		else if ((strPar1.length() > 1) && (strPar1.substr(0,2) == "-t")){
			BIC_ENV::ClearSetPar(&tSetPar);
			strPar1 = strPar1.substr(2);
			tSetPar.flag |= GL_setInterval;
			tSetPar.t = atoi(strPar1.c_str());
		}
		else{
			tSetPar.blnum = G_TRUE;
			if (strPar1 == "s"){
				tSetPar.flag |= GL_Select;
				BIC_ENV::SetPar(env,BIC_ENV::GetGCList(env),&tSetPar);
				blupdate = G_TRUE;
			}
			else if (strPar1 == "uns"){
				tSetPar.flag &= (~GL_Select);
				BIC_ENV::SetPar(env,BIC_ENV::GetGCList(env),&tSetPar);
				blupdate = G_TRUE;
			}
			else{
				groupDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
				group = (COMMAND_GROUP*)FindInDownChainByDRNodeID(BIC_ENV::GetGCList(env),groupDID);
				tSetPar.flag &= (~GL_Select);
				BIC_ENV::SetPar(env,group,&tSetPar);
				blupdate = G_TRUE;
			}
		}
		SplitPar1(&strPar1, &strPar2);
	}
	if (blupdate == G_TRUE)
		SetblUpdate(BIC_ENV::GetGCList(env));
	PrintSuccess(env);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
static void BIC_ENV::ClearSetPar(BIC_GCM_SET_PAR *tPar){
	if (tPar->blnum){
		tPar->flag = 0;
		tPar->c = 0;
		tPar->t = 0;
		tPar->s = 0;
		tPar->blnum = G_FALSE;
	}
};
//------------------------------------------------------------------------------------------//
static bool32 BIC_ENV::SetPar(CMD_ENV* env, COMMAND_GROUP* tGroup, BIC_GCM_SET_PAR* tPar){
	STDSTR strPrintData;
	if (tGroup != nullptr){
		tGroup->rwLock.W_set();
		if ((tPar->flag & GL_setSelect))
			tGroup->blEnableAutoRun = tPar->s;
		if ((tPar->flag & GL_setCycle))
			tGroup->autoRunTimes = tPar->c;
		if ((tPar->flag & GL_setInterval))
			tGroup->intervalTime = tPar->t;
		tGroup->rwLock.W_clr();
		BIC_BASE::PrintResult(env,COLOR(COL_clDefault,tGroup->Compose(&strPrintData,GL_showCycle | GL_showInterval)));
	}
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
static bool32 BIC_ENV::SetPar(CMD_ENV* env, GC_LIST* tGroupList, BIC_GCM_SET_PAR* tPar){
	if (tGroupList == nullptr)
		return G_FALSE;
	TREE_DownChain_Traversal_LINE(COMMAND_GROUP,tGroupList,
		if ((tPar->flag & GL_Select) && _opNode->blEnableAutoRun)
			SetPar(env,_opNode,tPar);
		if (((tPar->flag & GL_Select) == G_FALSE) && (_opNode->blEnableAutoRun == G_FALSE))
			SetPar(env,_opNode,tPar);
	);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
CMDID BIC_GC_CLONE::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Clone group by gID");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[gID]","Group gID");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_CLONE::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	COMMAND_GROUP	*nextGroup,*newGroup;
	STDSTR			strPar1,strPar2;
	uint32			groupDID;
	
	newGroup = nullptr;
	nextGroup = nullptr;
	if (msg.length() > 0){
		SplitPar1(strPar1, strPar2, msg);
		while(strPar1.length() > 0){
			groupDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
			nextGroup = (COMMAND_GROUP*)FindInDownChainByDRNodeID(BIC_ENV::GetGCList(env),groupDID);
			if (nextGroup != nullptr){
				if (newGroup == nullptr){
					newGroup = (COMMAND_GROUP*)BIC_ENV::GetGCList(env)->GetNewNode();
					if (newGroup != nullptr){
						newGroup->name = "new copy group";
						COMMAND_GROUP::CopyCommandGroup_nolook(newGroup,nextGroup,G_FALSE);
					}
				}
				else{
					COMMAND_GROUP::CopyCommandGroup_nolook(newGroup,nextGroup,G_FALSE);
				}
			}
			SplitPar1(&strPar1, &strPar2);
		}
	}
	else{
		newGroup = (COMMAND_GROUP*)BIC_ENV::GetGCList(env)->GetNewNode();
		if (newGroup != nullptr){
			newGroup->name = "new group";
			COMMAND_GROUP::CopyCommandGroup_nolook(newGroup,nextGroup,G_FALSE);
		}
	}
	
	if (newGroup != nullptr){
		BIC_ENV::GetGCList(env)->AppendDownNode(newGroup);
		BIC_ENV::GC_LS_PringGroup(env,newGroup, CL_showCycle | CL_showTimeout);
		SetblUpdate(BIC_ENV::GetGCList(env));
		PrintSuccess(env);
	}
	else{
		PrintFail(env);
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_DEL::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Delete group by gID");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[gID]","Group gID");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_DEL::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	STDSTR		strPar1,strPar2;
	uint32		groupDID;
	TNF			*node;
	
	if (msg.length() == 0){
		PrintFail(env);
		return(cgCommandID);
	}
	SplitPar1(strPar1, strPar2, msg);
	if (strPar1 == "all"){
		BIC_ENV::GetGCList(env)->Empty();
		PrintResult(env,"Delete gID:all");
		SetblUpdate(BIC_ENV::GetGCList(env));
	}
	else{
		while(strPar1.length() > 0){
			groupDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
			node = FindInDownChainByDRNodeID(BIC_ENV::GetGCList(env), groupDID);
			BIC_ENV::GetGCList(env)->MoveToTrash(node,node);
			PrintResult(env,"Delete gID:",Str_ToStr(groupDID));
			SplitPar1(&strPar1, &strPar2);
		};
		SetblUpdate(BIC_ENV::GetGCList(env));
	}
	PrintSuccess(env);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_MVUP::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Move group up");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"<gID>"	,"Group gID");
	PrintHelpSubItem(env,"[step]"	,"Default value is 1");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_MVUP::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	COMMAND_GROUP	*nextGroup;
	STDSTR			strPar1,strPar2;
	uint32			groupDID,step;
	
	if (msg.length() > 0){
		SplitPar1(strPar1, strPar2, msg);
		groupDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
		step = (uint32)strtol(strPar2.c_str(),nullptr,10);
		if (step == 0)
			step = 1;
		nextGroup = (COMMAND_GROUP*)FindInDownChainByDRNodeID(BIC_ENV::GetGCList(env), groupDID);
		if (nextGroup != nullptr){
			while(step-- > 0)
				MovePrior(nextGroup,nextGroup);
			TNF::SetblUpdate(BIC_ENV::GetGCList(env));
			PrintSuccess(env);
			return(cgCommandID);
		}
	}
	PrintFail(env);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_MVDN::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Move group down");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"<gID>"	,"Group gID");
	PrintHelpSubItem(env,"[step]"	,"Default value is 1");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_MVDN::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	COMMAND_GROUP	*nextGroup;
	STDSTR			strPar1,strPar2;
	uint32			groupDID,step;
	
	if (msg.length() > 0){
		SplitPar1(strPar1, strPar2, msg);
		groupDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
		step = (uint32)strtol(strPar2.c_str(),nullptr,10);
		if (step == 0)
			step = 1;
		nextGroup = (COMMAND_GROUP*)FindInDownChainByDRNodeID(BIC_ENV::GetGCList(env), groupDID);
		if (nextGroup != nullptr){
			while(step-- > 0)
				MoveNext(nextGroup,nextGroup);
			TNF::SetblUpdate(BIC_ENV::GetGCList(env));
			PrintSuccess(env);
			return(cgCommandID);
		}
	}
	PrintFail(env);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_NAME::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Set group name by gID");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"<gID>"	,"Group gID");
	PrintHelpSubItem(env,"<name>"	,"Name");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_NAME::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	COMMAND_GROUP	*nextGroup;
	STDSTR			strPar1,strPar2,strPrintData;
	uint32			gDID;
	
	nextGroup = nullptr;
	if (msg.length() > 0){
		SplitPar1(strPar1, strPar2, msg);
		gDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
		nextGroup = (COMMAND_GROUP*)FindInDownChainByDRNodeID(BIC_ENV::GetGCList(env), gDID);
		if (nextGroup != nullptr){
			nextGroup->rwLock.W_set();
			nextGroup->name = strPar2;
			nextGroup->rwLock.W_clr();
			PrintResult(env,COLOR(COL_clDefault,nextGroup->Compose(&strPrintData,GL_showCycle | GL_showInterval)));
			TNF::SetblUpdate(BIC_ENV::GetGCList(env));
			PrintSuccess(env);
			return(cgCommandID);
		}
	}
	PrintFail(env);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//






















//------------------------------------------------------------------------------------------//
CMDID GC_GROUP_BASE::Execute(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* gID)const{
	uint32 gDID = *static_cast<uint32*>(gID);
	COMMAND_GROUP	*group;
	if (mID == (uint32)cgCommandID){
		group = (COMMAND_GROUP*)FindInDownChainByDRNodeID(BIC_ENV::GetGCList(env), gDID);
		if (group == nullptr){
			PrintFail(env,"ERROR gID");
			return BIC_ID_RETURN;
		}
		return(BIC_BASE::Execute(env,mID,msg,group));
	}
	return(CMD_ID_NO);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_GROUP_LS::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "List command");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[-s]"	,"Show selected item");
	PrintHelpSubItem(env,"[-c]"	,"Show Cycle column");
	PrintHelpSubItem(env,"[-t]"	,"Show Timeout column");
	PrintHelpSubItem(env,"[-d]"	,"Show condition");
	PrintHelpSubItem(env,"[-a]"	,"Show all");
	PrintHelpSubItem(env,"[cID]","Command cID");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_GROUP_LS::Command(CMD_ENV* env,const STDSTR& msg,void* cg)const{
	COMMAND_GROUP*	group = static_cast<COMMAND_GROUP*>(cg);
	STDSTR			strPar1,strPar2,strcDID,strPrintData;
	uint32			flag_gl,cDID;

	flag_gl = 0;
	strcDID = "";

	if (msg.length() > 0){
		SplitPar1(strPar1, strPar2, msg);
		while(strPar1.length() > 0){
			if (strPar1 == "-s"){
				flag_gl |= CL_showSelect;
			}
			else if (strPar1 == "-c"){
				flag_gl |= CL_showCycle;
			}
			else if (strPar1 == "-t"){
				flag_gl |= CL_showTimeout;
			}
			else if (strPar1 == "-d"){
				flag_gl |= CL_showDetail;
			}
			else if (strPar1 == "-a"){
				flag_gl |= CL_showCycle;
				flag_gl |= CL_showTimeout;
				flag_gl |= CL_showDetail;
			}
			else{
				strcDID += strPar1;
				strcDID += ' ';
			}
			SplitPar1(&strPar1, &strPar2);
		}
	}
	if (strcDID.length() == 0){
		BIC_ENV::GC_LS_PringGroup(env,group,flag_gl);
	}
	else{
		PrintStrNL(env
				   ,COLOR(COL_clDefault,IUD(DEV_LINE_STAR))
				   ,COMMAND_NODE::GetTitle(&strPrintData, flag_gl));
		while(strcDID.length() > 0){
			strPar1 = Str_ReadSubItem(&strcDID," ");
			cDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
			BIC_ENV::GC_LS_PringCommand(env,(COMMAND_NODE*)FindInDownChainByDRNodeID(group, cDID),flag_gl);
		}
		PrintStrNL(env,COLOR(COL_clDefault,IUD(DEV_LINE_STAR)));
	}
	
	PrintSuccess(env);

	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
struct BIC_GCM_GROUP_SET_PAR{
	bool32	s,r,n,blnum;
	uint32	flag;
	STDSTR	c,t;
};
//------------------------------------------------------------------------------------------//
namespace BIC_ENV {
	static void		ClearSetPar (BIC_GCM_GROUP_SET_PAR* tPar);
	static bool32	SetPar		(CMD_ENV* env, COMMAND_NODE* tCommand, BIC_GCM_GROUP_SET_PAR* tPar);
	static bool32	SetPar		(CMD_ENV* env, COMMAND_GROUP* tGroup, BIC_GCM_GROUP_SET_PAR* tPar);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_GROUP_SET::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Set command");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[+]"	,"Set command selected");
	PrintHelpSubItem(env,"[-]"	,"Set command disselected");
	PrintHelpSubItem(env,"[-cN]","Set cycle, N is value");
	PrintHelpSubItem(env,"[-tN]","Set timeout, N is value");
	PrintHelpSubItem(env,"[+r]","Enable send \"\\r\" in the end");
	PrintHelpSubItem(env,"[-r]","Disable send \"\\r\" in the end");
	PrintHelpSubItem(env,"[+n]","Enable send \"\\n\" in the end");
	PrintHelpSubItem(env,"[-n]","Disable send \"\\n\" in the end");
	PrintHelpSubItem(env,"<cID>","Command cID, s/uns");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_GROUP_SET::Command(CMD_ENV* env,const STDSTR& msg,void* cg)const{
	
	//set +/- num num
	//set c3 num num
	//set t200 num num
	//set + c3 t300 num num - num num c4 num
	COMMAND_GROUP*	group = static_cast<COMMAND_GROUP*>(cg);
	STDSTR			strPar1,strPar2,strPrintData;
	uint32			cDID;
	BIC_GCM_GROUP_SET_PAR	setPar;
	COMMAND_NODE	*command;
	int32			blupdate;
	
	if (msg.length() == 0){
		PrintFail(env);
		return(cgCommandID);
	}
	
	PrintResult(env,COL_clCyan,COMMAND_NODE::GetTitle(&strPrintData,CL_showCycle | CL_showTimeout));
	
	command = nullptr;
	setPar.blnum = G_TRUE;
	BIC_ENV::ClearSetPar(&setPar);
	SplitPar1(strPar1, strPar2, msg);
	blupdate = G_FALSE;
	while(strPar1.length() > 0){
		if (strPar1 == "+"){
			BIC_ENV::ClearSetPar(&setPar);
			setPar.flag |= CL_setSelect;
			setPar.s = G_TRUE;
		}
		else if (strPar1 == "-"){
			BIC_ENV::ClearSetPar(&setPar);
			setPar.flag |= CL_setSelect;
			setPar.s = G_FALSE;
		}
		else if (strPar1 == "+r"){
			BIC_ENV::ClearSetPar(&setPar);
			setPar.flag |= CL_setCR;
			setPar.r = G_TRUE;
		}
		else if (strPar1 == "-r"){
			BIC_ENV::ClearSetPar(&setPar);
			setPar.flag |= CL_setCR;
			setPar.r = G_FALSE;
		}
		else if (strPar1 == "+n"){
			BIC_ENV::ClearSetPar(&setPar);
			setPar.flag |= CL_setCR;
			setPar.n = G_TRUE;
		}
		else if (strPar1 == "-n"){
			BIC_ENV::ClearSetPar(&setPar);
			setPar.flag |= CL_setCR;
			setPar.n = G_FALSE;
		}
		else if ((strPar1.length() > 1) && (strPar1.substr(0,2) == "-c")){
			BIC_ENV::ClearSetPar(&setPar);
			strPar1 = strPar1.substr(2);
			setPar.flag |= CL_setCycle;
			setPar.c = strPar1;
		}
		else if ((strPar1.length() > 1) && (strPar1.substr(0,2) == "-t")){
			BIC_ENV::ClearSetPar(&setPar);
			strPar1 = strPar1.substr(2);
			setPar.flag |= CL_setTimeout;
			setPar.t = strPar1;
		}
		else{
			setPar.blnum = G_TRUE;
			if (strPar1 == "s"){
				setPar.flag |= CL_Select;
				BIC_ENV::SetPar(env,group,&setPar);
				blupdate = G_TRUE;
			}
			else if (strPar1 == "uns"){
				setPar.flag &= (~CL_Select);
				BIC_ENV::SetPar(env,group,&setPar);
				blupdate = G_TRUE;
			}
			else{
				cDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
				command = (COMMAND_NODE*)FindInDownChainByDRNodeID(group, cDID);
				setPar.flag &= (~CL_Select);
				BIC_ENV::SetPar(env,command,&setPar);
				blupdate = G_TRUE;
			}
		}
		SplitPar1(&strPar1, &strPar2);
	}
	if (blupdate)
		TNF::SetblUpdate(BIC_ENV::GetGCList(env));
	PrintSuccess(env);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
static void BIC_ENV::ClearSetPar(BIC_GCM_GROUP_SET_PAR* tPar){
	if (tPar->blnum != 0){
		tPar->flag = 0;
		tPar->c = "";
		tPar->t = "";
		tPar->s = 0;
		tPar->r = 0;
		tPar->n = 0;
		tPar->blnum = 0;
	}
};
//------------------------------------------------------------------------------------------//
static bool32 BIC_ENV::SetPar(CMD_ENV* env, COMMAND_NODE* tCommand, BIC_GCM_GROUP_SET_PAR* tPar){
	STDSTR strPrintData;
	if (tCommand != nullptr){
		tCommand->rwLock.W_set();
		if (tPar->flag & CL_setSelect)
			tCommand->blEnableSend = tPar->s;
		if (tPar->flag & CL_setCycle)
			tCommand->StrCycle = tPar->c;
		if (tPar->flag & CL_setTimeout)
			tCommand->StrTimeout = tPar->t;
		if (tPar->flag & CL_setCR){
			if (tPar->r == G_FALSE){
				if (tPar->n == G_FALSE){
					tCommand->cmdTail = CMD_NONE;
				}
				else{
					tCommand->cmdTail = CMD_N;
				}
			}
			else{
				if (tPar->n == G_FALSE){
					tCommand->cmdTail = CMD_R;
				}
				else{
					tCommand->cmdTail = CMD_RN;
				}
			}
		}
		tCommand->rwLock.W_clr();
		BIC_BASE::PrintResult(env,COLOR(COL_clDefault,tCommand->Compose(&strPrintData, CL_showCycle | CL_showTimeout)));
	}
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
static bool32 BIC_ENV::SetPar(CMD_ENV* env, COMMAND_GROUP* tGroup, BIC_GCM_GROUP_SET_PAR* tPar){
	
	if (tGroup == nullptr)
		return 0;
	TREE_DownChain_Traversal_LINE(COMMAND_NODE,tGroup,
		if ((tPar->flag & CL_Select) && _opNode->blEnableSend)
			SetPar(env,_opNode,tPar);
			if (((tPar->flag & CL_Select) == G_FALSE) && (_opNode->blEnableSend == G_FALSE))
				SetPar(env,_opNode,tPar);
	);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
CMDID BIC_GC_GROUP_CLONE::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Clone command by cID");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[cID]","Command cID");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_GROUP_CLONE::Command(CMD_ENV* env,const STDSTR& msg,void* cg)const{
	COMMAND_GROUP*	group = static_cast<COMMAND_GROUP*>(cg);
	COMMAND_NODE	*command,*newNode;
	uint32			cDID;
	STDSTR			strPrintData;
	
	command = nullptr;
	newNode = nullptr;

	newNode = (COMMAND_NODE*)group->GetNewNode();
	if (newNode == nullptr){
		PrintFail(env);
	}
	else{
		newNode->StrCommand = "new command";
		if (msg.length() > 0){
			cDID = (uint32)strtol(msg.c_str(),nullptr,10);
			command = (COMMAND_NODE*)FindInDownChainByDRNodeID(group, cDID);
			if (command != nullptr)
				COMMAND_NODE::CopyCommandNode(newNode,command);
		}
		group->AppendDownNode(newNode);
		PrintStrNL(env
				   ,COLOR(COL_clDefault,IUD(DEV_LINE_STAR))
				   ,COMMAND_NODE::GetTitle(&strPrintData, CL_showCycle | CL_showTimeout | CL_showDetail));
		BIC_ENV::GC_LS_PringCommand(env,newNode, CL_showCycle | CL_showTimeout | CL_showDetail);
		PrintStrNL(env,COLOR(COL_clDefault,IUD(DEV_LINE_STAR)));
		TNF::SetblUpdate(BIC_ENV::GetGCList(env));
		PrintSuccess(env);
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_GROUP_DEL::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Delete command by cID");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[cID]","Command cID");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_GROUP_DEL::Command(CMD_ENV* env,const STDSTR& msg,void* cg)const{
	COMMAND_GROUP*	group = static_cast<COMMAND_GROUP*>(cg);
	STDSTR			strPar1,strPar2;
	uint32			cDID;
	TNF				*node;
	
	if (msg.length() == 0){
		PrintFail(env);
	}
	else{
		SplitPar1(strPar1, strPar2, msg);
		if (strPar1 == "all"){
			if (group != nullptr)
				CleanDownTree(group, group);
			PrintResult(env,"Delete gID:all");
			SetblUpdate(BIC_ENV::GetGCList(env));
		}
		else{
			while(strPar1.length() > 0){
				cDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
				node = FindInDownChainByDRNodeID(group, cDID);
				BIC_ENV::GetGCList(env)->MoveToTrash(node,node);
				PrintResult(env,"Delete cID:",Str_ToStr(cDID));
				SplitPar1(&strPar1, &strPar2);
			};
			SetblUpdate(BIC_ENV::GetGCList(env));
		}
		PrintSuccess(env);
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_GROUP_MVUP::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Move command up");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"<cID>"	,"Command cID");
	PrintHelpSubItem(env,"[step]"	,"Default value is 1");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_GROUP_MVUP::Command(CMD_ENV* env,const STDSTR& msg,void* cg)const{
	COMMAND_GROUP*	group = static_cast<COMMAND_GROUP*>(cg);
	COMMAND_NODE*	command;
	STDSTR			strPar1,strPar2;
	uint32			cDID,step;
	
	if (msg.length() > 0){
		SplitPar1(strPar1, strPar2, msg);
		cDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
		step = (uint32)strtol(strPar2.c_str(),nullptr,10);
		if (step == 0)
			step = 1;
		command = (COMMAND_NODE*)FindInDownChainByDRNodeID(group, cDID);
		if (command != nullptr){
			while(step-- > 0)
				MovePrior(command,command);
			TNF::SetblUpdate(BIC_ENV::GetGCList(env));
			PrintSuccess(env);
			return(cgCommandID);
		}
	}
	PrintFail(env);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_GROUP_MVDN::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env, cgCommand, "Move command down");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"<cID>"	,"Command cID");
	PrintHelpSubItem(env,"[step]"	,"Default value is 1");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_GROUP_MVDN::Command(CMD_ENV* env,const STDSTR& msg,void* cg)const{
	COMMAND_GROUP*	group = static_cast<COMMAND_GROUP*>(cg);
	COMMAND_NODE*	command;
	STDSTR			strPar1,strPar2;
	uint32			cDID,step;
	
	if (msg.length() > 0){
		SplitPar1(strPar1, strPar2, msg);
		cDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
		step = (uint32)strtol(strPar2.c_str(),nullptr,10);
		if (step == 0)
			step = 1;
		command = (COMMAND_NODE*)FindInDownChainByDRNodeID(group, cDID);
		if (command != nullptr){
			while(step-- > 0)
				MoveNext(command,command);
			TNF::SetblUpdate(BIC_ENV::GetGCList(env));
			PrintSuccess(env);
			return(cgCommandID);
		}
	}
	PrintFail(env);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
#define BIC_GC_GROUP_SET(_name,text,var) \
CMDID BIC_GC_GROUP_##_name::Help(CMD_ENV* env,uint32 flag)const{\
	PrintHelpItem(env, cgCommand, text);\
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))\
		return(cgCommandID);\
	PrintHelpSubItem(env,"<cID>"	,"Command cID");\
	PrintHelpSubItem(env,"<name>"	,"Name");\
	return(cgCommandID);\
};\
CMDID BIC_GC_GROUP_##_name::Command(CMD_ENV* env,const STDSTR& msg,void* cg)const{\
	COMMAND_GROUP*	group = static_cast<COMMAND_GROUP*>(cg);\
	COMMAND_NODE*	command;\
	STDSTR			strPar1,strPar2,strPrintData;\
	uint32			cDID;\
	\
	if (msg.length() > 0){\
		SplitPar1(strPar1, strPar2, msg);\
		cDID = (uint32)strtol(strPar1.c_str(),nullptr,10);\
		command = (COMMAND_NODE*)FindInDownChainByDRNodeID(group, cDID);\
		if (command != nullptr){\
			command->rwLock.W_set();\
			command->var = strPar2;\
			command->rwLock.W_clr();\
			PrintStrNL(env\
						,COLOR(COL_clDefault,IUD(DEV_LINE_STAR))\
						,COMMAND_NODE::GetTitle(&strPrintData, CL_showCycle | CL_showTimeout | CL_showDetail));\
			BIC_ENV::GC_LS_PringCommand(env,command, CL_showCycle | CL_showTimeout | CL_showDetail);\
			PrintStrNL(env,COLOR(COL_clDefault,IUD(DEV_LINE_STAR)));\
			TNF::SetblUpdate(BIC_ENV::GetGCList(env));\
			PrintSuccess(env);\
			return(cgCommandID);\
		}\
	}\
	PrintFail(env);\
	return(cgCommandID);\
};
//------------------------------------------------------------------------------------------//
BIC_GC_GROUP_SET(COMMAND,	"Set command by cID",StrCommand);
BIC_GC_GROUP_SET(CONTINUE,	"Set continue by cID",StrContinue);
BIC_GC_GROUP_SET(RESEND,	"Set resend by cID",StrResend);
BIC_GC_GROUP_SET(CSTOP,		"Set cstop by cID",StrStop);
BIC_GC_GROUP_SET(CATCH,		"Set catch by cID",StrCatch);
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
BIC_GC_GROUP::BIC_GC_GROUP(void) : BIC_BASE_S(){
	cgCommandID = BIC_ID_GC_GROUP;
	cgCommand = "g/group";
	cgConsoleName = "";
	cgHelpName = "Into group by gID";
	
	AppendDown(cgSub_ls)
	< cgSub_set < cgSub_clone < cgSub_del
	< cgSub_mvup < cgSub_mvdn
	< cgSub_command < cgSub_continue < cgSub_resend < cgSub_cstop < cgSub_catch;
};
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_GROUP::Help(CMD_ENV* env,uint32 flag)const{
	if (B_ChkFLAG32(flag, CMD_blPrintSubCMD)){
		PrintALine(env,COL_clDYellow,"Build-in command explain:");
		TraversalChildHelp(env,flag & CMD_blPrintSimple);
	}
	else{
		PrintHelpItem(env, cgCommand, "->" ,cgHelpName);
		if (B_ChkFLAG32(flag, CMD_blPrintSimple))
			return(cgCommandID);
		PrintHelpSubItem(env,"<gID>","Group gID");
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_GC_GROUP::MessageProcessing(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* p)const{
	CMDID	retCode = cgCommandID;
	uint32	gDID;
	STDSTR	strPar1,strPar2,*name;
	
	if (mID == (uint32)cgCommandID){
		if (msg.length() == 0){
			PrintFail(env);
		}
		else{
			if ((msg == "?") || (msg == "-help"))
				return(Help(env,CMD_blPrintSubCMD | CMD_blPrintSimple));
			if (msg == "??")
				return(Help(env,CMD_blPrintSubCMD));
			
			SplitPar1(strPar1, strPar2, msg);
			gDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
			if (FindInDownChainByDRNodeID(BIC_ENV::GetGCList(env), gDID) == nullptr){
				PrintFail(env,"ERROR gID");
			}
			else{
				name = (STDSTR*)&cgConsoleName;
				*name = "group-" + Str_ToStr(gDID);
				retCode = Execute(env,cgCommandID,strPar2,&gDID);
				*name = "group";
				if (retCode == BIC_ID_RETURN)
					retCode = cgCommandID;
			}
		}
	}
	return(retCode);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
BIC_GCM::BIC_GCM(void) : BIC_BASE_S(){
	cgCommandID = BIC_ID_GC;
	cgCommand = "gcm";
	cgConsoleName = cgCommand;
	cgHelpName = "GCM";
	
	AppendDown(cgSub_eg)
	< cgSub_load < cgSub_save
	< cgSub_ls< cgSub_set< cgSub_clone< cgSub_del
	< cgSub_mvup< cgSub_mvdn
	< cgSub_name < cgSub_group;
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* BIC_GCM_h */
