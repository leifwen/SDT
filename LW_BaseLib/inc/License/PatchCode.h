//
//  PatchCode.h
//  SDT
//
//  Created by Leif Wen on 06/03/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "ADS_Crypto.h"
#ifdef ADS_Crypto_h
//------------------------------------------------------------------------------------------//
#ifndef PatchCode_h
#define PatchCode_h
#ifdef PatchCode_h
//------------------------------------------------------------------------------------------//
class DTIME;
//------------------------------------------------------------------------------------------//
class PATCHCODE : private PNF_BLOCK{
	protected:
		enum		{RFLAG_C = 1, RFLAG_S = PNF_BLOCK::RFLAG_S + PNF_BLOCK::RFLAG_C};
	private:
		enum		{PC_blGet = RFLAG_CREATE(0)};
	public:
				 PATCHCODE(void);
		virtual	~PATCHCODE(void);
	protected:
		PNF_SC			pn_PatchTime;
		PNFSC_AES		pn_FNLic;
		PNF_SC			pn_RSA_Bin;
		PNF_SC			pn_RSA_Admin;
		PNFS_MAIL_SHA1	pn_SMCList;
		PNF_LEN			pn_Len;
		RSA*			cgRSA_Admin;
		RSA*			cgRSA_Bin;
		STDSTR			cgCalcHash;
	public:
						void		Release			(void);
						bool32		AddPatchCode	(STDSTR* retMsg,const STDSTR& fileName);
						bool32		IsReady			(void);
	public:
		static			PATCHCODE& 	LoadPatchCode	(void);
		static	const	STDSTR&		GetSMCList		(STDSTR* smcList);
		static	const	STDSTR&		GetHash			(STDSTR* hash);
		static	const	STDSTR&		CalcHash		(STDSTR* hash);
		static	const	DTIME&		GetTime			(DTIME* patchTime);
		static	const	STDSTR		GetFNLic		(void);
		static			RSA*		GetRSA_Bin		(void);
		static			RSA*		GetRSA_Admin	(void);
};
//------------------------------------------------------------------------------------------//
class REG_Content : protected PNF_BLOCK{
	public:
				 REG_Content(void);
		virtual	~REG_Content(void){;};
	public:
		PNF_SC			pn_RSA_Bin;
		PNFS_MAIL_SHA1	pn_SMCList;
		PNF_SC			pn_MKEY;
		PNF_SC			pn_Hash;
	public:
				void 	FillZero	(void);
				ARRAY*	DataIn		(void);
				ARRAY*	Write		(const RSA* rsa_bin,const STDSTR& smcList,const STDSTR& mKey,const STDSTR& hash);
				bool32	Read		(RSA** rsa_bin,STDSTR* retMKey,const UVOut& smcList,const RSA* rsa_admin);
};
//------------------------------------------------------------------------------------------//
class REG_SIGN : private MAIL_SHA1{
	public:
				 REG_SIGN(void);
		virtual	~REG_SIGN(void){;};
	public:
		REG_Content		cgRegContent;
	public:
				ARRAY* 		Create			(STDSTR* retMsg);
				bool32		CreateREGFILE	(STDSTR* retMsg);
				bool32 		Decode			(RSA** rsa_bin,STDSTR* retMKey,const UVOut& smcList,const RSA* rsa_admin,const UVIn& _in);
};
//------------------------------------------------------------------------------------------//
class LINENSE_Content : protected PNF_BLOCK{
	public:
				 LINENSE_Content(void);
		virtual	~LINENSE_Content(void){;};
	public:
		AESMK_SHA1		pn_RegTime;
		AESMK_SHA1		pn_ApproveTime;
		LIST_AESMK_SHA1	pn_SMCBlock;
	public:
				void 	FillZero	(void);
				ARRAY*	DataIn		(void);
				ARRAY*	Write		(RSA** rsa_bin,const RSA* rsa_admin,const uint64& approveSeconds,const UVIn& regMsg);
				bool32	Read		(const UVOut& smc,const STDSTR& mKey,uint32 order);
				bool32	Read		(DTIME* cRTime,DTIME* cATime,const STDSTR& mKey);
};
//------------------------------------------------------------------------------------------//
class LINENSE_SIGN : private MAIL_SHA1{
	public:
				 LINENSE_SIGN(void);
		virtual	~LINENSE_SIGN(void){;};
	public:
				ARRAY* 	Create	(const uint64& approveSeconds,const UVIn& regMsg);
				bool32	Decode	(LINENSE_Content* content,const UVIn& _in);
};
//------------------------------------------------------------------------------------------//
bool32	DecryptSMC		(STDSTR* smc,LINENSE_Content* linc,uint32 codeNo);
bool32	DecryptTime		(DTIME* cRTime,DTIME* cATime,LINENSE_Content* linc);
//------------------------------------------------------------------------------------------//
#endif /* PatchCode_h */
#endif /* PatchCode_h */
#endif /* ADS_Crypto_h */
