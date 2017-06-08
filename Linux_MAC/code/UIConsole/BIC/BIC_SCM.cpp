//
//  BIC_SCM.cpp
//  SDT
//
//  Created by Leif Wen on 05/04/2017.
//  Copyright © 2017 leif wen. All rights reserved.
//
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "BIC_SCM.h"
#include "Comm_Convert.h"
#include "Comm_File.h"
#include "AppLayer.h"
//------------------------------------------------------------------------------------------//
#ifdef BIC_SCM_hpp
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_SCM_EG::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	STDSTR		strData;
	
	PrintNL(env)
	<< COL_DivStar_Default
	<< COL_clYellow
	<< NL << "[scNode]"
	<< NL << "  blEnableCR ="
	<< NL << "  strCommand ="
	<< NL << "[scNode_end]"
	<< NL << COL_DivStar_Default
	<< Endl;
	PrintSuccess(env);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_SCM_LOAD::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	uint32	readType;
	STDSTR	fileN;
	STDSTR	strPar;
	strPar = par;
	strPar = Str_SplitSubItem(&strPar, ' ');
	
	readType = READINITYPE_NONE | (READINITYPE_AddSingleCommand | READINITYPE_CoverSingleCommand);
	fileN = "Default.ini";
	if (par.length() > 0)
		fileN = strPar;
	
	if (CFS_CheckFile(fileN) > 0){
		env->cSDTApp->m_ParRecord.ReadScriptFromIniFile(fileN,readType);
		env->cSDTApp->m_SCList.SetblUpdate();
		PrintSuccess(env,"Read from <" + fileN + ">");
	}
	else{
		PrintFail(env);
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_SCM_SAVE::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
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
//------------------------------------------------------------------------------------------//
int32 BIC_SCM_LS::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	int32		j;
	STDSTR		strPrintData;
	SC_NODE		*fromNode,*breakNode;
	uint32		fromNodeDRID;
	
	PrintNL(env) << COL_DivStar_Default << SC_NODE::GetTitle(&strPrintData) << "\n" << Endl;
	
	j = 0;
	
	fromNode =nullptr;
	breakNode = nullptr;
	fromNodeDRID = 0;
	do{
		if (breakNode == nullptr){
			fromNode = (SC_NODE*)GetcgDown(&env->cSDTApp->m_SCList);
		}
		else{
			breakNode = nullptr;
			fromNode = (SC_NODE*)FindInLChildRChainByDRNodeID(&env->cSDTApp->m_SCList, fromNodeDRID);
		}
		TREE_RChain_Traversal_LINE(SC_NODE,fromNode,
			PrintStrNL(env,Data(COL_clDefault,&operateNode_t->Compose(&strPrintData)));
			if ((env->blExit == 0) && (++j > 19) && (nextNode_t != nullptr)){
				breakNode = nextNode_t;
				fromNodeDRID = GetdRNodeID(breakNode);
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
	PrintSuccess(env);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_SCM_SET::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	STDSTR		strPar1,strPar2,strPrintData;
	uint32		sID;
	uint32		blCR;
	SC_NODE		*selectNode;
	int32		blupdate;
	
	blCR = 0;
	
	if (par.length() == 0){
		PrintFail(env);
		return(cgReturnCode);
	}
	PrintResult(env,Data(COL_clCyan,&SC_NODE::GetTitle(&strPrintData)));
	
	SplitPar1(strPar1, strPar2, par);
	blupdate = 0;
	while(strPar1.length() > 0){
		if (strPar1 == "+cr"){
			blCR = 1;
		}
		else if (strPar1 == "-cr"){
			blCR = 0;
		}
		else if (strPar1 == "all"){
			TREE_LChildRChain_Traversal_LINE(SC_NODE,(&env->cSDTApp->m_SCList),
				operateNode_t->Spin_InUse_set();
				operateNode_t->blEnableSendCR = blCR;
				operateNode_t->Spin_InUse_clr();
				PrintResult(env,Data(COL_clBlue,&operateNode_t->Compose(&strPrintData)));
			);
			blupdate = 1;
		}
		else{
			sID = (uint32)strtol(strPar1.c_str(),nullptr,10);
			selectNode = (SC_NODE*)FindInLChildRChainByDRNodeID(&env->cSDTApp->m_SCList, sID);
			if (selectNode != nullptr){
				selectNode->Spin_InUse_set();
				selectNode->blEnableSendCR = blCR;
				selectNode->Spin_InUse_clr();
				PrintResult(env,Data(COL_clBlue,&selectNode->Compose(&strPrintData)));
				blupdate = 1;
			}
		}
		SplitPar1(&strPar1, &strPar2);
	}
	if (blupdate == 1)
		env->cSDTApp->m_SCList.SetblUpdate();
	PrintSuccess(env);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_SCM_CLONE::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	SC_NODE		*nextNode,*newNode;
	uint32		sDID;
	STDSTR		strPrintData;
	
	newNode = (SC_NODE*)env->cSDTApp->m_SCList.GetNewNode();
	if (newNode == nullptr){
		PrintFail(env);
		return(cgReturnCode);
	}
	
	newNode->StrCommand = "new command";
	if (par.length() > 0){
		sDID = (uint32)strtol(par.c_str(),nullptr,10);
		nextNode = (SC_NODE*)FindInLChildRChainByDRNodeID(&env->cSDTApp->m_SCList, sDID);
		if (nextNode != nullptr)
			SC_NODE::Copy(newNode,nextNode);
	}
	env->cSDTApp->m_SCList.AddNode(newNode);
	PrintResult(env,Data(COL_clBlue,&newNode->Compose(&strPrintData)));
	env->cSDTApp->m_SCList.SetblUpdate();
	PrintSuccess(env);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_SCM_DEL::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	STDSTR		strPar1,strPar2;
	uint32		sDID;
	
	if (par.length() == 0){
		PrintFail(env);
		return(cgReturnCode);
	}
	SplitPar1(strPar1, strPar2, par);
	while(strPar1.length() > 0){
		sDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
		env->cSDTApp->m_SCList.MoveToTrash(FindInLChildRChainByDRNodeID(&env->cSDTApp->m_SCList, sDID));
		env->cSDTApp->m_SCList.SetblUpdate();
		PrintResult(env,"Delete sID:",Str_ToString(sDID));
		SplitPar1(&strPar1, &strPar2);
	};
	env->cSDTApp->m_SCList.SetblUpdate();
	PrintSuccess(env);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_SCM_MVUP::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	SC_NODE		*nextNode;
	STDSTR		strPar1,strPar2;
	uint32		sDID,step;
	
	if (par.length() > 0){
		SplitPar1(strPar1, strPar2, par);
		sDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
		step = (uint32)strtol(strPar2.c_str(),nullptr,10);
		if (step == 0)
			step = 1;
		nextNode = (SC_NODE*)FindInLChildRChainByDRNodeID(&env->cSDTApp->m_SCList, sDID);
		if (nextNode != nullptr){
			while(step-- > 0)
				MoveUp(nextNode);
			env->cSDTApp->m_SCList.SetblUpdate();
			PrintSuccess(env);
			return(cgReturnCode);
		}
	}
	PrintFail(env);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_SCM_MVDN::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	SC_NODE		*nextNode;
	STDSTR		strPar1,strPar2;
	uint32		sDID,step;
	
	if (par.length() > 0){
		SplitPar1(strPar1, strPar2, par);
		sDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
		step = (uint32)strtol(strPar2.c_str(),nullptr,10);
		if (step == 0)
			step = 1;
		nextNode = (SC_NODE*)FindInLChildRChainByDRNodeID(&env->cSDTApp->m_SCList, sDID);
		if (nextNode != nullptr){
			while(step-- > 0)
				MoveDown(nextNode);
			env->cSDTApp->m_SCList.SetblUpdate();
			PrintSuccess(env);
			return(cgReturnCode);
		}
	}
	PrintFail(env);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_SCM_COMMAND::Command(BIC_ENV *env, const STDSTR &par,void *p)const{
	SC_NODE		*nextNode;
	STDSTR		strPar1,strPar2,strPrintData;
	uint32		sDID;
	
	nextNode = nullptr;
	if (par.length() > 0){
		SplitPar1(strPar1, strPar2, par);
		sDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
		nextNode = (SC_NODE*)FindInLChildRChainByDRNodeID(&env->cSDTApp->m_SCList, sDID);
		if (nextNode != nullptr){
			nextNode->Spin_InUse_set();
			nextNode->StrCommand = strPar2;
			nextNode->Spin_InUse_clr();
			PrintResult(env,Data(COL_clBlue,&nextNode->Compose(&strPrintData)));
			env->cSDTApp->m_SCList.SetblUpdate();
			PrintSuccess(env);
			return(cgReturnCode);
		}
	}
	PrintFail(env);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_SCM_SEND::Command(BIC_ENV *env, const STDSTR &par,void *eda)const{
	ExpandDeviceAttr* attr = static_cast<ExpandDeviceAttr*>(eda);
	SC_NODE		*nextNode;
	STDSTR		StrCommand;
	int32		blEnableSendCR;
	uint32		sDID;
	
	StrCommand = "";
	nextNode = nullptr;
	if (env->cSDTApp->m_Device.IsConnected() != 0){
		if (par.length() > 0){
			sDID = (uint32)strtol(par.c_str(),nullptr,10);
			nextNode = (SC_NODE*)FindInLChildRChainByDRNodeID(&env->cSDTApp->m_SCList, sDID);
			if (nextNode != nullptr){
				nextNode->Spin_InUse_set();
				StrCommand = nextNode->StrCommand;
				blEnableSendCR = nextNode->blEnableSendCR;
				nextNode->Spin_InUse_clr();
				env->cstdout->PrintEnable();
				attr->device->SendCommandWithPrint(StrCommand,(CMD_TAIL)blEnableSendCR,G_ESCAPE_ON);
				PressAnyKey(env,attr);
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
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
