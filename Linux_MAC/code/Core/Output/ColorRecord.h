/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: ColorRecord.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.7.5
*/

//------------------------------------------------------------------------------------------//
#include "Comm_PA.h"
//------------------------------------------------------------------------------------------//
#ifdef Comm_PAH
#ifndef ColorRecordH
#define ColorRecordH
#ifdef ColorRecordH
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Unix
enum{
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
};
enum{
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
};
#endif
#ifdef CommonDefH_VC
enum{
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
enum{
	COL_DivLine			= COL_clMaroon,
	COL_Time			= COL_clDCyan,
	COL_NormalMessage	= COL_clPurple,
	COL_ImportantMessage= COL_clRed,
	COL_WarningMessage1	= COL_ImportantMessage,
	COL_WarningMessage2	= COL_clBlue,
};
#define	ColData(x,y)				x << y
#define	COL_DivLine_Maroon			ColData(COL_DivLine			,DIVIDING_LINE)
#define	COL_DivStar_Default			ColData(COL_clDefault		,DEV_LINE_START)
#define	COL_Time_DCyan				ColData(COL_Time			,SYS_MakeTimeNow())
#define	COL_DivLineTime				ColData(COL_DivLine_Maroon	,COL_Time_DCyan)
#define	COL_MessageNormal(_str)		ColData(COL_NormalMessage	,_str)
#define	COL_MessageImportant(_str)	ColData(COL_ImportantMessage,_str)
#define	COL_MessageWarning1(_str)	ColData(COL_WarningMessage1	,_str)
#define	COL_MessageWarning2(_str)	ColData(COL_WarningMessage2	,_str)

#define	SpaceStr(_str)				((_str.length() > 0)?" ":"") << _str
#define	SpaceColStr(_col,_str)		ColData(_col,SpaceStr(_str))

#define	ColStr(_colstr)				_colstr.col << ((_colstr.p->length() > 0)?" ":"") << *_colstr.p
//------------------------------------------------------------------------------------------//
namespace {
	template <typename TYPE_P>struct		_ColData{uint32 col;TYPE_P p;};
	template <typename TYPE_P>static inline _ColData<TYPE_P> Data	(uint32 col,TYPE_P p){return{col,p};};
};
class COLSTR{
	public:
		virtual ~COLSTR(void){;};
	public:
		uint32	col;
		STDSTR	str;
		STDSTR	*p;
	public:
		COLSTR(void)								{col = -1;p = &str;str = "";};
		COLSTR(const char *data)					{col = -1;p = &str;str = data;};
		COLSTR(const STDSTR& C)						{col = -1;p = (STDSTR*)&C;str = "";};
		COLSTR(const _ColData<const char*> &data)	{col = data.col;p = &str;str = data.p;};
		COLSTR(const _ColData<STDSTR*> &data)		{col = data.col;p = data.p;str = "";};
		COLSTR(const _ColData<const STDSTR*> &data)	{col = data.col;p = (STDSTR*)data.p;str = "";};
};
//------------------------------------------------------------------------------------------//
CreateOperatorSet(Addr)
CreateOperatorSet(Group)
CreateOperatorClr(Group);
CreateOperatorSet(Enforce)
CreateOperatorSet(Col)
CreateOperatorFun(NL)
CreateOperatorFun(G1)
CreateOperatorFun(G2)
CreateOperatorFun(G3)
//------------------------------------------------------------------------------------------//
class COLRECORD : public PROTOCOL_NODE{
	public:
		enum {CRD_NL = 0x8000,CRD_G1 = 0x4000,CRD_G2 = 0x2000,CRD_G3 = 0x1000,CRD_ADDRMASK = 0x0FFF
			,CRD_GROUPMASK = CRD_G1 | CRD_G2 | CRD_G3
			,CRD_GNLMASK = CRD_NL | CRD_GROUPMASK
			,CRD_DEFGROUP = CRD_G1 | CRD_G2};
				 COLRECORD(void) : PROTOCOL_NODE(){Add(pn_Head).Add(pn_CTRL < pn_NL < pn_Group < pn_Adress) < pn_COL < pnlc_text < pn_Trail;};
		virtual ~COLRECORD(void){;};
	protected:
		PROTOCOL_NODE	pn_Head;
		PROTOCOL_NODE	pn_Trail;
	
