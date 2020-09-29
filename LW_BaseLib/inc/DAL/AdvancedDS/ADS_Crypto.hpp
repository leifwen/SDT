//
//  ADS_Crypto.hpp
//  SDT
//
//  Created by Leif Wen on 25/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#ifndef ADS_Crypto_hpp
#define ADS_Crypto_hpp
//------------------------------------------------------------------------------------------//
#include "ADS_Crypto.h"
#ifdef ADS_Crypto_h
#include "DS_STRING.h"
//------------------------------------------------------------------------------------------//
template <typename T_DIR> PNFSC_DIR<T_DIR>::PNFSC_DIR(void) : PNF_SC(){
	cgDIR_W.InitSize(DSTF_ARRAY_SIZE);
	cgDIR_R.InitSize(DSTF_ARRAY_SIZE);
	PNF_SC::SetSelfName("PNFSC");
	cgDIR_W.SetSelfName("DSTFW:" + cgDIR_W.selfName);
	cgDIR_R.SetSelfName("DSTFR:" + cgDIR_R.selfName);
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
template <typename T_DIR> inline void PNFSC_DIR<T_DIR>::SetSelfName(const STDSTR& strName){
	selfName = strName;
	cgDIR_W.SetUpName(GetFullName(this));
	cgDIR_R.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
template <typename T_DIR> inline void PNFSC_DIR<T_DIR>::SetUpName(const STDSTR& strName){
	fatherName = strName;
	cgDIR_W.SetUpName(GetFullName(this));
	cgDIR_R.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
template <typename T_DIR> inline PNFSC_DIR<T_DIR>& PNFSC_DIR<T_DIR>::InitCFG(uint32 cfg,const void* par){
	if (B_ChkFLAG32(cfg, CFG_INIT_WR))
		cgDIR_W.InitCFG	(cfg | DSTF_DIRBASE::CFG_DIR0, par);
	if (B_ChkFLAG32(cfg, CFG_INIT_RE))
		cgDIR_R.InitCFG	(cfg & (~DSTF_DIRBASE::CFG_DIR0), par);
	return(*this);
};
//------------------------------------------------------------------------------------------//
template <typename T_DIR>
IOSE PNFSC_DIR<T_DIR>::DoTransform(IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	if (_out.uvid == UVID_SELF){
		return(PNF_SC::DoTransform(_ios,_out,data,length));
	}
	else{
		return(cgDIR_W.Transform(_ios,OUD(this), data, length));
	}
};
//------------------------------------------------------------------------------------------//
template <typename T_DIR> IOSE PNFSC_DIR<T_DIR>::DoFinal(IOS* _ios,const UVOut& _out){
	IOSE rcode = IOS_FINISH;
	IOS_update(&rcode,cgDIR_W.Final(_ios,OUD(this)));
	IOS_update(&rcode,PNF_SC::DoFinal(_ios,_out));
	return(rcode);
};
//------------------------------------------------------------------------------------------//
template <typename T_DIR> inline IOSE PNFSC_DIR<T_DIR>::_Begin(IOS* _ios){
	return(PNF_SC::_Begin(_ios));
};
//------------------------------------------------------------------------------------------//
template <typename T_DIR> IOSE PNFSC_DIR<T_DIR>::Read(IOS* _ios,const UVOut& _out){
	IOSE rcode = IOS_OK;
	IOS_update(&rcode,PNF_SC::Read(_ios, OUD(&cgDIR_R,_out)));
	IOS_update(&rcode,cgDIR_R.Final(_ios,_out));
	return(rcode);
};
//------------------------------------------------------------------------------------------//
template <typename T_DIR>
inline const PNFSC_DIR<T_DIR>& PNFSC_DIR<T_DIR>::operator =	(const UVIn& _in){
	Write(cgStartup.ios, _in);
	return(*this);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
#ifdef ALG_RSA_h
template <typename T_DIGEST> PNFS_MAIL<T_DIGEST>::PNFS_MAIL(void) : PNFB_SHELL(){
	STDSTR sKey = " ";
	AddBlockSubPN(pnml_Text) < pnml_KeyDigest;
	SetBlockPoint(&pnml_Text);
	
	cgRSApuk = nullptr;
	cgRSAprk = nullptr;
	cgSKey = " ";

	TNFP::SetSelfName("PNFS_MAIL");
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> inline void PNFS_MAIL<T_DIGEST>::SetSelfName(const STDSTR& strName){
	selfName = strName;
	cgDigestW.SetUpName(GetFullName(this));
	cgDigestR.SetUpName(GetFullName(this));
	pnml_Text.SetUpName(GetFullName(this));
	pnml_KeyDigest.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> inline void PNFS_MAIL<T_DIGEST>::SetUpName(const STDSTR& strName){
	fatherName = strName;
	cgDigestW.SetUpName(GetFullName(this));
	cgDigestR.SetUpName(GetFullName(this));
	pnml_Text.SetUpName(GetFullName(this));
	pnml_KeyDigest.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> void PNFS_MAIL<T_DIGEST>::InitPN(const ARRAY* _out,const ARRAY* _in){
	if (_out == nullptr)
		_out = &cgArray;
	if (_in == nullptr)
		_in = &cgArray;
	
	PNFB_SHELL::InitPN		(_out,_in,0,G_ENDIAN_LITTLE);
	pnml_Text.InitPN		(_out,_in);
	pnml_KeyDigest.InitPN	(_out,_in);
};
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> inline PNFS_MAIL<T_DIGEST>& PNFS_MAIL<T_DIGEST>::InitCFG(uint32 cfg,const void* _rsa){
	if (B_ChkFLAG32(cfg,CFG_INIT_CFG))
		pnml_Text.InitCFG(cfg | CFG_INIT_PAR,&cgSKey);
	if (B_ChkFLAG32(cfg,CFG_INIT_WR) && B_ChkFLAG32(cfg,CFG_INIT_PAR))
		cgRSApuk = (RSA*)_rsa;
	if (B_ChkFLAG32(cfg,CFG_INIT_RE) && B_ChkFLAG32(cfg,CFG_INIT_PAR))
		cgRSAprk = (RSA*)_rsa;
	return(*this);
};
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> IOSE PNFS_MAIL<T_DIGEST>::_Begin(IOS* _ios){
	IOSE	rcode = IOS_OK;
	cgDigestW.InitCFG();
	cgSKey = ALG_CreateRandKey(32);

	IOS_update(&rcode, PNFB_SHELL::_Begin(_ios));
	pnml_Text.InitCFG(CFG_INIT_WR_PAR,&cgSKey);
	IOS_update(&rcode, pnml_Text._Begin(_ios));
	return(rcode);
};
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> IOSE PNFS_MAIL<T_DIGEST>::_Endl(IOS* _ios){
	IOSE	rcode = IOS_OK;
	STDSTR	strContent;
	uint64	num = 0;

	IOS_update(&rcode,pnml_Text._Endl(_ios));
	
	cgDigestW.Final(nullptr,_NONE());
	cgDigestW.GetResult(nullptr,&cgSKey);
	if (cgStartup.ios != nullptr)
		num = cgStartup.ios->total_in;

	IOS_update(&rcode,pnml_KeyDigest.Write(cgStartup.ios,ALG_RSA_Encrypt_Puk(_EMPTY(&strContent),cgRSApuk,cgSKey)));
	
	if (cgStartup.ios != nullptr)
		cgStartup.ios->total_in = num;

	IOS_update(&rcode,PNFB_SHELL::_Endl(_ios));
	return(rcode);
};
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST>
IOSE PNFS_MAIL<T_DIGEST>::DoTransform(IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	IOSE rcode;
	rcode = pnml_Text.Transform(_ios,data,length);
	cgDigestW.Transform(nullptr,_NONE(),data,length);
	return(rcode);
};
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> IOSE PNFS_MAIL<T_DIGEST>::Read(IOS* _ios,const UVOut& _out){
	STDSTR	strContent,sKey,strDigest;
	
	pnml_KeyDigest.Read(nullptr,_EMPTY(&strContent));
	if (strContent.length() > 0){
		if (ALG_RSA_Decrypt_Prk(_EMPTY(&sKey),cgRSAprk, strContent).length() > 32){
			strDigest = sKey.substr(32);
			sKey.erase(32);
			
			cgDigestR.InitCFG();
			pnml_Text.InitCFG(CFG_INIT_RE_PAR,&sKey).Read(nullptr,OUD(&cgDigestR,_EMPTY(&strContent)));
			
			cgDigestR.GetResult(nullptr,_EMPTY(&sKey));
			
			if (sKey == strDigest)
				return(Save(_ios, _out, strContent));
		}
	}
	return(IOS_update(_ios, IOS_ERR));
};
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> inline const PNFS_MAIL<T_DIGEST>& PNFS_MAIL<T_DIGEST>::operator = (const UVIn& _in){
	Write(nullptr,_in);
	return(*this);
};
#endif
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
#ifdef ALG_RSA_h
template <typename T_DIGEST> MAIL<T_DIGEST>::MAIL(void) : PNFS_MAIL<T_DIGEST>(){

	PNFS_MAIL<T_DIGEST>::InitPN	(&cgArrayW,&cgArrayR);
	
	TNFP::SetSelfName("MAIL");
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> inline void MAIL<T_DIGEST>::SetSelfName(const STDSTR& strName){
	PNFS_MAIL<T_DIGEST>::SetSelfName(strName);
};
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> inline void MAIL<T_DIGEST>::SetUpName(const STDSTR& strName){
	PNFS_MAIL<T_DIGEST>::SetUpName(strName);
};
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> MAIL<T_DIGEST>& MAIL<T_DIGEST>::Init(uint32 size,uint32 cfg){
	PNFS_MAIL<T_DIGEST>::InitCFG(cfg | MAIL::CFG_INIT_RE_CFG | MAIL::CFG_INIT_WR_CFG,nullptr);
	cgArrayW.InitSize(size);
	cgArrayR.InitSize(size);
	return(*this);
};
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> const ARRAY& MAIL<T_DIGEST>::Write(const RSA* rsa_puk,const UVIn& _in){
	cgArrayW.Reset();

	this->InitCFG(MAIL::CFG_INIT_WR_PAR,rsa_puk).Write(nullptr,_in);
	
	return(cgArrayW);
};
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> IOSE MAIL<T_DIGEST>::Decode(const UVOut& _out,const RSA *rsa_prk,const UVIn& _in){
	STDSTR	strContent,sKey,strDigest;
	
	cgArrayR.Reset();
	this->Save(nullptr, &cgArrayR, _in);
	
	this->InitCFG(MAIL::CFG_INIT_RE_PAR,rsa_prk);
	
	if (this->Analysis(0) > 0)
		return(this->Read(nullptr,_out));
	return IOS_ERR;
};
#endif
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> PNFB_AESHASH<T_DIGEST>::PNFB_AESHASH(void) : PNF_BLOCK(){
	AppendDown(pn_Text) < pn_Hash;
	TNFP::SetSelfName("PNF_AESHASH");
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> inline void PNFB_AESHASH<T_DIGEST>::SetSelfName(const STDSTR& strName){
	selfName = strName;
	cgDigestW.SetUpName(GetFullName(this));
	cgDigestR.SetUpName(GetFullName(this));
	pn_Text.SetUpName(GetFullName(this));
	pn_Hash.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> inline void PNFB_AESHASH<T_DIGEST>::SetUpName(const STDSTR& strName){
	fatherName = strName;
	cgDigestW.SetUpName(GetFullName(this));
	cgDigestR.SetUpName(GetFullName(this));
	pn_Text.SetUpName(GetFullName(this));
	pn_Hash.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> void PNFB_AESHASH<T_DIGEST>::InitPN(const ARRAY* _out,const ARRAY* _in){
	if (_out == nullptr)
		_out = &cgArray;
	if (_in == nullptr)
		_in = &cgArray;

	PNF_BLOCK::InitPN(_out, _in, &pn_Text);
	pn_Text.InitPN	 (_out, _in);
	pn_Hash.InitPN	 (_out, _in, cgDigestW.GetResultSize());
};
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> inline PNFB_AESHASH<T_DIGEST>& PNFB_AESHASH<T_DIGEST>::InitCFG(uint32 cfg,const void* par){
	pn_Text.InitCFG(cfg, par);
	return(*this);
};
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> IOSE PNFB_AESHASH<T_DIGEST>::_Begin(IOS* _ios){
	IOSE	rcode = IOS_OK;
	cgDigestW.InitCFG();
	IOS_update(&rcode,PNF_BLOCK::_Begin(_ios));
	IOS_update(&rcode,pn_Text._Begin(_ios));
	return(rcode);
};
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> IOSE PNFB_AESHASH<T_DIGEST>::_Endl(IOS* _ios){
	IOSE	rcode = IOS_OK;
	IOS_update(&rcode,pn_Text._Endl(_ios));
	cgDigestW.Final(nullptr,_NONE());
	IOS_update(&rcode,pn_Hash.Write(cgStartup.ios, IUD(cgDigestW.cgArray.GetPointer(0),cgDigestW.GetResultSize())));
	if (cgStartup.ios != nullptr)
		cgStartup.ios->total_in -= cgDigestW.GetResultSize();
	IOS_update(&rcode,PNF_BLOCK::_Endl(_ios));
	return(rcode);
};
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST>
IOSE PNFB_AESHASH<T_DIGEST>::DoTransform(IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	IOSE rcode;
	rcode = pn_Text.Transform(_ios,data,length);
	cgDigestW.Transform(nullptr,_NONE(),data,length);
	return(rcode);
};
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> IOSE PNFB_AESHASH<T_DIGEST>::Read(IOS* _ios,const UVOut& _out){
	STDSTR	strO,strC,strContent;

	cgDigestR.InitCFG();
	pn_Text.Read(nullptr, OUD(&cgDigestR,&strContent));

	cgDigestR.GetResult(nullptr,OUD_HEX(_EMPTY(&strC)));
	pn_Hash.Read(nullptr, OUD_HEX(_EMPTY(&strO)));

	if (strC == strO)
		return(Save(_ios, _out, strContent));
	
	return(IOS_update(_ios, IOS_ERR));
};
//------------------------------------------------------------------------------------------//









#define T_AESMK PNFB_AESMK_PNFSC<T_DIGEST>
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> T_AESMK::PNFB_AESMK_PNFSC(void) : PNF_BLOCK(){
	AppendDown(pnaesmk_Key) < pnaesmk_Text;
	cgMKeyW = "";
	cgMKeyR = "";
	TNFP::SetSelfName("PNFB_AESMK_PNFSC");
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> inline void T_AESMK::SetSelfName(const STDSTR& strName){
	selfName = strName;
	pnaesmk_Key.SetUpName(GetFullName(this));
	pnaesmk_Text.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> inline void T_AESMK::SetUpName(const STDSTR& strName){
	fatherName = strName;
	pnaesmk_Key.SetUpName(GetFullName(this));
	pnaesmk_Text.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> void T_AESMK::InitPN(const ARRAY* _out,const ARRAY* _in){
	if (_out == nullptr)
		_out = &cgArray;
	if (_in == nullptr)
		_in = &cgArray;
	
	PNF_BLOCK::InitPN		(_out,_in,&pnaesmk_Text);
	pnaesmk_Key.InitPN		(_out,_in);
	pnaesmk_Text.InitPN		(_out,_in);
};
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> inline T_AESMK& T_AESMK::InitCFG(uint32 cfg,const void* mKey){
	if (B_ChkFLAG32(cfg,DSTF::CFG_INIT_CFG)){
		pnaesmk_Key.InitCFG(cfg | DSTF::CFG_INIT_PAR,&cgMKeyW);
		pnaesmk_Text.InitCFG(cfg | DSTF::CFG_INIT_PAR,&cgMKeyW);
	}
	if (B_ChkFLAG32(cfg,DSTF::CFG_INIT_WR) && B_ChkFLAG32(cfg,DSTF::CFG_INIT_PAR))
		cgMKeyW = *(STDSTR*)mKey;
	if (B_ChkFLAG32(cfg,DSTF::CFG_INIT_RE) && B_ChkFLAG32(cfg,DSTF::CFG_INIT_PAR))
		cgMKeyR = *(STDSTR*)mKey;
	return(*this);
};
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> IOSE T_AESMK::_Begin(IOS* _ios){
	//strMulitKey is in HEX, used '|' to split
	STDSTR		randKey,sKey,mKey;
	IOSE		rcode = IOS_OK;
	uint64		total_in = 0;
	
	if (_ios != nullptr)
		total_in = _ios->total_in;
	
	IOS_update(&rcode, PNF_BLOCK::_Begin(_ios));
	randKey = ALG_CreateRandKey(32);
	
	IOS_update(&rcode, pnaesmk_Key._Begin(_ios));
	mKey = cgMKeyW;
	while(mKey.length() > 0){
		sKey = Str_ReadSubItem(&mKey, "|");
		if (sKey.length() > 0){
			pnaesmk_Key.NewText();
			pnaesmk_Key.InitCFG(DSTF::CFG_INIT_WR_PAR,&sKey);
			pnaesmk_Key << randKey;
		}
	};
	IOS_update(&rcode, pnaesmk_Key._Endl(_ios));
	
	if (_ios != nullptr)
		_ios->total_in = total_in;

	pnaesmk_Text.InitCFG(DSTF::CFG_INIT_WR_PAR,&randKey);
	IOS_update(&rcode, pnaesmk_Text._Begin(_ios));
	return(rcode);
};
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> IOSE T_AESMK::_Endl(IOS* _ios){
	IOSE	rcode = IOS_OK;
	IOS_update(&rcode,pnaesmk_Text._Endl(_ios));
	IOS_update(&rcode,PNF_BLOCK::_Endl(_ios));
	return(rcode);
};
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> IOSE T_AESMK::Write(IOS* _ios,const UVIn& _in){
	return(PNF_BLOCK::Write(_ios,_in));
};
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> IOSE T_AESMK::Write(IOS* _ios,const STDSTR& mulitKey,const UVIn& _in){
	//strMulitKey is in HEX, used '|' to split
	cgMKeyW = mulitKey;
	return(Write(_ios,_in));
};
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> IOSE T_AESMK::Read(IOS* _ios,const UVOut& _out){
	return(Read(_ios,_out,cgMKeyR));
};
//------------------------------------------------------------------------------------------//
template <typename T_DIGEST> IOSE T_AESMK::Read(IOS* _ios,const UVOut& _out,const STDSTR& mulitKey){
	STDSTR	randKey,mKey,sKey;
	uint32	num,i;
	IOSE 	rcode;
	
	if ((_ios != nullptr) && (_ios->rcode <= 0))
		return(_ios->rcode);

	num = pnaesmk_Key.ReadQty();
	i = 0;
	while (i++ < num) {
		mKey = mulitKey;
		while (mKey.length() > 0){
			sKey = Str_ReadSubItem(&mKey, "|");
			pnaesmk_Key.InitCFG(DSTF::CFG_INIT_RE_PAR,&sKey).Read(nullptr,_EMPTY(&randKey),i);
			pnaesmk_Text.InitCFG(DSTF::CFG_INIT_RE_PAR,&randKey);
			rcode = pnaesmk_Text.Read(IOS_reset(_ios),_out);
			if (rcode > 0)
				return(IOS_update(_ios, IOS_OK));
		};
	};
	return(IOS_update(_ios, IOS_ERR));
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
template <typename T_SHELL> bool32 DSTF_TEST::AESMK_Test(IOS* _ios,T_SHELL* _shell,STDSTR* _out,STDSTR mKey,const STDSTR& _in){
	bool32 retbl,result;
	STDSTR sKey;
	
	_shell[0].GetDefArrayWR()->Reset();
	_shell[0].GetDefArrayRE()->Reset();
	_shell[1].GetDefArrayWR()->Reset();
	_shell[1].GetDefArrayRE()->Reset();

	printf("----------------------------------------------------------\n");
	_shell[0].Write(IOS_clr(_ios), mKey, _in);
	PrintResult(_shell[0].selfName + " Write   ",ShowINOUT(_ios),1);
	
	PrintResult(_shell[1].selfName + " Analysis","",_shell[1].Analysis(0));
	
	result = G_TRUE;
	sKey = mKey;
	retbl = _shell[1].Read(IOS_clr(_ios), _EMPTY(_out), sKey) > 0;
	result &= retbl;
	PrintResult(_shell[1].selfName + " Key     ",sKey,G_TRUE);
	PrintResult(_shell[1].selfName + " Read    ",ShowINOUT(_ios),retbl);
	PrintResult(_shell[1].selfName + " in==out ",ShowINOUT("",_in.length(),_out->length()),(*_out == _in));
	result &= (*_out == _in);
	do{
		sKey = Str_ReadSubItem(&mKey, "|");
		retbl = _shell[1].Read(IOS_clr(_ios), _EMPTY(_out), sKey) > 0;
		result &= retbl;
		PrintResult(_shell[1].selfName + " Key     ",sKey,G_TRUE);
		PrintResult(_shell[1].selfName + " Read    ",ShowINOUT(_ios),retbl);
		PrintResult(_shell[1].selfName + " in==out ",ShowINOUT("",_in.length(),_out->length()),(*_out == _in));
		result &= (*_out == _in);
	}while (mKey.length() > 0);
	
	sKey = "testkey";
	retbl = _shell[1].Read(IOS_clr(_ios), _EMPTY(_out), sKey) > 0;
	result &= (retbl <= 0);
	PrintResult(_shell[1].selfName + " Key     ",sKey,G_TRUE);
	PrintResult(_shell[1].selfName + " Read    ",ShowINOUT(_ios),retbl);
	PrintResult(_shell[1].selfName + " in==out ",ShowINOUT("",_in.length(),_out->length()),(*_out == _in));
	result &= (*_out != _in);

	_shell[0].GetDefArrayWR()->Reset();
	_shell[0].GetDefArrayRE()->Reset();
	_shell[1].GetDefArrayWR()->Reset();
	_shell[1].GetDefArrayRE()->Reset();
	return(result);
};
//------------------------------------------------------------------------------------------//
#endif /* ADS_Crypto_h */
#endif /* ADS_Crypto_hpp */
