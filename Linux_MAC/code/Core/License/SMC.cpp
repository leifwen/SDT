/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: SMC.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.03.26
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "SMC.h"
#include "Comm_Convert.h"
#include "Comm_File.h"
#include "Comm_Crypto.h"
#include "License_Checker.h"
#ifdef USE_OPENSSL
//------------------------------------------------------------------------------------------//
int32 SMC_SourceCodeAnalysis(STDSTR *retStr,const STDSTR &sFN){
	uint8			buffer[1024 * 8];
	int32			count;
	std::fstream	fileStream;
	STDSTR			uint32Stack,strSearch,strTemp;
	STDSTR::size_type	searchLocate,searchRet,searchOffset,srearchLength,lengthEncrypt,lengthStrBeg,lengthStrEnd;
	uint64			num;
	
	if (CFS_CheckFile(sFN) == 0)
		return 0;
	
	fileStream.open(sFN.c_str(),std::ios::in|std::ios::binary);
	
	uint32Stack = "";
	*retStr = "";
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
		strSearch += Str_CharToASCIIStr(buffer,num,G_ESCAPE_OFF);
		searchLocate = 0;
		do{
			searchRet = strSearch.find("SMCEncrypt",searchLocate);
			if (searchRet == STDSTR::npos){//no find.
				searchOffset += strSearch.length() - lengthStrBeg + 1;
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
					*retStr += strTemp;
					*retStr += ',';
					srearchLength = searchOffset + searchRet + lengthStrEnd - (STDSTR::size_type)Str_HexToDec(strTemp);
					*retStr += Str_DecToHex(srearchLength);
					*retStr += ',';
					continue;
				}
			}
		}while(1);
	}while(!fileStream.eof());
Check_end:;
	fileStream.close();
	if (count != 0)
		*retStr = "";
	return(count == 0);
}
//------------------------------------------------------------------------------------------//
void SMC_EncryptCYCodeBlock(uint8 *data,uint32 num){
	//SMCEncrypt_Begin-----------------SMCEncrypt_End
	//\xEB\x0E**************-----------------\xEB\x0C************
	//|<-      strBeg    ->||<-  strCode  ->||<-    strEnd    ->|
	STDSTR::size_type	lengthStrEnd;
	uint8	*p;
	
	lengthStrEnd = strlen("SMCEncrypt_End");

	p = data;
	*p++ = '\xEB';
	*p++ = '\x0E';//lengthStrBeg - 2
	
	RAND_bytes(p,0x0E);
	p = data + num - lengthStrEnd;
	*p++ = '\xEB';
	*p++ = '\x0C';//lengthStrEnd - 2
	
	RAND_bytes(p,0x0C);
}
//------------------------------------------------------------------------------------------//
int32 SMC_CreateCYCodeStream(STDSTR *retStr,const STDSTR &sFN_SDT,STDSTR strAnalysisFIFO){
	uint8			buffer[1024 * 2];
	std::fstream	fileStream;
	int32			ret;
	uint64			offset,length,num;
#ifdef Comm_PAH
	CCY_FNLC_AES	cyBlock;
	FIFO8			tFIFO_Result;
	
	if (CFS_CheckFile(sFN_SDT) == 0)
		return 0;
	
	tFIFO_Result.Init(1024 * 8);
	cyBlock.Init(&tFIFO_Result);
	
	fileStream.open(sFN_SDT.c_str(),std::ios::in|std::ios::binary);
	
	ret = 0;
	while(strAnalysisFIFO.length() > 0){
		offset = Str_HexToDec(Str_ReadSubItem(&strAnalysisFIFO,","));
		length = Str_HexToDec(Str_ReadSubItem(&strAnalysisFIFO,","));
		
		fileStream.seekp(offset,std::ios::beg);
		fileStream.read((char*)buffer,(std::size_t)length);
		num = fileStream.gcount();

		if (num != length)
			goto SMC_err;
		
		SMC_EncryptCYCodeBlock(buffer,(uint32)length);
		cyBlock.SetContent(buffer,(uint32)length,CCY_AESKey32Bye("SMCEncrypt"));
	}
	*retStr = "";
	tFIFO_Result.Get(retStr, -1);
	ret = 1;
SMC_err:
	fileStream.close();
#endif
	return(ret);
}
//------------------------------------------------------------------------------------------//
int32 SMC_SourceCodeReplace(const STDSTR &sFN_SDT,STDSTR strAnalysisFIFO){
	uint8			buffer[2];
	std::fstream	fileStream;
	uint16			codeNo;
	uint64			offset,length;
	STDSTR			strContent;
	
	if (CFS_CheckFile(sFN_SDT) == 0)
		return 0;
	
	fileStream.open(sFN_SDT.c_str(),std::ios::in|std::ios::out|std::ios::binary);
	
	codeNo = 0;
	while(strAnalysisFIFO.length() > 0){
		++ codeNo;
		offset = Str_HexToDec(Str_ReadSubItem(&strAnalysisFIFO,","));
		length = Str_HexToDec(Str_ReadSubItem(&strAnalysisFIFO,","));
		
		Str_Uint16ToChar(buffer,codeNo);
		strContent = Str_CharToASCIIStr(buffer,2,G_ESCAPE_OFF);
		strContent += CCY_CreateRandKey((uint32)length - 2);

		fileStream.seekp(offset,std::ios::beg);
		fileStream << strContent;
		fileStream.flush();
	}
	fileStream.close();
	return 1;
}
//------------------------------------------------------------------------------------------//
#ifdef SMC_YESH
int32 SMC_DecryptCYCodeBlock(STDSTR *retStr,const FIFO8 &cyCodeStream,uint32 codeNo){
	STDSTR			sMKey;
	uint32			retRead;
	CCY_FN_AES_MK	cyBlock_MK;

	cyBlock_MK.Init(&cyCodeStream, CCT_AES256, CCT_AES_CBC, CCT_SHA256);
	
	retRead = 0;
	while(codeNo-- > 0){
		cyBlock_MK.AnalysisFrame(&cyCodeStream, retRead);
		retRead += cyBlock_MK.GetLength();
	}

	if (retRead != 0){
		if (cyBlock_MK.ReadContent(retStr, MakeMulitKey(&sMKey), &cyCodeStream) != 0)
			return 1;
	}
	*retStr = "";
	return 0;
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_VC
//------------------------------------------------------------------------------------------//
FIFO8	g_SMCCyCodeStream;
//------------------------------------------------------------------------------------------//
int32 SMC_BLOCK::Init(uint8 *cOffset,uint32 cCodeSize){
	int32	blRet;
	
	blRet = 0;
	if (blInit == 0){
		try{
			cgOldMemCode = Str_CharToASCIIStr(cOffset,cCodeSize,G_ESCAPE_OFF);
			cgMemOffset = cOffset;
			cgCodeSize = cCodeSize;
			blInit = 1;
			blRet = 1;
		}
		catch(...){
			blInit = 0;
			blRet = -1;
		}
	}
	return(blRet);
}
//------------------------------------------------------------------------------------------//
int32 SMC_BLOCK::Decrypt(void){
	int32	blRet,cyCodeNo;
	STDSTR	runCode;
	
	blRet = 0;
	if (blInit == 1){
		cyCodeNo = Str_CharToUint16((uint8*)cgOldMemCode.c_str());
		if (SMC_DecryptCYCodeBlock(&runCode,g_SMCCyCodeStream,cyCodeNo) != 0){
			if (cgCodeSize == runCode.length()){
				try{
					CopyMemory(cgMemOffset,runCode.c_str(),cgCodeSize);
					blRet = 1;
				}
				catch(...){
					blRet = -1;
				}
			}
		}
	}
	return(blRet);
}
//------------------------------------------------------------------------------------------//
int32 SMC_BLOCK::Resume(void){
	int32	blRet;
	
	blRet = 0;
	if (blInit == 1){
		try{
			CopyMemory(cgMemOffset,cgOldMemCode.c_str(),cgCodeSize);
			blRet = 1;
		}
		catch(...){
			blRet = -1;
		}
	}
	return(blRet);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif
