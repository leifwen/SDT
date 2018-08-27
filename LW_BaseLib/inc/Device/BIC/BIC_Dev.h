//
//  BIC_Dev.h
//  SDT
//
//  Created by Leif Wen on 13/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "BIC_Commu.h"
#include "Device.h"
#if defined BIC_Commu_h && defined Device_h
//------------------------------------------------------------------------------------------//
#ifndef BIC_Dev_h
#define BIC_Dev_h
#ifdef BIC_Dev_h
class ExpandDeviceAttr;
class IPCOMLIST;
class SCRIPT;
class TFileSend;
class CMUXDriver;
//------------------------------------------------------------------------------------------//
namespace BIC_ENV_DEV {
	enum	{CMD_VID_EDA = BIC_ENV_COMMU::CMD_VID_NEXT
			,CMD_VID_EDA_M
			,CMD_VID_EDA_A
			,CMD_VID_EDA_S
			,CMD_VID_VALIDCOMLIST
			,CMD_VID_SCRIPT
			,CMD_VID_FILESEND
			,CMD_VID_CMUX
			,CMD_VID_NEXT
	};
	void				InitQuantity	(CMD_ENV* env);
	void				Init			(CMD_ENV* env);
	void				DeInit			(CMD_ENV* env);
	
	ExpandDeviceAttr*	GetEDA			(CMD_ENV* env);
	ExpandDeviceAttr*	GetEDA_M		(CMD_ENV* env);
	ExpandDeviceAttr*	GetEDA_A		(CMD_ENV* env);
	ExpandDeviceAttr*	GetEDA_S		(CMD_ENV* env);
	IPCOMLIST*			GetValidComList	(CMD_ENV* env);
	SCRIPT*				GetScript		(CMD_ENV* env);
	TFileSend*			GetFileSend		(CMD_ENV* env);
	CMUXDriver*			GetCMUX			(CMD_ENV* env);
	
	CMD_SETVAR(SetEDA,			CMD_VID_EDA,			ExpandDeviceAttr*);
	CMD_SETVAR(SetEDA_M,		CMD_VID_EDA_M,			ExpandDeviceAttr*);
	CMD_SETVAR(SetEDA_A,		CMD_VID_EDA_A,			ExpandDeviceAttr*);
	CMD_SETVAR(SetEDA_S,		CMD_VID_EDA_S,			ExpandDeviceAttr*);
	CMD_SETVAR(SetValidComList,	CMD_VID_VALIDCOMLIST,	IPCOMLIST*);
	CMD_SETVAR(SetScript,		CMD_VID_SCRIPT,			SCRIPT*);
	CMD_SETVAR(SetFileSend,		CMD_VID_FILESEND,		TFileSend*);
	CMD_SETVAR(SetCMUX,			CMD_VID_CMUX,			CMUXDriver*);
};
//------------------------------------------------------------------------------------------//
class BIC_CONNECT : public BIC_BASE{
	public:
				 BIC_CONNECT(void) : BIC_BASE() {cgCommandID = BIC_ID_CONNECT;cgCommand = "c/connect";};
		virtual ~BIC_CONNECT(void){;};
	public:
		virtual	CMDID	Command				(CMD_ENV* env,const STDSTR& msg,void* p)const;
		virtual	CMDID	Help				(CMD_ENV* env,uint32 flag)const;
	public:
		static	bool32	SetConnectPar		(CMD_ENV* env,ExpandDeviceAttr* eda,const STDSTR &par,uint32 devType);
		static	bool32	SetConnectPar2		(CMD_ENV* env,ExpandDeviceAttr* eda,const STDSTR &par);
	public:
		static	uint8	PressAnyKey			(CMD_ENV* env,ExpandDeviceAttr* eda,SYS_TIME_S* dly);
		static	bool32	OnlineMode			(CMD_ENV* env,ExpandDeviceAttr* attr,CMD_TAIL tail);
	
		static	void	PrintTitle			(CMD_ENV* env,ExpandDeviceAttr* eda,bool32 blPrintTail);
};
//------------------------------------------------------------------------------------------//
#define BIC_CLASSTYPE_CONNECT(_NAME,_CMD)		CMD_CLASSTYPE(BIC_##_NAME,BIC_CONNECT,BIC_ID_##_NAME,_CMD);
//------------------------------------------------------------------------------------------//
BIC_CLASSTYPE			(MAIN,			"main/m");
BIC_CLASSTYPE			(AUX,			"aux/a");
BIC_CLASSTYPE			(SECOND,		"second/s");
BIC_CLASSTYPE_CONNECT	(ONLINE,		"o/online");
BIC_CLASSTYPE			(DISCONNECT,	"d/disconnect");
BIC_CLASSTYPE			(CR,			"cr");
BIC_CLASSTYPE			(ECHO,			"echo");
BIC_CLASSTYPE			(REC,			"rec");
BIC_CLASSTYPE			(DEV,			"dev");
//------------------------------------------------------------------------------------------//
#endif /* BIC_Dev_h */
#endif /* BIC_Dev_h */
#endif /* BIC_Define_h */
