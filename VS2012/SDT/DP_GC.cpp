// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Global.h"
#include "SDT.h"
#include "DP_GC.h"
#include "DP_GCProperties.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//------------------------------------------------------------------------------------------//
CGCTree	g_wndGCTree;
//------------------------------------------------------------------------------------------//
CGCTree::CGCTree(void) : CMyCTreeCtrl(){
	m_GCList = NULL;
	m_OnShowGroup = NULL;
}
//------------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CGCTree,CMyCTreeCtrl)
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()
//------------------------------------------------------------------------------------------//
void CGCTree::LoadData(GC_LIST *tGCList){
	HTREEITEM	hRoot,hItem;
	int32		blCheckHead;

	DeleteAllItems();
	m_GCList = tGCList;
	blCheckHead = 0;
	hRoot = TVI_ROOT;
	hItem = NULL;
	m_GCList->Spin_InUse_set();
	RTREE_LChildRChain_Traversal_LINE(COMMAND_GROUP, m_GCList,
		operateNode_t->Spin_InUse_set();
		if (operateNode_t->name.substr(0,2) == "//"){
			Expand(hRoot, TVE_EXPAND);
			blCheckHead = 1;
			hRoot = TVI_ROOT;
			hItem = AddNode(Str_ANSIToUnicode(operateNode_t->name).c_str(), FALSE, hRoot);
		}
		else{
			hItem = AddNode(Str_ANSIToUnicode(Str_IntToString(RTREE_NODE::GetdRNodeID(operateNode_t)) + " : " + operateNode_t->name).c_str(), FALSE, hRoot);
		}
		if (hItem != NULL){
			SetItemData(hItem, RTREE_NODE::GetdRNodeID(operateNode_t));
			if (blCheckHead != 0){
				hRoot = hItem;
				blCheckHead = 0;
			}
		}
		operateNode_t->Spin_InUse_clr();
		//LoadNodeL3(hItem,group);
	);
	m_GCList->Spin_InUse_clr();
	Expand(hRoot,TVE_EXPAND);
	PostMessageW(WM_VSCROLL,SB_TOP,0);
}
//------------------------------------------------------------------------------------------//
void CGCTree::LoadNodeL3(HTREEITEM rootItem,COMMAND_GROUP *group){
	HTREEITEM		hItem;
	std::string		strShowName;

	group->Spin_InUse_set();
	RTREE_LChildRChain_Traversal_LINE(COMMAND_NODE, group,
		operateNode_t->Spin_InUse_set();
		if ((operateNode_t->blSameAsCommand != 0) && (operateNode_t->StrShowName.length() == 0))
			operateNode_t->StrShowName = operateNode_t->StrCommand;
		if ((operateNode_t->blSameAsCommand == 0) && (operateNode_t->StrShowName.length() > 0)){
			hItem = AddNode(Str_ANSIToUnicode(operateNode_t->StrShowName).c_str(), operateNode_t->blEnableSend != 0, rootItem);
		}
		else{
			hItem = AddNode(Str_ANSIToUnicode(operateNode_t->StrCommand).c_str(), operateNode_t->blEnableSend != 0, rootItem);
		}
		if (hItem != NULL)
			SetItemData(hItem, RTREE_NODE::GetdRNodeID(operateNode_t));
		operateNode_t->Spin_InUse_clr();
	);
	group->Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
BOOL CGCTree::DoShowEdit(HTREEITEM hItem){
	COMMAND_GROUP *group;
	COMMAND_NODE *command;
	if (GSDTApp.m_Script.IsStop() == 0)
		return TRUE;
	if (CMyCTreeCtrl::DoShowEdit(hItem)){
		switch(CheckNodeLevel(hItem)){
			case 0:
				break;
			case 1:
				m_GCList->Spin_InUse_set();
				group = GetGroup(hItem);
				if (group != NULL){
					group->Spin_InUse_set();
					if (group->name.substr(0, 2) == "//"){
						m_Edit.SetWindowTextW(Str_ANSIToUnicode(group->name.substr(2)).c_str());
						m_Edit.SetSel(-1);
					}
					group->Spin_InUse_clr();
				}
				m_GCList->Spin_InUse_clr();
				break;
			case 2:
				m_GCList->Spin_InUse_set();
				group = GetGroup(hItem);
				if (group != NULL){
					group->Spin_InUse_set();
					m_Edit.SetWindowTextW(Str_ANSIToUnicode(group->name).c_str());
					m_Edit.SetSel(-1);
					group->Spin_InUse_clr();
				}
				m_GCList->Spin_InUse_clr();
				break;
			case 3:
				m_GCList->Spin_InUse_set();
				command = GetCommand(hItem);
				if (command != NULL){
					command->Spin_InUse_set();
					m_Edit.SetWindowTextW(Str_ANSIToUnicode(command->StrCommand).c_str());
					m_Edit.SetSel(-1);
					command->Spin_InUse_clr();
				}
				m_GCList->Spin_InUse_clr();
				break;
			default:;
		}
		return TRUE;
	}
	return FALSE;
}
//------------------------------------------------------------------------------------------//
void CGCTree::OnEditCheckSave(HTREEITEM hItem){
	INT32	itmeLevel;
	if (GSDTApp.m_Script.IsStop() == 0)
		return;
	itmeLevel = CheckNodeLevel(hItem);
	m_GCList->Spin_InUse_set();
	switch(itmeLevel){
		case 0:
		case 1:
		case 2:
			SaveNodeL2(hItem);
			break;
		case 3:
			SaveNodeCheckL3(hItem);
			break;
		default:;
	}
	SelectItem(hItem);
	m_GCList->Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void CGCTree::SaveNodeCheckL3(HTREEITEM hItem){
	COMMAND_NODE	*command;
	CString			text;
	std::wstring	strwText;
	if (hItem == NULL)
		return;

	command = GetCommand(hItem);

	if (command != NULL){
		command->Spin_InUse_set();
		command->blEnableSend = GetCheck(hItem);
		command->Spin_InUse_clr();
	}
}
//------------------------------------------------------------------------------------------//
void CGCTree::OnEditSave(HTREEITEM hItem){
	INT32	itmeLevel;
	if (GSDTApp.m_Script.IsStop() == 0)
		return;
	itmeLevel = CheckNodeLevel(hItem);
	m_GCList->Spin_InUse_set();
	switch(itmeLevel){
		case 0:
		case 1:
		case 2:
			SaveNodeL2(hItem);
			break;
		case 3:
			SaveNodeL3(hItem);
			break;
		default:;
	}
	SelectItem(hItem);
	m_GCList->Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void CGCTree::SaveNodeL2(HTREEITEM hItem){
	COMMAND_GROUP	*node;
	CString			text;
	std::wstring	strwText;
	if (hItem == NULL)
		return;
	node = (COMMAND_GROUP*)RTREE_NODE::FindInLChildRChainByDRNodeID(m_GCList, GetItemData(hItem));
	if (node != NULL){
		node->Spin_InUse_set();
		text = GetItemText(hItem);
		strwText = text.GetBuffer(0);
		node->name = Str_UnicodeToANSI(strwText);
		node->blEnableAutoRun = GetCheck(hItem);
		if (GetParentItem(hItem) == NULL){
			if (node->name.substr(0,2) != "//"){
				node->name= "//" + node->name;
				text = _T("//") + text;
				SetItemText(m_EditItem,text);
			}
		}
		else{
			if (node->name.substr(0, 2) == "//"){
				node->name = Str_Trim(node->name.substr(2));
				text = Str_ANSIToUnicode(node->name).c_str();
			}
			SetItemText(m_EditItem, Str_ANSIToUnicode(Str_IntToString(RTREE_NODE::GetdRNodeID(node)) + " : " + node->name).c_str());
		}
		node->Spin_InUse_clr();
	}
}
//------------------------------------------------------------------------------------------//
void CGCTree::SaveNodeL3(HTREEITEM hItem){
	COMMAND_NODE	*command;
	CString			text;
	std::wstring	strwText;
	if (hItem == NULL)
		return;

	command = GetCommand(hItem);

	if (command != NULL){
		text = GetItemText(hItem);
		strwText = text;
		command->Spin_InUse_set();
		command->StrCommand = Str_UnicodeToANSI(strwText);
		command->blEnableSend = GetCheck(hItem);
		if (command->blSameAsCommand == 0)
			SetItemText(m_EditItem,Str_ANSIToUnicode(command->StrShowName).c_str());
		command->Spin_InUse_clr();
	}
}
//------------------------------------------------------------------------------------------//
HTREEITEM CGCTree::CreateNode(HTREEITEM hItem){
	HTREEITEM		retItem;
	COMMAND_GROUP	*node;
	INT32	nodeLevel;

	nodeLevel = CheckNodeLevel(hItem);
	m_GCList->Spin_InUse_set();
	do{
		if (nodeLevel < 3){
			retItem = GroupCopy();
			if (retItem != NULL){
				DelGroupSubNode(m_OnShowGroup);
				m_OnShowGroup = retItem;
				LoadNodeL3(retItem, (COMMAND_GROUP*)RTREE_NODE::GetLastChild(m_GCList));
				Expand(retItem, TVE_EXPAND);
				SelectItem(retItem);
				break;
			}
		}

		retItem = NULL;
		switch (nodeLevel){
		case 0:
			retItem = CreateNodeL1();
			break;
		case 1:
			if (ItemHasChildren(hItem)){
				retItem = CreateNodeL1(hItem);
			}
			else{
				retItem = CreateNodeL2(hItem);
			}
			break;
		case 2:
			node = (COMMAND_GROUP*)RTREE_NODE::FindInLChildRChainByDRNodeID(m_GCList, GetItemData(hItem));
			if (node->GetFirstChild(node) != NULL){
				retItem = CreateNodeL2(hItem);
			}
			else{
				if (m_OnShowGroup != NULL){
					DelGroupSubNode(m_OnShowGroup);
					m_OnShowGroup = NULL;
				}
				retItem = CreateNodeL3(hItem);
				m_OnShowGroup = hItem;
			}
			break;
		case 3:
			COMMAND_NODE	*newCommand, *sComand;
			retItem = CreateNodeL3(hItem);
			sComand = GetCommand(hItem);
			newCommand = GetCommand(retItem);
			COMMAND_NODE::CopyCOMMAND_NODE(sComand, newCommand);
			SetItemText(retItem, GetItemText(hItem));
			SetCheck(retItem, GetCheck(hItem));
			break;
		}
		
		if (retItem != NULL)
			Expand(GetParentItem(retItem), TVE_EXPAND);
		SelectItem(retItem);
	} while (0);
	m_GCList->Spin_InUse_clr();
	return(retItem);
}
//------------------------------------------------------------------------------------------//
HTREEITEM CGCTree::GroupCopy(void){
	COMMAND_GROUP	*newGroup, *newNode;
	HTREEITEM hItem1,hItem2;

	hItem1 = NULL;
	hItem2 = NULL;
	newGroup = NULL;
	newNode = NULL;

	RTREE_LChildRChain_Traversal_LINE(COMMAND_GROUP, m_GCList,
		if (operateNode_t->blEnableAutoRun != 0){
			if (newNode == NULL){
				newGroup = new COMMAND_GROUP;
				if (newGroup == NULL){
					break;
				}
				else{
					newNode = new COMMAND_GROUP;
					if (newNode == NULL){
						delete newGroup;
						newGroup = NULL;
						break;
					}
					else{
						hItem1 = AddNode(_T("//New copy group"), 0, TVI_ROOT, TVI_LAST);
						if (hItem1 == NULL){
							delete newNode;
							delete newGroup;
							newNode = NULL;
							newGroup = NULL;
							break;
						}
						else{
							hItem2 = AddNode(_T("New copy command group"), 0, hItem1, TVI_LAST);
							if (hItem2 == NULL){
								DeleteItem(hItem1);
								delete newNode;
								delete newGroup;
								newNode = NULL;
								newGroup = NULL;
								break;
							}
							else{
								COMMAND_GROUP::CopyCOMMAND_GROUP(operateNode_t, newNode, 0);
							}
						}

					}
				}
			}
			else{
				COMMAND_GROUP::CopyCOMMAND_GROUP(operateNode_t, newNode, 0);
			}
		}
	);
	if (newNode != NULL){
		newGroup->name = "//New copy group";
		m_GCList->AddNode(newGroup);
		SetItemData(hItem1, RTREE_NODE::GetdRNodeID(newGroup));

		newNode->name = "New copy command group";
		m_GCList->AddNode(newNode);
		SetItemText(hItem2, Str_ANSIToUnicode(Str_IntToString(RTREE_NODE::GetdRNodeID(newNode)) + " : " + newNode->name).c_str());
		SetItemData(hItem2, RTREE_NODE::GetdRNodeID(newNode));
	}
	return(hItem2);
}
//------------------------------------------------------------------------------------------//
HTREEITEM CGCTree::CreateNodeL1(HTREEITEM hItem){
	//输入为根节点中的一个
	COMMAND_GROUP	*node,*newNode;
	HTREEITEM		retItem;
	node = NULL;
	newNode = NULL;
	retItem = NULL;
	if (hItem == NULL){
		hItem = TVI_LAST;
	}
	else{
		retItem = GetNextItem(hItem,TVGN_NEXT);
		if (retItem == NULL){
			node = NULL;
		}
		else{
			node = (COMMAND_GROUP*)RTREE_NODE::GetPriorBrother(RTREE_NODE::FindInLChildRChainByDRNodeID(m_GCList, GetItemData(retItem)));
		}
	}
	newNode = new COMMAND_GROUP;
	if (newNode != NULL){
		retItem = AddNode(_T("//New group"),0,TVI_ROOT,hItem);
		if (retItem == NULL){
			delete newNode;
		}
		else{
			newNode->name = "//New group";
			if (node == NULL){
				RTREE_NODE::InsertLChild(m_GCList, newNode);
			}
			else{
				RTREE_NODE::InsertRChild(node, newNode);
			}
			SetItemData(retItem, RTREE_NODE::GetdRNodeID(newNode));
		}
	}
	return(retItem);
}
//------------------------------------------------------------------------------------------//
HTREEITEM CGCTree::CreateNodeL2(HTREEITEM hItem){
	COMMAND_GROUP	*newNode, *hNode;
	HTREEITEM		fatherItem,retItem;
	INT32			nodeLevel;

	if (hItem == NULL)
		return NULL;
	retItem = NULL;
	nodeLevel = CheckNodeLevel(hItem);
	if (nodeLevel == 2){//create item after hItem
		fatherItem = GetParentItem(hItem);
		hNode = (COMMAND_GROUP*)RTREE_NODE::FindInLChildRChainByDRNodeID(m_GCList, GetItemData(hItem));
	}
	else{//create item in the last
		fatherItem = hItem;
		hItem = GetNextItem(fatherItem,TVGN_NEXT);
		if (hItem == NULL){
			hNode = (COMMAND_GROUP*)RTREE_NODE::GetLastChild(m_GCList);
		}
		else{
			hNode = (COMMAND_GROUP*)RTREE_NODE::GetPriorBrother(RTREE_NODE::FindInLChildRChainByDRNodeID(m_GCList, GetItemData(hItem)));
		}
		hItem = TVI_LAST;
	}

	newNode = new COMMAND_GROUP;
	if (newNode != NULL){
		retItem = AddNode(_T("New command group"),0,fatherItem,hItem);
		if (retItem == NULL){
			delete newNode;
		}
		else{
			newNode->name = "New command group";
			RTREE_NODE::InsertRChild(hNode, newNode);
			SetItemText(retItem, Str_ANSIToUnicode(Str_IntToString(RTREE_NODE::GetdRNodeID(newNode)) + " : " + newNode->name).c_str());
			SetItemData(retItem, RTREE_NODE::GetdRNodeID(newNode));
		}
	}
	return(retItem);
}
//------------------------------------------------------------------------------------------//
HTREEITEM CGCTree::CreateNodeL3(HTREEITEM hItem){
	HTREEITEM		retItem,fatherItem;
	COMMAND_GROUP	*fatherNode;
	COMMAND_NODE	*newCommand,*hNode;
	INT32			nodeLevel;
	if (hItem == NULL)
		return NULL;

	retItem = NULL;
	nodeLevel = CheckNodeLevel(hItem);
	if (nodeLevel == 3){//create item after hItem
		fatherItem = GetParentItem(hItem);
		fatherNode = (COMMAND_GROUP*)RTREE_NODE::FindInLChildRChainByDRNodeID(m_GCList, GetItemData(fatherItem));
		hNode = (COMMAND_NODE*)RTREE_NODE::FindInLChildRChainByDRNodeID(fatherNode, GetItemData(hItem));
	}
	else{//create item in the last
		fatherItem = hItem;
		hItem = TVI_LAST;
		fatherNode = (COMMAND_GROUP*)RTREE_NODE::FindInLChildRChainByDRNodeID(m_GCList, GetItemData(fatherItem));
		hNode = (COMMAND_NODE*)RTREE_NODE::GetLastChild(fatherNode);
	}
	newCommand = new COMMAND_NODE;
	if (newCommand != NULL){
		retItem = AddNode(_T("New command"), newCommand->blEnableSend, fatherItem, hItem);
		if (retItem == NULL){
			delete newCommand;
		}
		else{
			newCommand->StrCommand = "New command";
			RTREE_NODE::InsertRChild(hNode, newCommand);
			SetItemData(retItem, RTREE_NODE::GetdRNodeID(newCommand));
		}
	}
	return(retItem);
}
//------------------------------------------------------------------------------------------//
HTREEITEM CGCTree::DelNode(HTREEITEM delItem){
	INT32	nodeLevel;
	if (delItem == NULL)
		return(delItem);
	m_GCList->Spin_InUse_set();
	nodeLevel = CheckNodeLevel(delItem);
	switch(nodeLevel){
		case 1:
			delItem = DelNodeL1(delItem);
			break;
		case 2:
			delItem = DelNodeL2(delItem);
			break;
		case 3:
			delItem = DelNodeL3(delItem);
			break;
		default:;
	}
	RTREE_NODE::CleanTrash(m_GCList);
	SelectItem(delItem);
	m_GCList->Spin_InUse_clr();
	return(delItem);
}
//------------------------------------------------------------------------------------------//
HTREEITEM CGCTree::DelNodeL1(HTREEITEM delItem){
	HTREEITEM		nextItem;
	COMMAND_GROUP	*delNode,*endNode;

	if (delItem == NULL)
		return NULL;
	if (delItem == m_SelectItem)
		m_SelectItem = NULL;
	if (delItem == m_HotItem)
		m_HotItem = NULL;

	delNode = (COMMAND_GROUP*)RTREE_NODE::FindInLChildRChainByDRNodeID(m_GCList,GetItemData(delItem));
	nextItem = GetNextItem(delItem,TVGN_NEXT);
	if (nextItem == NULL){
		endNode = (COMMAND_GROUP*)RTREE_NODE::GetLastChild(m_GCList);
		nextItem = GetNextItem(delItem,TVGN_PREVIOUS);
	}
	else{
		endNode = (COMMAND_GROUP*)RTREE_NODE::FindInLChildRChainByDRNodeID(m_GCList, GetItemData(nextItem));
		endNode = (COMMAND_GROUP*)RTREE_NODE::GetPriorBrother(endNode);
	}
	RTREE_NODE::MoveNodesToTrash(delNode, endNode, m_GCList);
	DeleteItem(delItem);
	return(nextItem);
}
//------------------------------------------------------------------------------------------//
HTREEITEM CGCTree::DelNodeL2(HTREEITEM delItem){
	HTREEITEM		nextItem;
	COMMAND_GROUP	*delNode;

	if (delItem == NULL)
		return NULL;
	if (delItem == m_SelectItem)
		m_SelectItem = NULL;
	if (delItem == m_HotItem)
		m_HotItem = NULL;

	nextItem = GetNextItem(delItem,TVGN_NEXT);
	if (nextItem == NULL)
		nextItem = GetNextItem(delItem,TVGN_PREVIOUS);
	if (nextItem == NULL)
		nextItem = GetParentItem(delItem);
	delNode = (COMMAND_GROUP*)RTREE_NODE::FindInLChildRChainByDRNodeID(m_GCList, GetItemData(delItem));
	RTREE_NODE::MoveNodeToTrash(delNode, m_GCList);
	DeleteItem(delItem);
	return(nextItem);
}
//------------------------------------------------------------------------------------------//
HTREEITEM CGCTree::DelNodeL3(HTREEITEM delItem){
	HTREEITEM		nextItem,fatherItem;
	COMMAND_GROUP	*fatherNode;
	COMMAND_NODE	*delCommand;
	if (delItem == NULL)
		return NULL;
	if (delItem == m_SelectItem)
		m_SelectItem = NULL;
	if (delItem == m_HotItem)
		m_HotItem = NULL;

	nextItem = GetNextItem(delItem,TVGN_NEXT);
	if (nextItem == NULL)
		nextItem = GetNextItem(delItem,TVGN_PREVIOUS);
	if (nextItem == NULL)
		nextItem = GetParentItem(delItem);

	fatherItem = GetParentItem(delItem);
	fatherNode = (COMMAND_GROUP*)RTREE_NODE::FindInLChildRChainByDRNodeID(m_GCList, GetItemData(fatherItem));
	delCommand = (COMMAND_NODE*)RTREE_NODE::FindInLChildRChainByDRNodeID(fatherNode, GetItemData(delItem));

	RTREE_NODE::MoveNodeToTrash(delCommand, m_GCList);
	DeleteItem(delItem);
	return(nextItem);
}
//------------------------------------------------------------------------------------------//
HTREEITEM CGCTree::UpNode(HTREEITEM moveItem){
	INT32	nodeLevel;
	if (moveItem == NULL)
		return(moveItem);

	nodeLevel = CheckNodeLevel(moveItem);
	switch(nodeLevel){
		case 1:
			moveItem = UpNodeL1(moveItem);
			break;
		case 2:
			moveItem = UpNodeL2(moveItem);
			break;
		case 3:
			moveItem = UpNodeL3(moveItem);
			break;
		default:;
	}
	m_GCList->Spin_InUse_set();
	SelectItem(moveItem);
	m_GCList->Spin_InUse_clr();
	return(moveItem);
}
//------------------------------------------------------------------------------------------//
HTREEITEM CGCTree::UpNodeL1(HTREEITEM moveItem){
	HTREEITEM	PriorItem;
	UINT		blExpanded;
	if (moveItem == NULL)
		return(moveItem);

	PriorItem = GetNextItem(moveItem,TVGN_PREVIOUS);
	if (PriorItem == NULL)
		return(moveItem);
	blExpanded = TVIS_EXPANDED & GetItemState(moveItem,TVIS_EXPANDED);
	PriorItem = DownNode(PriorItem);
	moveItem = GetNextItem(PriorItem,TVGN_PREVIOUS);
	if (blExpanded > 0)
		Expand(moveItem,TVE_EXPAND);
	return(moveItem);
}
//------------------------------------------------------------------------------------------//
HTREEITEM CGCTree::UpNodeL2(HTREEITEM moveItem){
	HTREEITEM		PriorItem,fatherItem;
	COMMAND_GROUP	*moveNode,*priorNode;
	if (moveItem == NULL)
		return(moveItem);

	PriorItem = GetNextItem(moveItem,TVGN_PREVIOUS);
	if (PriorItem != NULL){
		PriorItem = DownNode(PriorItem);
		moveItem = GetNextItem(PriorItem,TVGN_PREVIOUS);
	}
	else{//moveItem it the first item in this group
		m_GCList->Spin_InUse_set();
		moveNode = (COMMAND_GROUP*)RTREE_NODE::FindInLChildRChainByDRNodeID(m_GCList, GetItemData(moveItem));
		priorNode = (COMMAND_GROUP*)RTREE_NODE::GetPriorBrother(moveNode);
		if (priorNode != (COMMAND_GROUP*)RTREE_NODE::GetFirstChild(m_GCList)){//lv2 item cannot move to the first of link
			fatherItem = GetParentItem(moveItem);
			fatherItem = GetNextItem(fatherItem, TVGN_PREVIOUS);
			RTREE_NODE::MoveNodesUpInRChain(moveNode);
			DeleteItem(moveItem);
			moveItem = AddNode(Str_ANSIToUnicode(Str_IntToString(RTREE_NODE::GetdRNodeID(moveNode)) + " : " + moveNode->name).c_str(), moveNode->blEnableAutoRun, fatherItem);
			if (moveItem != NULL)
				SetItemData(moveItem, RTREE_NODE::GetdRNodeID(moveNode));
		}
		m_GCList->Spin_InUse_clr();
	}
	return(moveItem);
}
//------------------------------------------------------------------------------------------//
HTREEITEM CGCTree::UpNodeL3(HTREEITEM moveItem){
	HTREEITEM	PriorItem;

	if (moveItem == NULL)
		return(moveItem);

	PriorItem = GetNextItem(moveItem,TVGN_PREVIOUS);
	if (PriorItem == NULL)
		return(moveItem);
	PriorItem = DownNode(PriorItem);
	moveItem = GetNextItem(PriorItem,TVGN_PREVIOUS);
	return(moveItem);
}
//------------------------------------------------------------------------------------------//
HTREEITEM CGCTree::DownNode(HTREEITEM moveItem){
	INT32	nodeLevel;
	if (moveItem == NULL)
		return(moveItem);
	m_GCList->Spin_InUse_set();
	nodeLevel = CheckNodeLevel(moveItem);
	switch(nodeLevel){
		case 1:
			moveItem = DownNodeL1(moveItem);
			break;
		case 2:
			moveItem = DownNodeL2(moveItem);
			break;
		case 3:
			moveItem = DownNodeL3(moveItem);
			break;
		default:;
	}
	SelectItem(moveItem);
	m_GCList->Spin_InUse_clr();
	return(moveItem);
}
//------------------------------------------------------------------------------------------//
HTREEITEM CGCTree::DownNodeL1(HTREEITEM moveItem){
	HTREEITEM		endItem,nextItem;
	COMMAND_GROUP	*insertNode,*endNode,*moveNode;
	UINT			blExpanded;
	if (moveItem == NULL)
		return(moveItem);

	blExpanded = TVIS_EXPANDED & GetItemState(moveItem,TVIS_EXPANDED);

	nextItem = GetNextItem(moveItem,TVGN_NEXT);
	if (nextItem == NULL)//moveItem is the last
		return(moveItem);

	moveNode = (COMMAND_GROUP*)RTREE_NODE::FindInLChildRChainByDRNodeID(m_GCList, GetItemData(moveItem));
	endNode = (COMMAND_GROUP*)RTREE_NODE::GetPriorBrother(RTREE_NODE::FindInLChildRChainByDRNodeID(m_GCList, GetItemData(nextItem)));

	endItem = GetNextItem(nextItem,TVGN_NEXT);
	if (endItem == NULL){
		insertNode = (COMMAND_GROUP*)RTREE_NODE::GetLastChild(m_GCList);
	}
	else{
		insertNode = (COMMAND_GROUP*)RTREE_NODE::GetPriorBrother(RTREE_NODE::FindInLChildRChainByDRNodeID(m_GCList, GetItemData(endItem)));
	}
	RTREE_NODE::MoveNodesAfterInRChain(moveNode, endNode, insertNode);

	DeleteItem(moveItem);
	moveNode->Spin_InUse_set();
	moveItem = AddNode(Str_ANSIToUnicode(moveNode->name).c_str(), 0, TVI_ROOT, nextItem);
	moveNode->Spin_InUse_clr();
	if (moveItem != NULL){
		moveNode->Spin_InUse_set();
		SetItemData(moveItem, RTREE_NODE::GetdRNodeID(moveNode));
		moveNode->Spin_InUse_clr();
		RTREE_RChain_Traversal_LINE(COMMAND_GROUP, moveNode,
			if (operateNode_t == endNode)
				break;
			if (nextNode_t != NULL){
				nextNode_t->Spin_InUse_set();
				nextItem = AddNode(Str_ANSIToUnicode(Str_IntToString(RTREE_NODE::GetdRNodeID(nextNode_t)) + " : " + nextNode_t->name).c_str(), nextNode_t->blEnableAutoRun, moveItem);
				SetItemData(nextItem, RTREE_NODE::GetdRNodeID(nextNode_t));
				nextNode_t->Spin_InUse_clr();
			}
		);
	}

	if ((blExpanded > 0) && (moveItem != NULL))
		Expand(moveItem,TVE_EXPAND);
	return(moveItem);
}
//------------------------------------------------------------------------------------------//
HTREEITEM CGCTree::DownNodeL2(HTREEITEM moveItem){
	HTREEITEM		fatherItem,fatherNextItem,nextItem;
	COMMAND_GROUP	*moveNode;

	if (moveItem == NULL)
		return(moveItem);
	fatherItem = GetParentItem(moveItem);
	fatherNextItem = GetNextItem(fatherItem,TVGN_NEXT);//father next item
	moveNode = (COMMAND_GROUP*)RTREE_NODE::FindInLChildRChainByDRNodeID(m_GCList, GetItemData(moveItem));
	if (fatherNextItem == NULL){//moveItem in the last group
		if (moveNode == (COMMAND_GROUP*)RTREE_NODE::GetLastChild(m_GCList))//moveItem is the last item;
			return(moveItem);
	}
	nextItem = GetNextItem(moveItem,TVGN_NEXT);
	moveNode->Spin_InUse_set();
	if (nextItem == NULL){//hItem is the last item in the group;
		nextItem = GetNextItem(fatherNextItem,TVGN_CHILD);
		RTREE_NODE::MoveNodesDownInRChain(moveNode);
		RTREE_NODE::MoveNodesDownInRChain(moveNode);//move to next group second node.

		DeleteItem(moveItem);
		moveItem = AddNode(Str_ANSIToUnicode(Str_IntToString(RTREE_NODE::GetdRNodeID(moveNode)) + " : " + moveNode->name).c_str(), moveNode->blEnableAutoRun, fatherNextItem, nextItem);
		if (moveItem != NULL)
			SetItemData(moveItem, RTREE_NODE::GetdRNodeID(moveNode));
		DownNodeL2(nextItem);
		moveItem = GetNextItem(fatherNextItem,TVGN_CHILD);
	}
	else{
		RTREE_NODE::MoveNodesDownInRChain(moveNode);
		DeleteItem(moveItem);
		moveItem = AddNode(Str_ANSIToUnicode(Str_IntToString(RTREE_NODE::GetdRNodeID(moveNode)) + " : " + moveNode->name).c_str(), moveNode->blEnableAutoRun, fatherItem, nextItem);
		if (moveItem != NULL)
			SetItemData(moveItem, RTREE_NODE::GetdRNodeID(moveNode));
	}
	moveNode->Spin_InUse_clr();
	return(moveItem);
}
//------------------------------------------------------------------------------------------//
HTREEITEM CGCTree::DownNodeL3(HTREEITEM moveItem){
	HTREEITEM		nextItem,fatherItem;
	COMMAND_GROUP	*fatherNode;
	COMMAND_NODE	*moveCommand;
	if (moveItem == NULL)
		return(moveItem);

	fatherItem = GetParentItem(moveItem);
	fatherNode = (COMMAND_GROUP*)RTREE_NODE::FindInLChildRChainByDRNodeID(m_GCList, GetItemData(fatherItem));
	moveCommand = (COMMAND_NODE*)RTREE_NODE::FindInLChildRChainByDRNodeID(fatherNode, GetItemData(moveItem));
	if (moveCommand == (COMMAND_NODE*)RTREE_NODE::GetLastChild(fatherNode)){//last item
		return(moveItem);
	}
	RTREE_NODE::MoveNodesDownInRChain(moveCommand);

	nextItem = GetNextItem(moveItem,TVGN_NEXT);
	DeleteItem(moveItem);
	moveCommand->Spin_InUse_set();
	if (moveCommand->blSameAsCommand == 0){
		moveItem = AddNode(Str_ANSIToUnicode(moveCommand->StrShowName).c_str(),moveCommand->blEnableSend,fatherItem,nextItem);
	}
	else{
		moveItem = AddNode(Str_ANSIToUnicode(moveCommand->StrCommand).c_str(),moveCommand->blEnableSend,fatherItem,nextItem);
	}
	if (moveItem != NULL)
		SetItemData(moveItem, RTREE_NODE::GetdRNodeID(moveCommand));
	
	moveCommand->Spin_InUse_clr();
	return(moveItem);
}
//------------------------------------------------------------------------------------------//
void CGCTree::OnLButtonDown(UINT nFlags, CPoint point){
	HTREEITEM		hItem;
	CMyCTreeCtrl::OnLButtonDown(nFlags,point);
	hItem = HitTest(point);
	m_GCList->Spin_InUse_set();
	if (CheckNodeLevel(hItem) == 2){
		COMMAND_GROUP *group = GetGroup(hItem);
		if (group != NULL){
			group->Spin_InUse_set();
			SetCheck(hItem, group->blEnableAutoRun);
			group->Spin_InUse_clr();
		}
	}
	if (CheckNodeLevel(hItem) == 3){
		COMMAND_NODE *command = GetCommand(hItem);
		if (command != NULL){
			command->Spin_InUse_set();
			SetCheck(hItem, command->blEnableSend);
			command->Spin_InUse_clr();
		}
	}
	m_GCList->Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
void CGCTree::OnLButtonDblClk(UINT nFlags, CPoint point){
	HTREEITEM		hItem;
	int32			ret;
	COMMAND_GROUP	*group;
	COMMAND_NODE	*command;

	hItem = HitTest(point);
	switch(CheckNodeLevel(hItem)){
		case 0:
		case 1:
			CMyCTreeCtrl::OnLButtonDblClk(nFlags,point);
			break;
		case 2:
			//if (GSDTApp.m_Device.CheckblConnect() != 0){
				m_GCList->Spin_InUse_set();
				group = GetGroup(hItem);
				if (group != NULL)
					group->Spin_InUse_set();
					ret = GSDTApp.m_Script.Execute(&GSDTApp.m_Device, group);
					group->Spin_InUse_clr();
				if (ret == 0){
					AfxMessageBox(_T("Script is running!"));
				}
				else{
					g_wndGCPropList.EnableWindow(FALSE);
				}
				m_GCList->Spin_InUse_clr();
			//}
			break;
		case 3:
			//if (GSDTApp.m_Device.CheckblConnect() != 0){
				m_GCList->Spin_InUse_set();
				command = GetCommand(hItem);
				if (command != NULL){
					COMMAND_NODE::Init(&m_CommandNode);
					COMMAND_NODE::CopyCOMMAND_NODE(command, &m_CommandNode);
					if (nFlags & MK_CONTROL){
						m_CommandNode.blEnableSend = 1;
						if (GSDTApp.m_Script.Execute(&GSDTApp.m_Device,&m_CommandNode) == 0)
							AfxMessageBox(_T("Script is running!"));
					}
					else{
						GSDTApp.m_Device.SendCommand(m_CommandNode.StrCommand, m_CommandNode.blEnableHEX, m_CommandNode.blEnableSendCR, G_ESCAPE_ON);
					}
				}
				m_GCList->Spin_InUse_clr();
			//}
			break;
		default:;
	}
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.hwndTrack = GetSafeHwnd();
	tme.dwFlags = TME_CANCEL | TME_HOVER;
	tme.dwHoverTime = 300;
	_TrackMouseEvent(&tme);
}
//------------------------------------------------------------------------------------------//
void CGCTree::OnRButtonDown(UINT nFlags, CPoint point){
	HTREEITEM		hItem,pItem;
	COMMAND_GROUP	*group;
	hItem = HitTest(point);
	pItem = GetParentItem(hItem);
	CMyCTreeCtrl::OnRButtonDown(nFlags,point);
	if ((hItem != NULL) && (pItem != NULL)){
		if ((m_OnShowGroup != hItem) && (m_OnShowGroup != pItem)){
			m_GCList->Spin_InUse_set();
			group = (COMMAND_GROUP*)RTREE_NODE::FindInLChildRChainByDRNodeID(m_GCList, GetItemData(hItem));
			if (group != NULL){
				Expand(hItem,TVE_COLLAPSE);
				DelGroupSubNode(m_OnShowGroup);
				m_OnShowGroup = hItem;
				LoadNodeL3(hItem,group);
				Expand(hItem,TVE_EXPAND);
				SelectItem(hItem);
			}
			m_GCList->Spin_InUse_clr();
		}
		else if (m_OnShowGroup == hItem){
			Expand(hItem,TVE_COLLAPSE);
			DelGroupSubNode(m_OnShowGroup);
			m_OnShowGroup = NULL;
			
		}
	}
}
//------------------------------------------------------------------------------------------//
void CGCTree::OnRButtonDblClk(UINT nFlags, CPoint point){
	HTREEITEM		hItem;
	hItem = HitTest(point);
	if (CheckNodeLevel(hItem) == 3){
		hItem = GetParentItem(hItem);
		if (m_OnShowGroup != NULL){
			DelGroupSubNode(m_OnShowGroup);
			m_OnShowGroup = NULL;
		}
		m_GCList->Spin_InUse_set();
		SelectItem(hItem);
		m_GCList->Spin_InUse_clr();
	}
}
//------------------------------------------------------------------------------------------//
BOOL CGCTree::SelectItem(HTREEITEM hItem){
	UpdataDock(hItem, 0);
	return(CMyCTreeCtrl::SelectItem(hItem));
}
//------------------------------------------------------------------------------------------//
void CGCTree::UpdataDock(HTREEITEM hItem, int32 blenforce){
	INT32	nodeLevel;

	nodeLevel = CheckNodeLevel(hItem);

	switch (nodeLevel){
	case 0:
	case 1:
			g_wndGCPropList.LoadInit();
			g_wndGCCommandList.LoadData(NULL, blenforce);
		break;
	case 2:
			g_wndGCPropList.LoadGroup(hItem, GetGroup(hItem), blenforce);
			g_wndGCCommandList.LoadData(GetGroup(hItem), blenforce);
		break;
	case 3:
			g_wndGCPropList.LoadCommand(hItem, GetCommand(hItem), blenforce);
			g_wndGCCommandList.LoadData(GetGroup(GetParentItem(hItem)), blenforce);
		break;
	default:
		break;
	}
}
//------------------------------------------------------------------------------------------//
COMMAND_GROUP *CGCTree::GetGroup(HTREEITEM hItem){
	if (hItem == NULL)
		return NULL;

	if (CheckNodeLevel(hItem) < 3)
		return ((COMMAND_GROUP*)RTREE_NODE::FindInLChildRChainByDRNodeID(m_GCList, GetItemData(hItem)));
	return NULL;
}
//------------------------------------------------------------------------------------------//
COMMAND_NODE *CGCTree::GetCommand(HTREEITEM hItem){
	HTREEITEM		fatherItem;
	COMMAND_GROUP	*fatherNode;
	COMMAND_NODE	*command;

	if (hItem == NULL)
		return NULL;
	if (CheckNodeLevel(hItem) < 3)
		return NULL;

	fatherItem = GetParentItem(hItem);
	fatherNode = (COMMAND_GROUP*)RTREE_NODE::FindInLChildRChainByDRNodeID(m_GCList, GetItemData(fatherItem));
	command = (COMMAND_NODE*)RTREE_NODE::FindInLChildRChainByDRNodeID(fatherNode, GetItemData(hItem));
	return(command);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class CGCViewMenuButton : public CMFCToolBarMenuButton{
	friend class CClassView;
	DECLARE_SERIAL(CGCViewMenuButton)
public:
	CGCViewMenuButton(HMENU hMenu = NULL) : CMFCToolBarMenuButton((UINT)-1, hMenu, -1){
	}
	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE){
		pImages = CMFCToolBar::GetImages();
		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);
		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);
		pImages->EndDrawImage(ds);
	}
};
IMPLEMENT_SERIAL(CGCViewMenuButton, CMFCToolBarMenuButton, 1)
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CGCViewDP::CGCViewDP(void){
}
//------------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CGCViewDP, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_SETFOCUS()
	ON_WM_PAINT()

	ON_COMMAND(ID_TOOLBAR_GC_ADD, &CGCViewDP::OnGC_Add)
	ON_COMMAND(ID_TOOLBAR_GC_DEL, &CGCViewDP::OnGC_Del)
	ON_COMMAND(ID_TOOLBAR_GC_UP, &CGCViewDP::OnGC_Up)
	ON_COMMAND(ID_TOOLBAR_GC_DOWN, &CGCViewDP::OnGC_Down)
	ON_COMMAND(ID_TOOLBAR_GC_EXPAND, &CGCViewDP::OnGC_Expand)
	ON_COMMAND(ID_TOOLBAR_GC_COLLAPSE, &CGCViewDP::OnGC_Collapse)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_GC_ADD,&CGCViewDP::OnUpdateGC_Add)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_GC_DEL,&CGCViewDP::OnUpdateGC_Del)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_GC_UP,&CGCViewDP::OnUpdateGC_Up)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_GC_DOWN,&CGCViewDP::OnUpdateGC_Down)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_GC_EDIT,&CGCViewDP::OnUpdateGC_Edit)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_GC_LOAD,&CGCViewDP::OnUpdateGC_LOAD)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_GC_IMPORT,&CGCViewDP::OnUpdateGC_IMPORT)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_GC_SAVE,&CGCViewDP::OnUpdateGC_SAVE)

	ON_COMMAND(ID_TOOLBAR_GC_LOAD, &CGCViewDP::OnButtonClickGC_LOAD)
	ON_COMMAND(ID_TOOLBAR_GC_IMPORT,&CGCViewDP::OnButtonClickGC_IMPORT)
	ON_COMMAND(ID_TOOLBAR_GC_SAVE, &CGCViewDP::OnButtonClickGC_SAVE)
	ON_COMMAND(ID_TOOLBAR_GC_RUN, &CGCViewDP::OnButtonClickGC_Run)
	ON_COMMAND(ID_TOOLBAR_GC_STOP, &CGCViewDP::OnButtonClickGC_STOP)
