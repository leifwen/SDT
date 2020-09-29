//
//  ADS_SSL.h
//  SDT
//
//  Created by Leif Wen on 26/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#include "ADS_Crypto.h"
#include "DS_Bitmap.h"
#if defined ADS_Crypto_h && defined DS_Bitmap_h
//------------------------------------------------------------------------------------------//
#ifndef ADS_SSL_h
#define ADS_SSL_h
#ifdef ADS_SSL_h
//------------------------------------------------------------------------------------------//
class CSSL_T0 : private DSTF{
	public:
		//CFG_AES_CBC	495/503,505/519,1007/1015,1017/1031
		//CFG_AES_CFB8	495/502,505/512,1007/1014,1017/1024
		enum	{MAX_PACKAGE_DEALYMS = 1024
				,MAX_PACKAGE_SIZE = 1024 / 2
				,MAX_PACKAGE_SIZE_ADJUST_CBC = (512 - 495)
				,MAX_PACKAGE_SIZE_ADJUST_CFB8 = (512 - 505)
				,MAX_PACKAGE_SIZE_ADJUST_T0 = MAX_PACKAGE_SIZE_ADJUST_CBC
		};
	public:
				 CSSL_T0(void);
		virtual	~CSSL_T0(void){;};
	private:
		MESG16_AES			cgMsg;
		uint32				cgDefCFG;
		STDSTR				cgDefKey;
		TNF					cgT1SList;
		DS_SpinLock			cgSendLock;
	private:
		double				cgPackageDlyMS;
		uint32				cgPort;
	public:
				void 		InitPN				(const ARRAY* _out,const ARRAY* _in);
		virtual void		SetSelfName			(const STDSTR& strName);
		virtual	void		SetUpName			(const STDSTR& strName);
	public:
				void		SetConfig			(uint32 cfg,const STDSTR& sKey);
				void		SetKey				(			const STDSTR& sKey);
				void		Reset				(void);
	public:
				void		SetPackageDlyMS		(double dlyMS);
				double		GetPackageDlyMS		(void);
	private:
		virtual	IOSE		DoTransform			(IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
	public:
				void		Register			(TNF* csslT1);
				bool32		Delivery			(void);
				IOSE		Send				(IOS* _ios,uint32 port,const UVIn& _in);
};
//------------------------------------------------------------------------------------------//
struct T1ID{
	uint32 ctrlID;
	uint32 blockID;
	uint32 orderID;
};
//------------------------------------------------------------------------------------------//
T1ID SetT1ID(const uint32& _ctrlID,const uint32& _blockID,const uint32& _orderID);
//------------------------------------------------------------------------------------------//
class CSSL_PNFB_PKG : public PNF_BLOCK{
	public:
		enum	{MAX_PACKAGE_SIZE_ADJUST_T1 = 1 + 1 + 1 + 2};
	public:
				 CSSL_PNFB_PKG(void);
		virtual	~CSSL_PNFB_PKG(void){;};
	private:
		PNF_VAR			pn_CtrlID;
		PNF_VAR			pn_BlockID;
		PNF_VAR			pn_OrderID;
		PNF_SC			pn_Info;
	public:
		CSSL_PNFB_PKG& 	InitPN			(const ARRAY* array);
		CSSL_PNFB_PKG& 	InitPN			(uint32	size);
	public:
		bool32			InvalidMark		(void)const;
		bool32			IsInvalid		(void)const;
		uint32			ReadCtrlID		(void)const;
		uint32			ReadBlockID		(void)const;
		uint32			ReadOrderID		(void)const;
		uint32			MaxBlockID		(void)const;
		uint32			MaxOrderID		(void)const;
	public:
		IOSE			FindPkg			(uint32* cID,uint32  bID,uint32  oID,bool32 blInvalidMark);
		IOSE			FindPkg_maxOID	(uint32* cID,uint32  bID,uint32* oID,bool32 blInvalidMark);
		IOSE			FindPkg_minOID	(uint32* cID,uint32  bID,uint32* oID,bool32 blInvalidMark);
		IOSE			FindPkg_minBID	(uint32* cID,uint32* bID);
		IOSE			FindPkg_first	(uint32* cID,uint32* bID,uint32* oID);
		IOSE			FindPkg_next	(uint32* cID,uint32* bID,uint32* oID);
		IOSE			FindPkg_invalid	(uint32 pkgSize);
	public:
		IOSE			WritePkg		(IOS* _ios,const T1ID& _id,const UVIn& _in);
		IOSE 			ReadPkg			(IOS* _ios,const UVOut& _out,uint32* cID,uint32 bID,uint32 oID,bool32 blInvalidMark);
		IOSE			ReadPkg_maxOID	(IOS* _ios,const UVOut& _out,uint32* cID,uint32  bID,uint32* oID,bool32 blInvalidMark);

