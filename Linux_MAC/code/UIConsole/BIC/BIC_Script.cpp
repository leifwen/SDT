/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: BIC_Script.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
 
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "BIC_Socket.h"
#include "Comm_Convert.h"
#include "Comm_File.h"
#include "AppLayer.h"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_RUN::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Execute group command script.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [gID [ct]]"		,"Execute group by gID.");
	PrintHelpItem(tBICPAR,"     "				,"ct: Cycle times.");
	return(cgReturnCode);
}//------------------------------------------------------------------------------------------//
int32 BIC_RUN::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	int32			retCode;
	uint32			num;
	std::string		strPar1,strPar2;
	COMMAND_GROUP	*group;
	
	*ret = "";
	
	if (par.length() == 0){
		tBICPAR->oDevNode->Enable();
		retCode = tBICPAR->sdtApp->m_Script.Execute(&tBICPAR->sdtApp->m_Device,&tBICPAR->sdtApp->m_GCList);
		if (retCode == 0){
			PrintDoRet(tBICPAR,"Script is running!");
			return(cgReturnCode);
		}
	}
	else{
		strPar2 = Str_Trim(par);
		strPar1 = Str_ReadSubItem(&strPar2," ");
		Str_LTrimSelf(strPar2);
		
		num = (uint32)strtol(strPar1.c_str(),nullptr,10);

		tBICPAR->sdtApp->m_GCList.Spin_InUse_set();
		group = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&tBICPAR->sdtApp->m_GCList, num);
		if (group != nullptr){
			if (strPar2.length() > 0)
				group->autoRunTimes = atoi(strPar2.c_str());
			tBICPAR->oDevNode->Enable();
			group->Spin_InUse_set();
			retCode = tBICPAR->sdtApp->m_Script.Execute(&tBICPAR->sdtApp->m_Device,group);
			group->Spin_InUse_clr();
			if (retCode == 0)
				PrintDoRet(tBICPAR,"Script is running!");
		}
		else{
			group = nullptr;
			PrintDoRet(tBICPAR,"ERROR");
		}
		tBICPAR->sdtApp->m_GCList.Spin_InUse_clr();
		if (group == nullptr)
			return(cgReturnCode);
	}
	
	while(tBICPAR->blExit == 0){
		uint8	chkey;
		SYS_SleepMS(10);
		tBICPAR->blInPressKeyMode = 1;
		chkey = BI_ReadChar(tBICPAR,0);
		if (chkey == 27){
			break;
		}
		else if (chkey == '\r'){
			tBICPAR->oDevNode->Enable();
		}
		if (tBICPAR->sdtApp->m_Script.IsStop() != 0)
			break;
	}
	tBICPAR->blInPressKeyMode = 0;
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_STOP::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Stop script execution.");
	return(cgReturnCode);
}//------------------------------------------------------------------------------------------//
int32 BIC_STOP::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	if (par.length() == 0){
		tBICPAR->oDevNode->Enable();
		tBICPAR->sdtApp->m_Script.StopRun();
		PrintDoRet(tBICPAR,"Script is stopped!");
		PressAnyKey(tBICPAR);
		return(cgReturnCode);
	}
	return(Help(tBICPAR));
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_SEND::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Send data if connected.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     <data>","Data.");
	return(cgReturnCode);
}//------------------------------------------------------------------------------------------//
int32 BIC_SEND::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	if (par.length() == 0)
		return(cgReturnCode);
	if (tBICPAR->sdtApp->m_Device.CheckblConnect() != 0){
		tBICPAR->oDevNode->Enable();
		tBICPAR->sdtApp->m_Device.SendCommand(par,0,0,G_ESCAPE_ON);
		PressAnyKey(tBICPAR);
	}
	else{
		PrintDoRet(tBICPAR,"Fail execute due to no connected");
	}
	
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_SENDA::Help(BICPAR *tBICPAR,int32 blDetail)const{
	PrintHelpItem(tBICPAR,cgCommand,"Send data automatically if connected.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     <s>","Time interval.");
	PrintHelpItem(tBICPAR,"     <data>","Data.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_SENDA::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	std::string		strPar1,strPar2;
	COMMAND_NODE	commandNode;
	int32			blScriptAT;
	
	if (par.length() == 0)
		return(Help(tBICPAR));
	if (tBICPAR->sdtApp->m_Device.CheckblConnect() != 0){
		
		COMMAND_NODE::Init(&commandNode);
		
		strPar2 = Str_Trim(par);
		strPar1 = Str_ReadSubItem(&strPar2," ");
		Str_LTrimSelf(strPar2);
		
		commandNode.StrCommand = strPar2;
		commandNode.StrTimeout = strPar1;
		commandNode.StrCycle = '0';
		commandNode.blEnableHEX = 0;
		commandNode.blEnableSendCR = 0;
		
		blScriptAT = tBICPAR->sdtApp->m_Device.cEDevFlag.blScriptAT;
		tBICPAR->sdtApp->m_Device.cEDevFlag.blScriptAT = 0;
		
		tBICPAR->oDevNode->Enable();
		if (tBICPAR->sdtApp->m_Script.Execute(&tBICPAR->sdtApp->m_Device,&commandNode) == 0){
			PrintDoRet(tBICPAR,"Script is running!");
			return(cgReturnCode);
		}
		while(tBICPAR->blExit == 0){
			uint8	chkey;
			SYS_SleepMS(10);
			tBICPAR->blInPressKeyMode = 1;
			chkey = BI_ReadChar(tBICPAR,0);
			if (chkey == 27){
				break;
			}
			else if (chkey == '\r'){
				tBICPAR->oDevNode->Enable();
			}
			if (tBICPAR->sdtApp->m_Script.IsStop() != 0)
				break;
		}
		tBICPAR->blInPressKeyMode = 0;

		tBICPAR->sdtApp->m_Device.cEDevFlag.blScriptAT = blScriptAT;
	}
	else{
		PrintDoRet(tBICPAR,"Fail execute due to no connected");
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_SCRIPT::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Set script parameter.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR, "     [-l"				, "List Script Build-in Command.");
	PrintHelpItem(tBICPAR, "      |[-AT <on|off>]"	, "Check standard AT response.");
	PrintHelpItem(tBICPAR, "       [-E <on|off>]"	, "Display script BIC excution.");
	PrintHelpItem(tBICPAR, "       [-CE <on|off>]]"	, "Display script BIC explain report.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_SCRIPT::Command(BICPAR *tBICPAR, const std::string &par, std::string *ret)const{
	int32 blcheck;
	std::string		strCommand, strPar1, strPar2;
	*ret = "";
	strCommand = Str_Trim(par);
	
	blcheck = 0;
	if (par == "-l"){
		tBICPAR->sdtApp->m_Script.Help(tBICPAR->oDevNode);
		return(cgReturnCode);
	}
	do{
		strPar1 = Str_ReadSubItem(&strCommand, " ");
		Str_LTrimSelf(strCommand);
		strPar2 = Str_ReadSubItem(&strCommand, " ");
		if (strPar1 == "-AT"){
			if (strPar2 == "on"){
				tBICPAR->sdtApp->m_Device.cEDevFlag.blScriptAT = 1;
				blcheck = 1;
			}
			else if (strPar2 == "off"){
				tBICPAR->sdtApp->m_Device.cEDevFlag.blScriptAT = 0;
				blcheck = 1;
			}
		}
		else if (strPar1 == "-E"){
			if (strPar2 == "on"){
				tBICPAR->sdtApp->m_Device.cEDevFlag.blEnablePrintSBICinfo = 1;
				blcheck = 1;
			}
			else if (strPar2 == "off"){
				tBICPAR->sdtApp->m_Device.cEDevFlag.blEnablePrintSBICinfo = 0;
				blcheck = 1;
			}
		}
		else if (strPar1 == "-CE"){
			if (strPar2 == "on"){
				tBICPAR->sdtApp->m_Device.cEDevFlag.blCommandExplain = 1;
				blcheck = 1;
			}
			else	if (strPar2 == "off"){
				tBICPAR->sdtApp->m_Device.cEDevFlag.blCommandExplain = 0;
				blcheck = 1;
			}
		}
	} while (strCommand.length() > 0);
	
	if ((par.length() > 0) && (blcheck == 0))
		return(Help(tBICPAR));
	
	if (tBICPAR->sdtApp->m_Device.cEDevFlag.blScriptAT == 0){
		PrintStrN(tBICPAR, " disable check AT,", RICH_LIN_clDefault);
	}
	else{
		PrintStrN(tBICPAR, " enable check AT,", RICH_LIN_clDefault);
	}
	if (tBICPAR->sdtApp->m_Device.cEDevFlag.blEnablePrintSBICinfo == 0){
		PrintStr(tBICPAR, " disable show script BIC excution,", RICH_LIN_clDefault);
	}
	else{
		PrintStr(tBICPAR, " enable show script BIC excution,", RICH_LIN_clDefault);
	}
	if (tBICPAR->sdtApp->m_Device.cEDevFlag.blCommandExplain == 0){
		PrintStr(tBICPAR, " disable show script BIC explain.\n", RICH_LIN_clDefault);
	}
	else{
		PrintStr(tBICPAR, " enable show script BIC explain.\n", RICH_LIN_clDefault);
	}
	
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//














//------------------------------------------------------------------------------------------//
int32 BIC_SCM::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "->SCM.");
	if (blDetail == 0)
		return(cgReturnCode);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_SCM::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	int32			retCode;
	
	retCode = BIC_Node_S::Command(tBICPAR,par,ret);
	if (retCode == BI_RETCODE_NO)
		return(cgSub_ls.Command(tBICPAR, par, ret));
	return(retCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_SCM_LOAD::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Load single command from default <default.ini> file.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [filename]"			,"Filename.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_SCM_LOAD::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	uint32	readType;
	std::string		fileN;
	
	*ret = "";
	readType = READINITYPE_NONE | (READINITYPE_AddSingleCommand | READINITYPE_CoverSingleCommand);
	fileN = "Default.ini";
	if (par.length() > 0)
		fileN = par;
	
	if (CFS_CheckFile(fileN) > 0){
		tBICPAR->sdtApp->m_ParRecord.ReadScriptFromIniFile(fileN,readType);
		tBICPAR->sdtApp->m_SCList.SetblUpdate();
		PrintDoRet(tBICPAR,"Read from <" + fileN + "> successful");
	}
	else{
		PrintDoRet(tBICPAR,"Read fail");
	}
	
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_SCM_SAVE::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Save to file.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     <filename>"			,"Filename.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_SCM_SAVE::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	uint32	readType;
	
	*ret = "";
	readType = READINITYPE_NONE | READINITYPE_AddGroupCommand;
	readType |= (READINITYPE_AddSingleCommand);
	readType |= (READINITYPE_AddDeviceList);
	
	if (par.length() > 0){
		tBICPAR->sdtApp->m_ParRecord.WriteScriptToIniFile(par,readType);
		PrintDoRet(tBICPAR,"Save to <" + par + "> successful");
	}
	else{
		PrintDoRet(tBICPAR,"Save fail");
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_SCM_LS::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "List single command.");
	if (blDetail == 0)
		return(cgReturnCode);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_SCM_LS::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	int32			j;
	std::string		strPrintData;
	SC_NODE			*fromNode,*breakNode;
	uint32			fromNodeDRID;
	
	*ret = "";
	PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
	PrintStrN(tBICPAR,"  sID    ",RICH_LIN_clDefault);
	PrintStr(tBICPAR,"  command\n",RICH_LIN_clDefault);
	
	j = 0;

	fromNode =nullptr;
	breakNode = nullptr;
	fromNodeDRID = 0;
	do{
		tBICPAR->sdtApp->m_SCList.Spin_InUse_set();
		if (breakNode == nullptr){
			fromNode = (SC_NODE*)GetFirstChild(&tBICPAR->sdtApp->m_SCList);
		}
		else{
			breakNode = nullptr;
			fromNode = (SC_NODE*)FindInLChildRChainByDRNodeID(&tBICPAR->sdtApp->m_SCList, fromNodeDRID);
		}
		RTREE_RChain_Traversal_LINE(SC_NODE,fromNode,
			operateNode_t->Spin_InUse_set();
			strPrintData = Str_IntToString(GetdRNodeID(operateNode_t)) + ".";
			if (strPrintData.length() < 5)
				strPrintData.insert(0,5 - strPrintData.length(),' ');
			strPrintData += (operateNode_t->blEnableSendCR == 0)?"      ":"  CR  ";
			strPrintData += operateNode_t->StrCommand;
			operateNode_t->Spin_InUse_clr();
			strPrintData += "\n";
			PrintStr(tBICPAR,strPrintData,RICH_LIN_clDefault);
			if ((tBICPAR->blExit == 0) && (++j > 19) && (nextNode_t != nullptr)){
				breakNode = nextNode_t;
				fromNodeDRID = GetdRNodeID(breakNode);
				break;
			}
		);
		tBICPAR->sdtApp->m_SCList.Spin_InUse_clr();
		if ((tBICPAR->blExit == 0) && (j > 19) && (breakNode != nullptr)){
			uint8 retChar;
			j = 0;
			retChar = PrintPressAnyKey(tBICPAR);
			if (retChar == 27){
				break;
			}
			else if (retChar == ' '){
				SetblExit(tBICPAR);
			}
		}
	}while(breakNode != nullptr);

	PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
	PrintDoRet(tBICPAR,"OK");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_SCM_SET::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Set command.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [+cr]"			,"Enable send \"\\r\" in the end.");
	PrintHelpItem(tBICPAR,"     [-cr]"			,"Disable send \"\\r\" in the end.");
	PrintHelpItem(tBICPAR,"     <sID>"			,"Command sID, all.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_SCM_SET::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	std::string		strPar1,strPar2;
	uint32			sID;
	uint32			blCR;
	SC_NODE			*selectNode;

	*ret = "";
	blCR = 0;

	if (par.length() > 0){
		strPar2 = par;
		tBICPAR->sdtApp->m_SCList.Spin_InUse_set();
		while(strPar2.length() > 0){
			strPar1 = Str_Trim(Str_ReadSubItem(&strPar2, " "));
			if (strPar1.length() > 0){
				if (strPar1 == "+cr"){
					blCR = 1;
				}
				else if (strPar1 == "-cr"){
					blCR = 0;
				}
				else if (strPar1 == "all"){
					RTREE_LChildRChain_Traversal_LINE(SC_NODE,(&tBICPAR->sdtApp->m_SCList),
						operateNode_t->Spin_InUse_set();
						operateNode_t->blEnableSendCR = blCR;
						operateNode_t->Spin_InUse_clr();
					);
					tBICPAR->sdtApp->m_SCList.SetblUpdate();
				}
				else{
					sID = (uint32)strtol(strPar1.c_str(),nullptr,10);
					selectNode = (SC_NODE*)FindInLChildRChainByDRNodeID(&tBICPAR->sdtApp->m_SCList, sID);
					if (selectNode != nullptr){
						selectNode->Spin_InUse_set();
						selectNode->blEnableSendCR = blCR;
						selectNode->Spin_InUse_clr();
						tBICPAR->sdtApp->m_SCList.SetblUpdate();
					}
				}
			}
		}
		tBICPAR->sdtApp->m_SCList.Spin_InUse_clr();
	}
	PrintDoRet(tBICPAR,"OK");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_SCM_CLONE::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Clone command by sID.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [sID]"			,"Command sID.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_SCM_CLONE::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	SC_NODE			*nextNode,*newNode;
	std::string		strDoRet;
	uint32			sDID;
	
	*ret = "";
	strDoRet = "Fail";
	tBICPAR->sdtApp->m_SCList.Spin_InUse_set();
	if (par.length() > 0){
		sDID = (uint32)strtol(par.c_str(),nullptr,10);
		nextNode = (SC_NODE*)FindInLChildRChainByDRNodeID(&tBICPAR->sdtApp->m_SCList, sDID);
		if (nextNode != nullptr){
			newNode = new SC_NODE;
			if (newNode != nullptr){
				SC_NODE::Copy(nextNode, newNode);
				tBICPAR->sdtApp->m_SCList.AddNode(newNode);
				tBICPAR->sdtApp->m_SCList.SetblUpdate();
				strDoRet = "Success";
			}
		}
	}
	else{
		newNode = new SC_NODE;
		if (newNode != nullptr){
			newNode->StrCommand = "new command";
			tBICPAR->sdtApp->m_SCList.AddNode(newNode);
			tBICPAR->sdtApp->m_SCList.SetblUpdate();
			strDoRet = "Success";
		}
	}
	tBICPAR->sdtApp->m_SCList.Spin_InUse_clr();
	PrintDoRet(tBICPAR,strDoRet);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_SCM_DEL::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Delete command by sID.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     <sID>"			,"Command sID.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_SCM_DEL::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	std::string		strPar1,strPar2;
	uint32			sDID;
	
	*ret = "";
	if (par.length() > 0){
		strPar2 = par;
		tBICPAR->sdtApp->m_SCList.Spin_InUse_set();
		while(strPar2.length() > 0){
			strPar1 = Str_Trim(Str_ReadSubItem(&strPar2, " "));
			if (strPar1.length() > 0){
				sDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
				MoveNodeToTrash(FindInLChildRChainByDRNodeID(&tBICPAR->sdtApp->m_SCList, sDID),&tBICPAR->sdtApp->m_SCList);
				tBICPAR->sdtApp->m_SCList.SetblUpdate();
			}
		}
		CleanTrash(&tBICPAR->sdtApp->m_SCList);
		tBICPAR->sdtApp->m_SCList.Spin_InUse_clr();
	}
	PrintDoRet(tBICPAR,"Success");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_SCM_MVUP::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Move command up.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     <sID>"			,"Command sID.");
	PrintHelpItem(tBICPAR,"     [step]"			,"Default value is 1.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_SCM_MVUP::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	SC_NODE			*nextNode;
	std::string		strPar1,strPar2,strDoRet;
	uint32			sDID,step;
	
	*ret = "";
	strDoRet = "Fail";
	if (par.length() > 0){
		strPar2 = par;
		strPar1 = Str_Trim(Str_ReadSubItem(&strPar2, " "));
		sDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
		step = (uint32)strtol(strPar2.c_str(),nullptr,10);
		if (step == 0)
			step = 1;
		tBICPAR->sdtApp->m_SCList.Spin_InUse_set();
		nextNode = (SC_NODE*)FindInLChildRChainByDRNodeID(&tBICPAR->sdtApp->m_SCList, sDID);
		if (nextNode != nullptr){
			while(step-- > 0)
				MoveNodesUpInRChain(nextNode);
			strDoRet = "Success";
			tBICPAR->sdtApp->m_SCList.SetblUpdate();
		}
		tBICPAR->sdtApp->m_SCList.Spin_InUse_clr();
	}
	PrintDoRet(tBICPAR,strDoRet);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_SCM_MVDN::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Move command down.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     <sID>"			,"Command sID.");
	PrintHelpItem(tBICPAR,"     [step]"			,"Default value is 1.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_SCM_MVDN::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	SC_NODE			*nextNode;
	std::string		strPar1,strPar2,strDoRet;
	uint32			sDID,step;
	
	*ret = "";
	strDoRet = "Fail";
	if (par.length() > 0){
		strPar2 = par;
		strPar1 = Str_Trim(Str_ReadSubItem(&strPar2, " "));
		sDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
		step = (uint32)strtol(strPar2.c_str(),nullptr,10);
		if (step == 0)
			step = 1;
		tBICPAR->sdtApp->m_SCList.Spin_InUse_set();
		nextNode = (SC_NODE*)FindInLChildRChainByDRNodeID(&tBICPAR->sdtApp->m_SCList, sDID);
		if (nextNode != nullptr){
			while(step-- > 0)
				MoveNodesDownInRChain(nextNode);
			strDoRet = "Success";
			tBICPAR->sdtApp->m_SCList.SetblUpdate();
		}
		tBICPAR->sdtApp->m_SCList.Spin_InUse_clr();
	}
	PrintDoRet(tBICPAR,strDoRet);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_SCM_EG::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Single command record example.");
	if (blDetail == 0)
		return(cgReturnCode);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_SCM_EG::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	std::string		strData;
	
	*ret = "";
	PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
	strData = "[scNode]\r\n";
	strData += "  blEnableCR =\r\n";
	strData += "  strCommand =\r\n";
	strData += "[scNode_end]\r\n";
	PrintStrN(tBICPAR,strData,RICH_LIN_clBrown);
	PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
	PrintDoRet(tBICPAR,"OK");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_SCM_SEND::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Send command by sID if connected.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"    <sID>"		,"Command sID.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_SCM_SEND::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	SC_NODE			*nextNode;
	std::string		StrCommand;
	int32			blEnableSendCR;
	uint32			sDID;
	
	*ret = "";
	StrCommand = "";
	blEnableSendCR = 0;
	nextNode = nullptr;
	if (tBICPAR->sdtApp->m_Device.CheckblConnect() != 0){
		if (par.length() > 0){
			sDID = (uint32)strtol(par.c_str(),nullptr,10);
			tBICPAR->sdtApp->m_SCList.Spin_InUse_set();
			nextNode = (SC_NODE*)FindInLChildRChainByDRNodeID(&tBICPAR->sdtApp->m_SCList, sDID);
			if (nextNode != nullptr){
				nextNode->Spin_InUse_set();
				StrCommand = nextNode->StrCommand;
				blEnableSendCR = nextNode->blEnableSendCR;
				nextNode->Spin_InUse_clr();
			}
			tBICPAR->sdtApp->m_SCList.Spin_InUse_clr();
			if (nextNode != nullptr){
				tBICPAR->oDevNode->Enable();
				tBICPAR->sdtApp->m_Device.SendCommand(StrCommand,0,blEnableSendCR,G_ESCAPE_ON);
				PressAnyKey(tBICPAR);
				PrintDoRet(tBICPAR,"Success");
			}
			else{
				PrintDoRet(tBICPAR,"Fail");
			}
		}
	}
	else{
		PrintDoRet(tBICPAR,"Fail execute due to no connected");
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_SCM_COMMAND::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Set command content by sID.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"    <sID>"		,"Command sID.");
	PrintHelpItem(tBICPAR,"    <content>"	,"Content.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_SCM_COMMAND::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	SC_NODE			*nextNode;
	std::string		strPar1,strPar2;
	uint32			sDID;
	
	*ret = "";
	nextNode = nullptr;
	if (par.length() > 0){
		strPar2 = par;
		strPar1 = Str_ReadSubItem(&strPar2, " ");
		sDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
		tBICPAR->sdtApp->m_SCList.Spin_InUse_set();
		nextNode = (SC_NODE*)FindInLChildRChainByDRNodeID(&tBICPAR->sdtApp->m_SCList, sDID);
		if (nextNode != nullptr){
			nextNode->Spin_InUse_set();
			nextNode->StrCommand = strPar2;
			nextNode->Spin_InUse_clr();
			tBICPAR->sdtApp->m_SCList.SetblUpdate();
		}
		tBICPAR->sdtApp->m_SCList.Spin_InUse_clr();
	}
	if (nextNode != nullptr){
		PrintDoRet(tBICPAR,"Success");
	}
	else {
		PrintDoRet(tBICPAR,"Fail");
	}

	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//








//------------------------------------------------------------------------------------------//
int32 BIC_GCM::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "->GCM.");
	if (blDetail == 0)
		return(cgReturnCode);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	int32			retCode;
	
	retCode = BIC_Node_S::Command(tBICPAR,par,ret);
	if (retCode == BI_RETCODE_NO)
		return(cgSub_ls.Command(tBICPAR, par, ret));
	return(retCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_LOAD::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Load group from default <default.ini> file.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [filename]"			,"Filename.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_LOAD::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	uint32	readType;
	std::string		fileN;
	
	*ret = "";
	readType = READINITYPE_NONE | READINITYPE_AddGroupCommand | READINITYPE_CoverGroupCommand;
	
	fileN = "Default.ini";
	if (par.length() > 0)
		fileN = par;
	if (CFS_CheckFile(fileN) > 0){
		tBICPAR->sdtApp->m_ParRecord.ReadScriptFromIniFile(fileN,readType);
		tBICPAR->sdtApp->m_GCList.SetblUpdate();
		PrintDoRet(tBICPAR,"Read from <" + fileN + "> successful");
	}
	else{
		PrintDoRet(tBICPAR,"Read fail");
	}
	
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_LS::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "List group.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [-s]"			,"Show selected item.");
	PrintHelpItem(tBICPAR,"     [-c]"			,"Show Cycle column.");
	PrintHelpItem(tBICPAR,"     [-t]"			,"Show Timeout/interval column.");
	PrintHelpItem(tBICPAR,"     [-d]"			,"Show condition.");
	PrintHelpItem(tBICPAR,"     [-a]"			,"Show all.");
	PrintHelpItem(tBICPAR,"     [gID]"			,"Group gID, s/uns.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_LS::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	std::string		strPar1,strPar2,strGroupDID;
	uint32			flag_gl,groupNum;
	
	*ret = "";
	flag_gl = 0;
	strGroupDID = "";
	
	if (par.length() > 0){
		strPar2 = par;
		while(strPar2.length() > 0){
			strPar1 = Str_Trim(Str_ReadSubItem(&strPar2, " "));
			if (strPar1 == "-s"){
				flag_gl |= BIC_GCM_LS_FLAG_GLshowSelect;
				flag_gl |= BIC_GCM_LS_FLAG_CLshowSelect;
			}
			else if (strPar1 == "-c"){
				flag_gl |= BIC_GCM_LS_FLAG_GLshowCycle;
				flag_gl |= BIC_GCM_LS_FLAG_CLshowCycle;
			}
			else if (strPar1 == "-t"){
				flag_gl |= BIC_GCM_LS_FLAG_GLshowInterval;
				flag_gl |= BIC_GCM_LS_FLAG_CLshowTimeout;
			}
			else if (strPar1 == "-d"){
				flag_gl |= BIC_GCM_LS_FLAG_CLshowDetail;
			}
			else if (strPar1 == "-a"){
				flag_gl |= BIC_GCM_LS_FLAG_GLshowCycle;
				flag_gl |= BIC_GCM_LS_FLAG_CLshowCycle;
				flag_gl |= BIC_GCM_LS_FLAG_GLshowInterval;
				flag_gl |= BIC_GCM_LS_FLAG_CLshowTimeout;
				flag_gl |= BIC_GCM_LS_FLAG_CLshowDetail;
			}
			else{
				strGroupDID += strPar1;
				strGroupDID += ' ';
			}
		}
	}
	
	if (strGroupDID.length() == 0){
		BIC_GCM_LS_PrintGroupList(tBICPAR,&tBICPAR->sdtApp->m_GCList,flag_gl);
	}
	else{
		while(strGroupDID.length() > 0){
			strPar1 = Str_ReadSubItem(&strGroupDID," ");
			if (strPar1 == "s"){
				flag_gl |= BIC_GCM_LS_FLAG_GLshowSelect;
				flag_gl |= BIC_GCM_LS_FLAG_GLshowCL;
				BIC_GCM_LS_PrintGroupList(tBICPAR,&tBICPAR->sdtApp->m_GCList,flag_gl);
			}
			else if (strPar1 == "uns"){
				flag_gl &= (~BIC_GCM_LS_FLAG_GLshowSelect);
				flag_gl |= BIC_GCM_LS_FLAG_GLshowCL;
				BIC_GCM_LS_PrintGroupList(tBICPAR,&tBICPAR->sdtApp->m_GCList,flag_gl);
			}
			else {
				groupNum = (uint32)strtol(strPar1.c_str(),nullptr,10);
				tBICPAR->sdtApp->m_GCList.Spin_InUse_set();
				BIC_GCM_LS_PringCommandList(tBICPAR,(COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&tBICPAR->sdtApp->m_GCList, groupNum),flag_gl);
				tBICPAR->sdtApp->m_GCList.Spin_InUse_clr();
			}
		}
	}
	PrintDoRet(tBICPAR,"OK");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_LS::BIC_GCM_LS_PrintGroupList(BICPAR *tBICPAR,GC_LIST *tGroupList,uint64 flag){
	COMMAND_GROUP	*fromNode,*breakNode;
	uint32			fromNodeDRID;
	int32 j;
	std::string		strPrintData,strTemp;
	
	if (tGroupList == nullptr)
		return 0;
	
	PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
	PrintStrN(tBICPAR,"  gID",RICH_LIN_clDefault);
	if ((flag & BIC_GCM_LS_FLAG_GLshowCycle) != 0)
		PrintStr(tBICPAR,"  cycle",RICH_LIN_clDefault);
	if ((flag & BIC_GCM_LS_FLAG_GLshowInterval) != 0)
		PrintStr(tBICPAR,"  interval",RICH_LIN_clDefault);
	PrintStr(tBICPAR,"  group name\n",RICH_LIN_clDefault);
	
	j = 0;

	fromNodeDRID = 0;
	fromNode = nullptr;
	breakNode = nullptr;
	
	do{
		tGroupList->Spin_InUse_set();
		if (breakNode == nullptr){
			fromNode = (COMMAND_GROUP*)GetFirstChild(tGroupList);
		}
		else{
			breakNode = nullptr;
			fromNode = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(tGroupList, fromNodeDRID);
		}
		RTREE_RChain_Traversal_LINE(COMMAND_GROUP,fromNode,
			operateNode_t->Spin_InUse_set();
			do{
				if ((flag & BIC_GCM_LS_FLAG_GLshowCL) != 0){
					if (((flag & BIC_GCM_LS_FLAG_GLshowSelect) != 0) && (operateNode_t->blEnableAutoRun != 0))
						BIC_GCM_LS_PringCommandList(tBICPAR,operateNode_t,flag);
					if (((flag & BIC_GCM_LS_FLAG_GLshowSelect) == 0) && (operateNode_t->blEnableAutoRun == 0))
						BIC_GCM_LS_PringCommandList(tBICPAR,operateNode_t,flag);
					j = 20;
					break;
				}
				if (((flag & BIC_GCM_LS_FLAG_GLshowSelect) != 0) && (operateNode_t->blEnableAutoRun == 0))
					break;
				strPrintData = ' ';
				if (operateNode_t->blEnableAutoRun != 0)
					strPrintData = "*";
				strTemp = Str_IntToString(GetdRNodeID(operateNode_t)) + ".";
				if (strTemp.length() < 4)
					strTemp.insert(0,4 - strTemp.length(),' ');
				strTemp += "  ";
				strPrintData += strTemp;
				if ((flag & BIC_GCM_LS_FLAG_GLshowCycle) != 0){
					strTemp = Str_IntToString(operateNode_t->autoRunTimes);
					if (strTemp.length() < 5)
						strTemp.insert(0,5 - strTemp.length(),' ');
					strTemp += "  ";
					strPrintData += strTemp;
				}
				if ((flag & BIC_GCM_LS_FLAG_GLshowInterval) != 0){
					strTemp = Str_IntToString(operateNode_t->intervalTime) + "ms";
					if (strTemp.length() < 8)
						strTemp.insert(0,8 - strTemp.length(),' ');
					strTemp += "  ";
					strPrintData += strTemp;
				}
				strPrintData += operateNode_t->name;
				strPrintData += "\n";
				if (operateNode_t->blEnableAutoRun == 0){
					PrintStrN(tBICPAR,strPrintData,RICH_LIN_clDefault);
				}
				else{
					PrintStrN(tBICPAR,strPrintData,RICH_LIN_clLightBlue);
				}
				++j;
			}while(0);
			operateNode_t->Spin_InUse_clr();

			if ((tBICPAR->blExit == 0) && (j > 19) && (nextNode_t != nullptr)){
				breakNode = nextNode_t;
				fromNodeDRID = GetdRNodeID(nextNode_t);
				break;
			}
		);
		tGroupList->Spin_InUse_clr();
		if ((tBICPAR->blExit == 0) && (j > 19) && (breakNode != nullptr)){
			uint8 retChar;
			j = 0;
			retChar = PrintPressAnyKey(tBICPAR);
			if (retChar == 27){
				break;
			}
			else if (retChar == ' '){
				SetblExit(tBICPAR);
			}
		}
	}while(breakNode != nullptr);
	PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_LS::BIC_GCM_LS_PringCommandList(BICPAR *tBICPAR,COMMAND_GROUP *tGroup,uint64 flag){
	std::string		strPrintData,strTemp;

	if (tGroup != nullptr){
		PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
		
		PrintStr(tBICPAR," gID      : ",RICH_LIN_clBrown);
		PrintStr(tBICPAR,Str_IntToString(GetdRNodeID(tGroup)) + "\n",RICH_LIN_clLightBlue);
		PrintStr(tBICPAR," Name     : ",RICH_LIN_clBrown);
		PrintStr(tBICPAR,tGroup->name + "\n",RICH_LIN_clLightBlue);
		PrintStr(tBICPAR," Interval : ",RICH_LIN_clBrown);
		PrintStr(tBICPAR,Str_IntToString(tGroup->intervalTime) + "ms\n",RICH_LIN_clLightBlue);
		PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
		
		BIC_GCM_LS_PringCommandTitle(tBICPAR,flag);
		RTREE_RChain_Traversal_LINE(COMMAND_NODE, GetcgLChild(tGroup),
			operateNode_t->Spin_InUse_set();
			BIC_GCM_LS_PringCommand(tBICPAR,operateNode_t,flag);
			operateNode_t->Spin_InUse_clr();
		);
		
		PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_LS::BIC_GCM_LS_PringCommandTitle(BICPAR *tBICPAR,uint64 flag){
	PrintStrN(tBICPAR,"  cID  ",RICH_LIN_clDefault);
	if ((flag & BIC_GCM_LS_FLAG_CLshowCycle) != 0)
		PrintStr(tBICPAR,"  cycle",RICH_LIN_clDefault);
	if ((flag & BIC_GCM_LS_FLAG_CLshowTimeout) != 0)
		PrintStr(tBICPAR,"  timeout",RICH_LIN_clDefault);
	
	PrintStr(tBICPAR,"  command\n",RICH_LIN_clDefault);
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_LS::BIC_GCM_LS_PringCommand(BICPAR *tBICPAR,COMMAND_NODE *command,uint64 flag){
	std::string		strPrintData,strTemp;
	
	if (command != nullptr){
		if (((flag & BIC_GCM_LS_FLAG_CLshowSelect) != 0) && (command->blEnableSend == 0))
			return 1;
		
		strPrintData = ' ';
		if (command->blEnableSend != 0)
			strPrintData = '*';
		
		strTemp = Str_IntToString(GetdRNodeID(command)) + ".";
		if (strTemp.length() < 4)
			strTemp.insert(0,4 - strTemp.length(),' ');
		if (command->blEnableHEX != 0){
			strTemp += 'H';
		}
		else{
			strTemp += ' ';
		}
		if (command->blEnableSendCR != 0){
			strTemp += 'R';
		}
		else{
			strTemp += ' ';
		}
		strTemp += ' ';
		strPrintData += strTemp;
		
		if ((flag & BIC_GCM_LS_FLAG_CLshowCycle) != 0){
			strTemp = command->StrCycle;
			if (strTemp.length() < 5)
				strTemp.insert(0,5 - strTemp.length(),' ');
			strTemp += ' ';
			strPrintData += strTemp;
		}
		if ((flag & BIC_GCM_LS_FLAG_CLshowTimeout) != 0){
			strTemp = command->StrTimeout + "s";
			if (strTemp.length() < 7)
				strTemp.insert(0,7 - strTemp.length(),' ');
			strTemp += "  ";
			strPrintData += strTemp;
		}
		
		strPrintData += command->StrCommand;
		strPrintData += "\r\n";
		if (command->blEnableSend != 0){
			PrintStrN(tBICPAR,strPrintData,RICH_LIN_clLightBlue);
		}
		else{
			PrintStrN(tBICPAR,strPrintData,RICH_LIN_clDefault);
		}
		if ((flag & BIC_GCM_LS_FLAG_CLshowDetail) != 0){
			PrintStrN(tBICPAR,"     Continue : " + command->StrContinue,RICH_LIN_clLightCyan);
			PrintStrN(tBICPAR,"     Resend   : " + command->StrResend,RICH_LIN_clLightCyan);
			PrintStrN(tBICPAR,"     CStop    : " + command->StrStop,RICH_LIN_clLightCyan);
			PrintStrN(tBICPAR,"     Catch    : " + command->StrCatch,RICH_LIN_clLightCyan);
		}
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
struct BIC_GCM_SET_PAR{
	int32	c,t,s,blnum;
	uint64	flag;
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
int32 BIC_GCM_SET::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Set group.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [+]"			,"Set group selected.");
	PrintHelpItem(tBICPAR,"     [-]"			,"Set group disselected.");
	PrintHelpItem(tBICPAR,"     [-cN]"			,"Set cycle, N is value.");
	PrintHelpItem(tBICPAR,"     [-tN]"			,"Set interval, N is value.");
	PrintHelpItem(tBICPAR,"     <gID>"			,"Group gID, s/uns.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_SET::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	//set +/- num num
	//set c3 num num
	//set t200 num num
	//set + c3 t300 num num - num num c4 num
	std::string		strPar1,strPar2;
	uint32			groupDID;
	BIC_GCM_SET_PAR	tSetPar;
	COMMAND_GROUP	*group;
	
	group = nullptr;
	
	tSetPar.blnum = 1;
	ClearSetPar(&tSetPar);
	if (par.length() > 0){
		strPar2 = par;
		while(strPar2.length() > 0){
			strPar1 = Str_Trim(Str_ReadSubItem(&strPar2, " "));
			if (strPar1.length() > 0){
				if (strPar1 == "+"){
					ClearSetPar(&tSetPar);
					tSetPar.flag |= BIC_GCM_LS_FLAG_GLsetSelect;
					tSetPar.s = 1;
				}
				else if (strPar1 == "-"){
					ClearSetPar(&tSetPar);
					tSetPar.flag |= BIC_GCM_LS_FLAG_GLsetSelect;
					tSetPar.s = 0;
				}
				else if ((strPar1.length() > 1) && (strPar1.substr(0,2) == "-c")){
					ClearSetPar(&tSetPar);
					strPar1 = strPar1.substr(2);
					tSetPar.flag |= BIC_GCM_LS_FLAG_GLsetCycle;
					tSetPar.c = atoi(strPar1.c_str());
				}
				else if ((strPar1.length() > 1) && (strPar1.substr(0,2) == "-t")){
					ClearSetPar(&tSetPar);
					strPar1 = strPar1.substr(2);
					tSetPar.flag |= BIC_GCM_LS_FLAG_GLsetInterval;
					tSetPar.t = atoi(strPar1.c_str());
				}
				else{
					tSetPar.blnum = 1;
					if (strPar1 == "s"){
						tSetPar.flag |= BIC_GCM_LS_FLAG_GLSelect;
						tBICPAR->sdtApp->m_GCList.Spin_InUse_set();
						SetPar(tBICPAR,&tBICPAR->sdtApp->m_GCList,&tSetPar);
						tBICPAR->sdtApp->m_GCList.SetblUpdate();
						tBICPAR->sdtApp->m_GCList.Spin_InUse_clr();
					}
					else if (strPar1 == "uns"){
						tSetPar.flag &= (~BIC_GCM_LS_FLAG_GLSelect);
						tBICPAR->sdtApp->m_GCList.Spin_InUse_set();
						SetPar(tBICPAR,&tBICPAR->sdtApp->m_GCList,&tSetPar);
						tBICPAR->sdtApp->m_GCList.SetblUpdate();
						tBICPAR->sdtApp->m_GCList.Spin_InUse_clr();
					}
					else{
						groupDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
						tBICPAR->sdtApp->m_GCList.Spin_InUse_set();
						group = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&tBICPAR->sdtApp->m_GCList,groupDID);
						tSetPar.flag &= (~BIC_GCM_LS_FLAG_GLSelect);
						if (group != nullptr){
							group->Spin_InUse_set();
							SetPar(tBICPAR,group,&tSetPar);
							tBICPAR->sdtApp->m_GCList.SetblUpdate();
							group->Spin_InUse_clr();
						}
						tBICPAR->sdtApp->m_GCList.Spin_InUse_clr();
					}
				}
			}
		}
	}
	PrintDoRet(tBICPAR,"OK");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_SET::SetPar(BICPAR *tBICPAR, COMMAND_GROUP *tGroup, struct BIC_GCM_SET_PAR *tPar){
	if (tGroup != nullptr){
		if ((tPar->flag & BIC_GCM_LS_FLAG_GLsetSelect) != 0)
			tGroup->blEnableAutoRun = tPar->s;
		if ((tPar->flag & BIC_GCM_LS_FLAG_GLsetCycle) != 0)
			tGroup->autoRunTimes = tPar->c;
		if ((tPar->flag & BIC_GCM_LS_FLAG_GLsetInterval) != 0)
			tGroup->intervalTime = tPar->t;
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_SET::SetPar(BICPAR *tBICPAR, GC_LIST *tGroupList, struct BIC_GCM_SET_PAR *tPar){
	if (tGroupList == nullptr)
		return 0;
	RTREE_LChildRChain_Traversal_LINE(COMMAND_GROUP,tGroupList,
		operateNode_t->Spin_InUse_set();
		if (((tPar->flag & BIC_GCM_LS_FLAG_GLSelect) != 0) && (operateNode_t->blEnableAutoRun != 0))
			SetPar(tBICPAR,operateNode_t,tPar);
		if (((tPar->flag & BIC_GCM_LS_FLAG_GLSelect) == 0) && (operateNode_t->blEnableAutoRun == 0))
			SetPar(tBICPAR,operateNode_t,tPar);
		operateNode_t->Spin_InUse_clr();
	);
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_CLONE::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Clone group by gID.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [gID]"			,"Group gID.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_CLONE::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	COMMAND_GROUP	*nextGroup,*newGroup;
	std::string		strPar1,strPar2;
	uint32			groupDID;
	
	*ret = "";
	newGroup = nullptr;
	nextGroup = nullptr;
	tBICPAR->sdtApp->m_GCList.Spin_InUse_set();
	if (par.length() > 0){
		strPar2 = par;
		while(strPar2.length() > 0){
			strPar1 = Str_Trim(Str_ReadSubItem(&strPar2, " "));
			if (strPar1.length() > 0){
				groupDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
				nextGroup = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&tBICPAR->sdtApp->m_GCList,groupDID);
				if (nextGroup != nullptr){
					if (newGroup == nullptr){
						newGroup = new COMMAND_GROUP;
						if (newGroup != nullptr){
							newGroup->name = "new copy group";
							COMMAND_GROUP::CopyCOMMAND_GROUP(nextGroup,newGroup,0);
						}
					}
					else{
						COMMAND_GROUP::CopyCOMMAND_GROUP(nextGroup,newGroup,0);
					}
				}
			}
		}
	}
	else{
		newGroup = new COMMAND_GROUP;
		if (newGroup != nullptr){
			newGroup->name = "new group";
			COMMAND_GROUP::CopyCOMMAND_GROUP(nextGroup,newGroup,0);
		}
	}
	
	if (newGroup != nullptr){
		tBICPAR->sdtApp->m_GCList.AddNode(newGroup);
		BIC_GCM_LS::BIC_GCM_LS_PringCommandList(tBICPAR,newGroup, BIC_GCM_LS_FLAG_CLshowCycle | BIC_GCM_LS_FLAG_CLshowTimeout);
		tBICPAR->sdtApp->m_GCList.SetblUpdate();
		PrintDoRet(tBICPAR,"Success");
	}
	else{
		PrintDoRet(tBICPAR,"Fail");
	}
	tBICPAR->sdtApp->m_GCList.Spin_InUse_clr();
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_DEL::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Delete group by gID.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     <gID>"			,"Group gID.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_DEL::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	COMMAND_GROUP	*nextGroup;
	std::string		strPar1,strPar2;
	uint32			groupDID;
	
	*ret = "";
	nextGroup = nullptr;
	tBICPAR->sdtApp->m_GCList.Spin_InUse_set();
	if (par.length() > 0){
		strPar2 = par;
		while(strPar2.length() > 0){
			strPar1 = Str_Trim(Str_ReadSubItem(&strPar2, " "));
			if (strPar1.length() > 0){
				groupDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
				MoveNodeToTrash(FindInLChildRChainByDRNodeID(&tBICPAR->sdtApp->m_GCList, groupDID),&tBICPAR->sdtApp->m_GCList);
				tBICPAR->sdtApp->m_GCList.SetblUpdate();
			}
		}
	}
	CleanTrash(&tBICPAR->sdtApp->m_GCList);
	tBICPAR->sdtApp->m_GCList.Spin_InUse_clr();
	PrintDoRet(tBICPAR,"Success");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_MVUP::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Move group up.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     <gID>"			,"Group gID.");
	PrintHelpItem(tBICPAR,"     [step]"			,"Default value is 1.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_MVUP::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	COMMAND_GROUP	*nextGroup;
	std::string		strPar1,strPar2,strDoRet;
	uint32			groupDID,step;
	
	*ret = "";
	nextGroup = nullptr;
	strDoRet = "Fail";
	if (par.length() > 0){
		strPar2 = par;
		strPar1 = Str_Trim(Str_ReadSubItem(&strPar2, " "));
		groupDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
		step = (uint32)strtol(strPar2.c_str(),nullptr,10);
		if (step == 0)
			step = 1;
		tBICPAR->sdtApp->m_GCList.Spin_InUse_set();
		nextGroup = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&tBICPAR->sdtApp->m_GCList, groupDID);
		if (nextGroup != nullptr){
			while(step-- > 0)
				MoveNodesUpInRChain(nextGroup);
			strDoRet = "Success";
			tBICPAR->sdtApp->m_GCList.SetblUpdate();
		}
		tBICPAR->sdtApp->m_GCList.Spin_InUse_clr();
	}
	PrintDoRet(tBICPAR,strDoRet);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_MVDN::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Move group down.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     <gID>"			,"Group gID.");
	PrintHelpItem(tBICPAR,"     [step]"			,"Default value is 1.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_MVDN::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	COMMAND_GROUP	*nextGroup;
	std::string		strPar1,strPar2,strDoRet;
	uint32			groupDID,step;
	
	*ret = "";
	nextGroup = nullptr;
	strDoRet = "Fail";
	if (par.length() > 0){
		strPar2 = par;
		strPar1 = Str_Trim(Str_ReadSubItem(&strPar2, " "));
		groupDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
		step = (uint32)strtol(strPar2.c_str(),nullptr,10);
		if (step == 0)
			step = 1;
		tBICPAR->sdtApp->m_GCList.Spin_InUse_set();
		nextGroup = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&tBICPAR->sdtApp->m_GCList, groupDID);
		if (nextGroup != nullptr){
			while(step-- > 0)
				MoveNodesDownInRChain(nextGroup);
			strDoRet = "Success";
			tBICPAR->sdtApp->m_GCList.SetblUpdate();
		}
		tBICPAR->sdtApp->m_GCList.Spin_InUse_clr();
	}
	PrintDoRet(tBICPAR,strDoRet);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_EG::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Group record example.");
	if (blDetail == 0)
		return(cgReturnCode);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_EG::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	std::string		strData;
	
	*ret = "";
	PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
	strData  = "[grouplist]\r\n";
	strData += "  [group]\r\n";
	strData += "    groupName =\r\n";
	strData += "    intervalTime =\r\n";
	strData += "    [node]\r\n";
	strData += "      StrCommand =\r\n";
	strData += "      blEnableSend =\r\n";
	strData += "      blEnableSendCR =\r\n";
	strData += "      blEnableHEX =\r\n";
	strData += "      StrShowName =\r\n";
	strData += "      StrTimeout =\r\n";
	strData += "      StrCycle =\r\n";
	strData += "      StrContinue =\r\n";
	strData += "      StrResend =\r\n";
	strData += "      StrStop =\r\n";
	strData += "      StrCatch =\r\n";
	strData += "    [node_end]\r\n";
	strData += "    [node]\r\n";
	strData += "    [node_end]\r\n";
	strData += "  [group_end]\r\n";
	strData += "  [group]\r\n";
	strData += "  [group_end]\r\n";
	strData += "[grouplist_end]\r\n";
	PrintStrN(tBICPAR,strData,RICH_LIN_clBrown);
	PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
	PrintDoRet(tBICPAR,"OK");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GN::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Set group name by gID.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"    <gID>"		,"Group gID.");
	PrintHelpItem(tBICPAR,"    <name>"		,"Name.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GN::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	COMMAND_GROUP	*nextGroup;
	std::string		strPar1,strPar2;
	uint32			gDID;
	
	*ret = "";
	
	nextGroup = nullptr;
	if (par.length() > 0){
		strPar2 = par;
		strPar1 = Str_ReadSubItem(&strPar2, " ");
		gDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
		tBICPAR->sdtApp->m_GCList.Spin_InUse_set();
		nextGroup = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&tBICPAR->sdtApp->m_GCList, gDID);
		if (nextGroup != nullptr){
			nextGroup->Spin_InUse_set();
			nextGroup->name = strPar2;
			nextGroup->Spin_InUse_clr();
			tBICPAR->sdtApp->m_GCList.SetblUpdate();
		}
		tBICPAR->sdtApp->m_GCList.Spin_InUse_clr();
	}
	if (nextGroup != nullptr){
		PrintDoRet(tBICPAR,"Success");
	}
	else {
		PrintDoRet(tBICPAR,"Fail");
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//















//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "->Into group by gID.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"    <gID>"		,"Group gID.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	int32			retCode;
	COMMAND_GROUP	*nextGroup;
	uint32			gDID;
	std::string		strPar1,strPar2;
	
	*ret = "";
	nextGroup = nullptr;
	if (par.length() > 0){
		strPar2 = par;
		strPar1 = Str_ReadSubItem(&strPar2, " ");
		gDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
		nextGroup = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&tBICPAR->sdtApp->m_GCList, gDID);
		if (nextGroup != nullptr){
			tBICPAR->gDID = gDID;
			retCode = ExecuteLC(tBICPAR,strPar2,"-" + Str_IntToString(gDID),ret);
			tBICPAR->gDID = 0;
			if (retCode == BI_RETCODE_RETURN)
				return(cgReturnCode);
			return(retCode);
		}
	}
	PrintDoRet(tBICPAR,"Fail");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_LS::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "List command.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [-s]"			,"Show selected item.");
	PrintHelpItem(tBICPAR,"     [-c]"			,"Show Cycle column.");
	PrintHelpItem(tBICPAR,"     [-t]"			,"Show Timeout column.");
	PrintHelpItem(tBICPAR,"     [-d]"			,"Show condition.");
	PrintHelpItem(tBICPAR,"     [-a]"			,"Show all.");
	PrintHelpItem(tBICPAR,"     [cID]"			,"Command cID.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_LS::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	std::string		strPar1,strPar2,strcDID;
	uint32			flag_gl,cDID,ret_Code;
	COMMAND_GROUP	*group;
	COMMAND_NODE	*command;
	
	*ret = "";
	flag_gl = 0;
	strcDID = "";
	tBICPAR->sdtApp->m_GCList.Spin_InUse_set();
	group = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&tBICPAR->sdtApp->m_GCList, tBICPAR->gDID);
	if (group == nullptr){
		PrintDoRet(tBICPAR,"ERROR gID");
		ret_Code = BI_RETCODE_RETURN;
	}
	else{
		if (par.length() > 0){
			strPar2 = par;
			while(strPar2.length() > 0){
				strPar1 = Str_Trim(Str_ReadSubItem(&strPar2, " "));
				if (strPar1 == "-s"){
					flag_gl |= BIC_GCM_LS_FLAG_CLshowSelect;
				}
				else if (strPar1 == "-c"){
					flag_gl |= BIC_GCM_LS_FLAG_CLshowCycle;
				}
				else if (strPar1 == "-t"){
					flag_gl |= BIC_GCM_LS_FLAG_CLshowTimeout;
				}
				else if (strPar1 == "-d"){
					flag_gl |= BIC_GCM_LS_FLAG_CLshowDetail;
				}
				else if (strPar1 == "-a"){
					flag_gl |= BIC_GCM_LS_FLAG_CLshowCycle;
					flag_gl |= BIC_GCM_LS_FLAG_CLshowTimeout;
					flag_gl |= BIC_GCM_LS_FLAG_CLshowDetail;
				}
				else{
					strcDID += strPar1;
					strcDID += ' ';
				}
			}
		}
		if (strcDID.length() == 0){
			group->Spin_InUse_set();
			BIC_GCM_LS::BIC_GCM_LS_PringCommandList(tBICPAR,group,flag_gl);
			group->Spin_InUse_clr();
		}
		else{
			PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
			BIC_GCM_LS::BIC_GCM_LS_PringCommandTitle(tBICPAR,flag_gl);
			while(strcDID.length() > 0){
				strPar1 = Str_ReadSubItem(&strcDID," ");
				cDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
					command = (COMMAND_NODE*)FindInLChildRChainByDRNodeID(group, cDID);
				if (command != nullptr){
					command->Spin_InUse_set();
					BIC_GCM_LS::BIC_GCM_LS_PringCommand(tBICPAR,command,flag_gl);
					command->Spin_InUse_clr();
				}
			}
			PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
		}

		PrintDoRet(tBICPAR,"OK");
		ret_Code = cgReturnCode;
	}
	tBICPAR->sdtApp->m_GCList.Spin_InUse_clr();
	return(ret_Code);
}
//------------------------------------------------------------------------------------------//
struct BIC_GCM_GROUP_SET_PAR{
	int32	s,r,h,blnum;
	uint64	flag;
	std::string		c,t;
};
void ClearSetPar(struct BIC_GCM_GROUP_SET_PAR *tPar){
	if (tPar->blnum != 0){
		tPar->flag = 0;
		tPar->c = "";
		tPar->t = "";
		tPar->s = 0;
		tPar->r = 0;
		tPar->h = 0;
		tPar->blnum = 0;
	}
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_SET::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Set command.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [+]"			,"Set command selected.");
	PrintHelpItem(tBICPAR,"     [-]"			,"Set command disselected.");
	PrintHelpItem(tBICPAR,"     [-cN]"			,"Set cycle, N is value.");
	PrintHelpItem(tBICPAR,"     [-tN]"			,"Set timeout, N is value.");
	PrintHelpItem(tBICPAR,"     [-H]"			,"Set HEX.");
	PrintHelpItem(tBICPAR,"     [-A]"			,"Set ASCII.");
	PrintHelpItem(tBICPAR,"     [+cr]"			,"Enable send \"\\r\" in the end.");
	PrintHelpItem(tBICPAR,"     [-cr]"			,"Disable send \"\\r\" in the end.");
	PrintHelpItem(tBICPAR,"     <cID>"			,"Command cID, s/uns.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_SET::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	//set +/- num num
	//set c3 num num
	//set t200 num num
	//set + c3 t300 num num - num num c4 num
	std::string		strPar1,strPar2;
	uint32			cDID,ret_code;
	BIC_GCM_GROUP_SET_PAR	setPar;
	COMMAND_GROUP	*group;
	COMMAND_NODE	*command;
	
	group = nullptr;
	command = nullptr;
	
	setPar.blnum = 1;
	ClearSetPar(&setPar);
	ret_code = cgReturnCode;
	if (par.length() > 0){
		tBICPAR->sdtApp->m_GCList.Spin_InUse_set();
		group = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&tBICPAR->sdtApp->m_GCList, tBICPAR->gDID);
		if (group != nullptr){
			strPar2 = par;
			while(strPar2.length() > 0){
				strPar1 = Str_Trim(Str_ReadSubItem(&strPar2, " "));
				if (strPar1.length() > 0){
					if (strPar1 == "+"){
						ClearSetPar(&setPar);
						setPar.flag |= BIC_GCM_LS_FLAG_CLsetSelect;
						setPar.s = 1;
					}
					else if (strPar1 == "-"){
						ClearSetPar(&setPar);
						setPar.flag |= BIC_GCM_LS_FLAG_CLsetSelect;
						setPar.s = 0;
					}
					else if (strPar1 == "-H"){
						ClearSetPar(&setPar);
						setPar.flag |= BIC_GCM_LS_FLAG_CLsetHEX;
						setPar.h = 1;
					}
					else if (strPar1 == "-A"){
						ClearSetPar(&setPar);
						setPar.flag |= BIC_GCM_LS_FLAG_CLsetHEX;
						setPar.h = 0;
					}
					else if (strPar1 == "+cr"){
						ClearSetPar(&setPar);
						setPar.flag |= BIC_GCM_LS_FLAG_CLsetCR;
						setPar.r = 1;
					}
					else if (strPar1 == "-cr"){
						ClearSetPar(&setPar);
						setPar.flag |= BIC_GCM_LS_FLAG_CLsetCR;
						setPar.r = 0;
					}
					else if ((strPar1.length() > 1) && (strPar1.substr(0,2) == "-c")){
						ClearSetPar(&setPar);
						strPar1 = strPar1.substr(2);
						setPar.flag |= BIC_GCM_LS_FLAG_CLsetCycle;
						setPar.c = strPar1;
					}
					else if ((strPar1.length() > 1) && (strPar1.substr(0,2) == "-t")){
						ClearSetPar(&setPar);
						strPar1 = strPar1.substr(2);
						setPar.flag |= BIC_GCM_LS_FLAG_CLsetTimeout;
						setPar.t = strPar1;
					}
					else{
						setPar.blnum = 1;
						if (strPar1 == "s"){
							setPar.flag |= BIC_GCM_LS_FLAG_CLSelect;
							SetPar(tBICPAR,group,&setPar);
						}
						else if (strPar1 == "uns"){
							setPar.flag &= (~BIC_GCM_LS_FLAG_CLSelect);
							SetPar(tBICPAR,group,&setPar);
						}
						else{
							cDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
							command = (COMMAND_NODE*)FindInLChildRChainByDRNodeID(group, cDID);
							setPar.flag &= (~BIC_GCM_LS_FLAG_CLSelect);
							if (command != nullptr){
								command->Spin_InUse_set();
								SetPar(tBICPAR,command,&setPar);
								command->Spin_InUse_clr();
							}
						}
					}
				}
			}
			tBICPAR->sdtApp->m_GCList.SetblUpdate();
			PrintDoRet(tBICPAR,"OK");
		}
		else{
			PrintDoRet(tBICPAR,"ERROR gID");
			ret_code = BI_RETCODE_RETURN;
		}
		tBICPAR->sdtApp->m_GCList.Spin_InUse_clr();
	}
	return(ret_code);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_SET::SetPar(BICPAR *tBICPAR, COMMAND_NODE *tCommand, struct BIC_GCM_GROUP_SET_PAR *tPar){
	if (tCommand != nullptr){
		if ((tPar->flag & BIC_GCM_LS_FLAG_CLsetSelect) != 0)
			tCommand->blEnableSend = tPar->s;
		if ((tPar->flag & BIC_GCM_LS_FLAG_CLsetCycle) != 0)
			tCommand->StrCycle = tPar->c;
		if ((tPar->flag & BIC_GCM_LS_FLAG_CLsetTimeout) != 0)
			tCommand->StrTimeout = tPar->t;
		if ((tPar->flag & BIC_GCM_LS_FLAG_CLsetHEX) != 0)
			tCommand->blEnableHEX = tPar->h;
		if ((tPar->flag & BIC_GCM_LS_FLAG_CLsetCR) != 0)
			tCommand->blEnableSendCR = tPar->r;
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_SET::SetPar(BICPAR *tBICPAR, COMMAND_GROUP *tGroup, struct BIC_GCM_GROUP_SET_PAR *tPar){

	if (tGroup == nullptr)
		return 0;
	RTREE_LChildRChain_Traversal_LINE(COMMAND_NODE,tGroup,
		operateNode_t->Spin_InUse_set();
		if (((tPar->flag & BIC_GCM_LS_FLAG_CLSelect) != 0) && (operateNode_t->blEnableSend != 0))
			SetPar(tBICPAR,operateNode_t,tPar);
		if (((tPar->flag & BIC_GCM_LS_FLAG_CLSelect) == 0) && (operateNode_t->blEnableSend == 0))
			SetPar(tBICPAR,operateNode_t,tPar);
		operateNode_t->Spin_InUse_clr();
	);
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_CLONE::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Clone command by cID.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     [cID]"			,"Command cID.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_CLONE::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	COMMAND_NODE	*nextNode,*newNode;
	COMMAND_GROUP	*group;
	uint32			cDID,ret_code;
	
	*ret = "";
	nextNode = nullptr;
	newNode = nullptr;
	group = nullptr;
	tBICPAR->sdtApp->m_GCList.Spin_InUse_set();
	group = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&tBICPAR->sdtApp->m_GCList, tBICPAR->gDID);
	if (group != nullptr){
		newNode = new COMMAND_NODE;
		if (newNode != nullptr){
			newNode->StrCommand = "new command";
			if (par.length() > 0){
				cDID = (uint32)strtol(par.c_str(),nullptr,10);
				nextNode = (COMMAND_NODE*)FindInLChildRChainByDRNodeID(group, cDID);
				if (nextNode != nullptr)
					COMMAND_NODE::CopyCOMMAND_NODE(nextNode, newNode);
			}
			
			PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
			BIC_GCM_LS::BIC_GCM_LS_PringCommandTitle(tBICPAR,BIC_GCM_LS_FLAG_CLshowCycle | BIC_GCM_LS_FLAG_CLshowTimeout | BIC_GCM_LS_FLAG_CLshowDetail);
			BIC_GCM_LS::BIC_GCM_LS_PringCommand(tBICPAR,newNode, BIC_GCM_LS_FLAG_CLshowCycle | BIC_GCM_LS_FLAG_CLshowTimeout | BIC_GCM_LS_FLAG_CLshowDetail);
			PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
			PrintDoRet(tBICPAR,"Success");
			group->AddNode(newNode);
			tBICPAR->sdtApp->m_GCList.Spin_InUse_set();
		}
		else{
			PrintDoRet(tBICPAR,"Fail");
		}
		ret_code = cgReturnCode;
	}
	else{
		PrintDoRet(tBICPAR,"ERROR gID");
		ret_code = BI_RETCODE_RETURN;
	}
	tBICPAR->sdtApp->m_GCList.Spin_InUse_clr();
	return(ret_code);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_DEL::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Delete command by cID.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     <cID>"			,"Command cID.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_DEL::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	COMMAND_GROUP	*group;
	COMMAND_NODE	*nextNode;
	std::string		strPar1,strPar2;
	uint32			cDID;
	
	*ret = "";
	nextNode = nullptr;
	if (par.length() > 0){
		tBICPAR->sdtApp->m_GCList.Spin_InUse_set();
		group = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&tBICPAR->sdtApp->m_GCList, tBICPAR->gDID);
		if (group != nullptr){
			strPar2 = par;
			while(strPar2.length() > 0){
				strPar1 = Str_Trim(Str_ReadSubItem(&strPar2, " "));
				if (strPar1.length() > 0){
					cDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
					nextNode = (COMMAND_NODE*)FindInLChildRChainByDRNodeID(group, cDID);
					MoveNodeToTrash(nextNode,&tBICPAR->sdtApp->m_GCList);
					tBICPAR->sdtApp->m_GCList.SetblUpdate();
				}
			}
		}
		CleanTrash(&tBICPAR->sdtApp->m_GCList);
		tBICPAR->sdtApp->m_GCList.Spin_InUse_clr();
		if (group == nullptr){
			PrintDoRet(tBICPAR,"ERROR gID");
			return(BI_RETCODE_RETURN);
		}
	}
	PrintDoRet(tBICPAR,"Success");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_MVUP::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Move command up.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     <cID>"			,"Command cID.");
	PrintHelpItem(tBICPAR,"     [step]"			,"Default value is 1.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_MVUP::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	COMMAND_GROUP	*group;
	COMMAND_NODE	*nextNode;
	std::string		strPar1,strPar2,strDoRet;
	uint32			cDID,step;
	
	*ret = "";
	nextNode = nullptr;
	group = nullptr;
	strDoRet = "Fail";
	if (par.length() > 0){
		tBICPAR->sdtApp->m_GCList.Spin_InUse_set();
		group = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&tBICPAR->sdtApp->m_GCList, tBICPAR->gDID);
		if (group != nullptr){
			strPar2 = par;
			strPar1 = Str_Trim(Str_ReadSubItem(&strPar2, " "));
			cDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
			step = (uint32)strtol(strPar2.c_str(),nullptr,10);
			if (step == 0)
				step = 1;
			
			nextNode = (COMMAND_NODE*)FindInLChildRChainByDRNodeID(group, cDID);
			if (nextNode != nullptr){
				while(step-- > 0)
					MoveNodesUpInRChain(nextNode);
				strDoRet = "Success";
				tBICPAR->sdtApp->m_GCList.SetblUpdate();
			}
		}
		tBICPAR->sdtApp->m_GCList.Spin_InUse_clr();
		if (group == nullptr){
			PrintDoRet(tBICPAR,"ERROR gID");
			return(BI_RETCODE_RETURN);
		}
	}
	PrintDoRet(tBICPAR,strDoRet);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_MVDN::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Move command down.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"     <cID>"			,"Command cID.");
	PrintHelpItem(tBICPAR,"     [step]"			,"Default value is 1.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_MVDN::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	COMMAND_GROUP	*group;
	COMMAND_NODE	*nextNode;
	std::string		strPar1,strPar2,strDoRet;
	uint32			cDID,step;
	
	*ret = "";
	nextNode = nullptr;
	group = nullptr;
	strDoRet = "Fail";
	if (par.length() > 0){
		tBICPAR->sdtApp->m_GCList.Spin_InUse_set();
		group = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&tBICPAR->sdtApp->m_GCList, tBICPAR->gDID);
		if (group != nullptr){
			strPar2 = par;
			strPar1 = Str_Trim(Str_ReadSubItem(&strPar2, " "));
			cDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
			step = (uint32)strtol(strPar2.c_str(),nullptr,10);
			if (step == 0)
				step = 1;
			
			nextNode = (COMMAND_NODE*)FindInLChildRChainByDRNodeID(group, cDID);
			if (nextNode != nullptr){
				while(step-- > 0)
					MoveNodesDownInRChain(nextNode);
				strDoRet = "Success";
				tBICPAR->sdtApp->m_GCList.SetblUpdate();
			}
		}
		tBICPAR->sdtApp->m_GCList.Spin_InUse_clr();
		if (group == nullptr){
			PrintDoRet(tBICPAR,"ERROR gID");
			return(BI_RETCODE_RETURN);
		}
	}
	PrintDoRet(tBICPAR,strDoRet);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_COMMAND::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Set command by cID.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"    <cID>"		,"Command cID.");
	PrintHelpItem(tBICPAR,"    <name>"		,"Name.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_COMMAND::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	COMMAND_GROUP	*group;
	COMMAND_NODE	*command;
	std::string		strPar1,strPar2,strDoRet;
	uint32			cDID;
	
	*ret = "";
	strDoRet = "Fail";
	group = nullptr;
	if (par.length() > 0){
		tBICPAR->sdtApp->m_GCList.Spin_InUse_set();
		group = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&tBICPAR->sdtApp->m_GCList, tBICPAR->gDID);
		if (group != nullptr){
			strPar2 = par;
			strPar1 = Str_ReadSubItem(&strPar2, " ");
			cDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
			command = (COMMAND_NODE*)FindInLChildRChainByDRNodeID(group, cDID);
			if (command != nullptr){
				command->Spin_InUse_set();
				command->StrCommand = strPar2;
				PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
				BIC_GCM_LS::BIC_GCM_LS_PringCommandTitle(tBICPAR,BIC_GCM_LS_FLAG_CLshowCycle | BIC_GCM_LS_FLAG_CLshowTimeout | BIC_GCM_LS_FLAG_CLshowDetail);
				BIC_GCM_LS::BIC_GCM_LS_PringCommand(tBICPAR,command,BIC_GCM_LS_FLAG_CLshowCycle | BIC_GCM_LS_FLAG_CLshowTimeout | BIC_GCM_LS_FLAG_CLshowDetail);
				command->Spin_InUse_clr();
				PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
				strDoRet = "Success";
				tBICPAR->sdtApp->m_GCList.SetblUpdate();
			}
		}
		tBICPAR->sdtApp->m_GCList.Spin_InUse_clr();
		if (group == nullptr){
			PrintDoRet(tBICPAR,"ERROR gID");
			return(BI_RETCODE_RETURN);
		}
	}
	PrintDoRet(tBICPAR,strDoRet);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_CONTINUE::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Set continue by cID.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"    <cID>"		,"Command cID.");
	PrintHelpItem(tBICPAR,"    <name>"		,"Name.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_CONTINUE::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	COMMAND_GROUP	*group;
	COMMAND_NODE	*command;
	std::string		strPar1,strPar2,strDoRet;
	uint32			cDID;
	
	*ret = "";
	strDoRet = "Fail";
	group = nullptr;
	if (par.length() > 0){
		tBICPAR->sdtApp->m_GCList.Spin_InUse_set();
		group = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&tBICPAR->sdtApp->m_GCList, tBICPAR->gDID);
		if (group != nullptr){
			strPar2 = par;
			strPar1 = Str_ReadSubItem(&strPar2, " ");
			cDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
			command = (COMMAND_NODE*)FindInLChildRChainByDRNodeID(group, cDID);
			if (command != nullptr){
				command->Spin_InUse_set();
				command->StrContinue = strPar2;
				PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
				BIC_GCM_LS::BIC_GCM_LS_PringCommandTitle(tBICPAR,BIC_GCM_LS_FLAG_CLshowCycle | BIC_GCM_LS_FLAG_CLshowTimeout | BIC_GCM_LS_FLAG_CLshowDetail);
				BIC_GCM_LS::BIC_GCM_LS_PringCommand(tBICPAR,command,BIC_GCM_LS_FLAG_CLshowCycle | BIC_GCM_LS_FLAG_CLshowTimeout | BIC_GCM_LS_FLAG_CLshowDetail);
				command->Spin_InUse_clr();
				PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
				strDoRet = "Success";
				tBICPAR->sdtApp->m_GCList.SetblUpdate();
			}
		}
		tBICPAR->sdtApp->m_GCList.Spin_InUse_clr();
		if (group == nullptr){
			PrintDoRet(tBICPAR,"ERROR gID");
			return(BI_RETCODE_RETURN);
		}
	}
	PrintDoRet(tBICPAR,strDoRet);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_RESEND::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Set resend by cID.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"    <cID>"		,"Command cID.");
	PrintHelpItem(tBICPAR,"    <name>"		,"Name.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_RESEND::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	COMMAND_GROUP	*group;
	COMMAND_NODE	*command;
	std::string		strPar1,strPar2,strDoRet;
	uint32			cDID;
	
	*ret = "";
	strDoRet = "Fail";
	group = nullptr;
	if (par.length() > 0){
		tBICPAR->sdtApp->m_GCList.Spin_InUse_set();
		group = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&tBICPAR->sdtApp->m_GCList, tBICPAR->gDID);
		if (group != nullptr){
			strPar2 = par;
			strPar1 = Str_ReadSubItem(&strPar2, " ");
			cDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
			command = (COMMAND_NODE*)FindInLChildRChainByDRNodeID(group, cDID);
			if (command != nullptr){
				command->Spin_InUse_set();
				command->StrResend = strPar2;
				PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
				BIC_GCM_LS::BIC_GCM_LS_PringCommandTitle(tBICPAR,BIC_GCM_LS_FLAG_CLshowCycle | BIC_GCM_LS_FLAG_CLshowTimeout | BIC_GCM_LS_FLAG_CLshowDetail);
				BIC_GCM_LS::BIC_GCM_LS_PringCommand(tBICPAR,command,BIC_GCM_LS_FLAG_CLshowCycle | BIC_GCM_LS_FLAG_CLshowTimeout | BIC_GCM_LS_FLAG_CLshowDetail);
				command->Spin_InUse_clr();
				PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
				strDoRet = "Success";
				tBICPAR->sdtApp->m_GCList.SetblUpdate();
			}
		}
		tBICPAR->sdtApp->m_GCList.Spin_InUse_clr();
		if (group == nullptr){
			PrintDoRet(tBICPAR,"ERROR gID");
			return(BI_RETCODE_RETURN);
		}
	}
	PrintDoRet(tBICPAR,strDoRet);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_CSTOP::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Set cstop by cID.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"    <cID>"		,"Command cID.");
	PrintHelpItem(tBICPAR,"    <name>"		,"Name.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_CSTOP::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	COMMAND_GROUP	*group;
	COMMAND_NODE	*command;
	std::string		strPar1,strPar2,strDoRet;
	uint32			cDID;
	
	*ret = "";
	strDoRet = "Fail";
	group = nullptr;
	if (par.length() > 0){
		tBICPAR->sdtApp->m_GCList.Spin_InUse_set();
		group = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&tBICPAR->sdtApp->m_GCList, tBICPAR->gDID);
		if (group != nullptr){
			strPar2 = par;
			strPar1 = Str_ReadSubItem(&strPar2, " ");
			cDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
			command = (COMMAND_NODE*)FindInLChildRChainByDRNodeID(group, cDID);
			if (command != nullptr){
				command->Spin_InUse_set();
				command->StrStop = strPar2;
				PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
				BIC_GCM_LS::BIC_GCM_LS_PringCommandTitle(tBICPAR,BIC_GCM_LS_FLAG_CLshowCycle | BIC_GCM_LS_FLAG_CLshowTimeout | BIC_GCM_LS_FLAG_CLshowDetail);
				BIC_GCM_LS::BIC_GCM_LS_PringCommand(tBICPAR,command,BIC_GCM_LS_FLAG_CLshowCycle | BIC_GCM_LS_FLAG_CLshowTimeout | BIC_GCM_LS_FLAG_CLshowDetail);
				command->Spin_InUse_clr();
				PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
				strDoRet = "Success";
				tBICPAR->sdtApp->m_GCList.SetblUpdate();
			}
		}
		tBICPAR->sdtApp->m_GCList.Spin_InUse_clr();
		if (group == nullptr){
			PrintDoRet(tBICPAR,"ERROR gID");
			return(BI_RETCODE_RETURN);
		}
	}
	PrintDoRet(tBICPAR,strDoRet);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_CATCH::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Set catch by cID.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"    <cID>"		,"Command cID.");
	PrintHelpItem(tBICPAR,"    <name>"		,"Name.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_CATCH::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	COMMAND_GROUP	*group;
	COMMAND_NODE	*command;
	std::string		strPar1,strPar2,strDoRet;
	uint32			cDID;
	
	*ret = "";
	strDoRet = "Fail";
	group = nullptr;
	if (par.length() > 0){
		tBICPAR->sdtApp->m_GCList.Spin_InUse_set();
		group = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&tBICPAR->sdtApp->m_GCList, tBICPAR->gDID);
		if (group != nullptr){
			strPar2 = par;
			strPar1 = Str_ReadSubItem(&strPar2, " ");
			cDID = (uint32)strtol(strPar1.c_str(),nullptr,10);
			command = (COMMAND_NODE*)FindInLChildRChainByDRNodeID(group, cDID);
			if (command != nullptr){
				command->Spin_InUse_set();
				command->StrCatch = strPar2;
				PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
				BIC_GCM_LS::BIC_GCM_LS_PringCommandTitle(tBICPAR,BIC_GCM_LS_FLAG_CLshowCycle | BIC_GCM_LS_FLAG_CLshowTimeout | BIC_GCM_LS_FLAG_CLshowDetail);
				BIC_GCM_LS::BIC_GCM_LS_PringCommand(tBICPAR,command,BIC_GCM_LS_FLAG_CLshowCycle | BIC_GCM_LS_FLAG_CLshowTimeout | BIC_GCM_LS_FLAG_CLshowDetail);
				command->Spin_InUse_clr();
				PrintStrN(tBICPAR,DEV_LINE_START,RICH_LIN_clDefault);
				strDoRet = "Success";
				tBICPAR->sdtApp->m_GCList.SetblUpdate();
			}
		}
		tBICPAR->sdtApp->m_GCList.Spin_InUse_clr();
		if (group == nullptr){
			PrintDoRet(tBICPAR,"ERROR gID");
			return(BI_RETCODE_RETURN);
		}
	}
	PrintDoRet(tBICPAR,strDoRet);
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_SEND::Help(BICPAR *tBICPAR, int32 blDetail)const{
	PrintHelpItem(tBICPAR, cgCommand, "Send command by cID.");
	if (blDetail == 0)
		return(cgReturnCode);
	PrintHelpItem(tBICPAR,"    <cID>"		,"Command cID.");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_GCM_GROUP_SEND::Command(BICPAR *tBICPAR, const std::string &par,std::string *ret)const{
	COMMAND_GROUP	*group;
	COMMAND_NODE	command,*node;
	uint32			cDID;
	
	*ret = "";
	node = nullptr;
	COMMAND_NODE::Init(&command);
	
	if (par.length() > 0){
		tBICPAR->sdtApp->m_GCList.Spin_InUse_set();
		group = (COMMAND_GROUP*)FindInLChildRChainByDRNodeID(&tBICPAR->sdtApp->m_GCList, tBICPAR->gDID);
		if (group != nullptr){
			cDID = (uint32)strtol(par.c_str(),nullptr,10);
			node = (COMMAND_NODE*)FindInLChildRChainByDRNodeID(group, cDID);
			if (node != nullptr){
				COMMAND_NODE::CopyCOMMAND_NODE(node, &command);
				command.blEnableSend = 1;
			}
		}
		tBICPAR->sdtApp->m_GCList.Spin_InUse_clr();
		if (group == nullptr){
			PrintDoRet(tBICPAR,"ERROR gID");
			return(BI_RETCODE_RETURN);
		}
	}
	if (node != nullptr){
		tBICPAR->oDevNode->Enable();
		if (tBICPAR->sdtApp->m_Script.Execute(&tBICPAR->sdtApp->m_Device,&command) == 0)
			PrintDoRet(tBICPAR,"Script is running!");
		while(tBICPAR->blExit == 0){
			uint8	chkey;
			SYS_SleepMS(10);
			tBICPAR->blInPressKeyMode = 1;
			chkey = BI_ReadChar(tBICPAR,0);
			if (chkey == 27){
				break;
			}
			else if (chkey == '\r'){
				tBICPAR->oDevNode->Enable();
			}
			if (tBICPAR->sdtApp->m_Script.IsStop() != 0)
				break;
		}
		tBICPAR->blInPressKeyMode = 0;
	}
	else{
		PrintDoRet(tBICPAR,"Fail");
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//

