#ifdef	SWVERSION_AUXDEVICE
//------------------------------------------------------------------------------------------//
void CMainFrame::OnRiComboAuxcom(void){
	int	id;
	CMFCRibbonComboBox *COMBox;
	COMBox = (CMFCRibbonComboBox*)m_wndRibbonBar.FindByID(ID_RI_COMBO_AUXCOM);
	id = COMBox->FindItem(COMBox->GetEditText());
	if (id >= 0){
		id = COMBox->GetItemData(id);
		m_ValidAuxComList.selectedNode = (IPCOMNAME*)TREE_NODE::FindInLChildRChainByDRNodeID(&m_ValidAuxComList,id);
		SetSelectComboBox_AuxCOM();
	}
	else{
		m_ValidAuxComList.selectedNode = NULL;
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnRiComboAuxbr(void){
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateRiComboAuxcom(CCmdUI *pCmdUI){
	pCmdUI->Enable(theApp.GSDTApp.m_Device2.IsOpened() == 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateRiComboAuxbr(CCmdUI *pCmdUI){
	pCmdUI->Enable(theApp.GSDTApp.m_Device2.IsOpened() == 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnRiCheckAuxescape(void){
	if (B_ChkFLAG64(*theApp.GSDTApp.m_Device2.cgEDA.envcfg, ODEV_FLAG_EnEscape) == 0){
		B_SetFLAG64(*theApp.GSDTApp.m_Device2.cgEDA.envcfg, ODEV_FLAG_EnEscape);
		B_ClrFLAG64(*theApp.GSDTApp.m_Device2.cgEDA.envcfg, ODEV_FLAG_EnHEXViewMode);
	}
	else{
		B_ClrFLAG64(*theApp.GSDTApp.m_Device2.cgEDA.envcfg, ODEV_FLAG_EnEscape);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnRiCheckAuxhex(void){
	B_ClrFLAG64(*theApp.GSDTApp.m_Device2.cgEDA.envcfg, ODEV_FLAG_EnEscape);
	B_SetFLAG64(*theApp.GSDTApp.m_Device2.cgEDA.envcfg, ODEV_FLAG_EnHEXViewMode);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnRiCheckAuxascii(void){
	B_ClrFLAG64(*theApp.GSDTApp.m_Device2.cgEDA.envcfg, ODEV_FLAG_EnHEXViewMode);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnRiButtonAuxopen(void){
	if (theApp.GSDTApp.m_Device2.IsOpened() == 0){
		if (DoAuxopen())
			return;
	}
	theApp.GSDTApp.m_Device2.Close();
}
//------------------------------------------------------------------------------------------//
BOOL CMainFrame::DoAuxopen(void){
	uint32			type;
	int32			baudrate;
	std::string		result;
	CString			sstrPortName,sstrBR;
	CMFCRibbonComboBox *COMBox,*BRBox;

	COMBox = (CMFCRibbonComboBox*)m_wndRibbonBar.FindByID(ID_RI_COMBO_AUXCOM);
	BRBox = (CMFCRibbonComboBox*)m_wndRibbonBar.FindByID(ID_RI_COMBO_AUXBR);
	sstrPortName = COMBox->GetEditText();
	sstrBR = BRBox->GetEditText();

	type = CheckIPComInput(Str_UnicodeToANSI(sstrPortName.GetBuffer(0)),&result);
	baudrate = atoi(Str_UnicodeToANSI(sstrBR.GetBuffer(0)).c_str());

#ifdef	SWVERSION_AUXDEVICE
	return(theApp.GSDTApp.m_Device2.Open(SetOpenPar(type,result, baudrate, theApp.GSDTApp.m_Device2.CheckEcho())) != 0);
#endif
	return(FALSE);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnRiCheckAuxdsr(void){
	if ((theApp.GSDTApp.m_Device2.cgEDA.IsComOpened() != 0)){
		m_blAuxDSRFlow = !m_blAuxDSRFlow;
		theApp.GSDTApp.m_Device2.ACom()->SetDSRFlow(m_blAuxDSRFlow);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnRiCheckAuxdtrh(void){
	if ((theApp.GSDTApp.m_Device2.cgEDA.IsComOpened() != 0))
		theApp.GSDTApp.m_Device2.ACom()->SetDTR(1);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnRiCheckAuxdtrl(void){
	if ((theApp.GSDTApp.m_Device2.cgEDA.IsComOpened() != 0))
		theApp.GSDTApp.m_Device2.ACom()->SetDTR(0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnRiCheckAuxcts(void){
	if ((theApp.GSDTApp.m_Device2.cgEDA.IsComOpened() != 0)){
		m_blAuxCTSFlow = !m_blAuxCTSFlow;
		theApp.GSDTApp.m_Device2.ACom()->SetCTSFlow(m_blAuxCTSFlow);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnRiCheckAuxrtsh(void){
	if ((theApp.GSDTApp.m_Device2.cgEDA.IsComOpened() != 0))
		theApp.GSDTApp.m_Device2.ACom()->SetRTS(1);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnRiCheckAuxrtsl(void){
	if ((theApp.GSDTApp.m_Device2.cgEDA.IsComOpened() != 0))
		theApp.GSDTApp.m_Device2.ACom()->SetRTS(0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateRiCheckAuxescape(CCmdUI *pCmdUI){
	pCmdUI->SetCheck(B_ChkFLAG64(*theApp.GSDTApp.m_Device2.cgEDA.envcfg, ODEV_FLAG_EnEscape) != 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateRiCheckAuxhex(CCmdUI *pCmdUI){
	pCmdUI->SetCheck(B_ChkFLAG64(*theApp.GSDTApp.m_Device2.cgEDA.envcfg, ODEV_FLAG_EnHEXViewMode) != 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateRiCheckAuxascii(CCmdUI *pCmdUI){
	pCmdUI->SetCheck(B_ChkFLAG64(*theApp.GSDTApp.m_Device2.cgEDA.envcfg, ODEV_FLAG_EnHEXViewMode) == 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateRiButtonAuxopen(CCmdUI *pCmdUI){
	pCmdUI->SetCheck(theApp.GSDTApp.m_Device2.IsOpened() != 0);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateRiCheckAuxdsr(CCmdUI *pCmdUI){
	if (theApp.GSDTApp.m_Device2.cgEDA.IsComOpened()){
		pCmdUI->Enable();
		pCmdUI->SetCheck(m_blAuxDSRFlow);
	}
	else{
		pCmdUI->SetCheck(FALSE);
		pCmdUI->Enable(FALSE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateRiCheckAuxdtrh(CCmdUI *pCmdUI){
	if (theApp.GSDTApp.m_Device2.cgEDA.IsComOpened()){
		pCmdUI->Enable();
		pCmdUI->SetCheck(theApp.GSDTApp.m_Device2.ACom()->GetDTRStatus() != 0);
	}
	else{
		pCmdUI->SetCheck(FALSE);
		pCmdUI->Enable(FALSE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateRiCheckAuxdtrl(CCmdUI *pCmdUI){
	if (theApp.GSDTApp.m_Device2.cgEDA.IsComOpened()){
		pCmdUI->Enable();
		pCmdUI->SetCheck(theApp.GSDTApp.m_Device2.ACom()->GetDTRStatus() == 0);
	}
	else{
		pCmdUI->SetCheck(TRUE);
		pCmdUI->Enable(FALSE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateRiCheckAuxcts(CCmdUI *pCmdUI){
	if (theApp.GSDTApp.m_Device2.cgEDA.IsComOpened()){
		pCmdUI->Enable();
		pCmdUI->SetCheck(m_blAuxCTSFlow);
	}
	else{
		pCmdUI->SetCheck(FALSE);
		pCmdUI->Enable(FALSE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateRiCheckAuxrtsh(CCmdUI *pCmdUI){
	if (theApp.GSDTApp.m_Device2.cgEDA.IsComOpened()){
		pCmdUI->Enable();
		pCmdUI->SetCheck(theApp.GSDTApp.m_Device2.ACom()->GetRTSStatus() != 0);
	}
	else{
		pCmdUI->SetCheck(FALSE);
		pCmdUI->Enable(FALSE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::OnUpdateRiCheckAuxrtsl(CCmdUI *pCmdUI){
	if (theApp.GSDTApp.m_Device2.cgEDA.IsComOpened()){
		pCmdUI->Enable();
		pCmdUI->SetCheck(theApp.GSDTApp.m_Device2.ACom()->GetRTSStatus() == 0);
	}
	else{
		pCmdUI->SetCheck(TRUE);
		pCmdUI->Enable(FALSE);
	}
}
//------------------------------------------------------------------------------------------//
void CMainFrame::InitComboBox_AuxCOM(void){
	CreateComboBox_AuxCOMList();
	m_ValidAuxComList.selectedNode = (IPCOMNAME*)TREE_NODE::GetTail(TREE_NODE::GetDown(&m_ValidAuxComList));
	SetSelectComboBox_AuxCOM();
}
//------------------------------------------------------------------------------------------//
void CMainFrame::InitComboBox_AuxBR(void){
	CMFCRibbonComboBox *BRBox;
	BRBox = (CMFCRibbonComboBox*)m_wndRibbonBar.FindByID(ID_RI_COMBO_AUXBR);
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
	BRBox->SetDropDownHeight(10 * 25);
}
//------------------------------------------------------------------------------------------//
void CMainFrame::SetSelectComboBox_AuxCOM(void){
	CMFCRibbonComboBox *COMBox,*BRBox;
	CString		sstrPortName,sstrShowName,sstrBR;
	IPCOMNAME	*listItem;

	if (m_ValidAuxComList.selectedNode == NULL)
		return;

	COMBox = (CMFCRibbonComboBox*)m_wndRibbonBar.FindByID(ID_RI_COMBO_AUXCOM);
	BRBox = (CMFCRibbonComboBox*)m_wndRibbonBar.FindByID(ID_RI_COMBO_AUXBR);

	listItem = m_ValidAuxComList.selectedNode;
	
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
void CMainFrame::CreateComboBox_AuxCOMList(void){
	CString		sstrPortName,sstrShowName,sstrBackup;
	int32		comBoxHigh;
	CMFCRibbonComboBox *COMBox;

	COMBox = (CMFCRibbonComboBox*)m_wndRibbonBar.FindByID(ID_RI_COMBO_AUXCOM);
	sstrBackup = COMBox->GetEditText();
	COMBox->RemoveAllItems();

	theApp.GSDTApp.m_IPComList.Refresh();
	theApp.GSDTApp.m_IPComList.ExportCOM(&m_ValidAuxComList);

	m_ValidAuxComList.InUse_set();

	TREE_LChildRChain_Traversal_LINE(IPCOMNAME, (&m_ValidAuxComList),
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
			m_ValidAuxComList.selectedNode = _opNode;
	);
	m_ValidAuxComList.InUse_clr();
	if (m_ValidAuxComList.selectedNode != NULL)
		COMBox->SelectItem((DWORD_PTR)TREE_NODE::GetdRNodeID(m_ValidAuxComList.selectedNode));
	COMBox->SetEditText(sstrBackup);
	comBoxHigh = COMBox->GetCount() * 25;
	if (comBoxHigh > 15 * 25)
		comBoxHigh = 15 * 25;
	COMBox->SetDropDownHeight(comBoxHigh);
	COMBox->EnableDropDownListResize(TRUE);
}
//------------------------------------------------------------------------------------------//
#endif