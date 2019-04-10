//
//  ADS_SSL.h
//  SDT
//
//  Created by Leif Wen on 26/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "ADS_Crypto.h"
#ifdef ADS_Crypto_h
//------------------------------------------------------------------------------------------//
#ifndef ADS_SSL_h
#define ADS_SSL_h
#ifdef ADS_SSL_h
//------------------------------------------------------------------------------------------//
class CSSL_T0 : private DSTF{
	public:
		enum	{MAX_PACKAGE_DEALYTM = 1024 * 4};
	public:
				 CSSL_T0(void);
		virtual	~CSSL_T0(void){;};
	private:
		MESG16_AES		cgMsg;
		uint32			cgDefCFG;
		STDSTR			cgDefKey;
		TNF				cgT1SList;
	private:
		double			cgPackageDlyMS;
		uint32			cgPort;
	public:
				void 		InitPN				(const ARRAY* _out,const ARRAY* _in,G_ENDIAN endian = G_ENDIAN_LITTLE);
		virtual void		SetSelfName			(const STDSTR& strName);
		virtual	void		SetFatherName		(const STDSTR& strName);
	public:
				void		SetConfig			(uint32 cfg,const STDSTR& sKey);
				void		SetKey				(			const STDSTR& sKey);
				void		Reset				(void);
	public:
				void		SetPackageDlyMS		(double dlyMS);
				double		GetPackageDlyMS		(void);
	private:
		virtual	ioss		DoTransform			(IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
	public:
				void		Register			(TNF* csslT1);
				void		Delivery			(void);
				bool32		Send				(IOSTATUS* _ios,uint32 port,const UVIn& _in);
};
//------------------------------------------------------------------------------------------//
struct T1ID{
	uint32 ctrlID;
	uint32 blockID;
	uint32 orderID;
};
//------------------------------------------------------------------------------------------//
class CSSL_PNFBT1 : public PNF_BLOCK{
	public:
		enum	{MAX_PACKAGE_SIZE = 1024 / 2};
	public:
				 CSSL_PNFBT1(void);
		virtual	~CSSL_PNFBT1(void){;};
	private:
		PNF_VAR			pn_CtrlID;
		PNF_VAR			pn_BlockID;
		PNF_VAR			pn_OrderID;
		PNF_SC			pn_Info;
	public:
				void 	InitPN		(const ARRAY* _out,const ARRAY* _in,G_ENDIAN endian = G_ENDIAN_LITTLE);
	public:
				uint32	ReadCtrlID	(void)const;
				uint32	ReadBlockID	(void)const;
				uint32	ReadOrderID	(void)const;
	public:
				ioss	Write		(IOSTATUS* _ios,const T1ID& _id,const UVIn& _in);
};
//------------------------------------------------------------------------------------------//
class CSSL_T1_RX : private DSTF{
	public:
				 CSSL_T1_RX(void);
		virtual	~CSSL_T1_RX(void){;};
	private:
		CSSL_PNFBT1		pn_L1;
		T1ID			cgIDRE;
		ALG_ZLIB		cgZLib;
	public:
				void 	InitPN			(G_ENDIAN endian = G_ENDIAN_LITTLE);
		virtual	void	SetSelfName		(const STDSTR& strName);
		virtual	void	SetFatherName	(const STDSTR& strName);
	public:
				void	Empty			(void);
				void	Reset			(void);
				bool32	HasData			(void);
	public:
				void	ReceiveFromT0	(IOSTATUS* _ios,const uint8* data,const uint64& length);
				bool32	ReadNext		(IOSTATUS* _ios,const UVOut &_out,T1ID* _id);
				bool32	ReadCMD			(uint32* ctrlID,uint32* retOrderID,const uint32& blockID);
};
//------------------------------------------------------------------------------------------//
class CSSL_T1_TX : private DSTF{
	public:
		enum	{MAX_PACKAGE_SIZE_ADJUST = CSSL_PNFBT1::MAX_PACKAGE_SIZE - 14};
	protected:
		enum	{RFLAG_C = 1, RFLAG_S = DSTF::RFLAG_S + DSTF::RFLAG_C};
		enum	{CSSL_blInit = RFLAG_CREATE(0)};
	public:
				 CSSL_T1_TX(void);
		virtual	~CSSL_T1_TX(void);
	private:
		CSSL_PNFBT1		pn_L1;
		T1ID			cgIDRE;
		T1ID			cgIDWR;
		ALG_ZLIB_CTX	cgCTX;
	public:
				void 	InitPN			(G_ENDIAN endian = G_ENDIAN_LITTLE);
		virtual	void	SetSelfName		(const STDSTR& strName);
		virtual	void	SetFatherName	(const STDSTR& strName);
	public:
				void	Empty			(void);
				void	Reset			(void);
		const 	uint32&	GetBlockID		(void);
		const 	uint32&	GetOrderID		(void);
	public:
				uint64	CompressUpdate	(IOSTATUS* _ios,const uint8* data,const uint64& length);
				bool32	CompressFinal	(IOSTATUS* _ios);
	private:
				bool32	SendToT0		(IOSTATUS* _ios,CSSL_T0* csslT0,const uint32& port,uint32 offset);
	public:
				bool32	UpdateToT0		(IOSTATUS* _ios,CSSL_T0* csslT0,const uint32& port);
				bool32	ReUpdateToT0	(IOSTATUS* _ios,CSSL_T0* csslT0,const uint32& port,const uint32& blockID,const uint32& orderID);
				void	CleanBlock		(const uint32& blockID,const uint32& orderID);
	public:
				bool32	SendCMD			(IOSTATUS* _ios,CSSL_T0* csslT0,const uint32& port,const T1ID& _id);
};
//------------------------------------------------------------------------------------------//
class CSSL_T1HD : private DSTF{
	private:
		enum	{REQUESTTIMES = 5};
	protected:
		enum	{RFLAG_C = 1, RFLAG_S = DSTF::RFLAG_S + DSTF::RFLAG_C};
		enum	{CSSL_blSendOK = RFLAG_CREATE(1)};
	public:
				 CSSL_T1HD(void);
		virtual	~CSSL_T1HD(void){;};
	private:
		CSSL_T1_RX		cgRx;
		CSSL_T1_TX		cgTx;
		CSSL_T0*		cgT0;
		uint32			cgPort;
	public:
				void		InitPN			(CSSL_T0* csslT0,uint32 port,G_ENDIAN endian = G_ENDIAN_LITTLE);
		virtual	void		SetSelfName		(const STDSTR& strName);
		virtual	void		SetFatherName	(const STDSTR& strName);
	public:
				void		Empty			(void);
				void		Reset			(void);
		const	uint32&		GetPort			(void);
				void		ReceiveFromT0	(IOSTATUS* _ios,const uint8* data,const uint64& length);
	protected:
		virtual	ioss		DoTransform		(IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
		virtual	ioss		DoFinal			(IOSTATUS* _ios,const UVOut& _out);
				bool32 		CheckACK		(IOSTATUS* _ios);
	public:
				void		Lock			(void);
				void		Unlock			(void);
				bool32		SendPackage		(IOSTATUS* _ios,const UVIn&  _in);
				bool32		ReadPackage		(IOSTATUS* _ios,const UVOut& _out);
};
//------------------------------------------------------------------------------------------//
class CSSL_T1 : private TNFP{
	public:
				 CSSL_T1(void);
		virtual	~CSSL_T1(void){;};
	protected:
		CSSL_T1HD		cgRx;
		CSSL_T1HD		cgTx;
	public:
		virtual	void	Init			(CSSL_T0* csslT0,uint32 port);
		virtual	void	SetSelfName		(const STDSTR& strName);
		virtual	void	SetFatherName	(const STDSTR& strName);
	public:
				void	Empty			(void);
				void	Reset			(void);
	public:
				bool32	Send			(IOSTATUS* _ios,const UVIn&  _in);
				bool32	Read			(IOSTATUS* _ios,const UVOut& _out);
};
//------------------------------------------------------------------------------------------//
class CSSL_T1MSG : public CSSL_T1{
	private:
		enum	{MESG_SIZE = 1024 * 8};
	public:
				 CSSL_T1MSG(void) : CSSL_T1(){;};
		virtual	~CSSL_T1MSG(void){;};
	private:
		ARRAY		cgArrayTx;
		ARRAY		cgArrayRx;
		MESG_SC		cgMsg;
	public:
					void	Init		(CSSL_T0* csslT0,uint32 port);
					bool32	Send		(IOSTATUS* _ios,uint32  mID,const UVIn&  _in);
					uint32&	Read		(IOSTATUS* _ios,uint32* mID,const UVOut& msg);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* ADS_SSL_h */
#endif /* ADS_SSL_h */
#endif /* ADS_Protocol_h */
