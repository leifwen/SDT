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

// SDT.h : main header file for the SDT application
//
//------------------------------------------------------------------------------------------//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "Global.h"
//------------------------------------------------------------------------------------------//
class CSDTApp : public CWinAppEx{
	public:
		CSDTApp(void);
	public:// Overrides
		virtual BOOL	InitInstance(void);
		virtual int		ExitInstance(void);
	// Implementation
		UINT	m_nAppLook;
		BOOL	m_bHiColorIcons;
		BOOL	m_bCanClose;
		KERNEL	GSDTApp;
	public:
		virtual void PreLoadState	(void);
		virtual void LoadCustomState(void);
		virtual void SaveCustomState(void);
	public:
		DECLARE_MESSAGE_MAP()
};
//------------------------------------------------------------------------------------------//
#define WM_ON_BICCLEAN		(WM_USER + 100)
extern CSDTApp theApp;
//------------------------------------------------------------------------------------------//