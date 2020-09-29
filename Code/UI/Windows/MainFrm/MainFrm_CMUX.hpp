#ifdef	SWVERSION_CMUX
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateCMUXStart(CCmdUI *pCmdUI){
	if (m_CMUXView != NULL)
		m_CMUXView->RefreshCOM();
	pCmdUI->SetCheck(theApp.GSDTApp.m_CMUXDriver.IsOpened() != 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateCMUXStart1(CCmdUI *pCmdUI){
	if (m_CMUXView != NULL)
		m_CMUXView->RefreshCOM();
	pCmdUI->SetCheck(theApp.GSDTApp.m_CMUXDriver.IsOpened() != 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnCMUXStart(void){
	if (theApp.GSDTApp.m_CMUXDriver.IsOpened() != 0){
		theApp.GSDTApp.m_CMUXDriver.CloseSelf(0);
		return;
	}

	if (theApp.GSDTApp.m_CMUXDriver.GetCDevBus()->EDA()->IsComOpened() == 0)
		return;
	theApp.GSDTApp.m_CMUXDriver.Open(CMUX_DEFATCMDS, CMUXDriver::CMUX_blInitInThread);
	CMUXCFrmCreate();
	m_MCFrm->MDIActivate();
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnCMUXStart1(void){//without AT
	if (theApp.GSDTApp.m_CMUXDriver.IsOpened() != 0){
		theApp.GSDTApp.m_CMUXDriver.CloseSelf(0);
		return;
	}

	if (theApp.GSDTApp.m_CMUXDriver.GetCDevBus()->EDA()->IsComOpened() == 0)
		return;

	theApp.GSDTApp.m_CMUXDriver.Open("1000\n 300\n T\n OK\n AT+CMUX=0\r",CMUXDriver::CMUX_blInitInThread);
	CMUXCFrmCreate();
	m_MCFrm->MDIActivate();
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnCMUXCLD(void){
	if (theApp.GSDTApp.m_CMUXDriver.IsOpened() == 0)
		return;
	theApp.GSDTApp.m_CMUXDriver.SendCLD();
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnCMUXPSC(void){
	if (theApp.GSDTApp.m_CMUXDriver.IsOpened() == 0)
		return;
	theApp.GSDTApp.m_CMUXDriver.SendStdPSC(0xff);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnCMUXPSC0(void){
	if (theApp.GSDTApp.m_CMUXDriver.IsOpened() == 0)
		return;
	theApp.GSDTApp.m_CMUXDriver.SendStdPSC(0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnCMUXPSC1(void){
	if (theApp.GSDTApp.m_CMUXDriver.IsOpened() == 0)
		return;
	theApp.GSDTApp.m_CMUXDriver.SendStdPSC(1);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnCMUXPSC7(void){
	if (theApp.GSDTApp.m_CMUXDriver.IsOpened() == 0)
		return;
	theApp.GSDTApp.m_CMUXDriver.SendStdPSC(7);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnCMUXPSC9(void){
	if (theApp.GSDTApp.m_CMUXDriver.IsOpened() == 0)
		return;
	theApp.GSDTApp.m_CMUXDriver.SendStdPSC(9);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnCMUXFC(void){
	if (theApp.GSDTApp.m_CMUXDriver.IsOpened() == 0)
		return;
	m_blCMUXFC = !m_blCMUXFC;
	theApp.GSDTApp.m_CMUXDriver.SendFCC(m_blCMUXFC);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateCMUXFC(CCmdUI *pCmdUI){
	pCmdUI->SetCheck(m_blCMUXFC);
}
//------------------------------------------------------------------------------------------//
#endif