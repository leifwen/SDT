
// SDTRegDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SDTReg.h"
#include "SDTRegDlg.h"
#include "afxdialogex.h"
#include "Comm_FIFO.h"
#include "Comm_Convert.h"
#include "SYS_Time.h"
#include "Comm_File.h"
#include "Comm_Crypto.h"
#include "Checker.h"
#include "SMC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CSDTRegDlg::CSDTRegDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSDTRegDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSDTRegDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, ShowEdit);
}

BEGIN_MESSAGE_MAP(CSDTRegDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CSDTRegDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSDTRegDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CSDTRegDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CSDTRegDlg message handlers

BOOL CSDTRegDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSDTRegDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{

	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSDTRegDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSDTRegDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//------------------------------------------------------------------------------------------//
void CSDTRegDlg::OnDropFiles(HDROP hDropInfo){
	int				nFileCount;
	TCHAR			szFileName[MAX_PATH];

	nFileCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, MAX_PATH);
	if (nFileCount == 1){
		ZeroMemory(szFileName,MAX_PATH);
		DragQueryFile(hDropInfo,0,szFileName,MAX_PATH);
		m_FileName = szFileName;

		ShowEdit.SetSel(ShowEdit.GetWindowTextLength(),ShowEdit.GetWindowTextLength());
		ShowEdit.ReplaceSel(m_FileName);
		ShowEdit.SetSel(ShowEdit.GetWindowTextLength(),ShowEdit.GetWindowTextLength());
		ShowEdit.ReplaceSel(_T("\r\n"));
	}
	DragFinish (hDropInfo);
}
//------------------------------------------------------------------------------------------//
void CSDTRegDlg::OnBnClickedButton1()
{
	std::wstring	file;
	std::string		ret;
	Reg_SDT_PatchCode	SDTPC;

	file = m_FileName;
	if (file.length() > 0){
		SDTPC.PatchToSDT(Str_UnicodeToANSI(file), &ret);
		ShowEdit.SetSel(ShowEdit.GetWindowTextLength(),ShowEdit.GetWindowTextLength());
		ShowEdit.ReplaceSel(Str_ANSIToUnicode(ret).c_str());
	}
}
//------------------------------------------------------------------------------------------//
void CSDTRegDlg::OnBnClickedButton2()
{
	std::wstring	file;
	std::string		ret,strContent,strReg;
	SYS_DateTime	aTime;
	Linense_Signature	tLS;

	file = m_FileName;
	
	SYS_EncodeTimeABS(&aTime,"99/00/00,00/00/00");

	if (file.length() > 0){
		ret = "Create 99Y \"License.key\" ";
		CFS_ReadFile(&strReg, Str_UnicodeToANSI(file));
		if (tLS.Encode(&strContent, strReg, (uint64)aTime.GetSec()) > 0){
			CFS_WriteFile("License.key", strContent);
			ret += "successful.\r\n";
		}
		else{
			ret += "fail.\r\n";
		}
		ShowEdit.SetSel(ShowEdit.GetWindowTextLength(),ShowEdit.GetWindowTextLength());
		ShowEdit.ReplaceSel(Str_ANSIToUnicode(ret).c_str());
	}
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//

void CSDTRegDlg::OnBnClickedButton4()
{
	std::wstring	file;
	std::string		ret, strContent, strReg;
	SYS_DateTime	aTime;
	Linense_Signature	tLS;

	file = m_FileName;
	
	SYS_EncodeTimeABS(&aTime,"00/00/00,02/00/00");

	if (file.length() > 0){
		ret = "Create 2H \"License.key\" ";
		CFS_ReadFile(&strReg, Str_UnicodeToANSI(file));
		if (tLS.Encode(&strContent, strReg, (uint64)aTime.GetSec()) > 0){
			CFS_WriteFile("License.key", strContent);
			ret += "successful.\r\n";
		}
		else{
			ret += "fail.\r\n";
		}
		ShowEdit.SetSel(ShowEdit.GetWindowTextLength(),ShowEdit.GetWindowTextLength());
		ShowEdit.ReplaceSel(Str_ANSIToUnicode(ret).c_str());
	}
}
