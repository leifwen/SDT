//
//  SList.cpp
//  SDT
//
//  Created by Leif Wen on 17/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "SList.h"
#ifdef SList_h
#include "DS_STRING.h"
#include "SYS_File.h"
//------------------------------------------------------------------------------------------//
SC_NODE::SC_NODE(void) : TNF(){
	Init();
};
//------------------------------------------------------------------------------------------//
void SC_NODE::Init(void){
	blEnableSendCR = 1;
	StrCommand = "";
};
//------------------------------------------------------------------------------------------//
STDSTR&	SC_NODE::Export(uint32 ver,STDSTR* strOut){
	switch (ver) {
		case 0:;
		case 1:;
		case 2:;
		case 3:;
			break;
  		case 4:
  		default:
			ExportV0_4(ver,strOut);
			break;
	}
	return(*strOut);
};
//------------------------------------------------------------------------------------------//
void SC_NODE::Import(uint32 ver,STDSTR* strIn){
	switch (ver) {
		case 0:;
		case 1:;
		case 2:;
		case 3:;
			break;
		case 4:
		default:
			ImportV0_4(ver,strIn);
			break;
	}
	return;
};
//------------------------------------------------------------------------------------------//
STDSTR& SC_NODE::ExportV0_4(uint32 ver,STDSTR* strOut){
	//V0.4
	//SingleCommand
	//		[scNode]
	//			blEnableCR =
	//			strCommand =
	//		[scNode_end]
	
	rwLock.R_set();
	*strOut  += "  [scNode]\n";
	if (blEnableSendCR == 0)
		*strOut += ("    blEnableSendCR = 0\n");
	*strOut += ("    strCommand = " + StrCommand + "\n");
	*strOut +=  "  [scNode_end]\n";
	rwLock.R_clr();
	return(*strOut);
};
//------------------------------------------------------------------------------------------//
void SC_NODE::ImportV0_4(uint32 ver,STDSTR* strIn){
	//V0.4
	//SingleCommand
	//		[scNode]
	//			blEnableSendCR =
	//			strCommand =
	//		[scNode_end]
	STDSTR		strLine,strItem;
	
	rwLock.W_set();
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
	rwLock.W_clr();
};
//------------------------------------------------------------------------------------------//
void SC_NODE::Copy(SC_NODE* node1,const SC_NODE * node2){
	node1->rwLock.W_set();
	((SC_NODE*)node2)->rwLock.R_set();
	node1->blEnableSendCR = node2->blEnableSendCR;
	node1->StrCommand = node2->StrCommand;
	((SC_NODE*)node2)->rwLock.R_clr();
	node1->rwLock.W_clr();
};
//------------------------------------------------------------------------------------------//
STDSTR&	SC_NODE::GetTitle(STDSTR* retStr){
	*retStr = " sID      command";
	return(*retStr);
};
//------------------------------------------------------------------------------------------//
STDSTR&	SC_NODE::Compose(STDSTR* retStr){
	rwLock.R_set();
	*retStr = Str_ToStr(GetDRNodeID(this));
	Str_AddSpaceInFront(retStr,4);
	*retStr += (blEnableSendCR == 0)?".      ":".  CR  ";
	*retStr += StrCommand;
	rwLock.R_clr();
	return(*retStr);
};
//------------------------------------------------------------------------------------------//








//------------------------------------------------------------------------------------------//
SC_LIST::SC_LIST(void) : TNFP(){
	SetSelfName("SC_LIST");
};
//------------------------------------------------------------------------------------------//
SC_LIST::~SC_LIST(void){
	Empty();
};
//------------------------------------------------------------------------------------------//
TNF* SC_LIST::CreateNode(void){
	return(new SC_NODE);
};
//------------------------------------------------------------------------------------------//
void SC_LIST::MoveToTrash(TNF* tFirstNode,TNF* tEndNode){
	MoveNodesToTrash(this,tFirstNode,tEndNode);
};
//------------------------------------------------------------------------------------------//
void SC_LIST::Empty(void){
	 CleanDownTree(this,this);
};
//------------------------------------------------------------------------------------------//
void SC_LIST::Save(const STDSTR& fileName){
	STDSTR	strResult;
	strResult = "[V0.4]\n";
	
	CFS_WriteFile(fileName,Export(4,&strResult));
};
//------------------------------------------------------------------------------------------//
void SC_LIST::Load(const STDSTR& fileName){
	STDSTR	strResult,strLine;
	uint32	ver;
	
	strResult = "";
	CFS_ReadFile(&strResult,fileName);
	
	strLine = Str_LowerCase(Str_Trim(Str_ReadSubItem(&strResult,"\n")));
	
	ver = 4;
	if (strLine == "[v0.1]"){
		ver = 1;
	}
	else if (strLine == "[v0.2]"){
		ver = 2;
	}
	else if (strLine == "[v0.3]"){
		ver = 3;
	}
	
	while(strResult.length() > 0){
		strLine = Str_LowerCase(Str_Trim(Str_ReadSubItem(&strResult,"\n")));
		if (strLine == "[singlecommand]")
			Import(ver,&strResult);
	};
};
//------------------------------------------------------------------------------------------//
STDSTR&	SC_LIST::Export(uint32 ver,STDSTR* strOut){
	switch (ver) {
		case 0:;
		case 1:;
		case 2:;
		case 3:;
			break;
		case 4:
		default:
			ExportV0_4(ver,strOut);
			break;
	}
	return(*strOut);
};
//------------------------------------------------------------------------------------------//
void SC_LIST::Import(uint32 ver,STDSTR* strIn){
	switch (ver) {
		case 0:;
		case 1:;
		case 2:;
		case 3:;
			break;
		case 4:
		default:
			ImportV0_4(ver,strIn);
			break;
	}
	return;
};
//------------------------------------------------------------------------------------------//
STDSTR& SC_LIST::ExportV0_4(uint32 ver,STDSTR* strOut){
	//V0.4
	//SingleCommand
	//	[singleCommand]
	//		[scNode]
	//		[scNode_end]
	//		[scNode]
	//		[scNode_end]
	//	[singleCommand_end]
	
	*strOut += "[singleCommand]\n";
	TREE_DownChain_Traversal_LINE(SC_NODE,this, _opNode->Export(ver,strOut));
	*strOut += "[singleCommand_end]\n";
	return(*strOut);
};
//------------------------------------------------------------------------------------------//
void SC_LIST::ImportV0_4(uint32 ver,STDSTR* strIn){
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
			node = (SC_NODE*)GetNewNode();
			if (node != nullptr){
				node->Import(ver,strIn);
				AppendDownNode(node);
			}
		}
	}
};
//------------------------------------------------------------------------------------------//
#endif /* SList_h */
