//
//  ColorRecord.h
//  SDT
//
//  Created by Leif Wen on 29/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "ADS_Protocol.h"
//------------------------------------------------------------------------------------------//
#ifndef _ColDataH
#define _ColDataH
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Unix
enum COLORENUM{
	COL_LIN_NONE = 0,
	COL_LIN_clDefault,
	
	COL_LIN_clBlack,
	COL_LIN_clRed,
	COL_LIN_clGreen,
	COL_LIN_clYellow,
	COL_LIN_clBlue,
	COL_LIN_clMagenta,
	COL_LIN_clCyan,
	COL_LIN_clGray,
	
	COL_LIN_clBrightBlack,
	COL_LIN_clBrightRed,
	COL_LIN_clBrightGreen,
	COL_LIN_clBrightYellow,
	COL_LIN_clBrightBlue,
	COL_LIN_clBrightMagenta,
	COL_LIN_clBrightCyan,
	COL_LIN_clBrightGray,
	COL_LIN_clPurple,
	
	COL_NONE		= COL_LIN_NONE,
	COL_clDefault	= COL_LIN_clDefault,
	
	COL_clBlack		= COL_LIN_clBlack,
	COL_clMaroon	= COL_LIN_clRed,
	COL_clDGreen	= COL_LIN_clGreen,
	COL_clDYellow	= COL_LIN_clYellow,
	COL_clDBlue		= COL_LIN_clBlue,
	COL_clDMagenta	= COL_LIN_clMagenta,
	COL_clDCyan		= COL_LIN_clCyan,
	COL_clDGray		= COL_LIN_clGray,
	
	COL_clGray		= COL_LIN_clBrightBlack,
	COL_clRed		= COL_LIN_clBrightRed,
	COL_clGreen		= COL_LIN_clBrightGreen,
	COL_clYellow	= COL_LIN_clBrightYellow,
	COL_clBlue		= COL_LIN_clBrightBlue,
	COL_clMagenta	= COL_LIN_clBrightMagenta,
	COL_clCyan		= COL_LIN_clBrightCyan,
	COL_clWhite		= COL_LIN_clBrightGray,
	
	COL_clPurple	= COL_LIN_clPurple,
	
	COL_FF = 0xff,
};
#endif
#ifdef CommonDefH_VC
enum COLORENUM{
	COL_NONE = 0,
	COL_clDefault,
	
	COL_clBlack,
	COL_clMaroon,
	COL_clDGreen,
	COL_clDYellow,
	COL_clDBlue,
	COL_clDMagenta,
	COL_clDCyan,
	COL_clDGray,
	
	COL_clGray,
	COL_clRed,
	COL_clGreen	,
	COL_clYellow,
	COL_clBlue,
	COL_clMagenta,
	COL_clCyan,
	COL_clWhite,
	
	COL_clPurple,
	
	COL_FF = 0xff,
};
#endif
//------------------------------------------------------------------------------------------//
#define	COL_DivLine				COL_clMaroon
#define	COL_Time				COL_clDCyan
#define	COL_NormalMessage		COL_clPurple
#define	COL_ImportantMessage	COL_clRed
#define	COL_WarningMessage1		COL_ImportantMessage
#define	COL_WarningMessage2		COL_clBlue

#define	ColData(x,y)			x << y
#define	COL_DivLine_Maroon		ColData(COL_DivLine			,DIVIDING_LINE)
#define	COL_DivStar_Default		ColData(COL_clDefault		,DEV_LINE_STAR)
#define	COL_Time_DCyan			ColData(COL_Time			,SYS_MakeTimeNow())
#define	COL_DivLineTime			ColData(COL_DivLine_Maroon	,COL_Time_DCyan)
//------------------------------------------------------------------------------------------//
struct _ColData{
	COLORENUM	col;
	UVIn		uvin;
};
template <typename UV>	static inline _ColData COLOR	(COLORENUM col,const UV&	_in);
						static inline _ColData COLOR	(COLORENUM col,const UVIn&	_in);
