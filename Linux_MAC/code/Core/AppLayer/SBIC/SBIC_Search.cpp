/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: SBIC_Search.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.08.12
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "SBIC_Search.h"
#include "Comm_Convert.h"
#include "CommandGList.h"
//------------------------------------------------------------------------------------------//
#ifdef SBIC_SearchH
//------------------------------------------------------------------------------------------//
int32 SBIC_Search::Command(SBIC_ENV *env,const STDSTR &par,void *p)const{
	int32 	i;
	STDSTR	strTempData,strSearchRet;
	OUTPUT_NODE	*cOut;
	
	env->cgFunRet = 'F';
	strSearchRet = "";
	if (env != nullptr){
		if (Search(&env->cgSearchRet,env->cstdin_Str,par) != 0){
			env->cgFunRet = 'T';
		}
		else if (env->cgCommand != nullptr){
			env->cgCommand->StrCatch = "Fail";
			++ env->cgCommand->catchTimes;
		}
		if ((CheckPrintSBICinfo(env) != 0) && (CheckCommandExplain(env) != 0)){
			i = 0;
			strSearchRet = env->cgSearchRet;
			DeviceLock(env);
			cOut = GetDevOut(env);
			PrintNL(cOut) << COL_DivLineTime << COL_NormalMessage << " Execute: Search:: " << par << "\n"
			<< COL_clRed;
			while(strSearchRet.length() > 0){
				*cOut << "  RET(" << Str_ToString(++i) << ") = " << Str_HEXToASCII(Str_ReadSubItem(&strSearchRet,",")) << "\n";
			};
			*cOut << Endl;
			DeviceUnlock(env);
		}
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
void SBIC_Search::FormatString(STDSTR *returnStr,const STDSTR &strIn){
	//formate to HEX without space
	//escape *,?
	STDSTR				strData,strResult;
	STDSTR::size_type	i,length;
	uint8				charData;
	
	length = strIn.length();
	i = 0;
	*returnStr = "";
	strData = "";
	while(i < length){
		charData = strIn[i];
		++ i;
		if ((charData == '*') || (charData == '?')){
			*returnStr += Str_ASCIIToHEX(strData,G_ESCAPE_ON);
			*returnStr += charData;
			strData = "";
		}
		else{
			if ((charData == '\\') && (i < length)){	//   (\*)(\?)
				charData = strIn[i];
				++ i;
				if ((charData != '*') && (charData != '?'))
					strData += '\\';
			}
			strData += charData;
		}
	}
	*returnStr += Str_ASCIIToHEX(strData,G_ESCAPE_ON);
}
//------------------------------------------------------------------------------------------//
STDSTR  SBIC_Search::SplitLetter(STDSTR::size_type *offset,const STDSTR &searchStr){
	STDSTR	subStr;
	uint8	charData;
	STDSTR::size_type	i,length;
	
	length = searchStr.length();
	subStr = "";
	i = *offset;
	do{
		charData = searchStr[i];
		if ((charData == '*') || (charData == '?'))
			break;
		subStr += charData;
	}while(++i < length);
	
	if (subStr.length() > 0)
		*offset = i;
	return(Str_HEXToASCII(subStr));
}
//------------------------------------------------------------------------------------------//
int32  SBIC_Search::SplitSymbol(const STDSTR &strSearch,STDSTR::size_type *offset,STDSTR::size_type *symbolLength,int32 *blOnlyQuestionMark){
	//3 type symbol 1-->*,2-->*?,3--?
	STDSTR	strD1;
	uint8	charData;
	STDSTR::size_type	length;
	int32				blSymbol,symbolType;
	
	length = strSearch.length();
	
	strD1 = "";
	blSymbol = 0;
	*symbolLength = 0;
	*blOnlyQuestionMark = 1;//if ?
	symbolType = 0;
	do{
		charData = strSearch[*offset];
		if (charData == '*'){
			if (symbolType == 0)
				symbolType = 1;
			if (symbolType == 1){
				blSymbol = 1;
				*blOnlyQuestionMark = 0;
				continue;
			}
		}else if (charData == '?'){
			if (symbolType == 0)
				symbolType = 3;
			blSymbol = 1;
			*symbolLength += 1;
			continue;
		}
		break;
	}while(++(*offset) < length);
	return(blSymbol);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_Search::Search(STDSTR *retStr,const STDSTR &strContent,const STDSTR &searchString){
	//strContent is ASCII
	//strSearch is ASCII,escape \0xhh,\0Xhh,\a,\b,\f,\n,\r,\t,\v,\\,\',\",\0,\/,\*,\??
	STDSTR::size_type	strContentPostion,returnContentPostion;
	STDSTR	strD1,searchStr;
	STDSTR	result;
	
	FormatString(&searchStr,searchString);
	strContentPostion = 0;
	
	do{
		result = "";
		if (SearchSubString(&result,strContent,strContentPostion,searchStr,0,&returnContentPostion) != 0)
			break;
		strContentPostion += 1;
	}while(strContentPostion < strContent.length());
	if (retStr != nullptr)
		*retStr = result;
	return(result.length() > 0);
}
//------------------------------------------------------------------------------------------//
int32  SBIC_Search::SearchSubString(STDSTR *result
									,const STDSTR &strContent,STDSTR::size_type strContentPostion
									,const STDSTR &strSearch,STDSTR::size_type searchStrOffset
									,STDSTR::size_type	*returnContentPostion){
	STDSTR					strD1,strReslut,strD2,strSubRet;
	STDSTR::size_type		strContentNextPostion,reContentPostion;
	STDSTR::size_type		symbolLength,subSearchLength;
	int32					blOnlyQuestionMark;
	
	strReslut = "";
	strSubRet = *result;
	strD1 = SplitLetter(&searchStrOffset,strSearch);
	
	strContentPostion = strContent.find(strD1,strContentPostion);
	if (strContentPostion == STDSTR::npos)
		return 0;
	
	strContentNextPostion = (strContentPostion + strD1.length());
	
	if (SplitSymbol(strSearch,&searchStrOffset,&symbolLength,&blOnlyQuestionMark) != 0){
		if (searchStrOffset >= strSearch.length()){
			if (blOnlyQuestionMark != 0){
				if ((strContent.length() - strContentNextPostion) != symbolLength)
					return 0;
			}
			if ((strContent.length() - strContentNextPostion) < symbolLength)
				return 0;
			
			strReslut = strContent.substr(strContentNextPostion);
			if ((symbolLength > 0) && (blOnlyQuestionMark == 0)){//*??
				strD2 = strReslut.substr(strReslut.length() - symbolLength);
				strReslut = strReslut.substr(0,strReslut.length() - symbolLength);
				if (strReslut.length() == 0){
					strReslut = Str_ASCIIToHEX(strD2, G_ESCAPE_OFF);
				}else if (strD2.length() > 0){
					strReslut = Str_ASCIIToHEX(strReslut,G_ESCAPE_OFF) + "," + Str_ASCIIToHEX(strD2,G_ESCAPE_OFF);
				}
			}
			else{
				strReslut = Str_ASCIIToHEX(strReslut,G_ESCAPE_OFF);
			}
			if (result->length() == 0){
				*result = strReslut;
			}
			else if (strReslut.length() > 0){
				*result = strReslut + "," + *result;
			}
			*returnContentPostion = strContentPostion;
			return 1;
		}
		subSearchLength = 0;
		do{
			*result = strSubRet;
			if (SearchSubString(result,strContent,strContentNextPostion + symbolLength + subSearchLength,strSearch,searchStrOffset,&reContentPostion) != 0){
				if (blOnlyQuestionMark != 0){
					if ((reContentPostion - strContentNextPostion) != symbolLength)
						return 0;
				}
				strReslut = strContent.substr(strContentNextPostion,reContentPostion - strContentNextPostion);
				if ((symbolLength > 0) && (blOnlyQuestionMark == 0)){
					strD2 = strReslut.substr(strReslut.length() - symbolLength);
					strReslut = strReslut.substr(0,strReslut.length() - symbolLength);
					if (strReslut.length() == 0){
						strReslut =  Str_ASCIIToHEX(strD2,G_ESCAPE_OFF);
					}else if (strD2.length() > 0){
						strReslut = Str_ASCIIToHEX(strReslut,G_ESCAPE_OFF) + "," + Str_ASCIIToHEX(strD2,G_ESCAPE_OFF);
					}
				}
				else{
					strReslut = Str_ASCIIToHEX(strReslut,G_ESCAPE_OFF);
				}
				if (result->length() == 0){
					*result = strReslut;
				}
				else if (strReslut.length() > 0){
					*result = strReslut + "," + *result;
				}
				*returnContentPostion = strContentPostion;
				return 1;
			}
			subSearchLength += 1;
		}while(strContentNextPostion + symbolLength + subSearchLength < strContent.length());
		return 0;
	}
	*returnContentPostion = strContentPostion;
	return 1;
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
