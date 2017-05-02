/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: CommandSlist.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.11.13
 *
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "CommandSList.h"
#include "Comm_Convert.h"

#ifdef CommandSlistH
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void SC_NODE::Init(void){
	blEnableSendCR = 1;
	StrCommand = "";
}
//------------------------------------------------------------------------------------------//
STDSTR &SC_NODE::CreateNodeStrV0_4(STDSTR *retStr){
	//V0.4
	//SingleCommand
	//		[scNode]
	//			blEnableCR =
	//			strCommand =
	//		[scNode_end]
	
	Spin_InUse_set();
	*retStr  += "  [scNode]\n";
	if (blEnableSendCR == 0)
		*retStr += ("    blEnableSendCR = 0\n");
	*retStr += ("    strCommand = " + StrCommand + "\n");
	*retStr +=  "  [scNode_end]\n";
	Spin_InUse_clr();
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
STDSTR &SC_NODE::CreateNodeStrV0_2(STDSTR *retStr){
	//V0.2
	//SingleCommand
	//{(EnableCR,command)(EnableCR,command)}
	
	Spin_InUse_set();
	*retStr += '(';
	*retStr += Str_ASCIIToHEX(Str_ToString(blEnableSendCR),G_ESCAPE_OFF);
	*retStr += ",";
	*retStr += Str_ASCIIToHEX(StrCommand,G_ESCAPE_OFF);
	*retStr += ')';
	Spin_InUse_clr();
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
void SC_NODE::SetNodeV0_4(STDSTR *strIn){
	//V0.4
	//SingleCommand
	//		[scNode]
	//			blEnableSendCR =
	//			strCommand =
	//		[scNode_end]
	STDSTR		strLine,strItem;
	
	Spin_InUse_set();
	Init();
	while(strIn->length() > 0){
		strLine = Str_Trim(Str_ReadSubItem(strIn,"\n"));
		if (Str_LowerCase(strLine) == "[scnode_end]")
			break;
		strItem = Str_LowerCase(Str_Trim(Str_ReadSubItem(&strLine,"=")));
		Str_TrimSelf(strLine);
		
		if (strItem == "blenablesendcr"){
			blEnableSendCR = atoi(strLine.c_str());
		}
		else if (strItem == "strcommand"){
			StrCommand = strLine;
		}
	}
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void SC_NODE::SetNodeV0_2(STDSTR *strIn){
	//V0.2
	//SingleCommand
	//{(EnableCR,command)(EnableCR,command)}
	Spin_InUse_set();
	Init();
	blEnableSendCR = atoi(Str_HEXToASCII(Str_ReadSubItem(strIn,",")).c_str());
	StrCommand = Str_HEXToASCII(*strIn);
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void SC_NODE::Copy(SC_NODE *node1,const SC_NODE *node2){
	node1->Spin_InUse_set();
	((SC_NODE*)node2)->Spin_InUse_set();
	node1->blEnableSendCR = node2->blEnableSendCR;
	node1->StrCommand = node2->StrCommand;
	((SC_NODE*)node2)->Spin_InUse_clr();
	node1->Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
STDSTR&	SC_NODE::GetTitle(STDSTR *retStr){
	*retStr = "  sID      command";
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
STDSTR&	SC_NODE::Compose(STDSTR *retStr){
	Spin_InUse_set();
	*retStr = Str_ToString(GetdRNodeID(this));
	AddSpaceInFront(retStr,4);
	*retStr += (blEnableSendCR == 0)?".      ":".  CR  ";
	*retStr += StrCommand;
	Spin_InUse_clr();
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
STDSTR &SC_LIST::CreateSCListStrV0_4(STDSTR *retStr){
	//V0.4
	//SingleCommand
	//	[singleCommand]
	//		[scNode]
	//		[scNode_end]
	//		[scNode]
	//		[scNode_end]
	//	[singleCommand_end]
	
	*retStr += "[singleCommand]\n";
	TREE_LChildRChain_Traversal_LINE(SC_NODE,this, operateNode_t->CreateNodeStrV0_4(retStr));
	*retStr += "[singleCommand_end]\n";
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
STDSTR &SC_LIST::CreateSCListStrV0_2(STDSTR *retStr){
	//V0.2
	//SingleCommand
	//{(EnableCR,command)(EnableCR,command)}
	
	*retStr = '{';
	TREE_LChildRChain_Traversal_LINE(SC_NODE,this, operateNode_t->CreateNodeStrV0_4(retStr));
	*retStr += '}';
	return(*retStr);
}
//------------------------------------------------------------------------------------------//
void SC_LIST::SetSCListV0_4(STDSTR *strIn){
	//V0.4
	//SingleCommand
	//	[singleCommand]
	//		[scNode]
	//		[scNode_end]
	//		[scNode]
	//		[scNode_end]
	//	[singleCommand_end]
	
	SC_NODE		*node;
	STDSTR		strLine;

	while(strIn->length() > 0){
		strLine = Str_LowerCase(Str_Trim(Str_ReadSubItem(strIn,"\n")));
		if (strLine == "[singlecommand_end]")
			break;
		if (strLine == "[scnode]"){
			node = GetNewNode();
			if (node != nullptr){
				node->SetNodeV0_4(strIn);
				AddNode(node);
			}
		}
	}
}
//------------------------------------------------------------------------------------------//
void SC_LIST::SetSCListV0_2(STDSTR *strIn){
	//V0.2
	//SingleCommand
	//{(EnableCR,command)(EnableCR,command)}
	STDSTR	strResult;
	SC_NODE	*node;
	
	while(strIn->length() > 0){
		strResult = Str_ReadSubItem(strIn,"|");
		node = GetNewNode();
		if (node != nullptr){
			node->SetNodeV0_2(&strResult);
			AddNode(node);
		}
	}
}
//------------------------------------------------------------------------------------------//
#endif
