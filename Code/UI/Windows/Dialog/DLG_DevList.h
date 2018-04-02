#pragma once
#include "afxcmn.h"
#include "COM_ViewList.h"
#include "COM_ToolBar.h"
//------------------------------------------------------------------------------------------//
class CDevListCtrl : public CMyCListCtrl{
	public:
				 CDevListCtrl(void);
		virtual ~CDevListCtrl(void){;};
	protected:// Overrides
		virtual void	OnEditSave		(int hItem,int hSubItem);
		virtual BOOL	DoShowExItem	(int hItem,int hSubItem,int Type = 0);
		virtual void	OnComboBoxShow	(int hItem,int hSubItem);
		virtual void	OnComboBoxSave	(int hItem,int hSubItem);
	protected:
		IPCOMLIST	*m_IPCOMLIST;
	public:
		void	LoadData		(IPCOMLIST *tIPCOMLIST);
		int		CreateNode		(int node);
		int		GetSelectedNode	(void){return(m_SelectItem);};
		int		DelNode			(int delItem);
		int		UpNode			(int moveItem);
		int		DownNode		(int moveItem);
	protected:
		afx_msg int OnCreate	(LPCREATESTRUCT lpCreateStruct);
		DECLARE_MESSAGE_MAP()
};
//------------------------------------------------------------------------------------------//
class CDlgDevList : public CDialog{
	DECLARE_DYNAMIC(CDlgDevList)
	public:
		enum { IDD = IDD_DEVLIST };
				 CDlgDevList(CWnd* pParent = NULL);   // standard constructor
		virtual ~CDlgDevList(void){;};
	protected:
		CDevListCtrl	m_wndDevList;
		CMyToolBar		m_wndToolBar;
		int			CreateDevListBox	(void);
		int			CreateDevListToolBar(void);
	public:
		void	AdjustLayout			(void);
		void	OnChangeVisualStyle		(void);
	protected:
		virtual void DoDataExchange		(CDataExchange* pDX);    // DDX/DDV support
		virtual BOOL OnInitDialog		(void);
	protected:
		afx_msg void OnDL_Add			(void);
		afx_msg void OnDL_Del			(void);
		afx_msg void OnDL_Up			(void);
		afx_msg void OnDL_Down			(void);
		afx_msg void OnDL_Refresh		(void);
		afx_msg void OnDL_Load			(void);
		afx_msg void OnDL_Import		(void);
		afx_msg void OnDL_Save			(void);
		DECLARE_MESSAGE_MAP()
};
//------------------------------------------------------------------------------------------//