//
//  BIC_Define.h
//  SDT
//
//  Created by Leif Wen on 29/03/2017.
//  Copyright © 2017 leif wen. All rights reserved.
//

#include "BasicDef.h"
#ifndef BIC_Define_hpp
#define BIC_Define_hpp
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
enum{
	BI_RETCODE_EXIT	= -1,
	BI_RETCODE_NO	= 0,
	BI_RETCODE_EXIT_HELP,
	BI_RETCODE_VERSION,
	BI_RETCODE_RETURN,
	BI_RETCODE_HELP,
	
	BI_RETCODE_EXECUTE,
	BI_RETCODE_EXECUTE_BASH,
	BI_RETCODE_EXECUTE_SH,

	BI_RETCODE_DISPLAY,
	BI_RETCODE_NEWRECORD,
	
	BI_RETCODE_MAIN,
	BI_RETCODE_AUX,
	
	BI_RETCODE_DEV,
	BI_RETCODE_ECHO,
	BI_RETCODE_REC,
	
	BI_RETCODE_ACOM,
	BI_RETCODE_COM,
	BI_RETCODE_COM_BR,
	BI_RETCODE_COM_MS,
	BI_RETCODE_COM_DTR,
	BI_RETCODE_COM_RTS,
	BI_RETCODE_COM_LS,
	BI_RETCODE_COM_OPEN,
	BI_RETCODE_COM_CLOSE,
	
	BI_RETCODE_ASOCKET,
	BI_RETCODE_TCP,
	BI_RETCODE_UDP,
	BI_RETCODE_TCPS,
	BI_RETCODE_UDPS,
	BI_RETCODE_PORT,
	
	BI_RETCODE_SI,
	BI_RETCODE_SS,
	BI_RETCODE_SSD,
	
	BI_RETCODE_ONLINE,
	BI_RETCODE_CONNECT,
	BI_RETCODE_DISCONNECT,
	BI_RETCODE_CR,
	
	BI_RETCODE_SC,
	BI_RETCODE_SC_EG,
	BI_RETCODE_SC_LOAD,
	BI_RETCODE_SC_SAVE,
	BI_RETCODE_SC_LS,
	BI_RETCODE_SC_SET,
	BI_RETCODE_SC_CLONE,
	BI_RETCODE_SC_DEL,
	BI_RETCODE_SC_MVUP,
	BI_RETCODE_SC_MVDN,
	BI_RETCODE_SC_COMMAND,
	BI_RETCODE_SC_SEND,
	
	BI_RETCODE_GC,
	BI_RETCODE_GC_EG,
	BI_RETCODE_GC_LOAD,
	BI_RETCODE_GC_SAVE,
	BI_RETCODE_GC_LS,
	BI_RETCODE_GC_SET,
	BI_RETCODE_GC_CLONE,
	BI_RETCODE_GC_DEL,
	BI_RETCODE_GC_MVUP,
	BI_RETCODE_GC_MVDN,
	BI_RETCODE_GC_GN,
	BI_RETCODE_GC_GROUP,
	BI_RETCODE_GC_GROUP_LS,
	BI_RETCODE_GC_GROUP_SET,
	BI_RETCODE_GC_GROUP_CLONE,
	BI_RETCODE_GC_GROUP_DEL,
	BI_RETCODE_GC_GROUP_MVUP,
	BI_RETCODE_GC_GROUP_MVDN,
	BI_RETCODE_GC_GROUP_COMMAND,
	BI_RETCODE_GC_GROUP_CONTINUE,
	BI_RETCODE_GC_GROUP_RESEND,
	BI_RETCODE_GC_GROUP_CSTOP,
	BI_RETCODE_GC_GROUP_CATCH,
	BI_RETCODE_GC_GROUP_SEND,
	
	BI_RETCODE_SCRIPT,
	BI_RETCODE_STOP,
	BI_RETCODE_RUN,
	BI_RETCODE_SEND,
	BI_RETCODE_SENDA,
	BI_RETCODE_SENDFILE,
	
	BI_RETCODE_CMUX,
	BI_RETCODE_CMUX_ON,
	BI_RETCODE_CMUX_OFF,
	BI_RETCODE_CMUX_CLD,
	BI_RETCODE_CMUX_PSC,
	BI_RETCODE_CMUX_FC,
	BI_RETCODE_CMUX_DTR,
	BI_RETCODE_CMUX_RTS,
	BI_RETCODE_DLCI,
	
	BI_RETCODE_MSDT,
	BI_RETCODE_PATCHSDT,
	BI_RETCODE_APPROVE,
	BI_RETCODE_LSERVER,
	BI_RETCODE_CREGREPORT,
	
	BI_RETCODE_NTP,
	BI_RETCODE_NTPSERVER,
	BI_RETCODE_TERMINALSERVER,
	
	BI_RETCODE_RST,
	BI_RETCODE_RST_ON,
	BI_RETCODE_RST_OFF,
	BI_RETCODE_RST_LINK,
	BI_RETCODE_RST_MAPPING,
	BI_RETCODE_RST_LOGIN,
	
	BI_RETCODE_CALC,
	BI_RETCODE_CALC_HEX2DEC,
	BI_RETCODE_CALC_DEC2HEX,
	BI_RETCODE_CALC_MD5,
	BI_RETCODE_CALC_SHA1,
	BI_RETCODE_CALC_SHA224,
	BI_RETCODE_CALC_SHA256,
	BI_RETCODE_CALC_SHA384,
	BI_RETCODE_CALC_SHA512,
	BI_RETCODE_CALC_BAS64,
	
	BI_RETCODE_DICT,
	BI_RETCODE_DICT_DO,
	BI_RETCODE_DICT_READ,
	BI_RETCODE_DICT_SAVE,
	BI_RETCODE_DICT_FIND,
	BI_RETCODE_DICT_UPDATE,
};
//------------------------------------------------------------------------------------------//
class SDTAPP;
class SBUFFER;
class OUTPUT_NODE;
class DEVICE;
class ExpandDeviceAttr;
class IPCOMLIST;
//------------------------------------------------------------------------------------------//
struct BIC_ENV{
	SDTAPP		*cSDTApp;
	SBUFFER		*cstdin;
	OUTPUT_NODE *cstdout;
	int32		blExit;
	//-----------------------//
	IPCOMLIST			*validComList;
	ExpandDeviceAttr	*eda;
	ExpandDeviceAttr	*eda2;
	//-----------------------//
	int32 blInOnlineMode;
	int32 blInPressKeyMode;
	int32 blUseSecondLH;
	int32 blDisplayAuto;
	
	uint32 gDID;
};
//------------------------------------------------------------------------------------------//
		void SetDefConsoleAttr(BIC_ENV *env,SDTAPP *cSDTApp,SBUFFER *cstdin,OUTPUT_NODE *cstdout = nullptr);
inline	void SetDefConsoleAttr(BIC_ENV *env){
	env->cSDTApp = nullptr;
	env->cstdin = nullptr;;
	env->cstdout = nullptr;
	env->blExit = 0;
	
	env->validComList = nullptr;
	env->eda = nullptr;
	env->eda2 = nullptr;
	
	env->blInOnlineMode = 0;
	env->blInPressKeyMode = 0;
	env->blUseSecondLH = 0;
	env->blDisplayAuto = 1;
	
	env->gDID = 0;
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* BIC_Define_hpp */
