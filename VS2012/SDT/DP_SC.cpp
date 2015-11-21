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
#include "DP_SC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//------------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CSCTree,CMyCTreeCtrl)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void CSCTree::LoadData(SC_LIST *tSCList){
	HTREEITEM	hRoot,hItem;
	int32		blCheckHead;
	std::string	strTest;
	
	DeleteAllItems();
	m_SCList = tSCList;
	blCheckHead = 0;

	hRoot = TVI_ROOT;
	hItem = NULL;
	m_SCList->Spin_InUse_set();
	RTREE_LChildRChain_Traversal_LINE(SC_NODE, m_SCList,
		operateNode_t->Spin_InUse_set();
		strTest = operateNode_t->StrCommand;
		if (strTest.substr(0,2) == "//"){
			Expand(hRoot,TVE_EXPAND);
			blCheckHead = 1;
			hRoot = TVI_ROOT;
		}
		hItem = AddNode(Str_ANSIToUnicode(strTest).c_str(), operateNode_t->blEnableSendCR, hRoot);
		if (hItem != NULL){
			SetItemData(hItem, RTREE_NODE::GetdRNodeID(operateNode_t));
			if (blCheckHead != 0){
				hRoot = hItem;
				blCheckHead = 0;
			}
		}
		operateNode_t->Spin_InUse_clr();
	);
	m_SCList->Spin_InUse_clr();
	Expand(hRoot,TVE_EXPAND);
	PostMessageW(WM_VSCROLL,SB_TOP,0);
}
//------------------------------------------------------------------------------------------//
BOOL CSCTree::DoShowEdit(HTREEITEM hItem){
	SC_NODE *node;
	if (CMyCTreeCtrl::DoShowEdit(hItem)){
		if (GetParentItem(hItem) == NULL){
			m_SCList->Spin_InUse_set();
			node = (SC_NODE*)RTREE_NODE::FindInLChildRChainByDRNodeID(m_SCList,GetItemData(hItem));
			if (node != NULL){
				node->Spin_InUse_set();
				if (node->StrCommand.substr(0, 2) == "//"){
					m_Edit.SetWindowTextW(Str_ANSIToUnicode(node->StrCommand.substr(2)).c_str());
					m_Edit.SetSel(-1);
				}
				node->Spin_InUse_clr();
			}
			m_SCList->Spin_InUse_clr();
		}
		return TRUE;
	}
	return FALSE;
}
//------------------------------------------------------------------------------------------//
void CSCTree::OnEditSave(HTREEITEM hItem){
	SC_NODE		*node;
	CString		text;
	std::wstring	strwText;
	if (hItem == NULL)
		return;
	m_SCList->Spin_InUse_set();
	node = (SC_NODE*)RTREE_NODE::FindInLChildRChainByDRNodeID(m_SCList, GetItemData(hItem));
	if (node != NULL){
		node->Spin_InUse_set();
		text = GetItemText(hItem);
		strwText = text.GetBuffer(0);
		node->StrCommand = Str_UnicodeToANSI(strwText);
		node->blEnableSendCR = GetCheck(hItem);
		if (GetParentItem(hItem) == NULL){
			if (node->StrCommand.substr(0,2) != "//"){
				node->StrCommand = "//" + node->StrCommand;
				text = _T("//") + text;
				SetItemText(m_EditItem,text);
			}
		}
		else{
			if (node->StrCommand.substr(0,2) == "//"){
				node->StrCommand = node->StrCommand.substr(2);
				text = Str_ANSIToUnicode(node->StrCommand).c_str();
				SetItemText(m_EditItem,text);
			}
		}
		node->Spin_InUse_clr();
	}
	m_SCList->Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
HTREEITEM CSCTree::CreateNode(HTREEITEM hItem){
	HTREEITEM retItem;
	INT32	nodeLevel;
	retItem = NULL;
	nodeLevel = CheckNodeLevel(hItem);
	m_SCList->Spin_InUse_set();
	switch(nodeLevel){
		case 0:
			retItem = CreateNodeL1();
			break;
		case 1:
			if (ItemHasChildren(hItem)){
				retItem = CreateNodeL1(hItem);
				break;
			}
		case 2:
			retItem = CreateNodeL2(hItem);
		case 3:;
	}
	m_SCList->Spin_InUse_clr();
	if (retItem != NULL)
		Expand(GetParentItem(retItem),TVE_EXPAND);
	SelectItem(retItem);
	return(retItem);
}
//------------------------------------------------------------------------------------------//
HTREEITEM CSCTree::CreateNodeL1(HTREEITEM hItem){
	//输入为根节点中的一个
	//SClist中保存为//结构
	SC_NODE		*node,*newNode;
	HTREEITEM	retItem;
	node = NULL;
	newNode = NULL;
	retItem = NULL;
	if (hItem == NULL){
		hItem = TVI_LAST;
	}
	else{
		retItem = GetNextItem(hItem,TVGN_NEXT);
		if (retItem != NULL)
			node = (SC_NODE*)RTREE_NODE::GetPriorBrother(RTREE_NODE::FindInLChildRChainByDRNodeID(m_SCList, GetItemData(retItem)));
	}
	newNode = new SC_NODE;
	if (newNode != NULL){
		retItem = AddNode(_T("//New group"),0,TVI_ROOT,hItem);
		if (retItem == NULL){
			delete newNode;
		}
		else{
			newNode->StrCommand = "//New group";
			newNode->blEnableSendCR = 0;
			if (node == NULL){
				RTREE_NODE::InsertLChild(m_SCList, newNode);
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
HTREEITEM CSCTree::CreateNodeL2(HTREEITEM hItem){
	SC_NODE		*newNode;
	HTREEITEM	retItem;

	newNode = NULL;
	retItem = NULL;

	newNode = new SC_NODE;
	if (newNode != NULL){
		retItem = AddNode(_T("New single command"), 1, GetParentItem(hItem), hItem);
		if (retItem != NULL){
			delete newNode;
		}
		else{
			newNode->StrCommand = "New single command";
			newNode->blEnableSendCR = 1;
			RTREE_NODE::InsertRChild(RTREE_NODE::FindInLChildRChainByDRNodeID(m_SCList, GetItemData(hItem)), newNode);
			SetItemData(retItem, RTREE_NODE::GetdRNodeID(newNode));
		}
	}
	return(retItem);
}
//------------------------------------------------------------------------------------------//
HTREEITEM CSCTree::DelNode(HTREEITEM delItem){
	INT32	nodeLevel;
	if (delItem == NULL)
		return(delItem);
	m_SCList->Spin_InUse_set();
	nodeLevel = CheckNodeLevel(delItem);
	switch(nodeLevel){
		case 1:
			delItem = DelNodeL1(delItem);
			break;
		case 2:
			delItem = DelNodeL2(delItem);
			break;
		default:;
	}
	RTREE_NODE::CleanTrash(m_SCList);
	m_SCList->Spin_InUse_clr();
	SelectItem(delItem);
	return(delItem);
}
//------------------------------------------------------------------------------------------//
HTREEITEM CSCTree::DelNodeL1(HTREEITEM delItem){
	HTREEITEM	nextItem;
	SC_NODE		*delNode,*endNode;

	if (delItem == NULL)
		return NULL;
	if (delItem == m_SelectItem)
		m_SelectItem = NULL;
	if (delItem == m_HotItem)
		m_HotItem = NULL;

	delNode = (SC_NODE*)RTREE_NODE::FindInLChildRChainByDRNodeID(m_SCList, GetItemData(delItem));
	nextItem = GetNextItem(delItem,TVGN_NEXT);
	if (nextItem == NULL){
		endNode = (SC_NODE*)RTREE_NODE::GetLastChild(m_SCList);
		nextItem = GetNextItem(delItem,TVGN_PREVIOUS);
	}
	else{
		endNode = (SC_NODE*)RTREE_NODE::GetPriorBrother(RTREE_NODE::FindInLChildRChainByDRNodeID(m_SCList, GetItemData(nextItem)));
	}
	RTREE_NODE::MoveNodesToTrash(delNode, endNode, m_SCList);
	DeleteItem(delItem);
	return(nextItem);
}
//------------------------------------------------------------------------------------------//
HTREEITEM CSCTree::DelNodeL2(HTREEITEM delItem){
	HTREEITEM	nextItem;
	SC_NODE		*delNode;

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
	delNode = (SC_NODE*)RTREE_NODE::FindInLChildRChainByDRNodeID(m_SCList, GetItemData(delItem));
	RTREE_NODE::MoveNodeToTrash(delNode, m_SCList);
	DeleteItem(delItem);
	return(nextItem);
}
//------------------------------------------------------------------------------------------//
HTREEITEM CSCTree::UpNode(HTREEITEM moveItem){
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
		default:;
	}

	SelectItem(moveItem);
	return(moveItem);
}
//------------------------------------------------------------------------------------------//
HTREEITEM CSCTree::UpNodeL1(HTREEITEM moveItem){
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
HTREEITEM CSCTree::UpNodeL2(HTREEITEM moveItem){
	HTREEITEM	PriorItem,fatherItem;
	SC_NODE		*moveNode,*priorNode;
	if (moveItem == NULL)
		return(moveItem);

	PriorItem = GetNextItem(moveItem,TVGN_PREVIOUS);
	if (PriorItem != NULL){
		PriorItem = DownNode(PriorItem);
		moveItem = GetNextItem(PriorItem,TVGN_PREVIOUS);
	}
	else{//moveItem is the first item in this group
		m_SCList->Spin_InUse_set();
		moveNode = (SC_NODE*)RTREE_NODE::FindInLChildRChainByDRNodeID(m_SCList, GetItemData(moveItem));
		priorNode = (SC_NODE*)RTREE_NODE::GetPriorBrother(moveNode);
		if (priorNode != (SC_NODE*)RTREE_NODE::GetFirstChild(m_SCList)){//lv2 item cannot move to the first of link
			fatherItem = GetParentItem(moveItem);
			fatherItem = GetNextItem(fatherItem, TVGN_PREVIOUS);
			RTREE_NODE::MoveNodesUpInRChain(moveNode);
			DeleteItem(moveItem);
			moveNode->Spin_InUse_set();
			moveItem = AddNode(Str_ANSIToUnicode(moveNode->StrCommand).c_str(), moveNode->blEnableSendCR, fatherItem);
			if (moveItem != NULL)
				SetItemData(moveItem, RTREE_NODE::GetdRNodeID(moveNode));
			moveNode->Spin_InUse_clr();
		}
		m_SCList->Spin_InUse_clr();
	}
	return(moveItem);
}
//------------------------------------------------------------------------------------------//
HTREEITEM CSCTree::DownNode(HTREEITEM moveItem){
	INT32	nodeLevel;
	if (moveItem == NULL)
		return(moveItem);
	m_SCList->Spin_InUse_set();
	nodeLevel = CheckNodeLevel(moveItem);
	switch(nodeLevel){
		case 1:
			moveItem = DownNodeL1(moveItem);
			break;
		case 2:
			moveItem = DownNodeL2(moveItem);
			break;
		default:;
	}
	m_SCList->Spin_InUse_clr();
	SelectItem(moveItem);
	return(moveItem);
}
//------------------------------------------------------------------------------------------//
HTREEITEM CSCTree::DownNodeL1(HTREEITEM moveItem){
	HTREEITEM	endItem,nextItem;
	SC_NODE		*insertNode,*endNode,*moveNode;
	UINT		blExpanded;
	if (moveItem == NULL)
		return(moveItem);

	blExpanded = TVIS_EXPANDED & GetItemState(moveItem,TVIS_EXPANDED);

	nextItem = GetNextItem(moveItem,TVGN_NEXT);
	if (nextItem == NULL)//moveItem is the last
		return(moveItem);

	moveNode = (SC_NODE*)RTREE_NODE::FindInLChildRChainByDRNodeID(m_SCList, GetItemData(moveItem));
	endNode = (SC_NODE*)RTREE_NODE::GetPriorBrother(RTREE_NODE::FindInLChildRChainByDRNodeID(m_SCList, GetItemData(nextItem)));

	endItem = GetNextItem(nextItem,TVGN_NEXT);
	if (endItem == NULL){
		insertNode = (SC_NODE*)RTREE_NODE::GetLastChild(m_SCList);
	}
	else{
		insertNode = (SC_NODE*)RTREE_NODE::GetPriorBrother(RTREE_NODE::FindInLChildRChainByDRNodeID(m_SCList, GetItemData(endItem)));
	}

	RTREE_NODE::MoveNodesAfterInRChain(moveNode, endNode, insertNode);

	DeleteItem(moveItem);
	moveNode->Spin_InUse_set();
	moveItem = AddNode(Str_ANSIToUnicode(moveNode->StrCommand).c_str(),0,TVI_ROOT,nextItem);
	moveNode->Spin_InUse_clr();
	if (moveItem != NULL){
		moveNode->Spin_InUse_set();
		SetItemData(moveItem, RTREE_NODE::GetdRNodeID(moveNode));
		moveNode->Spin_InUse_clr();
		RTREE_RChain_Traversal_LINE(SC_NODE, moveNode,
			if (operateNode_t == endNode)
				break;
			if (nextNode_t != NULL){
				nextNode_t->Spin_InUse_set();
				nextItem = AddNode(Str_ANSIToUnicode(nextNode_t->StrCommand).c_str(), nextNode_t->blEnableSendCR, moveItem);
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
HTREEITEM CSCTree::DownNodeL2(HTREEITEM moveItem){
	HTREEITEM	fatherItem,fatherNextItem,nextItem;
	SC_NODE		*moveNode;

	if (moveItem == NULL)
		return(moveItem);
	fatherItem = GetParentItem(moveItem);
	fatherNextItem = GetNextItem(fatherItem,TVGN_NEXT);//father next item
	moveNode = (SC_NODE*)RTREE_NODE::FindInLChildRChainByDRNodeID(m_SCList, GetItemData(moveItem));
	if (fatherNextItem == NULL){//moveItem in the last group
		if (moveNode == (SC_NODE*)RTREE_NODE::GetLastChild(m_SCList))//moveItem is the last item;
			return(moveItem);
	}
	nextItem = GetNextItem(moveItem,TVGN_NEXT);
	moveNode->Spin_InUse_set();
	if (nextItem == NULL){//hItem is the last item in the group;
		nextItem = GetNextItem(fatherNextItem,TVGN_CHILD);
		RTREE_NODE::MoveNodesDownInRChain(moveNode);
		RTREE_NODE::MoveNodesDownInRChain(moveNode);//move to next group second node.
		DeleteItem(moveItem);
		moveItem = AddNode(Str_ANSIToUnicode(moveNode->StrCommand).c_str(),moveNode->blEnableSendCR,fatherNextItem,nextItem);
		if (moveItem != NULL)
			SetItemData(moveItem, RTREE_NODE::GetdRNodeID(moveNode));
		DownNodeL2(nextItem);
		moveItem = GetNextItem(fatherNextItem,TVGN_CHILD);
	}
	else{
		RTREE_NODE::MoveNodesDownInRChain(moveNode);
		DeleteItem(moveItem);
		moveItem = AddNode(Str_ANSIToUnicode(moveNode->StrCommand).c_str(),moveNode->blEnableSendCR,fatherItem,nextItem);
		if (moveItem != NULL)
			SetItemData(moveItem, RTREE_NODE::GetdRNodeID(moveNode));
	}
	moveNode->Spin_InUse_clr();
	return(moveItem);
}
//------------------------------------------------------------------------------------------//
void CSCTree::OnLButtonDblClk(UINT nFlags, CPoint point){
	SC_NODE		*lNode,copyNode;
	if (m_SelectItem != NULL){
		if (GetParentItem(m_SelectItem) != NULL){
			m_SCList->Spin_InUse_set();
			lNode = (SC_NODE*)RTREE_NODE::FindInLChildRChainByDRNodeID(m_SCList, GetItemData(m_SelectItem));
			if (lNode != NULL){
				lNode->Spin_InUse_set();
				copyNode.StrCommand = lNode->StrCommand;
				copyNode.blEnableSendCR = lNode->blEnableSendCR;
				lNode->Spin_InUse_clr();
			}
			m_SCList->Spin_InUse_clr();
			if (lNode != NULL)
				GSDTApp.m_Device.SendCommand(copyNode.StrCommand, 0, copyNode.blEnableSendCR, G_ESCAPE_ON);
		}
	}
	CMyCTreeCtrl::OnLButtonDblClk(nFlags,point);
}
//------------------------------------------------------------------------------------------//


//------------------------------------------------------------------------------------------//
class CSCViewMenuButton : public CMFCToolBarMenuButton{
	friend class CClassView;
	DECLARE_SERIAL(CSCViewMenuButton)
public:
	CSCViewMenuButton(HMENU hMenu = NULL) : CMFCToolBarMenuButton((UINT)-1, hMenu, -1){
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
IMPLEMENT_SERIAL(CSCViewMenuButton, CMFCToolBarMenuButton, 1)
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CSCViewDP::CSCViewDP(void){
}
//------------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CSCViewDP, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_SETFOCUS()
	ON_WM_PAINT()
	ON_COMMAND(ID_TOOLBAR_SC_ADD, &CSCViewDP::OnSC_Add)
	ON_COMMAND(ID_TOOLBAR_SC_DEL, &CSCViewDP::OnSC_Del)
	ON_COMMAND(ID_TOOLBAR_SC_UP, &CSCViewDP::OnSC_Up)
	ON_COMMAND(ID_TOOLBAR_SC_DOWN, &CSCViewDP::OnSC_Down)
	ON_COMMAND(ID_TOOLBAR_SC_EXPAND, &CSCViewDP::OnSC_Expand)
	ON_COMMAND(ID_TOOLBAR_SC_COLLAPSE, &CSCViewDP::OnSC_Collapse)

	ON_COMMAND(ID_TOOLBAR_SC_LOAD, &CSCViewDP::OnButtonClickSC_LOAD)
	ON_COMMAND(ID_TOOLBAR_SC_IMPORT, &CSCViewDP::OnButtonClickSC_IMPORT)
	ON_COMMAND(ID_TOOLBAR_SC_SAVE, &CSCViewDP::OnButtonClickSC_SAVE)
END_MESSAGE_MAP()
//------------------------------------------------------------------------------------------//
CSCViewDP::~CSCViewDP(void){
}
//------------------------------------------------------------------------------------------//
int CSCViewDP::OnCreate(LPCREATESTRUCT lpCreateStruct){
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (CreateSCTree() == -1)
		return -1;
	if (CreateSCToolBar() == -1)
		return -1;
	AdjustLayout();
	return 0;
}
//------------------------------------------------------------------------------------------//
void CSCViewDP::AdjustLayout(void){
	if (GetSafeHwnd() == NULL)
		return;

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndSCTree.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}
//------------------------------------------------------------------------------------------//
void CSCViewDP::OnSize(UINT nType, int cx, int cy){
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}
//------------------------------------------------------------------------------------------//
void CSCViewDP::OnSetFocus(CWnd* pOldWnd){
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndSCTree.SetFocus();
}
//------------------------------------------------------------------------------------------//
void CSCViewDP::OnPaint(void){
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndSCTree.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}
//------------------------------------------------------------------------------------------//
void CSCViewDP::OnContextMenu(CWnd* pWnd, CPoint point){
	CSCTree* pWndTree = (CSCTree*) &m_wndSCTree;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree){
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int CSCViewDP::CreateSCToolBar(void){
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_TOOLBAR_SC);
	m_wndToolBar.LoadToolBar(IDR_TOOLBAR_SC, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);
	
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);// All commands will be routed via this control , not via the parent frame:

	CMenu menuSort;
	menuSort.LoadMenu(IDR_POPUP_SC);
	m_wndToolBar.ReplaceButton(ID_TOOLBAR_SC_LOAD, CSCViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));
	CSCViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CSCViewMenuButton, m_wndToolBar.GetButton(10));
	if (pButton != NULL){
		pButton->m_bText = FALSE;
		pButton->m_bImage = TRUE;
		pButton->SetImage(0);
		pButton->SetMessageWnd(this);
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
void CSCViewDP::OnChangeVisualStyle(void){
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SCViewBarButton_hc : IDR_TOOLBAR_SC, 0, 0, TRUE /* Locked */);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int CSCViewDP::CreateSCTree(void){
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_wndSCTree.Create(WS_CHILD | WS_VISIBLE, rectDummy, this, 0)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	m_wndSCTree.LoadData(&GSDTApp.m_SCList);
	return 0;
}
//------------------------------------------------------------------------------------------//
void CSCViewDP::OnSC_Add(void){
	m_wndSCTree.CreateNode(m_wndSCTree.GetSelectedNode());
}
//------------------------------------------------------------------------------------------//
void CSCViewDP::OnSC_Del(void){
	m_wndSCTree.DelNode(m_wndSCTree.GetSelectedNode());
}
//------------------------------------------------------------------------------------------//
void CSCViewDP::OnSC_Up(void){
	m_wndSCTree.UpNode(m_wndSCTree.GetSelectedNode());
}
//------------------------------------------------------------------------------------------//
void CSCViewDP::OnSC_Down(void){
	m_wndSCTree.DownNode(m_wndSCTree.GetSelectedNode());
}
//------------------------------------------------------------------------------------------//
void CSCViewDP::OnSC_Expand(void){
	m_wndSCTree.ExpandAllRoot();
}
//------------------------------------------------------------------------------------------//
void CSCViewDP::OnSC_Collapse(void){
	m_wndSCTree.CollapseAllRoot();
}
//------------------------------------------------------------------------------------------//
void CSCViewDP::OnButtonClickSC_LOAD(void){
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
		readType = READINITYPE_NONE | READINITYPE_AddSingleCommand | READINITYPE_CoverSingleCommand;
		GSDTApp.m_ParRecord.ReadScriptFromIniFile(Str_UnicodeToANSI(strwText).c_str(),readType);
		m_wndSCTree.LoadData(&GSDTApp.m_SCList);
	}
}
//------------------------------------------------------------------------------------------//
void CSCViewDP::OnButtonClickSC_IMPORT(void){
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
		readType = READINITYPE_NONE | READINITYPE_AddSingleCommand;
		GSDTApp.m_ParRecord.ReadScriptFromIniFile(Str_UnicodeToANSI(strwText).c_str(),readType);
		m_wndSCTree.LoadData(&GSDTApp.m_SCList);
	}
}
//------------------------------------------------------------------------------------------//
void CSCViewDP::OnButtonClickSC_SAVE(void){
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
		readType = READINITYPE_NONE | READINITYPE_AddSingleCommand;
		GSDTApp.m_ParRecord.WriteScriptToIniFile(Str_UnicodeToANSI(strwText),readType);
	}
}
//------------------------------------------------------------------------------------------//
