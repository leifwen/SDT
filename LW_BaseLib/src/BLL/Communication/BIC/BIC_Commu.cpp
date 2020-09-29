//
//  BIC_Commu.cpp
//  SDT
//
//  Created by Leif Wen on 24/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "BIC_Commu.h"
#ifdef BIC_Commu_h
//------------------------------------------------------------------------------------------//
#include "Commu_AEXE.h"
#include "Commu_NTPServer.h"
#include "Commu_RST.h"
//------------------------------------------------------------------------------------------//
void BIC_ENV_COMMU::InitQuantity(CMD_ENV* env){
	if (env != nullptr){
		env->InitQuantity(CMD_VID_NEXT);
	}
};
//------------------------------------------------------------------------------------------//
void BIC_ENV_COMMU::Init(CMD_ENV* env){
	BIC_ENV::Init(env);
};
//------------------------------------------------------------------------------------------//
void BIC_ENV_COMMU::DeInit(CMD_ENV* env){
	BIC_ENV::DeInit(env);
};
//------------------------------------------------------------------------------------------//
AEXEPOOL* BIC_ENV_COMMU::GetAExePool(CMD_ENV* env){
#ifdef Commu_AEXE_h
	return(CMD_ENV::GetVar(env, CMD_VID_AEXEPOOL, (AEXEPOOL*)nullptr));
#else
	return(nullptr);
#endif
};
//------------------------------------------------------------------------------------------//
NTPServer* BIC_ENV_COMMU::GetNTPS(CMD_ENV* env){
#ifdef Commu_NTPServer_h
	return(CMD_ENV::GetVar(env,CMD_VID_NTPS,(NTPServer*)nullptr));
#else
	return(nullptr);
#endif
};
//------------------------------------------------------------------------------------------//
RSTBServer* BIC_ENV_COMMU::GetRSTServer(CMD_ENV* env){
#ifdef Commu_NTPServer_h
	return(CMD_ENV::GetVar(env,CMD_VID_RSTSERVER,(RSTBServer*)nullptr));
#else
	return(nullptr);
#endif
};
//------------------------------------------------------------------------------------------//
RSTBSocket* BIC_ENV_COMMU::GetRSTClient(CMD_ENV* env){
#ifdef Commu_NTPServer_h
	return(CMD_ENV::GetVar(env,CMD_VID_RSTCLIENT,(RSTBSocket*)nullptr));
#else
	return(nullptr);
#endif
};
//------------------------------------------------------------------------------------------//
TerminalServer* BIC_ENV_COMMU::GetTS(CMD_ENV* env){
#ifdef Commu_Terminal_h
	return(CMD_ENV::GetVar(env,CMD_VID_TS,(TerminalServer*)nullptr));
#else
	return(nullptr);
#endif
};
//------------------------------------------------------------------------------------------//












//------------------------------------------------------------------------------------------//
int32 BIC_COMMU::OnlineMode(CMD_ENV* env,COMMU_DRV* cmmu,CMD_TAIL tail)const{
	uint8	chKey;
	STDSTR	sendData,recData,strTail;
	uint32	ret = 0;
#ifdef Commu_Base_h
	strTail = "";
	switch (tail) {
		case CMD_R:	strTail = '\r';break;
		case CMD_N:	strTail = '\n';break;
		case CMD_RN:strTail = "\r\n";break;
		default:
			break;
	}
	//define ctrl^A~Z is 1~26,HEX is 01~1A. Escape ctrl^~ to 128
	
	PrintALine(env,COLOR(COL_clCyan,IUD("In online mode:")));
	SetInOnlineMode(env);
	SwitchToAux(env);
	PrintEnable(env);
	sendData = "";
	
	while((IsExit(env) == G_FALSE) && (cmmu->IsOpened()) && (OnlineModeExit(env) == G_FALSE)){
		if (cmmu->Read(_EMPTY(&recData),-1) > 0)
			PrintStr(env, COLOR(COL_DB_RxText,recData));
		
		SYS_SleepMS(10);
		chKey = ReadChar(env,G_FALSE);
		if ((chKey >= 32) && (chKey <= 126)){
			sendData += chKey;
		}
		else if (chKey == '\n'){
			PrintEnable(env);
			PrintStr(env,"\n");
			sendData += strTail;
			cmmu->Send(sendData,0);
			sendData = "";
		}
		else if (chKey == 27){
			ret = 1;
			break;
		}
		else{
			if (chKey == 0x80)
				chKey = 27;
			if ((chKey > 0) && (chKey <= 31))
				cmmu->Send(IUD(chKey),0);
		}
	}
	PrintEnable(env);
	PrintStr(env,"\n");
	ClrInOnlineMode(env);
	SwitchToMain(env);
#endif
	return(ret);
};
//------------------------------------------------------------------------------------------//
bool32 BIC_COMMU::OnlineModeExit(CMD_ENV* env)const{
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* BIC_Commu_h */