#endif
//------------------------------------------------------------------------------------------//
#ifdef ADS_Protocol_h
#ifndef ColorRecord_h
#define ColorRecord_h
#ifdef ColorRecord_h
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class	COLRECORD;
typedef COLRECORD	CRD;
class COLRECORD : public PNFB_SHELL{
	public:
		enum	{CRD_NL			= 0x8000
				,CRD_G1			= 0x4000//receive all G1&G2&G3 address
				,CRD_G2			= 0x2000//receive all G1&G3 address,and receive G2 address
				,CRD_G3			= 0x1000//receive G3 address
				,CRD_ADDRMASK	= 0x0FFF
				,CRD_GROUPMASK	= CRD_G1 | CRD_G2 | CRD_G3
				,CRD_GNLMASK	= CRD_NL | CRD_GROUPMASK
				,CRD_DEFGROUP	= CRD_G1 | CRD_G2
		};
	protected:
		enum	{RFLAG_C = 1, RFLAG_S = PNFB_SHELL::RFLAG_S + PNFB_SHELL::RFLAG_C};
		enum	{CRD_blAddData = RFLAG_CREATE(0)};
	public:
				 COLRECORD(uint32 size);
		virtual ~COLRECORD(void){;};
	protected:
		PNF_MASK		pn_CTRL;
		PNF_VAR			pn_NL;
		PNF_VAR			pn_Group;
		PNF_ADDR		pn_Adress;
		PNF_VAR			pn_COL;
		PNF_SC			pn_Text;

		uint32			cgDefCtrl;
		uint32			cgDefCol;
	private:
				void	InitPN				(void);
	public:
		static	inline	uint32	CheckNL		(uint32 ctrl);
		static	inline	uint32	CheckGroup	(uint32 ctrl,uint32 group);

	protected:
		inline	virtual	ioss	DoTransform	(IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
		inline	virtual	ioss	_Begin		(IOSTATUS* _ios);
		inline	virtual	ioss	_Endl		(void);
	private:
		inline			CRD&	SetS		(IOSTATUS* _ios,uint32 ctrl,COLORENUM col);
		inline			ioss	SetE		(void);
	public:
		inline			uint32	ReadCtrl	(void)const;
		inline			uint32	ReadCOL		(void)const;
		inline			ioss	Write		(IOSTATUS* _ios,uint32 ctrl,COLORENUM col,const UVIn& _in);
};
//------------------------------------------------------------------------------------------//
class	COLRECORD_CACHE;
typedef COLRECORD_CACHE		CRDC;
class COLRECORD_CACHE : public CRD{
	public:
				 COLRECORD_CACHE(uint32 size);
		virtual ~COLRECORD_CACHE(void){;};
	protected:
		uint32			cgGroupDisableStatus;
		uint32			cgInAddr;
		uint32			cgInEnforce;
		uint32			cgInNL;
		uint32			cgInClrGroup;
	private:
		inline			uint32	MakeCtrl			(uint32 ctrl,uint32 enforce);
	public:
		inline			void	DisableGroup		(uint32 group);
		inline			void	EnableGroup			(uint32 group);
		inline			bool32	CheckDisableGroup	(uint32 group);
	public:
		inline			void	Write	(IOSTATUS* _ios,COLORENUM col,const UVIn& _in,G_LOCK blLock = G_LOCK_ON,uint32 enforce = 0,uint32 addr = CRD_DEFGROUP);
		inline			void	WriteNL	(IOSTATUS* _ios,COLORENUM col,const UVIn& _in,G_LOCK blLock = G_LOCK_ON,uint32 enforce = 0,uint32 addr = CRD_DEFGROUP);
	public:
		inline	virtual	ioss	_Begin			(IOSTATUS* _ios);
		inline	virtual	ioss	_Endl			(void);
	public:
						void	NL				(void);
						void	SetCol			(COLORENUM col);
						void	SetAddr			(uint32 addr);
						void	SetEnforce		(uint32 group);
						void	ClrGroup		(uint32 group);
};
//------------------------------------------------------------------------------------------//
CreateOperatorSetUint32(Col);
CreateOperatorSetUint32(Addr);
CreateOperatorFun1(NL);
CreateOperatorFun2(NL);

CreateOperatorSetUint32(Group);
CreateOperatorClrUint32(Group);

CreateOperatorFun1(G1);
CreateOperatorFun1(G2);
CreateOperatorFun1(G3);
CreateOperatorFun2(G1);
CreateOperatorFun2(G2);
CreateOperatorFun2(G3);

CreateOperatorFun1(DefGroup);
CreateOperatorFun2(DefGroup);
//------------------------------------------------------------------------------------------//
#define TEMPLATE_FUN(_data,_fun)\
							inline			CRDN&	_fun		(void);\
template <typename T_DSTF>	inline	friend	T_DSTF& operator << (T_DSTF& _crdn,const _data& fun){\
	_crdn._fun();\
	return(_crdn);\
};
//------------------------------------------------------------------------------------------//
#define TEMPLATE_DATA(_data,_fun)\
							inline			CRDN&	_fun		(const _data& data);\
template <typename T_DSTF>	inline	friend	T_DSTF& operator << (T_DSTF& _crdn,const _data& data){\
	_crdn._fun(data);\
	return(_crdn);\
};
//------------------------------------------------------------------------------------------//
class	COLRECORD_NODE;
typedef COLRECORD_NODE	CRDN;
class COLRECORD_NODE : public DSTF{
	public:
				 COLRECORD_NODE(void);
		virtual ~COLRECORD_NODE(void){;};
	protected:
		CRDC			*cgCache;
		uint32			cgAddress;
		uint32			cgMask;
		uint32			cgExtraGroup;
	public:
		inline			void	SetAddress			(const CRDC* cache,uint32 addr,uint32 mask,uint32 exGroup);
		inline			uint32	GetAddress			(void)const;
		inline			uint32	GetGroup			(void)const;
		inline			void	SetExtraGroup		(uint32 group);
		inline			void	SetDevID			(uint32 devID);
		inline			uint32	GetDevID			(void);
	public:
		inline			void	PrintDisable		(void);
		inline			void	PrintEnable			(void);
		inline			int32	CheckPrintDisable	(void);
	protected:
		inline	virtual	ioss	DoTransform			(IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
		inline	virtual	ioss	DoFinal				(IOSTATUS* _ios,const UVOut& _out);
	public:
		inline	virtual	ioss	_Begin				(IOSTATUS* _ios);
		inline	virtual	ioss	_Endl				(void);
	public:
		inline			void	Write				(IOSTATUS* _ios,COLORENUM col,const UVIn& _in,G_LOCK blLock = G_LOCK_ON,uint32 extraGroup = 0)const;
		inline			void	WriteNL				(IOSTATUS* _ios,COLORENUM col,const UVIn& _in,G_LOCK blLock = G_LOCK_ON,uint32 extraGroup = 0)const;;
	public:
		inline			uint64	GetInLength			(const UVIn&	 _in);
		inline			uint64	GetInLength			(const _ColData& _in);
		inline			uint64	GetInLength			(const char*	 _in);
		inline			uint64	GetInLength			(const COLORENUM col);
	public:
		TEMPLATE_DATA	(_ColData,		SetColData);
		TEMPLATE_DATA	(COLORENUM,		SetColData);
		TEMPLATE_DATA	(_Group,		SetGroup);
		TEMPLATE_DATA	(_ClrGroup,		ClrGroup);
		TEMPLATE_FUN	(_NLF,			_NL);
		TEMPLATE_FUN	(_DefGroupF,	_DefGroup);
		TEMPLATE_FUN	(_G1F,			_G1);
		TEMPLATE_FUN	(_G2F,			_G2);
		TEMPLATE_FUN	(_G3F,			_G3);
};
//------------------------------------------------------------------------------------------//
#endif /* ColorRecord_h */
#endif /* ColorRecord_h */
#include "ColorRecord.hpp"
#endif
