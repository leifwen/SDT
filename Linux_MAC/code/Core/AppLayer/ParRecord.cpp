/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: ParRecord.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.11.13
 *
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "ParRecord.h"
#include "Comm_Convert.h"
#include "Comm_File.h"
#include "ComEnum.h"
#include "CommandGList.h"
#include "CommandSList.h"
//------------------------------------------------------------------------------------------//
void PR_Record::Init(TExpandPar *cExpandPar){
	cg_ExpandPar.g_IPComList = cExpandPar->g_IPComList;
	cg_ExpandPar.g_SCList = cExpandPar->g_SCList;
	cg_ExpandPar.g_GCList = cExpandPar->g_GCList;
}
//------------------------------------------------------------------------------------------//
void PR_Record::WriteScriptToIniFile(const std::string &strFileName,uint32 ReadPart){
	WriteScriptToIniFileV0_5(strFileName,ReadPart);
}
//------------------------------------------------------------------------------------------//
void PR_Record::WriteScriptToIniFileV0_5(const std::string &strFileName,uint32 ReadPart){
	/*
	 //V0.5
		V0.5:[V0.5]
		[ipcomlist]
	 		[ipcom]
				typeID =
				strIPComName =
				portBaudrate =
				tcpTypeOrFriendlyName =
				strUserDefineName =
			[ipcom_end]
			[ipcom]
			[ipcom_end]
		[ipcomlist_end]
		[singleCommand]
			[node]
				blEnableCR =
				strCommand =
	 		[node_end]
	 		[node]
	 		[node_end]
		[singleCommand_end]
		[grouplist]
			[group]
				groupName =
				intervalTime =
				[node]
					blEnableSend =
					blEnableSendCR =
					blEnableHEX =
					strTimeout =
					strCommand =
					strContinue =
					strStop =
					strResend =
					strCycle =
					strCatch =
				[node_end]
				[node]
				[node_end]
			[group_end]
			[group]
			[group_end]
		[grouplist_end]
	 */
	std::string		strResult,strTemp;
	IPCOMLIST		*iIPComList;
	GC_LIST			*iGroupList;
	SC_LIST			*iSCList;
	
	iIPComList = cg_ExpandPar.g_IPComList;
	iGroupList = cg_ExpandPar.g_GCList;
	iSCList = cg_ExpandPar.g_SCList;
	
	strResult = "[V0.5]\n";
	CFS_WriteFile(strFileName,strResult);
	if (ReadPart & READINITYPE_AddDeviceList)
		CFS_AddToFile(strFileName,iIPComList->CreateStrV0_5());
	
	if (ReadPart & READINITYPE_AddSingleCommand)
		CFS_AddToFile(strFileName,iSCList->CreateSCListStrV0_5());
	
	if (ReadPart & READINITYPE_AddGroupCommand)
		CFS_AddToFile(strFileName,iGroupList->CreateGroupListStrV0_5());
}
//------------------------------------------------------------------------------------------//
void PR_Record::WriteScriptToIniFileV0_4(const std::string &strFileName,uint32 ReadPart){
	/*
	 //V0.4
		V0.4:[V0.4],
		[ipcomlist]
			[ipcom]
				typeID =
				strIPComName =
				portBaudrate =
				tcpTypeOrFriendlyName =
				strUserDefineName =
			[ipcom_end]
			[ipcom]
			[ipcom_end]
		[ipcomlist_end]
		[singleCommand]
			[node]
				blEnableCR =
				strCommand =
			[node_end]
			[node]
			[node_end]
		[singleCommand_end]
		[grouplist]
			[group]
				groupName =
				intervalTime =
				[node]
					blEnableSend =
					blEnableSendCR =
					blEnableHEX =
					strTimeout =
					strCommand =
					strContinue =
					strStop =
					strResend =
					strCycle =
					strCatch =
				[node_end]
				[node]
				[node_end]
			[group_end]
			[group]
			[group_end]
		[grouplist_end]
	 */
	std::string		strResult,strTemp;
	IPCOMLIST		*iIPComList;
	GC_LIST			*iGroupList;
	SC_LIST			*iSCList;
	
	iIPComList = cg_ExpandPar.g_IPComList;
	iGroupList = cg_ExpandPar.g_GCList;
	iSCList = cg_ExpandPar.g_SCList;
	
	strResult = "[V0.4]\n";
	CFS_WriteFile(strFileName,strResult);
	if (ReadPart & READINITYPE_AddDeviceList)
		CFS_AddToFile(strFileName,iIPComList->CreateStrV0_4());
	
	if (ReadPart & READINITYPE_AddSingleCommand)
		CFS_AddToFile(strFileName,iSCList->CreateSCListStrV0_4());
	
	if (ReadPart & READINITYPE_AddGroupCommand)
		CFS_AddToFile(strFileName,iGroupList->CreateGroupListStrV0_4());
}
//------------------------------------------------------------------------------------------//
void PR_Record::WriteScriptToIniFileV0_3(const std::string &strFileName,uint32 ReadPart){
	/*
	 V0.3
		V0.3:[V0.3]
	 [V0.3]{}{DeviceList}{SingleCommand}{Group1}{Group2}
	 [V0.3]
	 {(typeID,strIPComName,portBaudrate,TCPTypeOrFriendlyName,strUserDefineName)(IPCOMNAME2)}
	 {(hexEnable,command)(hexEnable,command)}
	 {(GroupName,frameDelayTime)(sendEnable,enabled,hexEnable,timeout,command,continue1,stop1,resend,cycle)(Node2)}
	 {Group2}
	 
	 {}
	 */
	std::string		strResult,strTemp;
	IPCOMLIST		*iIPComList;
	GC_LIST			*iGroupList;
	SC_LIST			*iSCList;
	
	iIPComList = cg_ExpandPar.g_IPComList;
	iGroupList = cg_ExpandPar.g_GCList;
	iSCList = cg_ExpandPar.g_SCList;
	
	strResult = "[V0.3]{}";
	if (ReadPart & READINITYPE_AddDeviceList){
		strResult += iIPComList->CreateStrV0_3();
	}
	else{
		strResult += "{}";
	}
	if (ReadPart & READINITYPE_AddSingleCommand){
		strResult += iSCList->CreateSCListStrV0_3();
	}
	else{
		strResult += "{}";
	}
	if (ReadPart & READINITYPE_AddGroupCommand){
		strResult += iGroupList->CreateGroupListStrV0_3();
	}
	
	CFS_WriteFile(strFileName,strResult);
}
//------------------------------------------------------------------------------------------//
void PR_Record::WriteScriptToIniFileV0_2(const std::string &strFileName){
	/*
	 V0.2
	 Node:
	 (blEnableSend,blEnableSendCR,blEnableHEX,StrTimeout,StrCommand,StrContinue,StrStop,StrResend,StrCycle,StrCatch)
	 Group:,GroupName,frameDelayTime,count(Node),action
	 {(GroupName,frameDelayTime)(Node1)(Node2)}
	 Group List: Group
	 {Group1}{Group2}
	 SingleCommand
	 {(hexEnable,command)(hexEnable,command)}
		V0.2:[V0.2]
	 [V0.2]{SingleCommand}{Group}
	 [V0.2]
	 {(hexEnable,command)
	 (hexEnable,command)}
	 {(GroupName,frameDelayTime)
	 (sendEnable,enabled,hexEnable,timeout,command,continue1,stop1,resend,cycle)
	 (Node2)}
	 {Group2}
	 
	 */
	std::string		strResult,strTemp;
	GC_LIST			*iGroupList;
	SC_LIST			*iSCList;
	
	iGroupList = cg_ExpandPar.g_GCList;
	iSCList = cg_ExpandPar.g_SCList;
	
	strResult = "[V0.2]";
	strResult += iSCList->CreateSCListStrV0_2();
	strResult += iGroupList->CreateGroupListStrV0_2();
	
	strResult = Str_Replace(strResult," ","");
	
	CFS_WriteFile(strFileName,strResult);
}
//------------------------------------------------------------------------------------------//
void PR_Record::ReadScriptFromIniFile(const std::string &strFileName,uint32 ReadPart){
	std::string		strResult,strTemp;
	std::string::size_type	length;
	
	CFS_ReadFile(&strResult,strFileName);
	
	length = strResult.length();
	if (length > 6){
		strTemp = strResult.substr(0,6);
		if (strTemp == "[V0.2]"){						//V0.2
			strResult = strResult.substr(6,length - 6);
			ReadScriptFromIniFileV0_2(strResult,ReadPart);
			return;
		}
		if (strTemp == "[V0.3]"){						//V0.3
			strResult = strResult.substr(6,length - 6);
			ReadScriptFromIniFileV0_3(strResult,ReadPart);
			return;
		}
		if (strTemp == "[V0.4]"){						//V0.4
			strResult = strResult.substr(6,length - 6);
			ReadScriptFromIniFileV0_4(&strResult,ReadPart);
			return;
		}
		if (strTemp == "[V0.5]"){						//V0.5
			strResult = strResult.substr(6,length - 6);
			ReadScriptFromIniFileV0_5(&strResult,ReadPart);
			return;
		}
	}
}
//------------------------------------------------------------------------------------------//
void PR_Record::ReadScriptFromIniFileV0_5(std::string *strInput,uint32 ReadPart){
	/*
		V0.5.
		V0.5
		[ipcomlist]
	 [ipcom]
	 typeID =
	 strIPComName =
	 portBaudrate =
	 tcpTypeOrFriendlyName =
	 strUserDefineName =
	 [ipcom_end]
	 [ipcom]
	 [ipcom_end]
		[ipcomlist_end]
		[singleCommand]
	 [scNode]
	 blEnableCR =
	 strCommand =
	 [scNode_end]
	 [scNode]
	 [scNode_end]
		[singleCommand_end]
		[grouplist]
	 [group]
	 groupName =
	 intervalTime =
	 [node]
	 blEnableSend =
	 blEnableSendCR =
	 blEnableHEX =
	 strTimeout =
	 strCommand =
	 strContinue =
	 strStop =
	 strResend =
	 strCycle =
	 strCatch =
	 [node_end]
	 [node]
	 [node_end]
	 [group_end]
	 [group]
	 [group_end]
		[grouplist_end]
	 */
	std::string		strLine;
	IPCOMLIST		*iIPComList;
	GC_LIST			*iGroupList;
	SC_LIST			*iSCList;
	
	iIPComList = cg_ExpandPar.g_IPComList;
	iGroupList = cg_ExpandPar.g_GCList;
	iSCList = cg_ExpandPar.g_SCList;
	
	if (ReadPart & READINITYPE_CoverDeviceList)
		iIPComList->DestroyAll();
	if (ReadPart & READINITYPE_CoverSingleCommand)
		iSCList->DestroyAll();
	if (ReadPart & READINITYPE_CoverGroupCommand)
		iGroupList->DestroyAll();
	
	while(strInput->length() > 0){
		strLine = Str_Trim(Str_ReadSubItem(strInput,"\n"));
		if (strLine == "[ipcomlist]"){
			if (ReadPart & READINITYPE_AddDeviceList)
				iIPComList->SetIPComListV0_5(strInput);
		}
		else if (strLine == "[singleCommand]"){
			if (ReadPart & READINITYPE_AddSingleCommand)
				iSCList->SetSCListV0_5(strInput);
		}
		else if (strLine == "[grouplist]"){
			if (ReadPart & READINITYPE_AddGroupCommand)
				iGroupList->SetGroupListV0_5(strInput);
		}
	}
}
//------------------------------------------------------------------------------------------//
void PR_Record::ReadScriptFromIniFileV0_4(std::string *strInput,uint32 ReadPart){
	/*
		V0.4
		V0.4:[V0.4],
		[ipcomlist]
	 [ipcom]
	 typeID =
	 strIPComName =
	 portBaudrate =
	 tcpTypeOrFriendlyName =
	 strUserDefineName =
	 [ipcom_end]
	 [ipcom]
	 [ipcom_end]
		[ipcomlist_end]
		[singleCommand]
	 [scNode]
	 blEnableCR =
	 strCommand =
	 [scNode_end]
	 [scNode]
	 [scNode_end]
		[singleCommand_end]
		[grouplist]
	 [group]
	 groupName =
	 intervalTime =
	 [node]
	 blEnableSend =
	 blEnableSendCR =
	 blEnableHEX =
	 strTimeout =
	 strCommand =
	 strContinue =
	 strStop =
	 strResend =
	 strCycle =
	 strCatch =
	 [node_end]
	 [node]
	 [node_end]
	 [group_end]
	 [group]
	 [group_end]
		[grouplist_end]
	 */
	std::string		strLine;
	IPCOMLIST		*iIPComList;
	GC_LIST			*iGroupList;
	SC_LIST			*iSCList;
	
	iIPComList = cg_ExpandPar.g_IPComList;
	iGroupList = cg_ExpandPar.g_GCList;
	iSCList = cg_ExpandPar.g_SCList;
	
	if (ReadPart & READINITYPE_CoverDeviceList)
		iIPComList->DestroyAll();
	if (ReadPart & READINITYPE_CoverSingleCommand)
		iSCList->DestroyAll();
	if (ReadPart & READINITYPE_CoverGroupCommand)
		iGroupList->DestroyAll();
	
	while(strInput->length() > 0){
		strLine = Str_Trim(Str_ReadSubItem(strInput,"\n"));
		if (strLine == "[ipcomlist]"){
			if (ReadPart & READINITYPE_AddDeviceList)
				iIPComList->SetIPComListV0_4(strInput);
		}
		else if (strLine == "[singleCommand]"){
			if (ReadPart & READINITYPE_AddSingleCommand)
				iSCList->SetSCListV0_4(strInput);
		}
		else if (strLine == "[grouplist]"){
			if (ReadPart & READINITYPE_AddGroupCommand)
				iGroupList->SetGroupListV0_4(strInput);
		}
	}
}
//------------------------------------------------------------------------------------------//
void PR_Record::ReadScriptFromIniFileV0_3(std::string strInput,uint32 ReadPart){
	/*
	 V0.3
		V0.3 :[V0.3]
	 [V0.3]{}{DeviceList}{SingleCommand}{Group1}{Group2}
	 [V0.3]
	 {(typeID,strIPComName,portBaudrate,TCPTypeOrFriendlyName,strUserDefineName)(IPCOMNAME2)}
	 {(hexEnable,command)(hexEnable,command)}
	 {(GroupName,frameDelayTime)(sendEnable,enabled,hexEnable,timeout,command,continue1,stop1,resend,cycle)(Node2)}
	 {Group2}
	 
	 {}
	 */
	std::string		strResult,strTemp;
	IPCOMLIST		*iIPComList;
	GC_LIST			*iGroupList;
	SC_LIST			*iSCList;
	
	iIPComList = cg_ExpandPar.g_IPComList;
	iGroupList = cg_ExpandPar.g_GCList;
	iSCList = cg_ExpandPar.g_SCList;
	
	if (ReadPart & READINITYPE_CoverDeviceList)
		iIPComList->DestroyAll();
	if (ReadPart & READINITYPE_CoverSingleCommand)
		iSCList->DestroyAll();
	if (ReadPart & READINITYPE_CoverGroupCommand)
		iGroupList->DestroyAll();
	
	strInput = Str_Replace(strInput,"}{",":");
	strInput = Str_Replace(strInput,")(","|");
	strInput = Str_Replace(strInput,"{","");
	strInput = Str_Replace(strInput,"}","");
	strInput = Str_Replace(strInput,")","");
	strInput = Str_Replace(strInput,"(","");
	
	if (strInput.length() > 0){
		strResult = Str_ReadSubItem(&strInput,":");//{}
		strResult = Str_ReadSubItem(&strInput,":");//{DeviceList}
		if (ReadPart & READINITYPE_AddDeviceList)
			iIPComList->SetIPComListV0_3(&strResult);
		strResult = Str_ReadSubItem(&strInput,":");//{SingleCommand}
		if (ReadPart & READINITYPE_AddSingleCommand)
			iSCList->SetSCListV0_3(&strResult);
		if (ReadPart & READINITYPE_AddGroupCommand)
			iGroupList->SetGroupListV0_3(&strInput);//{GroupCommand}
	}
}
//------------------------------------------------------------------------------------------//
void PR_Record::ReadScriptFromIniFileV0_2(std::string strInput,uint32 ReadPart){
	/*
	 V0.2
	 Node:
	 (blEnableSend,blEnableSendCR,blEnableHEX,StrTimeout,StrCommand,StrContinue,StrStop,StrResend,StrCycle,StrCatch)
	 Group:,GroupName,frameDelayTime,count(Node),action
	 {(GroupName,frameDelayTime)(Node1)(Node2)}
	 Group List: Group
	 {Group1}{Group2}
	 SingleCommand
	 {(hexEnable,command)(hexEnable,command)}
	V0.2 [V0.2]
	 [V0.2]{SingleCommand}{Group}
	 [V0.2]
	 {(hexEnable,command)
	 (hexEnable,command)}
	 {(GroupName,frameDelayTime)
	 (sendEnable,enabled,hexEnable,timeout,command,continue1,stop1,resend,cycle)
	 (Node2)}
	 {Group2}
	 
	 */
	std::string		strResult;
	GC_LIST			*iGroupList;
	SC_LIST			*iSCList;
	
	iGroupList = cg_ExpandPar.g_GCList;
	iSCList = cg_ExpandPar.g_SCList;
	
	if (ReadPart & READINITYPE_CoverSingleCommand)
		iSCList->DestroyAll();
	if (ReadPart & READINITYPE_CoverGroupCommand)
		iGroupList->DestroyAll();
	
	strInput = Str_Replace(strInput,"}{",":");
	strInput = Str_Replace(strInput,")(","|");
	strInput = Str_Replace(strInput,"{","");
	strInput = Str_Replace(strInput,"}","");
	strInput = Str_Replace(strInput,")","");
	strInput = Str_Replace(strInput,"(","");
	
	if (strInput.length() > 0){
		strResult = Str_ReadSubItem(&strInput,":");//{SingleCommand}
		if (ReadPart & READINITYPE_AddSingleCommand)
			iSCList->SetSCListV0_2(&strResult);
		if (ReadPart & READINITYPE_AddGroupCommand)
			iGroupList->SetGroupListV0_2(&strInput);//{GroupCommand}
	}
}
//------------------------------------------------------------------------------------------//
