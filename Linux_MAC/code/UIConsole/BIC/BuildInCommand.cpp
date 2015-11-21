/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: BuildInCommand.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2015.01.05
 
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "BuildInCommand.h"
#include "Comm_Convert.h"
#include "AppLayer.h"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BuildInCommand::Do(BICPAR *tBICPAR){
	std::string		strCommand,strRet;
	int32			ret;
	
	ClrblExit(tBICPAR);
	PrintTitle(tBICPAR,tBICPAR->sdtApp->m_Device.cgDevType,1);
	if (BI_ReadCommand(tBICPAR) == 0)
		return(BI_RETCODE_NO);
	
	ret = BI_RETCODE_NO;
	
	strCommand = Str_Trim(Str_ReadSubItem(&tBICPAR->retCommand,"\r"));
	tBICPAR->retCommand = "";
	if (strCommand.length() > 0){
		ret = ExecuteLC(tBICPAR,strCommand,"",&strRet);
		if (ret == BI_RETCODE_NO)
			ret = cgC_MAIN.Command(tBICPAR,strCommand,&strRet);
#ifdef CommonDefH_Unix
		if (ret == BI_RETCODE_NO)
			ret = cgC_EXE_BASH.Command(tBICPAR,strCommand,&strRet);
#endif
	}
	if (ret == BI_RETCODE_NO){
		if (tBICPAR->blDisplayAuto != 0)
			tBICPAR->oDevNode->Enable();
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//