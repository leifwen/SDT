//
//  MSG_Handshake.cpp
//  SDT
//
//  Created by Leif Wen on 14/03/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "MSG_ID.h"
#include "MSG_Handshake.h"
#include "Commu_DBuf.h"
#ifdef MSG_Handshake_h
//------------------------------------------------------------------------------------------//
//#define LOGPRINT_ENABLE
//#define LOGMSG_ENABLE

#ifdef LOGMSG_ENABLE
#define ELogMSG(x) ELog(x);
#else
#define ELogMSG(x);
#endif

#include "SYS_Log.h"
//------------------------------------------------------------------------------------------//
MSG_HANDSHAKE::MSG_HANDSHAKE(uint32 size,void* p) : MSG_NODE(size,p){
	cgMSGID = MESG_ID_Handshake;
	cgRSA_Prk = RSA_new();
	cgRSA_Puk = RSA_new();
	SetRSA_Prk();
	
	TNFP::SetSelfName("MSG_HANDSHAKE");
	cgMail.SetSelfName("Mail");
	SetFatherName("");
};
//------------------------------------------------------------------------------------------//
MSG_HANDSHAKE::~MSG_HANDSHAKE(void){
	FreeRSA_Prk();
	CRYPTO_cleanup_all_ex_data();
};
//------------------------------------------------------------------------------------------//
void MSG_HANDSHAKE::Reset(void* commu){
	cgMail.Init(1024,MAIL_SHA1::CFG_AES256 | MAIL_SHA1::CFG_AES_CFB8 | MAIL_SHA1::CFG_DEFAULT_COMPRESSION);
	ClrSFlag(HS_blHandshakeY | HS_blHandshakeN | HS_blREQClose);
	MessageProcessing(nullptr,MESG_INI_Handshake,"",commu);
	if (cgRSA_Puk != nullptr){
		RSA_free(cgRSA_Puk);
		cgRSA_Puk = RSA_new();
	}
};
//------------------------------------------------------------------------------------------//
void MSG_HANDSHAKE::SetSelfName(const STDSTR& strName){
	selfName = strName;
	cgMail.SetFatherName(GetFullName(this));
}
//------------------------------------------------------------------------------------------//
void MSG_HANDSHAKE::SetFatherName(const STDSTR& strName){
	fatherName = strName;
	cgMail.SetFatherName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void MSG_HANDSHAKE::FreeRSA_Prk(void){
	try{
		if (cgRSA_Prk != nullptr){
			RSA_free(cgRSA_Prk);
			cgRSA_Prk = nullptr;
		}
		if (cgRSA_Puk != nullptr){
			RSA_free(cgRSA_Puk);
			cgRSA_Puk = nullptr;
		}
	}
	catch(...){;};
};
//------------------------------------------------------------------------------------------//
bool32 MSG_HANDSHAKE::SetRSA_Prk(void){
	
	return(ALG_RSA_CreateKey(cgRSA_Prk,2048));
};
//------------------------------------------------------------------------------------------//
bool32 MSG_HANDSHAKE::Send_REQ_Handshake(void){
	bool32	blRet;
	
	MSGSend(MESG_REQ_Handshake,ALG_CreateRandKey(32),blRet,"Send_REQ_Handshake()::");
	
	return(blRet);
}
//------------------------------------------------------------------------------------------//
bool32 MSG_HANDSHAKE::HandshakeCheck(CMD_ENV* env){

	while(CheckSFlag(HS_blHandshakeY | HS_blHandshakeN) == G_FALSE){
		if (CMD_NODE::ChkblExit(env))
			break;
		SYS_SleepMS(2);
	}
	return(CheckSFlag(HS_blHandshakeY));
}
//------------------------------------------------------------------------------------------//
bool32 MSG_HANDSHAKE::IsHandshakeFail(void){
	
	return(CheckSFlag(HS_blHandshakeN));
};
//------------------------------------------------------------------------------------------//
bool32 MSG_HANDSHAKE::Send_REQ_Close(CMD_ENV* env){
	bool32	err;
	
	MSGSend2(env,MESG_REQ_Close,IUD("88"),HS_blREQClose,HS_blREQClose,1,err,"Send_REQ_Close()::");
	
	if ((err == G_TRUE) || (err == -3))
		return G_TRUE;
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
bool32 MSG_HANDSHAKE::IsREQClose(void){
	
	return(CheckSFlag(HS_blREQClose));
};
//------------------------------------------------------------------------------------------//
CMDID MSG_HANDSHAKE::MessageProcessing(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* commu){
	COMMU_THREAD*	_commu = static_cast<COMMU_THREAD*>(commu);
	static	SYS_TIME_S		Time_Handshake;
	STDSTR	strMesg,strContent;
	bool32	blRet = G_TRUE;
	CMDID	retCode = mID;
	
	if ((mID >= MESG_INI_Handshake) && (mID <= MESG_ANS_Close)){
		SYS_Delay_SetTS(&Time_Handshake,MSG_CENTER::HandshakeTime << 10);
		ELogMPRecMsg(mID);
	}
	switch (mID){
		case MESG_INI_Handshake:
			ClrSFlag(HS_blHandshakeY | HS_blHandshakeN | HS_blREQClose);
			break;
		case MESG_REQ_Handshake:
			ClrSFlag(HS_blHandshakeY | HS_blHandshakeN);
			strMesg = ALG_CreateRandKey(32);
			MPSend(MESG_REQ_RSAPuk,ALG_RSA_Encode_Puk(&strMesg,cgRSA_Prk),blRet);
			break;
		case MESG_REQ_RSAPuk:
			blRet = G_FALSE;
			if (ALG_RSA_Decode_Puk(&cgRSA_Puk,msg.substr(32))){
				strMesg = ALG_CreateRandKey(32);
				ALG_RSA_Encode_Puk(&strMesg,cgRSA_Prk);
				MPSend(MESG_ANS_RSAPuk,IUD(cgMail.Write(cgRSA_Puk,strMesg)),blRet);
			}
			else{
				ELogMP("Fail MESG_REQ_RSAPuk,Decode");
			}
			break;
		case MESG_ANS_RSAPuk:
			blRet = G_FALSE;
			if (cgMail.Decode(_EMPTY(&strContent), cgRSA_Prk, msg)){
				ALG_RSA_Decode_Puk(&cgRSA_Puk,strContent.substr(32));
				strMesg = ALG_CreateRandKey(32);
				strMesg += Str_DecToHex(ALG_AES::CFG_AES256 | ALG_AES::CFG_AES_CFB8);
				MPSend(MESG_REQ_UpdateAESKey,IUD(cgMail.Write(cgRSA_Puk,strMesg)),blRet);
			}
			else{
				ELogMP("Fail MESG_ANS_RSAPuk,Signature");
			}
			break;
		case MESG_REQ_UpdateAESKey:
			blRet = G_FALSE;
			if (cgMail.Decode(_EMPTY(&strContent), cgRSA_Prk, msg) > 0){
				MPSend(MESG_ANS_UpdateAESKey,IUD(cgMail.Write(cgRSA_Puk,strContent)),blRet);
				if (blRet > 0){
					strMesg = ALG_AESKey32Bye(strContent.substr(0,32));
					strContent.erase(32);
					ELogMP("Using the new Key");
					SetConfig((uint32)Str_HexToDec(strContent),strMesg);
				}
			}
			else{
				ELogMP("Fail MESG_REQ_UpdateAESKey,Signature");
			}
			break;
		case MESG_ANS_UpdateAESKey:
			blRet = G_FALSE;
			if (cgMail.Decode(_EMPTY(&strContent), cgRSA_Prk, msg) > 0){
				strMesg = ALG_AESKey32Bye(strContent.substr(0,32));
				strContent.erase(32);
				ELogMP("Using the new Key");
				SetConfig((uint32)Str_HexToDec(strContent),strMesg);
				SYS_DelayMS(50);
				MPSend(MESG_REQ_Test,ALG_CreateRandKey(64),blRet);
			}
			else{
				ELogMP("Fail MESG_ANS_UpdateAESKey,Signature");
			}
			break;
		case MESG_REQ_Test:
			MPSend(MESG_ANS_Test,ALG_CreateRandKey(64),blRet);
			if (blRet == G_FALSE){
				ELogMP("MESG_REQ_Test fail");
				break;
			}
		case MESG_ANS_Test:
			ELogMP("MESG_REQ_Test success");
			if (CheckSFlag(HS_blHandshakeY | HS_blHandshakeN) == 0){
				SetSFlag(HS_blHandshakeY);
				cgMsgCenter->SetblREQHS();
				ELogMP("Handshake success");
			}
			break;
		case MESG_REQ_Close:
			MPSend(MESG_ANS_Close,IUD("88"),blRet);
			ELogMP("Call CloseSelf()");
			SetSFlag(HS_blREQClose);
			cgMsgCenter->SetblREQClose();
			_commu->CloseSelf(1);
			break;
		case MESG_ANS_Close:
			_commu->PushSend(100);
			SetSFlag(HS_blREQClose);
			cgMsgCenter->SetblREQClose();
			break;
		default:
			retCode = MESG_NONE;
			break;
	}
	if ((mID >= MESG_INI_Handshake) && (mID <= MESG_ANS_Close) && (blRet == G_FALSE))
		SetSFlag(HS_blHandshakeN);
	
	if ((CheckSFlag(HS_blHandshakeN)) || ((CheckSFlag(HS_blHandshakeY) == G_FALSE) && (SYS_Delay_CheckTS(&Time_Handshake)))){
		SYS_Delay_SetTS(&Time_Handshake,MSG_CENTER::HandshakeTime << 10);
		SetSFlag(HS_blHandshakeN);
	}
	if (CheckSFlag(HS_blHandshakeN)){
		ELogMP("Handshake fail, Call CloseSelf()");
		SetSFlag(HS_blREQClose);
		cgMsgCenter->SetblREQClose();
		_commu->CloseSelf(1);
	};

	return(retCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* MSG_Handshake_h */
