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

#pragma once
//------------------------------------------------------------------------------------------//
#include "COM_ViewList.h"
class GC_LIST;
class COMMAND_GROUP;
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class CGCCommandListCtrl : public CMyCListCtrl{
	public:
				 CGCCommandListCtrl(void);
		virtual ~CGCCommandListCtrl(void){;};
	protected:// Overrides
		virtual BOOL	DoShowExItem(int hItem,int hSubItem,int Type = 0){return FALSE;};
	protected:
		CFont			m_Font;
		COMMAND_GROUP	*m_Group;
	public:
		void	LoadData(COMMAND_GROUP *group, int32 blenforce = 0);
	protected:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		DECLARE_MESSAGE_MAP()
};
extern CGCCommandListCtrl	g_wndGCCommandList;
//------------------------------------------------------------------------------------------//
class CGCCommandViewDP : public CDockablePane{
	public:
				 CGCCommandViewDP(void);
		virtual ~CGCCommandViewDP(void);
	protected:
		int		CreateGCList(void);
	public:
		void	AdjustLayout(void);
	protected:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
		afx_msg void OnPaint(void);
		afx_msg void OnSetFocus(CWnd* pOldWnd);
		DECLARE_MESSAGE_MAP()
};
//------------------------------------------------------------------------------------------//