		void			CleanInvalidPkg	(void);
		void			CleanPackages	(uint32* cID,uint32 bID);
		void			CleanPackages	(uint32* cID,uint32 bID,uint32 oID);
};
//------------------------------------------------------------------------------------------//
class CSSL_MEMORY : public TNFP{
	public:
		enum	{MAX_PACKAGE_SIZE_ADJUST_MEM = CSSL_T0::MAX_PACKAGE_SIZE_ADJUST_T0 + CSSL_PNFB_PKG::MAX_PACKAGE_SIZE_ADJUST_T1};
	public:
	class PAGENODE : public TNF{
		public:
				 PAGENODE(uint32 size) : TNF(){array.InitSize(size);};
		virtual ~PAGENODE(void){;};
		public:
					ARRAY		array;
					DS_RWLock	rLock;
					DS_SpinLock	wLock;
			virtual	void		RemoveSelf	(void);
	};
	public:
				 CSSL_MEMORY(void);
		virtual ~CSSL_MEMORY(void);
	private:
				uint32				cgPageSize;
				uint32				cgMaxPages;
				uint32				cgPagesCount;
	private:
				CSSL_T0*			cgT0;
				uint32				cgPort;
				CSSL_PNFB_PKG		cgPkgRecT0;
	public:
						void 		Init			(uint32 port,const CSSL_T0* csslT0,uint32 maxPackage,uint32 maxPages);
		virtual 		void		SetSelfName		(const STDSTR& strName);
		virtual			void		SetUpName		(const STDSTR& strName);
				const	uint32&		GetPort			(void)const;
						void		SetPort			(uint32 port);
						void		Empty			(void);
						void		Close			(void);
	private:
		virtual			TNFP*		GetTrash		(void);
		virtual 		TNF*		CreateNode		(void);
	public:
						bool32		AddNewPage		(void);
						void		RemovePage		(PAGENODE* page);
	public:
						void		FindFinish		(PAGENODE* page);
						IOSE		FindPkg			(CSSL_PNFB_PKG* pkg,PAGENODE** retPage,uint32* cID,uint32  bID,uint32  oID,bool32 blInvalidMark);
						IOSE		FindPkg_maxOID	(CSSL_PNFB_PKG* pkg,PAGENODE** retPage,uint32* cID,uint32  bID,uint32* oID,bool32 blInvalidMark);
						IOSE		FindPkg_minOID	(CSSL_PNFB_PKG* pkg,PAGENODE** retPage,uint32* cID,uint32  bID,uint32* oID,bool32 blInvalidMark);
						IOSE		FindPkg_minBID	(CSSL_PNFB_PKG* pkg,PAGENODE** retPage,uint32* retOID,uint32* cID,uint32* bID);
						bool32		CheckInvalidPkg	(CSSL_PNFB_PKG* pkg);
	public:
						IOSE		RecFromT0		(const uint8* data,const uint64& length);
						IOSE		SendToT0		(CSSL_PNFB_PKG* pkg,IOS* _ios,const T1ID& _id);
	public:
						IOSE		WritePkg		(CSSL_PNFB_PKG* pkg,IOS* _ios,const T1ID& _id,const UVIn& _in);
						IOSE 		ReadPkg			(CSSL_PNFB_PKG* pkg,IOS* _ios,const UVOut& _out,uint32* cID,uint32  bID,uint32  oID,bool32 blInvalidMark);
						IOSE		ReadPkg_maxOID	(CSSL_PNFB_PKG* pkg,IOS* _ios,const UVOut& _out,uint32* cID,uint32  bID,uint32* oID,bool32 blInvalidMark);
	public:
						bool32		CheckPkg		(CSSL_PNFB_PKG* pkg,uint32* cID,uint32 bID,uint32 oID);
						void		CleanInvalidPkg	(CSSL_PNFB_PKG* pkg);
						void		CleanPackages	(CSSL_PNFB_PKG* pkg,uint32* cID,uint32 bID);
						void		CleanPackages	(CSSL_PNFB_PKG* pkg,uint32* cID,uint32 bID,uint32 oID);
};
//------------------------------------------------------------------------------------------//
class CSSL_T1RX : public TNFP{
	public:
				 CSSL_T1RX(void);
		virtual	~CSSL_T1RX(void){;};
	private:
		CSSL_MEMORY		cgMEM;
		CSSL_PNFB_PKG	cgPkg;
		T1ID			cgIDS;
		DS_SpinLock		cgLock;
	private:
		ARRAY			cgArray;
		ALG_ZLIB		cgZLib;
	public:
				void 		InitPN			(uint32 port,const CSSL_T0* csslT0,uint32 maxPackage,uint32 maxPages);
		virtual	void		SetSelfName		(const STDSTR& strName);
		virtual	void		SetUpName		(const STDSTR& strName);
		const	uint32&		GetPort			(void) const;
				void		SetPort			(uint32 port);
		CSSL_PNFB_PKG*		GetDefPkg		(void);
	public:
				void		Reset			(void);
				void		Empty			(void);
				void		Close			(void);
				void		Lock			(G_LOCK on_off);
	public:
				IOSE		RecFromT0		(const uint8* data,const uint64& length);
				IOSE		ReadOneBlock	(IOS* _ios,const UVOut &_out,uint32* retWaitOID,uint32 bID);
				IOSE		GetMinBID		(uint32* bID,uint32* oID);
				IOSE		ReadMaxACK		(CSSL_PNFB_PKG* pkg,IOS* _ios,const UVOut& _out,uint32* retCID,uint32 bID,uint32* oID);
				void		CleanPackages	(CSSL_PNFB_PKG* pkg,uint32 cID,uint32 bID,uint32 oID);
				void		CleanOneBlock	(uint32 bID);
};
//------------------------------------------------------------------------------------------//
class CSSL_T1TX : private DSTF{
	public:
				 CSSL_T1TX(void);
		virtual	~CSSL_T1TX(void);
	private:
		const	uint32	cgMaxPackage;
	private:
		CSSL_MEMORY		cgMEM;
		CSSL_PNFB_PKG	cgPkg;
		T1ID			cgIDS;
		DS_SpinLock		cgLock;
	private:
		ALG_ZLIB_CTX	cgCTX;
		uint32			cgSendOID;
	public:
				void 	InitPN			(uint32 port,const CSSL_T0* csslT0,uint32 maxPackage,uint32 maxPages);
		virtual	void	SetSelfName		(const STDSTR& strName);
		virtual	void	SetUpName		(const STDSTR& strName);
				void	SetPort			(uint32 port);
		CSSL_PNFB_PKG*	GetDefPkg		(void);
	public:
				void	Reset			(void);
				void	Empty			(void);
				void	Close			(void);
				void	Lock			(G_LOCK on_off);
	private:
		virtual	IOSE	DoTransform		(IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
		virtual	IOSE	DoFinal			(IOS* _ios,const UVOut& _out);
				IOSE	Compress		(IOS* _ios,const UVIn& _in);
				IOSE	SendToT0		(IOS* _ios);
	public:
				IOSE	SendOneBlock	(IOS* _ios,uint32* retFinishOID,uint32 bID,const UVIn& _in);
				IOSE	SendPkg			(CSSL_PNFB_PKG* pkg,IOS* _ios,const T1ID& _id,const UVIn& _in);//Msg length must be less then cgMaxPackage
				void	CleanPackages	(CSSL_PNFB_PKG* pkg,uint32 cID,uint32 bID,uint32 oID);
				void	CleanOneBlock	(uint32 bID);
};
//------------------------------------------------------------------------------------------//
class CSSL_T1FD : private TNFP{
	public:
				 CSSL_T1FD(void);
		virtual	~CSSL_T1FD(void);
	private:
		CSSL_T1RX		cgRx;
		uint32			cgPkgOID_RE;
		uint32			cgACKsOID_RE;
		uint32			cgBID_RE;
	private:
		CSSL_T1TX		cgTx;
		uint32			cgBID_WR;
	public:
					void		InitPN			(uint32 port,const CSSL_T0* csslT0,uint32 maxPackage,uint32 maxPages);
		virtual		void		SetSelfName		(const STDSTR& strName);
		virtual		void		SetUpName		(const STDSTR& strName);
	public:
					void		Reset			(void);
					void		Empty			(void);
					void		Close			(void);
					void		DetachT0		(void);
					void		SetPort			(uint32 port);
	private:
					IOSE		ReadOneBlock	(const ABORTCHECK& _ac,IOS* _ios,const UVOut &_out,	uint32 bID,uint32 maxDlyMS,const uint32 maxTimes);
					IOSE		SendOneBlock	(const ABORTCHECK& _ac,IOS* _ios,					uint32 bID,uint32 maxDlyMS,const UVIn& _in);
	public:
					void		RxLock			(G_LOCK on_off);
					void		TxLock			(G_LOCK on_off);
					IOSE		Read			(const ABORTCHECK& _ac,IOS* _ios,const UVOut &_out,	uint32 maxDlyMS,const uint32 maxTimes);
					IOSE		Send			(const ABORTCHECK& _ac,IOS* _ios,					uint32 maxDlyMS,const UVIn& _in);
};
//------------------------------------------------------------------------------------------//
class CSSL_T1 : public TNFP{
	public:
				 CSSL_T1(void);
		virtual	~CSSL_T1(void){;};
	protected:
		CSSL_T1FD		cgT1FD;
		uint32			cgPort;
		uint32			cgMaxDlyMS;
		uint32			cgMaxTimes;
	public:
		virtual	void	Init			(uint32 port,const CSSL_T0* csslT0,uint32 maxPackage,uint32 maxPages);
		virtual	void	SetSelfName		(const STDSTR& strName);
		virtual	void	SetUpName		(const STDSTR& strName);
	public:
				void	Reset			(void);
				void	Empty			(void);
				void	Close			(void);
				void	DetachT0		(void);
				void	SetDelayPar		(uint32 maxDlyMS,uint32 maxTimes);
				uint32	GetPort			(void);
				void	SetPort			(uint32 port);
	public:
				IOSE	Read			(const ABORTCHECK& _ac,IOS* _ios,const UVOut &_out);
				IOSE	Send			(const ABORTCHECK& _ac,IOS* _ios,const UVIn& _in);
};
//------------------------------------------------------------------------------------------//
class CSSL_T1MSG : public CSSL_T1{
	public:
		enum	{MESG_SIZE = 1024 * 8};
	public:
				 CSSL_T1MSG(void) : CSSL_T1(){;};
		virtual	~CSSL_T1MSG(void){;};
	private:
		ARRAY		cgArrayTx;
		ARRAY		cgArrayRx;
		MESG_SC		cgMsg;
	public:
		void		Init		(uint32 port,const CSSL_T0* csslT0,uint32 maxPackage,uint32 maxPages);
		uint32&		Read		(const ABORTCHECK& _ac,IOS* _ios,uint32* mID,const UVOut& msg);
		IOSE		Send		(const ABORTCHECK& _ac,IOS* _ios,uint32  mID,const UVIn&  _in);
};
//------------------------------------------------------------------------------------------//
class CSSL_DEV : public TNFP{
	public:
				 CSSL_DEV(void);
		virtual	~CSSL_DEV(void);
	private:
		DS_BITMAP	cgT1Map;
		DS_SpinLock	cgT1MapLock;
	public:
		CSSL_T0		cgT0;
	public:
		virtual void		SetSelfName		(const STDSTR& strName);
		virtual	void		SetUpName		(const STDSTR& strName);
	public:
				void		Init			(const ARRAY* _out,const ARRAY* _in);
				CSSL_T1*	GetT1			(uint32 port);
				void		SetPort			(CSSL_T1* csslT1,uint32 port);
				CSSL_T1*	AcquireT1		(uint32 maxPackage,uint32 maxPages,uint32 maxDlyMS,uint32 maxTimes);
				CSSL_T1MSG*	AcquireT1MSG	(uint32 maxPackage,uint32 maxPages,uint32 maxDlyMS,uint32 maxTimes);
				void		ReleaseT1		(CSSL_T1* csslT1);
};
//------------------------------------------------------------------------------------------//
#ifndef ADS_SSL_TEST
#include "SYS_Thread.h"
#ifdef SYS_Thread_h
#define ADS_SSL_TEST
#ifdef ADS_SSL_TEST
//------------------------------------------------------------------------------------------//
class CSSL_TEST_PORT : public TNFP{
	public:
				 CSSL_TEST_PORT(void) : TNFP(){;};
		virtual	~CSSL_TEST_PORT(void){Stop();};
	public:
		CSSL_T0		cgT0;
		CSSL_T1		cgT1;
		STDSTR		cgComStrBuf;
		bool32		cgblEcho;
	public:
		SYS_Thread<CSSL_TEST_PORT>	t0Thread;
		SYS_Thread<CSSL_TEST_PORT>	t1RxThread;
		SYS_Thread<CSSL_TEST_PORT>	t1TxThread;
	public:
		bool32		T0ThreadFunc	(void* p);
		bool32		T1RxThreadFunc	(void* strMD5);
		bool32		T1TxThreadFunc	(void* strData);
	public:
		virtual void		SetSelfName		(const STDSTR& strName);
		virtual	void		SetUpName		(const STDSTR& strName);

