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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//------------------------------------------------------------------------------------------//
IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)
//------------------------------------------------------------------------------------------//
CChildFrame::CChildFrame(void){
	m_bCanClose = FALSE;
}
//------------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()
//------------------------------------------------------------------------------------------//
CChildFrame::~CChildFrame(void){
	m_VG3D.UnregisterFromCache();
	cstdout.RemoveSelf();
}
//------------------------------------------------------------------------------------------//
void CChildFrame::OnClose(void){
	if (m_bCanClose == TRUE){
		m_VG3D.UnregisterFromCache();
		cstdout.RemoveSelf();
		CMDIChildWndEx::OnClose();
	}
	else if(theApp.m_bCanClose == TRUE){
		m_VG3D.UnregisterFromCache();
		cstdout.RemoveSelf();
		CMDIChildWndEx::OnClose();
	}
}
//------------------------------------------------------------------------------------------//
BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs){
	//cs.style &= ~FWS_ADDTOTITLE; //禁用MFC的默认标题功能

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