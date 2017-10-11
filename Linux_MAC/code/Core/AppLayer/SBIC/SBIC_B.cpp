/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: SBIC_B.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.08.12
 
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "SBIC_B.h"
#include "Comm_Convert.h"
#include "Device.h"
//------------------------------------------------------------------------------------------//
#ifdef SBIC_BH
//------------------------------------------------------------------------------------------//
int32 SBIC_Node::Check(SBIC_ENV *env,const STDSTR &rawIn,STDSTR *retPar)const{
	STDSTR	oCommand,oFullCMD,tmpCMD,retT,strT;
	STDSTR	subCommand,subStart,subEnd;
	oFullCMD = Str_LTrim(rawIn);
	oCommand = Str_LowerCase(cgCommand);
	
	do{
		tmpCMD = oFullCMD;
		subEnd = Str_ReadSubItem(&oCommand,"/");
		subCommand = "'" + Str_ReadSubItem(&subEnd,",");
		subStart = Str_ReadSubItem(&subEnd,",");
		if (subStart.length() > 0){
			strT = Str_LowerCase(Str_RTrim(Str_ReadSubItem(&tmpCMD,subStart)));
			if (strT == subCommand){
				if (subEnd.length() > 0){
					if (tmpCMD.length() >= subEnd.length()){
						strT = tmpCMD.substr(tmpCMD.length() - subEnd.length());
						if (strT == subEnd){
							*retPar = Str_Trim(tmpCMD.substr(0,tmpCMD.length() - subEnd.length()));
							return(cgReturnCode);
						}
					}
				}
				else{
					*retPar = Str_Trim(tmpCMD);
					return(cgReturnCode);
				}
			}
		}
		else if (Str_LowerCase(tmpCMD) == subCommand){
			*retPar = "";
			return(cgReturnCode);
		}
	}while (oCommand.length() > 0);
	*retPar = "";
	return(SBI_RETCODE_NO);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_Node::Execute(SBIC_ENV *env,const STDSTR &rawIn,void *p)const{
	STDSTR	 retPar;
	
	if (Check(env,rawIn,&retPar) == cgReturnCode)
		return(Command(env,retPar,p));
	if (cgblTrySubCMD != 0)
		return(Command(env,rawIn,p));
	return(SBI_RETCODE_NO);
}
//------------------------------------------------------------------------------------------//
STDSTR SBIC_TOOLS::DelComment(const STDSTR &strIn){
	//  escape \/ ,//
	//  abcd \\//
	STDSTR::size_type	i,length;
	STDSTR		strRet;
	uint8		charData,charData1;
	
	length = strIn.length();
	i = 0;
	strRet = "";
	
	if  (length > 0){
		while(i++ < length){
			charData = strIn[i - 1];
			if (i < length){
				charData1 = strIn[i];
				if ((charData == '/') && (charData1 == '/'))
					break;
				if ((charData == '\\') || (charData == '/')){
					strRet += charData;
					strRet += charData1;
					++ i;
					continue;
				}
			}
			strRet += charData;
		};
	}
	return(Str_Trim(strRet));
}
//------------------------------------------------------------------------------------------//
int32 SBIC_TOOLS::GetMS(const STDSTR &par){
	STDSTR	strUint;
	double	ms;
	
	ms = atof(par.c_str()) * 1000;
	if (par.length() > 3){
		strUint = Str_LowerCase(par.substr(par.length() - 2));
		if (strUint == "ms")
			ms /= 1000;
	}
	return((int32)ms);
}
//------------------------------------------------------------------------------------------//
STDSTR SBIC_TOOLS::GetMSSTR(int32 ms){
	STDSTR	ret;
	
	if (ms >= 1000){
		ret = Str_FloatToString((double)ms / 1000.0) + "s";
	}
	else if (ms > 0){
		ret = Str_ToString(ms) + "ms";
	}
	else{
		ret = "0s";
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
void SBIC_TOOLS::DeviceLock(SBIC_ENV *env){
#ifdef DeviceH
	env->eda->device->Spin_Child_set();
#endif
}
//------------------------------------------------------------------------------------------//
void SBIC_TOOLS::DeviceUnlock(SBIC_ENV *env){
#ifdef DeviceH
	env->eda->device->Spin_Child_clr();
#endif
}
//------------------------------------------------------------------------------------------//
OUTPUT_NODE	*SBIC_TOOLS::GetDevOut(SBIC_ENV *env){
#ifdef DeviceH
	return(env->eda->device->GetSelStdout());
#else
	return(nullptr);
#endif
}
//------------------------------------------------------------------------------------------//
void SBIC_TOOLS::PrintExecute(SBIC_ENV *env,const COLSTR &colStr2,const COLSTR &colStr3,const COLSTR &colStr4
							  ,const COLSTR &colStr5,const COLSTR &colStr6,const COLSTR &colStr7,const COLSTR &colStr8){
	if (env != nullptr){
		DeviceLock(env);
		if (CheckPrintSBICinfo(env) != 0)
			PrintWithDividingLine(GetDevOut(env),"Execute:",colStr2,colStr3,colStr4,colStr5,colStr6,colStr7,colStr8);
		DeviceUnlock(env);
	}
}
//------------------------------------------------------------------------------------------//
void SBIC_TOOLS::PrintStr(OUTPUT_NODE *node,const COLSTR &colStr1,const COLSTR &colStr2,const COLSTR &colStr3,const COLSTR &colStr4
						  ,const COLSTR &colStr5,const COLSTR &colStr6,const COLSTR &colStr7,const COLSTR &colStr8){
	OUTPUT_NODE::PrintStr(node,colStr1,colStr2,colStr3,colStr4,colStr5,colStr6,colStr7,colStr8);
}
//------------------------------------------------------------------------------------------//
void SBIC_TOOLS::PrintStrNL(OUTPUT_NODE *node,const COLSTR &colStr1,const COLSTR &colStr2,const COLSTR &colStr3,const COLSTR &colStr4
						  ,const COLSTR &colStr5,const COLSTR &colStr6,const COLSTR &colStr7,const COLSTR &colStr8){
	OUTPUT_NODE::PrintStrNL(node,colStr1,colStr2,colStr3,colStr4,colStr5,colStr6,colStr7,colStr8);
}
//------------------------------------------------------------------------------------------//
void SBIC_TOOLS::PrintWithTime(OUTPUT_NODE *node,const COLSTR &colStr1,const COLSTR &colStr2,const COLSTR &colStr3,const COLSTR &colStr4
							   ,const COLSTR &colStr5,const COLSTR &colStr6,const COLSTR &colStr7,const COLSTR &colStr8){
	OUTPUT_NODE::PrintWithTime(node,colStr1,colStr2,colStr3,colStr4,colStr5,colStr6,colStr7,colStr8);
}
//------------------------------------------------------------------------------------------//
void SBIC_TOOLS::PrintWithDividingLine(OUTPUT_NODE *node,const COLSTR &colStr1,const COLSTR &colStr2,const COLSTR &colStr3,const COLSTR &colStr4
									   ,const COLSTR &colStr5,const COLSTR &colStr6,const COLSTR &colStr7,const COLSTR &colStr8){
	OUTPUT_NODE::PrintWithDividingLine(node,colStr1,colStr2,colStr3,colStr4,colStr5,colStr6,colStr7,colStr8);
}
//------------------------------------------------------------------------------------------//
void SBIC_TOOLS::PrintMessage(OUTPUT_NODE *node,const COLSTR &colStr1,const COLSTR &colStr2,const COLSTR &colStr3,const COLSTR &colStr4
							  ,const COLSTR &colStr5,const COLSTR &colStr6,const COLSTR &colStr7,const COLSTR &colStr8){
	OUTPUT_NODE::PrintMessage(node,colStr1,colStr2,colStr3,colStr4,colStr5,colStr6,colStr7,colStr8);
}
//------------------------------------------------------------------------------------------//
void SBIC_TOOLS::PrintNormalMessage(OUTPUT_NODE *node,const STDSTR &strData1,const STDSTR &strData2,const STDSTR &strData3,const STDSTR &strData4
									,const STDSTR &strData5,const STDSTR &strData6,const STDSTR &strData7,const STDSTR &strData8){
	OUTPUT_NODE::PrintNormalMessage(node,strData1,strData2,strData3,strData4,strData5,strData6,strData7,strData8);
}
//------------------------------------------------------------------------------------------//
void SBIC_TOOLS::PrintWarningMessage(OUTPUT_NODE *node,const STDSTR &strData1C1,const STDSTR &strData2C2,const STDSTR &strData3C2,const STDSTR &strData4C2
									 ,const STDSTR &strData5C2,const STDSTR &strData6C2,const STDSTR &strData7C2,const STDSTR &strData8C2){
	OUTPUT_NODE::PrintWarningMessage(node,strData1C1,strData2C2,strData3C2,strData4C2,strData5C2,strData6C2,strData7C2,strData8C2);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
