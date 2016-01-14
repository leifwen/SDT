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
#ifndef Comm_BufferH
#define Comm_BufferH
//------------------------------------------------------------------------------------------//
#include "Comm_Tree.h"
#include "Comm_FIFO.h"
//------------------------------------------------------------------------------------------//
class PUB_SBUF : public RTREE_NODE{
	public:
		enum{RFLAG_C = 0, RFLAG_S = RTREE_NODE::RFLAG_S + RTREE_NODE::RFLAG_C};
	public:
				 PUB_SBUF(uint32 tSize) : RTREE_NODE()	{Init(tSize);};
				 PUB_SBUF(void) : RTREE_NODE()			{;};
		virtual ~PUB_SBUF(void){;};
	public:
		FIFO_UINT8	cgBufFIFO;
		inline	uint32			Init			(uint32 tSize)	{return(cgBufFIFO.Init(tSize));};
		inline	const uint32&	BufferMaxSize	(void)const		{return(cgBufFIFO.MaxSize());};
		inline	uint32			Unused			(void)const		{return(cgBufFIFO.Unused());};
		inline	uint32			Used			(void)const		{return(cgBufFIFO.Used());};
		inline	void			Clean			(void)			{cgBufFIFO.Empty();};
	
		uint32	Put				(const	uint8 *dataBuf,uint32 num);
		uint32	Get				(		uint8 *dataBuf,uint32 num);
		uint32	Put				(const	FIFO_UINT8 &dataFIFO,uint32 num,uint32 offset = 0);
		uint32	Get				(		FIFO_UINT8 *dataFIFO,uint32 num);
		uint32	Out				(uint32 num,G_LOCK_VAILD blLock = G_LOCK_ON);
	
