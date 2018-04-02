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
#include "COM_ViewTree.h"
#include "DP_GCProperties.h"
#include "DP_GCCommand.h"
#include "COM_ToolBar.h"
class GC_LIST;
class COMMAND_GROUP;
//------------------------------------------------------------------------------------------//
class CGCTree : public CMyCTreeCtrl{
	public:
				 CGCTree(void);
		virtual ~CGCTree(void){;};
	public:
		GC_LIST*		m_GCList;
	protected:
		COMMAND_NODE	m_CommandNode;
		HTREEITEM		m_OnShowGroup;

		BOOL		DoShowEdit		(HTREEITEM hItem);
		void		OnEditSave		(HTREEITEM hItem);
		void		OnEditCheckSave	(HTREEITEM hItem);
		void		SaveNodeL2		(HTREEITEM hItem);
		void		SaveNodeL3		(HTREEITEM hItem); 
		void		SaveNodeCheckL2(HTREEITEM hItem);
		void		SaveNodeCheckL3	(HTREEITEM hItem);
		HTREEITEM	CreateNodeL1	(HTREEITEM hItem = NULL);
		HTREEITEM	CreateNodeL2	(HTREEITEM hItem = NULL);
		HTREEITEM	CreateNodeL3	(HTREEITEM hItem = NULL);
		HTREEITEM	DownNodeL1		(HTREEITEM moveItem);
		HTREEITEM	DownNodeL2		(HTREEITEM moveItem);
		HTREEITEM	DownNodeL3		(HTREEITEM moveItem);
		HTREEITEM	UpNodeL1		(HTREEITEM moveItem);
		HTREEITEM	UpNodeL2		(HTREEITEM moveItem);
		HTREEITEM	UpNodeL3		(HTREEITEM moveItem);
		HTREEITEM	DelNodeL1		(HTREEITEM delItem);
		HTREEITEM	DelNodeL2		(HTREEITEM delItem);
		HTREEITEM	DelNodeL3		(HTREEITEM delItem);
		void		LoadNodeL3		(HTREEITEM rootItem,COMMAND_GROUP* group);
		BOOL		SelectItem		(HTREEITEM hItem);
	public:
		void		LoadData		(GC_LIST* tGCList);
		HTREEITEM	CreateNode		(HTREEITEM hItem = NULL);
		HTREEITEM	DelNode			(HTREEITEM hItem);
		HTREEITEM	UpNode			(HTREEITEM hItem);
		HTREEITEM	DownNode		(HTREEITEM hItem);
		COMMAND_GROUP	*GetGroup	(HTREEITEM hItem);
		COMMAND_NODE	*GetCommand	(HTREEITEM hItem);
		void		UpdataDock		(HTREEITEM hItem, int32 blenforce);
	protected:
		afx_msg void OnLButtonDown	(UINT nFlags, CPoint point);
		afx_msg void OnRButtonDown	(UINT nFlags, CPoint point);
		afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
		afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
		DECLARE_MESSAGE_MAP()
};
//------------------------------------------------------------------------------------------//
class CGCViewDP : public CDockablePane{
	public:
				 CGCViewDP(void) : CDockablePane(){;};
		virtual ~CGCViewDP(void){;};
	protected:
		CMyToolBar		m_wndToolBar;
	protected:
		int		CreateGCTree			(void);
		int		CreateGCToolBar			(void);
	public:
		void	AdjustLayout			(void);
		void	OnChangeVisualStyle		(void);
	protected:
		afx_msg int  OnCreate			(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnSize				(UINT nType, int cx, int cy);
		afx_msg void OnContextMenu		(CWnd* pWnd, CPoint point);
		afx_msg void OnPaint			(void);
		afx_msg void OnSetFocus			(CWnd* pOldWnd);

		afx_msg void OnGC_Add			(void);
		afx_msg void OnGC_Del			(void);
		afx_msg void OnGC_Up			(void);
		afx_msg void OnGC_Down			(void);
		afx_msg void OnGC_Expand		(void);
		afx_msg void OnGC_Collapse		(void);
		afx_msg void OnUpdateGC_Add		(CCmdUI* pCmdUI);
		afx_msg void OnUpdateGC_Del		(CCmdUI* pCmdUI);
		afx_msg void OnUpdateGC_Up		(CCmdUI* pCmdUI);
		afx_msg void OnUpdateGC_Down	(CCmdUI* pCmdUI);
		afx_msg void OnUpdateGC_Edit	(CCmdUI* pCmdUI);
		afx_msg void OnUpdateGC_LOAD	(CCmdUI* pCmdUI);
		afx_msg void OnUpdateGC_IMPORT	(CCmdUI* pCmdUI);
		afx_msg void OnUpdateGC_SAVE	(CCmdUI* pCmdUI);

		afx_msg void OnButtonClickGC_LOAD	(void);
		afx_msg void OnButtonClickGC_IMPORT	(void);
		afx_msg void OnButtonClickGC_SAVE	(void);
		afx_msg void OnButtonClickGC_STOP	(void);
		afx_msg void OnButtonClickGC_Run	(void);
		DECLARE_MESSAGE_MAP()
	public:
		static CGCTree& GetGCTree			(void);
};
//------------------------------------------------------------------------------------------//