		PROTOCOL_NODE	pn_CTRL;
		PROTOCOL_NODE	pn_NL;
		PROTOCOL_NODE	pn_Group;
		PROTOCOL_NODE	pn_Adress;
		PROTOCOL_NODE	pn_COL;
		PNODE_LC		pnlc_text;
	public:
		void Init(const FIFO8 *defFifo){
			PROTOCOL_NODE::Init	(FNT_MESSAGE,defFifo,0,G_ENDIAN_LITTLE);
			pn_Head.Init (FNT_HEAD, defFifo);pn_Head.SetFixValue("\xF8");
			pn_Trail.Init(FNT_TAIL, defFifo);pn_Trail.SetFixValue("\xF9");
			
			pn_CTRL.Init		(FNT_MASK,defFifo,2,G_ENDIAN_LITTLE);
			pn_NL.Init			(FNT_CTRL, defFifo);pn_NL.SetMaskBit	(CRD_NL);
			pn_Group.Init		(FNT_CTRL, defFifo);pn_Group.SetMaskBit	(CRD_GROUPMASK);
			pn_Adress.Init		(FNT_CTRL, defFifo);pn_Adress.SetMaskBit(CRD_ADDRMASK);
			pn_CTRL.FillMaskField();
			pn_COL.Init			(FNT_CTRL,defFifo,1,G_ENDIAN_LITTLE);//max 255 color
			pnlc_text.Init		(defFifo,4,G_ENDIAN_LITTLE);//max 2^32 byte per message
			
			cgDefCtrl = CRD_GROUPMASK;
			cgDefCol = COL_NONE;
			cgblAddData = 0;
			SetSelfName("COLRECORD");
		}
	public:
		inline	const	uint32	&GetTextOffset(void)const{return(pnlc_text.GetContentOffset());};
		inline	const	uint32	&GetTextLength(void)const{return(pnlc_text.GetContentLength());};
	public:
		static	inline	uint32	CheckNL		(uint32 tCtrl)					{return(B_ChkFLAG32(tCtrl,CRD_NL));};
		static	inline	uint32	CheckGroup	(uint32 tCtrl,uint32 tGroup)	{return(B_ChkFLAG32(tCtrl,tGroup & CRD_GROUPMASK));};
		static	inline	uint32	GetGroup	(uint32 tCtrl)					{return(tCtrl & CRD_GROUPMASK);};
		static	inline	uint32	GetAddress	(uint32 tCtrl)					{return(tCtrl & CRD_ADDRMASK);};
	public:
		inline			uint32	ReadCtrl(const FIFO8 *fifo = nullptr)const{return(pn_CTRL.GetValueCalc(fifo));};
		inline			uint32	ReadCOL	(const FIFO8 *fifo = nullptr)const{return(pn_COL.GetValueCalc(fifo));};
		inline	const	STDSTR	&ReadContent(STDSTR *retStrText,uint32 *retCOL = nullptr,uint32 *retCtrl = nullptr,const FIFO8 *fifo = nullptr)const{
			if (retCOL != nullptr)
				*retCOL = pn_COL.GetValueCalc(fifo);
			if (retCtrl != nullptr)
				*retCtrl = pn_COL.GetValueCalc(fifo);
			return(pnlc_text.ReadContent(retStrText,fifo));
		};
		inline	const	FIFO8	&ReadContent(FIFO8 *retFifoText,uint32 *retCOL = nullptr,uint32 *retCtrl = nullptr,const FIFO8 *fifo = nullptr)const{
			if (retCOL != nullptr)
				*retCOL = pn_COL.GetValueCalc(fifo);
			if (retCtrl != nullptr)
				*retCtrl = pn_COL.GetValueCalc(fifo);
			return(pnlc_text.ReadContent(retFifoText,fifo));
		};
		inline	const	COLRECORD	&SetContent	(uint32 tCOL,uint32 tCtrl,std::stringstream &streamIn){
			PROTOCOL_NODE::_Start();
			pn_Head.SetFIFOFixValue();
			pn_CTRL.SetFIFOByte(tCtrl);
			pn_COL.SetFIFOByte(tCOL);
			pnlc_text.SetContent(streamIn);
			pn_Trail.SetFIFOFixValue();
			PROTOCOL_NODE::_Endl();
			return(*this);
		};
		inline	const	COLRECORD	&SetContent	(uint32 tCOL,uint32 tCtrl,const PROTOCOL_NODE &pnIn){
			PROTOCOL_NODE::_Start();
			pn_Head.SetFIFOFixValue();
			pn_CTRL.SetFIFOByte(tCtrl);
			pn_COL.SetFIFOByte(tCOL);
			pnlc_text.SetContent(pnIn);
			pn_Trail.SetFIFOFixValue();
			PROTOCOL_NODE::_Endl();
			return(*this);
		};
		inline	const	COLRECORD	&SetContent	(uint32 tCOL,uint32 tCtrl,const FIFO8 &fifoIn,uint32 num,uint32 offset){
			PROTOCOL_NODE::_Start();
			pn_Head.SetFIFOFixValue();
			pn_CTRL.SetFIFOByte(tCtrl);
			pn_COL.SetFIFOByte(tCOL);
			pnlc_text.SetContent(fifoIn, num, offset);
			pn_Trail.SetFIFOFixValue();
			PROTOCOL_NODE::_Endl();
			return(*this);
		};
		inline	const	COLRECORD	&SetContent	(uint32 tCOL,uint32 tCtrl,const uint8 *data,uint32 num){
			PROTOCOL_NODE::_Start();
			pn_Head.SetFIFOFixValue();
			pn_CTRL.SetFIFOByte(tCtrl);
			pn_COL.SetFIFOByte(tCOL);
			pnlc_text.SetContent(data, num);
			pn_Trail.SetFIFOFixValue();
			PROTOCOL_NODE::_Endl();
			return(*this);
		};
		inline	const	COLRECORD	&SetContent	(uint32 tCOL,uint32 tCtrl,const uint8 data){
			PROTOCOL_NODE::_Start();
			pn_Head.SetFIFOFixValue();
			pn_CTRL.SetFIFOByte(tCtrl);
			pn_COL.SetFIFOByte(tCOL);
			pnlc_text.SetContent(&data, 1);
			pn_Trail.SetFIFOFixValue();
			PROTOCOL_NODE::_Endl();
			return(*this);
		};
		inline	const	COLRECORD	&SetContent	(uint32 tCOL,uint32 tCtrl,const STDSTR &strIn){
			return(COLRECORD::SetContent(tCOL,tCtrl,(uint8*)strIn.c_str(),(uint32)strIn.length()));
		};
	public:
		uint32	cgDefCtrl;
		uint32	cgDefCol;
		uint32  cgblAddData;
	public:
		inline	virtual	void	_Start(void){
			PROTOCOL_NODE::_Start();
			pn_Head.SetFIFOFixValue();
			pn_CTRL.SetFIFOByte(-1);
			pn_COL.SetFIFOByte(-1);
			pnlc_text._Start();
			cgblAddData = 0;
		};
		inline	virtual	void	_Endl(void){
			pnlc_text._Endl();
			pn_Trail.SetFIFOFixValue();
			pn_CTRL.UpdateFIFOByte(cgDefCtrl);
			pn_COL.UpdateFIFOByte(cgDefCol);
			PROTOCOL_NODE::_Endl();
		};
	public:
		inline COLRECORD& operator << (COLRECORD&(*fun)(COLRECORD&)){return((*fun)(*this));};
		friend inline COLRECORD& operator << (COLRECORD& _pn,std::stringstream &streamIn)		{_pn.pnlc_text << streamIn;	_pn.cgblAddData = 1;return(_pn);};
		friend inline COLRECORD& operator << (COLRECORD& _pn,const PROTOCOL_NODE &pnIn)			{_pn.pnlc_text << pnIn;		_pn.cgblAddData = 1;return(_pn);};
		friend inline COLRECORD& operator << (COLRECORD& _pn,const _Data<const FIFO8*> &data)	{_pn.pnlc_text << data;		_pn.cgblAddData = 1;return(_pn);};
		friend inline COLRECORD& operator << (COLRECORD& _pn,const _Data<const uint8*> &data)	{_pn.pnlc_text << data;		_pn.cgblAddData = 1;return(_pn);};
		friend inline COLRECORD& operator << (COLRECORD& _pn,const _Data<const char*> &data)	{_pn.pnlc_text << data;		_pn.cgblAddData = 1;return(_pn);};
		friend inline COLRECORD& operator << (COLRECORD& _pn,const char* data)					{_pn.pnlc_text << data;		_pn.cgblAddData = 1;return(_pn);};
		friend inline COLRECORD& operator << (COLRECORD& _pn,const STDSTR &strIn)				{_pn.pnlc_text << strIn;	_pn.cgblAddData = 1;return(_pn);};
		friend inline COLRECORD& operator << (COLRECORD& _pn,const COLSTR &colStrIn){
			if (colStrIn.col != -1)
				_pn << SetCol(colStrIn.col);
			_pn.pnlc_text << *colStrIn.p;
			_pn.cgblAddData = 1;
			return(_pn);
		};
	
