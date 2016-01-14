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

// MainFrm.cpp : implementation of the CMainFrame class
//
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Global.h"
#include "SDT.h"
#include "MainFrm.h"
#include <versionhelpers.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//------------------------------------------------------------------------------------------//
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_MESSAGE(WM_SETTEXT,&CMainFrame::OnSetText)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)

	ON_COMMAND(ID_RI_COMBO_COM, &CMainFrame::OnCOMBO_COM)
	ON_COMMAND(ID_RI_COMBO_BR, &CMainFrame::OnCOMBO_BR)
#ifdef	SWVERSION_COM
	ON_COMMAND(ID_RI_CHECK_DSR_FLOW, &CMainFrame::OnDSR_FLOW)
	ON_COMMAND(ID_RI_CHECK_CTS_FLOW, &CMainFrame::OnCTS_FLOW)
	ON_COMMAND(ID_RI_CHECK_DTR_HIGH, &CMainFrame::OnDTRHigh)
	ON_COMMAND(ID_RI_CHECK_DTR_LOW, &CMainFrame::OnDTRLow)
	ON_COMMAND(ID_RI_CHECK_RTS_HIGH, &CMainFrame::OnRTSHigh)
	ON_COMMAND(ID_RI_CHECK_RTS_LOW, &CMainFrame::OnRTSLow)
#endif
	ON_COMMAND(ID_RI_CHECK_ASCII,&CMainFrame::OnASCII)
	ON_COMMAND(ID_RI_CHECK_HEX,&CMainFrame::OnHEX)
	ON_COMMAND(ID_RI_CHECK_MSR,&CMainFrame::OnMSR)
	ON_COMMAND(ID_RI_CHECK_OSPMSG,&CMainFrame::OnOSPMSG)
	ON_COMMAND(ID_RI_CHECK_RECMSG,&CMainFrame::OnRECMSG)
	ON_COMMAND(ID_RI_CHECK_VIEW,&CMainFrame::OnDisplay)
	ON_COMMAND(ID_RI_BUTTON_CONNECT, &CMainFrame::OnButtonClickConnect)
	ON_COMMAND(ID_RI_BUTTON_ECHO, &CMainFrame::OnButtonClickEcho)
#ifdef SWVERSION_SCRIPT
	ON_COMMAND(ID_RI_BUTTON_SPRIPT_STOP, &CMainFrame::OnButtonClickScriptStop)
	ON_COMMAND(ID_RI_BUTTON_SCRIPT_AT,&CMainFrame::OnButtonClickScriptAT)
	ON_COMMAND(ID_RI_BUTTON_SCRIPT_CE,&CMainFrame::OnButtonClickScriptCE)
	ON_COMMAND(ID_RI_BUTTON_SCRIPT_SHOWSBIC, &CMainFrame::OnButtonClickScriptShowSBIC)
	ON_COMMAND(ID_RI_BUTTON_SC,&CMainFrame::OnButtonClickScriptSC)
	ON_COMMAND(ID_RI_BUTTON_GC,&CMainFrame::OnButtonClickScriptGC)
#endif
	ON_COMMAND(ID_RI_BUTTON_CLEANVIEW, &CMainFrame::OnButtonClickClean)
	ON_COMMAND(ID_RI_BUTTON_DEVLIST, &CMainFrame::OnButtonClickDevList)
	ON_COMMAND(ID_RI_BUTTON_SOCKETLIST,&CMainFrame::OnButtonClickScoketlist)
	ON_COMMAND(ID_RI_BUTTON_OPENRFD,&CMainFrame::OnButtonClickRecodrDir)
	ON_COMMAND(ID_RI_BUTTON_NEWRECORD,&CMainFrame::OnButtonClickNewRecord)
#ifdef SWVERSION_TOOLS
	ON_COMMAND(ID_RI_BUTTON_SEND,&CMainFrame::OnButtonClickSend)
	ON_COMMAND(ID_RI_BUTTON_CACL,&CMainFrame::OnButtonClickCacl)
#endif
	ON_UPDATE_COMMAND_UI(ID_RI_COMBO_COM,&CMainFrame::OnUpdateCOMBO_COM)
	ON_UPDATE_COMMAND_UI(ID_RI_COMBO_BR,&CMainFrame::OnUpdateCOMBO_BR)
#ifdef	SWVERSION_COM
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_DSR_FLOW,&CMainFrame::OnUpdateDSR_FLOW)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_CTS_FLOW,&CMainFrame::OnUpdateCTS_FLOW)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_DTR_HIGH,&CMainFrame::OnUpdateDTRHigh)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_DTR_LOW,&CMainFrame::OnUpdateDTRLow)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_RTS_HIGH,&CMainFrame::OnUpdateRTSHigh)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_RTS_LOW,&CMainFrame::OnUpdateRTSLow)
#endif
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_ASCII,&CMainFrame::OnUpdateASCII)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_HEX,&CMainFrame::OnUpdateHEX)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_MSR,&CMainFrame::OnUpdateMSR)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_OSPMSG,&CMainFrame::OnUpdateOSPMSG)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_RECMSG,&CMainFrame::OnUpdateRECMSG)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_VIEW,&CMainFrame::OnUpdateDisplay)
	ON_UPDATE_COMMAND_UI(ID_RI_BUTTON_CONNECT,&CMainFrame::OnUpdateConnect)
	ON_UPDATE_COMMAND_UI(ID_RI_BUTTON_ECHO,&CMainFrame::OnUpdateEcho)
	ON_UPDATE_COMMAND_UI(ID_RI_BUTTON_SOCKETLIST,&CMainFrame::OnUpdateScoketlist)
#ifdef	SWVERSION_SCRIPT
	ON_UPDATE_COMMAND_UI(ID_RI_BUTTON_SPRIPT_STOP,&CMainFrame::OnUpdateScrpit)
	ON_UPDATE_COMMAND_UI(ID_RI_BUTTON_SCRIPT_AT,&CMainFrame::OnUpdateScriptAT)
	ON_UPDATE_COMMAND_UI(ID_RI_BUTTON_SCRIPT_CE,&CMainFrame::OnUpdateScriptCE)
	ON_UPDATE_COMMAND_UI(ID_RI_BUTTON_SCRIPT_SHOWSBIC, &CMainFrame::OnUpdateScriptShowSBIC)
	ON_COMMAND(ID_TOOLBAR_GC_EDIT, &CMainFrame::OnButtonClickGCEdit)
	ON_COMMAND(ID_TOOLBAR_GCP_COMMAND, &CMainFrame::OnButtonClickGCPCOMMAND)
#endif
	ON_UPDATE_COMMAND_UI(ID_STATUSBAR_RX,&CMainFrame::OnUpdateSBAR_RX)
	ON_UPDATE_COMMAND_UI(ID_STATUSBAR_TX,&CMainFrame::OnUpdateSBAR_TX)
	ON_UPDATE_COMMAND_UI(ID_STATUSBAR_CTS,&CMainFrame::OnUpdateSBAR_CTS)
	ON_UPDATE_COMMAND_UI(ID_STATUSBAR_DSR,&CMainFrame::OnUpdateSBAR_DSR)
	ON_UPDATE_COMMAND_UI(ID_STATUSBAR_RING,&CMainFrame::OnUpdateSBAR_RING)
	ON_UPDATE_COMMAND_UI(ID_STATUSBAR_DCD,&CMainFrame::OnUpdateSBAR_DCD)
	ON_UPDATE_COMMAND_UI(ID_STATUSBAR_GETIP,&CMainFrame::OnUpdateSBAR_GetIP)
	ON_UPDATE_COMMAND_UI(ID_STATUSBAR_NONE,&CMainFrame::OnUpdateSBAR_NONE)
	ON_UPDATE_COMMAND_UI(ID_STATUSBAR_IP,&CMainFrame::OnUpdateSBAR_IP)
	ON_COMMAND(ID_STATUSBAR_GETIP, &CMainFrame::OnSBAR_GetIP)
	ON_WM_TIMER()

	ON_COMMAND(ID_APP_ABOUT,&CMainFrame::OnAppAbout)
	ON_MESSAGE(WM_ON_PRINTOUT,&CMainFrame::OnPrintout)
#ifdef	SWVERSION_AUXCOM
	ON_COMMAND(ID_RI_COMBO_AUXCOM, &CMainFrame::OnRiComboAuxcom)
	ON_COMMAND(ID_RI_COMBO_AUXBR, &CMainFrame::OnRiComboAuxbr)
	ON_COMMAND(ID_RI_CHECK_AUXESCAPE, &CMainFrame::OnRiCheckAuxescape)
	ON_COMMAND(ID_RI_CHECK_AUXHEX, &CMainFrame::OnRiCheckAuxhex)
	ON_COMMAND(ID_RI_CHECK_AUXASCII, &CMainFrame::OnRiCheckAuxascii)
	ON_COMMAND(ID_RI_BUTTON_AUXOPEN, &CMainFrame::OnRiButtonAuxopen)
	ON_COMMAND(ID_RI_CHECK_AUXDSR, &CMainFrame::OnRiCheckAuxdsr)
	ON_COMMAND(ID_RI_CHECK_AUXDTRH, &CMainFrame::OnRiCheckAuxdtrh)
	ON_COMMAND(ID_RI_CHECK_AUXDTRL, &CMainFrame::OnRiCheckAuxdtrl)
	ON_COMMAND(ID_RI_CHECK_AUXCTS, &CMainFrame::OnRiCheckAuxcts)
	ON_COMMAND(ID_RI_CHECK_AUXRTSH, &CMainFrame::OnRiCheckAuxrtsh)
	ON_COMMAND(ID_RI_CHECK_AUXRTSL, &CMainFrame::OnRiCheckAuxrtsl)
	ON_UPDATE_COMMAND_UI(ID_RI_COMBO_AUXCOM, &CMainFrame::OnUpdateRiComboAuxcom)
	ON_UPDATE_COMMAND_UI(ID_RI_COMBO_AUXBR, &CMainFrame::OnUpdateRiComboAuxbr)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_AUXESCAPE, &CMainFrame::OnUpdateRiCheckAuxescape)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_AUXHEX, &CMainFrame::OnUpdateRiCheckAuxhex)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_AUXASCII, &CMainFrame::OnUpdateRiCheckAuxascii)
	ON_UPDATE_COMMAND_UI(ID_RI_BUTTON_AUXOPEN, &CMainFrame::OnUpdateRiButtonAuxopen)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_AUXDSR, &CMainFrame::OnUpdateRiCheckAuxdsr)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_AUXDTRH, &CMainFrame::OnUpdateRiCheckAuxdtrh)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_AUXDTRL, &CMainFrame::OnUpdateRiCheckAuxdtrl)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_AUXCTS, &CMainFrame::OnUpdateRiCheckAuxcts)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_AUXRTSH, &CMainFrame::OnUpdateRiCheckAuxrtsh)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_AUXRTSL, &CMainFrame::OnUpdateRiCheckAuxrtsl)
