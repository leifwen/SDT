//
//  ADS_Crypto.h
//  SDT
//
//  Created by Leif Wen on 25/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#include "ADS_Protocol.h"
#include "ALG_BASE64.h"
#include "ALG_zlib.h"
#include "ALG_Digest.h"
#include "ALG_AES.h"
#include "ALG_RSA.h"
#include "ALG_CRC.h"
#if defined ADS_Protocol_h && defined ALG_BASE64_h && defined ALG_zlib_h && defined ALG_DS_DIGEST && defined ALG_AES_h && defined ALG_DS_CRC
//------------------------------------------------------------------------------------------//
#ifndef ADS_Crypto_h
#define ADS_Crypto_h
#ifdef ADS_Crypto_h
//------------------------------------------------------------------------------------------//
static inline	STDSTR	ALG_AESKey32Bye(const STDSTR &sKey)	{return(ALG_Digest_SHA256(sKey));};
//------------------------------------------------------------------------------------------//
class AES_ZLIB : public DSTF_AB_FRAME<ALG_ZLIB,ALG_AES>{
	protected:
		enum	{CFG_C = 9, CFG_S = DSTF_AB_FRAME::CFG_S + DSTF_AB_FRAME::CFG_C};
	public:
		enum	{CFG_Encrypt = CFG_ATOB, CFG_Decrypt = CFG_BTOA
				,CFG_AES128					= CFG_INIT_B_CFG | (ALG_AES::CFG_AES128 << 8)
				,CFG_AES192					= CFG_INIT_B_CFG | (ALG_AES::CFG_AES192 << 8)
				,CFG_AES256					= CFG_INIT_B_CFG | (ALG_AES::CFG_AES256 << 8)
				,CFG_AES_CBC				= CFG_INIT_B_CFG | (ALG_AES::CFG_AES_CBC << 8)
				,CFG_AES_CFB8				= CFG_INIT_B_CFG | (ALG_AES::CFG_AES_CFB8 << 8)
			
				,CFG_NO_COMPRESSION			= CFG_INIT_A_CFG | (ALG_ZLIB::CFG_NO_COMPRESSION << 16)
				,CFG_BEST_SPEED				= CFG_INIT_A_CFG | (ALG_ZLIB::CFG_BEST_SPEED << 16)
				,CFG_BEST_COMPRESSION		= CFG_INIT_A_CFG | (ALG_ZLIB::CFG_BEST_COMPRESSION << 16)
				,CFG_DEFAULT_COMPRESSION	= CFG_INIT_A_CFG | (ALG_ZLIB::CFG_DEFAULT_COMPRESSION << 16)
			
