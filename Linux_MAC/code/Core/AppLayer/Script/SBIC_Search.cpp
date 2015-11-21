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
#include "Device.h"
#include "CommandGList.h"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 SBIC_Search::Help(SBICPAR *tBICPAR,int32 blDetail)const{
	PrintB(tBICPAR,".CMD = Search=<Expression> -->Search expression in received data, and save the result.");
	PrintB(tBICPAR,"  Command = <'Search=<Expression>>[//COMMENT]");
	PrintP(tBICPAR,"  Notes: Expression's operators is * and ?, and only support ASCII format.");
	PrintP(tBICPAR,"     1.* means any character, ? means one character, support multiple * and ? combination.");
	PrintP(tBICPAR,"     2.The results returned are the data replaced by * and ?.");
	PrintP(tBICPAR,"     3.Only record the first match of the results.");
	PrintP(tBICPAR,"     4.Use 'RET (x) to get the results.");
	PrintP(tBICPAR,"     5.support \\0xhh, \\0Xhh, \\a, \\b, \\f, \\n, \\r, \\t, \\v, \\\\, \\', \\\", \\0, \\/, \\*, \\?.");
	PrintP(tBICPAR,"   eg:");
	PrintP(tBICPAR,"    Received data:HappyNewYear");
	PrintP(tBICPAR,"      Command = 'Search=Happy*Year     //Search Happy*Year, and record New as the result");
	PrintP(tBICPAR,"      'RET(1)=New");
	PrintP(tBICPAR,"    Received data:Name: Leif Wen.MP: +86-138-6062-4143.");
	PrintP(tBICPAR,"      Command = 'Search=Name: *.MP: *. //Search Name: *.MP: *., and record Leif Wen and +86-138-6062-4143 as result");
	PrintP(tBICPAR,"      'RET(1)=Leif Wen");
	PrintP(tBICPAR,"      'RET(2)=+86-138-6062-4143");
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
int32 SBIC_Search::Command(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const{
	int32 		i;
	std::string	strPData1,strPData2,strTempData,strSearchRet;
	*ret = 'F';
	strSearchRet = "";
	if (tBICPAR != nullptr){
		if (Search(tBICPAR->cgRecvbuf,par,&tBICPAR->cgSearchRet) != 0){
			*ret = 'T';
		}
		else if (tBICPAR->cgCommand != nullptr){
			tBICPAR->cgCommand->StrCatch = "Fail";
			++ tBICPAR->cgCommand->catchTimes;
		}
		if ((tBICPAR->cgDevice != nullptr) && (tBICPAR->cgDevice->cEDevFlag.blEnablePrintSBICinfo != 0) && (tBICPAR->cgDevice->cEDevFlag.blCommandExplain != 0)){
			strPData1 = SYS_MakeTimeNow();
			strPData1 += " Execute: ";
			strPData1 += "Search:: ";
			strPData1 += par;
			strPData1 += "\r\n";
			
			i = 0;
			strPData2 = "";
			strSearchRet = tBICPAR->cgSearchRet;
			while(strSearchRet.length() > 0){
				++ i;
				strTempData = Str_ReadSubItem(&strSearchRet,",");
				strPData2 += "  RET(";
				strPData2 += Str_IntToString(i);
				strPData2 += ") = ";
				strPData2 += Str_HEXToASCII(strTempData);
				strPData2 += "\r\n";
			}
			if (tBICPAR->cgDevice->cgODevList.cgOutput != nullptr){
				tBICPAR->cgDevice->cgODevList.cgOutput->Spin_InUse_set();
				tBICPAR->cgDevice->cgODevList.cgOutput->WriteDividingLine(RICH_CF_clMaroon);
				tBICPAR->cgDevice->cgODevList.cgOutput->WriteStrN(strPData1,RICH_CF_clMaroon);
				tBICPAR->cgDevice->cgODevList.cgOutput->WriteStr(strPData2,RICH_CF_clPurple);
				tBICPAR->cgDevice->cgODevList.cgOutput->Spin_InUse_clr();
			}
		}
		tBICPAR->cgFunRet = *ret;
	}
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
void SBIC_Search::FormatString(const std::string &strInput,std::string *returnStr){
	//formate to HEX without space
	//escape *,?
	std::string				strData,strResult;
	std::string::size_type	i,length;
	uint8					charData;
	
	length = strInput.length();
	i = 0;
	*returnStr = "";
	strData = "";
	while(i < length){
		charData = strInput[i];
		++ i;
		if ((charData == '*') || (charData == '?')){
			*returnStr += Str_ASCIIToHEX(strData,G_ESCAPE_ON);
			*returnStr += charData;
			strData = "";
		}
		else{
			if ((charData == '\\') && (i < length)){	//   (\*)(\?)
				charData = strInput[i];
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
int32 SBIC_Search::Search(const std::string &strContent,const std::string &stringSearch,std::string *retStr){
	//strContent is HEX without space
	//strSearch is ASCII,escape \0xhh,\0Xhh,\a,\b,\f,\n,\r,\t,\v,\\,\',\",\0,\/,\*,\??
	std::string	strD1,strSearch;
	std::string::size_type	strContentPostion,returnContentPostion;
	std::string				result;
	
	FormatString(stringSearch,&strSearch);
	strContentPostion = 0;
	
	do{
		result = "";
		if (SearchSubString(&result,strContent,strContentPostion,strSearch,0,&returnContentPostion) != 0)
			break;
		strContentPostion += 2;
	}while(strContentPostion < strContent.length());
	*retStr = result;
	
	return(result.length() > 0);
}
//------------------------------------------------------------------------------------------//
int32  SBIC_Search::SearchSubString(std::string *result
									,const std::string &strContent
									,std::string::size_type strContentPostion
									,const std::string &strSearch
									,std::string::size_type strSearchPostion
									,std::string::size_type	*returnContentPostion){
	//处理AA*,AA?,AA
	//*AA,?AA在上一层程序中滤除
	std::string::size_type		ret;
	std::string					strD1,strReslut,strD2,strSubRet;
	std::string::size_type		strContentNewPostion,strContentNextPostion,reContentPostion;
	std::string::size_type		symbolLength,subSearchLength;
	int32						blOnlyQuestionMark;
	
	strReslut = "";
	strSubRet = *result;
	strD1 = SplitLetter(strSearch,strSearchPostion,&strSearchPostion);//读出一个比较值
	
	ret = Str_DCompare(strContent,strD1,strContentPostion);
	if (ret == std::string::npos)
		return 0;
	
	strContentNewPostion = ret;//搜到strD1的新位置,
	strContentNextPostion = (strContentNewPostion + strD1.length());	//计算下一次预搜索的位置,此位置不包含运算符长度
	
	if (SplitSymbol(strSearch,strSearchPostion,&strSearchPostion,&symbolLength,&blOnlyQuestionMark) != 0){//有通配符,需要将结果入栈
		if (strSearchPostion >= strSearch.length()){					//搜索字符串以运算符结尾,本次为最后的比较
			if (blOnlyQuestionMark != 0){										//只有?运算符,比较个数
				if ((strContent.length() - strContentNextPostion) != symbolLength)
					return 0;
			}
			if ((strContent.length() - strContentNextPostion) < symbolLength)
				return 0;
			
			strReslut = strContent.substr(strContentNextPostion);
			if ((symbolLength > 0) && (blOnlyQuestionMark == 0)){
				strD2 = strReslut.substr(strReslut.length() - symbolLength);
				strReslut = strReslut.substr(0,strReslut.length() - symbolLength);
				if (strReslut.length() == 0){
					strReslut = strD2;
				}else if (strD2.length() > 0){
					strReslut = strReslut + "," + strD2;
				}
			}
			if (result->length() == 0){
				*result = strReslut;
			}
			else if (strReslut.length() > 0){
				*result = strReslut + "," + *result;
			}
			*returnContentPostion = strContentNewPostion;//纪录本次搜索到的字符的首位置
			return 1;
		}
		subSearchLength = 0;
		do{
			*result = strSubRet;
			if (SearchSubString(result,strContent,strContentNextPostion + symbolLength + subSearchLength,strSearch,strSearchPostion,&reContentPostion) != 0){
				if (blOnlyQuestionMark != 0){//搜索字符中只包含?,需要检测其间的个数
					if ((reContentPostion - strContentNextPostion) != symbolLength)//实际找到的位置不等于?的个数,无效
						return 0;
				}
				strReslut = strContent.substr(strContentNextPostion,reContentPostion - strContentNextPostion);
				if ((symbolLength > 0) && (blOnlyQuestionMark == 0)){
					strD2 = strReslut.substr(strReslut.length() - symbolLength);
					strReslut = strReslut.substr(0,strReslut.length() - symbolLength);
					if (strReslut.length() == 0){
						strReslut = strD2;
					}else if (strD2.length() > 0){
						strReslut = strReslut + "," + strD2;
					}
				}
				if (result->length() == 0){
					*result = strReslut;
				}
				else if (strReslut.length() > 0){
					*result = strReslut + "," + *result;
				}
				*returnContentPostion = strContentNewPostion;//纪录本次搜索到的字符的首位置
				return 1;
			}
			subSearchLength += 2;
		}while(strContentNextPostion + symbolLength + subSearchLength < strContent.length());
		return 0;
	}
	*returnContentPostion = strContentNewPostion;//纪录本次搜索到的字符的首位置
	return 1;
}
//------------------------------------------------------------------------------------------//
std::string  SBIC_Search::SplitLetter(const std::string &strSearch
									  ,std::string::size_type strSearchPostion
									  ,std::string::size_type *returnSearchPostion){
	//分割搜索字符串,返回*或?之前的字符串,以及*或?的位置
	std::string	strD1;
	uint8	charData;
	std::string::size_type	i,lengthStrSearch;
	
	lengthStrSearch = strSearch.length();
	strD1 = "";
	i = strSearchPostion;
	do{
		charData = strSearch[i];
		if ((charData == '*') || (charData == '?'))
			break;
		strD1 += charData;
	}while(++i < lengthStrSearch);
	*returnSearchPostion = i;
	if (strD1.length() == 0)
		*returnSearchPostion = strSearchPostion;
	return(strD1);
}
//------------------------------------------------------------------------------------------//
int32  SBIC_Search::SplitSymbol(const std::string &strSearch
								,std::string::size_type strSearchPostion
								,std::string::size_type *returnSearchPostion
								,std::string::size_type *symbolLength
								,int32 *blOnlyQuestionMark){
	//分割运算符*和?
	//返回运算*长度,类型,以及新的位置坐标
	//3 type symbol 1-->*,2-->*?,3--?
	std::string	strD1;
	uint8	charData;
	std::string::size_type	lengthStrSearch;
	int32	blSymbol,symbolType;
	
	lengthStrSearch = strSearch.length();
	
	strD1 = "";
	blSymbol = 0;
	*symbolLength = 0;
	*blOnlyQuestionMark = 1;//if ?
	symbolType = 0;
	do{
		charData = strSearch[strSearchPostion];
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
			*symbolLength += 2;
			continue;
		}
		break;
	}while(++strSearchPostion < lengthStrSearch);
	*returnSearchPostion = strSearchPostion;			//下次搜索的位置
	return(blSymbol);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
