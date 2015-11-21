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
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void SC_NODE::Init(SC_NODE *node,G_LOCK_VAILD blLock){
	node->Spin_InUse_set(blLock);
	node->blEnableSendCR = 1;
	node->StrCommand = "";
	node->Spin_InUse_clr(blLock);
}
//------------------------------------------------------------------------------------------//
std::string SC_NODE::CreateNodeStrV0_4(SC_NODE *node){
	//V0.4
	//SingleCommand
	//		[scNode]
	//			blEnableCR =
	//			strCommand =
	//		[scNode_end]
	std::string		strResult;
	
	node->Spin_InUse_set();
		strResult  =  "  [scNode]\n";
		if (node->blEnableSendCR == 0)
			strResult += ("    blEnableSendCR = 0\n");
		strResult += ("    strCommand = " + node->StrCommand + "\n");
		strResult +=  "  [scNode_end]\n";
	node->Spin_InUse_clr();
	return(strResult);
}
//------------------------------------------------------------------------------------------//
std::string SC_NODE::CreateNodeStrV0_2(SC_NODE *node){
	//V0.2
	//SingleCommand
	//{(EnableCR,command)(EnableCR,command)}
	std::string		strResult;
	
	node->Spin_InUse_set();
		strResult = '(';
		strResult += Str_ASCIIToHEX(Str_IntToString(node->blEnableSendCR),G_ESCAPE_OFF);
		strResult += ",";
		strResult += Str_ASCIIToHEX(node->StrCommand,G_ESCAPE_OFF);
		strResult += ')';
	node->Spin_InUse_clr();
	return(strResult);
}
//------------------------------------------------------------------------------------------//
void SC_NODE::SetNodeV0_4(SC_NODE *node,std::string *strInput){
	//V0.4
	//SingleCommand
	//		[scNode]
	//			blEnableSendCR =
	//			strCommand =
	//		[scNode_end]
	std::string		strLine,strItem;
	node->Spin_InUse_set();
		Init(node,G_LOCK_OFF);
		while(strInput->length() > 0){
			strLine = Str_Trim(Str_ReadSubItem(strInput,"\n"));
			if (Str_LowerCase(strLine) == "[scnode_end]")
				break;
			strItem = Str_LowerCase(Str_Trim(Str_ReadSubItem(&strLine,"=")));
			Str_TrimSelf(strLine);
			
			if (strItem == "blenablesendcr"){
				node->blEnableSendCR = atoi(strLine.c_str());
			}
			else if (strItem == "strcommand"){
				node->StrCommand = strLine;
			}
		}
	node->Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void SC_NODE::SetNodeV0_2(SC_NODE *node,std::string *strInput){
	//V0.2
	//SingleCommand
	//{(EnableCR,command)(EnableCR,command)}
	node->Spin_InUse_set();
		Init(node,G_LOCK_OFF);
		node->blEnableSendCR = atoi(Str_HEXToASCII(Str_ReadSubItem(strInput,",")).c_str());
		node->StrCommand = Str_HEXToASCII(*strInput);
	node->Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void SC_NODE::Copy(SC_NODE *node2,SC_NODE *node1){
	node1->Spin_InUse_set();
	node2->Spin_InUse_set();
	node1->blEnableSendCR = node2->blEnableSendCR;
	node1->StrCommand = node2->StrCommand;
	node2->Spin_InUse_clr();
	node1->Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
std::string SC_LIST::CreateSCListStrV0_4(void){
	//V0.4
	//SingleCommand
	//	[singleCommand]
	//		[scNode]
	//		[scNode_end]
	//		[scNode]
	//		[scNode_end]
	//	[singleCommand_end]
	
	std::string	strResult;
	
	strResult = "[singleCommand]\n";
	Spin_InUse_set();
	RTREE_LChildRChain_Traversal_LINE(SC_NODE,this,strResult += SC_NODE::CreateNodeStrV0_4(operateNode_t));
	Spin_InUse_clr();
	strResult += "[singleCommand_end]\n";
	return(strResult);
}
//------------------------------------------------------------------------------------------//
std::string SC_LIST::CreateSCListStrV0_2(void){
	//V0.2
	//SingleCommand
	//{(EnableCR,command)(EnableCR,command)}
	std::string		strResult;
	
	strResult = '{';
	Spin_InUse_set();
	RTREE_LChildRChain_Traversal_LINE(SC_NODE,this,strResult += SC_NODE::CreateNodeStrV0_4(operateNode_t));
	Spin_InUse_clr();

	strResult += '}';
	return(strResult);
}
//------------------------------------------------------------------------------------------//
void SC_LIST::SetSCListV0_4(std::string *strInput){
	//V0.4
	//SingleCommand
	//	[singleCommand]
	//		[scNode]
	//		[scNode_end]
	//		[scNode]
	//		[scNode_end]
	//	[singleCommand_end]
	SC_NODE			*node;
	std::string		strLine,strItem;
	Spin_InUse_set();
	while(strInput->length() > 0){
		strLine = Str_LowerCase(Str_Trim(Str_ReadSubItem(strInput,"\n")));
		if (strLine == "[singlecommand_end]")
			break;
		if (strLine == "[scnode]"){
			node = new SC_NODE;
			if (node != nullptr){
				SC_NODE::SetNodeV0_4(node,strInput);
				AddNode(node);
			}
		}
	}
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void SC_LIST::SetSCListV0_2(std::string *strInput){
	//V0.2
	//SingleCommand
	//{(EnableCR,command)(EnableCR,command)}
	std::string	strResult;
	SC_NODE			*node;
	Spin_InUse_set();
	while(strInput->length() > 0){
		strResult = Str_ReadSubItem(strInput,"|");
		node = new SC_NODE;
		if (node != nullptr){
			SC_NODE::SetNodeV0_2(node,&strResult);
			AddNode(node);
		}
	}
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
