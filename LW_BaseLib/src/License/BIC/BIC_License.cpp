//
//  BIC_License.cpp
//  SDT
//
//  Created by Leif Wen on 06/03/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "BIC_License.h"
#include "MSG_Register.h"
#ifdef BIC_License_h
//------------------------------------------------------------------------------------------//
#include "SYS_File.h"
//------------------------------------------------------------------------------------------//
CMDID BIC_PATCH::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Add Patch Code");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"<filename>"	,"BIN file name");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_PATCH::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	STDSTR		strPrint;
	STDSTR		fileName;
	PATCHCODE	pc;
	bool32		err = G_FALSE;
	
	fileName = msg;
	fileName = Str_SplitSubItem(&fileName, ' ');

	if (CFS_CheckFile(fileName) > 0)
		err = pc.AddPatchCode(_EMPTY(&strPrint),fileName);
	PrintStrNL(env,COLOR(COL_Result,strPrint));
	
	if (err > 0){
		PrintSuccess(env);
	}
	else{
		PrintFail(env);
	}
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_APPROVE::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Approve registration file");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"<hours>"			,"720H = 1M,8760H = 1Y,867240H = 99Y");
	PrintHelpSubItem(env,"<filename>"		,"PreRegistration.key");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_APPROVE::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	STDSTR			fileName,strPar1,strPar2;
	STDSTR			strReg,strContent;
	LINENSE_SIGN	tLS;
	ARRAY			*array;
	
	strReg = "";
	SplitPar1(strPar1, strPar2, msg);
	fileName = strPar2;
	fileName = Str_SplitSubItem(&fileName, ' ');
	
	array = tLS.Create(atoi(strPar1.c_str()) * 60 * 60,IUD_FILE(fileName));
	
	if (array->IsEmpty() == G_FALSE){
		Str_ReadSubItemR(&fileName, "/");
		fileName += "/License.key";
		DS_IO_NODE::GetDSIOList().Save(nullptr, OUD_FILEWR(fileName), IUD(array));
		PrintStrNL(env,COLOR(COL_clBlue,IUD("Create")),COL_Result,fileName,COL_clBlue,"successful.");
		PrintSuccess(env);
	}
	else{
		PrintFail(env);
	}
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_LSERVER::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Setup License Server");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[port]"		,"Server port, default is 16385");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_LSERVER::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	STDSTR		strPar,strPar1,strPar2,strPrint,strCommand;
	uint8		chkey;
	DTIME		tNow;
	LicenseBServer	m_LServer(1024 * 8);
	
	strPar = Str_Trim(msg);
	if (strPar.length() == 0)
		strPar = "16385";
	if (m_LServer.Open(SetOpenSSL(OPEN_TCPS, "", atoi(strPar.c_str())))){
		//PrintEnable(env);
		PrintResult(env,"License server is started at Port",strPar);
		PrintResult(env,"Press Esc to quit and stop the server");
		
		SwitchToAux(env);
		do{
			if (m_LServer.CheckblRequest()){
				ClrInPressAnyKeyMode(env);
				PrintResult(env,m_LServer.RequestSocketInfo(&strPrint));
				PrintResult(env,COL_clBlue,"Input reject/r or approve/a [hours] to response registration");
				PrintResult(env,COL_clBlue,"[720H = 1M,8760H = 1Y,867240H = 99Y]");
				while(ReadCommand(env,&strCommand)){
					Str_TrimSelf(strCommand);
					Str_LowerCaseSelf(strCommand);
					if (strCommand.length() > 0){
						if ((strCommand == "reject") || (strCommand == "r")){
							tNow.Now();
							m_LServer.RejectRegistration();
							PrintResult(env,COL_clPurple,SYS_MakeTimeNow(),"\"Reject registration\"");
							break;
						}
						else {
							strPar2 = Str_Trim(strCommand);
							strPar1 = Str_Trim(Str_ReadSubItem(&strPar2," "));
							if ((strPar1 == "approve") || (strPar1 == "a")){
								tNow.Now();
								m_LServer.ApproveRegistration(atoi(strPar2.c_str()));
								PrintResult(env,COL_clPurple,SYS_MakeTimeNow(),"\"Send approve Licence\"");
								break;
							}
						}
					}
					SYS_SleepMS(10);
				}
			}
			SetInPressAnyKeyMode(env);
			chkey = ReadChar(env,0);
			if (chkey == 27){
				break;
			}
			else if (chkey == '\r'){
				PrintEnable(env);
			}
			SYS_SleepMS(10);
		}while(1);
		SwitchToMain(env);
		m_LServer.Close();
	}
	ClrInPressAnyKeyMode(env);
	PrintSuccess(env,"License server is stopped");
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_REGISTRATION::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Registration");
	if (B_ChkFLAG32(flag, CMD_blPrintSimple))
		return(cgCommandID);
	PrintHelpSubItem(env,"[IP]"		,"Server address");
	PrintHelpSubItem(env,"[PORT]"	,"Server port, default is 16385");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_REGISTRATION::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	STDSTR				strReg;
	STDSTR				strPar1, strPar2, strPrint;
	LicenseBSocket		lSocket(1024 * 8,nullptr);
	bool32				retUpdate;
	REG_SIGN			regSign;
	ARRAY				*array;
	
	if (msg.length() == 0){
		if (regSign.CreateREGFILE(_EMPTY(&strPrint)) > 0){
			PrintStrNL(env,COLOR(COL_Result,strPrint));
			PrintSuccess(env);
		}
		else{
			PrintFail(env, strPrint);
		}
	}
	else{
		SplitPar1(strPar1,strPar2,msg);
		
		if (strPar2.length() == 0)
			strPar2 = "16385";
		
		PrintResult(env,"In doing pls wait...");
		if (lSocket.Open(SetOpenSSL(OPEN_TCP, strPar1, atoi(strPar2.c_str())))){
			PrintResult(env,"Connect to server successful");
			array = regSign.Create(_EMPTY(&strPrint));
			if (array->Used() > 0){
				PrintResult(env,"Create registration data successful");
				PrintResult(env,"Send registration data to server");
				retUpdate = lSocket.cgMsg.Send_REQ_License(env,array,20);
				if (retUpdate > 0){
					PrintSuccess(env,"Update License.key successful");
				}
				else if (retUpdate == 0){
					PrintFail(env,"Registration rejected");
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
		lSocket.Close();
	}
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//











//------------------------------------------------------------------------------------------//
BIC_BIN::BIC_BIN(void) : BIC_BASE_S(){
	cgCommandID = BIC_ID_BIN;
	cgCommand = "bin";
	cgConsoleName = cgCommand;
	cgHelpName = "BIN manage";
	
#ifdef CommonDefH_MAC
	Add(cgSub_patch) < cgSub_approve < cgSub_lserver;
#endif
#ifdef CommonDefH_VC
	Add(cgSub_registraion);
#endif
};
//------------------------------------------------------------------------------------------//
#endif /* BIC_License_h */
