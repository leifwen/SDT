/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: Terminal.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
 
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Terminal.h"
#include "AppLayer.h"
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Unix
#include <arpa/inet.h>
#endif
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void TerminalSocket::CloseDev(void){
	cgBICPAR.blExit = -1;
	GetcgSelfODdev()->RemoveSelf();
	return(APISocket::CloseDev());
}
//------------------------------------------------------------------------------------------//
int32 TerminalSocket::ExThreadFun(void){
	cgBICPAR.sdtApp = &GSDTApp;
	cgBICPAR.oDevNode = GetcgSelfODdev();
	cgBICPAR.charSBUF = &cgRxBuffer;
	cgBICPAR.validComList = &cgBICPAR.sdtApp->m_IPComList;
	cgBICPAR.retCommand = "";
	cgBICPAR.blExit = 0;
	cgBICPAR.blInOnlineMode = 0;
	cgBICPAR.blInPressKeyMode = 0;
	cgBICPAR.blUseSecondLH = 0;
	cgBICPAR.blDisplayAuto = 1;
	cgBICPAR.gDID = 0;
	
	cgBICPAR.sdtApp->m_oDevOutputListPool.AddNode(GetcgSelfODdev());
	
	while(exThread.IsTerminated() == 0){
		if (cgBICPAR.sdtApp->m_BIC_Terminal.Do(&cgBICPAR) == -1)
			break;
		GetcgSelfODdev()->Print();
		SYS_SleepMS(50);
	}

	ClrblConnected();
	return 0;
}
//------------------------------------------------------------------------------------------//
void TerminalSocket::ThreadsStart(void){
	txThread.ThreadRun();
	rxThread.ThreadRun();
	exThread.ThreadRun();
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//