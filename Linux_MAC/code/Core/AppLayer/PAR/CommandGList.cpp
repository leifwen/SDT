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

#ifdef CommandGListH
//------------------------------------------------------------------------------------------//
TREE_NODE	COMMAND_GROUP::sgSpareOwner((TRASH*)(nullptr));
//------------------------------------------------------------------------------------------//
void COMMAND_NODE::Init(void){
	StrCommand = "";
	blEnableSend = 1;
	cmdTail = CMD_R;
	
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
}
//------------------------------------------------------------------------------------------//
STDSTR &COMMAND_NODE::CreateNodeStrV0_6(STDSTR *retStr){
	//V0.6
	//	[node]
	//		StrCommand =
	//		blEnableSend =
	//		cmdTail =
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
	
	Spin_InUse_set();
	*retStr += "    [node]\n";
	*retStr += ("      strCommand = " + Str_Trim(StrCommand) + "\n");
	if (blEnableSend == 0)
		*retStr += ("      blEnableSend = 0\n");
	if (cmdTail != CMD_R)
		*retStr += ("      cmdTail = " + Str_ToString((uint32)cmdTail) + "\n");
	if (blSameAsCommand == 0){
		if (Str_Trim(StrShowName) != Str_Trim(StrCommand))
			*retStr += ("      StrShowName = " + Str_Trim(StrShowName) + "\n");
	}
	if ((StrTimeout.length() > 0) && (StrTimeout != "1"))
		*retStr += ("      strTimeout = " + Str_Trim(StrTimeout) + "\n");
	if ((StrCycle.length() > 0) && (StrCycle != "1"))
		*retStr += ("      strCycle = " + Str_Trim(StrCycle) + "\n");
	if (StrContinue.length() > 0)
		*retStr += ("      strContinue = " + Str_Trim(StrContinue) + "\n");
	if (StrResend.length() > 0)
		*retStr += ("      strResend = " + Str_Trim(StrResend) + "\n");
	if (StrStop.length() > 0)
		*retStr += ("      strStop = " + Str_Trim(StrStop) + "\n");
	if (StrCatch.length() > 0)
		*retStr += ("      strCatch = " + Str_Trim(StrCatch) + "\n");
	*retStr +=  "    [node_end]\n";
	Spin_InUse_clr();
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
STDSTR &COMMAND_NODE::CreateNodeStrV0_5(STDSTR *retStr){
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
	
	Spin_InUse_set();
		*retStr += "    [node]\n";
		*retStr += ("      strCommand = " + Str_Trim(StrCommand) + "\n");
		if (blEnableSend == 0)
			*retStr += ("      blEnableSend = 0\n");
		if (cmdTail == 0)
			*retStr += ("      blEnableSendCR = 0\n");
		//if (blEnableHEX != 0)
		//	*retStr += ("      blEnableHEX = 1\n");
		if (blSameAsCommand == 0){
			if (Str_Trim(StrShowName) != Str_Trim(StrCommand))
				*retStr += ("      StrShowName = " + Str_Trim(StrShowName) + "\n");
		}
		if ((StrTimeout.length() > 0) && (StrTimeout != "1"))
			*retStr += ("      strTimeout = " + Str_Trim(StrTimeout) + "\n");
		if ((StrCycle.length() > 0) && (StrCycle != "1"))
			*retStr += ("      strCycle = " + Str_Trim(StrCycle) + "\n");
		if (StrContinue.length() > 0)
			*retStr += ("      strContinue = " + Str_Trim(StrContinue) + "\n");
		if (StrResend.length() > 0)
			*retStr += ("      strResend = " + Str_Trim(StrResend) + "\n");
		if (StrStop.length() > 0)
			*retStr += ("      strStop = " + Str_Trim(StrStop) + "\n");
		if (StrCatch.length() > 0)
			*retStr += ("      strCatch = " + Str_Trim(StrCatch) + "\n");
		*retStr +=  "    [node_end]\n";
	Spin_InUse_clr();
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
STDSTR &COMMAND_NODE::CreateNodeStrV0_4(STDSTR *retStr){
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
	
	Spin_InUse_set();
		*retStr += "    [node]\n";
		if (blEnableSend == 0)
			*retStr += ("      blEnableSend = 0\n");
		if (cmdTail == 0)
			*retStr += ("      blEnableSendCR = 0\n");
		//if (blEnableHEX != 0)
		//	*retStr += ("      blEnableHEX = 1\n");
		if (StrTimeout != "")
			*retStr += ("      strTimeout = " + StrTimeout + "\n");
		*retStr += ("      strCommand = " + StrCommand + "\n");
		if (StrContinue != "")
			*retStr += ("      strContinue = " + StrContinue + "\n");
		if (StrStop != "")
			*retStr += ("      strStop = " + StrStop + "\n");
		if (StrResend != "")
			*retStr += ("      strResend = " + StrResend + "\n");
		if (StrCycle != "")
			*retStr += ("      strCycle = " + StrCycle + "\n");
		if (StrCatch != "")
			*retStr += ("      strCatch = " + StrCatch + "\n");
		*retStr +=  "    [node_end]\n";
	Spin_InUse_clr();
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
STDSTR &COMMAND_NODE::CreateNodeStrV0_2(STDSTR *retStr){
	//V0.2
	//(blEnableSend,blEnableSendCR,blEnableHEX,StrTimeout,StrCommand,StrContinue,StrStop,StrResend,StrCycle,StrCatch)
	
	Spin_InUse_set();
		*retStr += '(';
	
		*retStr += Str_ASCIIToHEX(Str_ToString(blEnableSend),G_ESCAPE_OFF);
		*retStr += ",";
	
		*retStr += Str_ASCIIToHEX(Str_ToString(cmdTail),G_ESCAPE_OFF);
		*retStr += ",";
	
		*retStr += Str_ASCIIToHEX(Str_ToString(G_HEX),G_ESCAPE_OFF);
		*retStr += ",";
		
		*retStr += Str_ASCIIToHEX(StrTimeout,G_ESCAPE_OFF);
		*retStr += ",";
		
		*retStr += Str_ASCIIToHEX(StrCommand,G_ESCAPE_OFF);
		*retStr += ",";
		
		*retStr += Str_ASCIIToHEX(StrContinue,G_ESCAPE_OFF);
		*retStr += ",";
		
		*retStr += Str_ASCIIToHEX(StrStop,G_ESCAPE_OFF);
		*retStr += ",";
		
		*retStr += Str_ASCIIToHEX(StrResend,G_ESCAPE_OFF);
		*retStr += ",";
		
		*retStr += Str_ASCIIToHEX(StrCycle,G_ESCAPE_OFF);
		*retStr += ",";
		
		*retStr += Str_ASCIIToHEX(StrCatch,G_ESCAPE_OFF);
		*retStr += ')';
	Spin_InUse_clr();
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
void COMMAND_NODE::SetNodeV0_6(STDSTR *strIn){
	//V0.6
	//	[node]
	//		StrCommand =
	//		blEnableSend =
	//		cmdTail =
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
	STDSTR		strLine,strItem;
	
	Spin_InUse_set();
	Init();
	while(strIn->length() > 0){
		strLine = Str_Trim(Str_ReadSubItem(strIn,"\n"));
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
		else if (strItem == "cmdtail"){
			cmdTail = (CMD_TAIL)(atoi(strLine.c_str()));
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
void COMMAND_NODE::SetNodeV0_5(STDSTR *strIn){
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
	STDSTR		strLine,strItem;
	int32		blHex;
	
	blHex = 0;
	Spin_InUse_set();
		Init();
		while(strIn->length() > 0){
			strLine = Str_Trim(Str_ReadSubItem(strIn,"\n"));
			if (Str_LowerCase(strLine) == "[node_end]"){
				if (blHex != 0)
					StrCommand = "'hex:" + StrCommand;
				break;
			}
			strItem = Str_LowerCase(Str_Trim(Str_ReadSubItem(&strLine,"=")));
			strLine = Str_Trim(strLine);
			if (strItem == "strcommand"){
				StrCommand = strLine;
			}
			else if (strItem == "blenablesend"){
				blEnableSend = (atoi(strLine.c_str()) != 0);
			}
			else if (strItem == "blenablesendcr"){
				cmdTail = (CMD_TAIL)(atoi(strLine.c_str()) != 0);
			}
			else if (strItem == "blenablehex"){
				blHex = (atoi(strLine.c_str()) != 0);
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
void COMMAND_NODE::SetNodeV0_4(STDSTR *strIn){
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
	STDSTR		strLine,strItem;
	int32		blHex;
	
	blHex = 0;
	Spin_InUse_set();
		Init();
		while(strIn->length() > 0){
			strLine = Str_Trim(Str_ReadSubItem(strIn,"\n"));
			if (Str_LowerCase(strLine) == "[node_end]"){
				if (blHex != 0)
					StrCommand = "'hex:" + StrCommand;
				break;
			}
			strItem = Str_LowerCase(Str_Trim(Str_ReadSubItem(&strLine,"=")));
			strLine = Str_Trim(strLine);
			if (strItem == "strcommand"){
				StrCommand = strLine;
			}
			else if (strItem == "blenablesend"){
				blEnableSend = (atoi(strLine.c_str()) != 0);
			}
			else if (strItem == "blenablesendcr"){
				cmdTail = (CMD_TAIL)(atoi(strLine.c_str()) != 0);
			}
			else if (strItem == "blenablehex"){
				blHex = (atoi(strLine.c_str()) != 0);
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
void COMMAND_NODE::SetNodeV0_3(STDSTR *strIn){
	//V0.3
	//update:StrTimeout
	//:(blEnableSend,blEnableSendCR,blEnableHEX,StrTimeout,StrCommand,StrContinue,StrStop,StrResend,StrCycle,StrCatch)
	int32		blHex;
	
	Spin_InUse_set();
		Init();
		blEnableSend = atoi(Str_HEXToASCII(Str_ReadSubItem(strIn,",")).c_str());
		cmdTail = (CMD_TAIL)atoi(Str_HEXToASCII(Str_ReadSubItem(strIn,",")).c_str());
		blHex = atoi(Str_HEXToASCII(Str_ReadSubItem(strIn,",")).c_str());
		
		StrTimeout = Str_HEXToASCII(Str_ReadSubItem(strIn,","));
		StrCommand = (blHex == 0)?"":"'hex:" + Str_HEXToASCII(Str_ReadSubItem(strIn,","));
		StrContinue = Str_HEXToASCII(Str_ReadSubItem(strIn,","));
		StrStop = Str_HEXToASCII(Str_ReadSubItem(strIn,","));
		StrResend = Str_HEXToASCII(Str_ReadSubItem(strIn,","));
		StrCycle = Str_HEXToASCII(Str_ReadSubItem(strIn,","));
		StrCatch = Str_HEXToASCII(Str_ReadSubItem(strIn,","));
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void COMMAND_NODE::SetNodeV0_2(STDSTR *strIn){
	//V0.2
	//:(blEnableSend,blEnableSendCR,blEnableHEX,StrTimeout,StrCommand,StrContinue,StrStop,StrResend,StrCycle,StrCatch)
	double		timeout;
	int32		blHex;

	Spin_InUse_set();
		Init();
		blEnableSend = atoi(Str_HEXToASCII(Str_ReadSubItem(strIn,",")).c_str());
		cmdTail = (CMD_TAIL)atoi(Str_HEXToASCII(Str_ReadSubItem(strIn,",")).c_str());
		blHex = atoi(Str_HEXToASCII(Str_ReadSubItem(strIn,",")).c_str());
		
		StrTimeout = Str_HEXToASCII(Str_ReadSubItem(strIn,","));
		timeout = atoi(StrTimeout.c_str()) / 1000.0;
		
		StrTimeout = Str_FloatToString(timeout);
		StrCommand = (blHex == 0)?"":"'hex:" + Str_HEXToASCII(Str_ReadSubItem(strIn,","));
		StrContinue = Str_HEXToASCII(Str_ReadSubItem(strIn,","));
		StrStop = Str_HEXToASCII(Str_ReadSubItem(strIn,","));
		StrResend = Str_HEXToASCII(Str_ReadSubItem(strIn,","));
		StrCycle = Str_HEXToASCII(Str_ReadSubItem(strIn,","));
		StrCatch = Str_HEXToASCII(Str_ReadSubItem(strIn,","));
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
STDSTR&	COMMAND_NODE::GetTitle(STDSTR *retStr,uint32 flag){
	*retStr  = "  cID    ";
	*retStr += (B_ChkFLAG32(flag,CL_showCycle) == 0)?"":"  cycle";
	*retStr += (B_ChkFLAG32(flag,CL_showTimeout) == 0)?"":"  timeout";
	*retStr += "  command";
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
STDSTR&	COMMAND_NODE::Compose(STDSTR *retStr,uint32 flag){
	STDSTR	strTemp;
	Spin_InUse_set();
	*retStr = Str_ToString(GetdRNodeID(this));
	AddSpaceInFront(retStr,3);
	*retStr = ((blEnableSend == 0)?" ":"*") + *retStr;
	switch (cmdTail) {
		case CMD_R: *retStr += ".  R ";break;
		case CMD_N: *retStr += ".   N";break;
		case CMD_RN:*retStr += ".  RN";break;
		case CMD_NONE:;
		default:
			*retStr += ".    ";break;
	}
	if (B_ChkFLAG32(flag,CL_showCycle) != 0){
		strTemp = StrCycle;
		AddSpaceInFront(&strTemp,7);
		*retStr += strTemp;
	}
	if (B_ChkFLAG32(flag,CL_showTimeout) != 0){
		strTemp = StrTimeout + "s";
		AddSpaceInFront(&strTemp,9);
		*retStr += strTemp;
	}
	*retStr += "  ";
	*retStr += StrCommand;
	Spin_InUse_clr();
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
STDSTR&	COMMAND_NODE::ComposeDetail(STDSTR *retStr){
	Spin_InUse_set();
	*retStr  = "     Continue : " + StrContinue + "\n";
	*retStr += "     Resend   : " + StrResend + "\n";
	*retStr += "     CStop    : " + StrStop + "\n";
	*retStr += "     Catch    : " + StrCatch + "\n";
	Spin_InUse_clr();
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void COMMAND_GROUP::Init(void){
	name = "";
	intervalTime = 200;
	
	blEnableAutoRun = 0;
	autoRunTimes = 1;
}
//------------------------------------------------------------------------------------------//
COMMAND_NODE *COMMAND_GROUP::GetNewNode(void){
	COMMAND_NODE *nNode;
	if (GetcgTrash(&sgSpareOwner) == nullptr)
		return((COMMAND_NODE*)CreateNode());
	nNode = (COMMAND_NODE*)GetcgDown(GetcgTrash(&sgSpareOwner));
	if (nNode == nullptr)
		return((COMMAND_NODE*)CreateNode());
	return((COMMAND_NODE*)Remove(GetcgTail(nNode)));
}
//------------------------------------------------------------------------------------------//
STDSTR &COMMAND_GROUP::CreateGroupStrV0_6(STDSTR *retStr){
	//V0.5
	//	[group]
	//		groupName =
	//		intervalTime =
	//		[node]
	//		[node_end]
	//		[node]
	//		[node_end]
	//	[group_end]
	
	Spin_InUse_set();
	*retStr +=  "  [group]\n";
	*retStr += ("    groupName = " + name + "\n");
	*retStr += ("    intervalTime = " + Str_ToString(intervalTime) + "\n");
	Spin_InUse_clr();
	
	TREE_LChildRChain_Traversal_LINE(COMMAND_NODE,this,operateNode_t->CreateNodeStrV0_6(retStr));
	
	*retStr += "  [group_end]\n";
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
STDSTR &COMMAND_GROUP::CreateGroupStrV0_5(STDSTR *retStr){
	//V0.5
	//	[group]
	//		groupName =
	//		intervalTime =
	//		[node]
	//		[node_end]
	//		[node]
	//		[node_end]
	//	[group_end]
	
	Spin_InUse_set();
	*retStr +=  "  [group]\n";
	*retStr += ("    groupName = " + name + "\n");
	*retStr += ("    intervalTime = " + Str_ToString(intervalTime) + "\n");
	Spin_InUse_clr();
	
	TREE_LChildRChain_Traversal_LINE(COMMAND_NODE,this,operateNode_t->CreateNodeStrV0_5(retStr));
	
	*retStr += "  [group_end]\n";
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
STDSTR &COMMAND_GROUP::CreateGroupStrV0_4(STDSTR *retStr){
	//V0.4
	//	[group]
	//		groupName =
	//		intervalTime =
	//		[node]
	//		[node_end]
	//		[node]
	//		[node_end]
	//	[group_end]
	
	*retStr +=  "  [group]\n";
	Spin_InUse_set();
	*retStr += ("    groupName = " + name + "\n");
	*retStr += ("    intervalTime = " + Str_ToString(intervalTime) + "\n");
	Spin_InUse_clr();
	
	TREE_LChildRChain_Traversal_LINE_nolock(COMMAND_NODE,this,operateNode_t->CreateNodeStrV0_4(retStr));
	
	*retStr += "  [group_end]\n";
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
STDSTR &COMMAND_GROUP::CreateGroupStrV0_2(STDSTR *retStr){
	//V0.2
	//Group:,GroupName,intervalTime,count(node),node
	//{(GroupName,intervalTime)(node1)(node2)}
	STDSTR	strResult;
	
	Spin_InUse_set();
	*retStr += "{(";
	*retStr += Str_ASCIIToHEX(name,G_ESCAPE_OFF);
	*retStr += ",";
	
	*retStr += Str_ASCIIToHEX(Str_ToString(intervalTime),G_ESCAPE_OFF);
	*retStr += ")";
	Spin_InUse_clr();
	
	TREE_LChildRChain_Traversal_LINE_nolock(COMMAND_NODE,this,operateNode_t->CreateNodeStrV0_2(retStr));
	
	*retStr += '}';
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
void COMMAND_GROUP::SetGroupV0_6(STDSTR *strIn){
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
	STDSTR		strLine,strItem;
	
	Spin_InUse_set();
	name = "";
	intervalTime = 200;
	while(strIn->length() > 0){
		strLine = Str_Trim(Str_ReadSubItem(strIn,"\n"));
		if (Str_LowerCase(strLine) == "[group_end]")
			break;
		if (Str_LowerCase(strLine) == "[node]"){
			node = GetNewNode();
			if (node != nullptr){
				node->SetNodeV0_6(strIn);
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
void COMMAND_GROUP::SetGroupV0_5(STDSTR *strIn){
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
	STDSTR		strLine,strItem;
	
	Spin_InUse_set();
	name = "";
	intervalTime = 200;
	while(strIn->length() > 0){
		strLine = Str_Trim(Str_ReadSubItem(strIn,"\n"));
		if (Str_LowerCase(strLine) == "[group_end]")
			break;
		if (Str_LowerCase(strLine) == "[node]"){
			node = GetNewNode();
			if (node != nullptr){
				node->SetNodeV0_5(strIn);
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
void COMMAND_GROUP::SetGroupV0_4(STDSTR *strIn){
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
	STDSTR		strLine,strItem;
	
	Spin_InUse_set();
	name = "";
	intervalTime = 200;
	while(strIn->length() > 0){
		strLine = Str_Trim(Str_ReadSubItem(strIn,"\n"));
		if (Str_LowerCase(strLine) == "[group_end]")
			break;
		if (Str_LowerCase(strLine) == "[node]"){
			node = GetNewNode();
			if (node != nullptr){
				node->SetNodeV0_4(strIn);
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
void COMMAND_GROUP::SetGroupV0_3(STDSTR *strIn){
	/*
	 Group:,GroupName,intervalTime,count(Node),action
	 {(GroupName,intervalTime)(Node1)(Node2)}
	 */
	STDSTR	strResult;
	COMMAND_NODE *node;
	
	Spin_InUse_set();
	strResult = Str_ReadSubItem(strIn,"|");
	name = Str_HEXToASCII(Str_ReadSubItem(&strResult,","));
	
	intervalTime = atoi(Str_HEXToASCII(Str_ReadSubItem(&strResult,",")).c_str());
	
	while(strIn->length() > 0){
		strResult = Str_ReadSubItem(strIn,"|");
		node = GetNewNode();
		if (node != nullptr){
			node->SetNodeV0_3(&strResult);
			AddNode(node);
		}
	}
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void COMMAND_GROUP::SetGroupV0_2(STDSTR *strIn){
	/*
	 Group:,GroupName,intervalTime,count(Node),action
	 {(GroupName,intervalTime)(Node1)(Node2)}
	 */
	STDSTR		strResult;
	COMMAND_NODE	*node;
	
	Spin_InUse_set();
	strResult = Str_ReadSubItem(strIn,"|");
	name = Str_HEXToASCII(Str_ReadSubItem(&strResult,","));
	
	intervalTime = atoi(Str_HEXToASCII(Str_ReadSubItem(&strResult,",")).c_str());
	
	while(strIn->length() > 0){
		strResult = Str_ReadSubItem(strIn,"|");
		node = GetNewNode();
		if (node != nullptr){
			node->SetNodeV0_2(&strResult);
			AddNode(node);
		}
	}
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void COMMAND_GROUP::ClearResult(void){
	TREE_LChildRChain_Traversal_LINE(COMMAND_NODE,this,
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
STDSTR&	COMMAND_GROUP::GetTitle(STDSTR *retStr,uint32 flag){
	*retStr  = "  gID";
	*retStr += (B_ChkFLAG32(flag,GL_showCycle) == 0)?"":"  cycle";
	*retStr += (B_ChkFLAG32(flag,GL_showInterval) == 0)?"":"  interval";
	*retStr += "  group name";
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
STDSTR&	COMMAND_GROUP::Compose(STDSTR *retStr,uint32 flag){
	STDSTR	strTemp;
	Spin_InUse_set();
	*retStr = Str_ToString(GetdRNodeID(this));
	AddSpaceInFront(retStr,3);
	*retStr = ((blEnableAutoRun == 0)?" ":"*") + *retStr;
	*retStr += '.';
	
	if (B_ChkFLAG32(flag,GL_showCycle) != 0){
		strTemp = Str_ToString(autoRunTimes);
		AddSpaceInFront(&strTemp,7);
		*retStr += strTemp;
	}
	if (B_ChkFLAG32(flag,GL_showInterval) != 0){
		strTemp = Str_ToString(intervalTime) + "ms";
		AddSpaceInFront(&strTemp,10);
		*retStr += strTemp;
	}
	*retStr += "  ";
	*retStr += name;
	Spin_InUse_clr();
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void GC_LIST::Empty(void){
	TREE_LChildRChain_Traversal_LINE(COMMAND_GROUP,this,operateNode_t->Empty());
	CleanChild(this);
};
//------------------------------------------------------------------------------------------//
STDSTR &GC_LIST::CreateGroupListStrV0_6(STDSTR *retStr){
	//V0.5
	//	[grouplist]
	//		[group]
	//		[group_end]
	//		[group]
	//		[group_end]
	//	[grouplist_end]
	
	*retStr += "[grouplist]\n";
	
	Spin_InUse_set();
	TREE_LChildRChain_Traversal_LINE(COMMAND_GROUP,this,operateNode_t->CreateGroupStrV0_6(retStr));
	Spin_InUse_clr();
	
	*retStr += "[grouplist_end]\n";
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
STDSTR &GC_LIST::CreateGroupListStrV0_5(STDSTR *retStr){
	//V0.5
	//	[grouplist]
	//		[group]
	//		[group_end]
	//		[group]
	//		[group_end]
	//	[grouplist_end]
	
	*retStr += "[grouplist]\n";
	
	Spin_InUse_set();
	TREE_LChildRChain_Traversal_LINE(COMMAND_GROUP,this,operateNode_t->CreateGroupStrV0_5(retStr));
	Spin_InUse_clr();
	
	*retStr += "[grouplist_end]\n";
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
STDSTR &GC_LIST::CreateGroupListStrV0_4(STDSTR *retStr){
	//V0.4
	//	[grouplist]
	//		[group]
	//		[group_end]
	//		[group]
	//		[group_end]
	//	[grouplist_end]
	
	*retStr += "[grouplist]\n";
	TREE_LChildRChain_Traversal_LINE(COMMAND_GROUP,this,operateNode_t->CreateGroupStrV0_4(retStr));
	*retStr += "[grouplist_end]\n";
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
STDSTR &GC_LIST::CreateGroupListStrV0_2(STDSTR *retStr){
	//V0.2
	//Group List: Group
	//{Group1}{Group2}

	TREE_LChildRChain_Traversal_LINE(COMMAND_GROUP,this,operateNode_t->CreateGroupStrV0_2(retStr));
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
void GC_LIST::SetGroupListV0_6(STDSTR *strIn){
	//V0.5
	//	[grouplist]
	//		[group]
	//		[group_end]
	//		[group]
	//		[group_end]
	//	[grouplist_end]
	COMMAND_GROUP	*group;
	STDSTR		strLine;
	
	while(strIn->length() > 0){
		strLine = Str_LowerCase(Str_Trim(Str_ReadSubItem(strIn,"\n")));
		if (strLine == "[grouplist_end]")
			break;
		if (strLine == "[group]"){
			group = GetNewNode();
			if (group != nullptr){
				group->SetGroupV0_6(strIn);
				AddNode(group);
			}
		}
	}
}
//------------------------------------------------------------------------------------------//
void GC_LIST::SetGroupListV0_5(STDSTR *strIn){
	//V0.5
	//	[grouplist]
	//		[group]
	//		[group_end]
	//		[group]
	//		[group_end]
	//	[grouplist_end]
	COMMAND_GROUP	*group;
	STDSTR		strLine;
	
	while(strIn->length() > 0){
		strLine = Str_LowerCase(Str_Trim(Str_ReadSubItem(strIn,"\n")));
		if (strLine == "[grouplist_end]")
			break;
		if (strLine == "[group]"){
			group = GetNewNode();
			if (group != nullptr){
				group->SetGroupV0_5(strIn);
				AddNode(group);
			}
		}
	}
}
//------------------------------------------------------------------------------------------//
void GC_LIST::SetGroupListV0_4(STDSTR *strIn){
	//V0.4
	//	[grouplist]
	//		[group]
	//		[group_end]
	//		[group]
	//		[group_end]
	//	[grouplist_end]
	COMMAND_GROUP	*group;
	STDSTR		strLine;
	
	while(strIn->length() > 0){
		strLine = Str_LowerCase(Str_Trim(Str_ReadSubItem(strIn,"\n")));
		if (strLine == "[grouplist_end]")
			break;
		if (strLine == "[group]"){
			group = GetNewNode();
			if (group != nullptr){
				group->SetGroupV0_4(strIn);
				AddNode(group);
			}
		}
	}
}
//------------------------------------------------------------------------------------------//
void GC_LIST::SetGroupListV0_3(STDSTR *strIn){
	//V0.3
	//Group List: Group
	//{Group1}{Group2}
	
	STDSTR	strResult;
	COMMAND_GROUP *group;
	
	while(strIn->length() > 0){
		strResult = Str_ReadSubItem(strIn,":");
		group = GetNewNode();
		if (group != nullptr){
			group->SetGroupV0_3(&strResult);
			AddNode(group);
		}
	}
}
//------------------------------------------------------------------------------------------//
void GC_LIST::SetGroupListV0_2(STDSTR *strIn){
	//V0.2 Group list
	//Group List: Group
	//{Group1}{Group2}
	
	STDSTR	strResult;
	COMMAND_GROUP *group;
	
	while(strIn->length() > 0){
		strResult = Str_ReadSubItem(strIn,":");
		group = GetNewNode();
		if (group != nullptr){
			group->SetGroupV0_2(&strResult);
			AddNode(group);
		}
	}
}
//------------------------------------------------------------------------------------------//
void GC_LIST::ClearTestResult(void){
	TREE_LChildRChain_Traversal_LINE(COMMAND_GROUP,this,operateNode_t->ClearResult());
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void COMMAND_NODE::CopyCOMMAND_NODE(COMMAND_NODE *node1,const COMMAND_NODE *node2){
	if ((node2 != nullptr) && (node1 != nullptr)){
		node1->Spin_InUse_set();
		((COMMAND_NODE*)node2)->Spin_InUse_set();
		node1->StrCommand = node2->StrCommand;
		node1->blEnableSend = node2->blEnableSend;
		node1->cmdTail = node2->cmdTail;
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
void COMMAND_GROUP::CopyCOMMAND_GROUP(COMMAND_GROUP *group1,const COMMAND_GROUP *group2,int32 blClear){
	COMMAND_NODE  *newNode;
	if ((group2 == nullptr) || (group1 == nullptr))
		return;
	if (blClear != 0){
		group1->Empty();
		group1->name = group2->name;
		group1->intervalTime = group2->intervalTime;
		group1->blEnableAutoRun = group2->blEnableAutoRun;
		group1->autoRunTimes = group2->autoRunTimes;
	}

	TREE_LChildRChain_Traversal_LINE(COMMAND_NODE,((COMMAND_GROUP*)group2),
		newNode = group1->GetNewNode();
		if (newNode != nullptr){
			COMMAND_NODE::CopyCOMMAND_NODE(newNode,operateNode_t);
			group1->AddNode(newNode);
		}
	);
}
//------------------------------------------------------------------------------------------//
void GC_LIST::CopyCOMMAND_GROUP_ENABLE(GC_LIST *tGroupList1,const GC_LIST *tGroupList2){
	COMMAND_GROUP	*newGroup;
	
	if ((tGroupList2 == nullptr) || (tGroupList1 == nullptr))
		return;
	tGroupList1->Empty();
	tGroupList1->Spin_InUse_set();
	((GC_LIST*)tGroupList2)->Spin_InUse_set();
	TREE_LChildRChain_Traversal_LINE(COMMAND_GROUP, ((GC_LIST*)tGroupList2),
		operateNode_t->Spin_InUse_set();
		if (operateNode_t->blEnableAutoRun != 0){
			newGroup = tGroupList1->GetNewNode();
			if (newGroup != nullptr){
				COMMAND_GROUP::CopyCOMMAND_GROUP(newGroup,operateNode_t);
				tGroupList1->AddNode(newGroup);
			}
		}
		operateNode_t->Spin_InUse_clr();
	);
	((GC_LIST*)tGroupList2)->Spin_InUse_clr();
	tGroupList1->Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
#endif
