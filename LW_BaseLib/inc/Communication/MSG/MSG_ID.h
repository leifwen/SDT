//
//  MSG_ID.h
//  SDT
//
//  Created by Leif Wen on 15/03/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#ifndef MSG_ID_h
#define MSG_ID_h
#include "BasicDefine.h"
//------------------------------------------------------------------------------------------//
enum MSGID{
	MESG_NONE = 0,
	
	MESG_ID_TEST,
	
	MESG_ID_Handshake,
	MESG_INI_Handshake,
	MESG_REQ_Handshake,
	MESG_REQ_RSAPuk,
	MESG_ANS_RSAPuk,
	MESG_REQ_UpdateAESKey,
	MESG_ANS_UpdateAESKey,
	MESG_REQ_Test,
	MESG_ANS_Test,
	MESG_REQ_Close,
	MESG_ANS_Close,
	
	MESG_ID_RMS,
	MESG_REQ_SetupMServer,
	MESG_ANS_SetupMServer,
	MESG_REQ_CloseMServer,
	MESG_ANS_CloseMServer,
	MESG_REQ_ListMServer,
	MESG_ANS_ListMServer,
	MESG_REQ_SetupDataChannel,
	MESG_ANS_SetupDataChannel,
	MESG_REQ_Link,
	MESG_ANS_Link,
	
	MESG_ID_TERMINAL,
	MESG_REQ_SetupTerminal,
	MESG_ANS_SetupTerminal,
	MESG_REQ_CloseTerminal,
	MESG_ANS_CloseTerminal,

	MESG_ID_REG,
	MESG_REQ_Registration,
	MESG_ANS_Registration,
	MESG_REQ_WriteLicense,
	MESG_ANS_WriteLicense,
	MESG_REQ_License,
	MESG_ANS_Approve,
	MESG_ANS_Reject,
	
	MESG_ID_FILE,
	MESG_REQ_FileCHSetup,
	MESG_ANS_FileCHSetup,
};
//------------------------------------------------------------------------------------------//
#define MSGSend(_mID,_in,_blRet,_Title1,_Title2) \
\
	ELogMSG(LogTitle(this,_Title1,_Title2,">>") << _GetMesgText(_mID));\
	_blRet = Send(_mID,_in);\
	ELogMSG(LogTitle(this,_Title1,_Title2,"::") << _GetMesgText(_mID) << ((_blRet > 0) ? "successful" : "fail"));
//------------------------------------------------------------------------------------------//
#define MSGSend2(_env,_mID,_in,_fY,_fN,_time,_blRet,_Title1,_Title2) \
\
	ELogMSG(LogTitle(this,_Title1,_Title2,">>") << _GetMesgText(_mID));\
	_blRet = Send(_env,_mID,_in,_fY,_fN,_time);\
	ELogMSG(LogTitle(this,_Title1,_Title2,"::") << _GetMesgText(_mID) << ((_blRet > 0) ? "successful" : "fail"));
//------------------------------------------------------------------------------------------//
#define MSGSend3(_mID,_in,_blRet,_Title1,_Title2) \
\
	ELogMSG(LogTitle(this,_Title1,_Title2,">>") << _GetMesgText(_mID) << "with MESG:" << _in);\
	_blRet = Send(_mID,IUD(_in));\
	ELogMSG(LogTitle(this,_Title1,_Title2,">>") << _GetMesgText(_mID) << ((_blRet > 0) ? "successful" : "fail"));
//------------------------------------------------------------------------------------------//
#define MPSend(_mID,_in,_blRet)						MSGSend(_mID,_in,_blRet,"MsgProcessing","Send")
#define MPSend2(_env,_mID,_in,_fY,_fN,_time,_blRet)	MSGSend2(_env,_mID,_in,_fY,_fN,_time,_blRet,"MsgProcessing","Send")
#define MPSend3(_mID,_in,_blRet)					MSGSend3(_mID,_in,_blRet,"MsgProcessing","Send")
#define ELogMP(_strMesg)							ELogMSG(LogTitle(this,"MsgProcessing","","::") << _strMesg);
#define ELogMPRecMsg(_mID) 							ELogMSG(LogTitle(this,"MsgProcessing","Rec","<<") << _GetMesgText(_mID));
#define ELogMPRecMsg2(_mID,_strMesg)				ELogMSG(LogTitle(this,"MsgProcessing","Rec","<<") << _GetMesgText(_mID) << ": " << _strMesg);
//------------------------------------------------------------------------------------------//
STDSTR	_GetMesgText(uint32 mID);
//------------------------------------------------------------------------------------------//
#endif /* MSG_ID_h */