		inline	void _NL(void){
			if (cgblAddData == 0){
				cgDefCtrl |= CRD_NL;
			}
			else{
				pnlc_text << "\n";
			}
		};
		virtual void _SetX(const _Addr &data){
			if (cgblAddData == 0){
				cgDefCtrl = data.value & (~CRD_NL);
			}
			else{
				COLRECORD::_Endl();
				COLRECORD::_Start();
				cgDefCtrl = data.value & (~CRD_NL);
			}
		}
		virtual void _SetX(const _Group &data){
			if (cgblAddData == 0){
				cgDefCtrl |= (data.value & CRD_GROUPMASK);
			}
			else if (B_ChkFLAG32(cgDefCtrl & CRD_GROUPMASK, data.value & CRD_GROUPMASK) == 0){
				COLRECORD::_Endl();
				COLRECORD::_Start();
				cgDefCtrl &= (~CRD_NL);
				cgDefCtrl |= data.value;
			}
		};
		virtual void _ClrX(const _cGroup &data){
			if (cgblAddData == 0){
				B_ClrFLAG32(cgDefCtrl,(data.value & CRD_GROUPMASK));
			}
			else if (B_ChkFLAG32(cgDefCtrl & CRD_GROUPMASK, data.value & CRD_GROUPMASK) == 0){
				COLRECORD::_Endl();
				COLRECORD::_Start();
				cgDefCtrl &= (~CRD_NL);
				cgDefCtrl |= data.value;
			}
		};
		virtual void _SetX(const _Col &data){
			if (data.value == -1)
				return;
			if (cgblAddData == 0){
				cgDefCol = data.value;
			}
			else if (cgDefCol != data.value){
				COLRECORD::_Endl();
				COLRECORD::_Start();
				B_ClrFLAG32(cgDefCtrl, CRD_NL);
				cgDefCol = data.value;
			}
		};
		virtual void _SetX(const _Enforce &data){;};
		friend inline COLRECORD& operator << (COLRECORD& _pn,const _Addr &data)		{_pn._SetX(data);return(_pn);};
		friend inline COLRECORD& operator << (COLRECORD& _pn,const _Group &data)	{_pn._SetX(data);return(_pn);};
		friend inline COLRECORD& operator << (COLRECORD& _pn,const _cGroup &data)	{_pn._ClrX(data);return(_pn);};
		inline	void	_G1(void){_SetX(SetGroup(CRD_G1));};
		inline	void	_G2(void){_SetX(SetGroup(CRD_G2));};
		inline	void	_G3(void){_SetX(SetGroup(CRD_G3));};
		friend inline COLRECORD& operator << (COLRECORD& _pn,const _Col &data)		{_pn._SetX(data);return(_pn);};
		friend inline COLRECORD& operator << (COLRECORD& _pn,uint32 data)			{_pn._SetX(SetCol(data));return(_pn);};
		friend inline COLRECORD& operator << (COLRECORD& _pn,const _Enforce &data)	{_pn._SetX(data);return(_pn);};
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class COLRECORD_CACHE : public COLRECORD{
	public:
				 COLRECORD_CACHE(void)			: COLRECORD()	{;};
				 COLRECORD_CACHE(uint32 tSize)	: COLRECORD()	{Init(tSize);};
		virtual ~COLRECORD_CACHE(void){;};
	public:
		inline	void	Init	(uint32 tSize){
			cgRecordFifo.Init(tSize);
			COLRECORD::Init(&cgRecordFifo);
			fnRecordOut.Init(&cgRecordFifo);
			SetSelfName("COLRECORD_CACHE");
		};
	protected:
		FIFO8			cgRecordFifo;
		COLRECORD		fnRecordOut;
	
		uint32			groupDisableStatus;
	private:
		inline	uint32	MakeCtrl			(uint32 tCtrl,uint32 tEnforce)	{return(B_SetFLAG32(B_ClrFLAG32(tCtrl,groupDisableStatus),tEnforce & CRD_GNLMASK));}
	public:
		inline	void	DisableGroup		(uint32 tGroup)					{Spin_InUse_set();B_SetFLAG32(groupDisableStatus,tGroup & CRD_GROUPMASK);Spin_InUse_clr();};
		inline	void	EnableGroup			(uint32 tGroup)					{Spin_InUse_set();B_ClrFLAG32(groupDisableStatus,tGroup & CRD_GROUPMASK);Spin_InUse_clr();};
		inline	int32	CheckDisableGroup	(uint32 tGroup)					{return(B_ChkFLAG32(groupDisableStatus,tGroup & CRD_GROUPMASK));};
	public:
		inline	COLRECORD_CACHE&	Write	(uint32 tCol,const FIFO8 &fifoIn,uint32 num,uint32 offset,G_LOCK blLock = G_LOCK_ON,uint32 tEnforce = 0,uint32 tAddr = CRD_DEFGROUP){
			Spin_InUse_set(blLock);
			SetContent(tCol, MakeCtrl(tAddr,tEnforce), fifoIn, num, offset);
			Spin_InUse_clr(blLock);
			return(*this);
		};
		inline	COLRECORD_CACHE&	Write	(uint32 tCol,std::stringstream &streamIn,G_LOCK blLock = G_LOCK_ON,uint32 tEnforce = 0,uint32 tAddr = CRD_DEFGROUP){
			Spin_InUse_set(blLock);
			SetContent(tCol, MakeCtrl(tAddr,tEnforce), streamIn);
			Spin_InUse_clr(blLock);
			return(*this);
		};
		inline	COLRECORD_CACHE&	Write	(uint32 tCol,const PROTOCOL_NODE &pnIn,G_LOCK blLock = G_LOCK_ON,uint32 tEnforce = 0,uint32 tAddr = CRD_DEFGROUP){
			Spin_InUse_set(blLock);
			SetContent(tCol, MakeCtrl(tAddr,tEnforce), pnIn);
			Spin_InUse_clr(blLock);
			return(*this);
		};
		inline	COLRECORD_CACHE&	Write	(uint32 tCol,const uint8 *data,uint32 num,G_LOCK blLock = G_LOCK_ON,uint32 tEnforce = 0,uint32 tAddr = CRD_DEFGROUP){
			Spin_InUse_set(blLock);
			SetContent(tCol, MakeCtrl(tAddr,tEnforce), data, num);
			Spin_InUse_clr(blLock);
			return(*this);
		};
		inline	COLRECORD_CACHE&	Write	(uint32 tCol,const uint8 data,G_LOCK blLock = G_LOCK_ON,uint32 tEnforce = 0,uint32 tAddr = CRD_DEFGROUP){
			Spin_InUse_set(blLock);
			SetContent(tCol, MakeCtrl(tAddr,tEnforce), &data, 1);
			Spin_InUse_clr(blLock);
			return(*this);
		};
		inline	COLRECORD_CACHE&	Write	(uint32 tCol,const STDSTR &strIn,G_LOCK blLock = G_LOCK_ON,uint32 tEnforce = 0,uint32 tAddr = CRD_DEFGROUP){
			Spin_InUse_set(blLock);
			SetContent(tCol, MakeCtrl(tAddr,tEnforce), strIn);
			Spin_InUse_clr(blLock);
			return(*this);
		};
		
		inline	COLRECORD_CACHE&	WriteNL	(uint32 tCol,const FIFO8 &fifoIn,uint32 num,uint32 offset,G_LOCK blLock = G_LOCK_ON,uint32 tEnforce = 0,uint32 tAddr = CRD_DEFGROUP){
			Spin_InUse_set(blLock);
			SetContent(tCol, MakeCtrl(tAddr,tEnforce | CRD_NL), fifoIn, num, offset);
			Spin_InUse_clr(blLock);
			return(*this);
		};
		inline	COLRECORD_CACHE&	WriteNL	(uint32 tCol,std::stringstream &streamIn,G_LOCK blLock = G_LOCK_ON,uint32 tEnforce = 0,uint32 tAddr = CRD_DEFGROUP){
			Spin_InUse_set(blLock);
			SetContent(tCol, MakeCtrl(tAddr,tEnforce | CRD_NL), streamIn);
			Spin_InUse_clr(blLock);
			return(*this);
		};
		inline	COLRECORD_CACHE&	WriteNL	(uint32 tCol,const PROTOCOL_NODE &pnIn,G_LOCK blLock = G_LOCK_ON,uint32 tEnforce = 0,uint32 tAddr = CRD_DEFGROUP){
			Spin_InUse_set(blLock);
			SetContent(tCol, MakeCtrl(tAddr,tEnforce | CRD_NL), pnIn);
			Spin_InUse_clr(blLock);
			return(*this);
		};
		inline	COLRECORD_CACHE&	WriteNL	(uint32 tCol,const uint8 data,G_LOCK blLock = G_LOCK_ON,uint32 tEnforce = 0,uint32 tAddr = CRD_DEFGROUP){
			Spin_InUse_set(blLock);
			SetContent(tCol, MakeCtrl(tAddr,tEnforce | CRD_NL), &data, 1);
			Spin_InUse_clr(blLock);
			return(*this);
		};
		inline	COLRECORD_CACHE&	WriteNL	(uint32 tCol,const uint8 *data,uint32 num,G_LOCK blLock = G_LOCK_ON,uint32 tEnforce = 0,uint32 tAddr = CRD_DEFGROUP){
			Spin_InUse_set(blLock);
			SetContent(tCol, MakeCtrl(tAddr,tEnforce | CRD_NL), data, num);
			Spin_InUse_clr(blLock);
			return(*this);
		};
		inline	COLRECORD_CACHE&	WriteNL	(uint32 tCol,const STDSTR &strIn,G_LOCK blLock = G_LOCK_ON,uint32 tEnforce = 0,uint32 tAddr = CRD_DEFGROUP){
			Spin_InUse_set(blLock);
			SetContent(tCol, MakeCtrl(tAddr,tEnforce | CRD_NL), strIn);
			Spin_InUse_clr(blLock);
			return(*this);
		};
	public:
		inline	virtual	void	_Start(void){Spin_InUse_set();cgDefCtrl = MakeCtrl(CRD_DEFGROUP,0);cgDefCol = COL_clDefault;COLRECORD::_Start();};
		inline	virtual	void	_Endl(void) {COLRECORD::_Endl();Spin_InUse_clr();};
	
		inline COLRECORD_CACHE& operator << (COLRECORD_CACHE&(*fun)(COLRECORD_CACHE&)){return((*fun)(*this));};
		
		virtual void _SetX(const _Addr &data)	{COLRECORD::_SetX(SetAddr(MakeCtrl(data.value, 0)));}
		virtual void _SetX(const _Group &data)	{COLRECORD::_SetX(SetGroup(MakeCtrl(cgDefCtrl | (data.value & CRD_GROUPMASK), 0)));};
		virtual void _SetX(const _Enforce &data){
			uint32 newCtrl;
			newCtrl = MakeCtrl(cgDefCtrl,data.value & CRD_GROUPMASK);
			if (cgblAddData == 0){
				cgDefCtrl = newCtrl;
			}
			else if (cgDefCtrl != newCtrl){
				COLRECORD::_Endl();
				COLRECORD::_Start();
				cgDefCtrl = newCtrl & (~CRD_NL);
			}
		};
};
//------------------------------------------------------------------------------------------//
CreateOperatorFun(EnforceDefGroup)
//------------------------------------------------------------------------------------------//
class COLRECORD_NODE : public TREE_NODE{
	public:
				 COLRECORD_NODE(void) : TREE_NODE(){SetSelfName("COLRECORD_NODE");SetAddress(nullptr,0,0,0);};
		virtual ~COLRECORD_NODE(void){;};
	protected:
		COLRECORD_CACHE	*cgRecordCache;
		uint32			cgAddress;
		uint32			cgMask;
		uint32			cgBroadcast;
		uint32			cgExtraGroup;
	public:
		inline	void	SetAddress		(const COLRECORD_CACHE *cache,uint32 addr,uint32 mask,uint32 exGroup){
			Spin_InUse_set();
			cgRecordCache = (COLRECORD_CACHE*)cache;
			cgAddress = addr;
			cgMask = mask;
			cgBroadcast = cgAddress | ((~mask) & COLRECORD::CRD_ADDRMASK);
			cgExtraGroup = exGroup;
			Spin_InUse_clr();
		};
		inline	void	SetExtraGroup	(uint32 group){cgExtraGroup = (group & COLRECORD::CRD_GROUPMASK);};
		inline	void	SetDevID		(uint32 devID){cgAddress = (cgAddress & (cgMask | COLRECORD::CRD_GROUPMASK)) | (devID & (~cgMask) & COLRECORD::CRD_ADDRMASK);};
		
