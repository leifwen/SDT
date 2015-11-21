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

// MainFrm.h : interface of the CMainFrame class
//
//------------------------------------------------------------------------------------------//
#pragma once
#include "DP_SC.h"
#include "DP_GC.h"
#include "DP_GCCommand.h"
#include "DP_GCProperties.h"
#include "DP_SocketList.h"
#include "RibbonStatusBar.h"
#include "DLG_DevList.h"
#include "ChildFrm.h"
#include "RichView.h"
#include "DP_Send.h"
#include "DP_Calculator.h"
#include "CMUXView.h"
#include "Global.h"
//------------------------------------------------------------------------------------------//
class CMainFrame : public CMDIFrameWndEx{
	DECLARE_DYNAMIC(CMainFrame)
	public:
				 CMainFrame(void);
		virtual ~CMainFrame(void);
	public:// Attributes
	public:// Operations
	public:// Overrides
		virtual BOOL PreCreateWindow(CREATESTRUCT &cs);
		virtual BOOL PreTranslateMessage(MSG *pMsg);
	public:// Implementation
		#ifdef _DEBUG
			virtual void AssertValid() const;
			virtual void Dump(CDumpContext& dc) const;
		#endif
		CChildFrame					*m_MCFrm;
		CChildFrame					*m_CMUXFrm;
		CCMUXView					*m_CMUXView;
		ODEV_NODE_SDOUT				m_oDevNodeAux;
		CString						m_MyTitle;
		MBIM_DevService				*m_MBIM_DevService;
		void		AuxCFrmCreate	(void);
		void		CMUXFrmCreate	(void);
	protected:  // control bar embedded members
		ITaskbarList3				*m_Pitl3;
		BOOL						m_Pitl3Created;
		CMFCRibbonBar				m_wndRibbonBar;
		CMFCRibbonApplicationButton	m_MainButton;
		CMyMFCRibbonStatusBar		m_wndStatusBar;
		CSCViewDP					m_wndSC;
		CGCViewDP					m_wndGC;
		CGCCommandViewDP			m_wndGCCommand;
		CGCPropertiesViewDP			m_wndGCProperties;
		CSocketListViewDP			m_wndSocket;
		CSendViewDP					m_wndSend;
		CCaclViewDP					m_wndCacl;
		CDlgDevList					m_DlgDevList;
		IPCOMLIST					m_ValidIPComList;
		IPCOMLIST					m_ValidAuxComList;
		INT32						m_blConnect;
		INT32						m_blAuxComConnect;
		INT32						m_blStop;
		BOOL						m_blDSRFlow;
		BOOL						m_blCTSFlow;
		BOOL						m_blAuxDSRFlow;
		BOOL						m_blAuxCTSFlow;
		BOOL						m_blCMUXFC;
		CString						m_localIP;
	protected:
		LRESULT	WindowProc				(UINT message, WPARAM wParam, LPARAM lParam);
		BOOL	CreateDockingWindows	(void);
		void	SetDockingWindowIcons	(BOOL bHiColorIcons);
		void	UIInit					(void);
		
		void	InitRibbonBar			(void);
		void	InitComboBox_COM		(void);
		void	InitComboBox_BR			(void);
		void	CreateComboBox_COMList	(void);
		void	SetSelectComboBox_COM	(void);
		#ifdef	SWVERSION_AUXCOM
		void	InitComboBox_AuxCOM		(void);
		void	InitComboBox_AuxBR		(void);
		void	CreateComboBox_AuxCOMList(void);
		void	SetSelectComboBox_AuxCOM(void);
		BOOL	DoAuxopen				(void);
		#endif
		int32	CheckCOMInput			(std::string strInput,std::string *strOutput);
		int32	CheckTCPInput			(std::string strInput,std::string *strOutput);
		int32	CheckUDPInput			(std::string strInput,std::string *strOutput);
		int32	CheckTCPServerInput		(std::string strInput,std::string *strOutput);
		int32	CheckUDPServerInput		(std::string strInput,std::string *strOutput);
		DEVICE::OPEN_TYPE	CheckIPComInput(std::string strInput, std::string *strOutput);
		int32	DoConnect				(void);
		void	DoScriptStop			(void);
		BOOL	IsWin7					(void);
	protected:// Generated message map functions
		afx_msg LRESULT OnSetText(WPARAM wParam,LPARAM lParam);
		afx_msg void	OnClose(void);
		afx_msg void	OnSysCommand(UINT nID, LPARAM lParam);
		afx_msg int		OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void	OnWindowManager();
		afx_msg void	OnApplicationLook(UINT id);
		afx_msg void	OnUpdateApplicationLook(CCmdUI* pCmdUI);
		
