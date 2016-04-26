/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: CommandGList.cpp
 * File ID	:
 * Remark	:
 * Writer   : Leif Wen
 * Date     : 2013.12.31
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "CommandGList.h"
#include "Comm_Convert.h"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void COMMAND_NODE::Init(G_LOCK_VAILD blLock){
	Spin_InUse_set(blLock);

	StrCommand = "";
	blEnableSend = 1;
	blEnableSendCR = 1;
	blEnableHEX = 0;
	
	StrShowName = "";
	blSameAsCommand = 1;
	
	StrTimeout = "1";
	StrCycle = "1";
	
	StrContinue = "";
	StrResend = "";
	StrStop = "";
	StrCatch = "";
	
	runTimes = 0;
	catchTimes = 0;
	timeoutTimes = 0;
	blFirstSynchronous = 0;
	timeST0.Clear();
	
	Spin_InUse_clr(blLock);
}
//------------------------------------------------------------------------------------------//
std::string COMMAND_NODE::CreateNodeStrV0_5(void){
	//V0.5
	//	[node]
	//		StrCommand =
	//		blEnableSend =
	//		blEnableSendCR =
	//		blEnableHEX =
	//		StrShowName =
	//		blSameAsCommand =
	//		StrTimeout =
	//		StrCycle =
	//		StrContinue =
	//		StrResend =
	//		StrStop =
	//		StrCatch =
	//	[node_end]
	std::string		strResult;
	
	Spin_InUse_set();
		strResult  =  "    [node]\n";
		strResult += ("      strCommand = " + Str_Trim(StrCommand) + "\n");
		if (blEnableSend == 0)
			strResult += ("      blEnableSend = 0\n");
		if (blEnableSendCR == 0)
			strResult += ("      blEnableSendCR = 0\n");
		if (blEnableHEX != 0)
			strResult += ("      blEnableHEX = 1\n");
		if (blSameAsCommand == 0){
			if (Str_Trim(StrShowName) != Str_Trim(StrCommand))
				strResult += ("      StrShowName = " + Str_Trim(StrShowName) + "\n");
		}
		if ((StrTimeout.length() > 0) && (StrTimeout != "1"))
			strResult += ("      strTimeout = " + Str_Trim(StrTimeout) + "\n");
		if ((StrCycle.length() > 0) && (StrCycle != "1"))
			strResult += ("      strCycle = " + Str_Trim(StrCycle) + "\n");
		if (StrContinue.length() > 0)
			strResult += ("      strContinue = " + Str_Trim(StrContinue) + "\n");
		if (StrResend.length() > 0)
			strResult += ("      strResend = " + Str_Trim(StrResend) + "\n");
		if (StrStop.length() > 0)
			strResult += ("      strStop = " + Str_Trim(StrStop) + "\n");
		if (StrCatch.length() > 0)
			strResult += ("      strCatch = " + Str_Trim(StrCatch) + "\n");
		strResult +=  "    [node_end]\n";
	Spin_InUse_clr();
	return(strResult);
}
//------------------------------------------------------------------------------------------//
std::string COMMAND_NODE::CreateNodeStrV0_4(void){
	//V0.4
	//	[node]
	//		blEnableSend =
	//		blEnableSendCR =
	//		blEnableHEX =
	//		strTimeout =
	//		strCommand =
	//		strContinue =
	//		strStop =
	//		strResend =
	//		strCycle =
	//		strCatch =
	//	[node_end]
	std::string		strResult;
	
	Spin_InUse_set();
		strResult  =  "    [node]\n";
		if (blEnableSend == 0)
			strResult += ("      blEnableSend = 0\n");
		if (blEnableSendCR == 0)
			strResult += ("      blEnableSendCR = 0\n");
		if (blEnableHEX != 0)
			strResult += ("      blEnableHEX = 1\n");
		if (StrTimeout != "")
			strResult += ("      strTimeout = " + StrTimeout + "\n");
		strResult += ("      strCommand = " + StrCommand + "\n");
		if (StrContinue != "")
			strResult += ("      strContinue = " + StrContinue + "\n");
		if (StrStop != "")
			strResult += ("      strStop = " + StrStop + "\n");
		if (StrResend != "")
			strResult += ("      strResend = " + StrResend + "\n");
		if (StrCycle != "")
			strResult += ("      strCycle = " + StrCycle + "\n");
		if (StrCatch != "")
			strResult += ("      strCatch = " + StrCatch + "\n");
		strResult +=  "    [node_end]\n";
	Spin_InUse_clr();
	return(strResult);
}
//------------------------------------------------------------------------------------------//
std::string COMMAND_NODE::CreateNodeStrV0_2(void){
	//V0.2
	//(blEnableSend,blEnableSendCR,blEnableHEX,StrTimeout,StrCommand,StrContinue,StrStop,StrResend,StrCycle,StrCatch)
	std::string		strResult,strTemp;
	
	Spin_InUse_set();
		strResult = '(';
		
		strTemp = Str_IntToString(blEnableSend);
		strResult += Str_ASCIIToHEX(strTemp,G_ESCAPE_OFF);
		strResult += ",";
		
		strTemp = Str_IntToString(blEnableSendCR);
		strResult += Str_ASCIIToHEX(strTemp,G_ESCAPE_OFF);
		strResult += ",";
		
		strTemp = Str_IntToString(blEnableHEX);
		strResult += Str_ASCIIToHEX(strTemp,G_ESCAPE_OFF);
		strResult += ",";
		
		strResult += Str_ASCIIToHEX(StrTimeout,G_ESCAPE_OFF);
		strResult += ",";
		
		strResult += Str_ASCIIToHEX(StrCommand,G_ESCAPE_OFF);
		strResult += ",";
		
		strResult += Str_ASCIIToHEX(StrContinue,G_ESCAPE_OFF);
		strResult += ",";
		
		strResult += Str_ASCIIToHEX(StrStop,G_ESCAPE_OFF);
		strResult += ",";
		
		strResult += Str_ASCIIToHEX(StrResend,G_ESCAPE_OFF);
		strResult += ",";
		
		strResult += Str_ASCIIToHEX(StrCycle,G_ESCAPE_OFF);
		strResult += ",";
		
		strResult += Str_ASCIIToHEX(StrCatch,G_ESCAPE_OFF);
		strResult += ')';
	Spin_InUse_clr();
	return(strResult);
}
//------------------------------------------------------------------------------------------//
void COMMAND_NODE::SetNodeV0_5(std::string *strInput){
	//V0.5
	//	[node]
	//		StrCommand =
	//		blEnableSend =
	//		blEnableSendCR =
	//		blEnableHEX =
	//		StrShowName =
	//		blSameAsCommand =
	//		StrTimeout =
	//		StrCycle =
	//		StrContinue =
	//		StrResend =
	//		StrStop =
	//		StrCatch =
	//	[node_end]
	std::string		strLine,strItem;
	
	Spin_InUse_set();
		Init(G_LOCK_OFF);
		while(strInput->length() > 0){
			strLine = Str_Trim(Str_ReadSubItem(strInput,"\n"));
			if (Str_LowerCase(strLine) == "[node_end]")
				break;
			strItem = Str_LowerCase(Str_Trim(Str_ReadSubItem(&strLine,"=")));
			strLine = Str_Trim(strLine);
			if (strItem == "strcommand"){
				StrCommand = strLine;
			}
			else if (strItem == "blenablesend"){
				blEnableSend = (atoi(strLine.c_str()) != 0);
			}
			else if (strItem == "blenablesendcr"){
				blEnableSendCR = (atoi(strLine.c_str()) != 0);
			}
			else if (strItem == "blenablehex"){
				blEnableHEX = (atoi(strLine.c_str()) != 0);
			}
			else if (strItem == "strshowname"){
				StrShowName = strLine;
				blSameAsCommand = 0;
			}
			else if (strItem == "strtimeout"){
				StrTimeout = strLine;
			}
			else if (strItem == "strcycle"){
				StrCycle = strLine;
			}
			else if (strItem == "strcontinue"){
				StrContinue = strLine;
			}
			else if (strItem == "strresend"){
				StrResend = strLine;
			}
			else if (strItem == "strstop"){
				StrStop = strLine;
			}
			else if (strItem == "strcatch"){
				StrCatch = strLine;
			}
		}
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void COMMAND_NODE::SetNodeV0_4(std::string *strInput){
	//V0.4
	//	[node]
	//		blEnableSend =
	//		blEnableSendCR =
	//		blEnableHEX =
	//		strTimeout =
	//		strCommand =
	//		strContinue =
	//		strStop =
	//		strResend =
	//		strCycle =
	//		strCatch =
	//	[node_end]
	std::string		strLine,strItem;
	Spin_InUse_set();
		Init(G_LOCK_OFF);
		while(strInput->length() > 0){
			strLine = Str_Trim(Str_ReadSubItem(strInput,"\n"));
			if (Str_LowerCase(strLine) == "[node_end]")
				break;
			strItem = Str_LowerCase(Str_Trim(Str_ReadSubItem(&strLine,"=")));
			strLine = Str_Trim(strLine);
			if (strItem == "strcommand"){
				StrCommand = strLine;
			}
			else if (strItem == "blenablesend"){
				blEnableSend = (atoi(strLine.c_str()) != 0);
			}
			else if (strItem == "blenablesendcr"){
				blEnableSendCR = (atoi(strLine.c_str()) != 0);
			}
			else if (strItem == "blenablehex"){
				blEnableHEX = (atoi(strLine.c_str()) != 0);
			}
			else if (strItem == "strtimeout"){
				StrTimeout = strLine;
			}
			else if (strItem == "strcycle"){
				StrCycle = strLine;
			}
			else if (strItem == "strcontinue"){
				StrContinue = strLine;
			}
			else if (strItem == "strresend"){
				StrResend = strLine;
			}
			else if (strItem == "strstop"){
				StrStop = strLine;
			}
			else if (strItem == "strcatch"){
				StrCatch = strLine;
			}
		}
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void COMMAND_NODE::SetNodeV0_3(std::string *strInput){
	//V0.3
	//update:StrTimeout
	//:(blEnableSend,blEnableSendCR,blEnableHEX,StrTimeout,StrCommand,StrContinue,StrStop,StrResend,StrCycle,StrCatch)
	Spin_InUse_set();
		Init(G_LOCK_OFF);
		blEnableSend = atoi(Str_HEXToASCII(Str_ReadSubItem(strInput,",")).c_str());
		blEnableSendCR = atoi(Str_HEXToASCII(Str_ReadSubItem(strInput,",")).c_str());
		blEnableHEX = atoi(Str_HEXToASCII(Str_ReadSubItem(strInput,",")).c_str());
		
		StrTimeout = Str_HEXToASCII(Str_ReadSubItem(strInput,","));
		StrCommand = Str_HEXToASCII(Str_ReadSubItem(strInput,","));
		StrContinue = Str_HEXToASCII(Str_ReadSubItem(strInput,","));
		StrStop = Str_HEXToASCII(Str_ReadSubItem(strInput,","));
		StrResend = Str_HEXToASCII(Str_ReadSubItem(strInput,","));
		StrCycle = Str_HEXToASCII(Str_ReadSubItem(strInput,","));
		StrCatch = Str_HEXToASCII(Str_ReadSubItem(strInput,","));
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void COMMAND_NODE::SetNodeV0_2(std::string *strInput){
	//V0.2
	//:(blEnableSend,blEnableSendCR,blEnableHEX,StrTimeout,StrCommand,StrContinue,StrStop,StrResend,StrCycle,StrCatch)
	double		timeout;
	Spin_InUse_set();
		Init(G_LOCK_OFF);
		blEnableSend = atoi(Str_HEXToASCII(Str_ReadSubItem(strInput,",")).c_str());
		blEnableSendCR = atoi(Str_HEXToASCII(Str_ReadSubItem(strInput,",")).c_str());
		blEnableHEX = atoi(Str_HEXToASCII(Str_ReadSubItem(strInput,",")).c_str());
		
		StrTimeout = Str_HEXToASCII(Str_ReadSubItem(strInput,","));
		timeout = atoi(StrTimeout.c_str()) / 1000.0;
		
		StrTimeout = Str_FloatToString(timeout);
		StrCommand = Str_HEXToASCII(Str_ReadSubItem(strInput,","));
		StrContinue = Str_HEXToASCII(Str_ReadSubItem(strInput,","));
		StrStop = Str_HEXToASCII(Str_ReadSubItem(strInput,","));
		StrResend = Str_HEXToASCII(Str_ReadSubItem(strInput,","));
		StrCycle = Str_HEXToASCII(Str_ReadSubItem(strInput,","));
		StrCatch = Str_HEXToASCII(Str_ReadSubItem(strInput,","));
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
COMMAND_GROUP::COMMAND_GROUP(void) : RTREE_NODE(){
	name = "";
	intervalTime = 200;
	
	blEnableAutoRun = 0;
	autoRunTimes = 1;
	CreateTrash(this);
}
//------------------------------------------------------------------------------------------//
std::string COMMAND_GROUP::CreateGroupStrV0_5(void){
	//V0.5
	//	[group]
	//		groupName =
	//		intervalTime =
	//		[node]
	//		[node_end]
	//		[node]
	//		[node_end]
	//	[group_end]
	std::string		strResult;
	
	Spin_InUse_set();
	strResult  =  "  [group]\n";
	strResult += ("    groupName = " + name + "\n");
	strResult += ("    intervalTime = " + Str_IntToString(intervalTime) + "\n");
	
	RTREE_LChildRChain_Traversal_LINE_nolock(COMMAND_NODE,this,strResult += operateNode_t->CreateNodeStrV0_5());
	Spin_InUse_clr();
	
	strResult += "  [group_end]\n";
	return(strResult);
}
//------------------------------------------------------------------------------------------//
std::string COMMAND_GROUP::CreateGroupStrV0_4(void){
	//V0.4
	//	[group]
	//		groupName =
	//		intervalTime =
	//		[node]
	//		[node_end]
	//		[node]
	//		[node_end]
	//	[group_end]
	std::string	strResult;
	
	strResult  =  "  [group]\n";
	Spin_InUse_set();
	strResult += ("    groupName = " + name + "\n");
	strResult += ("    intervalTime = " + Str_IntToString(intervalTime) + "\n");
	
	RTREE_LChildRChain_Traversal_LINE_nolock(COMMAND_NODE,this,strResult += operateNode_t->CreateNodeStrV0_4());
	Spin_InUse_clr();
	
	strResult += "  [group_end]\n";
	return(strResult);
}
//------------------------------------------------------------------------------------------//
std::string COMMAND_GROUP::CreateGroupStrV0_2(void){
	//V0.2
	//Group:,GroupName,intervalTime,count(node),node
	//{(GroupName,intervalTime)(node1)(node2)}
	std::string	strResult,strTemp;
	
	Spin_InUse_set();
	strResult = "{(";
	strResult += Str_ASCIIToHEX(name,G_ESCAPE_OFF);
	strResult	+= ",";
	
	strTemp = Str_IntToString(intervalTime);
	strResult += Str_ASCIIToHEX(strTemp,G_ESCAPE_OFF);
	strResult	+= ")";
	
	RTREE_LChildRChain_Traversal_LINE_nolock(COMMAND_NODE,this,strResult += operateNode_t->CreateNodeStrV0_2());
	Spin_InUse_clr();
	
	strResult	+= '}';
	return(strResult);
}
//------------------------------------------------------------------------------------------//
void COMMAND_GROUP::SetGroupV0_5(std::string *strInput){
	//V0.5
	//	[group]
	//		groupName =
	//		intervalTime =
	//		[node]
	//		[node_end]
	//		[node]
	//		[node_end]
	//	[group_end]
	
	COMMAND_NODE	*node;
	std::string		strLine,strItem;
	
	Spin_InUse_set();
	name = "";
	intervalTime = 200;
	while(strInput->length() > 0){
		strLine = Str_Trim(Str_ReadSubItem(strInput,"\n"));
		if (Str_LowerCase(strLine) == "[group_end]")
			break;
		if (Str_LowerCase(strLine) == "[node]"){
			node = new COMMAND_NODE;
			if (node != nullptr){
				node->SetNodeV0_5(strInput);
				AddNode(node);
			}
			continue;
		}
		strItem = Str_LowerCase(Str_Trim(Str_ReadSubItem(&strLine,"=")));
		strLine = Str_Trim(strLine);
		if (strItem == "groupname"){
			name = strLine;
		}
		else if (strItem == "intervaltime"){
			intervalTime = atoi(strLine.c_str());
		}
	}
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void COMMAND_GROUP::SetGroupV0_4(std::string *strInput){
	//V0.4
	//	[group]
	//		groupName =
	//		intervalTime =
	//		[node]
	//		[node_end]
	//		[node]
	//		[node_end]
	//	[group_end]
	
	COMMAND_NODE	*node;
	std::string		strLine,strItem;
	
	Spin_InUse_set();
	name = "";
	intervalTime = 200;
	while(strInput->length() > 0){
		strLine = Str_Trim(Str_ReadSubItem(strInput,"\n"));
		if (Str_LowerCase(strLine) == "[group_end]")
			break;
		if (Str_LowerCase(strLine) == "[node]"){
			node = new COMMAND_NODE;
			if (node != nullptr){
				node->SetNodeV0_4(strInput);
				AddNode(node);
			}
			continue;
		}
		strItem = Str_LowerCase(Str_Trim(Str_ReadSubItem(&strLine,"=")));
		strLine = Str_Trim(strLine);
		if (strItem == "groupname"){
			name = strLine;
		}
		else if (strItem == "intervaltime"){
			intervalTime = atoi(strLine.c_str());
		}
	}
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void COMMAND_GROUP::SetGroupV0_3(std::string *strInput){
	/*
	 Group:,GroupName,intervalTime,count(Node),action
	 {(GroupName,intervalTime)(Node1)(Node2)}
	 */
	std::string	strResult;
	COMMAND_NODE *node;
	
	Spin_InUse_set();
	strResult = Str_ReadSubItem(strInput,"|");
	name = Str_HEXToASCII(Str_ReadSubItem(&strResult,","));
	
	intervalTime = atoi(Str_HEXToASCII(Str_ReadSubItem(&strResult,",")).c_str());
	
	while(strInput->length() > 0){
		strResult = Str_ReadSubItem(strInput,"|");
		node = new COMMAND_NODE;
		if (node != nullptr){
			node->SetNodeV0_3(&strResult);
			AddNode(node);
		}
	}
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void COMMAND_GROUP::SetGroupV0_2(std::string *strInput){
	/*
	 Group:,GroupName,intervalTime,count(Node),action
	 {(GroupName,intervalTime)(Node1)(Node2)}
	 */
	std::string		strResult;
	COMMAND_NODE	*node;
	
	Spin_InUse_set();
	strResult = Str_ReadSubItem(strInput,"|");
	name = Str_HEXToASCII(Str_ReadSubItem(&strResult,","));
	
	intervalTime = atoi(Str_HEXToASCII(Str_ReadSubItem(&strResult,",")).c_str());
	
	while(strInput->length() > 0){
		strResult = Str_ReadSubItem(strInput,"|");
		node = new COMMAND_NODE;
		if (node != nullptr){
			node->SetNodeV0_2(&strResult);
			AddNode(node);
		}
	}
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void COMMAND_GROUP::ClearResult(void){
	RTREE_LChildRChain_Traversal_LINE_nolock(COMMAND_NODE,this,
		operateNode_t->Spin_InUse_set();
		operateNode_t->runTimes = 0;
		operateNode_t->catchTimes = 0;
		operateNode_t->timeoutTimes = 0;
		operateNode_t->blFirstSynchronous = 0;
		operateNode_t->timeST0.Clear();
		operateNode_t->Spin_InUse_clr();
	);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
std::string GC_LIST::CreateGroupListStrV0_5(void){
	//V0.5
	//	[grouplist]
	//		[group]
	//		[group_end]
	//		[group]
	//		[group_end]
	//	[grouplist_end]
	
	std::string	strResult;
	strResult = "[grouplist]\n";
	
	Spin_InUse_set();
	RTREE_LChildRChain_Traversal_LINE(COMMAND_GROUP,this,strResult += operateNode_t->CreateGroupStrV0_5());
	Spin_InUse_clr();
	
	strResult += "[grouplist_end]\n";
	return(strResult);
}
//------------------------------------------------------------------------------------------//
std::string GC_LIST::CreateGroupListStrV0_4(void){
	//V0.4
	//	[grouplist]
	//		[group]
	//		[group_end]
	//		[group]
	//		[group_end]
	//	[grouplist_end]
	
	std::string	strResult;
	strResult = "[grouplist]\n";
	
	Spin_InUse_set();
	RTREE_LChildRChain_Traversal_LINE(COMMAND_GROUP,this,strResult += operateNode_t->CreateGroupStrV0_4());
	Spin_InUse_clr();
	
	strResult += "[grouplist_end]\n";
	return(strResult);
}
//------------------------------------------------------------------------------------------//
std::string GC_LIST::CreateGroupListStrV0_2(void){
	//V0.2
	//Group List: Group
	//{Group1}{Group2}
	
	std::string	strResult;
	
	strResult = "";
	
	Spin_InUse_set();
	RTREE_LChildRChain_Traversal_LINE(COMMAND_GROUP,this,strResult += operateNode_t->CreateGroupStrV0_2());
	Spin_InUse_clr();
	return(strResult);
}
//------------------------------------------------------------------------------------------//
void GC_LIST::SetGroupListV0_5(std::string *strInput){
	//V0.5
	//	[grouplist]
	//		[group]
	//		[group_end]
	//		[group]
	//		[group_end]
	//	[grouplist_end]
	COMMAND_GROUP	*group;
	std::string		strLine,strItem;
	
	Spin_InUse_set();
	while(strInput->length() > 0){
		strLine = Str_LowerCase(Str_Trim(Str_ReadSubItem(strInput,"\n")));
		if (strLine == "[grouplist_end]")
			break;
		if (strLine == "[group]"){
			group = new COMMAND_GROUP;
			if (group != nullptr){
				group->SetGroupV0_5(strInput);
				AddNode(group);
			}
		}
	}
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void GC_LIST::SetGroupListV0_4(std::string *strInput){
	//V0.4
	//	[grouplist]
	//		[group]
	//		[group_end]
	//		[group]
	//		[group_end]
	//	[grouplist_end]
	COMMAND_GROUP	*group;
	std::string		strLine,strItem;
	
	Spin_InUse_set();
	while(strInput->length() > 0){
		strLine = Str_LowerCase(Str_Trim(Str_ReadSubItem(strInput,"\n")));
		if (strLine == "[grouplist_end]")
			break;
		if (strLine == "[group]"){
			group = new COMMAND_GROUP;
			if (group != nullptr){
				group->SetGroupV0_4(strInput);
				AddNode(group);
			}
		}
	}
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void GC_LIST::SetGroupListV0_3(std::string *strInput){
	//V0.3
	//Group List: Group
	//{Group1}{Group2}
	
	std::string	strResult;
	COMMAND_GROUP *group;
	
	Spin_InUse_set();
	while(strInput->length() > 0){
		strResult = Str_ReadSubItem(strInput,":");
		group = new COMMAND_GROUP;
		if (group != nullptr){
			group->SetGroupV0_3(&strResult);
			AddNode(group);
		}
	}
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void GC_LIST::SetGroupListV0_2(std::string *strInput){
	//V0.2 Group list
	//Group List: Group
	//{Group1}{Group2}
	
	std::string	strResult;
	COMMAND_GROUP *group;
	
	Spin_InUse_set();
	while(strInput->length() > 0){
		strResult = Str_ReadSubItem(strInput,":");
		group = new COMMAND_GROUP;
		if (group != nullptr){
			group->SetGroupV0_2(&strResult);
			AddNode(group);
		}
	}
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void GC_LIST::ClearTestResult(void){
	Spin_InUse_set();
	RTREE_LChildRChain_Traversal_LINE(COMMAND_GROUP,this,operateNode_t->ClearResult());
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void COMMAND_NODE::CopyCOMMAND_NODE(const COMMAND_NODE *node2,COMMAND_NODE *node1){
	if ((node2 != nullptr) && (node1 != nullptr)){
		node1->Spin_InUse_set();
		((COMMAND_NODE*)node2)->Spin_InUse_set();
		node1->StrCommand = node2->StrCommand;
		node1->blEnableSend = node2->blEnableSend;
		node1->blEnableSendCR = node2->blEnableSendCR;
		node1->blEnableHEX = node2->blEnableHEX;
		node1->StrShowName = node2->StrShowName;
		node1->blSameAsCommand = node2->blSameAsCommand;
		node1->StrTimeout = node2->StrTimeout;
		node1->StrCycle = node2->StrCycle;
		node1->StrContinue = node2->StrContinue;
		node1->StrStop = node2->StrStop;
		node1->StrResend = node2->StrResend;
		node1->StrCatch = node2->StrCatch;
		((COMMAND_NODE*)node2)->Spin_InUse_clr();
		node1->Spin_InUse_clr();
	}
}
//------------------------------------------------------------------------------------------//
void COMMAND_GROUP::CopyCOMMAND_GROUP(const COMMAND_GROUP *group2, COMMAND_GROUP *group1,int32 blClear){
	COMMAND_NODE  *newNode;
	if ((group2 == nullptr) || (group1 == nullptr))
		return;
	if (blClear != 0)
		group1->DestroyAll();

	if (blClear != 0){
		group1->name = group2->name;
		group1->intervalTime = group2->intervalTime;
		group1->blEnableAutoRun = group2->blEnableAutoRun;
		group1->autoRunTimes = group2->autoRunTimes;
	}

	RTREE_LChildRChain_Traversal_LINE_nolock(COMMAND_NODE,((COMMAND_GROUP*)group2),
		newNode = new COMMAND_NODE;
		if (newNode != nullptr){
			COMMAND_NODE::CopyCOMMAND_NODE(operateNode_t,newNode);
			group1->AddNode(newNode);
		}
	);
}
//------------------------------------------------------------------------------------------//
void GC_LIST::CopyCOMMAND_GROUP_ENABLE(const GC_LIST *tGroupList2, GC_LIST *tGroupList1){
	COMMAND_GROUP	*newGroup;
	
	if ((tGroupList2 == nullptr) || (tGroupList1 == nullptr))
		return;
	tGroupList1->DestroyAll();
	tGroupList1->Spin_InUse_set();
	((GC_LIST*)tGroupList2)->Spin_InUse_set();
	RTREE_LChildRChain_Traversal_LINE(COMMAND_GROUP, ((GC_LIST*)tGroupList2),
		operateNode_t->Spin_InUse_set();
		if (operateNode_t->blEnableAutoRun != 0){
			newGroup = new COMMAND_GROUP;
			if (newGroup != nullptr){
				COMMAND_GROUP::CopyCOMMAND_GROUP(operateNode_t,newGroup);
				tGroupList1->AddNode(newGroup);
			}
		}
		operateNode_t->Spin_InUse_clr();
	);
	((GC_LIST*)tGroupList2)->Spin_InUse_clr();
	tGroupList1->Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
