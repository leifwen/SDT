//
//  ADS_Protocol.h
//  SDT
//
//  Created by Leif Wen on 24/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
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
static inline void POS_reset	(ADS_POSITION* pos);
static inline void POS_hold		(ADS_POSITION* pos);
static inline void POS_update	(ADS_POSITION* pos);
//------------------------------------------------------------------------------------------//
struct IOSIN{
	uint64	avail_in;	// number of bytes available at next_in
	uint64	total_in;	// total number of input bytes read so far
	IOSE	rcode;
};
static inline const void	IOS_copy	(IOSIN* _iosin	,IOS* _ios);
static inline const IOSE&	IOS_copy	(IOS*	_ios	,const IOSIN& _iosin);
static inline const void	IOS_hdcopy	(IOS*	_ios	,const IOSIN& _iosin);
static inline const IOSE&	IOS_update	(IOSIN* _iosin	,const IOSE& _rcode);
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
		inline	virtual	ADSF&	_SetOut			(const UVOut& _out);
		inline	virtual	ADSF&	_SetIOS			(IOS* _ios);
	protected:
		inline	virtual	IOSE	DoTransform		(IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
		inline	virtual	IOSE	DoFinal			(IOS* _ios,const UVOut& _out);
	public:
		inline			IOSE	Transform		(IOS* _ios,const UVIn& _in);
		inline			IOSE	Transform		(IOS* _ios,const uint8* data,const uint64& length);
		inline			IOSE	Final			(IOS* _ios);
	public:
				virtual	IOSE	_Begin			(IOS* _ios);
				virtual	IOSE	_Endl			(IOS* _ios);
	public:
						IOSE	AllIn			(IOS* _ios,const UVIn&  _in);
		inline			IOSE	AllOut			(IOS* _ios,const UVOut& _out);
		inline			IOSE	FinalOut		(IOS* _ios,const UVOut& _out);
	public:
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
		inline	virtual	TNF*		AppendDownNode		(TNF* pn);
	protected:
		inline	virtual void		ResetPNLength	(void);
	public:
		inline			void		InitPN			(const ARRAY* _out,const ARRAY* _in,uint32 fixedNum);
		inline			void		SetFixedByte	(uint32 fixedNum);
		inline	const	uint32&		GetFixedByte	(void)const;
		inline			void		SyncPosRE		(const ADS_POSITION* pos);
		inline			void		SyncPosWR		(const ADS_POSITION* pos);
		inline	const	uint32&		GetOffsetWR		(void)const;
		inline	const	uint32&		GetLengthWR		(void)const;
		inline	const	uint32&		GetOffsetRE		(void)const;
		inline	const	uint32&		GetLengthRE		(void)const;
		inline	const	void		SetOffsetRE		(uint32 offset);
		inline	const	void		SetLengthRE		(uint32 length);
		inline			uint32		OutRE			(void);
		inline			uint32		OutWR			(void);
	public:
		inline	virtual	void		ResetPosWR		(void);
		inline	virtual	void		ResetPosRE		(void);
				virtual	bool32		Analysis		(uint32 startOffset);
		inline			IOSE		FinalRead		(IOS* _ios,const UVOut& _out);
		inline	virtual	IOSE		Read			(IOS* _ios,const UVOut& _out);
		inline	virtual	IOSE		Write			(IOS* _ios,const UVIn&  _in);
		inline	virtual	IOSE		WriteNone		(void);
		inline	virtual	void		CleanWR			(void);
		inline	virtual	void		CleanRE			(void);
};
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
		inline		G_ENDIAN	GetEndianType	(void);
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
		inline			uint32	GetMaxValue		(void)const;
		inline			bool32	SetMaxValueWR	(void)const;
		inline			bool32	SetMaxValueRE	(void)const;
		inline			bool32	IsMaxValueWR	(void)const;
		inline			bool32	IsMaxValueRE	(void)const;
	protected:
						IOSE	SetByte1		(IOS* _ios,uint32 data);
						IOSE	SetByte2		(IOS* _ios,uint32 data);
						IOSE	SetByte3		(IOS* _ios,uint32 data);
						IOSE	SetByte4		(IOS* _ios,uint32 data);
	protected:
						IOSE	UpdateByte1		(uint32 data);
						IOSE	UpdateByte2		(uint32 data);
						IOSE	UpdateByte3		(uint32 data);
						IOSE	UpdateByte4		(uint32 data);
	protected:
						IOSE	SetByte			(IOS* _ios,uint32 data);
						IOSE	UpdateByte		(uint32 data);
	public:
		inline			IOSE	Update			(uint32 data);
						IOSE	Write			(IOS* _ios,uint32 data);
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
				virtual	bool32		Analysis	(uint32 startOffset);
		inline			IOSE		Write		(IOS* _ios,uint32 data);
		inline	const	PNF_MASK& operator =	(uint32 data);
};
//------------------------------------------------------------------------------------------//
typedef PNF_VAR PNF_ADDR;
typedef PNF_VAR PNF_CTRL;
typedef PNF_VAR PNF_LEN;
typedef PNF_VAR PNF_CRC;
//------------------------------------------------------------------------------------------//
class PNF_HEAD : public PNF{
	public:
				 PNF_HEAD(void);
		virtual ~PNF_HEAD(void){;};
	private:
		STDSTR			cgFixedStr;
	public:
		inline			void		InitPN			(const ARRAY* _out,const ARRAY* _in,const STDSTR& fixedStr);
	private:
						bool32		CheckFixedStr	(void)const;
	public:
				virtual	bool32		Analysis		(uint32 startOffset);
						IOSE		Write			(IOS* _ios);
};
//------------------------------------------------------------------------------------------//
class PNF_TAIL : public PNF_HEAD{
	public:
				 PNF_TAIL(void) : PNF_HEAD(){;};
		virtual ~PNF_TAIL(void){;};
	public:
		inline			void		InitPN			(const ARRAY* _out,const ARRAY* _in,const STDSTR& fixedStr,PNF* pnContent = nullptr);
				virtual	bool32		Analysis		(uint32 startOffset);
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
		inline	virtual	TNF*		AppendDownNode		(TNF *pn);
	public:
		inline			void		InitPN			(const ARRAY* _out,const ARRAY* _in,const PNF* pn = nullptr);
		inline			void		SetBlockPoint	(const PNF* pn);
	protected:
		inline	virtual	IOSE		DoTransform		(IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
		inline	virtual bool32		ChecksumResult	(void)const;
	public:
				virtual	bool32		Analysis		(uint32 startOffset);
		inline	virtual	IOSE		Read			(IOS* _ios,const UVOut& _out);
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
				virtual IOSE		_Begin				(IOS* _ios);
				virtual	IOSE		_Endl				(IOS* _ios);
		inline	const	PNFB_LC&	operator =			(const UVIn& _in);
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
	
				virtual	IOSE		_Begin			(IOS* _ios);
				virtual	IOSE		_Endl			(IOS* _ios);
						IOSE		Write			(IOS* _ios,uint32* retNum,const uint8* data,uint32 num);
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
				virtual	IOSE	DoTransform	(IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
				virtual	IOSE	DoFinal		(IOS* _ios,const UVOut& _out);
				virtual	void	ResetPosWR	(void);
				virtual	void	ResetPosRE	(void);
	public:
		inline	virtual	IOSE	_Begin		(IOS* _ios);
	public:
				virtual	bool32	Analysis	(uint32 startOffset);
		inline	virtual	IOSE	Read		(IOS* _ios,const UVOut& _out);
	
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
	
				virtual	IOSE		_Begin			(IOS* _ios);
				virtual	IOSE		_Endl			(IOS* _ios);
				virtual	IOSE		Read			(IOS* _ios,const UVOut& _out);
	protected:
		inline	virtual	void		SetChecksum		(void);
						bool32		AnalysisR1		(void);
	public:
						bool32		TryGetFrame		(void);
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
		inline	virtual			void		SetUpName		(const STDSTR& strName);
	public:
								void		InitPN			(const ARRAY* _out,const ARRAY* _in);
		inline	virtual			PNFB_LIST&	InitCFG			(uint32 cfg,const void* par);
	protected:
				virtual			IOSE		DoTransform		(IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
	public:
				virtual			bool32		Analysis		(uint32 startOffset);
				virtual			IOSE		_Begin			(IOS* _ios);
				virtual			IOSE		_Endl			(IOS* _ios);
	public:
								PNFB_LIST&	NewText			(void);
	template <typename... Args> IOSE		Write			(IOS* _ios,const Args&... args);
								uint32		ReadQty			(void)const;
								IOSE		Read			(IOS* _ios,const UVOut& _out,uint32 order);
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
		inline	virtual	void	SetUpName		(const STDSTR& strName);
	public:
						void 	InitPN			(const ARRAY* _out,const ARRAY* _in,uint32 byteID = 1,uint32 byteCRC = 0,G_ENDIAN endian = G_ENDIAN_LITTLE);
		inline	virtual	PNFM&	InitCFG			(uint32 cfg,const void* par);
	public:
				virtual	IOSE	_Begin			(IOS* _ios);
				virtual	IOSE	_Endl			(IOS* _ios);
		inline	friend	PNFM&	operator << 	(PNFM& _pn,const _mID& mID)	{_pn.pnm_ID.Update(mID.value);return(_pn);};
	public:
		inline			uint32	ReadID			(void)const;
		inline			bool32	MarkedRead		(void)const;
		inline			bool32	IsUnread		(void)const;
		inline			IOSE	Write			(IOS* _ios,uint32 mID,const UVIn& _in);
};
//------------------------------------------------------------------------------------------//
#ifdef ALG_DS_CRC
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
		inline	virtual	void	SetUpName		(const STDSTR& strName);
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
#ifdef ALG_DS_CRC
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
	template <typename T_SHELL>	bool32 		SHELL_Test		(IOS* _ios,T_SHELL* _msg,STDSTR* _out,				const STDSTR& _in);
	template <typename T_SHELL>	bool32		SHELL_Test		(IOS* _ios,T_SHELL* _msg,STDSTR* _out,uint32 mID,	const STDSTR& _in);
	
	static 	inline STDSTR	ShowINOUT	(const STDSTR& name,uint64 _in,uint64 _out);
	static	inline STDSTR	ShowINOUT	(IOS* _ios);
	static	inline void		PrintResult	(const STDSTR& name,const STDSTR& method,bool32 blok);
};
//------------------------------------------------------------------------------------------//
#include "ADS_Protocol.hpp"
#endif /* ADS_Protocol_h */
#endif /* ADS_Protocol_h */
#endif /* DS_Transform_h */
