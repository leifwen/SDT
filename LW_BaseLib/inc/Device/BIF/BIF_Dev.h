//
//  BIF_Dev.h
//  SDT
//
//  Created by Leif Wen on 19/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "BIF_Define.h"
#include "Device.h"
#if defined BIF_Define_h && defined Device_h
//------------------------------------------------------------------------------------------//
#ifndef BIF_Dev_h
#define BIF_Dev_h
#ifdef BIF_Dev_h
//------------------------------------------------------------------------------------------//
class AEXEPOOL;
class ExpandDeviceAttr;
class SCRIPT;
//------------------------------------------------------------------------------------------//
namespace BIF_ENV_DEV {
	enum	{CMD_VID_EDA = BIF_ENV::CMD_VID_NEXT
			,CMD_VID_AEXEPOOL
			,CMD_VID_SCRIPT
			,CMD_VID_NEXT
	};
	void				InitQuantity	(CMD_ENV* env);
	void				Init			(CMD_ENV* env);
	void				DeInit			(CMD_ENV* env);
	
	ExpandDeviceAttr*	GetEDA			(CMD_ENV* env);
	AEXEPOOL*			GetAExePool		(CMD_ENV* env);
	SCRIPT*				GetScript		(CMD_ENV* env);
	
	CMD_SETVAR(SetEDA,			CMD_VID_EDA,			ExpandDeviceAttr*);
	CMD_SETVAR(SetAExePool,		CMD_VID_AEXEPOOL,		AEXEPOOL*);
	CMD_SETVAR(SetScript,		CMD_VID_SCRIPT,			SCRIPT*);
};
//------------------------------------------------------------------------------------------//
BIF_CLASSTYPE(OPEN,				"Open,=");
BIF_CLASSTYPE(CLOSE,			"Close,=");
BIF_CLASSTYPE(BR,				"BR,=");
BIF_CLASSTYPE(DTR,				"DTR,=");
BIF_CLASSTYPE(RTS,				"RTS,=");
BIF_CLASSTYPE(CTS,				"CTS,==");
BIF_CLASSTYPE(DSR,				"DRS,==");
BIF_CLASSTYPE(RING,				"RING,==");
BIF_CLASSTYPE(DCD,				"DCD,==");
BIF_CLASSTYPE(SETRECVDATAMODE,	"SetRecvDataMode,=");
BIF_CLASSTYPE(SETRECMSGREPORT,	"SetRecMsgReport,=");
//------------------------------------------------------------------------------------------//
#endif /* BIF_Dev_h */
#endif /* BIF_Dev_h */
#endif /* BIF_Define_h */
