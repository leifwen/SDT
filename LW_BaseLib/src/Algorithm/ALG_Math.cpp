//
//  ALG_Math.cpp
//  SDT
//
//  Created by Leif Wen on 2019/4/7.
//  Copyright © 2019 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "ALG_Math.h"
#include "DS_STRING.h"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
namespace FAO32 {
	static void	FormatString	(STDSTR* strOut,const STDSTR& expressions);
	static void	GetPoland		(STDSTR* polandQueue,const STDSTR& expressions);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
uint32 ALG_FAO32Calc(STDSTR* result,const STDSTR& expressions){
	STDSTR	dataT,strPDQueue,strRet,dataO;
	uint32	o1,o2;
	
	FAO32::GetPoland(&strPDQueue,expressions);
	strRet = "";
	while(strPDQueue.length() > 0){
		dataT = Str_ReadSubItem(&strPDQueue, ",");
		if (dataT == "*"){
			dataO = Str_ReadSubItem(&strRet,",");
			o2 = atoi(Str_Trim(dataO).c_str());
			dataO = Str_ReadSubItem(&strRet,",");
			o1 = atoi(Str_Trim(dataO).c_str());
			o1 *= o2;
			strRet = Str_ToStr(o1) + "," + strRet;
		}
		else if (dataT == "/"){
			dataO = Str_ReadSubItem(&strRet,",");
			o2 = atoi(Str_Trim(dataO).c_str());
			dataO = Str_ReadSubItem(&strRet,",");
			o1 = atoi(Str_Trim(dataO).c_str());
			o1 /= o2;
			strRet = Str_ToStr(o1) + "," + strRet;
		}
		else if (dataT == "+"){
			dataO = Str_ReadSubItem(&strRet,",");
			o2 = atoi(Str_Trim(dataO).c_str());
			dataO = Str_ReadSubItem(&strRet,",");
			o1 = atoi(Str_Trim(dataO).c_str());
			o1 += o2;
			strRet = Str_ToStr(o1) + "," + strRet;
		}
		else if (dataT == "-"){
			dataO = Str_ReadSubItem(&strRet,",");
			o2 = atoi(Str_Trim(dataO).c_str());
			dataO = Str_ReadSubItem(&strRet,",");
			o1 = atoi(Str_Trim(dataO).c_str());
			o1 -= o2;
			strRet = Str_ToStr(o1) + "," + strRet;
		}
		else{
			strRet = dataT + "," + strRet;
		}
	}
	strRet = Str_ReadSubItem(&strRet,",");
	if (result != nullptr)
		*result = strRet;
	return(atoi(Str_Trim(strRet).c_str()));
};
//------------------------------------------------------------------------------------------//
static void FAO32::FormatString(STDSTR* strOut,const STDSTR& expressions){
	//control char :+ - * / ()
	
	STDSTR		strData;
	STRSIZE		i,length;
	uint8		charData;
	
	length = expressions.length();
	i = 0;
	*strOut = "(,";
	strData = "";
	while(i < length){
		charData = expressions[i];
		++ i;
		if ((charData == '(') || (charData == ')') || (charData == '+') || (charData == '-') || (charData == '*') || (charData == '/')){
			Str_TrimSelf(strData);
			if (strData.length() > 0){
				*strOut += strData;
				*strOut += ',';
			}
			*strOut += charData;
			*strOut += ',';
			strData = "";
		}
		else if ((charData >= '0') && (charData <= '9')){
			strData += charData;
		}
	}
	Str_TrimSelf(strData);
	if (strData.length() > 0){
		*strOut += strData;
		*strOut += ',';
	}
	*strOut += ")";
};
//------------------------------------------------------------------------------------------//
static void FAO32::GetPoland(STDSTR* polandQueue,const STDSTR& expressions){
	STDSTR		stackT,strExp;
	STDSTR		dataT,dataT2;
	
	*polandQueue = "";
	stackT = "";
	
	FormatString(&strExp,expressions);
	
	while(strExp.length() > 0){
		dataT = Str_ReadSubItem(&strExp,",");
		if ((dataT == "*") || (dataT == "/")){
			while(stackT.length() > 0){
				dataT2 = Str_ReadSubItem(&stackT, ",");
				if ((dataT2 == "(") || (dataT2 == "+") || (dataT2 == "-")){
					stackT = dataT + "," + dataT2 + "," + stackT;
					break;
				}
				*polandQueue += dataT2;
				*polandQueue += ',';
			}
		}
		else if ((dataT == "+") || (dataT == "-")){
			while(stackT.length() > 0){
				dataT2 = Str_ReadSubItem(&stackT, ",");
				if (dataT2 == "("){
					stackT = dataT + "," + dataT2 + "," + stackT;
					break;
				}
				*polandQueue += dataT2;
				*polandQueue += ',';
			}
		}
		else if (dataT == ")"){
			while(stackT.length() > 0){
				dataT2 = Str_ReadSubItem(&stackT, ",");
				if (dataT2 == "(")
					break;
				*polandQueue += dataT2;
				*polandQueue += ',';
			}
		}
		else {
			stackT = dataT + "," + stackT;
		}
	}
};
//------------------------------------------------------------------------------------------//
