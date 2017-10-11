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

#ifdef ParRecordH
//------------------------------------------------------------------------------------------//
void PR_Record::Init(IPCOMLIST *iplist,SC_LIST *sclist,GC_LIST *gclist){
	cgIPComList = iplist;
	cgSCList = sclist;
	cgGCList = gclist;
}
//------------------------------------------------------------------------------------------//
void PR_Record::WriteScriptToIniFile(const STDSTR &strFileName,uint32 ReadPart){
	WriteScriptToIniFileV0_6(strFileName,ReadPart);
}
//------------------------------------------------------------------------------------------//
void PR_Record::WriteScriptToIniFileV0_6(const STDSTR &strFileName,uint32 ReadPart){
	/*
	 //V0.6
		V0.6:[V0.6]
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
					StrCommand =
					blEnableSend =
					cmtTail =
					blEnableHEX =
					StrShowName =
					blSameAsCommand =
					StrTimeout =
					StrCycle =
					StrContinue =
					StrResend =
					StrStop =
					StrCatch =
				[node_end]
				[node]
				[node_end]
			[group_end]
			[group]
			[group_end]
		[grouplist_end]
	 */
	STDSTR		strResult;
	
	strResult = "[V0.6]\n";
	CFS_WriteFile(strFileName,strResult);
	strResult = "";
	if (ReadPart & READINITYPE_AddDeviceList)
		CFS_AddToFile(strFileName,cgIPComList->CreateStrV0_6(&strResult));
	strResult = "";
	if (ReadPart & READINITYPE_AddSingleCommand)
		CFS_AddToFile(strFileName,cgSCList->CreateSCListStrV0_6(&strResult));
	strResult = "";
	if (ReadPart & READINITYPE_AddGroupCommand)
		CFS_AddToFile(strFileName,cgGCList->CreateGroupListStrV0_6(&strResult));
}
//------------------------------------------------------------------------------------------//
void PR_Record::WriteScriptToIniFileV0_5(const STDSTR &strFileName,uint32 ReadPart){
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
					StrCommand =
					blEnableSend =
					blEnableSendCR =
					blEnableHEX =
					StrShowName =
					blSameAsCommand =
					StrTimeout =
					StrCycle =
					StrContinue =
					StrResend =
					StrStop =
					StrCatch =
				[node_end]
				[node]
				[node_end]
			[group_end]
			[group]
			[group_end]
		[grouplist_end]
	 */
	STDSTR		strResult;
	
	strResult = "[V0.5]\n";
	CFS_WriteFile(strFileName,strResult);
	strResult = "";
	if (ReadPart & READINITYPE_AddDeviceList)
		CFS_AddToFile(strFileName,cgIPComList->CreateStrV0_5(&strResult));
	strResult = "";
	if (ReadPart & READINITYPE_AddSingleCommand)
		CFS_AddToFile(strFileName,cgSCList->CreateSCListStrV0_5(&strResult));
	strResult = "";
	if (ReadPart & READINITYPE_AddGroupCommand)
		CFS_AddToFile(strFileName,cgGCList->CreateGroupListStrV0_5(&strResult));
}
//------------------------------------------------------------------------------------------//
void PR_Record::WriteScriptToIniFileV0_4(const STDSTR &strFileName,uint32 ReadPart){
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
	STDSTR		strResult,strTemp;
	
	strResult = "[V0.4]\n";
	CFS_WriteFile(strFileName,strResult);
	strResult = "";
	if (ReadPart & READINITYPE_AddDeviceList)
		CFS_AddToFile(strFileName,cgIPComList->CreateStrV0_4(&strResult));
	strResult = "";
	if (ReadPart & READINITYPE_AddSingleCommand)
		CFS_AddToFile(strFileName,cgSCList->CreateSCListStrV0_4(&strResult));
	strResult = "";
	if (ReadPart & READINITYPE_AddGroupCommand)
		CFS_AddToFile(strFileName,cgGCList->CreateGroupListStrV0_4(&strResult));
}
//------------------------------------------------------------------------------------------//
void PR_Record::WriteScriptToIniFileV0_3(const STDSTR &strFileName,uint32 ReadPart){
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
	STDSTR		strResult;
	
	strResult = "[V0.3]{}";
	if (ReadPart & READINITYPE_AddDeviceList){
		 cgIPComList->CreateStrV0_3(&strResult);
	}
	else{
		strResult += "{}";
	}
	if (ReadPart & READINITYPE_AddSingleCommand){
		cgSCList->CreateSCListStrV0_3(&strResult);
	}
	else{
		strResult += "{}";
	}
	if (ReadPart & READINITYPE_AddGroupCommand){
		cgGCList->CreateGroupListStrV0_3(&strResult);
	}
	
	CFS_WriteFile(strFileName,strResult);
}
//------------------------------------------------------------------------------------------//
void PR_Record::WriteScriptToIniFileV0_2(const STDSTR &strFileName){
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
	STDSTR		strResult;
	
	strResult = "[V0.2]";
	cgSCList->CreateSCListStrV0_2(&strResult);
	cgGCList->CreateGroupListStrV0_2(&strResult);
	
	strResult = Str_Replace(strResult," ","");
	
	CFS_WriteFile(strFileName,strResult);
}
//------------------------------------------------------------------------------------------//
void PR_Record::ReadScriptFromIniFile(const STDSTR &strFileName,uint32 ReadPart){
	STDSTR		strResult,strTemp;
	STDSTR::size_type	length;
	
	CFS_ReadFile(&strResult,strFileName);
	
	length = strResult.length();
	if (length > 6){
		strTemp = strResult.substr(0,6);
		if (strTemp == "[V0.6]"){						//V0.6
			strResult = strResult.substr(6,length - 6);
			ReadScriptFromIniFileV0_6(&strResult,ReadPart);
			return;
		}
		else if (strTemp == "[V0.5]"){						//V0.5
			strResult = strResult.substr(6,length - 6);
			ReadScriptFromIniFileV0_5(&strResult,ReadPart);
			return;
		}
		else if (strTemp == "[V0.4]"){						//V0.4
			strResult = strResult.substr(6,length - 6);
			ReadScriptFromIniFileV0_4(&strResult,ReadPart);
			return;
		}
		else if (strTemp == "[V0.3]"){						//V0.3
			strResult = strResult.substr(6,length - 6);
			ReadScriptFromIniFileV0_3(strResult,ReadPart);
			return;
		}
		else if (strTemp == "[V0.2]"){						//V0.2
			strResult = strResult.substr(6,length - 6);
			ReadScriptFromIniFileV0_2(strResult,ReadPart);
			return;
		}
	}
}
//------------------------------------------------------------------------------------------//
void PR_Record::ReadScriptFromIniFileV0_6(STDSTR *strIn,uint32 ReadPart){
	STDSTR		strLine;
	
	if (ReadPart & READINITYPE_CoverDeviceList)
		cgIPComList->Empty();
	if (ReadPart & READINITYPE_CoverSingleCommand)
		cgSCList->Empty();
	if (ReadPart & READINITYPE_CoverGroupCommand)
		cgGCList->Empty();
	
	while(strIn->length() > 0){
		strLine = Str_Trim(Str_ReadSubItem(strIn,"\n"));
		if (strLine == "[ipcomlist]"){
			if (ReadPart & READINITYPE_AddDeviceList)
				cgIPComList->SetIPComListV0_6(strIn);
		}
		else if (strLine == "[singleCommand]"){
			if (ReadPart & READINITYPE_AddSingleCommand)
				cgSCList->SetSCListV0_6(strIn);
		}
		else if (strLine == "[grouplist]"){
			if (ReadPart & READINITYPE_AddGroupCommand)
				cgGCList->SetGroupListV0_6(strIn);
		}
	}
}
//------------------------------------------------------------------------------------------//
void PR_Record::ReadScriptFromIniFileV0_5(STDSTR *strIn,uint32 ReadPart){
	STDSTR		strLine;
	
	if (ReadPart & READINITYPE_CoverDeviceList)
		cgIPComList->Empty();
	if (ReadPart & READINITYPE_CoverSingleCommand)
		cgSCList->Empty();
	if (ReadPart & READINITYPE_CoverGroupCommand)
		cgGCList->Empty();
	
	while(strIn->length() > 0){
		strLine = Str_Trim(Str_ReadSubItem(strIn,"\n"));
		if (strLine == "[ipcomlist]"){
			if (ReadPart & READINITYPE_AddDeviceList)
				cgIPComList->SetIPComListV0_5(strIn);
		}
		else if (strLine == "[singleCommand]"){
			if (ReadPart & READINITYPE_AddSingleCommand)
				cgSCList->SetSCListV0_5(strIn);
		}
		else if (strLine == "[grouplist]"){
			if (ReadPart & READINITYPE_AddGroupCommand)
				cgGCList->SetGroupListV0_5(strIn);
		}
	}
}
//------------------------------------------------------------------------------------------//
void PR_Record::ReadScriptFromIniFileV0_4(STDSTR *strIn,uint32 ReadPart){
	STDSTR		strLine;
	
	if (ReadPart & READINITYPE_CoverDeviceList)
		cgIPComList->Empty();
	if (ReadPart & READINITYPE_CoverSingleCommand)
		cgSCList->Empty();
	if (ReadPart & READINITYPE_CoverGroupCommand)
		cgGCList->Empty();
	
	while(strIn->length() > 0){
		strLine = Str_Trim(Str_ReadSubItem(strIn,"\n"));
		if (strLine == "[ipcomlist]"){
			if (ReadPart & READINITYPE_AddDeviceList)
				cgIPComList->SetIPComListV0_4(strIn);
		}
		else if (strLine == "[singleCommand]"){
			if (ReadPart & READINITYPE_AddSingleCommand)
				cgSCList->SetSCListV0_4(strIn);
		}
		else if (strLine == "[grouplist]"){
			if (ReadPart & READINITYPE_AddGroupCommand)
				cgGCList->SetGroupListV0_4(strIn);
		}
	}
}
//------------------------------------------------------------------------------------------//
void PR_Record::ReadScriptFromIniFileV0_3(STDSTR strIn,uint32 ReadPart){
	STDSTR		strResult;
	
	if (ReadPart & READINITYPE_CoverDeviceList)
		cgIPComList->Empty();
	if (ReadPart & READINITYPE_CoverSingleCommand)
		cgSCList->Empty();
	if (ReadPart & READINITYPE_CoverGroupCommand)
		cgGCList->Empty();
	
	strIn = Str_Replace(strIn,"}{",":");
	strIn = Str_Replace(strIn,")(","|");
	strIn = Str_Replace(strIn,"{","");
	strIn = Str_Replace(strIn,"}","");
	strIn = Str_Replace(strIn,")","");
	strIn = Str_Replace(strIn,"(","");
	
	if (strIn.length() > 0){
		strResult = Str_ReadSubItem(&strIn,":");//{}
		strResult = Str_ReadSubItem(&strIn,":");//{DeviceList}
		if (ReadPart & READINITYPE_AddDeviceList)
			cgIPComList->SetIPComListV0_3(&strResult);
		strResult = Str_ReadSubItem(&strIn,":");//{SingleCommand}
		if (ReadPart & READINITYPE_AddSingleCommand)
			cgSCList->SetSCListV0_3(&strResult);
		if (ReadPart & READINITYPE_AddGroupCommand)
			cgGCList->SetGroupListV0_3(&strIn);//{GroupCommand}
	}
}
//------------------------------------------------------------------------------------------//
void PR_Record::ReadScriptFromIniFileV0_2(STDSTR strIn,uint32 ReadPart){
	STDSTR		strResult;

	if (ReadPart & READINITYPE_CoverSingleCommand)
		cgSCList->Empty();
	if (ReadPart & READINITYPE_CoverGroupCommand)
		cgGCList->Empty();
	
	strIn = Str_Replace(strIn,"}{",":");
	strIn = Str_Replace(strIn,")(","|");
	strIn = Str_Replace(strIn,"{","");
	strIn = Str_Replace(strIn,"}","");
	strIn = Str_Replace(strIn,")","");
	strIn = Str_Replace(strIn,"(","");
	
	if (strIn.length() > 0){
		strResult = Str_ReadSubItem(&strIn,":");//{SingleCommand}
		if (ReadPart & READINITYPE_AddSingleCommand)
			cgSCList->SetSCListV0_2(&strResult);
		if (ReadPart & READINITYPE_AddGroupCommand)
			cgGCList->SetGroupListV0_2(&strIn);//{GroupCommand}
	}
}
//------------------------------------------------------------------------------------------//
#endif
