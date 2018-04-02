//
//  GList.cpp
//  SDT
//
//  Created by Leif Wen on 17/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "GList.h"
#ifdef GList_h
#include "DS_STRING.h"
#include "SYS_File.h"
//------------------------------------------------------------------------------------------//
COMMAND_NODE::COMMAND_NODE(void) : TREE_NODE(){
	Init();
	SetSelfName("COMMAND_NODE");
};
//------------------------------------------------------------------------------------------//
void COMMAND_NODE::Init(void){
	StrCommand = "";
	blEnableSend = G_TRUE;
	cmdTail = CMD_R;
	
	StrShowName = "";
	blSameAsCommand = G_TRUE;
	
	StrTimeout = "1";
	StrCycle = "1";
	
	StrContinue = "";
	StrResend = "";
	StrStop = "";
	StrCatch = "";
	
	runTimes = 0;
	catchTimes = 0;
	timeoutTimes = 0;
	blFirstSynchronous = G_FALSE;
	timeST0.Clear();
}
//------------------------------------------------------------------------------------------//
STDSTR&	COMMAND_NODE::Export(uint32 ver,STDSTR* strOut){
	switch (ver) {
		case 0:;
		case 1:;
		case 2:;
		case 3:;
			break;
		case 4:;
			ExportV0_4(ver,strOut);
			break;
		case 5:;
			ExportV0_5(ver,strOut);
			break;
		case 6:;
		default:
			ExportV0_6(ver,strOut);
			break;
	}
	return(*strOut);
}
//------------------------------------------------------------------------------------------//
void COMMAND_NODE::Import(uint32 ver,STDSTR* strIn){
	switch (ver) {
		case 0:;
		case 1:;
		case 2:;
		case 3:;
			break;
		case 4:;
			ImportV0_4(ver,strIn);
			break;
		case 5:;
			ImportV0_5(ver,strIn);
			break;
		case 6:;
		default:
			ImportV0_6(ver,strIn);
			break;
	}
	return;
}
//------------------------------------------------------------------------------------------//
STDSTR& COMMAND_NODE::ExportV0_6(uint32 ver,STDSTR* strOut){
	//V0.6
	//	[node]
	//		StrCommand =
	//		blEnableSend =
	//		cmdTail =
	//		StrShowName =
	//		blSameAsCommand =
	//		StrTimeout =
	//		StrCycle =
	//		StrContinue =
	//		StrResend =
	//		StrStop =
	//		StrCatch =
	//	[node_end]
	
	InUse_set();
	*strOut += "    [node]\n";
	*strOut += ("      strCommand = " + Str_Trim(StrCommand) + "\n");
	if (blEnableSend == 0)
		*strOut += ("      blEnableSend = 0\n");
	if (cmdTail != CMD_R)
		*strOut += ("      cmdTail = " + Str_ToStr((uint32)cmdTail) + "\n");
	if (blSameAsCommand == 0){
		if (Str_Trim(StrShowName) != Str_Trim(StrCommand))
			*strOut += ("      StrShowName = " + Str_Trim(StrShowName) + "\n");
	}
	if ((StrTimeout.length() > 0) && (StrTimeout != "1"))
		*strOut += ("      strTimeout = " + Str_Trim(StrTimeout) + "\n");
	if ((StrCycle.length() > 0) && (StrCycle != "1"))
		*strOut += ("      strCycle = " + Str_Trim(StrCycle) + "\n");
	if (StrContinue.length() > 0)
		*strOut += ("      strContinue = " + Str_Trim(StrContinue) + "\n");
	if (StrResend.length() > 0)
		*strOut += ("      strResend = " + Str_Trim(StrResend) + "\n");
	if (StrStop.length() > 0)
		*strOut += ("      strStop = " + Str_Trim(StrStop) + "\n");
	if (StrCatch.length() > 0)
		*strOut += ("      strCatch = " + Str_Trim(StrCatch) + "\n");
	*strOut +=  "    [node_end]\n";
	InUse_clr();
	return(*strOut);
}
//------------------------------------------------------------------------------------------//
STDSTR& COMMAND_NODE::ExportV0_5(uint32 ver,STDSTR* strOut){
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
	
	InUse_set();
	*strOut += "    [node]\n";
	*strOut += ("      strCommand = " + Str_Trim(StrCommand) + "\n");
	if (blEnableSend == 0)
		*strOut += ("      blEnableSend = 0\n");
	if (cmdTail == 0)
		*strOut += ("      blEnableSendCR = 0\n");
	//if (blEnableHEX != 0)
	//	*strOut += ("      blEnableHEX = 1\n");
	if (blSameAsCommand == 0){
		if (Str_Trim(StrShowName) != Str_Trim(StrCommand))
			*strOut += ("      StrShowName = " + Str_Trim(StrShowName) + "\n");
	}
	if ((StrTimeout.length() > 0) && (StrTimeout != "1"))
		*strOut += ("      strTimeout = " + Str_Trim(StrTimeout) + "\n");
	if ((StrCycle.length() > 0) && (StrCycle != "1"))
		*strOut += ("      strCycle = " + Str_Trim(StrCycle) + "\n");
	if (StrContinue.length() > 0)
		*strOut += ("      strContinue = " + Str_Trim(StrContinue) + "\n");
	if (StrResend.length() > 0)
		*strOut += ("      strResend = " + Str_Trim(StrResend) + "\n");
	if (StrStop.length() > 0)
		*strOut += ("      strStop = " + Str_Trim(StrStop) + "\n");
	if (StrCatch.length() > 0)
		*strOut += ("      strCatch = " + Str_Trim(StrCatch) + "\n");
	*strOut +=  "    [node_end]\n";
	InUse_clr();
	return(*strOut);
}
//------------------------------------------------------------------------------------------//
STDSTR& COMMAND_NODE::ExportV0_4(uint32 ver,STDSTR* strOut){
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
	
	InUse_set();
	*strOut += "    [node]\n";
	if (blEnableSend == 0)
		*strOut += ("      blEnableSend = 0\n");
	if (cmdTail == 0)
		*strOut += ("      blEnableSendCR = 0\n");
	//if (blEnableHEX != 0)
	//	*strOut += ("      blEnableHEX = 1\n");
	if (StrTimeout != "")
		*strOut += ("      strTimeout = " + StrTimeout + "\n");
	*strOut += ("      strCommand = " + StrCommand + "\n");
	if (StrContinue != "")
		*strOut += ("      strContinue = " + StrContinue + "\n");
	if (StrStop != "")
		*strOut += ("      strStop = " + StrStop + "\n");
	if (StrResend != "")
		*strOut += ("      strResend = " + StrResend + "\n");
	if (StrCycle != "")
		*strOut += ("      strCycle = " + StrCycle + "\n");
	if (StrCatch != "")
		*strOut += ("      strCatch = " + StrCatch + "\n");
	*strOut +=  "    [node_end]\n";
	InUse_clr();
	return(*strOut);
}
//------------------------------------------------------------------------------------------//
void COMMAND_NODE::ImportV0_6(uint32 ver,STDSTR* strIn){
	//V0.6
	//	[node]
	//		StrCommand =
	//		blEnableSend =
	//		cmdTail =
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
	
	InUse_set();
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
	InUse_clr();
}
//------------------------------------------------------------------------------------------//
void COMMAND_NODE::ImportV0_5(uint32 ver,STDSTR* strIn){
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
	InUse_set();
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
	InUse_clr();
}
//------------------------------------------------------------------------------------------//
void COMMAND_NODE::ImportV0_4(uint32 ver,STDSTR* strIn){
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
	InUse_set();
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
	InUse_clr();
}
//------------------------------------------------------------------------------------------//
STDSTR&	COMMAND_NODE::GetTitle(STDSTR* retStr,uint32 flag){
	*retStr  = " cID    ";
	*retStr += (B_ChkFLAG32(flag,CL_showCycle) == 0)?"":"  cycle";
	*retStr += (B_ChkFLAG32(flag,CL_showTimeout) == 0)?"":"  timeout";
	*retStr += "  command";
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
STDSTR&	COMMAND_NODE::Compose(STDSTR* retStr,uint32 flag){
	STDSTR	strTemp;
	InUse_set();
	*retStr = Str_ToStr(GetdRNodeID(this));
	Str_AddSpaceInFront(retStr,3);
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
		Str_AddSpaceInFront(&strTemp,7);
		*retStr += strTemp;
	}
	if (B_ChkFLAG32(flag,CL_showTimeout) != 0){
		strTemp = StrTimeout + "s";
		Str_AddSpaceInFront(&strTemp,9);
		*retStr += strTemp;
	}
	*retStr += "  ";
	*retStr += StrCommand;
	InUse_clr();
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
STDSTR&	COMMAND_NODE::ComposeDetail(STDSTR* retStr){
	InUse_set();
	*retStr  = "     Continue : " + StrContinue + "\n";
	*retStr += "     Resend   : " + StrResend + "\n";
	*retStr += "     CStop    : " + StrStop + "\n";
	*retStr += "     Catch    : " + StrCatch + "\n";
	InUse_clr();
	return(*retStr);
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
COMMAND_GROUP::COMMAND_GROUP(void) : TREE_NODE(){
	Init();
	SetSelfName("COMMAND_GROUP");
}
//------------------------------------------------------------------------------------------//
COMMAND_GROUP::~COMMAND_GROUP(void){
	CleanChild(this, this);
};
//------------------------------------------------------------------------------------------//
void COMMAND_GROUP::Init(void){
	name = "";
	intervalTime = 200;
	
	blEnableAutoRun = G_FALSE;
	autoRunTimes = 1;
}
//------------------------------------------------------------------------------------------//
TNFP& COMMAND_GROUP::GetTrashOwer(void){
	static TNFP sgSpareOwner;
	return(sgSpareOwner);
}
//------------------------------------------------------------------------------------------//
TNFP* COMMAND_GROUP::GetTrash(void){
	return(GetTrashOwer().GetTrash());
};
//------------------------------------------------------------------------------------------//
TNF* COMMAND_GROUP::CreateNode(void){
	return(SetSubNodeFatherName(new COMMAND_NODE));
}
//------------------------------------------------------------------------------------------//
STDSTR&	COMMAND_GROUP::Export(uint32 ver,STDSTR* strOut){
	switch (ver) {
		case 0:;
		case 1:;
		case 2:;
		case 3:;
			break;
		case 4:;
		default:
			ExportV0_4(ver,strOut);
			break;
	}
	return(*strOut);
}
//------------------------------------------------------------------------------------------//
void COMMAND_GROUP::Import(uint32 ver,STDSTR* strIn){
	switch (ver) {
		case 0:;
		case 1:;
		case 2:;
		case 3:;
			break;
		case 4:;
		default:
			ImportV0_4(ver,strIn);
			break;
	}
	return;
}
//------------------------------------------------------------------------------------------//
STDSTR& COMMAND_GROUP::ExportV0_4(uint32 ver,STDSTR* strOut){
	//V0.4
	//	[group]
	//		groupName =
	//		intervalTime =
	//		[node]
	//		[node_end]
	//		[node]
	//		[node_end]
	//	[group_end]
	
	*strOut +=  "  [group]\n";
	InUse_set();
	*strOut += ("    groupName = " + name + "\n");
	*strOut += ("    intervalTime = " + Str_ToStr(intervalTime) + "\n");
	InUse_clr();
	
	TREE_LChildRChain_Traversal_LINE_nolock(COMMAND_NODE,this,_opNode->Export(ver,strOut));
	
	*strOut += "  [group_end]\n";
	return(*strOut);
}
//------------------------------------------------------------------------------------------//
void COMMAND_GROUP::ImportV0_4(uint32 ver,STDSTR* strIn){
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
	
	InUse_set();
	name = "";
	intervalTime = 200;
	while(strIn->length() > 0){
		strLine = Str_Trim(Str_ReadSubItem(strIn,"\n"));
		if (Str_LowerCase(strLine) == "[group_end]")
			break;
		if (Str_LowerCase(strLine) == "[node]"){
			node = (COMMAND_NODE*)GetNewNode();
			if (node != nullptr){
				node->Import(ver,strIn);
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
	InUse_clr();
}
//------------------------------------------------------------------------------------------//
void COMMAND_GROUP::ClearResult(void){
	TREE_LChildRChain_Traversal_LINE(COMMAND_NODE,this,
		_opNode->InUse_set();
		_opNode->runTimes = 0;
		_opNode->catchTimes = 0;
		_opNode->timeoutTimes = 0;
		_opNode->blFirstSynchronous = G_FALSE;
		_opNode->timeST0.Clear();
		_opNode->InUse_clr();
	);
}
//------------------------------------------------------------------------------------------//
STDSTR&	COMMAND_GROUP::GetTitle(STDSTR* retStr,uint32 flag){
	*retStr  = " gID";
	*retStr += (B_ChkFLAG32(flag,GL_showCycle) == 0)?"":"  cycle";
	*retStr += (B_ChkFLAG32(flag,GL_showInterval) == 0)?"":"  interval";
	*retStr += "  group name";
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
STDSTR&	COMMAND_GROUP::Compose(STDSTR* retStr,uint32 flag){
	STDSTR	strTemp;
	InUse_set();
	*retStr = Str_ToStr(GetdRNodeID(this));
	Str_AddSpaceInFront(retStr,3);
	*retStr = ((blEnableAutoRun == 0)?" ":"*") + *retStr;
	*retStr += '.';
	
	if (B_ChkFLAG32(flag,GL_showCycle) != 0){
		strTemp = Str_ToStr(autoRunTimes);
		Str_AddSpaceInFront(&strTemp,7);
		*retStr += strTemp;
	}
	if (B_ChkFLAG32(flag,GL_showInterval) != 0){
		strTemp = Str_ToStr(intervalTime) + "ms";
		Str_AddSpaceInFront(&strTemp,10);
		*retStr += strTemp;
	}
	*retStr += "  ";
	*retStr += name;
	InUse_clr();
	return(*retStr);
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
GC_LIST::GC_LIST(void) : TREE_NODE(){
	SetSelfName("GC_LIST");
};
//------------------------------------------------------------------------------------------//
GC_LIST::~GC_LIST(void){
	Empty();
};
//------------------------------------------------------------------------------------------//
TNF* GC_LIST::CreateNode(void){
	return(SetSubNodeFatherName(new COMMAND_GROUP));
};
//------------------------------------------------------------------------------------------//
void GC_LIST::MoveToTrash(TNF* tFirstNode,TNF* tEndNode){
	Remove(tFirstNode,tEndNode);
	TREE_RChain_Traversal_LINE_nolock(COMMAND_GROUP,tFirstNode,CleanChild(_opNode,_opNode));
	MoveNodesToTrash(this,tFirstNode,tEndNode);
};
//------------------------------------------------------------------------------------------//
void GC_LIST::Empty(void){
	TREE_LChildRChain_Traversal_LINE(COMMAND_GROUP,this,CleanChild(_opNode,_opNode));
	CleanChild(this,this);
};
//------------------------------------------------------------------------------------------//
void GC_LIST::Save(const STDSTR& fileName){
	STDSTR	strResult;
	strResult = "[V0.6]\n";
	
	CFS_WriteFile(fileName,Export(6,&strResult));
};
//------------------------------------------------------------------------------------------//
void GC_LIST::Load(const STDSTR& fileName){
	STDSTR	strResult,strLine;
	uint32	ver;
	
	strResult = "";
	CFS_ReadFile(&strResult,fileName);
	
	strLine = Str_LowerCase(Str_Trim(Str_ReadSubItem(&strResult,"\n")));
	
	ver = 6;
	if (strLine == "[v0.1]"){
		ver = 1;
	}
	else if (strLine == "[v0.2]"){
		ver = 2;
	}
	else if (strLine == "[v0.3]"){
		ver = 3;
	}
	else if (strLine == "[v0.4]"){
		ver = 4;
	}
	else if (strLine == "[v0.5]"){
		ver = 5;
	}

	while(strResult.length() > 0){
		strLine = Str_LowerCase(Str_Trim(Str_ReadSubItem(&strResult,"\n")));
		if (strLine == "[grouplist]")
			Import(ver,&strResult);
	};
};
//------------------------------------------------------------------------------------------//
STDSTR&	GC_LIST::Export(uint32 ver,STDSTR* strOut){
	switch (ver) {
		case 0:;
		case 1:;
		case 2:;
		case 3:;
			break;
		case 4:;
		default:
			ExportV0_4(ver,strOut);
			break;
	}
	return(*strOut);
}
//------------------------------------------------------------------------------------------//
void GC_LIST::Import(uint32 ver,STDSTR* strIn){
	switch (ver) {
		case 0:;
		case 1:;
		case 2:;
		case 3:;
			break;
		case 4:;
		default:
			ImportV0_4(ver,strIn);
			break;
	}
	return;
}
//------------------------------------------------------------------------------------------//
STDSTR& GC_LIST::ExportV0_4(uint32 ver,STDSTR* strOut){
	//V0.4
	//	[grouplist]
	//		[group]
	//		[group_end]
	//		[group]
	//		[group_end]
	//	[grouplist_end]
	
	*strOut += "[grouplist]\n";
	TREE_LChildRChain_Traversal_LINE(COMMAND_GROUP,this,_opNode->Export(ver,strOut));
	*strOut += "[grouplist_end]\n";
	return(*strOut);
}
//------------------------------------------------------------------------------------------//
void GC_LIST::ImportV0_4(uint32 ver,STDSTR* strIn){
	//V0.4
	//	[grouplist]
	//		[group]
	//		[group_end]
	//		[group]
	//		[group_end]
	//	[grouplist_end]
	COMMAND_GROUP	*group;
	STDSTR			strLine;
	
	while(strIn->length() > 0){
		strLine = Str_LowerCase(Str_Trim(Str_ReadSubItem(strIn,"\n")));
		if (strLine == "[grouplist_end]")
			break;
		if (strLine == "[group]"){
			group = (COMMAND_GROUP*)GetNewNode();
			if (group != nullptr){
				group->Import(ver,strIn);
				AddNode(group);
			}
		}
	}
}
//------------------------------------------------------------------------------------------//
void GC_LIST::ClearTestResult(void){
	TREE_LChildRChain_Traversal_LINE(COMMAND_GROUP,this,_opNode->ClearResult());
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void COMMAND_NODE::CopyCOMMAND_NODE(COMMAND_NODE* node1,const COMMAND_NODE* node2){
	if ((node2 != nullptr) && (node1 != nullptr)){
		node1->InUse_set();
		((COMMAND_NODE*)node2)->InUse_set();
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
		((COMMAND_NODE*)node2)->InUse_clr();
		node1->InUse_clr();
	}
}
//------------------------------------------------------------------------------------------//
void COMMAND_GROUP::CopyCOMMAND_GROUP(COMMAND_GROUP* group1,const COMMAND_GROUP* group2,bool32 blClear){
	COMMAND_NODE  *newNode;
	if ((group2 == nullptr) || (group1 == nullptr))
		return;
	if (blClear != 0){
		CleanChild(group1, group1);
		group1->name = group2->name;
		group1->intervalTime = group2->intervalTime;
		group1->blEnableAutoRun = group2->blEnableAutoRun;
		group1->autoRunTimes = group2->autoRunTimes;
	}
	
	TREE_LChildRChain_Traversal_LINE(COMMAND_NODE,((COMMAND_GROUP*)group2),
		newNode = (COMMAND_NODE*)group1->GetNewNode();
		if (newNode != nullptr){
			COMMAND_NODE::CopyCOMMAND_NODE(newNode,_opNode);
			group1->AddNode(newNode);
		}
	);
}
//------------------------------------------------------------------------------------------//
void GC_LIST::CopyCOMMAND_GROUP_ENABLE(GC_LIST* tGroupList1,const GC_LIST* tGroupList2){
	COMMAND_GROUP	*newGroup;
	
	if ((tGroupList2 == nullptr) || (tGroupList1 == nullptr))
		return;
	tGroupList1->Empty();
	tGroupList1->InUse_set();
	((GC_LIST*)tGroupList2)->InUse_set();
	TREE_LChildRChain_Traversal_LINE(COMMAND_GROUP, ((GC_LIST*)tGroupList2),
		_opNode->InUse_set();
		if (_opNode->blEnableAutoRun != 0){
			newGroup = (COMMAND_GROUP*)tGroupList1->GetNewNode();
			if (newGroup != nullptr){
				COMMAND_GROUP::CopyCOMMAND_GROUP(newGroup,_opNode);
				tGroupList1->AddNode(newGroup);
			}
		}
		_opNode->InUse_clr();
	);
	((GC_LIST*)tGroupList2)->InUse_clr();
	tGroupList1->InUse_clr();
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* GList_h */
