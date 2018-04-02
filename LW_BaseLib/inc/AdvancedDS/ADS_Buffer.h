//
//  ADS_Buffer.h
//  SDT
//
//  Created by Leif Wen on 01/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "DS_Transform.h"
#ifdef DS_Transform_h
//------------------------------------------------------------------------------------------//
#ifndef ADS_Buffer_h
#define ADS_Buffer_h
#ifdef ADS_Buffer_h
//------------------------------------------------------------------------------------------//
class SBUF : public TREE_NODE{
	public:
				 SBUF(void) : TREE_NODE(){;};
				 SBUF(uint32 size);
		virtual ~SBUF(void){;};
	public:
		inline void		InLock_set(G_LOCK blLock = G_LOCK_ON);
		inline void		InLock_clr(G_LOCK blLock = G_LOCK_ON);
		inline bool32	InLock_try(G_LOCK blLock = G_LOCK_ON);
		
		inline void		OutLock_set(G_LOCK blLock = G_LOCK_ON);
		inline void		OutLock_clr(G_LOCK blLock = G_LOCK_ON);
		inline bool32	OutLock_try(G_LOCK blLock = G_LOCK_ON);
	public:
		ARRAY	cgArray;
		uint32	cgAlreadyReadNum;//used for already read bytes record of input array
	public:
		inline			SBUF&	InitSize	(uint32 size);
		inline			void	Empty		(void);
		inline	const	uint32&	MaxSize		(void)const;
		inline			uint32	Unused		(void)const;
		inline			uint32	Used		(void)const;
		inline			bool32	IsEmpty		(void)const;
		inline			bool32	IsFull		(void)const;
	public:
		inline			ARRAY&	GetArray	(void);
						uint32	Put			(IOSTATUS* _ios,const UVIn&  _in,				G_LOCK blLock = G_LOCK_OFF);
						uint32	Get			(IOSTATUS* _ios,const UVOut& _out,	uint32 num,	G_LOCK blLock = G_LOCK_OFF);
						void	Out			(									uint32 num,	G_LOCK blLock = G_LOCK_OFF);
};
//------------------------------------------------------------------------------------------//
class DBUF_FRAME : public TREE_NODE{
	public:
				 DBUF_FRAME(void);
				 DBUF_FRAME(uint32 size);
		virtual	~DBUF_FRAME(void){;};
	protected:
		uint32		cgMaxSize;
		SBUF		cgRxSBUF;
		SBUF		cgTxSBUF;
		uint64		cgRxBytes;
		uint64		cgTxBytes;
		uint64		cgFwBytes;
	public:
				virtual	DBUF_FRAME&	InitSize		(uint32 size);
				virtual	void		Empty			(void);
		inline	const	uint32&		RxSBUFMaxSize	(void)const;
		inline	const	uint32&		TxSBUFMaxSize	(void)const;
		inline			uint32		UnreadBytes		(void)const;
		inline			uint32		UnsentBytes		(void)const;
		inline	const	uint64&		FwBytes			(void)const;
		inline	const	uint64&		RxBytes			(void)const;
		inline	const	uint64&		TxBytes			(void)const;
	public:
		inline			uint32		Put				(IOSTATUS* _ios,const UVIn&  _in);
		inline			uint32		Get				(IOSTATUS* _ios,const UVOut& _out,uint32 num);
};
//------------------------------------------------------------------------------------------//
class DBUF : public DBUF_FRAME{
	public:
				 DBUF(void);
				 DBUF(uint32 size);
		virtual	~DBUF(void){;};
	private:
		DBUF*			cgSelDB;
	public:
		inline			DBUF*	GetNewChild		(void);
		inline			DBUF*	GetFDB			(void);
		inline			DBUF*	GetSelDB		(void);
		inline			DBUF*	AddChild		(DBUF* sdb);
		inline			void	JoinTo			(DBUF* fdb);
				virtual	void	ChildSetSel		(DBUF* sdb);
				virtual	void	ChildClrSel		(DBUF* sdb);
		inline	virtual	void	SetblSelected	(void);
		inline	virtual	void	ClrblSelected	(void);
		inline	virtual	void	RemoveSelf		(void);
	public:
		inline	virtual	uint32	SSend			(const UVIn&  _in);
		inline	virtual	uint32	SRead			(const UVOut& _out,uint32 num);
	public:
		inline			void	SEmpty			(void);
		inline			uint32	SRxSBUFMaxSize	(void);
		inline			uint32	STxSBUFMaxSize	(void);
		inline			uint32	SUnreadBytes	(void);
		inline			uint32	SUnsentBytes	(void);
		inline			uint64	SFwBytes		(void);
		inline			uint64	SRxBytes		(void);
		inline			uint64	STxBytes		(void);
};
//------------------------------------------------------------------------------------------//
#include "ADS_Buffer.hpp"
#endif /* ADS_Buffer_h */
#endif /* ADS_Buffer_h */
#endif /* DS_Transform_h */