#endif
#ifdef	SWVERSION_CMUX	
	ON_COMMAND(ID_RI_CMUX_START, &CMainFrame::OnCMUXStart)
	ON_COMMAND(ID_RI_CMUX_START1, &CMainFrame::OnCMUXStart1)
	ON_COMMAND(ID_RI_CMUX_CLD, &CMainFrame::OnCMUXCLD)
	ON_COMMAND(ID_RI_CMUX_STDPSC, &CMainFrame::OnCMUXPSC)
	ON_COMMAND(ID_RI_CMUX_PSC0, &CMainFrame::OnCMUXPSC0)
	ON_COMMAND(ID_RI_CMUX_PSC1, &CMainFrame::OnCMUXPSC1)
	ON_COMMAND(ID_RI_CMUX_PSC7, &CMainFrame::OnCMUXPSC7)
	ON_COMMAND(ID_RI_CMUX_PSC9, &CMainFrame::OnCMUXPSC9)
	ON_COMMAND(ID_RI_CMUX_FC, &CMainFrame::OnCMUXFC)
	ON_UPDATE_COMMAND_UI(ID_RI_CMUX_FC, &CMainFrame::OnUpdateCMUXFC)
	ON_UPDATE_COMMAND_UI(ID_RI_CMUX_START, &CMainFrame::OnUpdateCMUXStart)
	ON_UPDATE_COMMAND_UI(ID_RI_CMUX_START1, &CMainFrame::OnUpdateCMUXStart1)
#endif
	END_MESSAGE_MAP()
