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
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_MESSAGE					(WM_SETTEXT,											&CMainFrame::OnSetText)
	ON_COMMAND					(ID_WINDOW_MANAGER,										&CMainFrame::OnWindowManager)
	ON_COMMAND_RANGE			(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7,	&CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE	(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7,	&CMainFrame::OnUpdateApplicationLook)

	ON_COMMAND(ID_RI_COMBO_COM,				&CMainFrame::OnCOMBO_COM)
	ON_COMMAND(ID_RI_COMBO_BR,				&CMainFrame::OnCOMBO_BR)
#ifdef SWVERSION_COM
	ON_COMMAND(ID_RI_CHECK_DSR_FLOW, 		&CMainFrame::OnDSR_FLOW)
	ON_COMMAND(ID_RI_CHECK_CTS_FLOW, 		&CMainFrame::OnCTS_FLOW)
	ON_COMMAND(ID_RI_CHECK_DTR_HIGH, 		&CMainFrame::OnDTRHigh)
	ON_COMMAND(ID_RI_CHECK_DTR_LOW,			&CMainFrame::OnDTRLow)
	ON_COMMAND(ID_RI_CHECK_RTS_HIGH, 		&CMainFrame::OnRTSHigh)
	ON_COMMAND(ID_RI_CHECK_RTS_LOW, 		&CMainFrame::OnRTSLow)
#endif
	ON_COMMAND(ID_RI_CHECK_ASCII, 			&CMainFrame::OnASCII)
	ON_COMMAND(ID_RI_CHECK_HEX, 			&CMainFrame::OnHEX)
	ON_COMMAND(ID_RI_CHECK_MSR, 			&CMainFrame::OnMSR)
	ON_COMMAND(ID_RI_CHECK_OSPMSG,			&CMainFrame::OnOSPMSG)
	ON_COMMAND(ID_RI_CHECK_RECMSG,			&CMainFrame::OnRECMSG)
	ON_COMMAND(ID_RI_CHECK_VIEW,			&CMainFrame::OnDisplay)
	ON_COMMAND(ID_RI_BUTTON_CONNECT,		&CMainFrame::OnButtonClickConnect)
	ON_COMMAND(ID_RI_BUTTON_ECHO,			&CMainFrame::OnButtonClickEcho)
#ifdef SWVERSION_SCRIPT
	ON_COMMAND(ID_RI_BUTTON_SPRIPT_STOP,	&CMainFrame::OnButtonClickScriptStop)
	ON_COMMAND(ID_RI_BUTTON_SCRIPT_AT,		&CMainFrame::OnButtonClickScriptAT)
	ON_COMMAND(ID_RI_BUTTON_SCRIPT_CE,		&CMainFrame::OnButtonClickScriptCE)
	ON_COMMAND(ID_RI_BUTTON_SCRIPT_SHOWSBIC,&CMainFrame::OnButtonClickScriptShowSBIC)
	ON_COMMAND(ID_RI_BUTTON_SC,				&CMainFrame::OnButtonClickScriptSC)
	ON_COMMAND(ID_RI_BUTTON_GC,				&CMainFrame::OnButtonClickScriptGC)
#endif
	ON_COMMAND(ID_RI_BUTTON_CLEANVIEW,		&CMainFrame::OnButtonClickClean)
	ON_COMMAND(ID_RI_BUTTON_DEVLIST,		&CMainFrame::OnButtonClickDevList)
	ON_COMMAND(ID_RI_BUTTON_SOCKETLIST,		&CMainFrame::OnButtonClickScoketlist)
	ON_COMMAND(ID_RI_BUTTON_OPENRFD,		&CMainFrame::OnButtonClickRecodrDir)
	ON_COMMAND(ID_RI_BUTTON_NEWRECORD,		&CMainFrame::OnButtonClickNewRecord)
#ifdef SWVERSION_TOOLS
	ON_COMMAND(ID_RI_BUTTON_SEND,			&CMainFrame::OnButtonClickSend)
	ON_COMMAND(ID_RI_BUTTON_CACL,			&CMainFrame::OnButtonClickCacl)
#endif
	ON_UPDATE_COMMAND_UI(ID_RI_COMBO_COM,	&CMainFrame::OnUpdateCOMBO_COM)
	ON_UPDATE_COMMAND_UI(ID_RI_COMBO_BR,	&CMainFrame::OnUpdateCOMBO_BR)
#ifdef SWVERSION_COM
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_DSR_FLOW,			&CMainFrame::OnUpdateDSR_FLOW)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_CTS_FLOW,			&CMainFrame::OnUpdateCTS_FLOW)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_DTR_HIGH,			&CMainFrame::OnUpdateDTRHigh)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_DTR_LOW,			&CMainFrame::OnUpdateDTRLow)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_RTS_HIGH,			&CMainFrame::OnUpdateRTSHigh)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_RTS_LOW,			&CMainFrame::OnUpdateRTSLow)
#endif
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_ASCII,				&CMainFrame::OnUpdateASCII)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_HEX,				&CMainFrame::OnUpdateHEX)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_MSR,				&CMainFrame::OnUpdateMSR)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_OSPMSG,			&CMainFrame::OnUpdateOSPMSG)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_RECMSG,			&CMainFrame::OnUpdateRECMSG)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_VIEW,				&CMainFrame::OnUpdateDisplay)
	ON_UPDATE_COMMAND_UI(ID_RI_BUTTON_CONNECT,			&CMainFrame::OnUpdateConnect)
	ON_UPDATE_COMMAND_UI(ID_RI_BUTTON_ECHO,				&CMainFrame::OnUpdateEcho)
	ON_UPDATE_COMMAND_UI(ID_RI_BUTTON_SOCKETLIST,		&CMainFrame::OnUpdateScoketlist)
