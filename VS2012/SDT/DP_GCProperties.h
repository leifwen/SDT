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
class CPropertiesToolBar : public CMFCToolBar{
	public:
		virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler){
			CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
		};
		virtual BOOL AllowShowOnList() const {return FALSE;};
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class CGCPropertyGridCtrl : public CMFCPropertyGridCtrl{
	public:
		enum GCPtype{GCP_NONE,GCP_GROUP,GCP_COMMAND};
				 CGCPropertyGridCtrl(void);
		virtual ~CGCPropertyGridCtrl(void){;};
	protected:
		GCPtype			m_type;
		COMMAND_NODE	*m_command;
		COMMAND_GROUP	*m_group;
		HTREEITEM		m_hItem;
		mutable  BOOL	m_blIsModified;
		CMFCPropertyGridProperty *CreateEnableItem(INT32 id,CString title,int32 blEnable,CString description = _T(""));
		CMFCPropertyGridProperty *CreateCMDTailItem(INT32 id, CString title, CMD_TAIL tail, CString description = _T(""));
		CMFCPropertyGridProperty *CreateYesItem(INT32 id,CString title,int32 blEnable,CString description = _T(""));
		CMFCPropertyGridProperty *CreateTxtItem(INT32 id,CString title,CString content,CString description = _T(""));
		CMFCPropertyGridProperty *CreateIntItem(INT32 id,CString title,long content,CString description = _T(""));
		CMFCPropertyGridProperty *CreateFloItem(INT32 id,CString title,double content,CString description = _T(""));
		void	CreateCommandP(COMMAND_NODE *command);
		void	LoadCommandP(COMMAND_NODE *command);
		void	CreateGroupP(COMMAND_GROUP *group);
		void	LoadGroupP(COMMAND_GROUP *group);
		virtual void OnPropertyChanged(CMFCPropertyGridProperty* pProp) const;
	public:
		void	LoadInit	(void);
		void	LoadGroup(HTREEITEM hItem, COMMAND_GROUP *group, int32 blenforce = 0);
		void	LoadCommand(HTREEITEM hItem, COMMAND_NODE *command, int32 blenforce = 0);
	protected:
};
extern CGCPropertyGridCtrl	g_wndGCPropList;
//------------------------------------------------------------------------------------------//
class CGCPropertiesViewDP : public CDockablePane{
	// Construction
	public:
				 CGCPropertiesViewDP(void);
		virtual ~CGCPropertiesViewDP(void);
	protected:
		CPropertiesToolBar		m_wndToolBar;
	protected:
		int		CreatePropList(void);
		int		CreateGCToolBar(void);
	public:
		void	AdjustLayout(void);
		void	OnChangeVisualStyle(void);
		void	LoadGroup	(HTREEITEM hItem,COMMAND_GROUP *group);
		void	LoadCommand	(HTREEITEM hItem,COMMAND_NODE *command);
	protected:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg void OnSetFocus(CWnd* pOldWnd);
		afx_msg void OnPaint(void);
		afx_msg void OnExpandAllProperties();
		afx_msg void OnSortProperties();
		afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
		afx_msg void OnUpdateCOMMAND(CCmdUI* pCmdUI);
		DECLARE_MESSAGE_MAP()
};
//------------------------------------------------------------------------------------------//
