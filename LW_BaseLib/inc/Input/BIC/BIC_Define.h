//
//  BIC_Define.h
//  SDT
//
//  Created by Leif Wen on 11/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "CMD.h"
#ifdef CMD_h
//------------------------------------------------------------------------------------------//
#ifndef BIC_Define_h
#define BIC_Define_h
#ifdef BIC_Define_h
//------------------------------------------------------------------------------------------//
enum{
	BIC_ID_EXIT 		= CMD_NODE::CMD_ID_EXIT,
	BIC_ID_NO			= CMD_NODE::CMD_ID_NO,
	BIC_ID_HELP			= CMD_NODE::CMD_ID_HELP,
	
	BIC_ID_CONSOLE		= CMD_NODE::CMD_ID_NEXT,
	BIC_ID_EXIT_HELP,
	BIC_ID_VERSION,
	BIC_ID_RETURN,
	BIC_ID_DISPLAY,
	BIC_ID_NEWRECORD,
	BIC_ID_SELFNAME,
	BIC_ID_DAEMON,
	
	BIC_ID_CALC,
	BIC_ID_CALC_HEX2DEC,
	BIC_ID_CALC_DEC2HEX,
	BIC_ID_CALC_MD5,
	BIC_ID_CALC_SHA1,
	BIC_ID_CALC_SHA224,
	BIC_ID_CALC_SHA256,
	BIC_ID_CALC_SHA384,
	BIC_ID_CALC_SHA512,
	BIC_ID_CALC_BASE64,

	BIC_ID_SC,
	BIC_ID_SC_EG,
	BIC_ID_SC_LOAD,
	BIC_ID_SC_SAVE,
	BIC_ID_SC_LS,
	BIC_ID_SC_SET,
	BIC_ID_SC_CLONE,
	BIC_ID_SC_DEL,
	BIC_ID_SC_MVUP,
	BIC_ID_SC_MVDN,
	BIC_ID_SC_COMMAND,
	
	BIC_ID_GC,
	BIC_ID_GC_EG,
	BIC_ID_GC_LOAD,
	BIC_ID_GC_SAVE,
	BIC_ID_GC_LS,
	BIC_ID_GC_SET,
	BIC_ID_GC_CLONE,
	BIC_ID_GC_DEL,
	BIC_ID_GC_MVUP,
	BIC_ID_GC_MVDN,
	BIC_ID_GC_NAME,
	
	BIC_ID_GC_GROUP,
	BIC_ID_GC_GROUP_LS,
	BIC_ID_GC_GROUP_SET,
	BIC_ID_GC_GROUP_CLONE,
	BIC_ID_GC_GROUP_DEL,
	BIC_ID_GC_GROUP_MVUP,
	BIC_ID_GC_GROUP_MVDN,
	BIC_ID_GC_GROUP_COMMAND,
	BIC_ID_GC_GROUP_CONTINUE,
	BIC_ID_GC_GROUP_RESEND,
	BIC_ID_GC_GROUP_CSTOP,
	BIC_ID_GC_GROUP_CATCH,
	
	BIC_ID_EXECUTE,
	BIC_ID_EXECUTE_BASH,
	BIC_ID_EXECUTE_SH,
	
	BIC_ID_NTPS,
	BIC_ID_NTP,
	
	BIC_ID_RST,
	BIC_ID_RST_ON,
	BIC_ID_RST_OFF,
	BIC_ID_RST_LINK,
	BIC_ID_RST_SFS,
	BIC_ID_RST_MAPPING,
	BIC_ID_RST_LOGIN,
	BIC_ID_RST_TS,
	BIC_ID_RST_APPROVE,

	BIC_ID_MAIN,
	BIC_ID_AUX,
	BIC_ID_SECOND,
	BIC_ID_CONNECT,
	BIC_ID_ONLINE,
	BIC_ID_DISCONNECT,
	BIC_ID_CR,
	BIC_ID_ECHO,
	BIC_ID_REC,
	BIC_ID_DEV,

	BIC_ID_ACOM,
	BIC_ID_COM,
	BIC_ID_COM_BR,
	BIC_ID_COM_MS,
	BIC_ID_COM_DTR,
	BIC_ID_COM_RTS,
	BIC_ID_COM_CTSFLOW,
	BIC_ID_COM_DSRFLOW,
	BIC_ID_COM_LSCOM,
	BIC_ID_COM_OPENV,