#ifdef SWVERSION_SCRIPT
	ON_UPDATE_COMMAND_UI(ID_RI_BUTTON_SPRIPT_STOP,		&CMainFrame::OnUpdateScrpit)
	ON_UPDATE_COMMAND_UI(ID_RI_BUTTON_SCRIPT_AT,		&CMainFrame::OnUpdateScriptAT)
	ON_UPDATE_COMMAND_UI(ID_RI_BUTTON_SCRIPT_CE,		&CMainFrame::OnUpdateScriptCE)
	ON_UPDATE_COMMAND_UI(ID_RI_BUTTON_SCRIPT_SHOWSBIC,	&CMainFrame::OnUpdateScriptShowSBIC)
	ON_COMMAND(ID_TOOLBAR_GC_EDIT,						&CMainFrame::OnButtonClickGCEdit)
	ON_COMMAND(ID_TOOLBAR_GCP_COMMAND,					&CMainFrame::OnButtonClickGCPCOMMAND)
#endif
	ON_UPDATE_COMMAND_UI(ID_STATUSBAR_RX,		&CMainFrame::OnUpdateSBAR_RX)
	ON_UPDATE_COMMAND_UI(ID_STATUSBAR_TX,		&CMainFrame::OnUpdateSBAR_TX)
	ON_UPDATE_COMMAND_UI(ID_STATUSBAR_CTS,		&CMainFrame::OnUpdateSBAR_CTS)
	ON_UPDATE_COMMAND_UI(ID_STATUSBAR_DSR,		&CMainFrame::OnUpdateSBAR_DSR)
	ON_UPDATE_COMMAND_UI(ID_STATUSBAR_RING,		&CMainFrame::OnUpdateSBAR_RING)
	ON_UPDATE_COMMAND_UI(ID_STATUSBAR_DCD,		&CMainFrame::OnUpdateSBAR_DCD)
	ON_UPDATE_COMMAND_UI(ID_STATUSBAR_GETIP,	&CMainFrame::OnUpdateSBAR_GetIP)
	ON_UPDATE_COMMAND_UI(ID_STATUSBAR_NONE,		&CMainFrame::OnUpdateSBAR_NONE)
	ON_UPDATE_COMMAND_UI(ID_STATUSBAR_IP,		&CMainFrame::OnUpdateSBAR_IP)
	ON_COMMAND(ID_STATUSBAR_GETIP,				&CMainFrame::OnSBAR_GetIP)
	ON_WM_TIMER()

	ON_COMMAND(ID_APP_ABOUT,					&CMainFrame::OnAppAbout)
	ON_MESSAGE(WM_ON_BICCLEAN,					&CMainFrame::OnBICClean)