		afx_msg void	OnCOMBO_COM		(void);
		afx_msg void	OnCOMBO_BR		(void);
		afx_msg void	OnDSR_FLOW		(void);
		afx_msg void	OnCTS_FLOW		(void);
		afx_msg void	OnDTRHigh		(void);
		afx_msg void	OnDTRLow		(void);
		afx_msg void	OnRTSHigh		(void);
		afx_msg void	OnRTSLow		(void);
		afx_msg void	OnASCII			(void);
		afx_msg void	OnHEX			(void);
		afx_msg void	OnMSR			(void);
		afx_msg void	OnOSPMSG		(void);
		afx_msg void	OnRECMSG		(void);
		afx_msg void	OnDisplay		(void);
		
		afx_msg void	OnButtonClickConnect	(void);
		afx_msg void	OnButtonClickScriptStop	(void);
		afx_msg void	OnButtonClickEcho		(void);
		afx_msg void	OnButtonClickScriptAT	(void);
		afx_msg void	OnButtonClickScriptCE	(void);
		afx_msg void	OnButtonClickScriptShowSBIC(void);
		afx_msg void	OnButtonClickScriptSC	(void);
		afx_msg void	OnButtonClickScriptGC	(void);
		afx_msg void	OnButtonClickClean		(void);
		afx_msg void	OnButtonClickDevList	(void);
		afx_msg void	OnButtonClickScoketlist	(void);
		afx_msg void	OnButtonClickRecodrDir	(void);
		afx_msg void	OnButtonClickNewRecord	(void);
		afx_msg void	OnButtonClickSend		(void);
		afx_msg void	OnButtonClickCacl		(void);

