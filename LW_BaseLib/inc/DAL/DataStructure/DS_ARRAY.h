//
//  DS_ARRAY.h
//  SDT
//
//  Created by Leif Wen on 18/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#ifndef DS_ARRAY_h
#define DS_ARRAY_h
#ifdef DS_ARRAY_h
//------------------------------------------------------------------------------------------//
#include "BasicDefine.h"
//------------------------------------------------------------------------------------------//
template <typename DS_T> class DS_ARRAY_FRAME{
	public:
		enum	{DSAF_blNoMem = 0, DSAF_blNormal};
	public:
				 DS_ARRAY_FRAME(void);
		virtual ~DS_ARRAY_FRAME(void);
	protected:
		DS_T*		cgDataBuffer;
		uint32		maxSize;
		uint32		offsetIn;
		uint32		offsetOut;
		uint32		offsetPreIn;
		uint32		countPreIn;
		uint32		status;
	public:
						uint32	InitSize		(uint32 size);
		inline			void	Empty			(void);
		inline			void	Reset			(void);
		inline			uint32	Unused			(void)const;
		inline			uint32	Used			(void)const;
		inline	const	uint32&	MaxSize			(void)const;
		inline			bool32	IsEmpty			(void)const;
		inline			bool32	IsFull			(void)const;
		inline			bool32	IsNoMEM			(void)const;
	public:
				uint32	Prepare_Set				(void);
				uint32	Prepare_Clr				(bool32 blGiveUp = G_FALSE);
				bool32	IsInPrepare				(void);
		inline	uint32	GetOffsetPreIn			(void)const;
		inline	uint32	GetPreInNum				(void)const;
		inline	DS_T*	GetPointer				(uint32 offset = 0)const;
	public:
				uint32	CalcOutLength			(uint32& fLength,uint32& offset)const;
				uint32	CalcInLength			(uint32& fLength,uint32& offset)const;
				uint32	CalcOutCapacity			(uint32& fLength,uint32& offset)const;
				uint32	CalcInCapacity			(uint32& fLength,uint32& offset)const;
				uint32	CalcInCapacityAbsolute	(uint32& fLength,uint32& offset)const;
				uint32	CalcOutLengthInToPre	(uint32& fLength,uint32& offset)const;
				uint32	CalcOutCapacityInToPre	(uint32& fLength,uint32& offset)const;
	private:
		virtual	uint32	Copy					(DS_T*& to_addr,uint32& to_length,const DS_T*& in_addr,uint32& in_length)const;
	public:
				uint32	Put						(const	DS_T* data,uint32 num);
		inline	uint32	Put						(const	DS_T& data);
				uint32	Get						(		DS_T* dataOut,uint32 num);
				uint32	Read					(		DS_T* dataOut,uint32 num,uint32 offset = 0)const;
	
				uint32	Put						(const	DS_ARRAY_FRAME<DS_T>& _in ,uint32 num,uint32 offset = 0);
		inline	uint32	Get						(		DS_ARRAY_FRAME<DS_T>* _out,uint32 num);
		inline	uint32	Read					(		DS_ARRAY_FRAME<DS_T>* _out,uint32 num,uint32 offset = 0)const;
	
		uint32	UpdateByOffsetOut				(const	DS_T* data ,uint32 num,uint32 offset = 0);
		uint32	UpdateByOffsetIn				(const	DS_T* data ,uint32 num,uint32 offset = 0);
		uint32	ReadByOffsetIn					(		DS_T* data ,uint32 num,uint32 offset = 0);
	
		uint32	Out								(uint32 num);
		uint32	In								(uint32 num);
		uint32	Out_RollBack					(uint32 num);
		uint32	In_RollBack						(uint32 num);
};
//------------------------------------------------------------------------------------------/
class ARRAY : public DS_ARRAY_FRAME<uint8>{
	public:
				 ARRAY(void);
		virtual ~ARRAY(void){;};
	public:
		uint32	alreadyReadNum;
	protected:
				uint32	Copy		(uint8*& to_addr,uint32& to_length,const uint8*& in_addr,uint32& in_length) const;
	public:
		inline	uint32	Put			(const uint8* data ,uint32 num);
		inline	uint32	Put			(const uint8& data);
		inline	uint32	Put			(const char* data);
	
		inline	uint32	Get			(uint8* dataOut,uint32 num);
		inline	uint32	Read		(uint8* dataOut,uint32 num,uint32 offset = 0)const;
	
		inline	uint32	Put			(const	ARRAY& _in, uint32 num,uint32 offset = 0);
		inline	uint32	Get			(		ARRAY* _out,uint32 num);
		inline	uint32	Read		(		ARRAY* _out,uint32 num,uint32 offset = 0)const;
	public:
				void	FillZero	(void);
	
				uint32	PutInEscape	(const	uint8* data,   uint32 num);
				uint32	ReadInEscape(		uint8* dataOut,uint32 num,uint32 offset = 0)const;
		
				uint32	PutInHEX	(const uint8* data,uint32 num);//put STDSTR to FIFO,input in HEX mode.
	public:
				uint32	Put			(const STDSTR& strIn,G_ESCAPE blEscape);
		inline	uint32	PutInHEX	(const STDSTR& strIn);
		
		const	STDSTR&	Get			(STDSTR* _out,uint32 num);
		const	STDSTR&	Read		(STDSTR* _out,uint32 num,uint32 offset = 0)const;
	
		const	STDSTR&	GetInASCII	(STDSTR* _out,uint32 num);
		const	STDSTR&	ReadInASCII	(STDSTR* _out,uint32 num,uint32 offset = 0)const;
		
		const	STDSTR&	GetInHEX	(STDSTR* _out,uint32 num);//output as STDSTR,HEX mode without space.
		const	STDSTR&	ReadInHEX	(STDSTR* _out,uint32 num,uint32 offset = 0)const;//output as STDSTR,HEX mode with space.
		const	STDSTR&	GetInHEX_S	(STDSTR* _out,uint32 num);//output as STDSTR,HEX mode without space.
		const	STDSTR&	ReadInHEX_S	(STDSTR* _out,uint32 num,uint32 offset = 0)const;//output as STDSTR,HEX mode with space.
		
		const	STDSTR&	GetInLine	(STDSTR* _out,uint8 endChar = '\n');//output as STDSTR,original data.
		const	STDSTR&	ReadInLine	(STDSTR* _out,uint8 endChar = '\n',uint32 offset = 0)const;//output as STDSTR,original data.
};
//------------------------------------------------------------------------------------------//
static inline	ARRAY*		_EMPTY		(ARRAY* p)	{p->Reset();return(p);};
//------------------------------------------------------------------------------------------//
#include "DS_ARRAY.hpp"
#endif /* DS_ARRAY_h */
#endif /* DS_ARRAY_h */