#ifdef SWVERSION_AUXDEVICE
	ON_COMMAND(ID_RI_COMBO_AUXCOM,				&CMainFrame::OnRiComboAuxcom)
	ON_COMMAND(ID_RI_COMBO_AUXBR,				&CMainFrame::OnRiComboAuxbr)
	ON_COMMAND(ID_RI_CHECK_AUXESCAPE,			&CMainFrame::OnRiCheckAuxescape)
	ON_COMMAND(ID_RI_CHECK_AUXHEX,				&CMainFrame::OnRiCheckAuxhex)
	ON_COMMAND(ID_RI_CHECK_AUXASCII,			&CMainFrame::OnRiCheckAuxascii)
	ON_COMMAND(ID_RI_BUTTON_AUXOPEN,			&CMainFrame::OnRiButtonAuxopen)
	ON_COMMAND(ID_RI_CHECK_AUXDSR,				&CMainFrame::OnRiCheckAuxdsr)
	ON_COMMAND(ID_RI_CHECK_AUXDTRH,				&CMainFrame::OnRiCheckAuxdtrh)
	ON_COMMAND(ID_RI_CHECK_AUXDTRL,				&CMainFrame::OnRiCheckAuxdtrl)
	ON_COMMAND(ID_RI_CHECK_AUXCTS,				&CMainFrame::OnRiCheckAuxcts)
	ON_COMMAND(ID_RI_CHECK_AUXRTSH,				&CMainFrame::OnRiCheckAuxrtsh)
	ON_COMMAND(ID_RI_CHECK_AUXRTSL,				&CMainFrame::OnRiCheckAuxrtsl)
	ON_UPDATE_COMMAND_UI(ID_RI_COMBO_AUXCOM,	&CMainFrame::OnUpdateRiComboAuxcom)
	ON_UPDATE_COMMAND_UI(ID_RI_COMBO_AUXBR,		&CMainFrame::OnUpdateRiComboAuxbr)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_AUXESCAPE,	&CMainFrame::OnUpdateRiCheckAuxescape)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_AUXHEX,	&CMainFrame::OnUpdateRiCheckAuxhex)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_AUXASCII,	&CMainFrame::OnUpdateRiCheckAuxascii)
	ON_UPDATE_COMMAND_UI(ID_RI_BUTTON_AUXOPEN,	&CMainFrame::OnUpdateRiButtonAuxopen)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_AUXDSR,	&CMainFrame::OnUpdateRiCheckAuxdsr)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_AUXDTRH,	&CMainFrame::OnUpdateRiCheckAuxdtrh)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_AUXDTRL,	&CMainFrame::OnUpdateRiCheckAuxdtrl)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_AUXCTS,	&CMainFrame::OnUpdateRiCheckAuxcts)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_AUXRTSH,	&CMainFrame::OnUpdateRiCheckAuxrtsh)
	ON_UPDATE_COMMAND_UI(ID_RI_CHECK_AUXRTSL,	&CMainFrame::OnUpdateRiCheckAuxrtsl)
#endif
#ifdef SWVERSION_CMUX	
	ON_COMMAND(ID_RI_CMUX_START,				&CMainFrame::OnCMUXStart)
	ON_COMMAND(ID_RI_CMUX_START1,				&CMainFrame::OnCMUXStart1)
	ON_COMMAND(ID_RI_CMUX_CLD,					&CMainFrame::OnCMUXCLD)
	ON_COMMAND(ID_RI_CMUX_STDPSC,				&CMainFrame::OnCMUXPSC)
	ON_COMMAND(ID_RI_CMUX_PSC0,					&CMainFrame::OnCMUXPSC0)
	ON_COMMAND(ID_RI_CMUX_PSC1,					&CMainFrame::OnCMUXPSC1)
	ON_COMMAND(ID_RI_CMUX_PSC7,					&CMainFrame::OnCMUXPSC7)
	ON_COMMAND(ID_RI_CMUX_PSC9,					&CMainFrame::OnCMUXPSC9)
	ON_COMMAND(ID_RI_CMUX_FC,					&CMainFrame::OnCMUXFC)
	ON_UPDATE_COMMAND_UI(ID_RI_CMUX_FC, 		&CMainFrame::OnUpdateCMUXFC)
	ON_UPDATE_COMMAND_UI(ID_RI_CMUX_START,		&CMainFrame::OnUpdateCMUXStart)
	ON_UPDATE_COMMAND_UI(ID_RI_CMUX_START1,		&CMainFrame::OnUpdateCMUXStart1)
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
	m_MCFrm = NULL;
	m_AuxCFrm = NULL;
	m_CMUXCFrm = NULL;
	m_CMUXView = NULL;
	m_localIP = "255.255.255.255";
	m_Pitl3Created = FALSE;
	m_MyTitle = "";
}
//------------------------------------------------------------------------------------------//
CMainFrame::~CMainFrame(void){
}
//------------------------------------------------------------------------------------------//
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct){
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetTimer(1, 100, 0);
	SetTimer(2, 200, 0);
	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE;	// other styles available...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;			// set to FALSE to place close button at right of tab area
	mdiTabParams.m_bTabIcons = TRUE;						// set to TRUE to enable document icons on MDI taba
	mdiTabParams.m_bAutoColor = TRUE;						// set to FALSE to disable auto-coloring of MDI tabs
	mdiTabParams.m_bDocumentMenu = TRUE;					// enable the document menu at the right edge of the tab area
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
	
	CDockingManager::SetDockingMode(DT_SMART);						// enable Visual Studio 2005 style docking window behavior
	
	EnableAutoHidePanes(CBRS_ALIGN_ANY);							// enable Visual Studio 2005 style docking window auto-hide behavior
	
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : IDB_MENU_IMAGES_24);
																	// Load menu item image (not placed on any standard toolbars):

	if (CreateDockingWindows() == FALSE){							// create docking windows
		TRACE0("Failed to create docking windows\n");
		return -1;
	}

	OnApplicationLook(theApp.m_nAppLook);							// set the visual manager and style based on persisted value

	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);// Enable enhanced windows management dialog

	// Switch the order of document name and application name on the window title bar. This
	// improves the usability of the taskbar because the document name is visible with the thumbnail.
	ModifyStyle(0, FWS_PREFIXTITLE);
	
	return(MainCFrmCreate() == nullptr);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnTimer(UINT nIDEvent){
	switch(nIDEvent){ 
		case 1:
			m_wndStatusBar.DoPB();
			break;
		case 2:
			theApp.GSDTApp.m_Cache.Delivery();
			break;
		default:
			break;
	}
    CMDIFrameWndEx::OnTimer(nIDEvent);
}
//------------------------------------------------------------------------------------------//
CChildFrame* CMainFrame::MainCFrmCreate(void){
	if (m_MCFrm == nullptr){
		CCreateContext	context;

		context.m_pNewViewClass = RUNTIME_CLASS(CMyRichView);

		m_MCFrm = new CChildFrame();
		m_MCFrm->LoadFrame(IDI_MCFRM,WS_CHILD | WS_OVERLAPPEDWINDOW,this,&context);
		m_MCFrm->SetWindowText(_T("Main View"));
		m_MCFrm->ShowWindow(SW_SHOW);
		m_MCFrm->InitialUpdateFrame(NULL,true);

		m_MCFrm->InitSTDOUT("MAIN");
		m_MCFrm->InitConsole(&theApp.GSDTApp.m_Console);
		
		theApp.GSDTApp.InitSTDOUT(&m_MCFrm->m_stdout);
		theApp.GSDTApp.Run();
	}
	return (m_MCFrm);
}
//------------------------------------------------------------------------------------------//
CChildFrame* CMainFrame::AuxCFrmCreate(void){
	if (m_AuxCFrm == nullptr){
		CCreateContext	context;

		context.m_pNewViewClass = RUNTIME_CLASS(CMyRichView);

		m_AuxCFrm = new CChildFrame();
		m_AuxCFrm->LoadFrame(IDI_MCFRM, WS_CHILD | WS_OVERLAPPEDWINDOW, this, &context);
		m_AuxCFrm->SetWindowText(_T("Aux View"));
		m_AuxCFrm->ShowWindow(SW_SHOW);
		m_AuxCFrm->InitialUpdateFrame(NULL, true);

		m_AuxCFrm->InitVG3D("AUX",&theApp.GSDTApp.m_Cache);

		m_AuxCFrm->m_bCanClose = TRUE;
	}
	else{
		m_AuxCFrm->MDIActivate();
	}
	return(m_AuxCFrm);
}
//------------------------------------------------------------------------------------------//
CChildFrame* CMainFrame::CMUXCFrmCreate(void){
	if (m_CMUXCFrm == NULL){
		CCreateContext	context;

		context.m_pNewViewClass = RUNTIME_CLASS(CCMUXView);

		m_CMUXCFrm = new CChildFrame();
		m_CMUXCFrm->LoadFrame(IDI_MCFRM, WS_CHILD | WS_OVERLAPPEDWINDOW, this, &context);
		m_CMUXCFrm->SetWindowText(_T("CMUX COM setting"));
		m_CMUXCFrm->ShowWindow(SW_SHOW);
		m_CMUXCFrm->InitialUpdateFrame(NULL, true);

		m_CMUXCFrm->m_selfName = "CMUX";

		m_CMUXView = (CCMUXView*)m_CMUXCFrm->GetActiveView();
	}
	else{
		//m_CMUXFrm->MDIActivate();
	}
	return(m_CMUXCFrm);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnClose(void){
	theApp.m_bCanClose = TRUE;
	m_Pitl3->Release();
	m_Pitl3Created = FALSE;
	KillTimer(1);
	KillTimer(2);
	CoUninitialize();
	theApp.GSDTApp.Exit(DEFAULT_INI_FILE);
	CMDIFrameWndEx::OnClose();
}
//------------------------------------------------------------------------------------------//
#include "MainFrm_ON.hpp"
#include "MainFrm_Main.hpp"
#include "MainFrm_Aux.hpp"
#include "MainFrm_CMUX.hpp"
#include "MainFrm_SBAR.hpp"
#include "MainFrm_CHK.hpp"