		afx_msg void	OnUpdateEcho	(CCmdUI *pCmdUI);
		afx_msg void	OnUpdateDSR_FLOW(CCmdUI *pCmdUI);
		afx_msg void	OnUpdateCTS_FLOW(CCmdUI *pCmdUI);
		afx_msg void	OnUpdateDTRHigh	(CCmdUI *pCmdUI);
		afx_msg void	OnUpdateDTRLow	(CCmdUI *pCmdUI);
		afx_msg void	OnUpdateRTSHigh	(CCmdUI *pCmdUI);
		afx_msg void	OnUpdateRTSLow	(CCmdUI *pCmdUI);
		afx_msg void	OnUpdateConnect	(CCmdUI *pCmdUI);
		afx_msg void	OnUpdateScrpit	(CCmdUI *pCmdUI);
		afx_msg void	OnUpdateASCII	(CCmdUI *pCmdUI);
		afx_msg void	OnUpdateHEX		(CCmdUI *pCmdUI);
		afx_msg void	OnUpdateMSR		(CCmdUI *pCmdUI);
		afx_msg void	OnUpdateOSPMSG	(CCmdUI *pCmdUI);
		afx_msg void	OnUpdateRECMSG	(CCmdUI *pCmdUI);
		afx_msg void	OnUpdateDisplay	(CCmdUI *pCmdUI);
		afx_msg void	OnUpdateScriptAT(CCmdUI *pCmdUI);
		afx_msg void	OnUpdateScriptCE(CCmdUI *pCmdUI);
		afx_msg void	OnUpdateScriptShowSBIC(CCmdUI *pCmdUI);
		afx_msg void	OnUpdateCOMBO_COM(CCmdUI *pCmdUI);
		afx_msg void	OnUpdateCOMBO_BR(CCmdUI *pCmdUI);
		afx_msg void	OnUpdateScoketlist(CCmdUI *pCmdUI);
		#ifdef	SWVERSION_AUXCOM
		afx_msg void	OnRiComboAuxcom		(void);
		afx_msg void	OnRiComboAuxbr		(void);
		afx_msg void	OnRiCheckAuxescape	(void);
		afx_msg void	OnRiCheckAuxhex		(void);
		afx_msg void	OnRiCheckAuxascii	(void);
		afx_msg void	OnRiButtonAuxopen	(void);
		afx_msg void	OnRiCheckAuxdsr		(void);
		afx_msg void	OnRiCheckAuxdtrh	(void);
		afx_msg void	OnRiCheckAuxdtrl	(void);
		afx_msg void	OnRiCheckAuxcts		(void);
		afx_msg void	OnRiCheckAuxrtsh	(void);
		afx_msg void	OnRiCheckAuxrtsl	(void);
		afx_msg void	OnUpdateRiComboAuxcom	(CCmdUI *pCmdUI);
		afx_msg void	OnUpdateRiComboAuxbr	(CCmdUI *pCmdUI);
		afx_msg void	OnUpdateRiCheckAuxescape(CCmdUI *pCmdUI);
		afx_msg void	OnUpdateRiCheckAuxhex	(CCmdUI *pCmdUI);
		afx_msg void	OnUpdateRiCheckAuxascii	(CCmdUI *pCmdUI);
		afx_msg void	OnUpdateRiButtonAuxopen	(CCmdUI *pCmdUI);
		afx_msg void	OnUpdateRiCheckAuxdsr	(CCmdUI *pCmdUI);
		afx_msg void	OnUpdateRiCheckAuxdtrh	(CCmdUI *pCmdUI);
		afx_msg void	OnUpdateRiCheckAuxdtrl	(CCmdUI *pCmdUI);
		afx_msg void	OnUpdateRiCheckAuxcts	(CCmdUI *pCmdUI);
		afx_msg void	OnUpdateRiCheckAuxrtsh	(CCmdUI *pCmdUI);
		afx_msg void	OnUpdateRiCheckAuxrtsl	(CCmdUI *pCmdUI);
		#endif
		#ifdef	SWVERSION_CMUX
		afx_msg void	OnCMUXStart				(void);
		afx_msg void	OnCMUXStart1			(void);
		afx_msg void	OnCMUXCLD				(void);
		afx_msg void	OnCMUXPSC				(void);
		afx_msg void	OnCMUXPSC0				(void);
		afx_msg void	OnCMUXPSC1				(void);
		afx_msg void	OnCMUXPSC7				(void);
		afx_msg void	OnCMUXPSC9				(void);
		afx_msg void	OnCMUXFC				(void);
		afx_msg void	OnUpdateCMUXFC			(CCmdUI *pCmdUI);
		afx_msg void	OnUpdateCMUXStart		(CCmdUI *pCmdUI);
		afx_msg void	OnUpdateCMUXStart1		(CCmdUI *pCmdUI);
		#endif
		afx_msg void	OnButtonClickGCEdit		(void);
		afx_msg void	OnButtonClickGCPCOMMAND	(void);

		afx_msg	void OnUpdateSBAR_RX	(CCmdUI *pCmdUI);
		afx_msg void OnUpdateSBAR_TX	(CCmdUI *pCmdUI);
		afx_msg void OnUpdateSBAR_CTS	(CCmdUI *pCmdUI);
		afx_msg void OnUpdateSBAR_DSR	(CCmdUI *pCmdUI);
		afx_msg void OnUpdateSBAR_RING	(CCmdUI *pCmdUI);
		afx_msg void OnUpdateSBAR_DCD	(CCmdUI *pCmdUI);
		afx_msg void OnUpdateSBAR_GetIP	(CCmdUI *pCmdUI);
		afx_msg void OnUpdateSBAR_IP	(CCmdUI *pCmdUI);
		afx_msg void OnUpdateSBAR_NONE	(CCmdUI *pCmdUI);
		afx_msg void OnSBAR_GetIP		(void);
		afx_msg void OnTimer			(UINT nIDEvent);

		afx_msg void	OnAppAbout				(void);
		afx_msg LRESULT	OnPrintout(WPARAM wParam, LPARAM lParam);

		DECLARE_MESSAGE_MAP()
};
//------------------------------------------------------------------------------------------//
