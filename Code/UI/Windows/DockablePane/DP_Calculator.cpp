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
#include "DP_Calculator.h"
#include "MainFrm.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//------------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CPaneCEdit, CEdit)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()
//------------------------------------------------------------------------------------------//
BOOL CPaneCEdit::PreTranslateMessage(MSG* pMsg){
	UINT  nCode = pMsg->wParam;
	if (pMsg->message == WM_KEYDOWN){
		if (nCode == _T('A') && (::GetKeyState(VK_CONTROL) & 0x8000)){
			SetSel(0,-1);
			return TRUE;
		}
	}
	return(CEdit::PreTranslateMessage(pMsg));
}
//------------------------------------------------------------------------------------------//
void CPaneCEdit::OnLButtonDblClk(UINT nFlags, CPoint point){
	SetSel(0,-1);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CCaclViewDP, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_SETFOCUS()
	ON_WM_PAINT()
END_MESSAGE_MAP()
//------------------------------------------------------------------------------------------//
int CCaclViewDP::OnCreate(LPCREATESTRUCT lpCreateStruct){
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();
	if (!m_cView.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rectDummy,this,0)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}

	AdjustLayout();
	return 0;
}
//------------------------------------------------------------------------------------------//
void CCaclViewDP::AdjustLayout(void){
	if (GetSafeHwnd() == NULL)
		return;

	CRect rectT;
	GetClientRect(rectT);
	m_cView.SetWindowPos(NULL, rectT.left + 1, rectT.top + 1, rectT.Width() - 2, rectT.Height() - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}
//------------------------------------------------------------------------------------------//
void CCaclViewDP::OnSize(UINT nType, int cx, int cy){
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}
//------------------------------------------------------------------------------------------//
void CCaclViewDP::OnSetFocus(CWnd* pOldWnd){
	CDockablePane::OnSetFocus(pOldWnd);
}
//------------------------------------------------------------------------------------------//
void CCaclViewDP::OnPaint(void){
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_cView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}
//------------------------------------------------------------------------------------------//
void CCaclViewDP::OnContextMenu(CWnd* pWnd, CPoint point){
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------/
IMPLEMENT_DYNCREATE(CCaclPaneView, CView)
//------------------------------------------------------------------------------------------/
BEGIN_MESSAGE_MAP(CCaclPaneView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEACTIVATE()
	ON_COMMAND(ID_CACLBOX_HEX, 		&CCaclPaneView::OnHex)
	ON_COMMAND(ID_CACLBOX_ESCAPE, 	&CCaclPaneView::OnEscape)
	ON_COMMAND(ID_CACLBOX_LENGTH, 	&CCaclPaneView::OnLength)
	ON_COMMAND(ID_CACLBOX_H2A, 		&CCaclPaneView::OnH2A)
	ON_COMMAND(ID_CACLBOX_A2H, 		&CCaclPaneView::OnA2H)
	ON_COMMAND(ID_CACLBOX_XOR, 		&CCaclPaneView::OnXor)
	ON_COMMAND(ID_CACLBOX_OSP, 		&CCaclPaneView::OnOsp)
	ON_COMMAND(ID_CACLBOX_CACL, 	&CCaclPaneView::OnCacl)
END_MESSAGE_MAP()
//------------------------------------------------------------------------------------------//
CCaclPaneView::CCaclPaneView(void) : CView(){
	m_ButtonFont.CreateFont
		(13							// nHeight
		,0							// nWidth
		,0							// nEscapement
		,0							// nOrientation
		,FW_NORMAL					// nWeight
		,FALSE						// bItalic
		,FALSE						// bUnderline
		,0							// cStrikeOut
		,ANSI_CHARSET				// nCharSet
		,OUT_DEFAULT_PRECIS			// nOutPrecision
		,CLIP_DEFAULT_PRECIS		// nClipPrecision
		,DEFAULT_QUALITY			// nQuality
		,DEFAULT_PITCH | FF_SWISS	// nPitchAndFamily
		,_T("Tahoma"));//Courier New
}
//------------------------------------------------------------------------------------------//
void CCaclPaneView::PostNcDestroy(void){   
// CView::PostNcDestroy(); 
}
//------------------------------------------------------------------------------------------//
int CCaclPaneView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message){
	return 0;
}
//------------------------------------------------------------------------------------------//
BOOL CCaclPaneView::PreCreateWindow(CREATESTRUCT& cs){
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CView::PreCreateWindow(cs);
}
//------------------------------------------------------------------------------------------//
#ifdef _DEBUG
void CCaclPaneView::AssertValid() const{
	CView::AssertValid();
}
void CCaclPaneView::Dump(CDumpContext& dc) const{
	CView::Dump(dc);
}
#endif //_DEBUG
//------------------------------------------------------------------------------------------//
BOOL CCaclPaneView::PreTranslateMessage(MSG* pMsg){
	if (pMsg->message == WM_KEYDOWN){
		UINT  nCode = pMsg->wParam;
		if ((nCode == _T('A') || nCode == _T('C') || nCode == _T('X') || nCode == _T('V') || nCode == _T('Z'))
			&&(::GetKeyState(VK_CONTROL) & 0x8000)){
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;
		}
		if ((pMsg->wParam == VK_RETURN)
			&& !(::GetKeyState(VK_CONTROL) & 0x8000)
			&& !(::GetKeyState(VK_SHIFT) & 0x8000)
			&& !(::GetKeyState(VK_MENU) & 0x8000)){
			OnCacl();
			return TRUE;
		}
	}
	return(CView::PreTranslateMessage(pMsg));
}
//------------------------------------------------------------------------------------------//
void CCaclPaneView::OnDraw(CDC* pDC){
	CRect rectTree;

	m_cEdit.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	pDC->Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}
//------------------------------------------------------------------------------------------//
void CCaclPaneView::OnSize(UINT nType, int cx, int cy){
	CView::OnSize(nType, cx, cy);
	AdjustLayout();
}
//------------------------------------------------------------------------------------------//
int CCaclPaneView::OnCreate(LPCREATESTRUCT lpCreateStruct){
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (CreateEdit() == -1)
		return -1;

	AdjustLayout();
	return 0;
}
//------------------------------------------------------------------------------------------//
int CCaclPaneView::CreateEdit(void){
	CRect rectDummy;
	rectDummy.SetRectEmpty();
	if (!m_cEdit.Create(ES_MULTILINE|ES_AUTOVSCROLL|WS_CHILD|WS_VISIBLE|WS_VSCROLL, rectDummy, this, 0)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}

	if (!m_GroupSetting.Create(_T("Setting"),WS_CHILD|WS_VISIBLE|BS_GROUPBOX, rectDummy, this, 0)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	if (!m_CheckBoxHex.Create(_T("Hex"),WS_CHILD|WS_VISIBLE|BS_CHECKBOX, rectDummy, this, ID_CACLBOX_HEX)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	if (!m_CheckBoxEscape.Create(_T("EnEscape"),WS_CHILD|WS_VISIBLE|BS_CHECKBOX, rectDummy, this, ID_CACLBOX_ESCAPE)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	if (!m_RadioLength.Create(_T("Input Char Length"),WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON, rectDummy, this, ID_CACLBOX_LENGTH)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	if (!m_RadioH2A.Create(_T("HEX To ASCII"),WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON, rectDummy, this, ID_CACLBOX_H2A)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	if (!m_RadioA2H.Create(_T("ASCII To HEX"),WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON, rectDummy, this, ID_CACLBOX_A2H)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	if (!m_RadioXor.Create(_T("XOR Checksum (NMEA)"),WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON, rectDummy, this, ID_CACLBOX_XOR)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	if (!m_RadioOSP.Create(_T("Sirf OSP Checksum"),WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON, rectDummy, this, ID_CACLBOX_OSP)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	if (!m_ButtonCacl.Create(_T("Cacl"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, rectDummy, this, ID_CACLBOX_CACL)){
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}

	m_CheckBoxHex.SetFont(&m_ButtonFont);
	m_CheckBoxEscape.SetFont(&m_ButtonFont);
	m_CheckBoxEscape.SetCheck(TRUE);
	m_RadioLength.SetFont(&m_ButtonFont);
	m_RadioLength.SetCheck(TRUE);
	m_RadioH2A.SetFont(&m_ButtonFont);
	m_RadioA2H.SetFont(&m_ButtonFont);
	m_RadioXor.SetFont(&m_ButtonFont);
	m_RadioOSP.SetFont(&m_ButtonFont);
	m_RadioXor.SetFont(&m_ButtonFont);
	m_ButtonCacl.SetFont(&m_ButtonFont);
	m_cEdit.SetFont(&m_ButtonFont);
	return 0;
}
//------------------------------------------------------------------------------------------//
void CCaclPaneView::AdjustLayout(void){
	CRect rectClient,rectGroupS,rectGroupT;
	GetClientRect(rectClient);

	rectClient.InflateRect(-5, -5);

	rectGroupS = rectClient;
	rectGroupS.top = rectGroupS.bottom - 105;
	m_GroupSetting.SetWindowPos(NULL, rectGroupS.left, rectGroupS.top, rectGroupS.Width(), rectGroupS.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

	rectGroupT = rectClient;
	rectGroupT.bottom = rectGroupS.top - 5;
	m_cEdit.SetWindowPos(NULL, rectGroupT.left, rectGroupT.top, rectGroupT.Width(), rectGroupT.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

	rectGroupT = rectGroupS;
	rectGroupT.left += 10;
	rectGroupT.right = rectGroupT.left + 36;
	rectGroupT.top += 20;
	rectGroupT.bottom = rectGroupT.top + 20;
	m_CheckBoxHex.SetWindowPos(NULL, rectGroupT.left, rectGroupT.top, rectGroupT.Width(), rectGroupT.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

	rectGroupT.left = rectGroupT.right + 25;
	rectGroupT.right = rectGroupT.left + 70;
	m_CheckBoxEscape.SetWindowPos(NULL, rectGroupT.left, rectGroupT.top, rectGroupT.Width(), rectGroupT.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

	rectGroupT.left = rectGroupT.right + 25;
	rectGroupT.right = rectGroupT.left + 110;
	m_RadioLength.SetWindowPos(NULL, rectGroupT.left, rectGroupT.top, rectGroupT.Width(), rectGroupT.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

	rectGroupT.left = rectGroupS.left + 10;
	rectGroupT.right = rectGroupT.left + 80;
	rectGroupT.top += 20;
	rectGroupT.bottom = rectGroupT.top + 20;
	m_RadioH2A.SetWindowPos(NULL, rectGroupT.left, rectGroupT.top, rectGroupT.Width(), rectGroupT.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

	rectGroupT.left = rectGroupT.right + 10;
	rectGroupT.right = rectGroupT.left + 80;
	m_RadioA2H.SetWindowPos(NULL, rectGroupT.left, rectGroupT.top, rectGroupT.Width(), rectGroupT.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

	rectGroupT.left = rectGroupS.left + 10;
	rectGroupT.right = rectGroupT.left + 150;
	rectGroupT.top += 20;
	rectGroupT.bottom = rectGroupT.top + 20;
	m_RadioXor.SetWindowPos(NULL, rectGroupT.left, rectGroupT.top, rectGroupT.Width(), rectGroupT.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

	rectGroupT.top += 20;
	rectGroupT.bottom = rectGroupT.top + 20;
	m_RadioOSP.SetWindowPos(NULL, rectGroupT.left, rectGroupT.top, rectGroupT.Width(), rectGroupT.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

	rectGroupT = rectGroupS;
	rectGroupT.left += 190;
	rectGroupT.right -= 10;
	rectGroupT.top += 40;
	rectGroupT.bottom -= 10;
	m_ButtonCacl.SetWindowPos(NULL, rectGroupT.left, rectGroupT.top, rectGroupT.Width(), rectGroupT.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}
//------------------------------------------------------------------------------------------//
void CCaclPaneView::OnHex(void){
	m_CheckBoxHex.SetCheck(!m_CheckBoxHex.GetCheck());
	if (m_CheckBoxHex.GetCheck())
		m_CheckBoxEscape.SetCheck(FALSE);
}
//------------------------------------------------------------------------------------------//
void CCaclPaneView::OnEscape(void){
	m_CheckBoxEscape.SetCheck(!m_CheckBoxEscape.GetCheck());
	if (m_CheckBoxEscape.GetCheck())
		m_CheckBoxHex.SetCheck(FALSE);
}
//------------------------------------------------------------------------------------------//
void CCaclPaneView::OnLength(void){
	m_RadioLength.SetCheck(TRUE);
	m_RadioH2A.SetCheck(FALSE);
	m_RadioA2H.SetCheck(FALSE);
	m_RadioXor.SetCheck(FALSE);
	m_RadioOSP.SetCheck(FALSE);
	m_CheckBoxHex.EnableWindow(TRUE);
	m_CheckBoxEscape.EnableWindow(TRUE);
}
//------------------------------------------------------------------------------------------//
void CCaclPaneView::OnH2A(void){
	m_RadioLength.SetCheck(FALSE);
	m_RadioH2A.SetCheck(TRUE);
	m_RadioA2H.SetCheck(FALSE);
	m_RadioXor.SetCheck(FALSE);
	m_RadioOSP.SetCheck(FALSE);
	m_CheckBoxHex.SetCheck(TRUE);
	m_CheckBoxHex.EnableWindow(FALSE);
	m_CheckBoxEscape.SetCheck(FALSE);
	m_CheckBoxEscape.EnableWindow(FALSE);
}
//------------------------------------------------------------------------------------------//
void CCaclPaneView::OnA2H(void){
	m_RadioLength.SetCheck(FALSE);
	m_RadioH2A.SetCheck(FALSE);
	m_RadioA2H.SetCheck(TRUE);
	m_RadioXor.SetCheck(FALSE);
	m_RadioOSP.SetCheck(FALSE);
	m_CheckBoxHex.EnableWindow(TRUE);
	m_CheckBoxEscape.EnableWindow(TRUE);
}
//------------------------------------------------------------------------------------------//
void CCaclPaneView::OnXor(void){
	m_RadioLength.SetCheck(FALSE);
	m_RadioH2A.SetCheck(FALSE);
	m_RadioA2H.SetCheck(FALSE);
	m_RadioXor.SetCheck(TRUE);
	m_RadioOSP.SetCheck(FALSE);
	m_CheckBoxHex.EnableWindow(TRUE);
	m_CheckBoxEscape.EnableWindow(FALSE);
	m_CheckBoxEscape.SetCheck(FALSE);
}
//------------------------------------------------------------------------------------------//
void CCaclPaneView::OnOsp(void){
	m_RadioLength.SetCheck(FALSE);
	m_RadioH2A.SetCheck(FALSE);
	m_RadioA2H.SetCheck(FALSE);
	m_RadioXor.SetCheck(FALSE);
	m_RadioOSP.SetCheck(TRUE);
	m_CheckBoxHex.EnableWindow(FALSE);
	m_CheckBoxHex.SetCheck(TRUE);
	m_CheckBoxEscape.EnableWindow(FALSE);
	m_CheckBoxEscape.SetCheck(FALSE);
}
//------------------------------------------------------------------------------------------//
void CCaclPaneView::OnCacl(void){
	CString			cResult;
	std::wstring	wstrText;
	std::string		strResult, strOutput;
	ODEV_STDOUT		*cstdout;


	cstdout = &((CMainFrame*)AfxGetMainWnd())->AuxCFrmCreate()->m_stdout;
	cstdout->Clean();

	m_cEdit.GetWindowText(cResult);
	wstrText = cResult;
	strResult = Str_UnicodeToANSI(wstrText);

	if (m_RadioXor.GetCheck()){
		if (m_CheckBoxHex.GetCheck())
			strResult = Str_HEXToASCII(strResult);
		cstdout->Write(nullptr,COL_clMaroon, IUD("XOR checksum = "));
		cstdout->Write(nullptr, COL_clBlue, CaclNMEAChecksum(strResult));
	}
	else if (m_RadioOSP.GetCheck()){
		cstdout->Write(nullptr, COL_clMaroon, IUD("SiRf Checksum = "));
		cstdout->Write(nullptr, COL_clBlue, CaclSiRfChecksum(strResult));
	}
	else if (m_RadioA2H.GetCheck()){
		if (m_CheckBoxHex.GetCheck()){
			strResult = Str_HEXToHEXs(strResult);
		}
		else{
			strResult = Str_ASCIIToHEXs(strResult,(m_CheckBoxEscape.GetCheck() == 0)?G_ESCAPE_OFF:G_ESCAPE_ON);
		}
		cstdout->Write(nullptr, COL_clMaroon, IUD("Transform To HEX:\r\n"));
		cstdout->Write(nullptr, COL_clBlue, strResult);
	}
	else if (m_RadioH2A.GetCheck()){
		cstdout->Write(nullptr, COL_clMaroon, IUD("Transform To ASCII:\r\n"));
		cstdout->Write(nullptr, COL_clBlue, Str_HEXToASCII(strResult));
	}
	else if (m_RadioLength.GetCheck()){
		if (m_CheckBoxHex.GetCheck()){
			strResult = Str_ToStr(CaclHEXLength(strResult));
		}
		else{
			strResult = Str_ToStr(CaclASCIILength(strResult));
		}
		cstdout->Write(nullptr, COL_clMaroon, IUD("Input Character Length = "));
		cstdout->Write(nullptr, COL_clBlue, strResult);
	}

	cstdout->WriteNL(nullptr, COL_DivLine, IUD(DIVIDING_LINE));
	cstdout->Write(nullptr, COL_clMaroon, IUD("Input Character: \r\n"));
	cstdout->Write(nullptr, COL_clPurple, Str_UnicodeToANSI(wstrText));
	
	cstdout->ToHome();
}
//------------------------------------------------------------------------------------------//
std::string CCaclPaneView::CaclNMEAChecksum(const std::string &strInput){
	uint8		result,temp;
	uint32		length;
	ARRAY 		class_FIFO;

	length = strInput.length();
	class_FIFO.InitSize(length);
	class_FIFO.Put(strInput,G_ESCAPE_OFF);
	class_FIFO.Get(&result,1);
	while(class_FIFO.Get(&temp,1) != 0)
		result ^= temp;
	return(Str_CharToHEX(&result,1,G_SPACE_OFF));
}
//------------------------------------------------------------------------------------------//
std::string CCaclPaneView::CaclSiRfChecksum(const std::string &strInput){
	uint32		result;
	uint32		length;
	ARRAY 		class_FIFO;
	uint8		retChar[2],temp;

	length = strInput.length();
	class_FIFO.InitSize(length);
	class_FIFO.PutInHEX(strInput);
	result = 0;
	while(class_FIFO.Get(&temp,1) != 0){
		result += temp;
		result &= 0x7fff;
	}
	retChar[0] = (uint8)((result & 0xff00) >> 8);
	retChar[1] = (uint8)(result & 0x00ff);
	return(Str_CharToHEX(retChar, 2, G_SPACE_OFF));
}
//------------------------------------------------------------------------------------------//
uint32 CCaclPaneView::CaclHEXLength(const std::string &strInput){
	uint32		length;
	ARRAY		class_FIFO;

	length = strInput.length();
	class_FIFO.InitSize(length);
	class_FIFO.PutInHEX(strInput);
	return(class_FIFO.Used());
}
//------------------------------------------------------------------------------------------//
uint32 CCaclPaneView::CaclASCIILength(const std::string &strInput){
	uint32		length;
	ARRAY 		class_FIFO;

	length = strInput.length();
	class_FIFO.InitSize(length);
	class_FIFO.Put(strInput,(m_CheckBoxEscape.GetCheck() == 0)?G_ESCAPE_OFF:G_ESCAPE_ON);
	return(class_FIFO.Used());
}
//------------------------------------------------------------------------------------------//