/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: BIC_MSDT.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
 
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "BIC_MSDT.h"
#include "Comm_File.h"
#include "Comm_Convert.h"
#include "Commu_RMS.h"
#include "AppLayer.h"
//------------------------------------------------------------------------------------------//
#ifdef BIC_MSDT_ENABLE
//------------------------------------------------------------------------------------------//
int32 BIC_PATCHSDT::Command(BIC_ENV *env,const STDSTR &par,void *p)const{
	STDSTR		strPrint;
	STDSTR		strPar;
	Reg_SDT_PatchCode	SDTPC;
	
	strPar = par;
	strPar = Str_SplitSubItem(&strPar, ' ');
	if (CFS_CheckFile(strPar) > 0)
		SDTPC.PatchToSDT(strPar,&strPrint);
	
	PrintStrNL(env,Data(COL_clPurple,&strPrint));
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_APPROVE::Command(BIC_ENV *env,const STDSTR &par,void *p)const{
	STDSTR		strPar,strPar1,strPar2;
	STDSTR		strReg,strContent;
	Linense_Signature	tLS;
	
	strReg = "";
	SplitPar1(strPar1, strPar2, par);
	strPar = strPar2;
	strPar = Str_SplitSubItem(&strPar, ' ');
	
	CFS_ReadFile(&strReg, strPar);
	
	if (tLS.Encode(&strContent, strReg, atoi(strPar1.c_str()) * 60 * 60) > 0){
		Str_ReadSubItemR(&strPar, "/");
		strPar += "/License.key";
		CFS_WriteFile(strPar, strContent);
		PrintSuccess(env,"Create", strPar, "successful");
	}
	else{
		PrintFail(env);
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//\
//------------------------------------------------------------------------------------------//
int32 BIC_LSERVER::Command(BIC_ENV *env,const STDSTR &par,void *p)const{
	STDSTR		strPar,strPar1,strPar2,strPrint,strCommand;
	uint8		chkey;
	TIME		tNow;
	LicenseSSLServer	m_LServer(G_MAXBUFFER_SIZE);
	
	strPar = Str_Trim(par);
	if (par.length() == 0)
		strPar = "16385";
	if (m_LServer.Open(atoi(strPar.c_str())) > 0){
		PrintResult(env,"License server is started at Port",strPar);
		PrintResult(env,"Press Esc to quit and stop the server.");
		
		env->blUseSecondLH = 1;
		do{
			if (m_LServer.CheckblRequest() > 0){
				env->blInPressKeyMode = 0;
				PrintResult(env,m_LServer.RequestSocketInfo(&strPrint));
				PrintResult(env,Data(COL_clBlue,"Input reject or approve [hours] to response registration"));
				PrintResult(env,Data(COL_clBlue,"[720H = 1M,8760H = 1Y,867240H = 99Y]"));
				while(BI_ReadCommand(env,&strCommand) > 0){
					Str_TrimSelf(strCommand);
					Str_LowerCaseSelf(strCommand);
					if (strCommand.length() > 0){
						if ((strCommand == "reject") || (strCommand == "r")){
							tNow.Now();
							m_LServer.RejectRegistration();
							PrintResult(env,Data(COL_clPurple,""),SYS_MakeTimeNow(),"\"Reject registration\"");
							break;
						}
						else {
							strPar2 = Str_Trim(strCommand);
							strPar1 = Str_Trim(Str_ReadSubItem(&strPar2," "));
							if ((strPar1 == "approve") || (strPar1 == "a")){
								tNow.Now();
								m_LServer.ApproveRegistration(atoi(strPar2.c_str()) * 60 * 60);
								PrintResult(env,Data(COL_clPurple,""),SYS_MakeTimeNow(),"\"Send approve Licence\"");
								break;
							}
						}
					}
					SYS_SleepMS(10);
				}
			}
			env->blInPressKeyMode = 1;
			chkey = BI_ReadChar(env,0);
			if (chkey == 27){
				break;
			}
			else if (chkey == '\r'){
				env->cstdout->PrintEnable();
			}
			SYS_SleepMS(10);
		}while(1);
		env->blUseSecondLH = 0;
		m_LServer.CloseD();
	}
	env->blInPressKeyMode = 0;
	PrintSuccess(env,"License server is stopped");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
#endif
#ifdef BIC_CREGH
//------------------------------------------------------------------------------------------//
int32 BIC_CREGREPORT::Command(BIC_ENV *env,const STDSTR &par,void *p)const{
	STDSTR				strReg;
	STDSTR				strPar1, strPar2, strPrint;
	LicenseSSLSocket	lSocket(G_MAXBUFFER_SIZE);
	int32				retUpdate;
	
	if (par.length() == 0){
		if (REG_CreateREGFILE(&strPrint) > 0){
			PrintSuccess(env, strPrint);
		}
		else{
			PrintFail(env, strPrint);
		}
	}
	else{
		SplitPar1(strPar1,strPar2,par);
		
		if (strPar2.length() == 0)
			strPar2 = "16385";
		
		PrintResult(env,"In doing pls wait...");
		if (lSocket.OpenD(strPar1, atoi(strPar2.c_str()), CSType_TCP, 0) != 0){
			PrintResult(env,"Connect to server successful");
			if (lSocket.CreateRegSignature(&strReg,&strPrint) > 0){
				PrintResult(env,"Create registration data successful");
				PrintResult(env,"Send registration data to server");
				retUpdate = lSocket.SendRequestLicense(strReg,20);
				if (retUpdate > 0){
					PrintSuccess(env,"Update License.key successful");
				}
				else if (retUpdate == 0){
					PrintFail(env,"Registration is rejected");
				}
				else{
					PrintFail(env,"Timeout for getting License.key");
				}
			}
			else{
				PrintFail(env, strPrint);
			}
		}
		else{
			PrintFail(env,"Connect server fail");
		}
	}
	lSocket.CloseD();
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
#endif

