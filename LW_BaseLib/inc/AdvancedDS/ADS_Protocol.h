//
//  ADS_Protocol.h
//  SDT
//
//  Created by Leif Wen on 24/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "DS_Transform.h"
#ifdef DS_Transform_h
//------------------------------------------------------------------------------------------//
#ifndef ADS_Protocol_h
#define ADS_Protocol_h
#ifdef ADS_Protocol_h
#include "ALG_CRC.h"
//------------------------------------------------------------------------------------------//
struct ADS_POSITION{
	UVOut	uvOut;
	ARRAY	*array;
	uint32	offset;
	uint32	length;
};
static inline void POS_Reset	(ADS_POSITION* pos);
static inline void POS_Hold		(ADS_POSITION* pos);
static inline void POS_Update	(ADS_POSITION* pos);
//------------------------------------------------------------------------------------------//
#define CreateOperatorSetUint32(x) \
	struct 					_##x							{uint32 value;};\
	static inline _##x		Set##x	(const uint32 &value)	{return{value};};

#define CreateOperatorClrUint32(x) \
	struct					_Clr##x							{uint32 value;};\
	static inline _Clr##x	Clr##x	(const uint32 &value)	{return{value};};

#define CreateOperatorFun1(x) \
	template <typename T>static inline T& x(T& _tn){_tn._##x();return(_tn);};

#define CreateOperatorFun2(x) \
	struct 			_##x##F				{void* p;};\
	static inline	_##x##F	x	(void)	{return{(void*)nullptr};};

template <typename T>	static inline T&		Start	(T& _tn);

struct 			_BeginIOS 								{IOSTATUS* value;};
static inline	_BeginIOS	Begin	(IOSTATUS* _ios)	{return{(IOSTATUS*)_ios};};

CreateOperatorFun2(Begin);
CreateOperatorFun1(Endl);
CreateOperatorFun2(Endl);
//------------------------------------------------------------------------------------------//
class ADS_FIFO : public DSTF{
	protected:
		typedef ADS_FIFO ADSF;
	public:
				 ADS_FIFO(void);
		virtual ~ADS_FIFO(void){;};
	protected:
		ADS_POSITION		cgPosWR;
		ADS_POSITION		cgPosRE;
	public:
		inline			void	SetDefArrayWR	(const ARRAY* array = nullptr);
		inline			void	SetDefArrayRE	(const ARRAY* array = nullptr);
		inline			ARRAY*	GetDefArrayWR	(void)const;
		inline			ARRAY*	GetDefArrayRE	(void)const;
	protected:
		inline	virtual	ADSF&	DoTransform		(IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
		inline	virtual	ADSF&	DoFinal			(IOSTATUS* _ios,const UVOut& _out);
	public:
		inline			ADSF&	Transform		(IOSTATUS* _ios,const UVIn& _in);
		inline			ADSF&	Transform		(IOSTATUS* _ios,const uint8* data,const uint64& length);
		inline			ADSF&	Final			(IOSTATUS* _ios);
	public:
				virtual	ADSF&	_Begin			(IOSTATUS* _ios);
				virtual	ADSF&	_Endl			(void);
	public:
		inline			ADSF&	AllIn			(IOSTATUS* _ios,const UVIn&  _in);
		inline			ADSF&	AllOut			(IOSTATUS* _ios,const UVOut& _out);
	public:
		template <typename T_DSTF> inline friend T_DSTF& operator << (T_DSTF& _dstf,T_DSTF& (*fun)(T_DSTF&));
		template <typename T_DSTF> inline friend T_DSTF& operator << (T_DSTF& _dstf,const _BeginIOS& _sios);
		template <typename T_DSTF> inline friend T_DSTF& operator << (T_DSTF& _dstf,const _BeginF& _fun);
		template <typename T_DSTF> inline friend T_DSTF& operator << (T_DSTF& _dstf,const _EndlF& _fun);
};
//------------------------------------------------------------------------------------------//
enum {
	UVID_PNF = UVID_Next,
	UVID_PNF_Next,
};
//------------------------------------------------------------------------------------------//
class DS_IO_PNF : public DS_IO_NODE{
	public:
		enum	{DSIO_PNF_UVID = UVID_PNF};
	public:
				 DS_IO_PNF(void) : DS_IO_NODE(){;};
		virtual ~DS_IO_PNF(void){;};
	public:
		virtual	bool32	DoConvert	(DSIO* ioNode,	IOSTATUS* _ios,const UVOut& _out,const UVIn& _in);
		virtual	bool32	DoSave		(				IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
};
//------------------------------------------------------------------------------------------//
class 	PROTOCOL_NODE_FRAME;
typedef PROTOCOL_NODE_FRAME PNF;
class PROTOCOL_NODE_FRAME : public ADS_FIFO{
	protected:
			public:
				 PROTOCOL_NODE_FRAME(void);
		virtual ~PROTOCOL_NODE_FRAME(void){;};
	protected:
				uint32		cgFixedByte;
				PNF*		cgPNF;
	public:
		inline	virtual	TNF*		AddNode			(TNF* pn);
	protected:
		inline	virtual void		ResetPNLength	(void);
	public:
		inline			void		InitPN			(const ARRAY* _out,const ARRAY* _in,uint32 fixedNum);
		inline			void		SetFixedByte	(uint32 fixedNum);
		inline			uint32&		GetFixedByte	(void);
		inline			void		SyncPosRE		(const ADS_POSITION* pos);
		inline			void		SyncPosWR		(const ADS_POSITION* pos);
		inline	const	uint32&		GetOffsetWR		(void)const;
		inline	const	uint32&		GetLengthWR		(void)const;
		inline	const	uint32&		GetOffsetRE		(void)const;
		inline	const	uint32&		GetLengthRE		(void)const;
		inline	const	void		SetOffsetRE		(uint32 offset);
		inline	const	void		SetLengthRE		(uint32 length);
	public:
		inline	virtual	void		ResetPosWR		(void);
		inline	virtual	void		ResetPosRE		(void);
				virtual	bool32		Analysis		(uint32 startOffset);
		inline	virtual	bool32		Read			(IOSTATUS* _ios,const UVOut& _out);
		inline	virtual	PNF&		Write			(IOSTATUS* _ios,const UVIn&  _in);
		inline	virtual	PNF&		WriteNone		(void);
		inline	virtual	PNF&		CleanWR			(void);
		inline	virtual	PNF&		CleanRE			(void);
};
static inline		_UVBASE		OUDMesg			(PNF* p);
static inline		_UVBASE		IUDMesg			(PNF* p);
//------------------------------------------------------------------------------------------//
class PNF_VAR : public PNF{
	protected:
		enum	{RFLAG_C = 1, RFLAG_S = DSTF::RFLAG_S + DSTF::RFLAG_C};
		enum	{PNF_blEndianBig = RFLAG_CREATE(0)};
	public:
				 PNF_VAR(void);
		virtual ~PNF_VAR(void){;};
	protected:
		uint32			cgMask;
		uint32			cgMovebit;
	public:
		inline			void	InitPN			(const ARRAY* _out,const ARRAY* _in,uint32 fixedNum,G_ENDIAN endian = G_ENDIAN_LITTLE);
				virtual	void	SetEndian		(G_ENDIAN endian = G_ENDIAN_LITTLE);
	private:
						uint32	GetOValueWR		(void)const;
						uint32	GetOValueRE		(void)const;
	public:
						void	SetMaskBit		(uint32	mask);
		inline			uint32	GetValueWR		(void)const;
		inline			uint32	GetValueRE		(void)const;
		inline			uint32	GetValueAMaskWR	(void)const;
		inline			uint32	GetValueAMaskRE	(void)const;
		inline			uint32	GetValueCalcWR	(void)const;
		inline			uint32	GetValueCalcRE	(void)const;
	protected:
						void	SetByte1		(IOSTATUS* _ios,uint32 data);
						void	SetByte2		(IOSTATUS* _ios,uint32 data);
						void	SetByte3		(IOSTATUS* _ios,uint32 data);
						void	SetByte4		(IOSTATUS* _ios,uint32 data);
	protected:
						void	UpdateByte1		(uint32 data);
						void	UpdateByte2		(uint32 data);
						void	UpdateByte3		(uint32 data);
						void	UpdateByte4		(uint32 data);
	protected:
						void	SetByte			(IOSTATUS* _ios,uint32 data);
						void	UpdateByte		(uint32 data);
	public:
		inline	const	PNF_VAR& Update			(uint32 data);
		inline			PNF_VAR& Write			(IOSTATUS* _ios,uint32 data);
		inline	const	PNF_VAR& operator =		(uint32 data);
};
//------------------------------------------------------------------------------------------//
class PNF_MASK : public PNF_VAR{
	public:
				 PNF_MASK(void) : PNF_VAR(){;};
		virtual ~PNF_MASK(void){;};
	public:
				void	FillMaskFieldRE	(void);
				void	FillMaskFieldWR	(void);
	public:
				virtual	bool32		Analysis		(uint32 startOffset);
						PNF_MASK&	Write			(IOSTATUS* _ios,uint32 data);
};
//------------------------------------------------------------------------------------------//
typedef PNF_VAR PNF_ADDR;
typedef PNF_VAR PNF_CTRL;
typedef PNF_VAR PNF_LEN;
typedef PNF_VAR PNF_CRC;
//------------------------------------------------------------------------------------------//
class PNF_FIXED : public PNF{
	public:
				 PNF_FIXED(void);
		virtual ~PNF_FIXED(void){;};
	private:
		STDSTR			cgFixedStr;
	public:
		inline			void		InitPN			(const ARRAY* _out,const ARRAY* _in,const STDSTR& fixedStr);
	private:
						bool32		CheckFixedStr	(void)const;
	public:
				virtual	bool32		Analysis		(uint32 startOffset);
		inline			PNF_FIXED&	Write			(IOSTATUS* _ios);
};
//------------------------------------------------------------------------------------------//
typedef PNF_FIXED PNF_HEAD;
//------------------------------------------------------------------------------------------//
class PNF_TAIL : public PNF_FIXED{
	public:
				 PNF_TAIL(void) : PNF_FIXED(){;};
		virtual ~PNF_TAIL(void){;};
	public:
		inline			void	InitPN				(const ARRAY* _out,const ARRAY* _in,const STDSTR& fixedStr,PNF* pnContent = nullptr);
				virtual	bool32	Analysis			(uint32 startOffset);
};
//------------------------------------------------------------------------------------------//
class PNF_CONTENT : public PNF{
	public:
				 PNF_CONTENT(void) : PNF(){;};
		virtual ~PNF_CONTENT(void){;};
	public:
		inline			void			InitPN				(const ARRAY* _out,const ARRAY* _in,PNF_VAR* pnLength);
		inline			void			InitPN				(const ARRAY* _out,const ARRAY* _in,uint32 fixedNum);
	public:
				virtual void			ResetPNLength		(void);
		inline	const	PNF_CONTENT&	operator =			(const UVIn& _in);
};
//------------------------------------------------------------------------------------------//
class PNF_BLOCK : public PNF{
	public:
				 PNF_BLOCK(void);
		virtual ~PNF_BLOCK(void){;};
	public:
		inline	virtual	TNF*		AddNode			(TNF *pn);
	public:
		inline			void		InitPN			(const ARRAY* _out,const ARRAY* _in,const PNF* pn = nullptr);
		inline			void		SetBlockPoint	(const PNF* pn);
	protected:
		inline	virtual	PNF_BLOCK&	DoTransform		(IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
		inline	virtual bool32		ChecksumResult	(void)const;
	public:
				virtual	bool32		Analysis		(uint32 startOffset);
		inline	virtual	bool32		Read			(IOSTATUS* _ios,const UVOut& _out);
};
//------------------------------------------------------------------------------------------//
class PNFB_LC : public PNF_BLOCK{
	public:
				 PNFB_LC(void);
		virtual ~PNFB_LC(void){;};
	public:
		PNF_LEN			pnlc_Len;
		PNF_CONTENT		pnlc_Text;
	public:
		inline			void		InitPN				(const ARRAY* _out,const ARRAY* _in,uint32 fixedNum = 2,G_ENDIAN endian = G_ENDIAN_LITTLE);
		inline			void		SetEndian			(G_ENDIAN endian);
	public:
		inline	const	uint32&		GetContentOffsetIn	(void)const;
		inline	const	uint32&		GetContentLengthIn	(void)const;
		inline	const	uint32&		GetContentOffsetOut	(void)const;
		inline	const	uint32&		GetContentLengthOut	(void)const;
	public:
				virtual PNFB_LC&	_Begin				(IOSTATUS* _ios);
				virtual	PNFB_LC&	_Endl				(void);
};
//------------------------------------------------------------------------------------------//
class PNF_SCC : public PNFB_LC{//only used for PNF_SC
	public:
				 PNF_SCC(void);
		virtual ~PNF_SCC(void){;};
	public:
		uint32			cgMaxSize;
	public:
		inline			void		InitPN			(const ARRAY* _out,const ARRAY* _in,uint32 maxSize);
		inline			bool32		IsFull			(void);
	public:
				virtual	void		ResetPosWR		(void);
				virtual	void		ResetPosRE		(void);
	
				virtual	PNF_SCC&	_Begin			(IOSTATUS* _ios);
				virtual	PNF_SCC&	_Endl			(void);
						uint32		Write			(IOSTATUS* _ios,const uint8* data,uint32 num);
};
//------------------------------------------------------------------------------------------//
class PNF_SC : public PNF{
	public:
				 PNF_SC(void);
		virtual ~PNF_SC(void){;};
	private:
		PNF_SCC			pnsc_LC[4];
		uint8			cgEA;
	public:
						void	InitPN		(const ARRAY* _out,const ARRAY* _in);
	protected:
				virtual	PNF_SC&	DoTransform	(IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
				virtual	PNF_SC&	DoFinal		(IOSTATUS* _ios,const UVOut& _out);
				virtual	void	ResetPosWR	(void);
				virtual	void	ResetPosRE	(void);
	public:
		inline	virtual	PNF_SC&	_Begin		(IOSTATUS* _ios);
	public:
				virtual	bool32	Analysis	(uint32 startOffset);
		inline	virtual	bool32	Read		(IOSTATUS* _ios,const UVOut& _out);
	
		inline	const	PNF_SC&	operator =	(const UVIn& _in);
};
//------------------------------------------------------------------------------------------//
class PNFB_SHELL : public PNF_BLOCK{
	protected:
		enum	{RFLAG_C = 1, RFLAG_S = PNF_BLOCK::RFLAG_S + PNF_BLOCK::RFLAG_C};
		enum	{PNF_blEnCRC = RFLAG_CREATE(0),};
	public:
				 PNFB_SHELL(void);
		virtual ~PNFB_SHELL(void){;};
	protected:
		PNF_HEAD		pns_Head;
		PNF_BLOCK		pns_Block;
		PNF_CRC			pns_Checksum;
		PNF_TAIL		pns_Tail;
	public:
						void		InitPN			(const ARRAY* _out,const ARRAY* _in,uint32 byteCRC = 2,G_ENDIAN endian = G_ENDIAN_LITTLE);
		inline	virtual	void		DisableCRC		(void);
		inline			PNF_BLOCK&	AddBlockSubPN	(PNF& subPN);
	
				virtual	PNFB_SHELL&	_Begin			(IOSTATUS* _ios);
				virtual	PNFB_SHELL&	_Endl			(void);
				virtual	bool32		Read			(IOSTATUS* _ios,const UVOut& _out);
	protected:
		inline	virtual	void		SetChecksum		(void);
						bool32		AnalysisR1		(uint32 startOffset);
	public:
						bool32		AnalysisFrame	(uint32 startOffset = 0);
						bool32		TryGetFrame		(void);
						uint32		Out				(void);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
template <typename T_PNF> class PNFB_LIST : public PNF_BLOCK{
	public:
				 PNFB_LIST(void);
		virtual ~PNFB_LIST(void){;};
	protected:
		uint32					cgNum;
		PNF_LEN					pnl_Qty;
		T_PNF					pnl_Text;
	public:
		inline	virtual 		void		SetSelfName		(const STDSTR& strName);
		inline	virtual			void		SetFatherName	(const STDSTR& strName);
	public:
								void		InitPN			(const ARRAY* _out,const ARRAY* _in);
		inline	virtual			PNFB_LIST&	InitCFG			(uint32 cfg,const void* par);
	protected:
				virtual			PNFB_LIST&	DoTransform		(IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
	public:
				virtual			bool32		Analysis		(uint32 startOffset);
				virtual			PNFB_LIST&	_Begin			(IOSTATUS* _ios);
				virtual			PNFB_LIST&	_Endl			(void);
	public:
								PNFB_LIST&	NewText			(void);
	template <typename... Args> PNFB_LIST&	Write			(IOSTATUS* _ios,const Args&... args);
								uint32		ReadQty			(void)const;
								bool32		Read			(IOSTATUS* _ios,const UVOut& _out,uint32 order);
};
//------------------------------------------------------------------------------------------//
typedef PNFB_LIST<PNFB_LC>	LIST_LC;
typedef PNFB_LIST<PNF_SC>	LIST_SC;
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CreateOperatorSetUint32(mID);
template <typename T_PNF> class PNFS_MESG : public PNFB_SHELL{
	protected:
		typedef PNFS_MESG PNFM;
	public:
				 PNFS_MESG(void);
		virtual ~PNFS_MESG(void){;};
	public:
		PNF_VAR			pnm_ID;
		T_PNF			pnm_Text;
	public:
		inline	virtual void	SetSelfName		(const STDSTR& strName);
		inline	virtual	void	SetFatherName	(const STDSTR& strName);
	public:
						void 	InitPN			(const ARRAY* _out,const ARRAY* _in,uint32 byteID = 1,uint32 byteCRC = 0,G_ENDIAN endian = G_ENDIAN_LITTLE);
		inline	virtual	PNFM&	InitCFG			(uint32 cfg,const void* par);
	public:
				virtual	PNFM&	_Begin			(IOSTATUS* _ios);
				virtual	PNFM&	_Endl			(void);
		inline	friend	PNFM&	operator << 	(PNFM& _pn,const _mID& mID)	{_pn.pnm_ID.Update(mID.value);return(_pn);};
	public:
		inline			uint32	ReadID			(void)const;
		inline			PNFM&	Write			(IOSTATUS* _ios,uint32 mID,const UVIn& _in);
};
//------------------------------------------------------------------------------------------//
#ifdef ALG_CRC_h
template <typename T_PNF,typename T_CRC> class PNFS_MESG_CRC : public PNFS_MESG<T_PNF>{
	public:
				 PNFS_MESG_CRC(void);
		virtual ~PNFS_MESG_CRC(void){;};
	protected:
		T_CRC			cgCRC0;
		T_CRC			cgCRC1;
		uint32			cgCRCCFG;
	public:
		inline	virtual void	SetSelfName		(const STDSTR& strName);
		inline	virtual	void	SetFatherName	(const STDSTR& strName);
	public:
						void	InitPN			(const ARRAY* _out,const ARRAY* _in,uint32 byteID = 1,uint32 crcCFG = -1,G_ENDIAN endian = G_ENDIAN_LITTLE);
		inline			void	InitCFG_CRC		(uint32 crcCFG);
	protected:
				virtual	void	SetChecksum		(void);
				virtual bool32	ChecksumResult	(void)const;
};
#endif
//------------------------------------------------------------------------------------------//
typedef PNFS_MESG<PNFB_LC>					MESG_LC;
typedef PNFS_MESG<PNF_SC>					MESG_SC;
//------------------------------------------------------------------------------------------//
#ifdef ALG_CRC_h
typedef PNFS_MESG_CRC<PNFB_LC,ALG_CRC8>		MESG8_LC;
typedef PNFS_MESG_CRC<PNFB_LC,ALG_CRC16>	MESG16_LC;
typedef PNFS_MESG_CRC<PNFB_LC,ALG_CRC32>	MESG32_LC;

typedef PNFS_MESG_CRC<PNF_SC,ALG_CRC8>		MESG8_SC;
typedef PNFS_MESG_CRC<PNF_SC,ALG_CRC16>		MESG16_SC;
typedef PNFS_MESG_CRC<PNF_SC,ALG_CRC32>		MESG32_SC;
#endif
//------------------------------------------------------------------------------------------//
namespace DSTF_TEST {
	template <typename T_SHELL> T_SHELL*	SHELL_Create	(const STDSTR& name,T_SHELL* _nullptr,ARRAY* _out,ARRAY* _in,uint32 cfg,void* p);
	template <typename T_SHELL>	void 		SHELL_Test		(IOSTATUS* _ios,T_SHELL* _msg,STDSTR* _out,				const STDSTR& _in);
	template <typename T_SHELL>	void		SHELL_Test		(IOSTATUS* _ios,T_SHELL* _msg,STDSTR* _out,uint32 mID,	const STDSTR& _in);
	
	static 	inline STDSTR	ShowINOUT	(const STDSTR& name,uint64 _in,uint64 _out);
	static	inline STDSTR	ShowINOUT	(IOSTATUS* _ios);
	static	inline void		PrintResult	(const STDSTR& name,const STDSTR& method,bool32 blok);
};
//------------------------------------------------------------------------------------------//
#include "ADS_Protocol.hpp"
#endif /* ADS_Protocol_h */
#endif /* ADS_Protocol_h */
#endif /* DS_Transform_h */