		uint32	GetInHEX		(		std::string *strNewData,G_SPACE_VAILD blSpace);
		uint32	GetInASCII		(		std::string *strNewData,G_ESCAPE_VAILD blEscape);
		uint32	PutInHEX		(const	std::string &strNewData);
		uint32	PutInASCII		(const	std::string &strNewData,G_ESCAPE_VAILD blEscape);
};
//------------------------------------------------------------------------------------------//
class PUB_SBUF_LIST : public RTREE_NODE{
	public:
		enum{RFLAG_C = 0, RFLAG_S = RTREE_NODE::RFLAG_S + RTREE_NODE::RFLAG_C};
	public:
				 PUB_SBUF_LIST(void) : RTREE_NODE(){;};
		virtual ~PUB_SBUF_LIST(void){;};
	private:
	public:
		void	LClean			(void)									{RTREE_LChildRChain_T(PUB_SBUF,Clean());};
		void	LPut			(const uint8 *dataBuf,uint32 num)		{RTREE_LChildRChain_T(PUB_SBUF,Put(dataBuf,num));};
		void	LPut			(const FIFO_UINT8 &dataFIFO,uint32 num,uint32 offset = 0)
																{RTREE_LChildRChain_T(PUB_SBUF,Put(dataFIFO,num,offset));};
		void	LPutInHEX		(const std::string &strNewData)			{RTREE_LChildRChain_T(PUB_SBUF,PutInHEX(strNewData));};
		void	LPutInASCII		(const std::string &strNewData,G_ESCAPE_VAILD blEscape)
																{RTREE_LChildRChain_T(PUB_SBUF,PutInASCII(strNewData,blEscape));};
};
//------------------------------------------------------------------------------------------//
/*class PUB_SBUF_LISTEx : public RTREE_NODE{
	public:
		enum{RFLAG_C = 0, RFLAG_S = RTREE_NODE::RFLAG_S + RTREE_NODE::RFLAG_C};
	public:
				 PUB_SBUF_LISTEx(void) : RTREE_NODE(){;};
		virtual ~PUB_SBUF_LISTEx(void){;};
	private:
	public:
		void	SClean			(void)							{RTREE_LChildRChain_E_S(PUB_SBUF,Clean());};
		uint32	SGet			(uint8 *dataBuf,uint32 num)		{RTREE_LChildRChain_E_S_R(PUB_SBUF,Get(dataBuf,num),uint32);};
		uint32	SPut			(const uint8 *dataBuf,uint32 num){RTREE_LChildRChain_E_S_R(PUB_SBUF,Put(dataBuf,num),uint32);};
		uint32	SGetInHEX		(std::string *strNewData,G_SPACE_VAILD blSpace)
																{RTREE_LChildRChain_E_S_R(PUB_SBUF,GetInHEX(strNewData,blSpace),uint32);};
		uint32	SGetInASCII	(std::string *strNewData,G_ESCAPE_VAILD blEscape)
																{RTREE_LChildRChain_E_S_R(PUB_SBUF,GetInASCII(strNewData,blEscape),uint32);};
		uint32	SPutInHEX		(const std::string &strNewData)	{RTREE_LChildRChain_E_S_R(PUB_SBUF,PutInHEX(strNewData),uint32);};
		uint32	SPutInASCII	(const std::string &strNewData,G_ESCAPE_VAILD blEscape)
																{RTREE_LChildRChain_E_S_R(PUB_SBUF,PutInASCII(strNewData,blEscape),uint32);};
		uint32	SBufferMaxSize	(void)							{RTREE_LChildRChain_E_S_R(PUB_SBUF,BufferMaxSize(),uint32);};
		uint32	SUnused			(void)							{RTREE_LChildRChain_E_S_R(PUB_SBUF,Unused(),uint32);};
		uint32	SUsed			(void)							{RTREE_LChildRChain_E_S_R(PUB_SBUF,Used(),uint32);};
		uint64	STotalIn		(void)							{RTREE_LChildRChain_E_S_R(PUB_SBUF,TotalIn(),uint64);};
		uint64	STotalOut		(void)							{RTREE_LChildRChain_E_S_R(PUB_SBUF,TotalOut(),uint64);};
};*/
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class PUB_DBUF : public RTREE_NODE{
	public:
		enum{RFLAG_C = 0, RFLAG_S = RTREE_NODE::RFLAG_S + RTREE_NODE::RFLAG_C};
	public:
				 PUB_DBUF(uint32 tSize) : RTREE_NODE(){Init(tSize);};
		virtual	~PUB_DBUF(void){;};
	private:
		int32		Init	(uint32 tSize);
	protected:
		PUB_SBUF	cgRxBuffer;
		PUB_SBUF	cgTxBuffer;
		uint64		cgRxBytes;
		uint64		cgTxBytes;
		uint64		cgFwBytes;
	public:
		virtual	void	Enable	(void){cgRxBuffer.Enable();cgTxBuffer.Enable();RTREE_NODE::Enable();};
		virtual	void	Disable	(void){cgRxBuffer.Disable();cgTxBuffer.Disable();RTREE_NODE::Disable();};
		virtual	void	Clean	(void);
		inline	const uint32&	BufferRxMaxSize	(void)const	{return(cgRxBuffer.BufferMaxSize());};
		inline	const uint32&	BufferTxMaxSize	(void)const	{return(cgRxBuffer.BufferMaxSize());};
		inline	uint32			UnreadBytes		(void)const	{return(cgRxBuffer.Used());};
		inline	uint32			UnsentBytes		(void)const	{return(cgTxBuffer.Used());};
		inline	const uint64&	FwBytes			(void)const	{return(cgFwBytes);};
		inline	const uint64&	RxBytes			(void)const	{return(cgRxBytes);};
		inline	const uint64&	TxBytes			(void)const	{return(cgTxBytes);};

	
		inline	uint32	Read			(uint8 *dataBuf,uint32 num)						{return(cgRxBuffer.Get(dataBuf,num));};
		inline	uint32	Write			(const uint8 *dataBuf,uint32 num)				{return(cgTxBuffer.Put(dataBuf,num));};
		inline	uint32	Write			(const FIFO_UINT8 &dataFIFO,uint32 num,uint32 offset = 0)
																						{return(cgTxBuffer.Put(dataFIFO,num,offset));};
		inline	uint32	ReadInHEX		(std::string *strData,G_SPACE_VAILD blSpace)	{return(cgRxBuffer.GetInHEX(strData,blSpace));};
		inline	uint32	ReadInASCII		(std::string *strData,G_ESCAPE_VAILD blEscape)	{return(cgRxBuffer.GetInASCII(strData,blEscape));};
		inline	uint32	WriteInHEX		(const std::string &strNewData)					{return(cgTxBuffer.PutInHEX(strNewData));};
		inline	uint32	WriteInASCII	(const std::string &strNewData,G_ESCAPE_VAILD blEscape)	{return(cgTxBuffer.PutInASCII(strNewData,blEscape));};
};
//------------------------------------------------------------------------------------------//
class PUB_DBUF_LIST : public RTREE_NODE{
	public:
		enum{RFLAG_C = 0, RFLAG_S = RTREE_NODE::RFLAG_S + RTREE_NODE::RFLAG_C};
	public:
		 		 PUB_DBUF_LIST(void) : RTREE_NODE(){;};
		virtual	~PUB_DBUF_LIST(void){;};
	private:
	public:
		void	LClean			(void)							{RTREE_LChildRChain_T(PUB_DBUF,Clean());};
		void	LWrite			(const uint8 *dataBuf,uint32 num){RTREE_LChildRChain_T(PUB_DBUF,Write(dataBuf,num));};
		void	LWriteInHEX		(const std::string &strNewData)	{RTREE_LChildRChain_T(PUB_DBUF,WriteInHEX(strNewData));};
		void	LWriteInASCII	(const std::string &strNewData,G_ESCAPE_VAILD blEscape)
																{RTREE_LChildRChain_T(PUB_DBUF,WriteInASCII(strNewData,blEscape));};
	public:
		void	SClean			(void)							{RTREE_LChildRChain_E_S(PUB_DBUF,Clean());};
		uint32	SRead			(uint8 *dataBuf,uint32 num)		{RTREE_LChildRChain_E_S_R(PUB_DBUF,Read(dataBuf,num),uint32);};
		uint32	SWrite			(const uint8 *dataBuf,uint32 num){RTREE_LChildRChain_E_S_R(PUB_DBUF,Write(dataBuf,num),uint32);};
		uint32	SReadInHEX		(std::string *strNewData,G_SPACE_VAILD blSpace)
																{RTREE_LChildRChain_E_S_R(PUB_DBUF,ReadInHEX(strNewData,blSpace),uint32);};
		uint32	SReadInASCII	(std::string *strNewData,G_ESCAPE_VAILD blEscape)
																{RTREE_LChildRChain_E_S_R(PUB_DBUF,ReadInASCII(strNewData,blEscape),uint32);};
		uint32	SWriteInHEX		(const std::string &strNewData)	{RTREE_LChildRChain_E_S_R(PUB_DBUF,WriteInHEX(strNewData),uint32);};
		uint32	SWriteInASCII	(const std::string &strNewData,G_ESCAPE_VAILD blEscape)
																{RTREE_LChildRChain_E_S_R(PUB_DBUF,WriteInASCII(strNewData,blEscape),uint32);};

		uint32	SBufferRxMaxSize(void){RTREE_LChildRChain_E_S_R(PUB_DBUF,BufferRxMaxSize(),uint32);};
		uint32	SBufferTxMaxSize(void){RTREE_LChildRChain_E_S_R(PUB_DBUF,BufferTxMaxSize(),uint32);};
		uint32	SUnreadBytes	(void){RTREE_LChildRChain_E_S_R(PUB_DBUF,UnreadBytes(),uint32);};
		uint32	SUnsentBytes	(void){RTREE_LChildRChain_E_S_R(PUB_DBUF,UnsentBytes(),uint32);};
		uint64	SFwBytes		(void){RTREE_LChildRChain_E_S_R(PUB_DBUF,FwBytes(),uint64);};
		uint64	SRxBytes		(void){RTREE_LChildRChain_E_S_R(PUB_DBUF,RxBytes(),uint64);};
		uint64	STxBytes		(void){RTREE_LChildRChain_E_S_R(PUB_DBUF,TxBytes(),uint64);};
};
//------------------------------------------------------------------------------------------//
#endif