	BIC_ID_ASOCKET,
	BIC_ID_TCP,
	BIC_ID_UDP,
	BIC_ID_TCPS,
	BIC_ID_UDPS,
	BIC_ID_PORT,
	BIC_ID_SI,
	BIC_ID_SS,
	BIC_ID_SD,

	BIC_ID_CONN,

	BIC_ID_SC_SEND,
	BIC_ID_GC_GROUP_SEND,
	BIC_ID_SEND,
	BIC_ID_SENDFILE,
	BIC_ID_RECFILE,
	BIC_ID_RUN,
	BIC_ID_STOP,
	BIC_ID_SCRIPT,
	
	BIC_ID_DICT,
	BIC_ID_DICT_DO,
	BIC_ID_DICT_READ,
	BIC_ID_DICT_SAVE,
	BIC_ID_DICT_FIND,
	BIC_ID_DICT_UPDATE,
	BIC_ID_DICT_EMPTY,

	BIC_ID_CMUX,
	BIC_ID_CMUX_ON,
	BIC_ID_CMUX_OFF,
	BIC_ID_CMUX_CLD,
	BIC_ID_CMUX_PSC,
	BIC_ID_CMUX_FC,
	BIC_ID_CMUX_DTR,
	BIC_ID_CMUX_RTS,
	BIC_ID_CMUX_DLCI,
	BIC_ID_CMUX_LEN,

	BIC_ID_BIN,
	BIC_ID_PATCH,
	BIC_ID_APPROVE,
	BIC_ID_LSERVER,
	BIC_ID_REGISTRATION,
	
	BIC_ID_BLE,
	BIC_ID_BLE_SCAN,
	BIC_ID_BLE_LIST,
	BIC_ID_BLE_CONNECT,
	BIC_ID_BLE_DISCONNECT,
	BIC_ID_BLE_BIND,
	BIC_ID_BLE_UNBIND,
	BIC_ID_BLE_ATTACH,
	BIC_ID_BLE_WRITE,
	BIC_ID_BLE_READ,
	BIC_ID_BLE_INDICATE,
	
	BIC_ID_BT,
	BIC_ID_BT_SCAN,
	BIC_ID_BT_LIST,
	BIC_ID_BT_SPP,
};
//------------------------------------------------------------------------------------------//
#define COL_Result	COL_clYellow
#define	COL_Sucess	COL_NormalMessage
#define	COL_Fail	COL_ImportantMessage
class ARRAY;
class CONSOLE;
class SC_LIST;
class GC_LIST;
class ODEV_CACHE;
//------------------------------------------------------------------------------------------//
namespace BIC_ENV {
	enum	{CMD_VID_SELFNAME = CMD_ENV::CMD_VID_NEXT
			,CMD_VID_STDIN
			,CMD_VID_CACHE
			,CMD_VID_CONSOLE
			,CMD_VID_SCLIST
			,CMD_VID_GCLIST
			,CMD_VID_NEXT
	};
	static	inline	void		InitQuantity(CMD_ENV* env);
	static	inline  void		Init		(CMD_ENV* env);
	static	inline  void		DeInit		(CMD_ENV* env);

	CMD_VAR		(SelfName,	CMD_VID_SELFNAME,STDSTR);
	CMD_SETVAR	(SetSTDIN,	CMD_VID_STDIN,	ARRAY*);
	CMD_SETVAR	(SetCache,	CMD_VID_CACHE,	ODEV_CACHE*);
	CMD_SETVAR	(SetConsole,CMD_VID_CONSOLE,CONSOLE*);
	CMD_SETVAR	(SetSCList,	CMD_VID_SCLIST,	SC_LIST*);
	CMD_SETVAR	(SetGCList,	CMD_VID_GCLIST,	GC_LIST*);

