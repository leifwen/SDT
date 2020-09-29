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
	if (!m_wndCalc.Create(_T("Calculator"), this, CRect(0, 145, 300, 145 + 494), TRUE, ID_VIEW_CALCVIEW
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

	m_wndCalc.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndCalc);

	CDockablePane *pTabbedBar2 = NULL;
	m_wndGC.AttachToTabWnd(&m_wndSC, DM_SHOW, TRUE, &pTabbedBar2);
	m_wndSC.ShowPane(FALSE,FALSE,TRUE);

	CDockablePane *pTabbedBar = NULL;
	m_wndSend.AttachToTabWnd(&m_wndSC, DM_SHOW, TRUE, &pTabbedBar);
	m_wndSend.ShowPane(FALSE,FALSE,TRUE);

	CDockablePane *pTabbedBar3 = NULL;
	m_wndCalc.AttachToTabWnd(&m_wndSC, DM_SHOW, TRUE, &pTabbedBar3);
	m_wndCalc.ShowPane(FALSE,FALSE,TRUE);

	m_wndGC.ShowPane(FALSE,FALSE,TRUE);

	return TRUE;
}
//------------------------------------------------------------------------------------------//
LRESULT CMainFrame::OnSetText(WPARAM wParam,LPARAM lParam){
	m_strTitle = m_MyTitle + _T(SWVERSION);
	return(CMDIFrameWndEx::OnSetText(m_strTitle));
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnWindowManager(void){// CMainFrame message handlers
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

	HICON hCalcViewBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CALC : IDI_CALC), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndCalc.SetIcon(hCalcViewBarIcon, FALSE);
	UpdateMDITabbedBarsIcons();
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
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg){
	if (pMsg->message == WM_LBUTTONDOWN){
		CMFCRibbonComboBox *COMBox;
		COMBox = (CMFCRibbonComboBox*)m_wndRibbonBar.FindByID(ID_RI_COMBO_COM);
		if (pMsg->hwnd == m_wndRibbonBar.GetSafeHwnd()){
			if (COMBox->GetHighlighted() == COMBox)
				if (theApp.GSDTApp.m_CDevBusM.IsOpened() == 0)
					CreateComboBox_COMList();
		}
		#ifdef	SWVERSION_AUXDEVICE
		COMBox = (CMFCRibbonComboBox*)m_wndRibbonBar.FindByID(ID_RI_COMBO_AUXCOM);
		if (pMsg->hwnd == m_wndRibbonBar.GetSafeHwnd()){
			if (COMBox->GetHighlighted() == COMBox)
				if (theApp.GSDTApp.m_CDevBusA.IsOpened() == 0)
					CreateComboBox_AuxCOMList();
		}
		#endif
	}
	return(CMDIFrameWndEx::PreTranslateMessage(pMsg));
}
//------------------------------------------------------------------------------------------//
#ifdef _DEBUG
void CMainFrame::AssertValid(void) const 			{CMDIFrameWndEx::AssertValid();}
void CMainFrame::Dump		(CDumpContext& dc) const{CMDIFrameWndEx::Dump(dc);}
#endif //_DEBUG
//------------------------------------------------------------------------------------------//
