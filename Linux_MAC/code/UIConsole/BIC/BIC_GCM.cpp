//
//  BIC_GCM.cpp
//  SDT
//
//  Created by Leif Wen on 05/04/2017.
//  Copyright © 2017 leif wen. All rights reserved.
//
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "BIC_GCM.h"
#include "Comm_Convert.h"
#include "Comm_File.h"
#include "AppLayer.h"
//------------------------------------------------------------------------------------------//
#ifdef BIC_GCM_hpp
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_LOAD::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	uint32	readType;
	STDSTR		fileN;
	STDSTR	strPar;
	strPar = par;
	strPar = Str_SplitSubItem(&strPar, ' ');
	
	readType = READINITYPE_NONE | READINITYPE_AddGroupCommand | READINITYPE_CoverGroupCommand;
	
	fileN = "Default.ini";
	if (par.length() > 0)
		fileN = strPar;
	if (CFS_CheckFile(fileN) > 0){
		env->cSDTApp->m_ParRecord.ReadScriptFromIniFile(fileN,readType);
		env->cSDTApp->m_GCList.SetblUpdate();
		PrintSuccess(env,"Read from <" + fileN + ">");
	}
	else{
		PrintFail(env);
	}
	
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_SAVE::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	uint32	readType;
	STDSTR	strPar;
	strPar = par;
	strPar = Str_SplitSubItem(&strPar, ' ');
	
	readType = READINITYPE_NONE | READINITYPE_AddGroupCommand;
	readType |= (READINITYPE_AddSingleCommand);
	readType |= (READINITYPE_AddDeviceList);
	
	if (par.length() > 0){
		env->cSDTApp->m_ParRecord.WriteScriptToIniFile(strPar,readType);
		PrintSuccess(env,"Save to <" + strPar + ">");
	}
	else{
		PrintFail(env);
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_LS::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	STDSTR		strPar1,strPar2,strGroupDID;
	uint32		flag_gl,groupNum;
	
	flag_gl = 0;
	strGroupDID = "";
	
	if (par.length() > 0){
		SplitPar1(strPar1, strPar2, par);
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
		BIC_GCM_LS_PrintGroupList(env,&env->cSDTApp->m_GCList,flag_gl);
	}
	else{
		while(strGroupDID.length() > 0){
			strPar1 = Str_ReadSubItem(&strGroupDID," ");
			if (strPar1 == "s"){
				flag_gl |= GL_showSelect;
				flag_gl |= GL_showCL;
				BIC_GCM_LS_PrintGroupList(env,&env->cSDTApp->m_GCList,flag_gl);
			}
			else if (strPar1 == "uns"){
				flag_gl &= (~GL_showSelect);
				flag_gl |= GL_showCL;
				BIC_GCM_LS_PrintGroupList(env,&env->cSDTApp->m_GCList,flag_gl);
			}
			else {
				groupNum = (uint32)strtol(strPar1.c_str(),nullptr,10);
				BIC_GCM_LS_PringGroup(env,(COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&env->cSDTApp->m_GCList, groupNum),flag_gl);
			}
		}
	}
	PrintSuccess(env);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_LS::BIC_GCM_LS_PrintGroupList(BIC_ENV *env,GC_LIST *tGroupList,uint32 flag){
	COMMAND_GROUP	*fromNode,*breakNode;
	uint32			fromNodeDRID;
	int32 j;
	STDSTR		strPrintData,strTemp;
	
	if (tGroupList == nullptr)
		return 0;
	
	PrintStrNL(env,Data(COL_clDefault,DEV_LINE_START));
	
	if ((flag & GL_showCL) == 0)
		PrintStrNL(env,Data(COL_clDefault,&COMMAND_GROUP::GetTitle(&strPrintData, flag)));
	
	j = 0;
	fromNodeDRID = 0;
	fromNode = nullptr;
	breakNode = nullptr;
	
	do{
		if (breakNode == nullptr){
			fromNode = (COMMAND_GROUP*)GetcgDown(tGroupList);
		}
		else{
			breakNode = nullptr;
			fromNode = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(tGroupList, fromNodeDRID);
		}
		TREE_RChain_Traversal_LINE(COMMAND_GROUP,fromNode,
			do{
				if ((flag & GL_showCL) != 0){
					j = 0;
					if ((((flag & GL_showSelect) != 0) && (operateNode_t->blEnableAutoRun != 0))
						|| (((flag & GL_showSelect) == 0) && (operateNode_t->blEnableAutoRun == 0))){
						BIC_GCM_LS_PringGroup(env,operateNode_t,flag);
						j = 20;
					}
					break;
				}
				if (((flag & GL_showSelect) != 0) && (operateNode_t->blEnableAutoRun == 0))
					break;
				
				PrintStrNL(env,Data(COL_clDefault,&operateNode_t->Compose(&strPrintData,flag)));
				++j;
			}while(0);
			
			if ((env->blExit == 0) && (j > 19) && (nextNode_t != nullptr)){
				breakNode = nextNode_t;
				fromNodeDRID = GetdRNodeID(nextNode_t);
				break;
			}
		);
		if ((env->blExit == 0) && (j > 19) && (breakNode != nullptr)){
			uint8 retChar;
			j = 0;
			retChar = PrintPressAnyKey(env);
			if (retChar == 27){
				break;
			}
			else if (retChar == ' '){
				SetblExit(env);
			}
		}
	}while(breakNode != nullptr);
	PrintStrNL(env,Data(COL_clDefault,DEV_LINE_START));
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_LS::BIC_GCM_LS_PringGroup(BIC_ENV *env,COMMAND_GROUP *tGroup,uint32 flag){
	STDSTR	strPrintData;
	if (tGroup != nullptr){
		tGroup->Spin_InUse_set();
		PrintNL(env)
		<< COL_DivStar_Default
		<< COL_clDYellow << " gID      : " << COL_clBlue << Str_ToString(GetdRNodeID(tGroup)) << "\n"
		<< COL_clDYellow << " Name     : " << COL_clBlue << tGroup->name << "\n"
		<< COL_clDYellow << " Interval : " << COL_clBlue << Str_ToString(tGroup->intervalTime) << "ms\n"
		<< COL_DivStar_Default
		<< COMMAND_NODE::GetTitle(&strPrintData, flag)
		<< Endl;
		tGroup->Spin_InUse_clr();
		TREE_LChildRChain_Traversal_LINE(COMMAND_NODE, tGroup,BIC_GCM_LS_PringCommand(env,operateNode_t,flag););
		PrintStrNL(env,Data(COL_clDefault,DEV_LINE_START));
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_LS::BIC_GCM_LS_PringCommand(BIC_ENV *env,COMMAND_NODE *command,uint32 flag){
	STDSTR	strPrintData;
	
	if (command != nullptr){
		if (((flag & CL_showSelect) != 0) && (command->blEnableSend == 0))
			return 1;
		PrintStrNL(env,Data((command->blEnableSend == 0)?COL_clDefault:COL_clBlue,&command->Compose(&strPrintData, flag)));
		if ((flag & CL_showDetail) != 0)
			PrintStrNL(env,Data(COL_clCyan,&command->ComposeDetail(&strPrintData)));
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
struct BIC_GCM_SET_PAR{
	int32	c,t,s,blnum;
	uint32	flag;
};
void ClearSetPar(struct BIC_GCM_SET_PAR *tPar){
	if (tPar->blnum != 0){
		tPar->flag = 0;
		tPar->c = 0;
		tPar->t = 0;
		tPar->s = 0;
		tPar->blnum = 0;
	}
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_SET::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	//set +/- num num
	//set c3 num num
	//set t200 num num
	//set + c3 t300 num num - num num c4 num
	STDSTR			strPar1,strPar2,strPrintData;
	uint32			groupDID;
	BIC_GCM_SET_PAR	tSetPar;
	COMMAND_GROUP	*group;
	int32			blupdate;
	
	if (par.length() == 0){
		PrintFail(env);
		return(cgReturnCode);
	}

	PrintResult(env,Data(COL_clCyan,&COMMAND_GROUP::GetTitle(&strPrintData,GL_showCycle | GL_showInterval)));
	
	group = nullptr;
	tSetPar.blnum = 1;
	ClearSetPar(&tSetPar);
	SplitPar1(strPar1, strPar2, par);
	blupdate = 0;
	while(strPar1.length() > 0){
		if (strPar1 == "+"){
			ClearSetPar(&tSetPar);
			tSetPar.flag |= GL_setSelect;
			tSetPar.s = 1;
		}
		else if (strPar1 == "-"){
			ClearSetPar(&tSetPar);
			tSetPar.flag |= GL_setSelect;
			tSetPar.s = 0;
		}
		else if ((strPar1.length() > 1) && (strPar1.substr(0,2) == "-c")){
			ClearSetPar(&tSetPar);
			strPar1 = strPar1.substr(2);
			tSetPar.flag |= GL_setCycle;
			tSetPar.c = atoi(strPar1.c_str());
		}
		else if ((strPar1.length() > 1) && (strPar1.substr(0,2) == "-t")){
			ClearSetPar(&tSetPar);
			strPar1 = strPar1.substr(2);
			tSetPar.flag |= GL_setInterval;
			tSetPar.t = atoi(strPar1.c_str());
		}
		else{
			tSetPar.blnum = 1;
			if (strPar1 == "s"){
				tSetPar.flag |= GL_Select;
				SetPar(env,&env->cSDTApp->m_GCList,&tSetPar);
				blupdate = 1;
			}
			else if (strPar1 == "uns"){
				tSetPar.flag &= (~GL_Select);
				SetPar(env,&env->cSDTApp->m_GCList,&tSetPar);
				blupdate = 1;
			}
			else{
				groupDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
				group = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&env->cSDTApp->m_GCList,groupDID);
				tSetPar.flag &= (~GL_Select);
				SetPar(env,group,&tSetPar);
				blupdate = 1;
			}
		}
		SplitPar1(&strPar1, &strPar2);
	}
	if (blupdate == 1)
		env->cSDTApp->m_GCList.SetblUpdate();
	PrintSuccess(env);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_SET::SetPar(BIC_ENV *env, COMMAND_GROUP *tGroup, struct BIC_GCM_SET_PAR *tPar){
	STDSTR strPrintData;
	if (tGroup != nullptr){
		tGroup->Spin_InUse_set();
		if ((tPar->flag & GL_setSelect) != 0)
			tGroup->blEnableAutoRun = tPar->s;
		if ((tPar->flag & GL_setCycle) != 0)
			tGroup->autoRunTimes = tPar->c;
		if ((tPar->flag & GL_setInterval) != 0)
			tGroup->intervalTime = tPar->t;
		tGroup->Spin_InUse_clr();
		PrintResult(env,Data(COL_clDefault,&tGroup->Compose(&strPrintData,GL_showCycle | GL_showInterval)));
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_SET::SetPar(BIC_ENV *env, GC_LIST *tGroupList, struct BIC_GCM_SET_PAR *tPar){
	if (tGroupList == nullptr)
		return 0;
	TREE_LChildRChain_Traversal_LINE(COMMAND_GROUP,tGroupList,
		if (((tPar->flag & GL_Select) != 0) && (operateNode_t->blEnableAutoRun != 0))
			SetPar(env,operateNode_t,tPar);
		if (((tPar->flag & GL_Select) == 0) && (operateNode_t->blEnableAutoRun == 0))
			SetPar(env,operateNode_t,tPar);
	);
	return 1;
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_CLONE::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	COMMAND_GROUP	*nextGroup,*newGroup;
	STDSTR			strPar1,strPar2;
	uint32			groupDID;
	
	newGroup = nullptr;
	nextGroup = nullptr;
	if (par.length() > 0){
		SplitPar1(strPar1, strPar2, par);
		while(strPar1.length() > 0){
			groupDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
			nextGroup = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&env->cSDTApp->m_GCList,groupDID);
			if (nextGroup != nullptr){
				if (newGroup == nullptr){
					newGroup = (COMMAND_GROUP*)env->cSDTApp->m_GCList.GetNewNode();
					if (newGroup != nullptr){
						newGroup->name = "new copy group";
						COMMAND_GROUP::CopyCOMMAND_GROUP(newGroup,nextGroup,0);
					}
				}
				else{
					COMMAND_GROUP::CopyCOMMAND_GROUP(newGroup,nextGroup,0);
				}
			}
			SplitPar1(&strPar1, &strPar2);
		}
	}
	else{
		newGroup = (COMMAND_GROUP*)env->cSDTApp->m_GCList.GetNewNode();
		if (newGroup != nullptr){
			newGroup->name = "new group";
			COMMAND_GROUP::CopyCOMMAND_GROUP(newGroup,nextGroup,0);
		}
	}
	
	if (newGroup != nullptr){
		env->cSDTApp->m_GCList.AddNode(newGroup);
		BIC_GCM_LS::BIC_GCM_LS_PringGroup(env,newGroup, CL_showCycle | CL_showTimeout);
		env->cSDTApp->m_GCList.SetblUpdate();
		PrintSuccess(env);
	}
	else{
		PrintFail(env);
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_DEL::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	STDSTR		strPar1,strPar2;
	uint32		groupDID;
	
	if (par.length() == 0){
		PrintFail(env);
		return(cgReturnCode);
	}
	SplitPar1(strPar1, strPar2, par);
	while(strPar1.length() > 0){
		groupDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
		env->cSDTApp->m_GCList.MoveToTrash(FindInLChildRChainByDRNodeID(&env->cSDTApp->m_GCList, groupDID));
		PrintResult(env,"Delete gID:",Str_ToString(groupDID));
		SplitPar1(&strPar1, &strPar2);
	};
	env->cSDTApp->m_GCList.SetblUpdate();
	PrintSuccess(env);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_MVUP::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	COMMAND_GROUP	*nextGroup;
	STDSTR			strPar1,strPar2;
	uint32			groupDID,step;
	
	if (par.length() > 0){
		SplitPar1(strPar1, strPar2, par);
		groupDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
		step = (uint32)strtol(strPar2.c_str(),nullptr,10);
		if (step == 0)
			step = 1;
		nextGroup = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&env->cSDTApp->m_GCList, groupDID);
		if (nextGroup != nullptr){
			while(step-- > 0)
				MoveUp(nextGroup);
			env->cSDTApp->m_GCList.SetblUpdate();
			PrintSuccess(env);
			return(cgReturnCode);
		}
	}
	PrintFail(env);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_MVDN::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	COMMAND_GROUP	*nextGroup;
	STDSTR			strPar1,strPar2;
	uint32			groupDID,step;
	
	if (par.length() > 0){
		SplitPar1(strPar1, strPar2, par);
		groupDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
		step = (uint32)strtol(strPar2.c_str(),nullptr,10);
		if (step == 0)
			step = 1;
		nextGroup = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&env->cSDTApp->m_GCList, groupDID);
		if (nextGroup != nullptr){
			while(step-- > 0)
				MoveDown(nextGroup);
			env->cSDTApp->m_GCList.SetblUpdate();
			PrintSuccess(env);
			return(cgReturnCode);
		}
	}
	PrintFail(env);
	return(cgReturnCode);}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_EG::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	STDSTR		strData;
	
	PrintNL(env)
	<< COL_DivStar_Default
	<< COL_clYellow
	<< NL << "[grouplist]"
	<< NL << "  [group]"
	<< NL << "    groupName ="
	<< NL << "    intervalTime ="
	<< NL << "    [node]"
	<< NL << "      StrCommand ="
	<< NL << "      blEnableSend ="
	<< NL << "      cmdTail ="
	<< NL << "      StrShowName ="
	<< NL << "      StrTimeout ="
	<< NL << "      StrCycle ="
	<< NL << "      StrContinue ="
	<< NL << "      StrResend ="
	<< NL << "      StrStop ="
	<< NL << "      StrCatch ="
	<< NL << "    [node_end]"
	<< NL << "    [node]"
	<< NL << "    [node_end]"
	<< NL << "  [group_end]"
	<< NL << "  [group]"
	<< NL << "  [group_end]"
	<< NL << "[grouplist_end]"
	<< NL << COL_DivStar_Default
	<< Endl;
	PrintSuccess(env);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GN::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	COMMAND_GROUP	*nextGroup;
	STDSTR			strPar1,strPar2,strPrintData;
	uint32			gDID;
	
	nextGroup = nullptr;
	if (par.length() > 0){
		SplitPar1(strPar1, strPar2, par);
		gDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
		nextGroup = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&env->cSDTApp->m_GCList, gDID);
		if (nextGroup != nullptr){
			nextGroup->Spin_InUse_set();
			nextGroup->name = strPar2;
			nextGroup->Spin_InUse_clr();
			PrintResult(env,Data(COL_clDefault,&nextGroup->Compose(&strPrintData,GL_showCycle | GL_showInterval)));
			env->cSDTApp->m_GCList.SetblUpdate();
			PrintSuccess(env);
			return(cgReturnCode);
		}
	}
	PrintSuccess(env);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//











//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_LS::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	STDSTR			strPar1,strPar2,strcDID,strPrintData;
	uint32			flag_gl,cDID,ret_Code;
	COMMAND_GROUP	*group;
	
	flag_gl = 0;
	strcDID = "";
	group = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&env->cSDTApp->m_GCList, env->gDID);
	if (group == nullptr){
		PrintFail(env,"ERROR gID");
		ret_Code = BI_RETCODE_RETURN;
	}
	else{
		if (par.length() > 0){
			SplitPar1(strPar1, strPar2, par);
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
			BIC_GCM_LS::BIC_GCM_LS_PringGroup(env,group,flag_gl);
		}
		else{
			PrintStrNL(env,Data(COL_clDefault,DEV_LINE_START));
			PrintStrNL(env,COMMAND_NODE::GetTitle(&strPrintData, flag_gl));
			while(strcDID.length() > 0){
				strPar1 = Str_ReadSubItem(&strcDID," ");
				cDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
				BIC_GCM_LS::BIC_GCM_LS_PringCommand(env,(COMMAND_NODE*)FindInLChildRChainByDRNodeID(group, cDID),flag_gl);
			}
			PrintStrNL(env,Data(COL_clDefault,DEV_LINE_START));
		}
		
		PrintSuccess(env);
		ret_Code = cgReturnCode;
	}
	return(ret_Code);
}
//------------------------------------------------------------------------------------------//
struct BIC_GCM_GROUP_SET_PAR{
	int32	s,r,n,blnum;
	uint32	flag;
	STDSTR	c,t;
};
void ClearSetPar(struct BIC_GCM_GROUP_SET_PAR *tPar){
	if (tPar->blnum != 0){
		tPar->flag = 0;
		tPar->c = "";
		tPar->t = "";
		tPar->s = 0;
		tPar->r = 0;
		tPar->n = 0;
		tPar->blnum = 0;
	}
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_SET::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	//set +/- num num
	//set c3 num num
	//set t200 num num
	//set + c3 t300 num num - num num c4 num
	STDSTR			strPar1,strPar2,strPrintData;
	uint32			cDID;
	BIC_GCM_GROUP_SET_PAR	setPar;
	COMMAND_GROUP	*group;
	COMMAND_NODE	*command;
	int32			blupdate;
	
	if (par.length() == 0){
		PrintFail(env);
		return(cgReturnCode);
	}

	group = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&env->cSDTApp->m_GCList, env->gDID);
	if (group == nullptr){
		PrintFail(env,"ERROR gID");
		return(BI_RETCODE_RETURN);
	}
	
	PrintResult(env,Data(COL_clCyan,&COMMAND_NODE::GetTitle(&strPrintData,CL_showCycle | CL_showTimeout)));
	
	command = nullptr;
	setPar.blnum = 1;
	ClearSetPar(&setPar);
	SplitPar1(strPar1, strPar2, par);
	blupdate = 0;
	while(strPar1.length() > 0){
		if (strPar1 == "+"){
			ClearSetPar(&setPar);
			setPar.flag |= CL_setSelect;
			setPar.s = 1;
		}
		else if (strPar1 == "-"){
			ClearSetPar(&setPar);
			setPar.flag |= CL_setSelect;
			setPar.s = 0;
		}
		else if (strPar1 == "+r"){
			ClearSetPar(&setPar);
			setPar.flag |= CL_setCR;
			setPar.r = 1;
		}
		else if (strPar1 == "-r"){
			ClearSetPar(&setPar);
			setPar.flag |= CL_setCR;
			setPar.r = 0;
		}
		else if (strPar1 == "+n"){
			ClearSetPar(&setPar);
			setPar.flag |= CL_setCR;
			setPar.n = 1;
		}
		else if (strPar1 == "-n"){
			ClearSetPar(&setPar);
			setPar.flag |= CL_setCR;
			setPar.n = 0;
		}
		else if ((strPar1.length() > 1) && (strPar1.substr(0,2) == "-c")){
			ClearSetPar(&setPar);
			strPar1 = strPar1.substr(2);
			setPar.flag |= CL_setCycle;
			setPar.c = strPar1;
		}
		else if ((strPar1.length() > 1) && (strPar1.substr(0,2) == "-t")){
			ClearSetPar(&setPar);
			strPar1 = strPar1.substr(2);
			setPar.flag |= CL_setTimeout;
			setPar.t = strPar1;
		}
		else{
			setPar.blnum = 1;
			if (strPar1 == "s"){
				setPar.flag |= CL_Select;
				SetPar(env,group,&setPar);
				blupdate = 1;
			}
			else if (strPar1 == "uns"){
				setPar.flag &= (~CL_Select);
				SetPar(env,group,&setPar);
				blupdate = 1;
			}
			else{
				cDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
				command = (COMMAND_NODE*)FindInLChildRChainByDRNodeID(group, cDID);
				setPar.flag &= (~CL_Select);
				SetPar(env,command,&setPar);
				blupdate = 1;
			}
		}
		SplitPar1(&strPar1, &strPar2);
	}
	if (blupdate == 1)
		env->cSDTApp->m_GCList.SetblUpdate();
	PrintSuccess(env);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_SET::SetPar(BIC_ENV *env, COMMAND_NODE *tCommand, struct BIC_GCM_GROUP_SET_PAR *tPar){
	STDSTR strPrintData;
	if (tCommand != nullptr){
		tCommand->Spin_InUse_set();
		if ((tPar->flag & CL_setSelect) != 0)
			tCommand->blEnableSend = tPar->s;
		if ((tPar->flag & CL_setCycle) != 0)
			tCommand->StrCycle = tPar->c;
		if ((tPar->flag & CL_setTimeout) != 0)
			tCommand->StrTimeout = tPar->t;
		if ((tPar->flag & CL_setCR) != 0){
			if (tPar->r == 0){
				if (tPar->n == 0){
					tCommand->cmdTail = CMD_NONE;
				}
				else{
					tCommand->cmdTail = CMD_N;
				}
			}
			else{
				if (tPar->n == 0){
					tCommand->cmdTail = CMD_R;
				}
				else{
					tCommand->cmdTail = CMD_RN;
				}
			}
		}
		tCommand->Spin_InUse_clr();
		PrintResult(env,Data(COL_clDefault,&tCommand->Compose(&strPrintData, CL_showCycle | CL_showTimeout)));
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_SET::SetPar(BIC_ENV *env, COMMAND_GROUP *tGroup, struct BIC_GCM_GROUP_SET_PAR *tPar){
	
	if (tGroup == nullptr)
		return 0;
	TREE_LChildRChain_Traversal_LINE(COMMAND_NODE,tGroup,
		if (((tPar->flag & CL_Select) != 0) && (operateNode_t->blEnableSend != 0))
			SetPar(env,operateNode_t,tPar);
		if (((tPar->flag & CL_Select) == 0) && (operateNode_t->blEnableSend == 0))
			SetPar(env,operateNode_t,tPar);
	);
	return 1;
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_CLONE::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	COMMAND_NODE	*nextNode,*newNode;
	COMMAND_GROUP	*group;
	uint32			cDID;
	STDSTR	strPrintData;
	
	nextNode = nullptr;
	newNode = nullptr;
	group = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&env->cSDTApp->m_GCList, env->gDID);
	if (group == nullptr){
		PrintFail(env,"ERROR gID");
		return(BI_RETCODE_RETURN);
	}
	
	newNode = group->GetNewNode();
	if (newNode == nullptr){
		PrintFail(env);
		return(cgReturnCode);
	}

	newNode->StrCommand = "new command";
	if (par.length() > 0){
		cDID = (uint32)strtol(par.c_str(),nullptr,10);
		nextNode = (COMMAND_NODE*)FindInLChildRChainByDRNodeID(group, cDID);
		if (nextNode != nullptr)
			COMMAND_NODE::CopyCOMMAND_NODE(newNode,nextNode);
	}
	group->AddNode(newNode);
	PrintStrNL(env,Data(COL_clDefault,DEV_LINE_START));
	PrintStrNL(env,COMMAND_NODE::GetTitle(&strPrintData, CL_showCycle | CL_showTimeout | CL_showDetail));
	BIC_GCM_LS::BIC_GCM_LS_PringCommand(env,newNode, CL_showCycle | CL_showTimeout | CL_showDetail);
	PrintStrNL(env,Data(COL_clDefault,DEV_LINE_START));
	env->cSDTApp->m_GCList.SetblUpdate();
	PrintSuccess(env);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_DEL::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	COMMAND_GROUP	*group;
	STDSTR			strPar1,strPar2;
	uint32			cDID;
	
	group = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&env->cSDTApp->m_GCList, env->gDID);
	if (group == nullptr){
		PrintFail(env,"ERROR gID");
		return(BI_RETCODE_RETURN);
	}

	if (par.length() == 0){
		PrintFail(env);
		return(cgReturnCode);
	}

	SplitPar1(strPar1, strPar2, par);
	while(strPar1.length() > 0){
		cDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
		env->cSDTApp->m_GCList.MoveToTrash(FindInLChildRChainByDRNodeID(group, cDID));
		env->cSDTApp->m_GCList.SetblUpdate();
		PrintResult(env,"Delete cID:",Str_ToString(cDID));
		SplitPar1(&strPar1, &strPar2);
	}
	env->cSDTApp->m_GCList.SetblUpdate();
	PrintSuccess(env);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_MVUP::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	COMMAND_GROUP	*group;
	COMMAND_NODE	*nextNode;
	STDSTR			strPar1,strPar2;
	uint32			cDID,step;
	
	group = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&env->cSDTApp->m_GCList, env->gDID);
	if (group == nullptr){
		PrintFail(env,"ERROR gID");
		return(BI_RETCODE_RETURN);
	}

	if (par.length() > 0){
		SplitPar1(strPar1, strPar2, par);
		cDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
		step = (uint32)strtol(strPar2.c_str(),nullptr,10);
		if (step == 0)
			step = 1;
		nextNode = (COMMAND_NODE*)FindInLChildRChainByDRNodeID(group, cDID);
		if (nextNode != nullptr){
			while(step-- > 0)
				MoveUp(nextNode);
			env->cSDTApp->m_GCList.SetblUpdate();
			PrintSuccess(env);
			return(cgReturnCode);
		}
	}
	PrintFail(env);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_MVDN::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	COMMAND_GROUP	*group;
	COMMAND_NODE	*nextNode;
	STDSTR			strPar1,strPar2;
	uint32			cDID,step;
	
	group = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&env->cSDTApp->m_GCList, env->gDID);
	if (group == nullptr){
		PrintFail(env,"ERROR gID");
		return(BI_RETCODE_RETURN);
	}
	
	if (par.length() > 0){
		SplitPar1(strPar1, strPar2, par);
		cDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
		step = (uint32)strtol(strPar2.c_str(),nullptr,10);
		if (step == 0)
			step = 1;
		nextNode = (COMMAND_NODE*)FindInLChildRChainByDRNodeID(group, cDID);
		if (nextNode != nullptr){
			while(step-- > 0)
				MoveDown(nextNode);
			env->cSDTApp->m_GCList.SetblUpdate();
			PrintSuccess(env);
			return(cgReturnCode);
		}
	}
	PrintFail(env);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_COMMAND::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	COMMAND_GROUP	*group;
	COMMAND_NODE	*command;
	STDSTR			strPar1,strPar2,strPrintData;
	uint32			cDID;
	
	group = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&env->cSDTApp->m_GCList, env->gDID);
	if (group == nullptr){
		PrintFail(env,"ERROR gID");
		return(BI_RETCODE_RETURN);
	}

	if (par.length() > 0){
		SplitPar1(strPar1, strPar2, par);
		cDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
		command = (COMMAND_NODE*)FindInLChildRChainByDRNodeID(group, cDID);
		if (command != nullptr){
			command->Spin_InUse_set();
			command->StrCommand = strPar2;
			command->Spin_InUse_clr();
			PrintStrNL(env,Data(COL_clDefault,DEV_LINE_START));
			PrintStrNL(env,COMMAND_NODE::GetTitle(&strPrintData, CL_showCycle | CL_showTimeout | CL_showDetail));
			BIC_GCM_LS::BIC_GCM_LS_PringCommand(env,command, CL_showCycle | CL_showTimeout | CL_showDetail);
			PrintStrNL(env,Data(COL_clDefault,DEV_LINE_START));
			env->cSDTApp->m_GCList.SetblUpdate();
			PrintSuccess(env);
			return(cgReturnCode);
		}
	}
	PrintFail(env);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_CONTINUE::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	COMMAND_GROUP	*group;
	COMMAND_NODE	*command;
	STDSTR			strPar1,strPar2,strPrintData;
	uint32			cDID;
	
	group = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&env->cSDTApp->m_GCList, env->gDID);
	if (group == nullptr){
		PrintFail(env,"ERROR gID");
		return(BI_RETCODE_RETURN);
	}
	
	if (par.length() > 0){
		SplitPar1(strPar1, strPar2, par);
		cDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
		command = (COMMAND_NODE*)FindInLChildRChainByDRNodeID(group, cDID);
		if (command != nullptr){
			command->Spin_InUse_set();
			command->StrContinue = strPar2;
			command->Spin_InUse_clr();
			PrintStrNL(env,Data(COL_clDefault,DEV_LINE_START));
			PrintStrNL(env,COMMAND_NODE::GetTitle(&strPrintData, CL_showCycle | CL_showTimeout | CL_showDetail));
			BIC_GCM_LS::BIC_GCM_LS_PringCommand(env,command, CL_showCycle | CL_showTimeout | CL_showDetail);
			PrintStrNL(env,Data(COL_clDefault,DEV_LINE_START));
			env->cSDTApp->m_GCList.SetblUpdate();
			PrintSuccess(env);
			return(cgReturnCode);
		}
	}
	PrintFail(env);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_RESEND::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	COMMAND_GROUP	*group;
	COMMAND_NODE	*command;
	STDSTR			strPar1,strPar2,strPrintData;
	uint32			cDID;
	
	group = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&env->cSDTApp->m_GCList, env->gDID);
	if (group == nullptr){
		PrintFail(env,"ERROR gID");
		return(BI_RETCODE_RETURN);
	}
	
	if (par.length() > 0){
		SplitPar1(strPar1, strPar2, par);
		cDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
		command = (COMMAND_NODE*)FindInLChildRChainByDRNodeID(group, cDID);
		if (command != nullptr){
			command->Spin_InUse_set();
			command->StrResend = strPar2;
			command->Spin_InUse_clr();
			PrintStrNL(env,Data(COL_clDefault,DEV_LINE_START));
			PrintStrNL(env,COMMAND_NODE::GetTitle(&strPrintData, CL_showCycle | CL_showTimeout | CL_showDetail));
			BIC_GCM_LS::BIC_GCM_LS_PringCommand(env,command, CL_showCycle | CL_showTimeout | CL_showDetail);
			PrintStrNL(env,Data(COL_clDefault,DEV_LINE_START));
			env->cSDTApp->m_GCList.SetblUpdate();
			PrintSuccess(env);
			return(cgReturnCode);
		}
	}
	PrintFail(env);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_CSTOP::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	COMMAND_GROUP	*group;
	COMMAND_NODE	*command;
	STDSTR			strPar1,strPar2,strPrintData;
	uint32			cDID;
	
	group = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&env->cSDTApp->m_GCList, env->gDID);
	if (group == nullptr){
		PrintFail(env,"ERROR gID");
		return(BI_RETCODE_RETURN);
	}
	
	if (par.length() > 0){
		SplitPar1(strPar1, strPar2, par);
		cDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
		command = (COMMAND_NODE*)FindInLChildRChainByDRNodeID(group, cDID);
		if (command != nullptr){
			command->Spin_InUse_set();
			command->StrStop = strPar2;
			command->Spin_InUse_clr();
			PrintStrNL(env,Data(COL_clDefault,DEV_LINE_START));
			PrintStrNL(env,COMMAND_NODE::GetTitle(&strPrintData, CL_showCycle | CL_showTimeout | CL_showDetail));
			BIC_GCM_LS::BIC_GCM_LS_PringCommand(env,command, CL_showCycle | CL_showTimeout | CL_showDetail);
			PrintStrNL(env,Data(COL_clDefault,DEV_LINE_START));
			env->cSDTApp->m_GCList.SetblUpdate();
			PrintSuccess(env);
			return(cgReturnCode);
		}
	}
	PrintFail(env);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_CATCH::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	COMMAND_GROUP	*group;
	COMMAND_NODE	*command;
	STDSTR			strPar1,strPar2,strPrintData;
	uint32			cDID;
	
	group = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&env->cSDTApp->m_GCList, env->gDID);
	if (group == nullptr){
		PrintFail(env,"ERROR gID");
		return(BI_RETCODE_RETURN);
	}
	
	if (par.length() > 0){
		SplitPar1(strPar1, strPar2, par);
		cDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
		command = (COMMAND_NODE*)FindInLChildRChainByDRNodeID(group, cDID);
		if (command != nullptr){
			command->Spin_InUse_set();
			command->StrCatch = strPar2;
			command->Spin_InUse_clr();
			PrintStrNL(env,Data(COL_clDefault,DEV_LINE_START));
			PrintStrNL(env,COMMAND_NODE::GetTitle(&strPrintData, CL_showCycle | CL_showTimeout | CL_showDetail));
			BIC_GCM_LS::BIC_GCM_LS_PringCommand(env,command, CL_showCycle | CL_showTimeout | CL_showDetail);
			PrintStrNL(env,Data(COL_clDefault,DEV_LINE_START));
			env->cSDTApp->m_GCList.SetblUpdate();
			PrintSuccess(env);
			return(cgReturnCode);
		}
	}
	PrintFail(env);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_SEND::InPressKeyModeExit(BIC_ENV *env)const{
	return(env->cSDTApp->m_Script.IsStop());
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_SEND::Command(BIC_ENV *env, const STDSTR &par,void *eda)const{
	ExpandDeviceAttr* attr = static_cast<ExpandDeviceAttr*>(eda);
	COMMAND_GROUP	*group;
	COMMAND_NODE	command,*node;
	uint32			cDID;
	
	group = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&env->cSDTApp->m_GCList, env->gDID);
	if (group == nullptr){
		PrintFail(env,"ERROR gID");
		return(BI_RETCODE_RETURN);
	}

	command.Init();
	
	if (par.length() > 0){
		cDID = (uint32)strtol(par.c_str(),nullptr,10);
		node = (COMMAND_NODE*)FindInLChildRChainByDRNodeID(group, cDID);
		if (node != nullptr){
			COMMAND_NODE::CopyCOMMAND_NODE(&command,node);
			command.blEnableSend = 1;
			env->cstdout->PrintEnable();
			if (env->cSDTApp->m_Script.Execute(attr->device,&command) == 0){
				PrintFail(env,"script is running");
				return(cgReturnCode);
			}
			if (InPressKeyMode(env) > 0){
				env->cSDTApp->m_Script.Stop();
				PrintSuccess(env,"Pressed ESC key, stopped script");
			}
			return(cgReturnCode);
		}
	}
	PrintFail(env);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP::Command(BIC_ENV *env, const STDSTR &par,void *eda)const{
	int32			retCode;
	COMMAND_GROUP	*nextGroup;
	uint32			gDID;
	STDSTR			strPar1,strPar2;
	
	nextGroup = nullptr;
	if (par.length() > 0){
		SplitPar1(strPar1, strPar2, par);
		gDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
		nextGroup = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&env->cSDTApp->m_GCList, gDID);
		if (nextGroup != nullptr){
			env->gDID = gDID;
			retCode = ExecuteAsConsole(env,strPar2,"-" + Str_ToString(gDID),eda);
			env->gDID = 0;
			if (retCode == BI_RETCODE_RETURN)
				return(cgReturnCode);
			return(retCode);
		}
	}
	PrintFail(env);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
#endif