//------------------------------------------------------------------------------------------//
CMainFrame::CMainFrame(void){
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_WINDOWS_7);
	m_blDSRFlow = FALSE;
	m_blCTSFlow = FALSE;
	m_blAuxDSRFlow = FALSE;
	m_blAuxCTSFlow = FALSE;
	m_blAuxComConnect = FALSE;
	m_blCMUXFC = FALSE;
	m_blStop = 1;
	m_blConnect = 1;
	m_oDevNodeAux.UnInit();
	m_CMUXFrm = NULL;
	m_MCFrm = NULL;
	m_CMUXView = NULL;
	m_localIP = "255.255.255.255";
	m_Pitl3Created = FALSE;
	m_MyTitle = "";
	m_MBIM_DevService = new MBIM_DevService(NULL,1024);
}
//------------------------------------------------------------------------------------------//
CMainFrame::~CMainFrame(void){
}
//------------------------------------------------------------------------------------------//
LRESULT CMainFrame::OnSetText(WPARAM wParam,LPARAM lParam){
	m_strTitle = m_MyTitle + _T(SWVERSION);
	return(CMDIFrameWndEx::OnSetText(m_strTitle));
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnClose(void){
	theApp.m_bCanClose = TRUE;
	m_oDevNodeAux.UnInit();
	GSDTApp.m_Device2.Close(0);
	GSDTApp.m_Device.Close(0);
	m_Pitl3->Release();
	m_Pitl3Created = FALSE;
	KillTimer(1);
	KillTimer(2);
	CMDIFrameWndEx::OnClose();
}
//------------------------------------------------------------------------------------------//
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct){
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetTimer(1, 100, 0);
	SetTimer(2, 200, 0);
	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // other styles available...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;	// set to FALSE to place close button at right of tab area
	mdiTabParams.m_bTabIcons = TRUE;				// set to TRUE to enable document icons on MDI taba
	mdiTabParams.m_bAutoColor = TRUE;				// set to FALSE to disable auto-coloring of MDI tabs
	mdiTabParams.m_bDocumentMenu = TRUE;			// enable the document menu at the right edge of the tab area
	mdiTabParams.m_bTabCloseButton = FALSE;
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	if (IsWin7()){
		m_wndRibbonBar.Create(this);
		m_wndRibbonBar.LoadFromResource(IDR_RIBBON);
		InitRibbonBar();
	}

	if (!m_wndStatusBar.Create(this)){
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);
	// Load menu item image (not placed on any standard toolbars):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : IDB_MENU_IMAGES_24);


	// create docking windows
	if (CreateDockingWindows() == FALSE){
		TRACE0("Failed to create docking windows\n");
		return -1;
	}

	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	// Enable enhanced windows management dialog
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// Switch the order of document name and application name on the window title bar. This
	// improves the usability of the taskbar because the document name is visible with the thumbnail.
	ModifyStyle(0, FWS_PREFIXTITLE);

	UIInit();
	return 0;
}
//------------------------------------------------------------------------------------------//
BOOL CMainFrame::IsWin7(void){
	// win7的系统版本为NT6.1
	if (IsWindows7OrGreater())
		return TRUE;
	return FALSE;
} 
//------------------------------------------------------------------------------------------//
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs){
	if(!CMDIFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.cx = 940;
	cs.cy = 670;
	cs.x = (GetSystemMetrics(SM_CXSCREEN) - cs.cx)/2;
	cs.y = (GetSystemMetrics(SM_CYSCREEN) - cs.cy)/2 - 20; 
	return TRUE;
}
//------------------------------------------------------------------------------------------//
BOOL CMainFrame::CreateDockingWindows(){

	if (!m_wndSC.Create(_T("Single Command"), this, CRect(0, 145, 300, 145 + 494), TRUE, ID_VIEW_SCVIEW
		, WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI)){
		TRACE0("Failed to create File View window\n");
		return FALSE;
	}

	if (!m_wndGC.Create(_T("Group Command"), this, CRect(0, 145, 300, 145 + 494), TRUE, ID_VIEW_GCVIEW
		, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI)){
		TRACE0("Failed to create Class View window\n");
		return FALSE;
	}

	if (!m_wndGCProperties.Create(_T("GC Properties"), this, CRect(300, 145, 300 + 300, 145 + 494), TRUE, ID_VIEW_GCPROPERTIESVIEW
		, WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI)){
		TRACE0("Failed to create Properties window\n");
		return FALSE;
	}

	if (!m_wndGCCommand.Create(_T("Command List"), this, CRect(300, 145, 300 + 830, 145 + 494), TRUE, ID_VIEW_GCCOMVIEW
		, WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_ALIGN_BOTTOM | CBRS_FLOAT_MULTI)){
		TRACE0("Failed to create Properties window\n");
		return FALSE; // failed to create
	}

	if (!m_wndSocket.Create(_T("Socket List"), this, CRect(925, 145, 925 + 210, 145 + 494), TRUE, ID_VIEW_SCOKETVIEW
		, WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI)){
		TRACE0("Failed to create Properties window\n");
		return FALSE; // failed to create
	}
	if (!m_wndSend.Create(_T("Send"), this, CRect(0, 145, 300, 145 + 494), TRUE, ID_VIEW_SENDVIEW
		, WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT |CBRS_FLOAT_MULTI)){
		TRACE0("Failed to create Properties window\n");
		return FALSE; // failed to create
	}
	if (!m_wndCacl.Create(_T("Calculator"), this, CRect(0, 145, 300, 145 + 494), TRUE, ID_VIEW_CACLVIEW
		, WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT |CBRS_FLOAT_MULTI)){
		TRACE0("Failed to create Properties window\n");
		return FALSE; // failed to create
	}
	SetDockingWindowIcons(theApp.m_bHiColorIcons);

	m_wndSC.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndSC);

	m_wndGC.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndGC);
	
	m_wndGCCommand.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndGCCommand);

	m_wndGCProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndGCProperties);

	m_wndSocket.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndSocket);

	m_wndSend.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndSend);

	m_wndCacl.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndCacl);

	CDockablePane *pTabbedBar2 = NULL;
	m_wndGC.AttachToTabWnd(&m_wndSC, DM_SHOW, TRUE, &pTabbedBar2);
	m_wndSC.ShowPane(FALSE,FALSE,TRUE);

	CDockablePane *pTabbedBar = NULL;
	m_wndSend.AttachToTabWnd(&m_wndSC, DM_SHOW, TRUE, &pTabbedBar);
	m_wndSend.ShowPane(FALSE,FALSE,TRUE);

	CDockablePane *pTabbedBar3 = NULL;
	m_wndCacl.AttachToTabWnd(&m_wndSC, DM_SHOW, TRUE, &pTabbedBar3);
	m_wndCacl.ShowPane(FALSE,FALSE,TRUE);

	m_wndGC.ShowPane(FALSE,FALSE,TRUE);

	return TRUE;
}
//------------------------------------------------------------------------------------------//
void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons){
	HICON hSCViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_SCWND : IDI_SCWND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndSC.SetIcon(hSCViewIcon, FALSE);

	HICON hGCViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_GCWND : IDI_GCWND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndGC.SetIcon(hGCViewIcon, FALSE);

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_GCPWND : IDI_GCPWND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndGCProperties.SetIcon(hPropertiesBarIcon, FALSE);

	HICON hGCCommandViewBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_GCCOMMAND : IDI_GCCOMMAND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndGCCommand.SetIcon(hGCCommandViewBarIcon, FALSE);

	HICON hSocketViewBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_SOCKETFRM : IDI_SOCKETFRM), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndSocket.SetIcon(hSocketViewBarIcon, FALSE);

	HICON hSendViewBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_SENDWND : IDI_SENDWND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndSend.SetIcon(hSendViewBarIcon, FALSE);
	UpdateMDITabbedBarsIcons();

	HICON hCaclViewBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CACL : IDI_CACL), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndCacl.SetIcon(hCaclViewBarIcon, FALSE);
	UpdateMDITabbedBarsIcons();
}
//------------------------------------------------------------------------------------------//
#ifdef _DEBUG
void CMainFrame::AssertValid() const{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG
//------------------------------------------------------------------------------------------//
// CMainFrame message handlers
void CMainFrame::OnWindowManager(){
	ShowWindowsDialog();
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnApplicationLook(UINT id){
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook){
		case ID_VIEW_APPLOOK_WIN_2000:
			CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
			m_wndRibbonBar.SetWindows7Look(FALSE);
			break;
		case ID_VIEW_APPLOOK_OFF_XP:
			CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
			m_wndRibbonBar.SetWindows7Look(FALSE);
			break;
		case ID_VIEW_APPLOOK_WIN_XP:
			CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
			CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
			m_wndRibbonBar.SetWindows7Look(FALSE);
			break;
		case ID_VIEW_APPLOOK_OFF_2003:
			CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
			CDockingManager::SetDockingMode(DT_SMART);
			m_wndRibbonBar.SetWindows7Look(FALSE);
			break;
		case ID_VIEW_APPLOOK_VS_2005:
			CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
			CDockingManager::SetDockingMode(DT_SMART);
			m_wndRibbonBar.SetWindows7Look(FALSE);
			break;
		case ID_VIEW_APPLOOK_VS_2008:
			CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
			CDockingManager::SetDockingMode(DT_SMART);
			m_wndRibbonBar.SetWindows7Look(FALSE);
			break;
		case ID_VIEW_APPLOOK_WINDOWS_7:
			CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
			CDockingManager::SetDockingMode(DT_SMART);
			m_wndRibbonBar.SetWindows7Look(TRUE);
			break;
		default:
			switch (theApp.m_nAppLook){
				case ID_VIEW_APPLOOK_OFF_2007_BLUE:
					CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
					break;

				case ID_VIEW_APPLOOK_OFF_2007_BLACK:
					CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
					break;

				case ID_VIEW_APPLOOK_OFF_2007_SILVER:
					CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
					break;

				case ID_VIEW_APPLOOK_OFF_2007_AQUA:
					CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
					break;
			}
			CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
			CDockingManager::SetDockingMode(DT_SMART);
			m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI){
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::UIInit(void){
	CCreateContext context;

	context.m_pNewViewClass = RUNTIME_CLASS(CMyRichView);
	m_MCFrm = new CChildFrame();
	m_MCFrm->LoadFrame(IDI_MCFRM,WS_CHILD | WS_OVERLAPPEDWINDOW,this,&context);
	m_MCFrm->SetWindowText(_T("Main View"));
	m_MCFrm->ShowWindow(SW_SHOW);
	m_MCFrm->InitialUpdateFrame(NULL,true);

	m_MCFrm->oDevNode = GSDTApp.m_oDevOutputListPool.CreateODevSDOUT((CRichEditView*)m_MCFrm->GetActiveView(),NULL);

	((CMyRichView*)m_MCFrm->GetActiveView())->m_blEnableConsole = TRUE;
}
//------------------------------------------------------------------------------------------//
void CMainFrame::AuxCFrmCreate(void){
	if (m_oDevNodeAux.cgCFrm == NULL){
		CCreateContext	context;
		CChildFrame		*childpFrm;
		context.m_pNewViewClass = RUNTIME_CLASS(CMyRichView);
		childpFrm = new CChildFrame();
		childpFrm->LoadFrame(IDI_MCFRM,WS_CHILD | WS_OVERLAPPEDWINDOW,this,&context);
		childpFrm->SetWindowText(_T("Aux View"));
		childpFrm->ShowWindow(SW_SHOW);
		childpFrm->InitialUpdateFrame(NULL,true);
		m_oDevNodeAux.UnInit();
		m_oDevNodeAux.Init((CRichEditView*)childpFrm->GetActiveView(),childpFrm);
		childpFrm->oDevNode = &m_oDevNodeAux;
		childpFrm->m_bCanClose = TRUE;
	}
	else{
		m_oDevNodeAux.cgCFrm->MDIActivate();
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::CMUXFrmCreate(void){
	if (m_CMUXFrm == NULL){
		CCreateContext	context;
		CChildFrame		*childpFrm;
		context.m_pNewViewClass = RUNTIME_CLASS(CCMUXView);
		childpFrm = new CChildFrame();
		childpFrm->LoadFrame(IDI_MCFRM,WS_CHILD | WS_OVERLAPPEDWINDOW,this,&context);
		childpFrm->SetWindowText(_T("CMUX COM setting"));
		childpFrm->ShowWindow(SW_SHOW);
		childpFrm->InitialUpdateFrame(NULL,true);
		m_CMUXFrm = childpFrm;
		m_CMUXView = (CCMUXView*)m_CMUXFrm->GetActiveView();
	}
	else{
		//m_CMUXFrm->MDIActivate();
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::InitRibbonBar(void){
	InitComboBox_BR();
	InitComboBox_COM();
#ifdef	SWVERSION_AUXCOM
	InitComboBox_AuxBR();
	InitComboBox_AuxCOM();
#endif
}
//------------------------------------------------------------------------------------------//
void CMainFrame::InitComboBox_COM(void){
	CreateComboBox_COMList();
	m_ValidIPComList.selectedNode = (IPCOMNAME*)RTREE_NODE::GetLastChild(&m_ValidIPComList);
	SetSelectComboBox_COM();
}
//------------------------------------------------------------------------------------------//
void CMainFrame::InitComboBox_BR(void){
	CMFCRibbonComboBox *BRBox;
	BRBox = (CMFCRibbonComboBox*)m_wndRibbonBar.FindByID(ID_RI_COMBO_BR);
	BRBox->AddItem(_T("1200"));
	BRBox->AddItem(_T("2400"));
	BRBox->AddItem(_T("4800"));
	BRBox->AddItem(_T("9600"));
	BRBox->AddItem(_T("14400"));
	BRBox->AddItem(_T("19200"));
	BRBox->AddItem(_T("38400"));
	BRBox->AddItem(_T("56000"));
	BRBox->AddItem(_T("57600"));
	BRBox->AddItem(_T("115200"));
	BRBox->AddItem(_T("128000"));
	BRBox->AddItem(_T("230400"));
	BRBox->AddItem(_T("256000"));
	BRBox->AddItem(_T("460800"));
	BRBox->AddItem(_T("912600"));
	BRBox->SelectItem(9);
	BRBox->SetDropDownHeight(10 * 22);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::SetSelectComboBox_COM(void){
	CMFCRibbonComboBox *COMBox,*BRBox;
	CString		sstrPortName,sstrShowName,sstrBR;
	IPCOMNAME	*listItem;

	if (m_ValidIPComList.selectedNode == NULL)
		return;

	COMBox = (CMFCRibbonComboBox*)m_wndRibbonBar.FindByID(ID_RI_COMBO_COM);
	BRBox = (CMFCRibbonComboBox*)m_wndRibbonBar.FindByID(ID_RI_COMBO_BR);

	listItem = m_ValidIPComList.selectedNode;
	
	if (listItem->typeID == PublicDevice_DEVID_APICOM)
		sstrPortName = listItem->strIPComName.c_str();
	if (listItem->typeID == PublicDevice_DEVID_TCPClient){
		sstrPortName = "TCP:";
		sstrPortName += listItem->strIPComName.c_str();
	}
	if (listItem->typeID == PublicDevice_DEVID_UDPClient){
		sstrPortName = "UDP:";
		sstrPortName += listItem->strIPComName.c_str();
	}
	if (listItem->typeID == PublicDevice_DEVID_TCPServer)
		sstrPortName = "TCP SERVER";
	if (listItem->typeID == PublicDevice_DEVID_UDPServer)
		sstrPortName = "UDP SERVER";
	sstrShowName = listItem->strShowName.c_str();
	COMBox->SetEditText(sstrPortName);
	sstrBR = Str_IntToString(listItem->portBaudrate).c_str();
	BRBox->SetEditText(sstrBR);
}
//---------------------------------------------------------------------------
void CMainFrame::CreateComboBox_COMList(void){
	CString		sstrPortName,sstrShowName,sstrBackup;
	int32		comBoxHigh;
	CMFCRibbonComboBox *COMBox;

	COMBox = (CMFCRibbonComboBox*)m_wndRibbonBar.FindByID(ID_RI_COMBO_COM);
	sstrBackup = COMBox->GetEditText();
	COMBox->RemoveAllItems();

	GSDTApp.m_IPComList.Refresh();
	GSDTApp.m_IPComList.Export(&m_ValidIPComList);

	m_ValidIPComList.Spin_InUse_set();

	RTREE_LChildRChain_Traversal_LINE(IPCOMNAME, (&m_ValidIPComList),
		if (operateNode_t->typeID == PublicDevice_DEVID_APICOM)
			sstrPortName = operateNode_t->strIPComName.c_str();
		if (operateNode_t->typeID == PublicDevice_DEVID_TCPClient){
			sstrPortName = "TCP:";
			sstrPortName += operateNode_t->strIPComName.c_str();
		}
		if (operateNode_t->typeID == PublicDevice_DEVID_UDPClient){
			sstrPortName = "UDP:";
			sstrPortName += operateNode_t->strIPComName.c_str();
		}
		if (operateNode_t->typeID == PublicDevice_DEVID_TCPServer)
			sstrPortName = "TCP SERVER";
		if (operateNode_t->typeID == PublicDevice_DEVID_UDPServer)
			sstrPortName = "UDP SERVER";
		sstrShowName = operateNode_t->strShowName.c_str();
		COMBox->AddItem(sstrShowName, RTREE_NODE::GetdRNodeID(operateNode_t));
		if (sstrBackup == sstrPortName)
			m_ValidIPComList.selectedNode = operateNode_t;
	);
	m_ValidIPComList.Spin_InUse_clr();
	if (m_ValidIPComList.selectedNode != NULL)
		COMBox->SelectItem((DWORD_PTR)RTREE_NODE::GetdRNodeID(m_ValidIPComList.selectedNode));
	COMBox->SetEditText(sstrBackup);
	comBoxHigh = COMBox->GetCount() * 22;
	if (comBoxHigh > 15 * 22)
		comBoxHigh = 15 * 22;
	COMBox->SetDropDownHeight(comBoxHigh);
	COMBox->EnableDropDownListResize(TRUE);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnCOMBO_COM(void){
	int	id;

	CMFCRibbonComboBox *COMBox;
	COMBox = (CMFCRibbonComboBox*)m_wndRibbonBar.FindByID(ID_RI_COMBO_COM);
	id = COMBox->FindItem(COMBox->GetEditText());
	if (id >= 0){
		id = COMBox->GetItemData(id);
		m_ValidIPComList.selectedNode = (IPCOMNAME*)RTREE_NODE::FindInLChildRChainByDRNodeID(&m_ValidIPComList,id);
		SetSelectComboBox_COM();
	}
	else{
		m_ValidIPComList.selectedNode = NULL;
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnCOMBO_BR(void){	
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateCOMBO_COM(CCmdUI *pCmdUI){
	pCmdUI->Enable(GSDTApp.m_Device.CheckblConnect() == 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateCOMBO_BR(CCmdUI *pCmdUI){
	pCmdUI->Enable(GSDTApp.m_Device.CheckblConnect() == 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnButtonClickScoketlist(void){	
	if (m_wndSocket.IsVisible() == FALSE){
		m_wndSocket.ShowPane(TRUE,FALSE,TRUE);
		m_wndSocket.SetAutoHideMode(FALSE,CBRS_RIGHT);
	}
	else if (m_wndSocket.IsPaneVisible() == FALSE){
		m_wndSocket.ShowPane(TRUE,FALSE,TRUE);
	}
	else{
		m_wndSocket.ShowPane(FALSE,FALSE,TRUE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateScoketlist(CCmdUI *pCmdUI){
	if ((GSDTApp.m_Device.CheckblConnect() != 0) 
		&& ((GSDTApp.m_Device.cgDevType == DEVICE::DEVID_TCPServer)
		|| (GSDTApp.m_Device.cgDevType == DEVICE::DEVID_UDPServer))){
		pCmdUI->Enable();
	}
	else{
		pCmdUI->Enable(FALSE);
	}
}
//------------------------------------------------------------------------------------------//
LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam){   
	static UINT MessageTaskbarButtonCreate;   
	switch(message){
		case WM_CREATE:
			MessageTaskbarButtonCreate = RegisterWindowMessage(TEXT("TaskbarButtonCreated"));
			break;
		default:
			if(message == MessageTaskbarButtonCreate){   
				if(SUCCEEDED(CoInitialize(NULL))){
					HRESULT hr = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_Pitl3));
					if (SUCCEEDED(hr)){
						hr = m_Pitl3->HrInit();
						m_Pitl3Created = TRUE;
					}
				}
			}
			break;
	}
	return(CMDIFrameWndEx::WindowProc(message, wParam, lParam));   
}
//------------------------------------------------------------------------------------------//
BOOL CMainFrame::PreTranslateMessage(MSG *pMsg){
	if (pMsg->message == WM_LBUTTONDOWN){
		CMFCRibbonComboBox *COMBox;
		COMBox = (CMFCRibbonComboBox*)m_wndRibbonBar.FindByID(ID_RI_COMBO_COM);
		if (pMsg->hwnd == m_wndRibbonBar.GetSafeHwnd()){
			if (COMBox->GetHighlighted() == COMBox)
				if (GSDTApp.m_Device.CheckblConnect() == 0)
					CreateComboBox_COMList();
		}
		#ifdef	SWVERSION_AUXCOM
		COMBox = (CMFCRibbonComboBox*)m_wndRibbonBar.FindByID(ID_RI_COMBO_AUXCOM);
		if (pMsg->hwnd == m_wndRibbonBar.GetSafeHwnd()){
			if (COMBox->GetHighlighted() == COMBox)
				if (GSDTApp.m_Device2.CheckblConnect() == 0)
					CreateComboBox_AuxCOMList();
		}
		#endif
	}
	return(CMDIFrameWndEx::PreTranslateMessage(pMsg));
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnDSR_FLOW(void){
	if ((GSDTApp.m_Device.cgDevType == DEVICE::DEVID_APICOM) && (GSDTApp.m_Device.CheckblConnect() != 0)){
		m_blDSRFlow = !m_blDSRFlow;
		GSDTApp.m_Device.cgAPIECom->SetDSRFlow(m_blDSRFlow);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnCTS_FLOW(void){
	if ((GSDTApp.m_Device.cgDevType == DEVICE::DEVID_APICOM) && (GSDTApp.m_Device.CheckblConnect() != 0)){
		m_blCTSFlow = !m_blCTSFlow;
		GSDTApp.m_Device.cgAPIECom->SetCTSFlow(m_blCTSFlow);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnDTRHigh(void){
	if ((GSDTApp.m_Device.cgDevType == DEVICE::DEVID_APICOM) && (GSDTApp.m_Device.CheckblConnect() != 0))
		GSDTApp.ExecBIC("main dtr -H");
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnDTRLow(void){
	if ((GSDTApp.m_Device.cgDevType == DEVICE::DEVID_APICOM) && (GSDTApp.m_Device.CheckblConnect() != 0))
		GSDTApp.ExecBIC("main dtr -L");
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnRTSHigh(void){
	if ((GSDTApp.m_Device.cgDevType == DEVICE::DEVID_APICOM) && (GSDTApp.m_Device.CheckblConnect() != 0))
		GSDTApp.ExecBIC("main rts -H");
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnRTSLow(void){
	if ((GSDTApp.m_Device.cgDevType == DEVICE::DEVID_APICOM) && (GSDTApp.m_Device.CheckblConnect() != 0))
		GSDTApp.ExecBIC("main rts -L");
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateDSR_FLOW(CCmdUI *pCmdUI){
	if ((GSDTApp.m_Device.cgDevType == DEVICE::DEVID_APICOM) && (GSDTApp.m_Device.CheckblConnect() != 0)){
		pCmdUI->Enable();
		pCmdUI->SetCheck(m_blDSRFlow);
	}
	else{
		pCmdUI->SetCheck(FALSE);
		pCmdUI->Enable(FALSE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateCTS_FLOW(CCmdUI *pCmdUI){
	if ((GSDTApp.m_Device.cgDevType == DEVICE::DEVID_APICOM) && (GSDTApp.m_Device.CheckblConnect() != 0)){
		pCmdUI->Enable();
		pCmdUI->SetCheck(m_blCTSFlow);
	}
	else{
		pCmdUI->SetCheck(FALSE);
		pCmdUI->Enable(FALSE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateDTRHigh(CCmdUI *pCmdUI){
	if ((GSDTApp.m_Device.cgDevType == DEVICE::DEVID_APICOM) && (GSDTApp.m_Device.CheckblConnect() != 0)){
		pCmdUI->Enable();
		pCmdUI->SetCheck(GSDTApp.m_Device.cgAPIECom->GetDTRStatus() != 0);
	}
	else{
		pCmdUI->SetCheck(FALSE);
		pCmdUI->Enable(FALSE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateDTRLow(CCmdUI *pCmdUI){
	if ((GSDTApp.m_Device.cgDevType == DEVICE::DEVID_APICOM) && (GSDTApp.m_Device.CheckblConnect() != 0)){
		pCmdUI->SetCheck(GSDTApp.m_Device.cgAPIECom->GetDTRStatus() == 0);
	}
	else{
		pCmdUI->SetCheck();
		pCmdUI->Enable(FALSE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateRTSHigh(CCmdUI *pCmdUI){
	if ((GSDTApp.m_Device.cgDevType == DEVICE::DEVID_APICOM) && (GSDTApp.m_Device.CheckblConnect() != 0)){
		pCmdUI->Enable();
		pCmdUI->SetCheck(GSDTApp.m_Device.cgAPIECom->GetRTSStatus() != 0);
	}
	else{
		pCmdUI->SetCheck(FALSE);
		pCmdUI->Enable(FALSE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateRTSLow(CCmdUI *pCmdUI){
	if ((GSDTApp.m_Device.cgDevType == DEVICE::DEVID_APICOM) && (GSDTApp.m_Device.CheckblConnect() != 0)){
		pCmdUI->Enable();
		pCmdUI->SetCheck(GSDTApp.m_Device.cgAPIECom->GetRTSStatus() == 0);
	}
	else{
		pCmdUI->SetCheck();
		pCmdUI->Enable(FALSE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnButtonClickEcho(void){
	GSDTApp.m_Device.cEDevFlag.blEnableEcho = !GSDTApp.m_Device.cEDevFlag.blEnableEcho;
	if (GSDTApp.m_Device.cEDevFlag.blEnableEcho == 0){
		GSDTApp.m_Device.DisableEcho();
	}
	else{
		GSDTApp.m_Device.EnableEcho();
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateEcho(CCmdUI *pCmdUI){
	pCmdUI->SetCheck(GSDTApp.m_Device.cEDevFlag.blEnableEcho != 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnASCII(void){
	B_ClrFLAG64(GSDTApp.m_Device.cgODevList.cgODevFlagU64,ODEV_FLAG_EnHEXViewMode);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnHEX(void){
	B_SetFLAG64(GSDTApp.m_Device.cgODevList.cgODevFlagU64, ODEV_FLAG_EnHEXViewMode);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateASCII(CCmdUI *pCmdUI){
	pCmdUI->SetCheck(B_ChkFLAG64(GSDTApp.m_Device.cgODevList.cgODevFlagU64, ODEV_FLAG_EnHEXViewMode) == 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateHEX(CCmdUI *pCmdUI){
	pCmdUI->SetCheck(B_ChkFLAG64(GSDTApp.m_Device.cgODevList.cgODevFlagU64, ODEV_FLAG_EnHEXViewMode) != 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnMSR(void){
	if (B_ChkFLAG64(GSDTApp.m_Device.cgODevList.cgODevFlagU64, ODEV_FLAG_EnMSReport) == 0){
		B_SetFLAG64(GSDTApp.m_Device.cgODevList.cgODevFlagU64, ODEV_FLAG_EnMSReport);
	}
	else{
		B_ClrFLAG64(GSDTApp.m_Device.cgODevList.cgODevFlagU64, ODEV_FLAG_EnMSReport);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnOSPMSG(void){
	if (B_ChkFLAG64(GSDTApp.m_Device.cgODevList.cgODevFlagU64, ODEV_FLAG_EnOSPMsgLine) == 0){
		B_SetFLAG64(GSDTApp.m_Device.cgODevList.cgODevFlagU64, ODEV_FLAG_EnOSPMsgLine);
	}
	else{
		B_ClrFLAG64(GSDTApp.m_Device.cgODevList.cgODevFlagU64, ODEV_FLAG_EnOSPMsgLine);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnRECMSG(void){
	if (B_ChkFLAG64(GSDTApp.m_Device.cgODevList.cgODevFlagU64, ODEV_FLAG_EnRecMsg) == 0){
		B_SetFLAG64(GSDTApp.m_Device.cgODevList.cgODevFlagU64, ODEV_FLAG_EnRecMsg);
	}
	else{
		B_ClrFLAG64(GSDTApp.m_Device.cgODevList.cgODevFlagU64, ODEV_FLAG_EnRecMsg);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnDisplay(void){
	if (GSDTApp.m_Device.cgODevList.cgOutput->cODevSDOUT->CheckblEnabled() != 0){
		GSDTApp.m_Device.cgODevList.cgOutput->cODevSDOUT->Disable();
	}
	else{
		GSDTApp.m_Device.cgODevList.cgOutput->cODevSDOUT->Enable();
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateDisplay(CCmdUI *pCmdUI){
	pCmdUI->SetCheck(GSDTApp.m_Device.cgODevList.cgOutput->cODevSDOUT->CheckblEnabled() != 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateMSR(CCmdUI *pCmdUI){
	pCmdUI->SetCheck(B_ChkFLAG64(GSDTApp.m_Device.cgODevList.cgODevFlagU64, ODEV_FLAG_EnMSReport) != 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateOSPMSG(CCmdUI *pCmdUI){
	pCmdUI->SetCheck(B_ChkFLAG64(GSDTApp.m_Device.cgODevList.cgODevFlagU64, ODEV_FLAG_EnOSPMsgLine) != 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateRECMSG(CCmdUI *pCmdUI){
	pCmdUI->SetCheck(B_ChkFLAG64(GSDTApp.m_Device.cgODevList.cgODevFlagU64, ODEV_FLAG_EnRecMsg) != 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnButtonClickScriptAT(void){
	GSDTApp.m_Device.cEDevFlag.blScriptAT = !GSDTApp.m_Device.cEDevFlag.blScriptAT;
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnButtonClickScriptCE(void){
	GSDTApp.m_Device.cEDevFlag.blCommandExplain = !GSDTApp.m_Device.cEDevFlag.blCommandExplain;
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnButtonClickScriptShowSBIC(void){
	GSDTApp.m_Device.cEDevFlag.blEnablePrintSBICinfo = !GSDTApp.m_Device.cEDevFlag.blEnablePrintSBICinfo;
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateScriptAT(CCmdUI *pCmdUI){
	pCmdUI->SetCheck(GSDTApp.m_Device.cEDevFlag.blScriptAT != 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateScriptCE(CCmdUI *pCmdUI){
	pCmdUI->SetCheck(GSDTApp.m_Device.cEDevFlag.blCommandExplain != 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateScriptShowSBIC(CCmdUI *pCmdUI){
	pCmdUI->SetCheck(GSDTApp.m_Device.cEDevFlag.blEnablePrintSBICinfo != 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnButtonClickScriptStop(void){
	DoScriptStop();
}
//------------------------------------------------------------------------------------------//
void CMainFrame::DoScriptStop(void){
	GSDTApp.m_Script.StopRun();
	GSDTApp.m_FileSend.StopSend();
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnButtonClickGCEdit(void){
	if (m_wndGCProperties.IsVisible() == FALSE){
		m_wndGCProperties.ShowPane(TRUE,FALSE,TRUE);
		m_wndGCProperties.SetAutoHideMode(FALSE,CBRS_RIGHT);
	}
	else if (m_wndGCProperties.IsPaneVisible() == FALSE){
		m_wndGCProperties.ShowPane(TRUE,FALSE,TRUE);
	}
	else{
		m_wndGCProperties.ShowPane(FALSE,FALSE,TRUE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnButtonClickGCPCOMMAND(void){
	if (m_wndGCCommand.IsVisible() == FALSE){
		m_wndGCCommand.ShowPane(TRUE,FALSE,TRUE);
		m_wndGCCommand.SetAutoHideMode(FALSE,CBRS_RIGHT);
	}
	else if (m_wndGCCommand.IsPaneVisible() == FALSE){
		m_wndGCCommand.ShowPane(TRUE,FALSE,TRUE);
	}
	else{
		m_wndGCCommand.ShowPane(FALSE,FALSE,TRUE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnButtonClickScriptSC(void){
	if (m_wndSC.IsVisible() == FALSE){
		m_wndSC.ShowPane(TRUE,FALSE,TRUE);
		m_wndSC.SetAutoHideMode(FALSE,CBRS_LEFT);
	}
	else if (m_wndSC.IsPaneVisible() == FALSE){
		m_wndSC.ShowPane(TRUE,FALSE,TRUE);
	}
	else{
		m_wndSC.ShowPane(FALSE,FALSE,TRUE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnButtonClickScriptGC(void){
	if (m_wndGC.IsVisible() == FALSE){
		m_wndGC.ShowPane(TRUE,FALSE,TRUE);
		m_wndGC.SetAutoHideMode(FALSE,CBRS_LEFT);
	}
	else if (m_wndGC.IsPaneVisible() == FALSE){
		m_wndGC.ShowPane(TRUE,FALSE,TRUE);
	}
	else{
		m_wndGC.ShowPane(FALSE,FALSE,TRUE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnButtonClickClean(void){
	GSDTApp.m_Device.Clean();
	GSDTApp.m_Device2.Clean();
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnButtonClickDevList(void){
	m_DlgDevList.DoModal();
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnButtonClickConnect(void){

	if (GSDTApp.m_Device.CheckblConnect() == FALSE){
		if (DoConnect() == 0){
			DoScriptStop();
			GSDTApp.m_Device.Close(0);
			m_wndSocket.ShowPane(FALSE,FALSE,TRUE);
		}
		else{
			GSDTApp.m_ConsoleBICPAR.charSBUF->PutInASCII("c\r", G_ESCAPE_OFF);
		}
		return;
	}
	else{
		DoScriptStop();
		GSDTApp.m_Device.Close(1);
		m_wndSocket.ShowPane(FALSE,FALSE,TRUE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateScrpit(CCmdUI *pCmdUI){
	if (GSDTApp.m_Script.IsStop() != m_blStop){
		m_blStop = GSDTApp.m_Script.IsStop();
		if (m_blStop != 0){
			m_blConnect = !GSDTApp.m_Device.CheckblConnect();
		}
		else if (m_Pitl3Created){
			m_Pitl3->SetProgressState(GetSafeHwnd(),TBPF_INDETERMINATE);
		}
		m_wndStatusBar.SetPB(m_blStop == 0);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateConnect(CCmdUI *pCmdUI){
	CString		cTitle;
	g_wndSocketList.LoadData(GSDTApp.m_Device.cgAPISocketServer);
	if (GSDTApp.m_GCList.CheckblUpdate() != 0){
		g_wndGCTree.LoadData(&GSDTApp.m_GCList);
		GSDTApp.m_GCList.ClrblUpdate();
	}
	if (GSDTApp.m_SCList.CheckblUpdate() != 0){
		m_wndSC.m_wndSCTree.LoadData(&GSDTApp.m_SCList);
		GSDTApp.m_SCList.ClrblUpdate();
	}
	if (GSDTApp.m_Device.cgDevType == DEVICE::DEVID_TCPClient){
		if (GSDTApp.m_Device.cgAPISocket != NULL){
			if (GSDTApp.m_Device.cgAPISocket->CheckblSDC() != 0){
				if (GSDTApp.m_Script.IsRunSocketCommand() == 0){
					DoScriptStop();
					GSDTApp.m_Device.Close(0);
				}
			}
		}
	}
	pCmdUI->SetCheck(GSDTApp.m_Device.CheckblConnect() != 0);

	if (GSDTApp.m_Device.CheckblConnect() != m_blConnect){
		m_blConnect = GSDTApp.m_Device.CheckblConnect();
		if (m_blConnect != 0){
			m_MyTitle = Str_ANSIToUnicode(GSDTApp.m_Device.MakeFrmTitle()).c_str();
			if (m_MCFrm != NULL)
				m_MCFrm->SetWindowText(m_MyTitle);
			if (m_Pitl3Created){
				m_Pitl3->SetProgressValue(GetSafeHwnd(),100,100);
				m_Pitl3->SetProgressState(GetSafeHwnd(),TBPF_NORMAL);
			}
			m_MyTitle += _T(" - ");
		}
		else{
			m_MyTitle = "";
			if (m_MCFrm != NULL)
				m_MCFrm->SetWindowText(_T("Main View"));
			if (m_Pitl3Created){
				m_Pitl3->SetProgressValue(GetSafeHwnd(),100,100);
				m_Pitl3->SetProgressState(GetSafeHwnd(),TBPF_NOPROGRESS);
			}
		}
		cTitle = m_MyTitle + _T(SWVERSION);
		SetWindowText(cTitle);
	}
}
//------------------------------------------------------------------------------------------//
int32 CMainFrame::CheckCOMInput(std::string strInput,std::string *strOutput){
	std::string	result,strData;
	uint32		comNumber;
	
	result = Str_Trim(strInput);	
	strData = Str_UpperCase(result.substr(0,3));
	if (strData == "COM"){
		result = result.substr(3);
		comNumber = atoi(result.c_str());
		if ((comNumber > 0) && (comNumber <= 256)){
			*strOutput = Str_UpperCase(Str_Trim(strInput));
			return 1;
		}
	}
	*strOutput = "";
	return 0;
}
//------------------------------------------------------------------------------------------//
int32 CMainFrame::CheckTCPInput(std::string strInput,std::string *strOutput){
	std::string	result,strData;
	
	result = Str_Trim(strInput);	
	strData = Str_UpperCase(result.substr(0,4));
	if (strData == "TCP:"){
		*strOutput = Str_UpperCase(Str_Trim(result.substr(4)));
		return 1;
	}
	*strOutput = "";
	return 0;
}
//------------------------------------------------------------------------------------------//
int32 CMainFrame::CheckUDPInput(std::string strInput,std::string *strOutput){
	std::string	result,strData;
	
	result = Str_Trim(strInput);	
	strData = Str_UpperCase(result.substr(0,4));
	if (strData == "UDP:"){
		*strOutput = Str_UpperCase(Str_Trim(result.substr(4)));
		return 1;
	}
	*strOutput = "";
	return 0;
}
//------------------------------------------------------------------------------------------//
int32 CMainFrame::CheckTCPServerInput(std::string strInput,std::string *strOutput){
	*strOutput = "";
	if (Str_UpperCase(Str_Trim(strInput)) == "TCP SERVER")
		return 1;
	return 0;
}
//------------------------------------------------------------------------------------------//
int32 CMainFrame::CheckUDPServerInput(std::string strInput,std::string *strOutput){
	*strOutput = "";
	if (Str_UpperCase(Str_Trim(strInput)) == "UDP SERVER")
		return 1;
	return 0;
}
//------------------------------------------------------------------------------------------//
DEVICE::OPEN_TYPE CMainFrame::CheckIPComInput(std::string strInput, std::string *strOutput){
	std::string strTemp;
#ifdef SWVERSION_COM
	if (CheckCOMInput(strInput,strOutput) != 0)
		return(DEVICE::OP_COM);
#endif
#ifdef	SWVERSION_TCPClient
	if (CheckTCPInput(strInput,strOutput) != 0)
		return(DEVICE::OP_TCPClient);
#endif
#ifdef	SWVERSION_UDPCLient
	if (CheckUDPInput(strInput,strOutput) != 0)
		return(DEVICE::OP_UDPClient);
#endif
#ifdef	SWVERSION_TCPServer
	if (CheckTCPServerInput(strInput,strOutput) != 0)
		return(DEVICE::OP_TCPServer);
#endif
#ifdef	SWVERSION_UDPServer
	if (CheckUDPServerInput(strInput,strOutput) != 0)
		return(DEVICE::OP_UDPServer);
#endif
	*strOutput = strInput;
	return(DEVICE::OP_NONE);
}
//------------------------------------------------------------------------------------------//
int32 CMainFrame::DoConnect(void){
	int32			baudrate;
	DEVICE::OPEN_TYPE	type;
	std::string		result;
	CString			sstrPortName,sstrBR;
	CMFCRibbonComboBox *COMBox,*BRBox;

	COMBox = (CMFCRibbonComboBox*)m_wndRibbonBar.FindByID(ID_RI_COMBO_COM);
	BRBox = (CMFCRibbonComboBox*)m_wndRibbonBar.FindByID(ID_RI_COMBO_BR);
	sstrPortName = COMBox->GetEditText();
	sstrBR = BRBox->GetEditText();

	type = CheckIPComInput(Str_UnicodeToANSI(sstrPortName.GetBuffer(0)),&result);
	baudrate = atoi(Str_UnicodeToANSI(sstrBR.GetBuffer(0)).c_str());

	return(GSDTApp.m_Device.Open(type, result, baudrate, GSDTApp.m_Device.cEDevFlag.blEnableEcho) != 0);
}
//------------------------------------------------------------------------------------------//
LRESULT CMainFrame::OnPrintout(WPARAM wParam, LPARAM lParam){
	switch (wParam){
		case 0 :
			//GSDTApp.m_Device.UpdataUIRecord();
			//GSDTApp.m_Device2.UpdataUIRecord();
			break;
		case 1 :
			GSDTApp.m_Device.Clean();
			GSDTApp.m_Device2.Clean();
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnButtonClickRecodrDir(void){
	CString	strDir;
	
	strDir = ODEV_CreateLOGDIR().c_str();
	ShellExecute(NULL,_T("open"),strDir,NULL,NULL,SW_SHOWNORMAL);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnButtonClickNewRecord(void){
	GSDTApp.ExecBIC("newrecord");
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnButtonClickSend(void){
	if (m_wndSend.IsVisible() == FALSE){
		m_wndSend.ShowPane(TRUE,FALSE,TRUE);
		m_wndSend.SetAutoHideMode(FALSE,CBRS_LEFT);
	}
	else if (m_wndSend.IsPaneVisible() == FALSE){
		m_wndSend.ShowPane(TRUE,FALSE,TRUE);
	}
	else{
		m_wndSend.ShowPane(FALSE,FALSE,TRUE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnButtonClickCacl(void){
	if (m_wndCacl.IsVisible() == FALSE){
		m_wndCacl.ShowPane(TRUE,FALSE,TRUE);
		m_wndCacl.SetAutoHideMode(FALSE,CBRS_LEFT);
	}
	else if (m_wndCacl.IsPaneVisible() == FALSE){
		m_wndCacl.ShowPane(TRUE,FALSE,TRUE);
	}
	else{
		m_wndCacl.ShowPane(FALSE,FALSE,TRUE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnAppAbout(void){
	ODEV_NODE		*record;
	std::string strTemp;
	((CMainFrame*)AfxGetMainWnd())->AuxCFrmCreate();
	((CMainFrame*)AfxGetMainWnd())->m_oDevNodeAux.Clean();
	record = &((CMainFrame*)AfxGetMainWnd())->m_oDevNodeAux;
	
	GSDTApp.m_Script.Help(record);

	((CMainFrame*)AfxGetMainWnd())->m_oDevNodeAux.Print();
	((CMainFrame*)AfxGetMainWnd())->m_oDevNodeAux.cgRichEdit->GetRichEditCtrl().SetSel(0,0);
}
#ifdef	SWVERSION_AUXCOM
//------------------------------------------------------------------------------------------//
void CMainFrame::OnRiComboAuxcom(void){
	int	id;
	CMFCRibbonComboBox *COMBox;
	COMBox = (CMFCRibbonComboBox*)m_wndRibbonBar.FindByID(ID_RI_COMBO_AUXCOM);
	id = COMBox->FindItem(COMBox->GetEditText());
	if (id >= 0){
		id = COMBox->GetItemData(id);
		m_ValidAuxComList.selectedNode = (IPCOMNAME*)RTREE_NODE::FindInLChildRChainByDRNodeID(&m_ValidAuxComList,id);
		SetSelectComboBox_AuxCOM();
	}
	else{
		m_ValidAuxComList.selectedNode = NULL;
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnRiComboAuxbr(void){
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateRiComboAuxcom(CCmdUI *pCmdUI){
	pCmdUI->Enable(GSDTApp.m_Device2.CheckblConnect() == 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateRiComboAuxbr(CCmdUI *pCmdUI){
	pCmdUI->Enable(GSDTApp.m_Device2.CheckblConnect() == 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnRiCheckAuxescape(void){
	if (B_ChkFLAG64(GSDTApp.m_Device2.cgODevList.cgODevFlagU64, ODEV_FLAG_EnEscape) == 0){
		B_SetFLAG64(GSDTApp.m_Device2.cgODevList.cgODevFlagU64, ODEV_FLAG_EnEscape);
		B_ClrFLAG64(GSDTApp.m_Device2.cgODevList.cgODevFlagU64, ODEV_FLAG_EnHEXViewMode);
	}
	else{
		B_ClrFLAG64(GSDTApp.m_Device2.cgODevList.cgODevFlagU64, ODEV_FLAG_EnEscape);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnRiCheckAuxhex(void){
	B_ClrFLAG64(GSDTApp.m_Device2.cgODevList.cgODevFlagU64, ODEV_FLAG_EnEscape);
	B_SetFLAG64(GSDTApp.m_Device2.cgODevList.cgODevFlagU64, ODEV_FLAG_EnHEXViewMode);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnRiCheckAuxascii(void){
	B_ClrFLAG64(GSDTApp.m_Device2.cgODevList.cgODevFlagU64, ODEV_FLAG_EnHEXViewMode);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnRiButtonAuxopen(void){
	if (GSDTApp.m_Device2.CheckblConnect() == 0){
		if (DoAuxopen())
			return;
	}
	GSDTApp.m_Device2.Close(1);
}
//------------------------------------------------------------------------------------------//
BOOL CMainFrame::DoAuxopen(void){
	DEVICE::OPEN_TYPE	type;
	int32			baudrate;
	std::string		result;
	CString			sstrPortName,sstrBR;
	CMFCRibbonComboBox *COMBox,*BRBox;

	COMBox = (CMFCRibbonComboBox*)m_wndRibbonBar.FindByID(ID_RI_COMBO_AUXCOM);
	BRBox = (CMFCRibbonComboBox*)m_wndRibbonBar.FindByID(ID_RI_COMBO_AUXBR);
	sstrPortName = COMBox->GetEditText();
	sstrBR = BRBox->GetEditText();

	type = CheckIPComInput(Str_UnicodeToANSI(sstrPortName.GetBuffer(0)),&result);
	baudrate = atoi(Str_UnicodeToANSI(sstrBR.GetBuffer(0)).c_str());

#ifdef	SWVERSION_AUXCOM
	return(GSDTApp.m_Device2.Open(type, result, baudrate, GSDTApp.m_Device2.cEDevFlag.blEnableEcho) != 0);
#endif
	return(FALSE);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnRiCheckAuxdsr(void){
	if ((GSDTApp.m_Device2.cgDevType == DEVICE::DEVID_APICOM) && (GSDTApp.m_Device2.CheckblConnect() != 0)){
		m_blAuxDSRFlow = !m_blAuxDSRFlow;
		GSDTApp.m_Device2.cgAPIECom->SetDSRFlow(m_blAuxDSRFlow);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnRiCheckAuxdtrh(void){
	if ((GSDTApp.m_Device2.cgDevType == DEVICE::DEVID_APICOM) && (GSDTApp.m_Device2.CheckblConnect() != 0))
		GSDTApp.ExecBIC("aux dtr -H");
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnRiCheckAuxdtrl(void){
	if ((GSDTApp.m_Device2.cgDevType == DEVICE::DEVID_APICOM) && (GSDTApp.m_Device2.CheckblConnect() != 0))
		GSDTApp.ExecBIC("aux dtr -L");
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnRiCheckAuxcts(void){
	if ((GSDTApp.m_Device2.cgDevType == DEVICE::DEVID_APICOM) && (GSDTApp.m_Device2.CheckblConnect() != 0)){
		m_blAuxCTSFlow = !m_blAuxCTSFlow;
		GSDTApp.m_Device2.cgAPIECom->SetCTSFlow(m_blAuxCTSFlow);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnRiCheckAuxrtsh(void){
	if ((GSDTApp.m_Device2.cgDevType == DEVICE::DEVID_APICOM) && (GSDTApp.m_Device2.CheckblConnect() != 0))
		GSDTApp.ExecBIC("aux rts -H");
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnRiCheckAuxrtsl(void){
	if ((GSDTApp.m_Device2.cgDevType == DEVICE::DEVID_APICOM) && (GSDTApp.m_Device2.CheckblConnect() != 0))
		GSDTApp.ExecBIC("aux rts -L");
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateRiCheckAuxescape(CCmdUI *pCmdUI){
	pCmdUI->SetCheck(B_ChkFLAG64(GSDTApp.m_Device2.cgODevList.cgODevFlagU64, ODEV_FLAG_EnEscape) != 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateRiCheckAuxhex(CCmdUI *pCmdUI){
	pCmdUI->SetCheck(B_ChkFLAG64(GSDTApp.m_Device2.cgODevList.cgODevFlagU64, ODEV_FLAG_EnHEXViewMode) != 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateRiCheckAuxascii(CCmdUI *pCmdUI){
	pCmdUI->SetCheck(B_ChkFLAG64(GSDTApp.m_Device2.cgODevList.cgODevFlagU64, ODEV_FLAG_EnHEXViewMode) == 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateRiButtonAuxopen(CCmdUI *pCmdUI){
	pCmdUI->SetCheck(GSDTApp.m_Device2.CheckblConnect() != 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateRiCheckAuxdsr(CCmdUI *pCmdUI){
	if ((GSDTApp.m_Device2.cgDevType == DEVICE::DEVID_APICOM) && (GSDTApp.m_Device2.CheckblConnect() != 0)){
		pCmdUI->Enable();
		pCmdUI->SetCheck(m_blAuxDSRFlow);
	}
	else{
		pCmdUI->SetCheck(FALSE);
		pCmdUI->Enable(FALSE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateRiCheckAuxdtrh(CCmdUI *pCmdUI){
	if ((GSDTApp.m_Device2.cgDevType == DEVICE::DEVID_APICOM) && (GSDTApp.m_Device2.CheckblConnect() != 0)){
		pCmdUI->Enable();
		pCmdUI->SetCheck(GSDTApp.m_Device2.cgAPIECom->GetDTRStatus() != 0);
	}
	else{
		pCmdUI->SetCheck(FALSE);
		pCmdUI->Enable(FALSE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateRiCheckAuxdtrl(CCmdUI *pCmdUI){
	if ((GSDTApp.m_Device2.cgDevType == DEVICE::DEVID_APICOM) && (GSDTApp.m_Device2.CheckblConnect() != 0)){
		pCmdUI->Enable();
		pCmdUI->SetCheck(GSDTApp.m_Device2.cgAPIECom->GetDTRStatus() == 0);
	}
	else{
		pCmdUI->SetCheck(TRUE);
		pCmdUI->Enable(FALSE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateRiCheckAuxcts(CCmdUI *pCmdUI){
	if ((GSDTApp.m_Device2.cgDevType == DEVICE::DEVID_APICOM) && (GSDTApp.m_Device2.CheckblConnect() != 0)){
		pCmdUI->Enable();
		pCmdUI->SetCheck(m_blAuxCTSFlow);
	}
	else{
		pCmdUI->SetCheck(FALSE);
		pCmdUI->Enable(FALSE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateRiCheckAuxrtsh(CCmdUI *pCmdUI){
	if ((GSDTApp.m_Device2.cgDevType == DEVICE::DEVID_APICOM) && (GSDTApp.m_Device2.CheckblConnect() != 0)){
		pCmdUI->Enable();
		pCmdUI->SetCheck(GSDTApp.m_Device2.cgAPIECom->GetRTSStatus() != 0);
	}
	else{
		pCmdUI->SetCheck(FALSE);
		pCmdUI->Enable(FALSE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateRiCheckAuxrtsl(CCmdUI *pCmdUI){
	if ((GSDTApp.m_Device2.cgDevType == DEVICE::DEVID_APICOM) && (GSDTApp.m_Device2.CheckblConnect() != 0)){
		pCmdUI->Enable();
		pCmdUI->SetCheck(GSDTApp.m_Device2.cgAPIECom->GetRTSStatus() == 0);
	}
	else{
		pCmdUI->SetCheck(TRUE);
		pCmdUI->Enable(FALSE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::InitComboBox_AuxCOM(void){
	CreateComboBox_AuxCOMList();
	m_ValidAuxComList.selectedNode = (IPCOMNAME*)RTREE_NODE::GetLastChild(&m_ValidAuxComList);
	SetSelectComboBox_AuxCOM();
}
//------------------------------------------------------------------------------------------//
void CMainFrame::InitComboBox_AuxBR(void){
	CMFCRibbonComboBox *BRBox;
	BRBox = (CMFCRibbonComboBox*)m_wndRibbonBar.FindByID(ID_RI_COMBO_AUXBR);
	BRBox->AddItem(_T("1200"));
	BRBox->AddItem(_T("2400"));
	BRBox->AddItem(_T("4800"));
	BRBox->AddItem(_T("9600"));
	BRBox->AddItem(_T("14400"));
	BRBox->AddItem(_T("19200"));
	BRBox->AddItem(_T("38400"));
	BRBox->AddItem(_T("56000"));
	BRBox->AddItem(_T("57600"));
	BRBox->AddItem(_T("115200"));
	BRBox->AddItem(_T("128000"));
	BRBox->AddItem(_T("230400"));
	BRBox->AddItem(_T("256000"));
	BRBox->AddItem(_T("460800"));
	BRBox->AddItem(_T("912600"));
	BRBox->SelectItem(9);
	BRBox->SetDropDownHeight(10 * 22);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::SetSelectComboBox_AuxCOM(void){
	CMFCRibbonComboBox *COMBox,*BRBox;
	CString		sstrPortName,sstrShowName,sstrBR;
	IPCOMNAME	*listItem;

	if (m_ValidAuxComList.selectedNode == NULL)
		return;

	COMBox = (CMFCRibbonComboBox*)m_wndRibbonBar.FindByID(ID_RI_COMBO_AUXCOM);
	BRBox = (CMFCRibbonComboBox*)m_wndRibbonBar.FindByID(ID_RI_COMBO_AUXBR);

	listItem = m_ValidAuxComList.selectedNode;
	
	if (listItem->typeID == PublicDevice_DEVID_APICOM)
		sstrPortName = listItem->strIPComName.c_str();
	if (listItem->typeID == PublicDevice_DEVID_TCPClient){
		sstrPortName = "TCP:";
		sstrPortName += listItem->strIPComName.c_str();
	}
	if (listItem->typeID == PublicDevice_DEVID_UDPClient){
		sstrPortName = "UDP:";
		sstrPortName += listItem->strIPComName.c_str();
	}
	if (listItem->typeID == PublicDevice_DEVID_TCPServer)
		sstrPortName = "TCP SERVER";
	if (listItem->typeID == PublicDevice_DEVID_UDPServer)
		sstrPortName = "UDP SERVER";
	sstrShowName = listItem->strShowName.c_str();
	COMBox->SetEditText(sstrPortName);
	sstrBR = Str_IntToString(listItem->portBaudrate).c_str();
	BRBox->SetEditText(sstrBR);
}
//---------------------------------------------------------------------------
void CMainFrame::CreateComboBox_AuxCOMList(void){
	CString		sstrPortName,sstrShowName,sstrBackup;
	int32		comBoxHigh;
	CMFCRibbonComboBox *COMBox;

	COMBox = (CMFCRibbonComboBox*)m_wndRibbonBar.FindByID(ID_RI_COMBO_AUXCOM);
	sstrBackup = COMBox->GetEditText();
	COMBox->RemoveAllItems();

	GSDTApp.m_IPComList.Refresh();
	GSDTApp.m_IPComList.ExportCOM(&m_ValidAuxComList);

	m_ValidAuxComList.Spin_InUse_set();

	RTREE_LChildRChain_Traversal_LINE(IPCOMNAME, (&m_ValidAuxComList),
		if (operateNode_t->typeID == PublicDevice_DEVID_APICOM)
			sstrPortName = operateNode_t->strIPComName.c_str();
		if (operateNode_t->typeID == PublicDevice_DEVID_TCPClient){
			sstrPortName = "TCP:";
			sstrPortName += operateNode_t->strIPComName.c_str();
		}
		if (operateNode_t->typeID == PublicDevice_DEVID_UDPClient){
			sstrPortName = "UDP:";
			sstrPortName += operateNode_t->strIPComName.c_str();
		}
		if (operateNode_t->typeID == PublicDevice_DEVID_TCPServer)
			sstrPortName = "TCP SERVER";
		if (operateNode_t->typeID == PublicDevice_DEVID_UDPServer)
			sstrPortName = "UDP SERVER";
		sstrShowName = operateNode_t->strShowName.c_str();
		COMBox->AddItem(sstrShowName, RTREE_NODE::GetdRNodeID(operateNode_t));
		if (sstrBackup == sstrPortName)
			m_ValidAuxComList.selectedNode = operateNode_t;
	);
	m_ValidAuxComList.Spin_InUse_clr();
	if (m_ValidAuxComList.selectedNode != NULL)
		COMBox->SelectItem((DWORD_PTR)RTREE_NODE::GetdRNodeID(m_ValidAuxComList.selectedNode));
	COMBox->SetEditText(sstrBackup);
	comBoxHigh = COMBox->GetCount() * 22;
	if (comBoxHigh > 15 * 22)
		comBoxHigh = 15 * 22;
	COMBox->SetDropDownHeight(comBoxHigh);
	COMBox->EnableDropDownListResize(TRUE);
}
#endif
#ifdef	SWVERSION_CMUX
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateCMUXStart(CCmdUI *pCmdUI){
	if (m_CMUXView != NULL)
		m_CMUXView->RefreshCOM();
	pCmdUI->SetCheck(GSDTApp.m_CMUXDriver.CheckblStart());
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateCMUXStart1(CCmdUI *pCmdUI){
	if (m_CMUXView != NULL)
		m_CMUXView->RefreshCOM();
	pCmdUI->SetCheck(GSDTApp.m_CMUXDriver.CheckblStart());
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnCMUXStart(void){
	if (GSDTApp.m_Device.cgDevType != DEVICE::DEVID_APICOM || GSDTApp.m_Device.CheckblConnect() == 0)
		return;
	if (GSDTApp.m_CMUXDriver.CheckblStart() != 0){
		GSDTApp.m_CMUXDriver.CMUXClose();
		return;
	}
	GSDTApp.m_CMUXDriver.CMUXStart(1);
	CMUXFrmCreate();
	m_MCFrm->MDIActivate();
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnCMUXStart1(void){//without AT
	if (GSDTApp.m_Device.cgDevType != DEVICE::DEVID_APICOM || GSDTApp.m_Device.CheckblConnect() == 0)
		return;
	if (GSDTApp.m_CMUXDriver.CheckblStart() != 0){
		GSDTApp.m_CMUXDriver.CMUXClose();
		return;
	}	
	GSDTApp.m_CMUXDriver.CMUXStart(0);
	CMUXFrmCreate();
	m_MCFrm->MDIActivate();
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnCMUXCLD(void){
	if (GSDTApp.m_CMUXDriver.CheckblStart() == 0)
		return;
	GSDTApp.m_CMUXDriver.SendCMUXCLD();
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnCMUXPSC(void){
	if (GSDTApp.m_CMUXDriver.CheckblStart() == 0)
		return;
	GSDTApp.m_CMUXDriver.SendCMUXStdPSC(0xff);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnCMUXPSC0(void){
	if (GSDTApp.m_CMUXDriver.CheckblStart() == 0)
		return;
	GSDTApp.m_CMUXDriver.SendCMUXStdPSC(0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnCMUXPSC1(void){
	if (GSDTApp.m_CMUXDriver.CheckblStart() == 0)
		return;
	GSDTApp.m_CMUXDriver.SendCMUXStdPSC(1);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnCMUXPSC7(void){
	if (GSDTApp.m_CMUXDriver.CheckblStart() == 0)
		return;
	GSDTApp.m_CMUXDriver.SendCMUXStdPSC(7);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnCMUXPSC9(void){
	if (GSDTApp.m_CMUXDriver.CheckblStart() == 0)
		return;
	GSDTApp.m_CMUXDriver.SendCMUXStdPSC(9);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnCMUXFC(void){
	if (GSDTApp.m_CMUXDriver.CheckblStart() == 0)
		return;
	m_blCMUXFC = !m_blCMUXFC;
	GSDTApp.m_CMUXDriver.SendFCC(m_blCMUXFC);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateCMUXFC(CCmdUI *pCmdUI){
	pCmdUI->SetCheck(m_blCMUXFC);
}
#endif
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateSBAR_RX(CCmdUI *pCmdUI){
	CString	cText;
	if (GSDTApp.m_Device.CheckblConnect() != 0){
		pCmdUI->Enable();
		cText = _T("Received Bytes:  ");
		cText += Str_ANSIToUnicode(Str_UInt64ToString(GSDTApp.m_Device.RxBytes())).c_str();
		pCmdUI->SetText(cText);
	}
	else{
		pCmdUI->Enable(FALSE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateSBAR_TX(CCmdUI *pCmdUI){
	CString	cText;
	if (GSDTApp.m_Device.CheckblConnect() != 0){
		pCmdUI->Enable();
		cText = _T("Send Bytes:  ");
		cText += Str_ANSIToUnicode(Str_UInt64ToString(GSDTApp.m_Device.TxBytes())).c_str();
		pCmdUI->SetText(cText);
	}
	else{
		pCmdUI->Enable(FALSE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateSBAR_CTS(CCmdUI *pCmdUI){
	CString	cText;
	cText = "";
	if ((GSDTApp.m_Device.cgDevType == DEVICE::DEVID_APICOM) && (GSDTApp.m_Device.CheckblConnect() != 0)){
		pCmdUI->Enable();
		cText = _T("CTS = ");
		cText += Str_ANSIToUnicode(GSDTApp.m_Device.cgAPIECom->GetCTSStatus()).c_str();
	}
	else{
		pCmdUI->Enable(FALSE);
	}
	pCmdUI->SetText(cText);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateSBAR_DSR(CCmdUI *pCmdUI){
	CString	cText;
	cText = "";
	if ((GSDTApp.m_Device.cgDevType == DEVICE::DEVID_APICOM) && (GSDTApp.m_Device.CheckblConnect() != 0)){
		pCmdUI->Enable();
		cText = _T("DSR = ");
		cText += Str_ANSIToUnicode(GSDTApp.m_Device.cgAPIECom->GetDSRStatus()).c_str();
	}
	else{
		pCmdUI->Enable(FALSE);
	}
	pCmdUI->SetText(cText);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateSBAR_RING(CCmdUI *pCmdUI){
	CString	cText;
	cText = "";
	if ((GSDTApp.m_Device.cgDevType == DEVICE::DEVID_APICOM) && (GSDTApp.m_Device.CheckblConnect() != 0)){
		pCmdUI->Enable();
		cText = _T("RING = ");
		cText += Str_ANSIToUnicode(GSDTApp.m_Device.cgAPIECom->GetRINGStatus()).c_str();
	}
	else{
		pCmdUI->Enable(FALSE);
	}
	pCmdUI->SetText(cText);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateSBAR_DCD(CCmdUI *pCmdUI){
	CString	cText;
	cText = "";
	if ((GSDTApp.m_Device.cgDevType == DEVICE::DEVID_APICOM) && (GSDTApp.m_Device.CheckblConnect() != 0)){
		pCmdUI->Enable();
		cText = _T("DCD = ");
		cText += Str_ANSIToUnicode(GSDTApp.m_Device.cgAPIECom->GetDCDStatus()).c_str();
	}
	else{
		pCmdUI->Enable(FALSE);
	}
	pCmdUI->SetText(cText);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateSBAR_NONE(CCmdUI *pCmdUI){
	pCmdUI->Enable();
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateSBAR_GetIP(CCmdUI *pCmdUI){
	pCmdUI->Enable();
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateSBAR_IP(CCmdUI *pCmdUI){
	pCmdUI->SetText(m_localIP);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnTimer(UINT nIDEvent){
	switch(nIDEvent){ 
		case 1:
			m_wndStatusBar.DoPB();
			break;
		case 2:
			//if (GSDTApp.m_oDevOutputListPool.CheckblEnabled() != 0)
				//GSDTApp.m_oDevOutputListPool.PrintSDOUT();
			GSDTApp.m_Device.UpdataUIRecordSDOUT();
			break;
		default:
			break;
	}
    CMDIFrameWndEx::OnTimer(nIDEvent);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnSBAR_GetIP(void){
	std::string		stringData;
	char			HostName[64];
	HOSTENT			*lpHostEnt;
	ODEV_NODE		*record;
	std::wstring	wstrText;
	gethostname(HostName,sizeof(HostName));
	lpHostEnt = gethostbyname(HostName);
	if (lpHostEnt == NULL)
		return;

	if (lpHostEnt->h_addr_list[0] != NULL){
		stringData = inet_ntoa(*((struct in_addr *)(lpHostEnt->h_addr_list[0])));
		m_localIP = Str_ANSIToUnicode(stringData).c_str();
	}
	else{
		m_localIP = "255.255.255.255";
	}

	stringData = "HostName: ";
	stringData += HostName;
	stringData += "\r\n";
	
	AuxCFrmCreate();
	m_oDevNodeAux.Clean();
	record = &m_oDevNodeAux;
	record->WriteToStr(stringData,RICH_CF_clBlack);

	for (int i=0; lpHostEnt->h_addr_list[i]!=NULL; i++){
		stringData = inet_ntoa(*((struct in_addr *)(lpHostEnt->h_addr_list[i])));
		stringData = "Local IP: " + stringData + "\r\n";
		record->WriteToStr(stringData,RICH_CF_clBlack);
	}
	stringData = "";
	APIComSignature_GetNetCardInfo(&stringData);
	record->WriteToStr(stringData,RICH_CF_clBlack);
	m_oDevNodeAux.Print();
	m_oDevNodeAux.cgRichEdit->GetRichEditCtrl().SetSel(0,0);
}
//------------------------------------------------------------------------------------------//