				void		Init			(uint32 port,const ARRAY* _out,const ARRAY* _in,uint32 maxPackage,uint32 maxPages);
				void		StartT0			(void);
				void		RecInThread		(const STDSTR& strMD5);
				void		Stop			(void);
				void		EchoEnable		(bool32	blEnable);
};
//------------------------------------------------------------------------------------------//
class CSSL_TEST : public TNFP{
	public:
				 CSSL_TEST(void) : TNFP(){;};
		virtual	~CSSL_TEST(void){;};
	public:
		CSSL_TEST_PORT		cgPortA;
		CSSL_TEST_PORT		cgPortB;
		ARRAY				cgArrayX;
		ARRAY				cgArrayY;
	public:
		virtual void		SetSelfName		(const STDSTR& strName);
		virtual	void		SetUpName		(const STDSTR& strName);

				void		DefultInit		(void);
				void		InitA			(uint32 maxPackage,uint32 maxPages);
				void		InitB			(uint32 maxPackage,uint32 maxPages);
				void		InitXY			(uint32 maxPackageX,uint32 maxPackageY);
				void		Start			(void);
				void		Stop			(void);
				bool32		TestAtoB		(const UVIn& _in);
};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif
//------------------------------------------------------------------------------------------//
#endif /* ADS_SSL_h */
#endif /* ADS_SSL_h */
#endif /* ADS_Protocol_h */
