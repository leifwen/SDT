//
//  BIC_EXE.cpp
//  SDT
//
//  Created by Leif Wen on 16/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "BIC_EXE.h"
#ifdef BIC_EXE_h
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_EXEC::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Execute console command");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIC_EXEC::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	AEXE* aexe;
	if (BIC_ENV_COMMU::GetAExePool(env) == nullptr){
		PrintFail(env,"no set AExePool in BIC_ENV_DEV");
	}
	else{
		if (msg.length() == 0)
			return(cgCommandID);
		
		aexe = ExecuteCMD(env,msg);
		
		if (aexe == nullptr){
			PrintFail(env,"fork() failed");
			return(cgCommandID);
		}
		
		if (OnlineMode(env,aexe,CMD_N) > 0)
			PrintSuccess(env,"Pressed double ESC key, stopped child process");
		BIC_ENV_COMMU::GetAExePool(env)->CloseChild(aexe);
	}
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
AEXE* BIC_EXEC::ExecuteCMD(CMD_ENV* env,const STDSTR& cmd)const{
	
	return(BIC_ENV_COMMU::GetAExePool(env)->Execute("BIC_EXEC", cmd));
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_EXE_BASH::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Execute console command with using bash(default)");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
AEXE* BIC_EXE_BASH::ExecuteCMD(CMD_ENV* env,const STDSTR& cmd)const{
	
	return(BIC_ENV_COMMU::GetAExePool(env)->BASH("BIC_EXE_BASH", cmd));
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIC_EXE_SH::Help(CMD_ENV* env,uint32 flag)const{
	PrintHelpItem(env,cgCommand,"Execute console command with using sh");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
AEXE* BIC_EXE_SH::ExecuteCMD(CMD_ENV* env,const STDSTR& cmd)const{
	
	return(BIC_ENV_COMMU::GetAExePool(env)->SH("BIC_EXE_SH", cmd));
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* BIC_EXE_h */