		inline	const uint32	&GetAddress	(void)const{return(cgAddress);};
	public:
		inline	static	uint32	CheckNL		(uint32 tCtrl){return(COLRECORD::CheckNL(tCtrl));};
	public:
		inline	void	PrintDisable		(void){cgRecordCache->DisableGroup(cgAddress);};
		inline	void	PrintEnable			(void){cgRecordCache->EnableGroup(cgAddress);};
		inline	int32	CheckPrintDisable	(void){return(cgRecordCache->CheckDisableGroup(cgAddress));};
	public:
		inline	void	Write	(uint32 tCol,const FIFO8 &fifoIn,uint32 num,uint32 offset,G_LOCK blLock = G_LOCK_ON,uint32 tExtraGroup = 0)const{
			cgRecordCache->Write(tCol, fifoIn ,num ,offset, blLock, cgAddress | cgExtraGroup | tExtraGroup, cgAddress | cgExtraGroup);
		};
		inline	void	Write	(uint32 tCol,std::stringstream &streamIn,G_LOCK blLock = G_LOCK_ON,uint32 tExtraGroup = 0)const{
			cgRecordCache->Write(tCol, streamIn, blLock, cgAddress | tExtraGroup, cgAddress | cgExtraGroup);
		};
		inline	void	Write	(uint32 tCol,const PROTOCOL_NODE &pnIn,G_LOCK blLock = G_LOCK_ON,uint32 tExtraGroup = 0)const{
			cgRecordCache->Write(tCol, pnIn, blLock, cgAddress | tExtraGroup, cgAddress | cgExtraGroup);
		};
		inline	void	Write	(uint32 tCol,const uint8 *data,uint32 num,G_LOCK blLock = G_LOCK_ON,uint32 tExtraGroup = 0)const{
			cgRecordCache->Write(tCol, data, num, blLock, cgAddress | tExtraGroup, cgAddress | cgExtraGroup);
		};
		inline	void	Write	(uint32 tCol,const uint8 data,G_LOCK blLock = G_LOCK_ON,uint32 tExtraGroup = 0)const{
			cgRecordCache->Write(tCol, &data, 1, blLock, cgAddress | tExtraGroup, cgAddress | cgExtraGroup);
		};
		inline	void	Write	(uint32 tCol,const STDSTR &strIn,G_LOCK blLock = G_LOCK_ON,uint32 tExtraGroup = 0)const{
			cgRecordCache->Write(tCol, strIn, blLock, cgAddress | tExtraGroup, cgAddress | cgExtraGroup);
		};
		