				,CFG_INIT_CFGPAR			= DSTF::CFG_INIT_B | DSTF::CFG_INIT_CFGPAR
				,CFG_INIT_RE_CFGPAR			= DSTF::CFG_INIT_B | DSTF::CFG_INIT_RE_CFGPAR
				,CFG_INIT_RE_PAR			= DSTF::CFG_INIT_B | DSTF::CFG_INIT_RE_PAR
				,CFG_INIT_WR_CFGPAR			= DSTF::CFG_INIT_B | DSTF::CFG_INIT_WR_CFGPAR
				,CFG_INIT_WR_PAR			= DSTF::CFG_INIT_B | DSTF::CFG_INIT_WR_PAR
		};
	public:
				 AES_ZLIB(void);
		virtual	~AES_ZLIB(void){;};
	public:
		virtual	AES_ZLIB& 	InitCFG			(uint32 cfg,const void* key);
};
//------------------------------------------------------------------------------------------//
template <typename T_DIR> class PNFSC_DIR : public PNF_SC{
	private:
		enum {DSTF_ARRAY_SIZE = 1024 * 1};
	public:
				 PNFSC_DIR(void);
		virtual ~PNFSC_DIR(void){;};
	protected:
		T_DIR			cgDIR_W;
		T_DIR			cgDIR_R;
	public:
		inline	virtual void		SetSelfName		(const STDSTR& strName);
		inline	virtual	void		SetUpName		(const STDSTR& strName);
	public:
		inline	virtual	PNFSC_DIR&	InitCFG			(uint32 cfg,const void* par);
	protected:
				virtual	IOSE		DoTransform		(IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
				virtual	IOSE		DoFinal			(IOS* _ios,const UVOut& _out);
	public:
		inline	virtual	IOSE		_Begin			(IOS* _ios);
				virtual	IOSE		Read			(IOS* _ios,const UVOut& _out);
	
		inline	const	PNFSC_DIR&	operator =		(const UVIn& _in);
};
//------------------------------------------------------------------------------------------//
typedef PNFSC_DIR<ALG_BASE64>				PNFSC_B64;
typedef PNFSC_DIR<ALG_AES>					PNFSC_AES;
typedef PNFSC_DIR<ALG_ZLIB>					PNFSC_ZLIB;
typedef PNFSC_DIR<AES_ZLIB>					PNFSC_AZ;
//------------------------------------------------------------------------------------------//
typedef PNFB_LIST<PNFSC_B64>				LIST_B64;
typedef PNFB_LIST<PNFSC_AES>				LIST_AES;
typedef PNFB_LIST<PNFSC_ZLIB>				LIST_ZLIB;
typedef PNFB_LIST<PNFSC_AZ>					LIST_AZ;
//------------------------------------------------------------------------------------------//
typedef PNFS_MESG<PNFSC_B64>				MESG_B64;
typedef PNFS_MESG<PNFSC_AES>				MESG_AES;
typedef PNFS_MESG<PNFSC_ZLIB>				MESG_ZLIB;
typedef PNFS_MESG<PNFSC_AZ>					MESG_AZ;
//------------------------------------------------------------------------------------------//
typedef PNFS_MESG_CRC<PNFSC_B64	,ALG_CRC8>	MESG8_B64;
typedef PNFS_MESG_CRC<PNFSC_AES	,ALG_CRC8>	MESG8_AES;
typedef PNFS_MESG_CRC<PNFSC_ZLIB,ALG_CRC8>	MESG8_ZLIB;
typedef PNFS_MESG_CRC<PNFSC_AZ	,ALG_CRC8>	MESG8_AZ;

typedef PNFS_MESG_CRC<PNFSC_B64	,ALG_CRC16>	MESG16_B64;
typedef PNFS_MESG_CRC<PNFSC_AES	,ALG_CRC16>	MESG16_AES;
typedef PNFS_MESG_CRC<PNFSC_ZLIB,ALG_CRC16>	MESG16_ZLIB;
typedef PNFS_MESG_CRC<PNFSC_AZ	,ALG_CRC16>	MESG16_AZ;

typedef PNFS_MESG_CRC<PNFSC_B64	,ALG_CRC32>	MESG32_B64;
typedef PNFS_MESG_CRC<PNFSC_AES	,ALG_CRC32>	MESG32_AES;
typedef PNFS_MESG_CRC<PNFSC_ZLIB,ALG_CRC32>	MESG32_ZLIB;
typedef PNFS_MESG_CRC<PNFSC_AZ	,ALG_CRC32>	MESG32_AZ;
//------------------------------------------------------------------------------------------//
#ifdef ALG_RSA_h
template <typename T_DIGEST> class PNFS_MAIL : public PNFB_SHELL{
	public:
		enum	{CFG_AES128					= AES_ZLIB::CFG_AES128
				,CFG_AES192					= AES_ZLIB::CFG_AES192
				,CFG_AES256					= AES_ZLIB::CFG_AES256
				,CFG_AES_CBC				= AES_ZLIB::CFG_AES_CBC
				,CFG_AES_CFB8				= AES_ZLIB::CFG_AES_CFB8
			
				,CFG_NO_COMPRESSION			= AES_ZLIB::CFG_NO_COMPRESSION
				,CFG_BEST_SPEED				= AES_ZLIB::CFG_BEST_SPEED
				,CFG_BEST_COMPRESSION		= AES_ZLIB::CFG_BEST_COMPRESSION
				,CFG_DEFAULT_COMPRESSION	= AES_ZLIB::CFG_DEFAULT_COMPRESSION
			
				,CFG_INIT_CFGPAR			= AES_ZLIB::CFG_INIT_CFGPAR
				,CFG_INIT_RE_CFGPAR			= AES_ZLIB::CFG_INIT_RE_CFGPAR
				,CFG_INIT_RE_PAR			= AES_ZLIB::CFG_INIT_RE_PAR
				,CFG_INIT_WR_CFGPAR			= AES_ZLIB::CFG_INIT_WR_CFGPAR
				,CFG_INIT_WR_PAR			= AES_ZLIB::CFG_INIT_WR_PAR
		};
	public:
				 PNFS_MAIL(void);
		virtual	~PNFS_MAIL(void){;};
	private:
		T_DIGEST		cgDigestW;
		T_DIGEST		cgDigestR;
		PNFSC_AZ		pnml_Text;
		PNF_SC			pnml_KeyDigest;
		RSA* 			cgRSApuk;
		RSA* 			cgRSAprk;
		STDSTR			cgSKey;
	public:
		inline	virtual void			SetSelfName		(const STDSTR& strName);
		inline	virtual	void			SetUpName		(const STDSTR& strName);
	public:
						void			InitPN			(const ARRAY* _out,const ARRAY* _in);
		inline	virtual	PNFS_MAIL&		InitCFG			(uint32 cfg,const void* _rsa = nullptr);
	protected:
				virtual	IOSE			DoTransform		(IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
	public:
				virtual	IOSE			_Begin			(IOS* _ios);
				virtual	IOSE			_Endl			(IOS* _ios);
	public:
				virtual	IOSE			Read			(IOS* _ios,const UVOut& _out);
		inline	const	PNFS_MAIL&		operator =		(const UVIn& _in);
};
//------------------------------------------------------------------------------------------//
typedef PNFS_MAIL<ALG_SHA1>		PNFS_MAIL_SHA1;
typedef PNFS_MAIL<ALG_SHA224>	PNFS_MAIL_SHA224;
typedef PNFS_MAIL<ALG_SHA256>	PNFS_MAIL_SHA256;
typedef PNFS_MAIL<ALG_SHA384>	PNFS_MAIL_SHA384;
typedef PNFS_MAIL<ALG_SHA512>	PNFS_MAIL_SHA512;
typedef PNFS_MAIL<ALG_MD5>	 	PNFS_MAIL_MD5;
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> class MAIL : protected PNFS_MAIL<T_DIGEST>{
	public:
		enum	{CFG_AES128					= PNFS_MAIL<T_DIGEST>::CFG_AES128
				,CFG_AES192					= PNFS_MAIL<T_DIGEST>::CFG_AES192
				,CFG_AES256					= PNFS_MAIL<T_DIGEST>::CFG_AES256
				,CFG_AES_CBC				= PNFS_MAIL<T_DIGEST>::CFG_AES_CBC
				,CFG_AES_CFB8				= PNFS_MAIL<T_DIGEST>::CFG_AES_CFB8
		
				,CFG_NO_COMPRESSION			= PNFS_MAIL<T_DIGEST>::CFG_NO_COMPRESSION
				,CFG_BEST_SPEED				= PNFS_MAIL<T_DIGEST>::CFG_BEST_SPEED
				,CFG_BEST_COMPRESSION		= PNFS_MAIL<T_DIGEST>::CFG_BEST_COMPRESSION
				,CFG_DEFAULT_COMPRESSION	= PNFS_MAIL<T_DIGEST>::CFG_DEFAULT_COMPRESSION
			
				,CFG_INIT_CFGPAR			= PNFS_MAIL<T_DIGEST>::CFG_INIT_CFGPAR
				,CFG_INIT_RE_CFGPAR			= PNFS_MAIL<T_DIGEST>::CFG_INIT_RE_CFGPAR
				,CFG_INIT_RE_PAR			= PNFS_MAIL<T_DIGEST>::CFG_INIT_RE_PAR
				,CFG_INIT_WR_CFGPAR			= PNFS_MAIL<T_DIGEST>::CFG_INIT_WR_CFGPAR
				,CFG_INIT_WR_PAR			= PNFS_MAIL<T_DIGEST>::CFG_INIT_WR_PAR
		};
	public:
				 MAIL(void);
		virtual	~MAIL(void){;};
	private:
		ARRAY&	cgArrayW = PNFS_MAIL<T_DIGEST>::cgArray;
		ARRAY	cgArrayR;
	public:
		inline	virtual void			SetSelfName		(const STDSTR& strName);
		inline	virtual	void			SetUpName		(const STDSTR& strName);
	public:
		inline			MAIL&			Init			(uint32 size,uint32 cfg = CFG_AES256 | CFG_AES_CFB8);
	public:
				const	ARRAY&			Write			(					const RSA* rsa_puk,const UVIn& _in);
						IOSE			Decode			(const UVOut& _out,	const RSA* rsa_prk,const UVIn& _in);
};
//------------------------------------------------------------------------------------------//
typedef MAIL<ALG_SHA1>		MAIL_SHA1;
typedef MAIL<ALG_SHA224>	MAIL_SHA224;
typedef MAIL<ALG_SHA256>	MAIL_SHA256;
typedef MAIL<ALG_SHA384>	MAIL_SHA384;
typedef MAIL<ALG_SHA512>	MAIL_SHA512;
typedef MAIL<ALG_MD5>	 	MAIL_MD5;
//------------------------------------------------------------------------------------------//
#endif
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> class PNFB_AESHASH : public PNF_BLOCK{
	public:
				 PNFB_AESHASH(void);
		virtual ~PNFB_AESHASH(void){;};
	protected:
		T_DIGEST	cgDigestW;
		T_DIGEST	cgDigestR;
		PNFSC_AES	pn_Text;
		PNF_CONTENT	pn_Hash;
	public:
		inline	virtual void			SetSelfName		(const STDSTR& strName);
		inline	virtual	void			SetUpName		(const STDSTR& strName);
	public:
						void			InitPN			(const ARRAY* _out,const ARRAY* _in);
		inline	virtual	PNFB_AESHASH&	InitCFG			(uint32 cfg,const void* par);
	protected:
				virtual	IOSE			DoTransform		(IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length);
	public:
				virtual	IOSE			_Begin			(IOS* _ios);
				virtual	IOSE			_Endl			(IOS* _ios);
	public:
				virtual	IOSE			Read			(IOS* _ios,const UVOut& _out);
};
//------------------------------------------------------------------------------------------//
typedef PNFB_AESHASH<ALG_SHA1>		AES_SHA1;
typedef PNFB_AESHASH<ALG_SHA224>	AES_SHA224;
typedef PNFB_AESHASH<ALG_SHA256>	AES_SHA256;
typedef PNFB_AESHASH<ALG_SHA384>	AES_SHA384;
typedef PNFB_AESHASH<ALG_SHA512>	AES_SHA512;
typedef PNFB_AESHASH<ALG_MD5>	 	AES_MD5;
//------------------------------------------------------------------------------------------//
typedef PNFB_LIST<AES_SHA1>			LIST_AES_SHA1;
typedef PNFB_LIST<AES_SHA224>		LIST_AES_SHA224;
typedef PNFB_LIST<AES_SHA256>		LIST_AES_SHA256;
typedef PNFB_LIST<AES_SHA384>		LIST_AES_SHA384;
typedef PNFB_LIST<AES_SHA512>		LIST_AES_SHA512;
typedef PNFB_LIST<AES_MD5>			LIST_AES_MD5;
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> class PNFB_AESMK_PNFSC: public PNF_BLOCK{
	public:
				 PNFB_AESMK_PNFSC(void);
		virtual ~PNFB_AESMK_PNFSC(void){;};
	protected:
		STDSTR			cgMKeyW;
		STDSTR			cgMKeyR;
		LIST_AES					pnaesmk_Key;
		PNFB_AESHASH<T_DIGEST>		pnaesmk_Text;
	public:
		inline	virtual void				SetSelfName		(const STDSTR& strName);
		inline	virtual	void				SetUpName		(const STDSTR& strName);
	public:
						void				InitPN			(const ARRAY* _out,const ARRAY* _in);
		inline	virtual	PNFB_AESMK_PNFSC&	InitCFG			(uint32 cfg,const void* mKey);
	public:
				virtual	IOSE				_Begin			(IOS* _ios);
				virtual	IOSE				_Endl			(IOS* _ios);
	public:
				virtual	IOSE				Write			(IOS* _ios,const UVIn&  _in);
				virtual	IOSE				Read			(IOS* _ios,const UVOut& _out);
						IOSE				Write			(IOS* _ios,						const STDSTR& mulitKey,const UVIn&  _in);
						IOSE				Read			(IOS* _ios,const UVOut& _out,	const STDSTR& mulitKey);
};
//------------------------------------------------------------------------------------------//
typedef PNFB_AESMK_PNFSC<ALG_SHA1>		AESMK_SHA1;
typedef PNFB_AESMK_PNFSC<ALG_SHA224>	AESMK_SHA224;
typedef PNFB_AESMK_PNFSC<ALG_SHA256>	AESMK_SHA256;
typedef PNFB_AESMK_PNFSC<ALG_SHA384>	AESMK_SHA384;
typedef PNFB_AESMK_PNFSC<ALG_SHA512>	AESMK_SHA512;
typedef PNFB_AESMK_PNFSC<ALG_MD5>		AESMK_MD5;
//------------------------------------------------------------------------------------------//
typedef PNFB_LIST<AESMK_SHA1>			LIST_AESMK_SHA1;
typedef PNFB_LIST<AESMK_SHA224>			LIST_AESMK_SHA224;
typedef PNFB_LIST<AESMK_SHA256>			LIST_AESMK_SHA256;
typedef PNFB_LIST<AESMK_SHA384>			LIST_AESMK_SHA384;
typedef PNFB_LIST<AESMK_SHA512>			LIST_AESMK_SHA512;
typedef PNFB_LIST<AESMK_MD5>			LIST_AESMK_MD5;
//------------------------------------------------------------------------------------------//
namespace DSTF_TEST {
	template <typename T_SHELL> bool32	AESMK_Test	(IOS* _ios,T_SHELL* _shell,STDSTR* _out,STDSTR mKey,const STDSTR& _in);
};
//------------------------------------------------------------------------------------------//
#include "ADS_Crypto.hpp"
#endif /* ADS_Crypto_h */
#endif /* ADS_Crypto_h */
#endif
