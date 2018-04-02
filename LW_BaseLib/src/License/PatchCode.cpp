//
//  PatchCode.cpp
//  SDT
//
//  Created by Leif Wen on 06/03/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "PatchCode.h"
#include "SystemInfo.h"
#ifdef PatchCode_h
//------------------------------------------------------------------------------------------//
#include "SYS_File.h"
#include "SYS_Time.h"
//------------------------------------------------------------------------------------------//
#define CHK_RSA_S_FN	"RSA_PrivateKey_SDT_0_3_0.pem"
enum {PatchCodeSize = 1024 * 4};
//------------------------------------------------------------------------------------------//
namespace PCNS {
	static inline	bool32	ReadRSA_Admin	(RSA **rsa_admin);
	
	static			bool32	BinAnalysis		(STDSTR* smcInfo,const STDSTR& fileName);
	static			bool32	BinReplace		(				 const STDSTR& fileName,STDSTR smcInfo);
	static			bool32	CreateSMCList	(STDSTR* smcList,const STDSTR& fileName,STDSTR smcInfo);

	static const	STDSTR&	MakeMulitKey	(STDSTR* retMKey);
	static const	STDSTR&	GetMulitKey		(STDSTR* retMKey);
	static const	STDSTR&	FormatKey		(STDSTR* retKey,const STDSTR& mKey);
	static			void	FormatString	(STDSTR* retStr,const STDSTR& strIn);
	static			void	GetPoland		(STDSTR* polandQueue,const STDSTR& expressions);
	static			STDSTR	AandB			(STDSTR A,const STDSTR& B);
};
//------------------------------------------------------------------------------------------//
PATCHCODE::PATCHCODE(void) : PNF_BLOCK(){
	Add(pn_PatchTime) < pn_FNLic < pn_RSA_Bin < pn_RSA_Admin < pn_SMCList < pn_Len;
	
	InitSize(PatchCodeSize);
	
	PNF_BLOCK::InitPN		(&cgArray,&cgArray);

	pn_PatchTime.InitPN		(&cgArray,&cgArray);
	pn_FNLic.InitPN			(&cgArray,&cgArray);
	pn_RSA_Bin.InitPN		(&cgArray,&cgArray);
	pn_RSA_Admin.InitPN		(&cgArray,&cgArray);
	pn_SMCList.InitPN		(&cgArray,&cgArray);
	pn_Len.InitPN			(&cgArray,&cgArray,2,G_ENDIAN_LITTLE);
	
	pn_SMCList.InitCFG(PNFS_MAIL_SHA1::CFG_INIT_RE | PNFS_MAIL_SHA1::CFG_INIT_WR | PNFS_MAIL_SHA1::CFG_AES256 | PNFS_MAIL_SHA1::CFG_AES_CFB8);
	cgRSA_Admin = nullptr;
	cgRSA_Bin = nullptr;
	cgCalcHash = "";
};
//------------------------------------------------------------------------------------------//
PATCHCODE::~PATCHCODE(void){
	Release();
}
//------------------------------------------------------------------------------------------//
void PATCHCODE::Release(void){
	if (cgRSA_Admin != nullptr)
		RSA_free(cgRSA_Admin);
	if (cgRSA_Bin != nullptr)
		RSA_free(cgRSA_Bin);
	cgRSA_Admin = nullptr;
	cgRSA_Bin = nullptr;
	CRYPTO_cleanup_all_ex_data();
	ClrSFlag(PC_blGet);
}
//------------------------------------------------------------------------------------------//
bool32 PATCHCODE::IsReady(void){
	return (CheckSFlag(PC_blGet));
}
//------------------------------------------------------------------------------------------//
bool32 PATCHCODE::AddPatchCode(STDSTR* retMsg,const STDSTR& fileName){
	bool32		err;
	STDSTR		smcInfo,smcList,strStatus,strTime;
	DTIME 		patchTime;
	RSA			*rsa_admin,*rsa_bin;
	ALG_SHA1	sha1;
	ALG_AES		aes256;

	strStatus = "";
	err = 0;
	rsa_admin = RSA_new();
	do{
		if (PCNS::ReadRSA_Admin(&rsa_admin) < 1){
			strStatus += "Read admin RSA public key fail.\n";
			break;
		}
		strStatus += "Read admin RSA public key successful.\n";
		rsa_bin = RSA_new();
		do{
			-- err;
			if (ALG_RSA_CreateKey(rsa_bin, 2048) < 1){
				strStatus += "Create BIN RSA key fail.\n";
				break;
			}
			strStatus += "Create BIN RSA key successful.\n";
			-- err;
			if (CFS_CheckFile(fileName) < 1){
				strStatus += "Get BIN file fail.\n";
				break;
			}
			strStatus += "Get BIN file successful.\n";
			-- err;
			if (PCNS::BinAnalysis(_EMPTY(&smcInfo),fileName) < 1){
				strStatus += "BIN source code analysis fail.\n";
				break;
			}
			strStatus += "BIN source code analysis successful.\n";
			-- err;
			if (PCNS::CreateSMCList(_EMPTY(&smcList),fileName,smcInfo) < 1){
				strStatus += "Create SMC stream fail.\n";
				break;
			}
			strStatus += "Create SMC stream successful.\n";
			-- err;
			if (PCNS::BinReplace(fileName,smcInfo) < 1){
				strStatus += "BIN code replace fail.\n";
				break;
			}
			strStatus += "BIN code replace successful.\n";
			
			cgArray.Reset();
			strTime = Str_DecToHex((uint64)patchTime.Now().GetSec());
			pn_FNLic.InitCFG(DSTF::CFG_INIT_WR_CFGPAR | ALG_AES::CFG_AES128 | ALG_AES::CFG_AES_CFB8, &strTime);
			_Begin(nullptr);
			
			pn_PatchTime = strTime;
			pn_FNLic	 = IUD("License.key");
			pn_RSA_Bin	 = ALG_RSA_Encode_Prk (_EMPTY(&smcInfo),rsa_bin);
			pn_RSA_Admin = ALG_RSA_Encode_Puk (_EMPTY(&smcInfo),rsa_admin);
			_Endl();
			
			sha1.InitCFG() << Startup(nullptr,_NONE()) << IUD_FILE(fileName) << IUD(cgArray) << Finish();
			sha1.GetResult(nullptr, _EMPTY(&smcInfo));
			
			aes256
			.InitSize(1024)
			.InitCFG(ALG_AES::CFG_Encrypt | ALG_AES::CFG_AES256 | ALG_AES::CFG_AES_CFB8,&smcInfo)
			.AllIn(nullptr,_EMPTY(&smcInfo),smcList);
			
			pn_SMCList.InitCFG(CFG_INIT_WR_PAR,rsa_admin) = smcInfo;
			
			pn_Len = cgArray.Used() + 2;
			Save(nullptr, OUD_FILEADD(fileName), IUD(cgArray));
			
			strStatus += "Patch to BIN ";
			strStatus += Str_ToStr(cgArray.Used());
			strStatus += " bytes,successful.\n";
			err = (bool32)cgArray.Used();
			cgArray.Reset();
		}while(0);
		RSA_free(rsa_bin);
	}while(0);
	RSA_free(rsa_admin);

	if (retMsg != nullptr)
		*retMsg = strStatus;
	return(err);
}
//------------------------------------------------------------------------------------------//
PATCHCODE& PATCHCODE::LoadPatchCode(void){
	static	PATCHCODE	sgPatchCode;
	
	if (sgPatchCode.IsReady() == G_FALSE){
		uint8			buffer[1024 * 2];
		std::fstream	fileStream;
		uint64			offset,length,num;
		STDSTR			fileName,strContent;
#ifdef CommonDefH_VC
		CString			cName;
		std::wstring	strwName;
		TCHAR 			name[MAX_PATH];
		HMODULE 		module = GetModuleHandle(0);
		
		GetModuleFileName(module, name, sizeof(name));
		cName.Format(_T("%s"),name);
		strwName = cName;
		fileName = Str_UnicodeToANSI(strwName);
#endif
#ifdef CommonDefH_Unix
		fileName = "VSDT.txt";
#endif
		fileStream.open(fileName.c_str(),std::ios::in|std::ios::binary);
		fileStream.seekp(-2,std::ios::end);
		length = fileStream.tellg();
		fileStream.read((char*)buffer,2);
		offset = Str_CharToUint16(buffer,G_ENDIAN_LITTLE);
		
		fileStream.seekp(0 - offset,std::ios::end);
		
		sgPatchCode.cgArray.Reset();
		do{
			fileStream.read((char*)buffer, sizeof(buffer));
			num = fileStream.gcount();
			sgPatchCode.cgArray.InitSize(sgPatchCode.cgArray.Used() + sizeof(buffer));
			sgPatchCode.cgArray.Put(buffer, (uint32)num);
		}while(!fileStream.eof());
		fileStream.close();
		if (sgPatchCode.Analysis(0) > 0){
			sgPatchCode.pn_RSA_Bin.Read(nullptr, _EMPTY(&strContent));
			sgPatchCode.cgRSA_Bin = RSA_new();
			if (ALG_RSA_Decode_Prk(&sgPatchCode.cgRSA_Bin, strContent)){
				ALG_SHA1	sha1;
				length -= sgPatchCode.pn_SMCList.GetLengthRE();
				sha1.InitCFG().Calc(nullptr,_EMPTY(&sgPatchCode.cgCalcHash),_NONE(),IUD_FILE(fileName,length,0));
				sgPatchCode.SetSFlag(PC_blGet);
			}
			else{
				RSA_free(sgPatchCode.cgRSA_Bin);
				sgPatchCode.cgRSA_Bin = nullptr;
				sgPatchCode.cgCalcHash = "";
			}
		}
	}
	return(sgPatchCode);
}
//------------------------------------------------------------------------------------------//
const STDSTR& PATCHCODE::GetSMCList(STDSTR* smcList){
	if (LoadPatchCode().IsReady())
		LoadPatchCode().pn_SMCList.AllOut(nullptr,smcList);
	return(*smcList);
}
//------------------------------------------------------------------------------------------//
const STDSTR& PATCHCODE::GetHash(STDSTR* hash){
	if (LoadPatchCode().IsReady())
		*hash += LoadPatchCode().cgCalcHash;
	return(*hash);
}
//------------------------------------------------------------------------------------------//
const DTIME& PATCHCODE::GetTime(DTIME* patchTime){
	STDSTR	strContent;
	
	if (LoadPatchCode().IsReady()){
		LoadPatchCode().pn_PatchTime.Read(nullptr, _EMPTY(&strContent));
		*patchTime = (double)Str_HexToDec(strContent);
	}
	else{
		*patchTime = 0;
	}
	return(*patchTime);
}
//------------------------------------------------------------------------------------------//
const STDSTR PATCHCODE::GetFNLic(void){
	STDSTR	fileName = "";
	
	if (LoadPatchCode().IsReady()){
		LoadPatchCode().pn_PatchTime.Read(nullptr, _EMPTY(&fileName));
		LoadPatchCode().pn_FNLic.InitCFG(DSTF::CFG_INIT_RE_CFGPAR | ALG_AES::CFG_AES128 | ALG_AES::CFG_AES_CFB8, &fileName);
		LoadPatchCode().pn_FNLic.Read(nullptr, _EMPTY(&fileName));
	}
	return(fileName);
}
//------------------------------------------------------------------------------------------//
RSA* PATCHCODE::GetRSA_Bin(void){
	return (LoadPatchCode().cgRSA_Bin);
}
//------------------------------------------------------------------------------------------//
RSA* PATCHCODE::GetRSA_Admin(void){
	STDSTR	strContent;
	if (LoadPatchCode().IsReady()){
		if (LoadPatchCode().cgRSA_Admin == nullptr){
			LoadPatchCode().pn_RSA_Admin.Read(nullptr, _EMPTY(&strContent));
			LoadPatchCode().cgRSA_Admin = RSA_new();
			if (ALG_RSA_Decode_Puk(&LoadPatchCode().cgRSA_Admin, strContent) == G_FALSE){
				RSA_free(LoadPatchCode().cgRSA_Admin);
				LoadPatchCode().cgRSA_Admin = nullptr;
			}
		}
	}
	return(LoadPatchCode().cgRSA_Admin);
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
REG_Content::REG_Content(void) : PNF_BLOCK(){
	Add(pn_RSA_Bin) < pn_SMCList < pn_MKEY < pn_Hash;
	
	InitSize(PatchCodeSize);
	PNF_BLOCK::InitPN	(&cgArray,&cgArray);
	
	pn_RSA_Bin.InitPN	(&cgArray,&cgArray);
	pn_SMCList.InitPN	(&cgArray,&cgArray);
	pn_MKEY.InitPN		(&cgArray,&cgArray);
	pn_Hash.InitPN		(&cgArray,&cgArray);
};
//------------------------------------------------------------------------------------------//
void REG_Content::FillZero(void){
	cgArray.FillZero();
};
//------------------------------------------------------------------------------------------//
ARRAY* REG_Content::DataIn(void){
	return(&cgArray);
}
//------------------------------------------------------------------------------------------//
ARRAY* REG_Content::Write(const RSA* rsa_bin,const STDSTR& smcList,const STDSTR& mKey,const STDSTR& hash){
	STDSTR	strContent;
	cgArray.Empty();
	
	_Begin(nullptr);
	
	pn_RSA_Bin	= ALG_RSA_Encode_Puk(_EMPTY(&strContent), rsa_bin);
	cgArray.Put(smcList,G_ESCAPE_OFF);
	pn_MKEY		= mKey;
	pn_Hash		= hash;
	
	_Endl();
	return(&cgArray);
};
//------------------------------------------------------------------------------------------//
bool32 REG_Content::Read(RSA** rsa_bin,STDSTR* retMKey,const UVOut& smcList,const RSA* rsa_admin){
	STDSTR	strContent;
	ALG_AES	aes256;
	if (Analysis(0) > 0){
		pn_RSA_Bin.Read(nullptr, _EMPTY(&strContent));
		if (ALG_RSA_Decode_Puk(rsa_bin, strContent)){
			pn_Hash.Read(nullptr,_EMPTY(&strContent));
			aes256.InitSize(1024).InitCFG(ALG_AES::CFG_Decrypt | ALG_AES::CFG_AES256 | ALG_AES::CFG_AES_CFB8,&strContent);
			if (pn_SMCList.InitCFG(CFG_INIT_RE_PAR,rsa_admin).Read(nullptr, OUD(&aes256,smcList))){
				strContent = Str_ASCIIToHEX(strContent, G_ESCAPE_OFF) + "&[";
				pn_MKEY.Read(nullptr,&strContent);
				strContent += "]";
				PCNS::FormatKey(retMKey,strContent);
			}
		}
	}
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
REG_SIGN::REG_SIGN(void) : MAIL_SHA1(){
	MAIL_SHA1::Init(PatchCodeSize * 2,CFG_AES256 | CFG_AES_CFB8);
};
//------------------------------------------------------------------------------------------//
ARRAY* REG_SIGN::Create(STDSTR* retMsg){
	STDSTR	smcList,mKey,strHash,strStatus = "";
	RSA 	*rsa_bin,*rsa_admin;
	
	do{
		rsa_admin = PATCHCODE::GetRSA_Admin();
		rsa_bin = PATCHCODE::GetRSA_Bin();
		PATCHCODE::GetSMCList(_EMPTY(&smcList));
		PATCHCODE::GetHash(_EMPTY(&strHash));
		PCNS::GetMulitKey(_EMPTY(&mKey));
		
		if (rsa_admin == nullptr){
			strStatus += "Read admin public RSA key fail.\n";
			break;
		}
		if (rsa_bin == nullptr){
			strStatus += "Read bin privte RSA key fail.\n";
			break;
		}
		if (smcList.length() == 0){
			strStatus += "Read SMC fail.\n";
			break;
		}

		Write(rsa_admin, IUD(cgRegContent.Write(rsa_bin,smcList,mKey,strHash)));
	}while(0);
	if (retMsg != nullptr)
		*retMsg = strStatus;
	return(&cgArray);
}
//------------------------------------------------------------------------------------------//
bool32 REG_SIGN::CreateREGFILE(STDSTR* retMsg){
	STDSTR	strReg,retStatus;
	ARRAY	*array;
	bool32	err;
	
	array = Create(&retStatus);
	if (array->IsEmpty() == G_FALSE){
		Save(nullptr, OUD_FILEWR("PreRegistration.key"), IUD(array));
		retStatus += "Create \"PreRegistration.key\" successful.\n";
		err = G_TRUE;
	}
	else{
		retStatus += "Create \"PreRegistration.key\" fail.\n";
		err = G_FALSE;
	}
	if (retMsg != nullptr)
		*retMsg = retStatus;
	return(err);
}
//------------------------------------------------------------------------------------------//
bool32 REG_SIGN::Decode(RSA** rsa_bin,STDSTR* retMKey,const UVOut& smcList,const RSA* rsa_admin,const UVIn& _in){
	if (MAIL_SHA1::Decode(_EMPTY(cgRegContent.DataIn()), rsa_admin, _in)){
		if (cgRegContent.Read(rsa_bin, retMKey, smcList, rsa_admin))
			return G_TRUE;
	}
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
LINENSE_Content::LINENSE_Content(void) : PNF_BLOCK(){
	Add(pn_RegTime) < pn_ApproveTime < pn_SMCBlock;
	
	InitSize(PatchCodeSize * 2);
	PNF_BLOCK::InitPN		(&cgArray,&cgArray);
	
	pn_RegTime.InitPN		(&cgArray,&cgArray);
	pn_ApproveTime.InitPN	(&cgArray,&cgArray);
	pn_SMCBlock.InitPN		(&cgArray,&cgArray);
};
//------------------------------------------------------------------------------------------//
void LINENSE_Content::FillZero(void){
	cgArray.FillZero();
};
//------------------------------------------------------------------------------------------//
ARRAY* LINENSE_Content::DataIn(void){
	return(&cgArray);
}
//------------------------------------------------------------------------------------------//
ARRAY* LINENSE_Content::Write(RSA** rsa_bin,const RSA* rsa_admin,const uint64& approveSeconds,const UVIn& regMsg){
	REG_SIGN	regSign;
	DTIME		regTime;
	STDSTR		smc,mKey;
	LIST_SC		smcList;
	uint32		num,i;

	smcList.InitPN(nullptr, nullptr);
	smcList.InitSize(PatchCodeSize);

	if (regSign.Decode(rsa_bin,&mKey,_EMPTY(&smcList.cgArray),rsa_admin,regMsg)){
		cgArray.Reset();
		if (smcList.Analysis(0)){
			_Begin(nullptr);
			
			pn_RegTime.Write	(nullptr, mKey, Str_DecToHex((uint64)regTime.Now().GetSec()));
			pn_ApproveTime.Write(nullptr, mKey, Str_DecToHex(approveSeconds));
			
			pn_SMCBlock.InitCFG(DSTF::CFG_INIT_WR_CFGPAR | ALG_AES::CFG_AES128 | ALG_AES::CFG_AES_CFB8, &mKey);
			
			num = smcList.ReadQty();
			i = 0;
			
			pn_SMCBlock._Begin(nullptr);
			while(i++ < num){
				smcList.Read(nullptr, _EMPTY(&smc), i);
				pn_SMCBlock.NewText() << smc;
			}
			pn_SMCBlock._Endl();
			
			_Endl();
		}
	}
	return(&cgArray);
}
//------------------------------------------------------------------------------------------//
bool32 LINENSE_Content::Read(const UVOut& smc,const STDSTR& mKey,uint32 order){
	if (Analysis(0)){
		pn_SMCBlock.InitCFG(DSTF::CFG_INIT_RE_CFGPAR | ALG_AES::CFG_AES128 | ALG_AES::CFG_AES_CFB8, &mKey);
		return(pn_SMCBlock.Read(nullptr, smc, order));
	}
	return G_FALSE;
}
//------------------------------------------------------------------------------------------//
bool32 LINENSE_Content::Read(DTIME* cRTime,DTIME* cATime,const STDSTR& mKey){
	STDSTR	strTime;
	if (Analysis(0)){
		if (pn_RegTime.Read(nullptr, _EMPTY(&strTime), mKey)){
			*cRTime = (double)Str_HexToDec(strTime);
			if (pn_ApproveTime.Read(nullptr, _EMPTY(&strTime), mKey)){
				*cATime = (double)Str_HexToDec(strTime);
				return G_TRUE;
			};
		};
	}
	*cRTime = 0;
	*cATime = 0;
	return G_FALSE;
}
//------------------------------------------------------------------------------------------//
bool32 DecryptSMC(STDSTR* smc,LINENSE_Content* linc,uint32 codeNo){
	STDSTR	key;
	return(linc->Read(smc, PCNS::MakeMulitKey(_EMPTY(&key)),codeNo));
}
//------------------------------------------------------------------------------------------//
bool32 DecryptTime(DTIME* cRTime,DTIME* cATime,LINENSE_Content* linc){
	STDSTR	key;
	return(linc->Read(cRTime, cATime, PCNS::MakeMulitKey(_EMPTY(&key))));
}
//------------------------------------------------------------------------------------------//











//------------------------------------------------------------------------------------------//
LINENSE_SIGN::LINENSE_SIGN(void) : MAIL_SHA1(){
	MAIL_SHA1::Init(PatchCodeSize * 4,CFG_AES256 | CFG_AES_CFB8);
};
//------------------------------------------------------------------------------------------//
ARRAY* LINENSE_SIGN::Create(const uint64& approveSeconds,const UVIn& regMsg){
	LINENSE_Content	content;
	RSA 			*rsa_bin,*rsa_admin;
	ARRAY			*array;
	
	rsa_bin = RSA_new();
	rsa_admin = RSA_new();

	if (PCNS::ReadRSA_Admin(&rsa_admin)){
		array = content.Write(&rsa_bin, rsa_admin, approveSeconds, regMsg);
		if (array->IsEmpty() == G_FALSE)
			Write(rsa_bin, IUD(array));
	}
	
	RSA_free(rsa_bin);
	RSA_free(rsa_admin);
	CRYPTO_cleanup_all_ex_data();
	return(&cgArray);
}
//------------------------------------------------------------------------------------------//
bool32 LINENSE_SIGN::Decode(LINENSE_Content* content,const UVIn& _in){
	STDSTR	mKey;
	RSA 	*rsa_bin;
	
	rsa_bin = PATCHCODE::GetRSA_Bin();
	if (rsa_bin != nullptr){
		if (MAIL_SHA1::Decode(_EMPTY(content->DataIn()), rsa_bin, _in)){
			if (content->Read(_NONE(),PCNS::MakeMulitKey(_EMPTY(&mKey)),1))
				return G_TRUE;
			content->FillZero();
		}
	}
	return G_FALSE;
}
//------------------------------------------------------------------------------------------//









//------------------------------------------------------------------------------------------//
static inline bool32 PCNS::ReadRSA_Admin(RSA **rsa_admin){
	return(ALG_RSA_RD_Prk_PEM(rsa_admin, CHK_RSA_S_FN));
}
//------------------------------------------------------------------------------------------//
static bool32 PCNS::BinAnalysis(STDSTR* smcInfo,const STDSTR& fileName){
	uint8			buffer[1024 * 8];
	int32			count;
	std::fstream	fileStream;
	STDSTR			uint32Stack,strSearch,strTemp;
	STRSIZE			searchLocate,searchRet,searchOffset,srearchLength,lengthEncrypt,lengthStrBeg,lengthStrEnd;
	uint64			num;
	
	if (CFS_CheckFile(fileName) == G_FALSE)
		return G_FALSE;
	
	fileStream.open(fileName.c_str(),std::ios::in|std::ios::binary);
	
	uint32Stack = "";
	*smcInfo = "";
	strSearch = "";
	lengthEncrypt = strlen("SMCEncrypt");
	lengthStrBeg = strlen("SMCEncrypt_Begin");
	lengthStrEnd = strlen("SMCEncrypt_End");
	searchOffset = 0;
	count = 0;
	do{
		fileStream.read((char*)buffer, sizeof(buffer));
		num = fileStream.gcount();
		if (num < lengthStrEnd)
			break;
		strSearch += Str_CharToASCII(buffer,num,G_ESCAPE_OFF);
		searchLocate = 0;
		do{
			searchRet = strSearch.find("SMCEncrypt",searchLocate);
			if (searchRet == STDSTR::npos){//no find.
				searchOffset += (strSearch.length() - lengthStrBeg + 1);
				strSearch.erase(0,strSearch.length() - lengthStrBeg + 1);
				break;
			}
			searchLocate = searchRet + lengthEncrypt;
			if (strSearch.length() - searchRet >= lengthStrBeg){
				if (strSearch.substr(searchRet,lengthStrBeg) == "SMCEncrypt_Begin"){
					uint32Stack += ',';
					uint32Stack += Str_DecToHex(searchOffset + searchRet);
					++ count;
					continue;
				}
			}
			if (strSearch.length() - searchRet >= lengthStrEnd){
				if (strSearch.substr(searchRet,lengthStrEnd) == "SMCEncrypt_End"){
					strTemp = Str_ReadSubItemR(&uint32Stack, ",");
					if (strTemp.length() == 0)
						goto Check_end;
					
					-- count;
					*smcInfo += strTemp;
					*smcInfo += ',';
					srearchLength = searchOffset + searchRet + lengthStrEnd - (STRSIZE)Str_HexToDec(strTemp);
					*smcInfo += Str_DecToHex(srearchLength);
					*smcInfo += ',';
					continue;
				}
			}
		}while(1);
	}while(!fileStream.eof());
Check_end:;
	fileStream.close();
	if (count != 0)
		*smcInfo = "";
	return((count == 0) && (smcInfo->length() > 0));
}

//------------------------------------------------------------------------------------------//
static bool32 PCNS::BinReplace(const STDSTR& fileName,STDSTR smcInfo){
	uint8			buffer[2];
	std::fstream	fileStream;
	uint16			codeNo;
	uint64			offset,length;
	STDSTR			strContent;
	
	if (CFS_CheckFile(fileName) == G_FALSE)
		return G_FALSE;
	
	fileStream.open(fileName.c_str(),std::ios::in|std::ios::out|std::ios::binary);
	
	codeNo = 0;
	while(smcInfo.length() > 0){
		++ codeNo;
		offset = Str_HexToDec(Str_ReadSubItem(&smcInfo,","));
		length = Str_HexToDec(Str_ReadSubItem(&smcInfo,","));
		
		Str_Uint16ToChar(buffer,codeNo);
		strContent = Str_CharToASCII(buffer,2,G_ESCAPE_OFF);
		strContent += ALG_CreateRandKey((uint32)length - 2);
		
		fileStream.seekp(offset,std::ios::beg);
		fileStream << strContent;
		fileStream.flush();
	}
	fileStream.close();
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
static void SMC_DelSMCChars(uint8* data,uint32 num){
	//SMCEncrypt_Begin-----------------SMCEncrypt_End
	//\xEB\x0E**************-----------------\xEB\x0C************
	//|<-      strBeg    ->||<-  strCode  ->||<-    strEnd    ->|
	STRSIZE	lengthStrEnd;
	uint8		*p;
	
	lengthStrEnd = strlen("SMCEncrypt_End");
	
	p = data;
	*p++ = '\xEB';
	*p++ = '\x0E';
	
	RAND_bytes(p,0x0E);
	p = data + num - lengthStrEnd;
	*p++ = '\xEB';
	*p++ = '\x0C';
	
	RAND_bytes(p,0x0C);
}
//------------------------------------------------------------------------------------------//
static bool32 PCNS::CreateSMCList(STDSTR* smcList,const STDSTR& fileName,STDSTR smcInfo){
	bool32			err = G_FALSE;
	uint8			buffer[1024 * 2];
	std::fstream	fileStream;
	uint64			offset,length,num;
	LIST_SC			smcBlock;

	if (CFS_CheckFile(fileName) == G_FALSE)
		return G_FALSE;
	
	smcBlock.InitPN(nullptr,nullptr);
	smcBlock.InitSize(PatchCodeSize);
	
	fileStream.open(fileName.c_str(),std::ios::in|std::ios::binary);

	err = G_TRUE;
	smcBlock._Begin(nullptr);
	while(smcInfo.length() > 0){
		offset = Str_HexToDec(Str_ReadSubItem(&smcInfo,","));
		length = Str_HexToDec(Str_ReadSubItem(&smcInfo,","));
		
		fileStream.seekp(offset,std::ios::beg);
		fileStream.read((char*)buffer,(std::size_t)length);
		num = fileStream.gcount();
		
		if (num != length){
			err = G_FALSE;
			break;
		}
		
		SMC_DelSMCChars(buffer,(uint32)length);
		smcBlock.NewText() << IUD(buffer,length);
	}
	smcBlock._Endl();
	smcBlock.cgArray.Get(_EMPTY(smcList), -1);
	
	fileStream.close();
	return(err);
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
static const STDSTR& PCNS::MakeMulitKey(STDSTR* retMKey){
	STDSTR	strHash,strKey;
	
	PATCHCODE::GetHash(_EMPTY(&strHash));

	strKey = Str_ASCIIToHEX(strHash, G_ESCAPE_OFF) + "&";
 	PCNS::GetMulitKey(&strKey);
	return(PCNS::FormatKey(retMKey,strKey));
}
//------------------------------------------------------------------------------------------//
static const STDSTR& PCNS::GetMulitKey(STDSTR* retMKey){
	STDSTR	strKey,strKey2;
	
	SystemInfo_GetNetCardInfo(_EMPTY(&strKey));
	SystemInfo_GetHardiskInfo(&strKey);
	SystemInfo_GetCPUID(&strKey);
	
	FormatKey(_EMPTY(&strKey2),strKey);
	strKey = "[" + strKey2 + "]";
	*retMKey += strKey;
	return(*retMKey);
}
//------------------------------------------------------------------------------------------//
static const STDSTR& PCNS::FormatKey(STDSTR* retKey,const STDSTR& mKey){
	STDSTR	dataT,strPDQueue,strRet,fdata1,fdata2,fdata3;
	
	GetPoland(&strPDQueue,mKey);
	strRet = "";
	while(strPDQueue.length() > 0){
		dataT = Str_ReadSubItem(&strPDQueue, ",");
		if (dataT == "&"){
			fdata2 = Str_HEXToASCII(Str_ReadSubItem(&strRet,","));
			fdata1 = Str_HEXToASCII(Str_ReadSubItem(&strRet,","));
			fdata3 = "";
			fdata3 = AandB(fdata1,fdata2);
			strRet = Str_ASCIIToHEX(fdata3, G_ESCAPE_OFF) + "," + strRet;
		}
		else if (dataT == "|"){
			fdata2 = Str_HEXToASCII(Str_ReadSubItem(&strRet,","));
			fdata1 = Str_HEXToASCII(Str_ReadSubItem(&strRet,","));
			fdata3 = fdata1 + "|" + fdata2;
			strRet = Str_ASCIIToHEX(fdata3, G_ESCAPE_OFF) + "," + strRet;
		}
		else{
			strRet = dataT + "," + strRet;
		}
	}
	
	*retKey = Str_HEXToASCII(Str_ReadSubItem(&strRet,","));
	return(*retKey);
}
//------------------------------------------------------------------------------------------//
static STDSTR PCNS::AandB(STDSTR A,const STDSTR& B){
	STDSTR	ret,sA,sB,_B;
	
	ret = "";
	while (A.length() > 0) {
		sA = Str_ReadSubItem(&A,"|");
		_B = B;
		while (_B.length() > 0) {
			sB = Str_ReadSubItem(&_B,"|");
			ret += "|";
			ret += (sA + sB);
		}
	}
	if (ret[0] == '|')
		ret.erase(0,1);
	return(ret);
};
//------------------------------------------------------------------------------------------//
static void PCNS::FormatString(STDSTR* retStr,const STDSTR& strIn){
	//control char :&& || & | []
	//formate to HEX without space
	//escape && || & | []
	
	STDSTR		strData,strResult;
	STRSIZE		i,length;
	uint8		charData,charData1;
	
	length = strIn.length();
	i = 0;
	*retStr = "[,";
	strData = "";
	while(i < length){
		charData = strIn[i];
		++ i;
		if ((charData == '[') || (charData == ']')){
			Str_TrimSelf(strData);
			if (strData.length() > 0){
				*retStr += Str_ASCIIToHEX(strData,G_ESCAPE_OFF);
				*retStr += ',';
			}
			*retStr += charData;
			*retStr += ',';
			strData = "";
		}
		else if ((charData == '&') || (charData == '|') || (charData == '!')){
			if (i < length){
				charData1 = strIn[i];
				if (charData1 != charData){
					Str_TrimSelf(strData);
					if (strData.length() > 0){
						*retStr += Str_ASCIIToHEX(strData,G_ESCAPE_OFF);
						*retStr += ',';
					}
					*retStr += charData;
					*retStr += ',';
					strData = "";
				}
			}
		}
		else{
			if ((charData == '\\') && (i < length)){
				charData = strIn[i];
				++ i;
				if ((charData != '&') && (charData != '|') && (charData != '[') && (charData != ']'))
					strData += '\\';
			}
			strData += charData;
		}
	}
	Str_TrimSelf(strData);
	if (strData.length() > 0){
		*retStr += Str_ASCIIToHEX(strData,G_ESCAPE_OFF);
		*retStr += ',';
	}
	*retStr += ']';
}
//------------------------------------------------------------------------------------------//
static void PCNS::GetPoland(STDSTR* polandQueue,const STDSTR& expressions){
	STDSTR		stackT,strExp;
	STDSTR		dataT,dataT2;
	
	*polandQueue = "";
	stackT = "";
	
	FormatString(&strExp,expressions);
	
	while(strExp.length() > 0){
		dataT = Str_ReadSubItem(&strExp,",");
		if ((dataT == "&") || (dataT == "|")){
			while(stackT.length() > 0){
				dataT2 = Str_ReadSubItem(&stackT, ",");
				if (dataT2 == "["){
					stackT = dataT + "," + dataT2 + "," + stackT;
					break;
				}
				*polandQueue += dataT2;
				*polandQueue += ',';
			}
		}
		else if (dataT == "]"){
			while(stackT.length() > 0){
				dataT2 = Str_ReadSubItem(&stackT, ",");
				if (dataT2 == "[")
					break;
				*polandQueue += dataT2;
				*polandQueue += ',';
			}
		}
		else {
			stackT = dataT + "," + stackT;
		}
	}
}
//------------------------------------------------------------------------------------------//
#endif /* PatchCode_h */