END_MESSAGE_MAP()
//------------------------------------------------------------------------------------------//
CGCViewDP::~CGCViewDP(void){
}
//------------------------------------------------------------------------------------------//
int CGCViewDP::OnCreate(LPCREATESTRUCT lpCreateStruct){
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (CreateGCTree() == -1)
		return -1;
	if (CreateGCToolBar() == -1)
		return -1;
	return 0;
}
//------------------------------------------------------------------------------------------//
void CGCViewDP::AdjustLayout(){
	if (GetSafeHwnd() == NULL)
		return;

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	g_wndGCTree.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}
//------------------------------------------------------------------------------------------//
void CGCViewDP::OnSize(UINT nType, int cx, int cy){
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}
//------------------------------------------------------------------------------------------//
void CGCViewDP::OnSetFocus(CWnd* pOldWnd){
	CDockablePane::OnSetFocus(pOldWnd);
	g_wndGCTree.SetFocus();
}
//------------------------------------------------------------------------------------------//
void CGCViewDP::OnPaint(void){
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	g_wndGCTree.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}
//------------------------------------------------------------------------------------------//
void CGCViewDP::OnContextMenu(CWnd* pWnd, CPoint point){
	CTreeCtrl* pWndTree = (CTreeCtrl*)&g_wndGCTree;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree){
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}
}

//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int CGCViewDP::CreateGCToolBar(void){
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_TOOLBAR_GC);
	m_wndToolBar.LoadToolBar(IDR_TOOLBAR_GC, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);
	
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);// All commands will be routed via this control , not via the parent frame:

	CMenu menuSort;
	menuSort.LoadMenu(IDR_POPUP_GC);
	m_wndToolBar.ReplaceButton(ID_TOOLBAR_GC_LOAD, CGCViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));
	CGCViewMenuButton* pButton = DYNAMIC_DOWNCAST(CGCViewMenuButton, m_wndToolBar.GetButton(10));
	if (pButton != NULL){
		pButton->m_bText = FALSE;
		pButton->m_bImage = TRUE;
		pButton->SetImage(0);
		pButton->SetMessageWnd(this);
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
void CGCViewDP::OnChangeVisualStyle(void){
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_GCViewBarButton_hc : IDR_TOOLBAR_GC, 0, 0, TRUE /* Locked */);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int CGCViewDP::CreateGCTree(void){
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!g_wndGCTree.Create(WS_CHILD | WS_VISIBLE, rectDummy, this, 0)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	g_wndGCTree.LoadData(&GSDTApp.m_GCList);
	return 0;
}
//------------------------------------------------------------------------------------------//
void CGCViewDP::OnGC_Add(void){
	g_wndGCTree.CreateNode(g_wndGCTree.GetSelectedNode());
}
//------------------------------------------------------------------------------------------//
void CGCViewDP::OnGC_Del(void){
	g_wndGCTree.DelNode(g_wndGCTree.GetSelectedNode());
}
//------------------------------------------------------------------------------------------//
void CGCViewDP::OnGC_Up(void){
	g_wndGCTree.UpNode(g_wndGCTree.GetSelectedNode());
}
//------------------------------------------------------------------------------------------//
void CGCViewDP::OnGC_Down(void){
	g_wndGCTree.DownNode(g_wndGCTree.GetSelectedNode());
}
//------------------------------------------------------------------------------------------//
void CGCViewDP::OnGC_Expand(void){
	g_wndGCTree.ExpandAllRoot();
}
//------------------------------------------------------------------------------------------//
void CGCViewDP::OnGC_Collapse(void){
	g_wndGCTree.CollapseAllRoot();
}
//------------------------------------------------------------------------------------------//
void CGCViewDP::OnUpdateGC_Add(CCmdUI *pCmdUI){
	pCmdUI->Enable(GSDTApp.m_Script.IsStop());
}
//------------------------------------------------------------------------------------------//
void CGCViewDP::OnUpdateGC_Del(CCmdUI *pCmdUI){
	pCmdUI->Enable(GSDTApp.m_Script.IsStop());
}
//------------------------------------------------------------------------------------------//
void CGCViewDP::OnUpdateGC_Up(CCmdUI *pCmdUI){
	pCmdUI->Enable(GSDTApp.m_Script.IsStop());
}
//------------------------------------------------------------------------------------------//
void CGCViewDP::OnUpdateGC_Down(CCmdUI *pCmdUI){
	pCmdUI->Enable(GSDTApp.m_Script.IsStop());
}
//------------------------------------------------------------------------------------------//
void CGCViewDP::OnUpdateGC_Edit(CCmdUI *pCmdUI){
	pCmdUI->Enable(TRUE);
	g_wndGCPropList.EnableWindow(GSDTApp.m_Script.IsStop());
}
//------------------------------------------------------------------------------------------//
void CGCViewDP::OnUpdateGC_LOAD(CCmdUI *pCmdUI){
	pCmdUI->Enable(GSDTApp.m_Script.IsStop());
}
//------------------------------------------------------------------------------------------//
void CGCViewDP::OnUpdateGC_IMPORT(CCmdUI *pCmdUI){
	pCmdUI->Enable(GSDTApp.m_Script.IsStop());
}
//------------------------------------------------------------------------------------------//
void CGCViewDP::OnUpdateGC_SAVE(CCmdUI *pCmdUI){
	pCmdUI->Enable(GSDTApp.m_Script.IsStop());
}
//------------------------------------------------------------------------------------------//
void CGCViewDP::OnButtonClickGC_LOAD(void){
	CString	cFileName;
	TCHAR	path[MAX_PATH];  
	uint32			readType;
	std::wstring	strwText;

	GetCurrentDirectory(MAX_PATH,path);
	CFileDialog hFileDlg(TRUE,_T("ini"),_T("*.ini")
		,OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_READONLY|OFN_NOCHANGEDIR
		,_T("definition file(*.ini)|*.ini|all(*.*)|*.*||"),NULL);
	hFileDlg.m_ofn.nFilterIndex = 1;
	hFileDlg.m_ofn.hwndOwner = GetSafeHwnd();
	hFileDlg.m_ofn.lStructSize = sizeof(OPENFILENAME);
	hFileDlg.m_ofn.lpstrTitle = _T("Open");
	hFileDlg.m_ofn.nMaxFile = MAX_PATH;
	hFileDlg.m_ofn.lpstrInitialDir = path;
	SetCurrentDirectory(path);

	if(hFileDlg.DoModal() == IDOK){
		cFileName = hFileDlg.GetPathName();
		strwText = cFileName.GetBuffer(0);
		readType = READINITYPE_NONE | READINITYPE_AddGroupCommand | READINITYPE_CoverGroupCommand;
		GSDTApp.m_ParRecord.ReadScriptFromIniFile(Str_UnicodeToANSI(strwText).c_str(),readType);
		g_wndGCTree.LoadData(&GSDTApp.m_GCList);
	}
}
//------------------------------------------------------------------------------------------//
void CGCViewDP::OnButtonClickGC_IMPORT(void){
	CString	cFileName;
	TCHAR	path[MAX_PATH];  
	uint32			readType;
	std::wstring	strwText;

	GetCurrentDirectory(MAX_PATH,path);
	CFileDialog hFileDlg(TRUE,_T("ini"),_T("*.ini")
		,OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_READONLY|OFN_NOCHANGEDIR
		,_T("definition file(*.ini)|*.ini|all(*.*)|*.*||"),NULL);
	hFileDlg.m_ofn.nFilterIndex = 1;
	hFileDlg.m_ofn.hwndOwner = GetSafeHwnd();
	hFileDlg.m_ofn.lStructSize = sizeof(OPENFILENAME);
	hFileDlg.m_ofn.lpstrTitle = _T("Import");
	hFileDlg.m_ofn.nMaxFile = MAX_PATH;
	hFileDlg.m_ofn.lpstrInitialDir = path;
	SetCurrentDirectory(path);

	if(hFileDlg.DoModal() == IDOK){
		cFileName = hFileDlg.GetPathName();
		strwText = cFileName.GetBuffer(0);
		readType = READINITYPE_NONE | READINITYPE_AddGroupCommand;
		GSDTApp.m_ParRecord.ReadScriptFromIniFile(Str_UnicodeToANSI(strwText).c_str(),readType);
		g_wndGCTree.LoadData(&GSDTApp.m_GCList);
	}
}
//------------------------------------------------------------------------------------------//
void CGCViewDP::OnButtonClickGC_SAVE(void){
	CString	cFileName;
	TCHAR	path[MAX_PATH];  
	uint32			readType;
	std::wstring	strwText;

	GetCurrentDirectory(MAX_PATH,path);
	CFileDialog hFileDlg(FALSE,_T("ini"),_T("*.ini")
		,OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_READONLY|OFN_NOCHANGEDIR
		,_T("definition file(*.ini)|*.ini|all(*.*)|*.*||"),NULL);
	hFileDlg.m_ofn.nFilterIndex = 1;
	hFileDlg.m_ofn.hwndOwner = GetSafeHwnd();
	hFileDlg.m_ofn.lStructSize = sizeof(OPENFILENAME);
	hFileDlg.m_ofn.lpstrTitle = _T("Save");
	hFileDlg.m_ofn.nMaxFile = MAX_PATH;
	hFileDlg.m_ofn.lpstrInitialDir = path;
	SetCurrentDirectory(path);

	if(hFileDlg.DoModal() == IDOK){
		cFileName = hFileDlg.GetPathName();
		strwText = cFileName.GetBuffer(0);
		readType = READINITYPE_NONE | READINITYPE_AddGroupCommand;
		GSDTApp.m_ParRecord.WriteScriptToIniFile(Str_UnicodeToANSI(strwText),readType);
	}
}
//------------------------------------------------------------------------------------------//
void CGCViewDP::OnButtonClickGC_STOP(void){
	GSDTApp.m_Script.StopRun();
}
//------------------------------------------------------------------------------------------//
void CGCViewDP::OnButtonClickGC_Run(void){
	int32	ret;
	//if (GSDTApp.m_Device.CheckblConnect() != 0){
		ret = GSDTApp.m_Script.Execute(&GSDTApp.m_Device,&GSDTApp.m_GCList);
		if (ret == 0){
			AfxMessageBox(_T("Script is running!"));
		}
		else{
			g_wndGCPropList.EnableWindow(FALSE);
		}
	//}
}
//------------------------------------------------------------------------------------------//
