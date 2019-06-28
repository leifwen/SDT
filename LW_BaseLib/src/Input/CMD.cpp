//
//  CMD.cpp
//  SDT
//
//  Created by Leif Wen on 06/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "CMD.h"
//------------------------------------------------------------------------------------------//
#ifdef CMD_h
#include "SYS_Time.h"
//------------------------------------------------------------------------------------------//
void** CreateBuffer(uint32 num){
	void** buffer;
	buffer = new void*[num];
	do{
		buffer[--num] = nullptr;
	}while(num > 0);
	return(buffer);
};
//------------------------------------------------------------------------------------------//
CMD_ENV::CMD_ENV(void) : BASE_FLAG(){
	buffer = nullptr;
	bufferSize = 0;
};
//------------------------------------------------------------------------------------------//
CMD_ENV::~CMD_ENV(void){
	UnInitQuantity();
};
//------------------------------------------------------------------------------------------//
void CMD_ENV::UnInitQuantity(void){
	try{
		if (buffer != nullptr)
			delete []buffer;
		bufferSize = 0;
		buffer = nullptr;
	}
	catch(...){};
};
//------------------------------------------------------------------------------------------//
void CMD_ENV::InitQuantity(uint32 num){
	if (buffer == nullptr){
		bufferSize = num;
		buffer = CreateBuffer(num);
	}
	else if (bufferSize < num){
		uint32 i = bufferSize;
		void** newBuffer = CreateBuffer(num);
		
		while(i-- > 0){
			newBuffer[i] = buffer[i];
		};
		UnInitQuantity();
		bufferSize = num;
		buffer = newBuffer;
	}
};
//------------------------------------------------------------------------------------------//
void CMD_ENV::CopyInit(const CMD_ENV* env){
	uint32 num;
	UnInitQuantity();
	InitQuantity(env->bufferSize);
	num = env->bufferSize;
	while(num-- > 0){
		buffer[num] = env->buffer[num];
	};
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
STDSTR CMD_BASE::DelComment(const STDSTR& strIn){
	//  escape \/ ,//
	//  abcd \\//
	STRSIZE		i,length;
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
};
//------------------------------------------------------------------------------------------//
uint32 CMD_BASE::GetMS(const STDSTR& strIn){
	STDSTR	strUint;
	double	ms;
	
	ms = atof(strIn.c_str()) * 1000;
	if (strIn.length() > 3){
		strUint = strIn.substr(strIn.length() - 2);
		Str_LowerCaseSelf(strUint);
		if (strUint == "ms")
			ms /= 1000;
	}
	return((uint32)ms);
};
//------------------------------------------------------------------------------------------//
STDSTR CMD_BASE::GetMSSTR(uint32 ms){
	STDSTR	ret;
	
	if (ms >= 1000){
		ret = Str_FloatToStr((double)ms / 1000.0) + "s";
	}
	else if (ms > 0){
		ret = Str_ToStr(ms) + "ms";
	}
	else{
		ret = "0s";
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//
void CMD_BASE::DelayMS(CMD_ENV* env,uint32 timeMS){
	SYS_TIME_S	timeS;
	SYS_Delay_SetTS(&timeS,timeMS);
	while(!(IsExit(env) || SYS_Delay_IsTimeout(&timeS)));
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
CMD_NODE::CMD_NODE(void) : CMD_BASE() {
	ClrSFlag(CMD_blTrySubCMD);
	cgCommandID = CMD_ID_NO;
	cgCommand = "";
};
//------------------------------------------------------------------------------------------//
CMDID CMD_NODE::ResolveIDFun(STDSTR** retMsg,const STDSTR& rawIn)const{
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
							**retMsg = Str_Trim(tmpCMD.substr(0,tmpCMD.length() - subEnd.length()));
							return(cgCommandID);
						}
					}
				}
				else{
					**retMsg = Str_Trim(tmpCMD);
					return(cgCommandID);
				}
			}
		}
		else if (Str_LowerCase(tmpCMD) == subCommand){
			**retMsg = "";
			return(cgCommandID);
		}
	}while (oCommand.length() > 0);
	*retMsg = (STDSTR*)&rawIn;
	return(CMD_ID_NO);
};
//------------------------------------------------------------------------------------------//
CMDID CMD_NODE::ResolveIDCMD(STDSTR** retMsg,const STDSTR& rawIn)const{
	STDSTR	subCommand,oCommand;
	STDSTR	cmdIn,parIn;
	
	oCommand = cgCommand;
	SplitPar1(cmdIn, parIn, rawIn);
	
	do{
		subCommand = Str_ReadSubItem(&oCommand,"/");
		if (cmdIn == subCommand){
			**retMsg = parIn;
			return(cgCommandID);
		}
		subCommand += '?';
		if (cmdIn == subCommand){
			**retMsg = '?';
			return(cgCommandID);
		}
		subCommand += '?';
		if (cmdIn == subCommand){
			**retMsg = "??";
			return(cgCommandID);
		}
	}while (oCommand.length() > 0);
	*retMsg = (STDSTR*)&rawIn;
	return(CMD_ID_NO);
};
//------------------------------------------------------------------------------------------//
CMDID CMD_NODE::ResolveID(STDSTR** retMsg,const STDSTR& rawIn)const{
	*retMsg = (STDSTR*)&rawIn;
	return(CMD_ID_NO);
};
//------------------------------------------------------------------------------------------//
CMDID CMD_NODE::Dispose(CMD_ENV* env,const STDSTR& rawIn,void* p)const{
	STDSTR	msg,*msgP;
	CMDID	mID;
	msgP = &msg;
	mID = ResolveID(&msgP,rawIn);
	return(MessageProcessing(env,mID,*msgP,p));
};
//------------------------------------------------------------------------------------------//
CMDID CMD_NODE::MessageProcessing(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* p)const{
	return(Execute(env,mID,msg,p));
};
//------------------------------------------------------------------------------------------//
CMDID CMD_NODE::Execute(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* p)const{
	//if mID == CMD_ID_NO, means msg is rawIn
	if (mID == (uint32)cgCommandID)
		return(Command(env,msg,p));
	if ((CheckSFlag(CMD_blTrySubCMD)) && (msg.length() > 0))
		return(TraversalChildExecute(env,mID,msg,p));
	return(CMD_ID_NO);
};
//------------------------------------------------------------------------------------------//
CMDID CMD_NODE::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID CMD_NODE::TraversalChildExecute(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* p)const{
	//if mID == CMD_ID_NO, means msg is rawIn
	CMDID	retCode;
	retCode = CMD_ID_NO;
	if (mID == CMD_ID_NO){
		TREE_DownChain_Traversal_LINE_nolock(CMD_NODE,this,
			retCode = _opNode->Dispose(env,msg,p);
			if (retCode != CMD_ID_NO)
				break;
		);
	}
	else{
		TREE_DownChain_Traversal_LINE_nolock(CMD_NODE,this,
			retCode = _opNode->MessageProcessing(env,mID,msg,p);
			if (retCode != CMD_ID_NO)
				break;
		);
	}
	return(retCode);
};
//------------------------------------------------------------------------------------------//
CMDID CMD_NODE::TraversalChildHelp(CMD_ENV* env,uint32 flag)const{
	TREE_DownChain_Traversal_LINE_nolock(CMD_NODE,this,
		if (_opNode->CheckSFlag(CMD_blHidenHelp) == G_FALSE)
			_opNode->Help(env,flag)
	);
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID CMD_NODE::Help(CMD_ENV* env,uint32 flag)const	{
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
void CMD_NODE::SetHidenHelp(void){
	SetSFlag(CMD_blHidenHelp);
};
//------------------------------------------------------------------------------------------//
void CMD_NODE::ClrHidenHelp(void){
	ClrSFlag(CMD_blHidenHelp);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* CMD_h */
