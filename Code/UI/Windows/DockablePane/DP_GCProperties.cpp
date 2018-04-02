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
#include "DP_GCProperties.h"
#include "DP_GC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//------------------------------------------------------------------------------------------//
CGCPropertyGridCtrl	g_wndGCPropList;
//------------------------------------------------------------------------------------------//
CGCPropertyGridCtrl::CGCPropertyGridCtrl(void){
	m_type = GCP_NONE;
	m_command = NULL;
	m_group = NULL;
	m_blIsModified = 0;
	m_hItem = NULL;
}
//------------------------------------------------------------------------------------------//
CMFCPropertyGridProperty *CGCPropertyGridCtrl::CreateCMDTailItem(INT32 id, CString title, CMD_TAIL tail, CString description){
	CMFCPropertyGridProperty *item;

	item = new CMFCPropertyGridProperty(title, (_variant_t)_T(""), description);
	item->AddOption(_T("None"));
	item->AddOption(_T("R"));
	item->AddOption(_T("N"));
	item->AddOption(_T("RN"));
	switch (tail){
		case CMD_NONE:item->SetValue(_T("None")); break;
		case CMD_R:item->SetValue(_T("R")); break;
		case CMD_N:item->SetValue(_T("N")); break;
		case CMD_RN:item->SetValue(_T("RN")); break;
	}
	item->AllowEdit(FALSE);
	item->SetData(id);
	return(item);
}
//------------------------------------------------------------------------------------------//
CMFCPropertyGridProperty *CGCPropertyGridCtrl::CreateEnableItem(INT32 id,CString title,int32 blEnable,CString description){
	CMFCPropertyGridProperty *item;
	
	item = new CMFCPropertyGridProperty(title,(_variant_t)_T(""),description);
	item->AddOption(_T("Enable"));
	item->AddOption(_T("Disable"));
	if (blEnable == 0){
		item->SetValue(_T("Disable"));
	}
	else{
		item->SetValue(_T("Enable"));
	}
	item->AllowEdit(FALSE);
	item->SetData(id);
	return(item);
}
//------------------------------------------------------------------------------------------//
CMFCPropertyGridProperty *CGCPropertyGridCtrl::CreateYesItem(INT32 id,CString title,int32 blEnable,CString description){
	CMFCPropertyGridProperty *item;
	
	item = new CMFCPropertyGridProperty(title,(_variant_t)_T(""),description);
	item->AddOption(_T("Yes"));
	item->AddOption(_T("No"));
	if (blEnable == 0){
		item->SetValue(_T("No"));
	}
	else{
		item->SetValue(_T("Yes"));
	}
	item->AllowEdit(FALSE);
	item->SetData(id);
	return(item);
}
//------------------------------------------------------------------------------------------//
CMFCPropertyGridProperty *CGCPropertyGridCtrl::CreateTxtItem(INT32 id,CString title,CString content,CString description){
	CMFCPropertyGridProperty *item = new CMFCPropertyGridProperty(title,content,description);
	item->SetData(id);
	return(item);
}
//------------------------------------------------------------------------------------------//
CMFCPropertyGridProperty *CGCPropertyGridCtrl::CreateIntItem(INT32 id,CString title,long content,CString description){
	CMFCPropertyGridProperty *item = new CMFCPropertyGridProperty(title,(_variant_t)content,description);
	item->SetData(id);
	return(item);
}
//------------------------------------------------------------------------------------------//
CMFCPropertyGridProperty *CGCPropertyGridCtrl::CreateFloItem(INT32 id,CString title,double content,CString description){
	CMFCPropertyGridProperty *item = new CMFCPropertyGridProperty(title,(_variant_t)content,description);
	item->SetData(id);
	return(item);
}
//------------------------------------------------------------------------------------------//
void CGCPropertyGridCtrl::LoadInit(void){
	EnableHeaderCtrl(FALSE);
	EnableDescriptionArea();
	MarkModifiedProperties();
	//SetVSDotNetLook();
	//SetGroupNameFullWidth(TRUE);
	if (m_type != GCP_NONE){
		RemoveAll();

		m_type = GCP_NONE;
		m_command = NULL;
		m_group = NULL;
		m_blIsModified = 0;
		m_hItem = NULL;
		Invalidate();
		UpdateWindow();
	}
}
//------------------------------------------------------------------------------------------//
void CGCPropertyGridCtrl::LoadGroup(HTREEITEM hItem, COMMAND_GROUP *group, int32 blenforce){
	if ((m_group != group) || (blenforce != 0)){
		m_hItem = hItem;
		m_group = group;
		m_command = NULL;
		if (group == NULL)
			return;
		group->InUse_set();
		if (m_type == GCP_GROUP){
			LoadGroupP(group);
		}
		else{
			RemoveAll();
			CreateGroupP(group);
			LoadGroupP(group);
			m_type = GCP_GROUP;
		}
		group->InUse_clr();
	}
}
//------------------------------------------------------------------------------------------//
void CGCPropertyGridCtrl::CreateGroupP(COMMAND_GROUP *group){
	CMFCPropertyGridProperty *pGroup = new CMFCPropertyGridProperty(_T("Group"));
	m_blIsModified = 0;
	pGroup->AddSubItem(CreateTxtItem(1,_T("Name"),_T(""),_T("Group Name.")));
	pGroup->AddSubItem(CreateIntItem(2,_T("Interval(ms)"),200,_T("Interval time between two command sending.Unit is ms.")));
	pGroup->AddSubItem(CreateEnableItem(3,_T("Enable"),0,_T("Enable/Disable execution.")));
	pGroup->AddSubItem(CreateIntItem(4,_T("Cycle unmber"),1,_T("Cycle unmber.")));
	AddProperty(pGroup);
}
//------------------------------------------------------------------------------------------//
void CGCPropertyGridCtrl::LoadGroupP(COMMAND_GROUP *group){
	CMFCPropertyGridProperty *pGroup;
	if (m_blIsModified != 0)
		ResetOriginalValues(FALSE);
	m_blIsModified = 0;
	pGroup = GetProperty(0);
	pGroup->GetSubItem(0)->SetValue(Str_ANSIToUnicode(group->name).c_str());
	pGroup->GetSubItem(1)->SetValue((_variant_t)(long)group->intervalTime);
	if (group->blEnableAutoRun == 0){
		pGroup->GetSubItem(2)->SetValue(_T("Disable"));
	}
	else{
		pGroup->GetSubItem(2)->SetValue(_T("Enable"));
	}
	pGroup->GetSubItem(3)->SetValue((_variant_t)(long)group->autoRunTimes);
}
//------------------------------------------------------------------------------------------//
void CGCPropertyGridCtrl::LoadCommand(HTREEITEM hItem, COMMAND_NODE *command, int32 blenforce){
	if ((m_command != command) || (blenforce != 0)){
		m_hItem = hItem;
		m_group = NULL;
		m_command = command;
		if (command == NULL)
			return;
		command->InUse_set();
		if (m_type == GCP_COMMAND){
			LoadCommandP(command);
		}
		else{
			RemoveAll();
			CreateCommandP(command);
			LoadCommandP(command);
			m_type = GCP_COMMAND;
		}
		command->InUse_clr();
	}
}
//------------------------------------------------------------------------------------------//
void CGCPropertyGridCtrl::CreateCommandP(COMMAND_NODE *command){
	CMFCPropertyGridProperty *pGroup;
	m_blIsModified = 0;
	pGroup = new CMFCPropertyGridProperty(_T("Display"));
	pGroup->AddSubItem(CreateTxtItem(5,_T("Name"),_T(""),_T("Show name.")));
	pGroup->AddSubItem(CreateYesItem(6,_T("Same as command"),1));
	AddProperty(pGroup);

	pGroup = new CMFCPropertyGridProperty(_T("Command"));
	pGroup->AddSubItem(CreateTxtItem(7,_T("Command"),_T("")));
	pGroup->AddSubItem(CreateEnableItem(8,_T("Enable"),1,_T("Enable/Disable send.")));
	pGroup->AddSubItem(CreateCMDTailItem(9,_T("Send <CR><LF>"),CMD_R,_T("Send <CR><LF> in the command end.")));
	AddProperty(pGroup);

	pGroup = new CMFCPropertyGridProperty(_T("Time"));
	pGroup->AddSubItem(CreateTxtItem(10,_T("Timeout(S)"),_T("Unit is s.")));
	pGroup->AddSubItem(CreateIntItem(11,_T("Cycle number"),1));
	AddProperty(pGroup);

	pGroup = new CMFCPropertyGridProperty(_T("Condition"));
	pGroup->AddSubItem(CreateTxtItem(12,_T("Continue"),_T("")));
	pGroup->AddSubItem(CreateTxtItem(13,_T("Resend"),_T("")));
	pGroup->AddSubItem(CreateTxtItem(14,_T("Stop"),_T("")));
	pGroup->AddSubItem(CreateTxtItem(15,_T("Catch"),_T("")));
	AddProperty(pGroup);
}
//------------------------------------------------------------------------------------------//
void CGCPropertyGridCtrl::LoadCommandP(COMMAND_NODE *command){
	CMFCPropertyGridProperty *pGroup;
	if (m_blIsModified != 0)
		ResetOriginalValues(FALSE);
	m_blIsModified = 0;
	pGroup = GetProperty(0);
	pGroup->GetSubItem(0)->SetValue(Str_ANSIToUnicode(command->StrShowName).c_str());
	if (command->blSameAsCommand == 0){
		pGroup->GetSubItem(0)->Enable();
		pGroup->GetSubItem(1)->SetValue(_T("No"));
	}
	else{
		pGroup->GetSubItem(0)->Enable(FALSE);
		pGroup->GetSubItem(1)->SetValue(_T("Yes"));
	}
	pGroup = GetProperty(1);
	pGroup->GetSubItem(0)->SetValue(Str_ANSIToUnicode(command->StrCommand).c_str());
	if (command->blEnableSend == 0){
		pGroup->GetSubItem(1)->SetValue(_T("Disable"));
	}
	else{
		pGroup->GetSubItem(1)->SetValue(_T("Enable"));
	}
	switch (command->cmdTail){
		case CMD_NONE:pGroup->GetSubItem(2)->SetValue(_T("None")); break;
		case CMD_R:pGroup->GetSubItem(2)->SetValue(_T("R")); break;
		case CMD_N:pGroup->GetSubItem(2)->SetValue(_T("N")); break;
		case CMD_RN:pGroup->GetSubItem(2)->SetValue(_T("RN")); break;
	}
	pGroup = GetProperty(2);
	pGroup->GetSubItem(0)->SetValue(Str_ANSIToUnicode(command->StrTimeout).c_str());
	int32 cycle;
	if (command->StrCycle.length() > 0){
		cycle = atoi(command->StrCycle.c_str());
	}
	else{
		cycle = 1;
	}
	pGroup->GetSubItem(1)->SetValue((_variant_t)(long)cycle);
	pGroup = GetProperty(3);
	pGroup->GetSubItem(0)->SetValue(Str_ANSIToUnicode(command->StrContinue).c_str());
	pGroup->GetSubItem(1)->SetValue(Str_ANSIToUnicode(command->StrResend).c_str());
	pGroup->GetSubItem(2)->SetValue(Str_ANSIToUnicode(command->StrStop).c_str());
	pGroup->GetSubItem(3)->SetValue(Str_ANSIToUnicode(command->StrCatch).c_str());
}
//------------------------------------------------------------------------------------------//
void CGCPropertyGridCtrl::OnPropertyChanged(CMFCPropertyGridProperty* pProp) const{
	CMFCPropertyGridProperty *pGroup;
	INT32	id,level;
	CString	cValue;
	int32	intValue;
	COMMAND_GROUP	*group;
	COMMAND_NODE	*command;

	HTREEITEM		selectItemFormGCTree;
	group = NULL;
	command = NULL;
	selectItemFormGCTree = CGCViewDP::GetGCTree().GetSelectedItem();
	level = CGCViewDP::GetGCTree().CheckNodeLevel(selectItemFormGCTree);
	CGCViewDP::GetGCTree().m_GCList->InUse_set();
	switch (level){
		case 0:
		case 1:
			goto End_Changed;
		case 2:
			group = CGCViewDP::GetGCTree().GetGroup(selectItemFormGCTree);
			if (group != m_group)
				goto End_Changed;
			break;
		case 3:
			command = CGCViewDP::GetGCTree().GetCommand(selectItemFormGCTree);
			if (command != m_command)
				goto End_Changed;
			break;
		default:
			goto End_Changed;
	}
	id = pProp->GetData();
	m_blIsModified = 1;
	switch(id){
		case 1:
			cValue = pProp->GetValue();
			m_group->name = Str_UnicodeToANSI(cValue.GetBuffer(0));
			CGCViewDP::GetGCTree().SetItemText(m_hItem,cValue);
			break;
		case 2:
			intValue = pProp->GetValue().intVal;
			m_group->intervalTime = intValue;
			break;
		case 3:
			cValue = pProp->GetValue();
			if (cValue == _T("Enable")){
				m_group->blEnableAutoRun = 1;
				CGCViewDP::GetGCTree().SetCheck(m_hItem,TRUE);
			}
			else if (cValue == _T("Disable")){
				m_group->blEnableAutoRun = 0;
				CGCViewDP::GetGCTree().SetCheck(m_hItem,FALSE);
			}
			break;
		case 4:
			intValue = pProp->GetValue().lVal;
			m_group->autoRunTimes = intValue;
			break;
		case 5:
			cValue = pProp->GetValue();
			m_command->StrShowName = Str_UnicodeToANSI(cValue.GetBuffer(0));
			if (m_command->blSameAsCommand == 0)
				CGCViewDP::GetGCTree().SetItemText(m_hItem,Str_ANSIToUnicode(m_command->StrShowName).c_str());
			break;
		case 6:
			cValue = pProp->GetValue();
			pGroup = GetProperty(0);
			if (cValue == _T("No")){
				pGroup->GetSubItem(0)->Enable();
				pGroup->GetSubItem(1)->SetValue(_T("No"));
				m_command->blSameAsCommand = 0;
				CGCViewDP::GetGCTree().SetItemText(m_hItem,Str_ANSIToUnicode(m_command->StrShowName).c_str());
			}
			else if (cValue == _T("Yes")){
				pGroup->GetSubItem(0)->Enable(FALSE);
				pGroup->GetSubItem(1)->SetValue(_T("Yes"));
				m_command->blSameAsCommand = 1;
				CGCViewDP::GetGCTree().SetItemText(m_hItem,Str_ANSIToUnicode(m_command->StrCommand).c_str());
			}
			break;
		case 7:
			cValue = pProp->GetValue();
			m_command->StrCommand = Str_UnicodeToANSI(cValue.GetBuffer(0));
			if (m_command->blSameAsCommand != 0)
				CGCViewDP::GetGCTree().SetItemText(m_hItem,Str_ANSIToUnicode(m_command->StrCommand).c_str());
			break;
		case 8:
			cValue = pProp->GetValue();
			if (cValue == _T("Disable")){
				m_command->blEnableSend = 0;
				CGCViewDP::GetGCTree().SetCheck(m_hItem,FALSE);
			}
			else if (cValue == _T("Enable")){
				m_command->blEnableSend = 1;
				CGCViewDP::GetGCTree().SetCheck(m_hItem,TRUE);
			}
			break;
		case 9:
			cValue = pProp->GetValue();
			if (cValue == _T("None")){
				m_command->cmdTail = CMD_NONE;
			}
			else if (cValue == _T("R")){
				m_command->cmdTail = CMD_R;
			}
			else if (cValue == _T("N")){
				m_command->cmdTail = CMD_N;
			}
			else if (cValue == _T("RN")){
				m_command->cmdTail = CMD_RN;
			}
			break;
		case 10:
			double doubleValue;
			cValue = pProp->GetValue();
			m_command->StrTimeout = Str_UnicodeToANSI(cValue.GetBuffer(0));
			doubleValue = atof(m_command->StrTimeout.c_str());
			m_command->StrTimeout = Str_FloatToStr(doubleValue);
			pProp->SetValue(Str_ANSIToUnicode(m_command->StrTimeout).c_str());
			break;
		case 11:
			intValue = pProp->GetValue().lVal;
			m_command->StrCycle = Str_ToStr(intValue);
			break;
		case 12:
			cValue = pProp->GetValue();
			m_command->StrContinue = Str_UnicodeToANSI(cValue.GetBuffer(0));
			break;
		case 13:
			cValue = pProp->GetValue();
			m_command->StrResend = Str_UnicodeToANSI(cValue.GetBuffer(0));
			break;
		case 14:
			cValue = pProp->GetValue();
			m_command->StrStop = Str_UnicodeToANSI(cValue.GetBuffer(0));
			break;
		case 15:
			cValue = pProp->GetValue();
			m_command->StrCatch = Str_UnicodeToANSI(cValue.GetBuffer(0));
			break;
	}
End_Changed:
	CGCViewDP::GetGCTree().m_GCList->InUse_clr();
	CMFCPropertyGridCtrl::OnPropertyChanged(pProp);
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CGCPropertiesViewDP, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_PAINT()

	ON_COMMAND(ID_TOOLBAR_GCP_EXPAND, &CGCPropertiesViewDP::OnExpandAllProperties)
	ON_COMMAND(ID_TOOLBAR_GCP_SORT, &CGCPropertiesViewDP::OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_GCP_SORT, &CGCPropertiesViewDP::OnUpdateSortProperties)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_GCP_COMMAND,&CGCPropertiesViewDP::OnUpdateCOMMAND)
