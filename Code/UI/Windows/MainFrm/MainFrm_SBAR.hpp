//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateSBAR_RX(CCmdUI *pCmdUI){
	CString	cText;
	if (theApp.GSDTApp.m_CDevBusM.IsOpened() != 0){
		pCmdUI->Enable();
		cText = _T("Received Bytes:  ");
		cText += Str_ANSIToUnicode(Str_ToStr(theApp.GSDTApp.m_CDevBusM.RxBytes())).c_str();
		pCmdUI->SetText(cText);
	}
	else{
		pCmdUI->Enable(FALSE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateSBAR_TX(CCmdUI *pCmdUI){
	CString	cText;
	if (theApp.GSDTApp.m_CDevBusM.IsOpened() != 0){
		pCmdUI->Enable();
		cText = _T("Send Bytes:  ");
		cText += Str_ANSIToUnicode(Str_ToStr(theApp.GSDTApp.m_CDevBusM.TxBytes())).c_str();
		pCmdUI->SetText(cText);
	}
	else{
		pCmdUI->Enable(FALSE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateSBAR_CTS(CCmdUI *pCmdUI){
	CString	cText;
	cText = "";
	if (theApp.GSDTApp.m_CDevBusM.EDA()->IsComOpened() != 0){
		pCmdUI->Enable();
		cText = _T("CTS = ");
		cText += Str_ANSIToUnicode(theApp.GSDTApp.m_CDevBusM.EDA()->AComCore()->GetCTSStatus()).c_str();
	}
	else{
		pCmdUI->Enable(FALSE);
	}
	pCmdUI->SetText(cText);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateSBAR_DSR(CCmdUI *pCmdUI){
	CString	cText;
	cText = "";
	if (theApp.GSDTApp.m_CDevBusM.EDA()->IsComOpened() != 0){
		pCmdUI->Enable();
		cText = _T("DSR = ");
		cText += Str_ANSIToUnicode(theApp.GSDTApp.m_CDevBusM.EDA()->AComCore()->GetDSRStatus()).c_str();
	}
	else{
		pCmdUI->Enable(FALSE);
	}
	pCmdUI->SetText(cText);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateSBAR_RING(CCmdUI *pCmdUI){
	CString	cText;
	cText = "";
	if (theApp.GSDTApp.m_CDevBusM.EDA()->IsComOpened() != 0){
		pCmdUI->Enable();
		cText = _T("RING = ");
		cText += Str_ANSIToUnicode(theApp.GSDTApp.m_CDevBusM.EDA()->AComCore()->GetRINGStatus()).c_str();
	}
	else{
		pCmdUI->Enable(FALSE);
	}
	pCmdUI->SetText(cText);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateSBAR_DCD(CCmdUI *pCmdUI){
	CString	cText;
	cText = "";
	if (theApp.GSDTApp.m_CDevBusM.EDA()->IsComOpened() != 0){
		pCmdUI->Enable();
		cText = _T("DCD = ");
		cText += Str_ANSIToUnicode(theApp.GSDTApp.m_CDevBusM.EDA()->AComCore()->GetDCDStatus()).c_str();
	}
	else{
		pCmdUI->Enable(FALSE);
	}
	pCmdUI->SetText(cText);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateSBAR_NONE(CCmdUI *pCmdUI){
	pCmdUI->Enable();
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateSBAR_GetIP(CCmdUI *pCmdUI){
	pCmdUI->Enable();
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateSBAR_IP(CCmdUI *pCmdUI){
	pCmdUI->SetText(m_localIP);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnSBAR_GetIP(void){
	CChildFrame*	pCFrm;
	std::string		stringData;
	char			HostName[64];
	HOSTENT			*lpHostEnt;
	std::wstring	wstrText;
	gethostname(HostName,sizeof(HostName));
	lpHostEnt = gethostbyname(HostName);
	if (lpHostEnt == NULL)
		return;

	if (lpHostEnt->h_addr_list[0] != NULL){
		stringData = inet_ntoa(*((struct in_addr *)(lpHostEnt->h_addr_list[0])));
		m_localIP = Str_ANSIToUnicode(stringData).c_str();
	}
	else{
		m_localIP = "255.255.255.255";
	}

	stringData = "HostName: ";
	stringData += HostName;
	stringData += "\r\n";
	
	pCFrm = AuxCFrmCreate();
	pCFrm->m_stdout.Clean();
	pCFrm->m_stdout.Write(nullptr, COL_clBlack, stringData);

	for (int i=0; lpHostEnt->h_addr_list[i]!=NULL; i++){
		stringData = inet_ntoa(*((struct in_addr *)(lpHostEnt->h_addr_list[i])));
		stringData = "Local IP: " + stringData + "\r\n";
		pCFrm->m_stdout.Write(nullptr, COL_clBlack, stringData);
	}
	stringData = "";
	SI_GetNetCardInfo(&stringData);
	pCFrm->m_stdout.Write(nullptr, COL_clBlack, stringData);
	pCFrm->m_stdout.ToHome();
}
//------------------------------------------------------------------------------------------//