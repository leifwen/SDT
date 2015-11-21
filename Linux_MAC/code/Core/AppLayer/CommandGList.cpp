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
void COMMAND_NODE::Init(COMMAND_NODE *node,G_LOCK_VAILD blLock){
	node->Spin_InUse_set(blLock);

	node->StrCommand = "";
	node->blEnableSend = 1;
	node->blEnableSendCR = 1;
	node->blEnableHEX = 0;
	
	node->StrShowName = "";
	node->blSameAsCommand = 1;
	
	node->StrTimeout = "1";
	node->StrCycle = "1";
	
	node->StrContinue = "";
	node->StrResend = "";
	node->StrStop = "";
	node->StrCatch = "";
	
	node->runTimes = 0;
	node->catchTimes = 0;
	node->timeoutTimes = 0;
	node->blFirstSynchronous = 0;
	node->timeST0.Clear();
	
	node->Spin_InUse_clr(blLock);
}
//------------------------------------------------------------------------------------------//
std::string COMMAND_NODE::CreateNodeStrV0_5(COMMAND_NODE *node){
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
	
	node->Spin_InUse_set();
		strResult  =  "    [node]\n";
		strResult += ("      strCommand = " + Str_Trim(node->StrCommand) + "\n");
		if (node->blEnableSend == 0)
			strResult += ("      blEnableSend = 0\n");
		if (node->blEnableSendCR == 0)
			strResult += ("      blEnableSendCR = 0\n");
		if (node->blEnableHEX != 0)
			strResult += ("      blEnableHEX = 1\n");
		if (node->blSameAsCommand == 0){
			if (Str_Trim(node->StrShowName) != Str_Trim(node->StrCommand))
				strResult += ("      StrShowName = " + Str_Trim(node->StrShowName) + "\n");
		}
		if ((node->StrTimeout.length() > 0) && (node->StrTimeout != "1"))
			strResult += ("      strTimeout = " + Str_Trim(node->StrTimeout) + "\n");
		if ((node->StrCycle.length() > 0) && (node->StrCycle != "1"))
			strResult += ("      strCycle = " + Str_Trim(node->StrCycle) + "\n");
		if (node->StrContinue.length() > 0)
			strResult += ("      strContinue = " + Str_Trim(node->StrContinue) + "\n");
		if (node->StrResend.length() > 0)
			strResult += ("      strResend = " + Str_Trim(node->StrResend) + "\n");
		if (node->StrStop.length() > 0)
			strResult += ("      strStop = " + Str_Trim(node->StrStop) + "\n");
		if (node->StrCatch.length() > 0)
			strResult += ("      strCatch = " + Str_Trim(node->StrCatch) + "\n");
		strResult +=  "    [node_end]\n";
	node->Spin_InUse_clr();
	return(strResult);
}
//------------------------------------------------------------------------------------------//
std::string COMMAND_NODE::CreateNodeStrV0_4(COMMAND_NODE *node){
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
	
	node->Spin_InUse_set();
		strResult  =  "    [node]\n";
		if (node->blEnableSend == 0)
			strResult += ("      blEnableSend = 0\n");
		if (node->blEnableSendCR == 0)
			strResult += ("      blEnableSendCR = 0\n");
		if (node->blEnableHEX != 0)
			strResult += ("      blEnableHEX = 1\n");
		if (node->StrTimeout != "")
			strResult += ("      strTimeout = " + node->StrTimeout + "\n");
		strResult += ("      strCommand = " + node->StrCommand + "\n");
		if (node->StrContinue != "")
			strResult += ("      strContinue = " + node->StrContinue + "\n");
		if (node->StrStop != "")
			strResult += ("      strStop = " + node->StrStop + "\n");
		if (node->StrResend != "")
			strResult += ("      strResend = " + node->StrResend + "\n");
		if (node->StrCycle != "")
			strResult += ("      strCycle = " + node->StrCycle + "\n");
		if (node->StrCatch != "")
			strResult += ("      strCatch = " + node->StrCatch + "\n");
		strResult +=  "    [node_end]\n";
	node->Spin_InUse_clr();
	return(strResult);
}
//------------------------------------------------------------------------------------------//
std::string COMMAND_NODE::CreateNodeStrV0_2(COMMAND_NODE *node){
	//V0.2
	//(blEnableSend,blEnableSendCR,blEnableHEX,StrTimeout,StrCommand,StrContinue,StrStop,StrResend,StrCycle,StrCatch)
	std::string		strResult,strTemp;
	
	node->Spin_InUse_set();
		strResult = '(';
		
		strTemp = Str_IntToString(node->blEnableSend);
		strResult += Str_ASCIIToHEX(strTemp,G_ESCAPE_OFF);
		strResult += ",";
		
		strTemp = Str_IntToString(node->blEnableSendCR);
		strResult += Str_ASCIIToHEX(strTemp,G_ESCAPE_OFF);
		strResult += ",";
		
		strTemp = Str_IntToString(node->blEnableHEX);
		strResult += Str_ASCIIToHEX(strTemp,G_ESCAPE_OFF);
		strResult += ",";
		
		strResult += Str_ASCIIToHEX(node->StrTimeout,G_ESCAPE_OFF);
		strResult += ",";
		
		strResult += Str_ASCIIToHEX(node->StrCommand,G_ESCAPE_OFF);
		strResult += ",";
		
		strResult += Str_ASCIIToHEX(node->StrContinue,G_ESCAPE_OFF);
		strResult += ",";
		
		strResult += Str_ASCIIToHEX(node->StrStop,G_ESCAPE_OFF);
		strResult += ",";
		
		strResult += Str_ASCIIToHEX(node->StrResend,G_ESCAPE_OFF);
		strResult += ",";
		
		strResult += Str_ASCIIToHEX(node->StrCycle,G_ESCAPE_OFF);
		strResult += ",";
		
		strResult += Str_ASCIIToHEX(node->StrCatch,G_ESCAPE_OFF);
		strResult += ')';
	node->Spin_InUse_clr();
	return(strResult);
}
//------------------------------------------------------------------------------------------//
void COMMAND_NODE::SetNodeV0_5(COMMAND_NODE *node,std::string *strInput){
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
	
	node->Spin_InUse_set();
		Init(node,G_LOCK_OFF);
		while(strInput->length() > 0){
			strLine = Str_Trim(Str_ReadSubItem(strInput,"\n"));
			if (Str_LowerCase(strLine) == "[node_end]")
				break;
			strItem = Str_LowerCase(Str_Trim(Str_ReadSubItem(&strLine,"=")));
			strLine = Str_Trim(strLine);
			if (strItem == "strcommand"){
				node->StrCommand = strLine;
			}
			else if (strItem == "blenablesend"){
				node->blEnableSend = (atoi(strLine.c_str()) != 0);
			}
			else if (strItem == "blenablesendcr"){
				node->blEnableSendCR = (atoi(strLine.c_str()) != 0);
			}
			else if (strItem == "blenablehex"){
				node->blEnableHEX = (atoi(strLine.c_str()) != 0);
			}
			else if (strItem == "strshowname"){
				node->StrShowName = strLine;
				node->blSameAsCommand = 0;
			}
			else if (strItem == "strtimeout"){
				node->StrTimeout = strLine;
			}
			else if (strItem == "strcycle"){
				node->StrCycle = strLine;
			}
			else if (strItem == "strcontinue"){
				node->StrContinue = strLine;
			}
			else if (strItem == "strresend"){
				node->StrResend = strLine;
			}
			else if (strItem == "strstop"){
				node->StrStop = strLine;
			}
			else if (strItem == "strcatch"){
				node->StrCatch = strLine;
			}
		}
	node->Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void COMMAND_NODE::SetNodeV0_4(COMMAND_NODE *node,std::string *strInput){
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
	node->Spin_InUse_set();
		Init(node,G_LOCK_OFF);
		while(strInput->length() > 0){
			strLine = Str_Trim(Str_ReadSubItem(strInput,"\n"));
			if (Str_LowerCase(strLine) == "[node_end]")
				break;
			strItem = Str_LowerCase(Str_Trim(Str_ReadSubItem(&strLine,"=")));
			strLine = Str_Trim(strLine);
			if (strItem == "strcommand"){
				node->StrCommand = strLine;
			}
			else if (strItem == "blenablesend"){
				node->blEnableSend = (atoi(strLine.c_str()) != 0);
			}
			else if (strItem == "blenablesendcr"){
				node->blEnableSendCR = (atoi(strLine.c_str()) != 0);
			}
			else if (strItem == "blenablehex"){
				node->blEnableHEX = (atoi(strLine.c_str()) != 0);
			}
			else if (strItem == "strtimeout"){
				node->StrTimeout = strLine;
			}
			else if (strItem == "strcycle"){
				node->StrCycle = strLine;
			}
			else if (strItem == "strcontinue"){
				node->StrContinue = strLine;
			}
			else if (strItem == "strresend"){
				node->StrResend = strLine;
			}
			else if (strItem == "strstop"){
				node->StrStop = strLine;
			}
			else if (strItem == "strcatch"){
				node->StrCatch = strLine;
			}
		}
	node->Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void COMMAND_NODE::SetNodeV0_3(COMMAND_NODE *node,std::string *strInput){
	//V0.3
	//update:StrTimeout
	//:(blEnableSend,blEnableSendCR,blEnableHEX,StrTimeout,StrCommand,StrContinue,StrStop,StrResend,StrCycle,StrCatch)
	node->Spin_InUse_set();
		Init(node,G_LOCK_OFF);
		node->blEnableSend = atoi(Str_HEXToASCII(Str_ReadSubItem(strInput,",")).c_str());
		node->blEnableSendCR = atoi(Str_HEXToASCII(Str_ReadSubItem(strInput,",")).c_str());
		node->blEnableHEX = atoi(Str_HEXToASCII(Str_ReadSubItem(strInput,",")).c_str());
		
		node->StrTimeout = Str_HEXToASCII(Str_ReadSubItem(strInput,","));
		node->StrCommand = Str_HEXToASCII(Str_ReadSubItem(strInput,","));
		node->StrContinue = Str_HEXToASCII(Str_ReadSubItem(strInput,","));
		node->StrStop = Str_HEXToASCII(Str_ReadSubItem(strInput,","));
		node->StrResend = Str_HEXToASCII(Str_ReadSubItem(strInput,","));
		node->StrCycle = Str_HEXToASCII(Str_ReadSubItem(strInput,","));
		node->StrCatch = Str_HEXToASCII(Str_ReadSubItem(strInput,","));
	node->Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void COMMAND_NODE::SetNodeV0_2(COMMAND_NODE *node,std::string *strInput){
	//V0.2
	//:(blEnableSend,blEnableSendCR,blEnableHEX,StrTimeout,StrCommand,StrContinue,StrStop,StrResend,StrCycle,StrCatch)
	double		timeout;
	node->Spin_InUse_set();
		Init(node,G_LOCK_OFF);
		node->blEnableSend = atoi(Str_HEXToASCII(Str_ReadSubItem(strInput,",")).c_str());
		node->blEnableSendCR = atoi(Str_HEXToASCII(Str_ReadSubItem(strInput,",")).c_str());
		node->blEnableHEX = atoi(Str_HEXToASCII(Str_ReadSubItem(strInput,",")).c_str());
		
		node->StrTimeout = Str_HEXToASCII(Str_ReadSubItem(strInput,","));
		timeout = atoi(node->StrTimeout.c_str()) / 1000.0;
		
		node->StrTimeout = Str_FloatToString(timeout);
		node->StrCommand = Str_HEXToASCII(Str_ReadSubItem(strInput,","));
		node->StrContinue = Str_HEXToASCII(Str_ReadSubItem(strInput,","));
		node->StrStop = Str_HEXToASCII(Str_ReadSubItem(strInput,","));
		node->StrResend = Str_HEXToASCII(Str_ReadSubItem(strInput,","));
		node->StrCycle = Str_HEXToASCII(Str_ReadSubItem(strInput,","));
		node->StrCatch = Str_HEXToASCII(Str_ReadSubItem(strInput,","));
	node->Spin_InUse_clr();
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
std::string COMMAND_GROUP::CreateGroupStrV0_5(COMMAND_GROUP *group){
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
	
	group->Spin_InUse_set();
	strResult  =  "  [group]\n";
	strResult += ("    groupName = " + group->name + "\n");
	strResult += ("    intervalTime = " + Str_IntToString(group->intervalTime) + "\n");
	
	RTREE_LChildRChain_Traversal_LINE_nolock(COMMAND_NODE,group,strResult += COMMAND_NODE::CreateNodeStrV0_5(operateNode_t));
	group->Spin_InUse_clr();
	
	strResult += "  [group_end]\n";
	return(strResult);
}
//------------------------------------------------------------------------------------------//
std::string COMMAND_GROUP::CreateGroupStrV0_4(COMMAND_GROUP *group){
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
	group->Spin_InUse_set();
	strResult += ("    groupName = " + group->name + "\n");
	strResult += ("    intervalTime = " + Str_IntToString(group->intervalTime) + "\n");
	
	RTREE_LChildRChain_Traversal_LINE_nolock(COMMAND_NODE,group,strResult += COMMAND_NODE::CreateNodeStrV0_4(operateNode_t));
	group->Spin_InUse_clr();
	
	strResult += "  [group_end]\n";
	return(strResult);
}
//------------------------------------------------------------------------------------------//
std::string COMMAND_GROUP::CreateGroupStrV0_2(COMMAND_GROUP *group){
	//V0.2
	//Group:,GroupName,intervalTime,count(node),node
	//{(GroupName,intervalTime)(node1)(node2)}
	std::string	strResult,strTemp;
	
	group->Spin_InUse_set();
	strResult = "{(";
	strResult += Str_ASCIIToHEX(group->name,G_ESCAPE_OFF);
	strResult	+= ",";
	
	strTemp = Str_IntToString(group->intervalTime);
	strResult += Str_ASCIIToHEX(strTemp,G_ESCAPE_OFF);
	strResult	+= ")";
	
	RTREE_LChildRChain_Traversal_LINE_nolock(COMMAND_NODE,group,strResult += COMMAND_NODE::CreateNodeStrV0_2(operateNode_t));
	group->Spin_InUse_clr();
	
	strResult	+= '}';
	return(strResult);
}
//------------------------------------------------------------------------------------------//
void COMMAND_GROUP::SetGroupV0_5(COMMAND_GROUP *group,std::string *strInput){
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
	
	group->Spin_InUse_set();
	group->name = "";
	group->intervalTime = 200;
	while(strInput->length() > 0){
		strLine = Str_Trim(Str_ReadSubItem(strInput,"\n"));
		if (Str_LowerCase(strLine) == "[group_end]")
			break;
		if (Str_LowerCase(strLine) == "[node]"){
			node = new COMMAND_NODE;
			if (node != nullptr){
				COMMAND_NODE::SetNodeV0_5(node,strInput);
				group->AddNode(node);
			}
			continue;
		}
		strItem = Str_LowerCase(Str_Trim(Str_ReadSubItem(&strLine,"=")));
		strLine = Str_Trim(strLine);
		if (strItem == "groupname"){
			group->name = strLine;
		}
		else if (strItem == "intervaltime"){
			group->intervalTime = atoi(strLine.c_str());
		}
	}
	group->Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void COMMAND_GROUP::SetGroupV0_4(COMMAND_GROUP *group,std::string *strInput){
	//V0.4
	//	[group]
	//		groupName =
	//		intervalTime =
	//		[node]
	//		[node_end]
	//		[node]
	//		[node_end]
	//	[group_end]
	
	COMMAND_NODE *node;
	std::string		strLine,strItem;
	
	group->Spin_InUse_set();
	group->name = "";
	group->intervalTime = 200;
	while(strInput->length() > 0){
		strLine = Str_Trim(Str_ReadSubItem(strInput,"\n"));
		if (Str_LowerCase(strLine) == "[group_end]")
			break;
		if (Str_LowerCase(strLine) == "[node]"){
			node = new COMMAND_NODE;
			if (node != nullptr){
				COMMAND_NODE::SetNodeV0_4(node,strInput);
				group->AddNode(node);
			}
			continue;
		}
		strItem = Str_LowerCase(Str_Trim(Str_ReadSubItem(&strLine,"=")));
		strLine = Str_Trim(strLine);
		if (strItem == "groupname"){
			group->name = strLine;
		}
		else if (strItem == "intervaltime"){
			group->intervalTime = atoi(strLine.c_str());
		}
	}
	group->Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void COMMAND_GROUP::SetGroupV0_3(COMMAND_GROUP *group,std::string *strInput){
	/*
	 Group:,GroupName,intervalTime,count(Node),action
	 {(GroupName,intervalTime)(Node1)(Node2)}
	 */
	std::string	strResult;
	COMMAND_NODE *node;
	
	group->Spin_InUse_set();
	strResult = Str_ReadSubItem(strInput,"|");
	group->name = Str_HEXToASCII(Str_ReadSubItem(&strResult,","));
	
	group->intervalTime = atoi(Str_HEXToASCII(Str_ReadSubItem(&strResult,",")).c_str());
	
	while(strInput->length() > 0){
		strResult = Str_ReadSubItem(strInput,"|");
		node = new COMMAND_NODE;
		if (node != nullptr){
			COMMAND_NODE::SetNodeV0_3(node,&strResult);
			group->AddNode(node);
		}
	}
	group->Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void COMMAND_GROUP::SetGroupV0_2(COMMAND_GROUP *group,std::string *strInput){
	/*
	 Group:,GroupName,intervalTime,count(Node),action
	 {(GroupName,intervalTime)(Node1)(Node2)}
	 */
	std::string		strResult;
	COMMAND_NODE	*node;
	
	group->Spin_InUse_set();
	strResult = Str_ReadSubItem(strInput,"|");
	group->name = Str_HEXToASCII(Str_ReadSubItem(&strResult,","));
	
	group->intervalTime = atoi(Str_HEXToASCII(Str_ReadSubItem(&strResult,",")).c_str());
	
	while(strInput->length() > 0){
		strResult = Str_ReadSubItem(strInput,"|");
		node = new COMMAND_NODE;
		if (node != nullptr){
			COMMAND_NODE::SetNodeV0_2(node,&strResult);
			group->AddNode(node);
		}
	}
	group->Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void COMMAND_GROUP::ClearResult(COMMAND_GROUP *group){
	RTREE_LChildRChain_Traversal_LINE_nolock(COMMAND_NODE,group,
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
	RTREE_LChildRChain_Traversal_LINE(COMMAND_GROUP,this,strResult += COMMAND_GROUP::CreateGroupStrV0_5(operateNode_t));
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
	RTREE_LChildRChain_Traversal_LINE(COMMAND_GROUP,this,strResult += COMMAND_GROUP::CreateGroupStrV0_4(operateNode_t));
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
	RTREE_LChildRChain_Traversal_LINE(COMMAND_GROUP,this,strResult += COMMAND_GROUP::CreateGroupStrV0_2(operateNode_t));
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
				COMMAND_GROUP::SetGroupV0_5(group,strInput);
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
				COMMAND_GROUP::SetGroupV0_4(group,strInput);
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
			COMMAND_GROUP::SetGroupV0_3(group,&strResult);
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
			COMMAND_GROUP::SetGroupV0_2(group,&strResult);
			AddNode(group);
		}
	}
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void GC_LIST::ClearTestResult(void){
	Spin_InUse_set();
	RTREE_LChildRChain_Traversal_LINE(COMMAND_GROUP,this,COMMAND_GROUP::ClearResult(operateNode_t));
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