	static	inline	ARRAY*		GetSTDIN	(CMD_ENV* env);
	static	inline	ODEV_CACHE*	GetCache	(CMD_ENV* env);
	static	inline	CONSOLE*	GetConsole	(CMD_ENV* env);
	static	inline	SC_LIST*	GetSCList	(CMD_ENV* env);
	static	inline	GC_LIST*	GetGCList	(CMD_ENV* env);
};
//------------------------------------------------------------------------------------------//
class BIC_BASE : public CMD_NODE{
	protected:
		enum	{RFLAG_C = 2, RFLAG_S = CMD_BASE::RFLAG_S + CMD_BASE::RFLAG_C};
		enum	{CMD_blPrintSimple		= RFLAG_CREATE(0)
				,CMD_blPrintSubCMD		= RFLAG_CREATE(1)};
	public:
				 BIC_BASE(void) : CMD_NODE(){;};
		virtual ~BIC_BASE(void){;};
	public:
		inline	virtual	CMDID	ResolveID				(STDSTR** retMsg,const STDSTR& rawIn)const;
				virtual	CMDID	Execute					(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* p)const;
	public:
		static			uint8	ReadChar				(CMD_ENV* env,bool32 blWait);
		static			bool32	ReadCommand				(CMD_ENV* env,STDSTR* retCMD);
	public:
		static			uint8	PrintPressAnyKey		(CMD_ENV* env);
						bool32	IntoPressKeyMode		(CMD_ENV* env)const;
				virtual	bool32	IsExitPressKeyMode		(CMD_ENV* env)const;
	public:
		static	inline	void	SetInPressAnyKeyMode	(CMD_ENV* env);
		static	inline	void	ClrInPressAnyKeyMode	(CMD_ENV* env);
		static	inline	void	SetInOnlineMode			(CMD_ENV* env);
		static	inline	void	ClrInOnlineMode			(CMD_ENV* env);
		static	inline	void	SetblDisplayAuto		(CMD_ENV* env);
		static	inline	void	ClrblDisplayAuto		(CMD_ENV* env);
		static	inline	bool32	IsDisplayAuto			(CMD_ENV* env);
		static	inline	void	SwitchToMain			(CMD_ENV* env);
		static	inline	void	SwitchToAux				(CMD_ENV* env);
	public:
		template<typename... Args>	static			void	PrintHelpItem		(CMD_ENV* env,const STDSTR& command,const Args&... args);
		template<typename... Args>	static			void	PrintHelpSubItem	(CMD_ENV* env,const STDSTR& command,const Args&... args);

		template<typename... Args>	static inline	void	PrintFail			(CMD_ENV* env,const Args&... args);
		template<typename... Args>	static inline	void	PrintSuccess		(CMD_ENV* env,const Args&... args);
		template<typename... Args>	static inline	void	PrintResult			(CMD_ENV* env,const Args&... args);
	
									static	inline	void	PrintFail			(CMD_ENV* env);
									static	inline	void	PrintSuccess		(CMD_ENV* env);
									static	inline	void	PrintResult			(CMD_ENV* env);
};
//------------------------------------------------------------------------------------------//
#define BIC_CLASSTYPE(_NAME,_CMD)		CMD_CLASSTYPE(BIC_##_NAME,BIC_BASE,BIC_ID_##_NAME,_CMD);
//------------------------------------------------------------------------------------------//
BIC_CLASSTYPE(VERSION,	"version");
BIC_CLASSTYPE(EXIT,		"exit");
BIC_CLASSTYPE(RETURN,	"r/return");
BIC_CLASSTYPE(DISPLAY,	"display");
BIC_CLASSTYPE(NEWRECORD,"nr/newrecord");
BIC_CLASSTYPE(SELFNAME,	"selfname");
#ifdef CommonDefH_Unix
BIC_CLASSTYPE(DAEMON,	"daemon");
#endif
//------------------------------------------------------------------------------------------//
class BIC_BASE_S : public BIC_BASE{
	public:
				 BIC_BASE_S(void);
		virtual ~BIC_BASE_S(void){;};
	protected:
		STDSTR		cgHelpName;
		STDSTR		cgConsoleName;
	public:
		BIC_EXIT	cgSub_EXIT;
		BIC_VERSION cgSub_VERSION;
		BIC_RETURN	cgSub_RETURN;
		BIC_DISPLAY cgSub_DISPLAY;
		BIC_BASE*	cgSub_Default;
	public:
				virtual		CMDID		Execute				(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* p)const;
				virtual		CMDID		Command				(CMD_ENV* env,const STDSTR& msg,void* p)const;
				virtual		CMDID		Help				(CMD_ENV* env,uint32 flag)const;
	public:
		inline	virtual		void		SetUpName			(const STDSTR& strName);
				virtual		BIC_BASE_S*	AppendDownNode		(TNF* tnfNode);
				virtual		void		PrintConsoleName	(CMD_ENV* env)const;
};
//------------------------------------------------------------------------------------------//
#define SETBICRESTULT(_result)	\
if (p != nullptr) \
	*(static_cast<bool32*>(p)) = _result;
//------------------------------------------------------------------------------------------//
#include "BIC_Define.hpp"
#endif /* BIC_Define_h */
#endif /* BIC_Define_h */
#endif /* CMD_h */
