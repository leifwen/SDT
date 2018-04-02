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

// ChildFrm.cpp : implementation of the CChildFrame class
//
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Global.h"
#include "SDT.h"
#include "ChildFrm.h"
#include "RichView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//------------------------------------------------------------------------------------------//
IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)
//------------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()
//------------------------------------------------------------------------------------------//
CChildFrame::CChildFrame(void) : CMDIChildWndEx(){
	
	m_bCanClose = FALSE;
}
//------------------------------------------------------------------------------------------//
CChildFrame::~CChildFrame(void){
	m_VG3D.UnregisterToCache();
	m_stdout.RemoveSelf();
}
//------------------------------------------------------------------------------------------//
void CChildFrame::InitConsole(CONSOLE* console){
	CMyRichView*	richView;
	richView = (CMyRichView*)GetActiveView();
	richView->m_Console = console;
};
//------------------------------------------------------------------------------------------//
void CChildFrame::InitSTDOUT(const STDSTR& selfName){
	m_selfName = selfName;
	m_stdout.selfName = selfName + ".STDOUT";
	m_VG3D.selfName = selfName + ".VG3D";
	m_stdout.Init(GetActiveView());
}
//------------------------------------------------------------------------------------------//
void CChildFrame::InitVG3D(const STDSTR& selfName,OUTPUT_CACHE* cache){
	InitSTDOUT(selfName);
	m_VG3D.RegisterToCache((OUTPUT_CACHE*)cache);
	m_VG3D.SetExtraGroup(0);
	m_VG3D.AddG3D_STDOUT(&m_stdout);
}
//------------------------------------------------------------------------------------------//
void CChildFrame::OnClose(void){
	if ((m_bCanClose == TRUE) || (theApp.m_bCanClose == TRUE)){
		m_VG3D.UnregisterToCache();
		m_stdout.RemoveSelf();
		CMDIChildWndEx::OnClose();
	}
}
//------------------------------------------------------------------------------------------//
BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs){
	//cs.style &= ~FWS_ADDTOTITLE; 

	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if(!CMDIChildWndEx::PreCreateWindow(cs))
		return FALSE;
	return TRUE;
}
//------------------------------------------------------------------------------------------//
// CChildFrame diagnostics
//------------------------------------------------------------------------------------------//
#ifdef _DEBUG
void CChildFrame::AssertValid() const{
	CMDIChildWndEx::AssertValid();
}
//------------------------------------------------------------------------------------------//
void CChildFrame::Dump(CDumpContext& dc) const{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG
//------------------------------------------------------------------------------------------//
// CChildFrame message handlers