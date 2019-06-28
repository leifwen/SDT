//
//  MSG_Handshake.h
//  SDT
//
//  Created by Leif Wen on 14/03/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "MSG_Center.h"
#ifdef MSG_Center_h
//------------------------------------------------------------------------------------------//
#ifndef MSG_Handshake_h
#define MSG_Handshake_h
#ifdef MSG_Handshake_h
//------------------------------------------------------------------------------------------//
class MSG_HANDSHAKE : public MSG_NODE{
	protected:
		enum	{RFLAG_C = 3, RFLAG_S = MSG_NODE::RFLAG_S + MSG_NODE::RFLAG_C};
		enum	{HS_blHandshakeY = RFLAG_CREATE(0),HS_blHandshakeN = RFLAG_CREATE(1),HS_blREQClose = RFLAG_CREATE(2)};
	public:
				 MSG_HANDSHAKE(void);
		virtual	~MSG_HANDSHAKE(void);
	protected:
		RSA*			cgRSA_Prk;
		RSA*			cgRSA_Puk;
		MAIL_SHA1		cgMail;
	private:
		virtual	CMDID		MessageProcessing	(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* _team);
				void		FreeRSA_Prk			(void);
				int32		SetRSA_Prk			(void);
	public:
		virtual	void		SetSelfName			(const STDSTR& strName);
		virtual	void		SetUpName			(const STDSTR& strName);
		virtual	void		Reset				(void* _team);
	public:
				bool32		Send_REQ_Handshake	(void);
				bool32		HandshakeCheck		(CMD_ENV* env);
				bool32		IsHandshakeFail		(void);
				bool32		Send_REQ_Close		(CMD_ENV* env);
				bool32		IsREQClose			(void);
};
//------------------------------------------------------------------------------------------//
#endif /* MSG_Handshake_h */
#endif /* MSG_Handshake_h */
#endif /* MSG_Center_h */