END_MESSAGE_MAP()
//------------------------------------------------------------------------------------------//
int CGCPropertiesViewDP::OnCreate(LPCREATESTRUCT lpCreateStruct){
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (CreatePropList() == -1)
		return -1;
	if (CreateGCToolBar() == -1)
		return -1;
	AdjustLayout();
	return 0;
}
//------------------------------------------------------------------------------------------//
void CGCPropertiesViewDP::AdjustLayout(void){
	if (GetSafeHwnd () == NULL || (AfxGetMainWnd() != NULL && AfxGetMainWnd()->IsIconic())){
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	GetGCPropList().SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}
//------------------------------------------------------------------------------------------//
void CGCPropertiesViewDP::OnSize(UINT nType, int cx, int cy){
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}
//------------------------------------------------------------------------------------------//
void CGCPropertiesViewDP::OnSetFocus(CWnd* pOldWnd){
	CDockablePane::OnSetFocus(pOldWnd);
	GetGCPropList().SetFocus();
}
//------------------------------------------------------------------------------------------//
void CGCPropertiesViewDP::OnPaint(void){
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	g_wndGCPropList.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}
//------------------------------------------------------------------------------------------//
int CGCPropertiesViewDP::CreatePropList(void){
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!GetGCPropList().Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 0)){
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}

	GetGCPropList().LoadInit();
	return 0;
}
//------------------------------------------------------------------------------------------//
int CGCPropertiesViewDP::CreateGCToolBar(void){

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_TOOLBAR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_TOOLBAR_PROPERTIES, 0, 0, TRUE /* Is locked */);
	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);
	return 0;
}
//------------------------------------------------------------------------------------------//
void CGCPropertiesViewDP::OnChangeVisualStyle(void){
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_TOOLBAR_PROPERTIES, 0, 0, TRUE /* Locked */);
}
//------------------------------------------------------------------------------------------//
void CGCPropertiesViewDP::OnExpandAllProperties(){
	GetGCPropList().ExpandAll();
}
//------------------------------------------------------------------------------------------//
void CGCPropertiesViewDP::OnSortProperties(){
	GetGCPropList().SetAlphabeticMode(!GetGCPropList().IsAlphabeticMode());
}
//------------------------------------------------------------------------------------------//
void CGCPropertiesViewDP::OnUpdateSortProperties(CCmdUI* pCmdUI){
	pCmdUI->SetCheck(GetGCPropList().IsAlphabeticMode());
}
//------------------------------------------------------------------------------------------//
void CGCPropertiesViewDP::OnUpdateCOMMAND(CCmdUI* pCmdUI){
	pCmdUI->Enable(TRUE);
}
//------------------------------------------------------------------------------------------//
CGCPropertyGridCtrl& CGCPropertiesViewDP::GetGCPropList(void){
	static CGCPropertyGridCtrl sg_wndGCPropList;
	return(sg_wndGCPropList);
}
//------------------------------------------------------------------------------------------//