		inline	void	WriteNL	(uint32 tCol,const FIFO8 &fifoIn,uint32 num,uint32 offset,G_LOCK blLock = G_LOCK_ON,uint32 tExtraGroup = 0)const{
			cgRecordCache->WriteNL(tCol, fifoIn ,num , offset, blLock, cgAddress | tExtraGroup, cgAddress | cgExtraGroup);
		};
		inline	void	WriteNL	(uint32 tCol,const PROTOCOL_NODE &pnIn,G_LOCK blLock = G_LOCK_ON,uint32 tExtraGroup = 0)const{
			cgRecordCache->WriteNL(tCol, pnIn, blLock, cgAddress | tExtraGroup, cgAddress | cgExtraGroup);
		};
		inline	void	WriteNL	(uint32 tCol,std::stringstream &streamIn,G_LOCK blLock = G_LOCK_ON,uint32 tExtraGroup = 0)const{
			cgRecordCache->WriteNL(tCol, streamIn, blLock, cgAddress | tExtraGroup, cgAddress | cgExtraGroup);
		};
		inline	void	WriteNL	(uint32 tCol,const uint8 *data,uint32 num ,G_LOCK blLock = G_LOCK_ON,uint32 tExtraGroup = 0)const{
			cgRecordCache->WriteNL(tCol, data, num, blLock, cgAddress | tExtraGroup, cgAddress | cgExtraGroup);
		};
		inline	void	WriteNL	(uint32 tCol,const uint8 data,G_LOCK blLock = G_LOCK_ON,uint32 tExtraGroup = 0)const{
			cgRecordCache->WriteNL(tCol, &data, 1, blLock, cgAddress | tExtraGroup, cgAddress | cgExtraGroup);
		};
		inline	void	WriteNL	(uint32 tCol,const STDSTR &strIn,G_LOCK blLock = G_LOCK_ON,uint32 tExtraGroup = 0)const{
			cgRecordCache->WriteNL(tCol, strIn, blLock, cgAddress | tExtraGroup, cgAddress | cgExtraGroup);
		};
	public:
		inline	virtual	void	_Start(void){*cgRecordCache << Start << SetAddr(cgAddress | cgExtraGroup) << SetEnforce(cgAddress);};
		inline	virtual	void	_Endl(void) {*cgRecordCache << Endl;};
	public:
		inline COLRECORD_NODE& operator << (COLRECORD_NODE&(*fun)(COLRECORD_NODE&)){return((*fun)(*this));};
		
