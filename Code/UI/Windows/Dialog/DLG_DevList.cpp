// DevList.cpp : implementation file
//
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Global.h"
#include "SDT.h"
#include "DLG_DevList.h"
#include "afxdialogex.h"
//------------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CDevListCtrl, CMyCListCtrl)
	ON_WM_CREATE()
END_MESSAGE_MAP()
//------------------------------------------------------------------------------------------//
CDevListCtrl::CDevListCtrl() : CMyCListCtrl(){
	m_IPCOMLIST = NULL;
}
//------------------------------------------------------------------------------------------//
BOOL CDevListCtrl::DoShowExItem(int hItem,int hSubItem,int type){
	CString	cText;
	BOOL	ret;
	cText = GetItemText(hItem,0);
	ret = FALSE;
	switch(hSubItem){
		case 0:
			if ((cText != "COM") && (cText != "Unavailable"))
				ret = DoShowComboBox(hItem,hSubItem);
			break;
		case 1:
			if ((cText == "TCP") || (cText == "UDP"))
				ret = DoShowEdit(hItem,hSubItem);
			break;
		case 2:
			ret = DoShowEdit(hItem,hSubItem);
			break;
		case 3:
			break;
		case 4:
			ret = DoShowEdit(hItem,hSubItem);
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
void CDevListCtrl::OnEditSave(int hItem,int hSubItem){
	CString			cstrText;
	std::wstring	strwText;
	IPCOMNAME		*newNode;
	cstrText = GetItemText(hItem,hSubItem);
	strwText = cstrText.GetBuffer(0);
	m_IPCOMLIST->Traversal_set();
	newNode = (IPCOMNAME*)TNF::FindInDownChainByDRNodeID(m_IPCOMLIST,GetItemData(hItem));
	newNode->rwLock.W_set();
	switch(hSubItem){
		case 0:
			if (cstrText == "TCP")
				newNode->typeID = PublicDevice_DEVID_TCPClient;
			if (cstrText == "UDP")
				newNode->typeID = PublicDevice_DEVID_UDPClient;
			if (cstrText == "TCP SERVER")
				newNode->typeID = PublicDevice_DEVID_TCPServer;
			if (cstrText == "UDP SERVER")
				newNode->typeID = PublicDevice_DEVID_UDPServer;
			break;
		case 1:
			newNode->strIPComName = Str_UnicodeToANSI(strwText);
			break;
		case 2:
			newNode->portBaudrate = atoi(Str_UnicodeToANSI(strwText).c_str());
			break;
		case 3:
			newNode->TCPTypeOrFriendlyName = Str_UnicodeToANSI(strwText);
			break;
		case 4:
			newNode->strUserDefineName = Str_UnicodeToANSI(strwText);
			break;
	}
	newNode->CreateShowName();
	newNode->rwLock.W_clr();
	m_IPCOMLIST->Traversal_clr();
}
//------------------------------------------------------------------------------------------//
void CDevListCtrl::OnComboBoxShow(int hItem,int hSubItem){
	m_ComboBox.InsertString(0,_T("TCP"));
	m_ComboBox.InsertString(1,_T("UDP"));
	m_ComboBox.InsertString(2,_T("TCP SERVER"));
	m_ComboBox.InsertString(3,_T("UDP SERVER"));
	CString	cText;
	cText = GetItemText(hItem,hSubItem);
	if (cText == "TCP")
		m_ComboBox.SetCurSel(0);
	if (cText == "UDP")
		m_ComboBox.SetCurSel(1);
	if (cText == "TCP SERVER")
		m_ComboBox.SetCurSel(2);
	if (cText == "UDP SERVER")
		m_ComboBox.SetCurSel(3);
	m_ComboBox.SetWindowTextW(GetItemText(hItem,hSubItem));
}
//------------------------------------------------------------------------------------------//
void CDevListCtrl::OnComboBoxSave(int hItem,int hSubItem){
	CString cstrText;
	cstrText = GetItemText(hItem,hSubItem);
	if ((cstrText == "TCP") || (cstrText == "UDP"))
		SetItemText(m_EditItem,3,_T("Socket Client"));
	if (cstrText == "TCP SERVER"){
		SetItemText(m_EditItem,1,_T("TCP Server"));
		SetItemText(m_EditItem,3,_T("Socket Server"));
	}
	if (cstrText == "UDP SERVER"){
		SetItemText(m_EditItem,1,_T("UDP Server"));
		SetItemText(m_EditItem,3,_T("Socket Server"));
	}
	OnEditSave(hItem,0);
	OnEditSave(hItem,1);
	OnEditSave(hItem,2);
	OnEditSave(hItem,3);
	OnEditSave(hItem,4);
}
//------------------------------------------------------------------------------------------//
int CDevListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct){
	if (CMyCListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	InsertColumn(0, _T("Type"), LVCFMT_LEFT, 80);
	InsertColumn(1, _T("IP or COM#"), LVCFMT_LEFT, 190);
	InsertColumn(2, _T("Port"), LVCFMT_LEFT, 60);
	InsertColumn(3, _T("Socket Type or COM Name"), LVCFMT_LEFT, 300);
	InsertColumn(4, _T("User Define Name"), LVCFMT_LEFT, 240);
	return 0;
}
//------------------------------------------------------------------------------------------//
void CDevListCtrl::LoadData(IPCOMLIST *tIPCOMLIST){

	int32		i;
	
	DeleteAllItems();
	m_SelectItem = -1;
	m_SelectSubItem = -1;
	m_IPCOMLIST = tIPCOMLIST;
	m_IPCOMLIST->Refresh();

	i = 0;
	m_IPCOMLIST->Traversal_set();
	TREE_DownChain_Traversal_LINE(IPCOMNAME, m_IPCOMLIST,
		_opNode->rwLock.R_set();
		if (_opNode->blAvailable != 0){
			if (_opNode->typeID == PublicDevice_DEVID_APICOM)
				InsertItem(i,_T("COM"));
			if (_opNode->typeID == PublicDevice_DEVID_TCPClient)
				InsertItem(i,_T("TCP"));
			if (_opNode->typeID == PublicDevice_DEVID_UDPClient)
				InsertItem(i,_T("UDP"));
			if (_opNode->typeID == PublicDevice_DEVID_TCPServer)
				InsertItem(i,_T("TCP SERVER"));
			if (_opNode->typeID == PublicDevice_DEVID_UDPServer)
				InsertItem(i,_T("UDP SERVER"));
		}
		else{
			InsertItem(i,_T("Unavailable"));
		}
		SetItemText(i, 1, Str_ANSIToUnicode(_opNode->strIPComName).c_str());
		SetItemText(i, 2, Str_ANSIToUnicode(Str_ToStr(_opNode->portBaudrate)).c_str());
		SetItemText(i, 3, Str_ANSIToUnicode(_opNode->TCPTypeOrFriendlyName).c_str());
		SetItemText(i, 4, Str_ANSIToUnicode(_opNode->strUserDefineName).c_str());
		SetItemData(i, TNF::GetDRNodeID(_opNode));
		++ i;
		_opNode->rwLock.R_clr();
	);

	m_IPCOMLIST->Traversal_clr();
}
//------------------------------------------------------------------------------------------//
int CDevListCtrl::CreateNode(int node){
	IPCOMNAME	*newNode;
	TNF			*lNode;
	if (node < 0)
		node = GetItemCount() - 1;

	newNode = new IPCOMNAME;
	if (newNode != NULL){
		newNode->typeID = PublicDevice_DEVID_TCPClient;
		newNode->strIPComName = "127.0.0.1";
		newNode->portBaudrate = 115200;
		newNode->TCPTypeOrFriendlyName = "Socket Client";
		newNode->strUserDefineName = "";
		newNode->CreateShowName();
		lNode = TNF::FindInDownChainByDRNodeID(m_IPCOMLIST, GetItemData(node));
		if (lNode != nullptr){
			TNF::InsertAfter(lNode, newNode);
		}
		else{
				m_IPCOMLIST->AppendDownNode(newNode);
		}

		InsertItem(node + 1,_T("TCP"));
		SetItemText(node + 1,1,_T("127.0.0.1"));
		SetItemText(node + 1,2,_T("115200"));
		SetItemText(node + 1,3,_T("Socket Client"));
		SetItemText(node + 1,4,_T(""));
		SetItemData(node + 1, TNF::GetDRNodeID(newNode));
	}

	SetItemState(node, 0, LVIS_SELECTED|LVIS_FOCUSED);
	SetItemState(node + 1, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	m_SelectItem = node + 1;
	m_SelectSubItem = node + 1;
	EnsureVisible(m_SelectSubItem, FALSE);
	return(m_SelectItem);
}
//------------------------------------------------------------------------------------------//
int CDevListCtrl::DelNode(int delItem){
	IPCOMNAME	*delNode;
	if (delItem < 0)
		return -1;

	delNode = (IPCOMNAME*)TNF::FindInDownChainByDRNodeID(m_IPCOMLIST, GetItemData(delItem));
	m_IPCOMLIST->MoveNodesToTrash(m_IPCOMLIST, delNode, delNode);
	m_IPCOMLIST->CleanTrash(m_IPCOMLIST);

	DeleteItem(delItem);
	if (delItem < GetItemCount()){
		SetItemState(delItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	}
	else{
		delItem = GetItemCount() - 1;
		SetItemState(delItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		m_SelectItem = delItem;
		m_SelectSubItem = delItem;
	}
	return(m_SelectItem);
}
//------------------------------------------------------------------------------------------//
int CDevListCtrl::UpNode(int moveItem){
	IPCOMNAME	*moveNode;
	int			PriorItem;
	if (moveItem < 1)
		return(moveItem);
	PriorItem = moveItem - 1;

	moveNode = (IPCOMNAME*)TNF::FindInDownChainByDRNodeID(m_IPCOMLIST, GetItemData(moveItem));
	TNF::MovePrior(moveNode,moveNode);
	DeleteItem(moveItem);
	moveNode->rwLock.R_set();
	if (moveNode->blAvailable != 0){
		if (moveNode->typeID == PublicDevice_DEVID_APICOM)
			InsertItem(PriorItem,_T("COM"));
		if (moveNode->typeID == PublicDevice_DEVID_TCPClient)
			InsertItem(PriorItem,_T("TCP"));
		if (moveNode->typeID == PublicDevice_DEVID_UDPClient)
			InsertItem(PriorItem,_T("UDP"));
		if (moveNode->typeID == PublicDevice_DEVID_TCPServer)
			InsertItem(PriorItem,_T("TCP SERVER"));
		if (moveNode->typeID == PublicDevice_DEVID_UDPServer)
			InsertItem(PriorItem,_T("UDP SERVER"));
	}
	else{
		InsertItem(PriorItem,_T("Unavailable"));
	}
	SetItemText(PriorItem,1,Str_ANSIToUnicode(moveNode->strIPComName).c_str());
	SetItemText(PriorItem,2,Str_ANSIToUnicode(Str_ToStr(moveNode->portBaudrate)).c_str());
	SetItemText(PriorItem,3,Str_ANSIToUnicode(moveNode->TCPTypeOrFriendlyName).c_str());
	SetItemText(PriorItem,4,Str_ANSIToUnicode(moveNode->strUserDefineName).c_str());
	SetItemData(PriorItem, TNF::GetDRNodeID(moveNode));
	moveNode->rwLock.R_clr();

	if (m_SelectItem == moveItem){
		SetItemState(m_SelectItem, 0, LVIS_SELECTED|LVIS_FOCUSED);
		SetItemState(PriorItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		m_SelectItem = PriorItem;
		m_SelectSubItem = PriorItem;
		EnsureVisible(m_SelectSubItem, FALSE);
	}
	return(PriorItem);
}
//------------------------------------------------------------------------------------------//
int CDevListCtrl::DownNode(int moveItem){
	if (moveItem >= (GetItemCount() - 1))
		return(moveItem);
	UpNode(moveItem + 1);
	if (m_SelectItem == moveItem){
		SetItemState(m_SelectItem, 0, LVIS_SELECTED|LVIS_FOCUSED);
		SetItemState(moveItem + 1, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		m_SelectItem = moveItem + 1;
		m_SelectSubItem = moveItem + 1;
		EnsureVisible(m_SelectSubItem, FALSE);
	}
	return 0;
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CDlgDevList, CDialog)
//------------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CDlgDevList, CDialog)
	ON_COMMAND(ID_TOOLBAR_DL_ADD, &CDlgDevList::OnDL_Add)
	ON_COMMAND(ID_TOOLBAR_DL_DEL, &CDlgDevList::OnDL_Del)
	ON_COMMAND(ID_TOOLBAR_DL_UP, &CDlgDevList::OnDL_Up)
	ON_COMMAND(ID_TOOLBAR_DL_DOWN, &CDlgDevList::OnDL_Down)
	ON_COMMAND(ID_TOOLBAR_DL_REFRESH, &CDlgDevList::OnDL_Refresh)
	ON_COMMAND(ID_TOOLBAR_DL_LOAD, &CDlgDevList::OnDL_Load)
	ON_COMMAND(ID_TOOLBAR_DL_IMPORT, &CDlgDevList::OnDL_Import)
	ON_COMMAND(ID_TOOLBAR_DL_SAVE, &CDlgDevList::OnDL_Save)
END_MESSAGE_MAP()
//------------------------------------------------------------------------------------------//
CDlgDevList::CDlgDevList(CWnd* pParent /*=NULL*/) : CDialog(CDlgDevList::IDD, pParent){
}
//------------------------------------------------------------------------------------------//
void CDlgDevList::DoDataExchange(CDataExchange* pDX){
	CDialog::DoDataExchange(pDX);
}
//------------------------------------------------------------------------------------------//
BOOL CDlgDevList::OnInitDialog(){
	CDialog::OnInitDialog();

	if (CreateDevListBox() == -1)
		return FALSE;
	if (CreateDevListToolBar() == -1)
		return FALSE;
	AdjustLayout();
	return TRUE;
}
//------------------------------------------------------------------------------------------//
void CDlgDevList::AdjustLayout(void){
	if (GetSafeHwnd() == NULL)
		return;

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndDevList.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}
//------------------------------------------------------------------------------------------//
int CDlgDevList::CreateDevListToolBar(void){
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_TOOLBAR_DL);
	m_wndToolBar.LoadToolBar(IDR_TOOLBAR_DL, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);
	
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);// All commands will be routed via this control , not via the parent frame:

	return 0;
}
//------------------------------------------------------------------------------------------//
void CDlgDevList::OnChangeVisualStyle(void){
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_DLBarButton_hc : IDR_TOOLBAR_DL, 0, 0, TRUE /* Locked */);
}
//------------------------------------------------------------------------------------------//
int CDlgDevList::CreateDevListBox(void){
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_wndDevList.Create(WS_BORDER | WS_CHILD | WS_VISIBLE | LVS_REPORT, rectDummy, this, 0)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	m_wndDevList.LoadData(&theApp.GSDTApp.m_IPComList);
	return 0;
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void CDlgDevList::OnDL_Add(void){
	m_wndDevList.CreateNode(m_wndDevList.GetSelectedNode());
}
//------------------------------------------------------------------------------------------//
void CDlgDevList::OnDL_Del(void){
	m_wndDevList.DelNode(m_wndDevList.GetSelectedNode());
}
//------------------------------------------------------------------------------------------//
void CDlgDevList::OnDL_Up(void){
	m_wndDevList.UpNode(m_wndDevList.GetSelectedNode());
}
//------------------------------------------------------------------------------------------//
void CDlgDevList::OnDL_Down(void){
	m_wndDevList.DownNode(m_wndDevList.GetSelectedNode());
}
//------------------------------------------------------------------------------------------//
void CDlgDevList::OnDL_Refresh(void){
	m_wndDevList.LoadData(&theApp.GSDTApp.m_IPComList);
}
//------------------------------------------------------------------------------------------//
void CDlgDevList::OnDL_Load(void){
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
		readType = READINITYPE_NONE | READINITYPE_AddDeviceList | READINITYPE_CoverDeviceList;
		theApp.GSDTApp.m_ParRecord.Load(Str_UnicodeToANSI(strwText).c_str(),readType);
		m_wndDevList.LoadData(&theApp.GSDTApp.m_IPComList);
	}
}
//------------------------------------------------------------------------------------------//
void CDlgDevList::OnDL_Import(void){
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
		readType = READINITYPE_NONE | READINITYPE_AddDeviceList;
		theApp.GSDTApp.m_ParRecord.Load(Str_UnicodeToANSI(strwText).c_str(),readType);
		m_wndDevList.LoadData(&theApp.GSDTApp.m_IPComList);
	}
}
//------------------------------------------------------------------------------------------//
void CDlgDevList::OnDL_Save(void){
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
		readType = READINITYPE_NONE | READINITYPE_AddDeviceList;
		theApp.GSDTApp.m_ParRecord.Save(Str_UnicodeToANSI(strwText),readType);
	}
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//