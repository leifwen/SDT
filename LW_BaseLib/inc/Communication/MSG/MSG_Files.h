//
//  MSG_Files.h
//  SDT
//
//  Created by Leif Wen on 2019/6/12.
//  Copyright © 2019 Leif Wen. All rights reserved.
//

#include "MSG_Center.h"
#ifdef MSG_Center_h
//------------------------------------------------------------------------------------------//
#ifndef MSG_Files_h
#define MSG_Files_h
#ifdef MSG_Files_h
//------------------------------------------------------------------------------------------//
typedef bool32(*MSG_ExternalFun)(CMD_ENV* env,const uint64& fileSize,const uint64& wrSize);
class MSG_Files : public MSG_NODE{
	protected:
		enum	{RFLAG_C = 6, RFLAG_S = MSG_NODE::RFLAG_S + MSG_NODE::RFLAG_C};
		enum	{REG_blFileCHSetupERR = RFLAG_CREATE(0)
				,REG_blFileCHSetupOK = RFLAG_CREATE(1)
				,REG_blFileCHCheckERR = RFLAG_CREATE(2)
				,REG_blFileCHCheckOK = RFLAG_CREATE(3)

				,REG_blFileSaveERR = RFLAG_CREATE(4)
				,REG_blFileSaveOK = RFLAG_CREATE(5)
		};
	public:
				 MSG_Files(void);
		virtual ~MSG_Files(void){;};
	private:
		STDSTR			cgFileName;
		uint32			cgFileOffset;
		CSSL_T1MSG*		cgT1FileCH;
		CMD_ENV*		cgENV;
		void*			cgTeam;
	private:
		SYS_Thread<MSG_Files>	t1FileRxThread;
		bool32					T1FileRxThreadFun	(void* _team);
	private:
		virtual	CMDID	MessageProcessing			(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* _team);
				CMDID	MessageProcessing2			(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* _team);
	public:
		virtual	void	SetSelfName					(const STDSTR& strName);
		virtual	void	SetUpName					(const STDSTR& strName);
		virtual	void	Reset						(void* commu);
		virtual	void	Close						(void* _team);
	private:
				bool32	SendInFileCH				(CMD_ENV* env,uint32 mID,const UVIn& _in,const uint64& flagY,const uint64& flagN,const uint32& waitTimeS);
				bool32	Send_REQ_FileCHSetup		(CMD_ENV* env,uint32 maxPackage,uint32 maxPages,uint32 maxDlyMS,uint32 maxTimes);
				bool32	Send_REQ_FileCheck			(CMD_ENV* env,void* fileStatus,const STDSTR& fn_src,const STDSTR& fn_des);
				bool32	ANS_FileCheck				(CMD_ENV* env,const STDSTR& recMsg);
	
				void	AppendFile					(CMD_ENV* env,const STDSTR& recMsg);
				void	OverWriteFile				(CMD_ENV* env,const STDSTR& recMsg);
	public:
				bool32	SendFile					(CMD_ENV* env,MSG_ExternalFun exFun,bool32 blOW,const STDSTR& fn_src,const STDSTR& fn_des);
};
//------------------------------------------------------------------------------------------//
#endif /* MSG_Files_h */
#endif /* MSG_Files_h */
#endif /* MSG_Center_h */