		friend inline COLRECORD_NODE& operator << (COLRECORD_NODE& _pn,std::stringstream &streamIn)		{*_pn.cgRecordCache << streamIn;return(_pn);};
		friend inline COLRECORD_NODE& operator << (COLRECORD_NODE& _pn,const PROTOCOL_NODE &pnIn)		{*_pn.cgRecordCache << pnIn;	return(_pn);};
		friend inline COLRECORD_NODE& operator << (COLRECORD_NODE& _pn,const _Data<const FIFO8*> &data)	{*_pn.cgRecordCache << data;	return(_pn);};
		friend inline COLRECORD_NODE& operator << (COLRECORD_NODE& _pn,const _Data<const uint8*> &data)	{*_pn.cgRecordCache << data;	return(_pn);};
		friend inline COLRECORD_NODE& operator << (COLRECORD_NODE& _pn,const _Data<const char*> &data)	{*_pn.cgRecordCache << data;	return(_pn);};
		friend inline COLRECORD_NODE& operator << (COLRECORD_NODE& _pn,const char *data)				{*_pn.cgRecordCache << data;	return(_pn);};
		friend inline COLRECORD_NODE& operator << (COLRECORD_NODE& _pn,const STDSTR &strIn)				{*_pn.cgRecordCache << strIn;	return(_pn);};
		friend inline COLRECORD_NODE& operator << (COLRECORD_NODE& _pn,const COLSTR &colStrIn)			{*_pn.cgRecordCache << colStrIn;return(_pn);};
		
		inline	void  _NL(void){cgRecordCache->_NL();};
		inline	void  _EnforceDefGroup(void){*cgRecordCache << SetEnforce(cgAddress | cgExtraGroup);};
		friend inline COLRECORD_NODE& operator << (COLRECORD_NODE& _pn,const _Col &data)				{*_pn.cgRecordCache << data;return(_pn);};
		friend inline COLRECORD_NODE& operator << (COLRECORD_NODE& _pn,const _cGroup &data)				{*_pn.cgRecordCache << data;return(_pn);};
		friend inline COLRECORD_NODE& operator << (COLRECORD_NODE& _pn,uint32 data)						{*_pn.cgRecordCache << SetCol(data);return(_pn);};
		friend inline COLRECORD_NODE& operator << (COLRECORD_NODE& _pn,const _Enforce &data)			{*_pn.cgRecordCache << SetEnforce(_pn.cgAddress | data.value);return(_pn);};
};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif

