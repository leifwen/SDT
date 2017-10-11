/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: Comm_Buffer.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2012.02.09
 * @2013.6.30 :	change to Liunx style
 				add init method
 				use int32 to replace bool
 				use int32 to replace uint32
*/

//------------------------------------------------------------------------------------------//
#include "Comm_FIFO.h"
#include "Comm_Tree.h"
#include "Comm_PA.h"
//------------------------------------------------------------------------------------------//
#if defined Comm_FIFOH && defined Comm_TreeH
#ifndef Comm_BufferH
#define Comm_BufferH
#ifdef Comm_BufferH
//------------------------------------------------------------------------------------------//
class SBUFFER : public TREE_NODE{
	public:
				 SBUFFER(void) : TREE_NODE(){;};
				 SBUFFER(uint32 tSize) : TREE_NODE(){Init(tSize);};
		virtual ~SBUFFER(void){;};
	public:
		inline void		InLock_set(G_LOCK blLock = G_LOCK_ON){Spin_InUse_set(blLock);};
		inline void		InLock_clr(G_LOCK blLock = G_LOCK_ON){Spin_InUse_clr(blLock);};
		inline int32	InLock_try(G_LOCK blLock = G_LOCK_ON){return(Spin_InUse_try(blLock));};
	
		inline void		OutLock_set(G_LOCK blLock = G_LOCK_ON){InDoing_set(blLock);};
		inline void		OutLock_clr(G_LOCK blLock = G_LOCK_ON){InDoing_clr(blLock);};
		inline int32	OutLock_try(G_LOCK blLock = G_LOCK_ON){return(InDoing_try(blLock));};
	public:
		FIFO8	cgFIFO;
		uint32	cgAlreadyReadNum;//used for already read num record of input fifo
	public:
		inline	uint32			Init			(uint32 tSize)	{cgAlreadyReadNum = 0;return(cgFIFO.Init(tSize));};
		inline	const uint32&	BufferMaxSize	(void)const		{return(cgFIFO.MaxSize());};
		inline	uint32			Unused			(void)const		{return(cgFIFO.Unused());};
		inline	uint32			Used			(void)const		{return(cgFIFO.Used());};
	public:
		void	Clean	(G_LOCK blLock = G_LOCK_OFF);
		void	Out		(uint32 num,G_LOCK blLock = G_LOCK_OFF);
	public:
			#ifdef Comm_PAH
				uint32	Put			(const PROTOCOL_NODE &pnIn							,G_LOCK blLock = G_LOCK_OFF);
			#endif
				uint32	Put			(const FIFO8 &fifoIn,uint32 num,uint32 offset = 0	,G_LOCK blLock = G_LOCK_OFF);
				uint32	Put			(const uint8 *data,uint32 num						,G_LOCK blLock = G_LOCK_OFF);
				uint32	Put			(const uint8 data									,G_LOCK blLock = G_LOCK_OFF){return(Put(&data,1,blLock));};
				uint32	Put			(const STDSTR &strIn,G_ESCAPE blEscape				,G_LOCK blLock = G_LOCK_OFF);
				uint32	PutInHEX	(const STDSTR &strIn								,G_LOCK blLock = G_LOCK_OFF);
	
				uint32	Get			(FIFO8 *fifoOut,uint32 num,G_LOCK blLock = G_LOCK_OFF);
				uint32	Get			(uint8 *dataOut	,uint32 num,G_LOCK blLock = G_LOCK_OFF);
		const STDSTR&	Get			(STDSTR *retStr,G_ESCAPE blEscape,G_LOCK blLock = G_LOCK_OFF);
		const STDSTR&	GetInHEX	(STDSTR *retStr,G_SPACE blSpace,G_LOCK blLock = G_LOCK_OFF);
	public:
		inline SBUFFER& operator << (std::stringstream &streamIn)		{cgFIFO << streamIn;			return(*this);};
	#ifdef Comm_PAH
		inline SBUFFER& operator << (const PROTOCOL_NODE &pnIn)			{pnIn.ReadAllContent(&cgFIFO);	return(*this);};
	#endif
		inline SBUFFER& operator << (const _Data<const FIFO8*> &data)	{cgFIFO << data;				return(*this);};
		inline SBUFFER& operator << (const _Data<const uint8*> &data)	{cgFIFO << data;				return(*this);};
		inline SBUFFER& operator << (const _Data<const char*> &data)	{cgFIFO << data;				return(*this);};
		inline SBUFFER& operator << (const STDSTR &strIn)				{cgFIFO << strIn;				return(*this);};
		inline SBUFFER& operator << (const char *data)					{cgFIFO << data;				return(*this);};
	
		inline SBUFFER& operator >> (std::stringstream *streamIn)		{cgFIFO >> streamIn;			return(*this);};
		inline SBUFFER& operator >> (const _Data<FIFO8*> &data)			{cgFIFO >> data;				return(*this);};
		inline SBUFFER& operator >> (const _Data<uint8*> &data)			{cgFIFO >> data;				return(*this);};
		inline SBUFFER& operator >> (const _Data<char*> &data)			{cgFIFO >> data;				return(*this);};
		inline SBUFFER& operator >> (STDSTR *strOutput)					{cgFIFO >> strOutput;			return(*this);};	
};
//------------------------------------------------------------------------------------------//
class SBUF_LIST : public TREE_NODE{
	public:
				 SBUF_LIST(void) : TREE_NODE(){;};         
		virtual ~SBUF_LIST(void){;};
	private:
	public:
		void	LClean		(G_LOCK blLock = G_LOCK_OFF)												{TREE_LChildRChain_T(SBUFFER,Clean(blLock));};
	#ifdef Comm_PAH
		void	LPut		(const PROTOCOL_NODE &pnIn,G_LOCK blLock = G_LOCK_OFF)						{TREE_LChildRChain_T(SBUFFER,Put(pnIn,blLock));};
	#endif
		void	LPut		(const FIFO8 &fifoIn,uint32 num,uint32 offset = 0,G_LOCK blLock = G_LOCK_OFF){TREE_LChildRChain_T(SBUFFER,Put(fifoIn,num,offset,blLock));};
		void	LPut		(const uint8 *data,uint32 num,G_LOCK blLock = G_LOCK_OFF)					{TREE_LChildRChain_T(SBUFFER,Put(data,num,blLock));};
		void	LPut		(const STDSTR &strIn,G_ESCAPE blEscape,G_LOCK blLock = G_LOCK_OFF)			{TREE_LChildRChain_T(SBUFFER,Put(strIn,blEscape,blLock));};
		void	LPutInHEX	(const STDSTR &strIn,G_LOCK blLock = G_LOCK_OFF)							{TREE_LChildRChain_T(SBUFFER,PutInHEX(strIn,blLock));};
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class DBUFFER_FRAME : public TREE_NODE{
	public:
				 DBUFFER_FRAME(void) : TREE_NODE(){;};
				 DBUFFER_FRAME(uint32 tSize) : TREE_NODE(){Init(tSize);};
		virtual	~DBUFFER_FRAME(void){;};
	public:
		void		Init	(uint32 tSize){
			cgBufMaxSize = tSize;
			cgRxSBUF.Init(tSize);
			cgTxSBUF.Init(tSize);
			cgRxBytes = 0;
			cgTxBytes = 0;
			cgFwBytes = 0;
		};
	protected:
		uint32		cgBufMaxSize;
		SBUFFER		cgRxSBUF;
		SBUFFER		cgTxSBUF;
		uint64		cgRxBytes;
		uint64		cgTxBytes;
		uint64		cgFwBytes;
	public:
		virtual	void	Enable		(void){cgRxSBUF.Enable();cgTxSBUF.Enable();TREE_NODE::Enable();};
		virtual	void	Disable		(void){cgRxSBUF.Disable();cgTxSBUF.Disable();TREE_NODE::Disable();};
		virtual	void	Clean		(void){
			cgRxBytes = 0;
			cgTxBytes = 0;
			cgFwBytes = 0;
			cgRxSBUF.Clean(G_LOCK_OFF);
			cgTxSBUF.Clean(G_LOCK_OFF);
		};
		inline	const uint32&	RxSUFMaxSize(void)const	{return(cgRxSBUF.BufferMaxSize());};
		inline	const uint32&	TxSUFMaxSize(void)const	{return(cgTxSBUF.BufferMaxSize());};
		inline	uint32			UnreadBytes	(void)const	{return(cgRxSBUF.Used());};
		inline	uint32			UnsentBytes	(void)const	{return(cgTxSBUF.Used());};
		inline	const uint64&	FwBytes		(void)const	{return(cgFwBytes);};
		inline	const uint64&	RxBytes		(void)const	{return(cgRxBytes);};
		inline	const uint64&	TxBytes		(void)const	{return(cgTxBytes);};

	protected://cgTxSBUF: mutli in and one out
	#ifdef Comm_PAH
		inline			uint32	TxPut		(const PROTOCOL_NODE &pnIn)							{return(cgTxSBUF.Put(pnIn				,G_LOCK_ON));};
	#endif
		inline			uint32	TxPut		(const FIFO8 &fifoIn,uint32 num,uint32 offset = 0)	{return(cgTxSBUF.Put(fifoIn,num,offset	,G_LOCK_ON));};
		inline			uint32	TxPut		(const uint8 *data,uint32 num)						{return(cgTxSBUF.Put(data,num			,G_LOCK_ON));};
		inline			uint32	TxPut		(const STDSTR &strIn,G_ESCAPE blEscape)				{return(cgTxSBUF.Put(strIn,blEscape		,G_LOCK_ON));};
		inline			uint32	TxPutInHEX	(const STDSTR &strIn)								{return(cgTxSBUF.PutInHEX(strIn			,G_LOCK_ON));};
	
		inline			uint32	TxGet		(FIFO8 *fifoOut,uint32 num)							{return(cgTxSBUF.Get(fifoOut,num		,G_LOCK_OFF));};
		inline			uint32	TxGet		(uint8 *dataOut,uint32 num)							{return(cgTxSBUF.Get(dataOut,num		,G_LOCK_OFF));};
		inline	const	STDSTR&	TxGet		(STDSTR *retStr,G_ESCAPE blEscape)					{return(cgTxSBUF.Get(retStr	,blEscape	,G_LOCK_OFF));};
		inline	const	STDSTR&	TxGetInHEX	(STDSTR *retStr,G_SPACE blSpace)					{return(cgTxSBUF.GetInHEX(retStr,blSpace,G_LOCK_OFF));};
		inline			void	TxOut		(uint32 num)										{cgTxSBUF.Out(num,G_LOCK_OFF);};
	protected://cgRxSBUF: one in and one out
	#ifdef Comm_PAH
		inline			uint32	RxPut		(const PROTOCOL_NODE &pnIn)							{return(cgRxSBUF.Put(pnIn				,G_LOCK_OFF));};
	#endif
		inline			uint32	RxPut		(const FIFO8 &fifoIn,uint32 num,uint32 offset = 0)	{return(cgRxSBUF.Put(fifoIn,num,offset	,G_LOCK_OFF));};
		inline			uint32	RxPut		(const uint8 *data,uint32 num)						{return(cgRxSBUF.Put(data,num			,G_LOCK_OFF));};
		inline			uint32	RxPut		(const STDSTR &strIn,G_ESCAPE blEscape)				{return(cgRxSBUF.Put(strIn,blEscape		,G_LOCK_OFF));};
		inline			uint32	RxPutInHEX	(const STDSTR &strIn)								{return(cgRxSBUF.PutInHEX(strIn			,G_LOCK_OFF));};
	
		inline			uint32	RxGet		(FIFO8 *fifoOut,uint32 num)				{return(cgRxSBUF.Get(fifoOut,num		,G_LOCK_OFF));};
		inline			uint32	RxGet		(uint8 *dataOut,uint32 num)				{return(cgRxSBUF.Get(dataOut,num		,G_LOCK_OFF));};
		inline	const	STDSTR&	RxGet		(STDSTR *retStr,G_ESCAPE blEscape)		{return(cgRxSBUF.Get(retStr	,blEscape	,G_LOCK_OFF));};
		inline	const	STDSTR&	RxGetInHEX	(STDSTR *retStr,G_SPACE blSpace)		{return(cgRxSBUF.GetInHEX(retStr,blSpace,G_LOCK_OFF));};
		inline			void	RxOut		(uint32 num)							{cgRxSBUF.Out(num,G_LOCK_OFF);};
	public:
		virtual			uint32	Read		(FIFO8 *fifoOut,uint32 num)				{return(RxGet(fifoOut,num));};
		virtual			uint32	Read		(uint8 *dataOut,uint32 num)				{return(RxGet(dataOut,num));};
		virtual	const	STDSTR&	Read		(STDSTR *retStr,G_ESCAPE blEscape)		{return(RxGet(retStr,blEscape));};
		virtual	const	STDSTR&	ReadInHEX	(STDSTR *retStr,G_SPACE blSpace)		{return(RxGetInHEX(retStr,blSpace));};
	public:
	#ifdef Comm_PAH
		virtual	uint32	Send		(const PROTOCOL_NODE &pnIn)						{return(TxPut(pnIn));};
	#endif
		virtual	uint32	Send		(const FIFO8 &fifoIn,uint32 num,uint32 offset)	{return(TxPut(fifoIn,num,offset));};
		virtual	uint32	Send		(const uint8 *data,uint32 num)					{return(TxPut(data,num));};
		virtual	uint32	Send		(const uint8 data)								{return(Send(&data,1));};
		virtual	uint32	Send		(const STDSTR &strIn,G_ESCAPE blEscape)			{return(TxPut(strIn,blEscape));};
		virtual	uint32	SendInHEX	(const STDSTR &strIn)							{return(TxPutInHEX(strIn));};
};
//------------------------------------------------------------------------------------------//
class DBUFFER : public DBUFFER_FRAME{
	public:
				 DBUFFER(void) : DBUFFER_FRAME(){;};
				 DBUFFER(uint32 tSize) : DBUFFER_FRAME(){Init(tSize);};
		virtual	~DBUFFER(void){;};
	public:
		void		Init	(uint32 tSize);
	private:
		DBUFFER		*cgSelDB;
	public:
				DBUFFER*	GetNewSon	(void){return(AddSon(static_cast<DBUFFER*>(GetNewNode())));};
		inline	DBUFFER		*GetFDB		(void){return(static_cast<DBUFFER*>(GetcgUp(this)));};
		inline	DBUFFER		*GetSelDB	(void){return(cgSelDB);};
		virtual	void		RemoveSelf	(void);
				DBUFFER		*AddSon		(DBUFFER *sdb);
				void		JoinTo		(DBUFFER *fdb);
		virtual	void		ChildSetSel	(DBUFFER *sdb);
		virtual	void		ChildClrSel	(DBUFFER *sdb);
		virtual	void		SetblSelected(void);
		virtual	void		ClrblSelected(void);
	public:
		inline			uint32	SRead		(FIFO8 *fifoOut,uint32 num){
			uint32 ret;
			Spin_InUse_set();
			ret = cgSelDB->Read(fifoOut,num);
			Spin_InUse_clr();
			return(ret);
		};
		inline			uint32	SRead		(uint8 *dataOut,uint32 num){
			uint32 ret;
			Spin_InUse_set();
			ret = cgSelDB->Read(dataOut,num);
			Spin_InUse_clr();
			return(ret);
		};
		inline	const	STDSTR&	SRead		(STDSTR *retStr,G_ESCAPE blEscape){
			Spin_InUse_set();
			cgSelDB->Read(retStr,blEscape);
			Spin_InUse_clr();
			return(*retStr);
		};
		inline	const	STDSTR&	SReadInHEX	(STDSTR *retStr,G_SPACE blSpace){
			Spin_InUse_set();
			cgSelDB->ReadInHEX(retStr,blSpace);
			Spin_InUse_clr();
			return(*retStr);
		};
	public:
	#ifdef Comm_PAH
		inline	uint32	SSend		(const PROTOCOL_NODE &pnIn){
			uint32 ret;
			Spin_InUse_set();
			ret = cgSelDB->Send(pnIn);
			Spin_InUse_clr();
			return(ret);
		};
	#endif
		inline	uint32	SSend		(const FIFO8 &fifoIn,uint32 num,uint32 offset){
			uint32 ret;
			Spin_InUse_set();
			ret = cgSelDB->Send(fifoIn,num,offset);
			Spin_InUse_clr();
			return(ret);
		};
		inline	uint32	SSend		(const uint8 *data,uint32 num){
			uint32 ret;
			Spin_InUse_set();
			ret = cgSelDB->Send(data,num);
			Spin_InUse_clr();
			return(ret);
		};
		inline	uint32	SSend		(const uint8 data){
			uint32 ret;
			Spin_InUse_set();
			ret = cgSelDB->Send(data);
			Spin_InUse_clr();
			return(ret);
		};
		inline	uint32	SSend		(const STDSTR &strIn,G_ESCAPE blEscape){
			uint32 ret;
			Spin_InUse_set();
			ret = cgSelDB->Send(strIn,blEscape);
			Spin_InUse_clr();
			return(ret);
		};
		inline	uint32	SSendInHEX	(const STDSTR &strIn){
			uint32 ret;
			Spin_InUse_set();
			ret = cgSelDB->SendInHEX(strIn);
			Spin_InUse_clr();
			return(ret);
		};
	public:
		inline void		SClean		(void){
			Spin_InUse_set();
			cgSelDB->Clean();
			Spin_InUse_clr();
		};
		inline uint32	SRxSUFMaxSize(void){
			uint32 num;
			Spin_InUse_set();
			num = cgSelDB->RxSUFMaxSize();
			Spin_InUse_clr();
			return(num);
		};
		inline uint32	STxSUFMaxSize(void){
			uint32 num;
			Spin_InUse_set();
			num = cgSelDB->TxSUFMaxSize();
			Spin_InUse_clr();
			return(num);
		};
		inline	uint32	SUnreadBytes(void){
			uint32 num;
			Spin_InUse_set();
			num = cgSelDB->UnreadBytes();
			Spin_InUse_clr();
			return(num);
		};
		inline	uint32	SUnsentBytes(void){
			uint32 num;
			Spin_InUse_set();
			num = cgSelDB->UnsentBytes();
			Spin_InUse_clr();
			return(num);
		};
		inline uint64	SFwBytes(void){
			uint64 num;
			Spin_InUse_set();
			num = cgSelDB->FwBytes();
			Spin_InUse_clr();
			return(num);
		};
		inline uint64	SRxBytes(void){
			uint64 num;
			Spin_InUse_set();
			num = cgSelDB->RxBytes();
			Spin_InUse_clr();
			return(num);
		};
		inline uint64	STxBytes(void){
			uint64 num;
			Spin_InUse_set();
			num = cgSelDB->TxBytes();
			Spin_InUse_clr();
			return(num);
		};
};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif
