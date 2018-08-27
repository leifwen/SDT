//------------------------------------------------------------------------------------------//
void CMainFrame::InitRibbonBar(void){
	InitComboBox_BR();
	InitComboBox_COM();
#ifdef	SWVERSION_AUXDEVICE
	InitComboBox_AuxBR();
	InitComboBox_AuxCOM();
#endif
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnButtonClickScoketlist(void){	
	if (m_wndSocket.IsVisible() == FALSE){
		m_wndSocket.ShowPane(TRUE,FALSE,TRUE);
		m_wndSocket.SetAutoHideMode(FALSE,CBRS_RIGHT);
	}
	else if (m_wndSocket.IsPaneVisible() == FALSE){
		m_wndSocket.ShowPane(TRUE,FALSE,TRUE);
	}
	else{
		m_wndSocket.ShowPane(FALSE,FALSE,TRUE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateScoketlist(CCmdUI *pCmdUI){
	pCmdUI->Enable(theApp.GSDTApp.m_DeviceM.cgEDA.IsServerOpened() != 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnCOMBO_COM(void){
	int	id;

	CMFCRibbonComboBox *COMBox;
	COMBox = (CMFCRibbonComboBox*)m_wndRibbonBar.FindByID(ID_RI_COMBO_COM);
	id = COMBox->FindItem(COMBox->GetEditText());
	if (id >= 0){
		id = COMBox->GetItemData(id);
		m_ValidIPComList.selectedNode = (IPCOMNAME*)TREE_NODE::FindInLChildRChainByDRNodeID(&m_ValidIPComList,id);
		SetSelectComboBox_COM();
	}
	else{
		m_ValidIPComList.selectedNode = NULL;
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnCOMBO_BR(void){	
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateCOMBO_COM(CCmdUI *pCmdUI){
	pCmdUI->Enable(theApp.GSDTApp.m_DeviceM.IsOpened() == 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateCOMBO_BR(CCmdUI *pCmdUI){
	pCmdUI->Enable(theApp.GSDTApp.m_DeviceM.IsOpened() == 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnDSR_FLOW(void){
	if (theApp.GSDTApp.m_DeviceM.cgEDA.IsComOpened()){
		m_blDSRFlow = !m_blDSRFlow;
		theApp.GSDTApp.m_DeviceM.ACom()->SetDSRFlow(m_blDSRFlow);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnCTS_FLOW(void){
	if (theApp.GSDTApp.m_DeviceM.cgEDA.IsComOpened()){
		m_blCTSFlow = !m_blCTSFlow;
		theApp.GSDTApp.m_DeviceM.ACom()->SetCTSFlow(m_blCTSFlow);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnDTRHigh(void){
	if (theApp.GSDTApp.m_DeviceM.cgEDA.IsComOpened())
		theApp.GSDTApp.m_DeviceM.ACom()->SetDTR(1);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnDTRLow(void){
	if (theApp.GSDTApp.m_DeviceM.cgEDA.IsComOpened())
		theApp.GSDTApp.m_DeviceM.ACom()->SetDTR(0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnRTSHigh(void){
	if (theApp.GSDTApp.m_DeviceM.cgEDA.IsComOpened())
		theApp.GSDTApp.m_DeviceM.ACom()->SetRTS(1);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnRTSLow(void){
	if (theApp.GSDTApp.m_DeviceM.cgEDA.IsComOpened())
		theApp.GSDTApp.m_DeviceM.ACom()->SetRTS(0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateDSR_FLOW(CCmdUI *pCmdUI){
	if (theApp.GSDTApp.m_DeviceM.cgEDA.IsComOpened()){
		pCmdUI->Enable();
		pCmdUI->SetCheck(m_blDSRFlow);
	}
	else{
		pCmdUI->SetCheck(FALSE);
		pCmdUI->Enable(FALSE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateCTS_FLOW(CCmdUI *pCmdUI){
	if (theApp.GSDTApp.m_DeviceM.cgEDA.IsComOpened()){
		pCmdUI->Enable();
		pCmdUI->SetCheck(m_blCTSFlow);
	}
	else{
		pCmdUI->SetCheck(FALSE);
		pCmdUI->Enable(FALSE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateDTRHigh(CCmdUI *pCmdUI){
	if (theApp.GSDTApp.m_DeviceM.cgEDA.IsComOpened()){
		pCmdUI->Enable();
		pCmdUI->SetCheck(theApp.GSDTApp.m_DeviceM.ACom()->GetDTRStatus() != 0);
	}
	else{
		pCmdUI->SetCheck(FALSE);
		pCmdUI->Enable(FALSE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateDTRLow(CCmdUI *pCmdUI){
	if (theApp.GSDTApp.m_DeviceM.cgEDA.IsComOpened()){
		pCmdUI->SetCheck(theApp.GSDTApp.m_DeviceM.ACom()->GetDTRStatus() == 0);
	}
	else{
		pCmdUI->SetCheck();
		pCmdUI->Enable(FALSE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateRTSHigh(CCmdUI *pCmdUI){
	if (theApp.GSDTApp.m_DeviceM.cgEDA.IsComOpened()){
		pCmdUI->Enable();
		pCmdUI->SetCheck(theApp.GSDTApp.m_DeviceM.ACom()->GetRTSStatus() != 0);
	}
	else{
		pCmdUI->SetCheck(FALSE);
		pCmdUI->Enable(FALSE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateRTSLow(CCmdUI *pCmdUI){
	if (theApp.GSDTApp.m_DeviceM.cgEDA.IsComOpened()){
		pCmdUI->Enable();
		pCmdUI->SetCheck(theApp.GSDTApp.m_DeviceM.ACom()->GetRTSStatus() == 0);
	}
	else{
		pCmdUI->SetCheck();
		pCmdUI->Enable(FALSE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnASCII(void){
	B_ClrFLAG64(*theApp.GSDTApp.m_DeviceM.cgEDA.envcfg,ODEV_FLAG_EnHEXViewMode);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnHEX(void){
	B_SetFLAG64(*theApp.GSDTApp.m_DeviceM.cgEDA.envcfg,ODEV_FLAG_EnHEXViewMode);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateASCII(CCmdUI *pCmdUI){
	pCmdUI->SetCheck(B_ChkFLAG64(*theApp.GSDTApp.m_DeviceM.cgEDA.envcfg, ODEV_FLAG_EnHEXViewMode) == 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateHEX(CCmdUI *pCmdUI){
	pCmdUI->SetCheck(B_ChkFLAG64(*theApp.GSDTApp.m_DeviceM.cgEDA.envcfg, ODEV_FLAG_EnHEXViewMode) != 0);
}

//------------------------------------------------------------------------------------------//
void CMainFrame::OnButtonClickEcho(void){
	if (theApp.GSDTApp.m_DeviceM.CheckEcho() == 0){
		theApp.GSDTApp.m_DeviceM.EnableEcho();
	}
	else{
		theApp.GSDTApp.m_DeviceM.DisableEcho();
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateEcho(CCmdUI *pCmdUI){
	pCmdUI->SetCheck(theApp.GSDTApp.m_DeviceM.CheckEcho() != 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnMSR(void){
	if (B_ChkFLAG64(*theApp.GSDTApp.m_DeviceM.cgEDA.envcfg, ODEV_FLAG_EnMSReport) == 0){
		B_SetFLAG64(*theApp.GSDTApp.m_DeviceM.cgEDA.envcfg, ODEV_FLAG_EnMSReport);
	}
	else{
		B_ClrFLAG64(*theApp.GSDTApp.m_DeviceM.cgEDA.envcfg, ODEV_FLAG_EnMSReport);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnOSPMSG(void){
	if (B_ChkFLAG64(*theApp.GSDTApp.m_DeviceM.cgEDA.envcfg, ODEV_FLAG_EnOSPMsgLine) == 0){
		B_SetFLAG64(*theApp.GSDTApp.m_DeviceM.cgEDA.envcfg, ODEV_FLAG_EnOSPMsgLine);
	}
	else{
		B_ClrFLAG64(*theApp.GSDTApp.m_DeviceM.cgEDA.envcfg, ODEV_FLAG_EnOSPMsgLine);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnRECMSG(void){
	if (B_ChkFLAG64(*theApp.GSDTApp.m_DeviceM.cgEDA.envcfg, ODEV_FLAG_EnRecMsg) == 0){
		B_SetFLAG64(*theApp.GSDTApp.m_DeviceM.cgEDA.envcfg, ODEV_FLAG_EnRecMsg);
	}
	else{
		B_ClrFLAG64(*theApp.GSDTApp.m_DeviceM.cgEDA.envcfg, ODEV_FLAG_EnRecMsg);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnDisplay(void){
	if (theApp.GSDTApp.m_Cache.GetG1_STDOUT()->CheckPrintDisable() == 0){
		theApp.GSDTApp.m_Cache.GetG1_STDOUT()->PrintDisable();
	}
	else{
		theApp.GSDTApp.m_Cache.GetG1_STDOUT()->PrintEnable();
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateDisplay(CCmdUI *pCmdUI){
	pCmdUI->SetCheck(theApp.GSDTApp.m_Cache.GetG1_STDOUT()->CheckPrintDisable() == 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateMSR(CCmdUI *pCmdUI){
	pCmdUI->SetCheck(B_ChkFLAG64(*theApp.GSDTApp.m_DeviceM.cgEDA.envcfg, ODEV_FLAG_EnMSReport) != 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateOSPMSG(CCmdUI *pCmdUI){
	pCmdUI->SetCheck(B_ChkFLAG64(*theApp.GSDTApp.m_DeviceM.cgEDA.envcfg, ODEV_FLAG_EnOSPMsgLine) != 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateRECMSG(CCmdUI *pCmdUI){
	pCmdUI->SetCheck(B_ChkFLAG64(*theApp.GSDTApp.m_DeviceM.cgEDA.envcfg, ODEV_FLAG_EnRecMsg) != 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnButtonClickScriptAT(void){
	if (theApp.GSDTApp.m_Script.CheckATResponse() == 0){
		theApp.GSDTApp.m_Script.SetblATResponse();
	}
	else{
		theApp.GSDTApp.m_Script.ClrblATResponse();
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnButtonClickScriptCE(void){
	if (theApp.GSDTApp.m_Script.CheckCommandExplain() == 0){
		theApp.GSDTApp.m_Script.SetblCommandExplain();
	}
	else{
		theApp.GSDTApp.m_Script.ClrblCommandExplain();
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnButtonClickScriptShowSBIC(void){
	if (theApp.GSDTApp.m_Script.CheckPrintSBICinfo() == 0){
		theApp.GSDTApp.m_Script.SetblPrintSBICinfo();
	}
	else{
		theApp.GSDTApp.m_Script.ClrblPrintSBICinfo();
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateScriptAT(CCmdUI *pCmdUI){
	pCmdUI->SetCheck(theApp.GSDTApp.m_Script.CheckATResponse() != 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateScriptCE(CCmdUI *pCmdUI){
	pCmdUI->SetCheck(theApp.GSDTApp.m_Script.CheckCommandExplain() != 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateScriptShowSBIC(CCmdUI *pCmdUI){
	pCmdUI->SetCheck(theApp.GSDTApp.m_Script.CheckPrintSBICinfo() != 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnButtonClickScriptStop(void){
	DoScriptStop();
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnButtonClickGCEdit(void){
	if (m_wndGCProperties.IsVisible() == FALSE){
		m_wndGCProperties.ShowPane(TRUE,FALSE,TRUE);
		m_wndGCProperties.SetAutoHideMode(FALSE,CBRS_RIGHT);
	}
	else if (m_wndGCProperties.IsPaneVisible() == FALSE){
		m_wndGCProperties.ShowPane(TRUE,FALSE,TRUE);
	}
	else{
		m_wndGCProperties.ShowPane(FALSE,FALSE,TRUE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnButtonClickGCPCOMMAND(void){
	if (m_wndGCCommand.IsVisible() == FALSE){
		m_wndGCCommand.ShowPane(TRUE,FALSE,TRUE);
		m_wndGCCommand.SetAutoHideMode(FALSE,CBRS_RIGHT);
	}
	else if (m_wndGCCommand.IsPaneVisible() == FALSE){
		m_wndGCCommand.ShowPane(TRUE,FALSE,TRUE);
	}
	else{
		m_wndGCCommand.ShowPane(FALSE,FALSE,TRUE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnButtonClickScriptSC(void){
	if (m_wndSC.IsVisible() == FALSE){
		m_wndSC.ShowPane(TRUE,FALSE,TRUE);
		m_wndSC.SetAutoHideMode(FALSE,CBRS_LEFT);
	}
	else if (m_wndSC.IsPaneVisible() == FALSE){
		m_wndSC.ShowPane(TRUE,FALSE,TRUE);
	}
	else{
		m_wndSC.ShowPane(FALSE,FALSE,TRUE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnButtonClickScriptGC(void){
	if (m_wndGC.IsVisible() == FALSE){
		m_wndGC.ShowPane(TRUE,FALSE,TRUE);
		m_wndGC.SetAutoHideMode(FALSE,CBRS_LEFT);
	}
	else if (m_wndGC.IsPaneVisible() == FALSE){
		m_wndGC.ShowPane(TRUE,FALSE,TRUE);
	}
	else{
		m_wndGC.ShowPane(FALSE,FALSE,TRUE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnButtonClickClean(void){
	theApp.GSDTApp.m_DeviceM.SEmpty();
	theApp.GSDTApp.m_DeviceA.SEmpty();
	m_MCFrm->m_stdout.Clean();
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnButtonClickDevList(void){
	m_DlgDevList.DoModal();
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnButtonClickConnect(void){
	if (theApp.GSDTApp.m_DeviceM.IsOpened() == FALSE){
		if (DoConnect() == 0){
			DoScriptStop();
			theApp.GSDTApp.m_DeviceM.Close();
			m_wndSocket.ShowPane(FALSE,FALSE,TRUE);
		}
		else{
			theApp.GSDTApp.ExecBIC("m");
			theApp.GSDTApp.ExecBIC("c");
		}
		return;
	}
	else{
		DoScriptStop();
		theApp.GSDTApp.m_DeviceM.Close();
		m_wndSocket.ShowPane(FALSE,FALSE,TRUE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateScrpit(CCmdUI *pCmdUI){
	if (theApp.GSDTApp.m_Script.IsStop() != m_blStop){
		m_blStop = theApp.GSDTApp.m_Script.IsStop();
		if (m_blStop != 0){
			m_blConnect = !theApp.GSDTApp.m_DeviceM.IsOpened();
		}
		else if (m_Pitl3Created){
			m_Pitl3->SetProgressState(GetSafeHwnd(),TBPF_INDETERMINATE);
		}
		m_wndStatusBar.SetPB(m_blStop == 0);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateConnect(CCmdUI *pCmdUI){
	CString		cTitle;
	CSocketListViewDP::GetSocketList().LoadData(theApp.GSDTApp.m_DeviceM.AServer());
	if (theApp.GSDTApp.m_GCList.CheckUpdate() != 0){
		CGCViewDP::GetGCTree().LoadData(&theApp.GSDTApp.m_GCList);
		theApp.GSDTApp.m_GCList.ClrblUpdate();
	}
	if (theApp.GSDTApp.m_SCList.CheckUpdate() != 0){
		m_wndSC.m_wndSCTree.LoadData(&theApp.GSDTApp.m_SCList);
		theApp.GSDTApp.m_SCList.ClrblUpdate();
	}
	pCmdUI->SetCheck(theApp.GSDTApp.m_DeviceM.IsOpened() != 0);

	if (theApp.GSDTApp.m_DeviceM.IsOpened() != m_blConnect){
		m_blConnect = theApp.GSDTApp.m_DeviceM.IsOpened();
		if (m_blConnect != 0){
			m_MyTitle = Str_ANSIToUnicode(theApp.GSDTApp.m_DeviceM.MakeFrmTitle()).c_str();
			if (m_MCFrm != NULL)
				m_MCFrm->SetWindowText(m_MyTitle);
			if (m_Pitl3Created){
				m_Pitl3->SetProgressValue(GetSafeHwnd(),100,100);
				m_Pitl3->SetProgressState(GetSafeHwnd(),TBPF_NORMAL);
			}
			m_MyTitle += _T(" - ");
		}
		else{
			m_MyTitle = "";
			if (m_MCFrm != NULL)
				m_MCFrm->SetWindowText(_T("Main View"));
			if (m_Pitl3Created){
				m_Pitl3->SetProgressValue(GetSafeHwnd(),100,100);
				m_Pitl3->SetProgressState(GetSafeHwnd(),TBPF_NOPROGRESS);
			}
		}
		cTitle = m_MyTitle + _T(SWVERSION);
		SetWindowText(cTitle);
	}
}
//------------------------------------------------------------------------------------------//
LRESULT CMainFrame::OnBICClean(WPARAM wParam, LPARAM lParam){
	switch (wParam){
		case 0:;
			theApp.GSDTApp.m_DeviceM.SEmpty();
			theApp.GSDTApp.m_DeviceA.SEmpty();
			m_MCFrm->m_stdout.Clean();
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnButtonClickRecodrDir(void){
	CString	strDir;
	
	strDir = ODEV_FILE::CreateLOGDIR().c_str();
	ShellExecute(NULL,_T("open"),strDir,NULL,NULL,SW_SHOWNORMAL);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnButtonClickNewRecord(void){
	m_MCFrm->m_stdout.PrintEnable();
	theApp.GSDTApp.m_Cache.CreateG2_FILE(ODEV_FILE::CreateNewLOGFileName());
	m_MCFrm->m_stdout.PrintMessageDot(&m_MCFrm->m_stdout, "Set new record file:\n", theApp.GSDTApp.m_Cache.GetG2_File()->GetFileName());
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnButtonClickSend(void){
	if (m_wndSend.IsVisible() == FALSE){
		m_wndSend.ShowPane(TRUE,FALSE,TRUE);
		m_wndSend.SetAutoHideMode(FALSE,CBRS_LEFT);
	}
	else if (m_wndSend.IsPaneVisible() == FALSE){
		m_wndSend.ShowPane(TRUE,FALSE,TRUE);
	}
	else{
		m_wndSend.ShowPane(FALSE,FALSE,TRUE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnButtonClickCacl(void){
	if (m_wndCacl.IsVisible() == FALSE){
		m_wndCacl.ShowPane(TRUE,FALSE,TRUE);
		m_wndCacl.SetAutoHideMode(FALSE,CBRS_LEFT);
	}
	else if (m_wndCacl.IsPaneVisible() == FALSE){
		m_wndCacl.ShowPane(TRUE,FALSE,TRUE);
	}
	else{
		m_wndCacl.ShowPane(FALSE,FALSE,TRUE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnAppAbout(void){
	AuxCFrmCreate();
	m_AuxCFrm->m_stdout.Clean();
	theApp.GSDTApp.m_Script.Help(&m_AuxCFrm->m_stdout);
	m_AuxCFrm->m_stdout.ToHome();
}
//------------------------------------------------------------------------------------------//
int32 CMainFrame::DoConnect(void){
	int32			baudrate;
	uint32			type;
	std::string		result;
	CString			sstrPortName,sstrBR;
	CMFCRibbonComboBox *COMBox,*BRBox;

	COMBox = (CMFCRibbonComboBox*)m_wndRibbonBar.FindByID(ID_RI_COMBO_COM);
	BRBox = (CMFCRibbonComboBox*)m_wndRibbonBar.FindByID(ID_RI_COMBO_BR);
	sstrPortName = COMBox->GetEditText();
	sstrBR = BRBox->GetEditText();

	type = CheckIPComInput(Str_UnicodeToANSI(sstrPortName.GetBuffer(0)),&result);
	baudrate = atoi(Str_UnicodeToANSI(sstrBR.GetBuffer(0)).c_str());

	return(theApp.GSDTApp.m_DeviceM.Open(SetOpenPar(type, result, baudrate, theApp.GSDTApp.m_DeviceM.CheckEcho())) != 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::DoScriptStop(void){
	theApp.GSDTApp.m_Script.Stop();
	theApp.GSDTApp.m_FileSend.Stop();
}
//------------------------------------------------------------------------------------------//
void CMainFrame::InitComboBox_COM(void){
	CreateComboBox_COMList();
	m_ValidIPComList.selectedNode = (IPCOMNAME*)TREE_NODE::GetTail(TREE_NODE::GetDown(&m_ValidIPComList));
	SetSelectComboBox_COM();
}
//------------------------------------------------------------------------------------------//
void CMainFrame::InitComboBox_BR(void){
	CMFCRibbonComboBox *BRBox;
	BRBox = (CMFCRibbonComboBox*)m_wndRibbonBar.FindByID(ID_RI_COMBO_BR);
	BRBox->AddItem(_T("1200"));
	BRBox->AddItem(_T("2400"));
	BRBox->AddItem(_T("4800"));
	BRBox->AddItem(_T("9600"));
	BRBox->AddItem(_T("14400"));
	BRBox->AddItem(_T("19200"));
	BRBox->AddItem(_T("38400"));
	BRBox->AddItem(_T("56000"));
	BRBox->AddItem(_T("57600"));
	BRBox->AddItem(_T("115200"));
	BRBox->AddItem(_T("128000"));
	BRBox->AddItem(_T("230400"));
	BRBox->AddItem(_T("256000"));
	BRBox->AddItem(_T("460800"));
	BRBox->AddItem(_T("912600"));
	BRBox->SelectItem(9);
	BRBox->SetDropDownHeight(8 * 25);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::SetSelectComboBox_COM(void){
	CMFCRibbonComboBox *COMBox,*BRBox;
	CString		sstrPortName,sstrShowName,sstrBR;
	IPCOMNAME	*listItem;

	if (m_ValidIPComList.selectedNode == NULL)
		return;

	COMBox = (CMFCRibbonComboBox*)m_wndRibbonBar.FindByID(ID_RI_COMBO_COM);
	BRBox = (CMFCRibbonComboBox*)m_wndRibbonBar.FindByID(ID_RI_COMBO_BR);

	listItem = m_ValidIPComList.selectedNode;
	
	if (listItem->typeID == PublicDevice_DEVID_APICOM)
		sstrPortName = listItem->strIPComName.c_str();
	if (listItem->typeID == PublicDevice_DEVID_TCPClient){
		sstrPortName = "TCP:";
		sstrPortName += listItem->strIPComName.c_str();
	}
	if (listItem->typeID == PublicDevice_DEVID_UDPClient){
		sstrPortName = "UDP:";
		sstrPortName += listItem->strIPComName.c_str();
	}
	if (listItem->typeID == PublicDevice_DEVID_TCPServer)
		sstrPortName = "TCP SERVER";
	if (listItem->typeID == PublicDevice_DEVID_UDPServer)
		sstrPortName = "UDP SERVER";
	sstrShowName = listItem->strShowName.c_str();
	COMBox->SetEditText(sstrPortName);
	sstrBR = Str_ToStr(listItem->portBaudrate).c_str();
	BRBox->SetEditText(sstrBR);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::CreateComboBox_COMList(void){
	CString		sstrPortName,sstrShowName,sstrBackup;
	int32		comBoxHigh;
	CMFCRibbonComboBox *COMBox;

	COMBox = (CMFCRibbonComboBox*)m_wndRibbonBar.FindByID(ID_RI_COMBO_COM);
	sstrBackup = COMBox->GetEditText();
	COMBox->RemoveAllItems();

	theApp.GSDTApp.m_IPComList.Refresh();
	theApp.GSDTApp.m_IPComList.Export(&m_ValidIPComList);

	m_ValidIPComList.InUse_set();

	TREE_LChildRChain_Traversal_LINE(IPCOMNAME, (&m_ValidIPComList),
		if (_opNode->typeID == PublicDevice_DEVID_APICOM)
			sstrPortName = _opNode->strIPComName.c_str();
		if (_opNode->typeID == PublicDevice_DEVID_TCPClient){
			sstrPortName = "TCP:";
			sstrPortName += _opNode->strIPComName.c_str();
		}
		if (_opNode->typeID == PublicDevice_DEVID_UDPClient){
			sstrPortName = "UDP:";
			sstrPortName += _opNode->strIPComName.c_str();
		}
		if (_opNode->typeID == PublicDevice_DEVID_TCPServer)
			sstrPortName = "TCP SERVER";
		if (_opNode->typeID == PublicDevice_DEVID_UDPServer)
			sstrPortName = "UDP SERVER";
		sstrShowName = _opNode->strShowName.c_str();
		COMBox->AddItem(sstrShowName, TREE_NODE::GetdRNodeID(_opNode));
		if (sstrBackup == sstrPortName)
			m_ValidIPComList.selectedNode = _opNode;
	);
	m_ValidIPComList.InUse_clr();
	if (m_ValidIPComList.selectedNode != NULL)
		COMBox->SelectItem((DWORD_PTR)TREE_NODE::GetdRNodeID(m_ValidIPComList.selectedNode));
	COMBox->SetEditText(sstrBackup);
	comBoxHigh = COMBox->GetCount() * 25 + 25;
	if (comBoxHigh > 15 * 25)
		comBoxHigh = 15 * 25;
	COMBox->SetDropDownHeight(comBoxHigh);
	COMBox->EnableDropDownListResize(TRUE);
}
//------------------------------------------------------------------------------------------//
