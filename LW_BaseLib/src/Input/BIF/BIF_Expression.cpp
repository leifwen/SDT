//
//  BIF_Expression.cpp
//  SDT
//
//  Created by Leif Wen on 09/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "BIF_Expression.h"
#ifdef BIF_Expression_h
#include "BIF_Transform.h"
#include "DS_STRING.h"
//------------------------------------------------------------------------------------------//
CMDID BIF_RECEIVE::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = Receive==<par> -->Check recevie buffer status.");
	PrintB(env,"  Command = <'Receive==<par>>");
	PrintP(env,"  Notes:1.support * and ?.");
	PrintP(env,"        2.Expression Operators is +.");
	PrintP(env,"        3.Can use \"\" set string. Use \\\", \\+ to escape \", +.");
	PrintP(env,"        4.Support \\0xhh, \\0Xhh, \\a, \\b, \\f, \\n, \\r, \\t, \\v, \\\\, \\', \\\", \\0, \\/, \\*, \\?.");
	PrintP(env,"        5.support sub command :same as 'combine.");
	PrintP(env,"   eg:");
	PrintP(env,"     Condition Expression = 'Receive== OK   //Expression is true if receive buffer contain OK");
	PrintP(env,"     Condition Expression = 'Receive== O+K  //Expression is true if receive buffer contain OK");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_RECEIVE::Command(CMD_ENV* env,const STDSTR& msg,void* retTrue)const{
	STDSTR *ret = static_cast<STDSTR*>(retTrue);
	STDSTR strForSend;

	*ret = 'F';
	if (env != nullptr){
		BIF_Transform(env,&strForSend,nullptr,msg,CMD_NONE,G_ESCAPE_ON);
		if (Str_Find(nullptr,BIF_ENV::STDIN(env),strForSend)){
			*ret = 'T';
		}
		else if (BIF_ENV::STDIN(env).find(strForSend) != STDSTR::npos){
			*ret = 'T';
		}
	}
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_TIMEOUT::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = Timeout -->Check timeout status.Used as sub-command in Condition Expression.");
	PrintB(env,"  Command = <'Timeout>");
	PrintP(env,"   eg:");
	PrintP(env,"     Condition Expression = 'Timeout  //Expression is true if Timeout");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_TIMEOUT::Command(CMD_ENV* env,const STDSTR& msg,void* retTrue)const{
	STDSTR *ret = static_cast<STDSTR*>(retTrue);
	
	*ret = 'F';
	if (env != nullptr){
		if (SYS_Delay_CheckTS(&BIF_ENV::RetDTime(env)))
			*ret = 'T';
	}
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_NULLPTR::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = Nullptr -->Check receive buffer status.Used as sub-command in Condition Expression.");
	PrintB(env,"  Command = <'Nullptr>");
	PrintP(env,"   eg:");
	PrintP(env,"     Condition Expression = 'Nullptr  //Expression is true if nothing received");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_NULLPTR::Command(CMD_ENV* env,const STDSTR& msg,void* retTrue)const{
	STDSTR *ret = static_cast<STDSTR*>(retTrue);
	
	*ret = 'F';
	if ((env != nullptr) && (BIF_ENV::STDIN(env).length() == 0))
		*ret = 'T';
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_RX_MORE::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = RX> <num> -->Check receive buffer status.Used as sub-command in Condition Expression.");
	PrintB(env,"  Command = <'RX> <num>>");
	PrintP(env,"   eg:");
	PrintP(env,"     Condition Expression = 'RX>10   //Expression is true if the bytes received is more then 10");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_RX_MORE::Command(CMD_ENV* env,const STDSTR& msg,void* retTrue)const{
	STDSTR *ret = static_cast<STDSTR*>(retTrue);
	
	*ret = 'F';
	if ((env != nullptr) && (BIF_ENV::STDIN(env).length() > (STRSIZE)strtoll(msg.c_str(),nullptr,10)))
		*ret = 'T';
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_RX_LESS::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = RX< <num> -->Check receive buffer status.Used as sub-command in Condition Expression.");
	PrintB(env,"  Command = <'RX< <num>>");
	PrintP(env,"   eg:");
	PrintP(env,"     Condition Expression = 'RX<10   //Expression is true if the bytes received is less then 10");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_RX_LESS::Command(CMD_ENV* env,const STDSTR& msg,void* retTrue)const{
	STDSTR *ret = static_cast<STDSTR*>(retTrue);
	
	*ret = 'F';
	if ((env != nullptr) && (BIF_ENV::STDIN(env).length() < (STRSIZE)strtoll(msg.c_str(),nullptr,10)))
		*ret = 'T';
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_RX_EQUAL::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = RX==<num> -->Check receive buffer status.Used as sub-command in Condition Expression.");
	PrintB(env,"  Command = <'RX==<num>>");
	PrintP(env,"   eg:");
	PrintP(env,"     Condition Expression = 'RX==10  //Expression is true if the bytes received equal to 10");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_RX_EQUAL::Command(CMD_ENV* env,const STDSTR& msg,void* retTrue)const{
	STDSTR *ret = static_cast<STDSTR*>(retTrue);
	
	*ret = 'F';
	if ((env != nullptr) && (BIF_ENV::STDIN(env).length() == (STRSIZE)strtoll(msg.c_str(),nullptr,10)))
		*ret = 'T';
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_LCS::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = LCS==<T|F> -->Check last command execution status.");
	PrintB(env,"  Command = <'LCS==<T|F>>");
	PrintP(env,"  Notes:1.T is true,F is false.");
	PrintP(env,"        2.support command,'Search, 'Open, 'Close, 'RunEXE, 'StopEXE, 'WaitEXE, 'GetEXE.");
	PrintP(env,"   eg:");
	PrintP(env,"     Last command is 'Open and execute successful.");
	PrintP(env,"     Condition Expression = 'LCS==T  //Expression is true if Open execute successful");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_LCS::Command(CMD_ENV* env,const STDSTR& msg,void* retTrue)const{
	STDSTR *ret = static_cast<STDSTR*>(retTrue);
	STDSTR strPar;
	
	*ret = 'F';
	if (env != nullptr){
		strPar = Str_LowerCase(Str_Trim(msg));
		if (BIF_ENV::RetFun(env) == "T"){
			if ((strPar == "t") || (strPar == "1") || (strPar == "true"))
				*ret = 'T';
		}
		else if ((strPar == "f") || (strPar == "0") || (strPar == "false")){
			*ret = 'T';
		}
	}
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
BIF_CONDITION::BIF_CONDITION(void) : BIF_BASE() {
	SetSFlag(CMD_blTrySubCMD);
	cgCommandID = BIF_ID_CONDITION;
	cgCommand = "Condition,=";
	
	Add(cgSubC_RECEIVE)
	< cgSubC_TIMEOUT
	< cgSubC_NULLPTR < cgSubC_RX_MORE < cgSubC_RX_LESS < cgSubC_RX_EQUAL
	< cgSubC_LCS;
};
//------------------------------------------------------------------------------------------//
CMDID BIF_CONDITION::Help(CMD_ENV* env,uint32 flag)const{
	TraversalChildHelp(env,flag);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_CONDITION::Dispose(CMD_ENV* env,const STDSTR& rawIn,void* retTrue)const{
	STDSTR *ret = static_cast<STDSTR*>(retTrue);

	*ret = "F";
	return(BIF_BASE::Dispose(env,rawIn,retTrue));
}
//------------------------------------------------------------------------------------------//
CMDID BIF_CONDITION::Command(CMD_ENV* env,const STDSTR& msg,void* retTrue)const{
	STDSTR *ret = static_cast<STDSTR*>(retTrue);
	
	*ret = 'F';
	return(TraversalChildExecute(env,CMD_ID_NO,msg,retTrue));
}
//------------------------------------------------------------------------------------------//
BIF_CONDITION& BIF_CONDITION::GetCondition(void){
	static BIF_CONDITION	sgCondition;
	return(sgCondition);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
namespace BIFEXP {
	static void	FormatString	(STDSTR* returnStr,const STDSTR& strIn);
	static void	GetPoland		(STDSTR* polandQueue,const STDSTR& expressions);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
bool32 BIF_CheckResult(SBUF* _in,const STDSTR &expressions,int32 timeoutMS){
	CMD_ENV	env;
	
	BIF_ENV::InitQuantity(&env);
	BIF_ENV::Init(&env);
	
	SYS_Delay_SetTS(&BIF_ENV::RetDTime(&env),timeoutMS);
	
	BIF_ENV::STDIN(&env) = "";
	do{
		_in->Get(nullptr, &BIF_ENV::STDIN(&env), -1);
		if (BIF_Expression(&env,expressions))
			return G_TRUE;
	}while(SYS_Delay_CheckTS(&BIF_ENV::RetDTime(&env)) == G_FALSE);
	
	BIF_ENV::DeInit(&env);
	return G_FALSE;
}
//------------------------------------------------------------------------------------------//
bool32 BIF_Expression(CMD_ENV* env,const STDSTR& expressions){
	STDSTR	dataT,strPDQueue,strRet,fdata1,fdata2,strSub,strSR,caclRet;
	static BIF_RECEIVE		sgReceive;
	
	BIFEXP::GetPoland(&strPDQueue,expressions);
	strRet = "";
	while(strPDQueue.length() > 0){
		dataT = Str_ReadSubItem(&strPDQueue, ",");
		if (dataT == "&"){
			fdata1 = Str_ReadSubItem(&strRet,",");
			fdata2 = Str_ReadSubItem(&strRet,",");
			if ((fdata1 == "T") && (fdata2 == "T")){
				strRet = "T," + strRet;
			}
			else{
				strRet = "F," + strRet;
			}
		}
		else if (dataT == "|"){
			fdata1 = Str_ReadSubItem(&strRet,",");
			fdata2 = Str_ReadSubItem(&strRet,",");
			if ((fdata1 == "T") || (fdata2 == "T")){
				strRet = "T," + strRet;
			}
			else{
				strRet = "F," + strRet;
			}
		}
		else if (dataT == "!"){
			fdata1 = Str_ReadSubItem(&strRet,",");
			if (fdata1 == "T"){
				strRet = "F," + strRet;
			}
			else{
				strRet = "T," + strRet;
			}
		}
		else{
			caclRet = 'F';
			dataT = Str_HEXToASCII(dataT);
			if (BIF_CONDITION::GetCondition().Dispose(env,dataT,&caclRet) == BIF_ID_NO)
				sgReceive.Command(env,dataT,&caclRet);
			strRet = caclRet + "," + strRet;
		}
	}
	fdata1 = Str_ReadSubItem(&strRet,",");
	return(fdata1 == "T");
}
//------------------------------------------------------------------------------------------//
static void BIFEXP::FormatString(STDSTR* returnStr,const STDSTR& strIn){
	//control char :&& || & | [] !
	//formate to HEX without space
	//escape && || & | [] !
	
	STDSTR		strData,strResult;
	STRSIZE		i,length;
	uint8		charData,charData1;
	
	length = strIn.length();
	i = 0;
	*returnStr = "[,";
	strData = "";
	while(i < length){
		charData = strIn[i];
		++ i;
		if ((charData == '[') || (charData == ']')){
			Str_TrimSelf(strData);
			if (strData.length() > 0){
				*returnStr += Str_ASCIIToHEX(strData,G_ESCAPE_OFF);
				*returnStr += ',';
			}
			*returnStr += charData;
			*returnStr += ',';
			strData = "";
		}
		else if ((charData == '&') || (charData == '|') || (charData == '!')){
			if (i < length){
				charData1 = strIn[i];
				if (charData1 != charData){
					Str_TrimSelf(strData);
					if (strData.length() > 0){
						*returnStr += Str_ASCIIToHEX(strData,G_ESCAPE_OFF);
						*returnStr += ',';
					}
					*returnStr += charData;
					*returnStr += ',';
					strData = "";
				}
			}
		}
		else{
			if ((charData == '\\') && (i < length)){
				charData = strIn[i];
				++ i;
				if ((charData != '&') && (charData != '|') && (charData != '[') && (charData != ']') && (charData != '!'))
					strData += '\\';
			}
			strData += charData;
		}
	}
	Str_TrimSelf(strData);
	if (strData.length() > 0){
		*returnStr += Str_ASCIIToHEX(strData,G_ESCAPE_OFF);
		*returnStr += ',';
	}
	*returnStr += ']';
}
//------------------------------------------------------------------------------------------//
static void BIFEXP::GetPoland(STDSTR* polandQueue,const STDSTR& expressions){
	STDSTR		stackT,strExp;
	STDSTR		dataT,dataT2;
	
	*polandQueue = "";
	stackT = "";
	
	FormatString(&strExp,expressions);
	
	while(strExp.length() > 0){
		dataT = Str_ReadSubItem(&strExp,",");
		if (dataT == "!"){
			while(stackT.length() > 0){
				dataT2 = Str_ReadSubItem(&stackT, ",");
				if ((dataT2 == "[") || (dataT2 == "&") || (dataT2 == "|")){
					stackT = dataT + "," + dataT2 + "," + stackT;
					break;
				}
				*polandQueue += dataT2;
				*polandQueue += ',';
			}
		}
		else if ((dataT == "&") || (dataT == "|")){
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
//------------------------------------------------------------------------------------------//
#endif /* BIF_Search_h */
