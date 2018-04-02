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
#include "RibbonStatusBar.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//------------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CMyMFCRibbonStatusBar, CMFCRibbonStatusBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//ON_WM_CONTEXTMENU()
	
END_MESSAGE_MAP()
//------------------------------------------------------------------------------------------//
CMyMFCRibbonStatusBar::CMyMFCRibbonStatusBar(void){
	m_blPBStart = FALSE;
}
//------------------------------------------------------------------------------------------//
int CMyMFCRibbonStatusBar::OnCreate(LPCREATESTRUCT lpCreateStruct){
	
	if (CMFCRibbonStatusBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	CString strTitlePane;
	CMFCRibbonStatusBarPane	*barPane;

	m_ProgressBar = new CMFCRibbonProgressBar(ID_STATUSBAR_PB,50);
	m_ProgressBar->SetRange(0,100);
	m_ProgressBar->SetPos(60);
	m_ProgressBar->SetInfiniteMode(TRUE);

	strTitlePane = _T("Send Bytes");
	barPane = new CMFCRibbonStatusBarPane(ID_STATUSBAR_TX,strTitlePane, TRUE); 
	AddElement(barPane,strTitlePane);
	barPane->SetAlmostLargeText(_T("Send Bytes:  0000000000"));
	
	strTitlePane = _T("Received Bytes");
	barPane = new CMFCRibbonStatusBarPane(ID_STATUSBAR_RX,strTitlePane, TRUE); 
	AddElement(barPane,strTitlePane);
	barPane->SetAlmostLargeText(_T("Received Bytes:  0000000000"));

	AddSeparator();
	strTitlePane = _T("CTS");
	barPane = new CMFCRibbonStatusBarPane(ID_STATUSBAR_CTS,strTitlePane, TRUE); 
	AddElement(barPane,strTitlePane);
	barPane->SetAlmostLargeText(_T("CTS = H"));
	barPane->SetText(_T(""));

	strTitlePane = _T("DSR");
	barPane = new CMFCRibbonStatusBarPane(ID_STATUSBAR_DSR,strTitlePane, TRUE); 
	AddElement(barPane,strTitlePane);
	barPane->SetAlmostLargeText(_T("DSR = H"));
	barPane->SetText(_T(""));

	strTitlePane = _T("RING");
	barPane = new CMFCRibbonStatusBarPane(ID_STATUSBAR_RING,strTitlePane, TRUE); 
	AddElement(barPane,strTitlePane);
	barPane->SetAlmostLargeText(_T("RING = H"));
	barPane->SetText(_T(""));

	strTitlePane = _T("DCD");
	barPane = new CMFCRibbonStatusBarPane(ID_STATUSBAR_DCD,strTitlePane, TRUE); 
	AddElement(barPane,strTitlePane);
	barPane->SetAlmostLargeText(_T("DCD = H"));
	barPane->SetText(_T(""));

	strTitlePane = _T("Local IP :");
	AddExtendedElement(m_ProgressBar,_T("Progress"));

	AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_GETIP,strTitlePane, FALSE),strTitlePane);

	strTitlePane = _T("IP");
	barPane = new CMFCRibbonStatusBarPane(ID_STATUSBAR_IP,strTitlePane, TRUE); 
	AddExtendedElement(barPane,strTitlePane);
	barPane->SetAlmostLargeText(_T("255.255.255.255"));
	barPane->SetText(_T("255.255.255.255"));

	return 0;
}
//------------------------------------------------------------------------------------------//
void CMyMFCRibbonStatusBar::OnSize(UINT nType, int cx, int cy){
	CMFCRibbonStatusBar::OnSize(nType, cx, cy);
	AdjustLayout();
}
//------------------------------------------------------------------------------------------//
void CMyMFCRibbonStatusBar::OnContextMenu(CWnd* pWnd, CPoint point){
	CMFCRibbonStatusBar::OnContextMenu(pWnd,point);
}
//------------------------------------------------------------------------------------------//
void CMyMFCRibbonStatusBar::DoPB(void){
	if (m_blPBStart){
		int	pos;
		pos = m_ProgressBar->GetPos() + 2;
		if (pos > 60)
			pos = 2;
		m_ProgressBar->SetPos(pos);
	}
	else{
		//m_ProgressBar->SetPos(60);
	}
}
//------------------------------------------------------------------------------------------//
void CMyMFCRibbonStatusBar::SetPB(BOOL blStart){
	m_blPBStart = blStart;
}
//------------------------------------------------------------------------------------------//
