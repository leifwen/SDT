//
//  MSG_Files.cpp
//  SDT
//
//  Created by Leif Wen on 2019/6/12.
//  Copyright © 2019 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "MSG_ID.h"
#include "MSG_Files.h"
#ifdef MSG_Files_h
//------------------------------------------------------------------------------------------//
#ifndef MSG_DN_FDIR
#define	MSG_DN_FDIR	"_Download"
#endif
//------------------------------------------------------------------------------------------//
//#define LOGPRINT_ENABLE
//#define LOGMSG_ENABLE

#ifdef LOGMSG_ENABLE
#define ELogMSG(x) ELog(x);
#else
#define ELogMSG(x);
#endif

#include "SYS_Log.h"
#include "Commu_SSL.h"
#include "SYS_File.h"
//------------------------------------------------------------------------------------------//
enum{
	MSG_FILES_ERR = 0,
	MSG_FILES_STATUS_OVERWRITE,
	MSG_FILES_STATUS_WRITE,
	MSG_FILES_STATUS_FINISH,
	MSG_FILES_STATUS_DIR,
	MSG_FILES_STATUS_ERR,

	MSG_FILES_OFFSET,
	MSG_FILES_OVERWRITE,
	MSG_FILES_APPEND,

	MSG_FILES_REQ_CHECK,
	MSG_FILES_ANS_CHECK,
};
//------------------------------------------------------------------------------------------//
const	STDSTR	ANS_TEXT_NOFILE = "nofile";
const	STDSTR	ANS_TEXT_DIR = "dir";
const	STDSTR	ANS_TEXT_HASHSIZE = "hashsize";
//------------------------------------------------------------------------------------------//
static STDSTR _GetFileMsgText(uint32 mID){
	STDSTR retStr;
	retStr = "(" + Str_ToStr(mID)+ ")";
	switch(mID){
		case MSG_FILES_ERR				:retStr += "MSG_FILES_ERR ";				break;
		case MSG_FILES_STATUS_OVERWRITE	:retStr += "MSG_FILES_STATUS_OVERWRITE ";	break;
		case MSG_FILES_STATUS_WRITE		:retStr += "MSG_FILES_STATUS_WRITE ";		break;
		case MSG_FILES_STATUS_FINISH	:retStr += "MSG_FILES_STATUS_FINISH ";		break;
		case MSG_FILES_STATUS_DIR		:retStr += "MSG_FILES_STATUS_DIR ";			break;
		case MSG_FILES_STATUS_ERR		:retStr += "MSG_FILES_STATUS_ERR ";			break;
			
		case MSG_FILES_OFFSET			:retStr += "MSG_FILES_OFFSET ";				break;
		case MSG_FILES_OVERWRITE		:retStr += "MSG_FILES_OVERWRITE ";			break;
		case MSG_FILES_APPEND			:retStr += "MSG_FILES_APPEND ";				break;

		case MSG_FILES_REQ_CHECK		:retStr += "MSG_FILES_REQ_CHECK ";			break;
		case MSG_FILES_ANS_CHECK		:retStr += "MSG_FILES_ANS_CHECK ";			break;

		default							:retStr = "";								break;
	}
	return(retStr);
};
//------------------------------------------------------------------------------------------//
#define MSGSendInFileCH2(_env,_mID,_in,_fY,_fN,_time,_blRet,_Title1,_Title2) \
\
	ELogMSG(LogTitle(this,_Title1,_Title2,">>") << _GetFileMsgText(_mID));\
	_blRet = SendInFileCH(_env,_mID,_in,_fY,_fN,_time);\
	ELogMSG(LogTitle(this,_Title1,_Title2,"::") << _GetFileMsgText(_mID) << ((_blRet > 0) ? "successful" : "fail"));
//------------------------------------------------------------------------------------------//
#define MSGSendInFileCH3(_mID,_in,_blRet,_Title1,_Title2) \
\
	ELogMSG(LogTitle(this,_Title1,_Title2,">>") << _GetFileMsgText(_mID) << "with MESG:" << _in);\
	_blRet = cgT1FileCH->Send(ACF(_IsClosed,cgTeam),nullptr,_mID,IUD(_in));\
	ELogMSG(LogTitle(this,_Title1,_Title2,">>") << _GetFileMsgText(_mID) << ((_blRet > 0) ? "successful" : "fail"));
//------------------------------------------------------------------------------------------//
MSG_Files::MSG_Files(void) : MSG_NODE(){
	cgMSGID = MESG_ID_FILE;
	cgENV = nullptr;
	cgTeam = nullptr;
	cgFileName = "";
	cgFileOffset = 0;
	cgT1FileCH = nullptr;
	t1FileRxThread.ThreadInit(this,&MSG_Files::T1FileRxThreadFunc,"T1FileCHRx");

	selfName = "MSG_Files";
	SetUpName("");
};
//------------------------------------------------------------------------------------------//
void MSG_Files::SetSelfName(const STDSTR& strName){
	selfName = strName;
	t1FileRxThread.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void MSG_Files::SetUpName(const STDSTR& strName){
	fatherName = strName;
	t1FileRxThread.SetUpName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void MSG_Files::Reset(void* _team){
	ClrSFlag(REG_blFileCHSetupERR | REG_blFileCHSetupOK | REG_blFileCHCheckERR | REG_blFileCHCheckOK);
};
//------------------------------------------------------------------------------------------//
void MSG_Files::Close(void* _team){
	t1FileRxThread.ThreadStop(_team);
	cgMsgCenter->GetSSLDev()->ReleaseT1(cgT1FileCH);
	cgT1FileCH = nullptr;
};
//------------------------------------------------------------------------------------------//
static bool32 _IsClosed(void* _team){
#ifdef Commu_SSL_h
	return(CommuIsClosed(_team));
#else
	return G_TRUE;
#endif
};
//------------------------------------------------------------------------------------------//
bool32 MSG_Files::SendInFileCH(CMD_ENV* env,uint32 mID,const UVIn& _in,const uint64& flagY,const uint64& flagN,const uint32& waitTimeS){
	SYS_TIME_S		timedly;
	bool32			err = G_FALSE;
	
	do{
		if (cgT1FileCH == nullptr)
			break;
		if (CMD_NODE::IsExit(env)){
			err = -1;
		}
		else{
			SYS_Delay_SetTS(&timedly, waitTimeS << 10);
			ClrSFlag(flagY | flagN);
			
			err = -3;
			if (cgT1FileCH->Send(ACF(_IsClosed,cgTeam),nullptr,mID,_in) == IOS_FINISH)
				err = Check(env,flagY,flagN,timedly);
		}
	}while(0);
	return(err);
};
//------------------------------------------------------------------------------------------//
bool32 MSG_Files::Send_REQ_FileCHSetup(CMD_ENV* env,uint32 maxPackage,uint32 maxPages,uint32 maxDlyMS,uint32 maxTimes){
	bool32	blRet = G_FALSE;
	STDSTR	msg;
	
	msg  = Str_DecToHex(maxPackage);
	msg += ',';
	msg += Str_DecToHex(maxPages);
	msg += ',';
	msg += Str_DecToHex(maxDlyMS);
	msg += ',';
	msg += Str_DecToHex(maxTimes);

	MSGSend2(env,MESG_REQ_FileCHSetup,msg
			 ,REG_blFileCHSetupOK
			 ,REG_blFileCHSetupERR
			 ,MSG_CENTER::HandshakeTime
			 ,blRet
			 ,"MSG_Files"
			 ,"Send_REQ_FileCH");
	return(blRet);
};
//------------------------------------------------------------------------------------------//
struct _FileStatus{
	uint64	offset;
	uint32	package;
	uint32	cfg;
};
//------------------------------------------------------------------------------------------//
bool32 MSG_Files::Send_REQ_FileCheck(CMD_ENV* env,void* fileStatus,const STDSTR& fn_src,const STDSTR& fn_des){
	bool32	blRet = G_FALSE;;
	STDSTR	msg,recMsg,hash_des,hash_src;
	uint64	fileSize_des,fileSize_src;
	ALG_MD5	md5;

	_FileStatus	*fs = (_FileStatus*)fileStatus;

	fs->cfg = MSG_FILES_ERR;
	fs->package = 0;
	fs->offset = 0;
	
	recMsg = "";
	msg = fn_des;
	msg += ',';
	msg += Str_DecToHex((uint64)&recMsg);
	MSGSendInFileCH2(env,MSG_FILES_REQ_CHECK,msg
					 ,REG_blFileCHCheckOK
					 ,REG_blFileCHCheckERR
					 ,MSG_CENTER::HandshakeTime
					 ,blRet
					 ,"MSG_Files"
					 ,"Send_REQ_FileCheck");
	if (blRet > 0){
		fs->package = (uint32)Str_HexToDec(Str_ReadSubItem(&recMsg, ","));
		if (fs->package > 1024 * (CSSL_T1MSG::MESG_SIZE / 1024 - 1))
			fs->package = 1024 * (CSSL_T1MSG::MESG_SIZE / 1024 - 1);

		msg = Str_ReadSubItem(&recMsg, ",");
		if (msg == ANS_TEXT_DIR){
			fs->cfg = MSG_FILES_STATUS_DIR;
			fs->offset = 0;
		}
		else if (msg == ANS_TEXT_NOFILE){
			fs->cfg = MSG_FILES_STATUS_WRITE;
			fs->offset = 0;
		}
		else if (msg == ANS_TEXT_HASHSIZE){
			hash_des = Str_ReadSubItem(&recMsg, ",");
			fileSize_des = Str_HexToDec(recMsg);
			fileSize_src = CFS_CheckFileSize(fn_src);
			if (fileSize_des == fileSize_src){
				md5.InitCFG().Calc(nullptr, OUD_HEX(_EMPTY(&hash_src)), _NONE(), IUD_FILE(fn_src));
				if (hash_des == hash_src){//fn_src == fn_des,no need send
					fs->cfg = MSG_FILES_STATUS_FINISH;
					fs->offset = fileSize_des;
				}
				else{//need overwrite
					fs->cfg = MSG_FILES_STATUS_OVERWRITE;
					fs->offset = 0;
				}
			}
			else if (fileSize_des < fileSize_src){//check whether need continue send
				md5.InitCFG().Calc(nullptr, OUD_HEX(_EMPTY(&hash_src)), _NONE(), IUD_FILE(fn_src,fileSize_des,0));
				if (hash_des == hash_src){//fn_src == fn_des,need continue send
					fs->cfg = MSG_FILES_STATUS_WRITE;
					fs->offset = fileSize_des;
				}
				else{//need overwrite
					fs->cfg = MSG_FILES_STATUS_OVERWRITE;
					fs->offset = 0;
				}
			}
		}
	}
	return(blRet);
};
//------------------------------------------------------------------------------------------//
static inline void GetHashSize(STDSTR* hash,uint64* fileSize,const STDSTR& fn_src){
	ALG_MD5	md5;
	md5.InitCFG().Calc(nullptr, OUD_HEX(_EMPTY(hash)), _NONE(), IUD_FILE(fn_src));
	*fileSize = CFS_CheckFileSize(fn_src);
};
//------------------------------------------------------------------------------------------//
bool32 MSG_Files::ANS_FileCheck(CMD_ENV* env,const STDSTR& recMsg){
	bool32	blRet = G_FALSE;
	STDSTR	fn_des,msg,hash,dir_des;
	uint64	fileSize;

	msg = recMsg;
	msg += ',';
	msg += Str_DecToHex(1024 * (CSSL_T1MSG::MESG_SIZE / 1024 - 1));
	msg += ',';
	
	dir_des = CFS_GetWorkDIR();
	fn_des = MSG_DN_FDIR;
	fn_des = CFS_FormatFileName(fn_des + "/" + Str_ReadSubItem(&msg, ","));
	cgFileName = CFS_FormatFileName(dir_des + "/" + fn_des);
	fn_des = CFS_GetDIR(fn_des);
	ELogMSG(LogTitle(this, "MSG_Files", "ANS_FileCheck", "::") << dir_des);
	ELogMSG(LogTitle(this, "MSG_Files", "ANS_FileCheck", "::") << fn_des);
	ELogMSG(LogTitle(this, "MSG_Files", "ANS_FileCheck", "::") << cgFileName);
	do{
		if (CFS_CreateDIRLoop(dir_des, fn_des) == G_FALSE){
			msg += ANS_TEXT_DIR;
			break;
		}
		
		if (CFS_CheckFile(cgFileName)){
			if (CFS_CheckDIR(cgFileName)){//check is DIR
				msg += ANS_TEXT_DIR;
				break;
			}
		}
		else{
			msg += ANS_TEXT_NOFILE;
			break;
		}
		
		GetHashSize(&hash, &fileSize, cgFileName);
		
		msg += ANS_TEXT_HASHSIZE;
		msg += ',';
		msg += hash;
		msg += ',';
		msg += Str_DecToHex(fileSize);
	}while(0);

	MSGSendInFileCH3(MSG_FILES_ANS_CHECK,msg,blRet,"MSG_Files","ANS_FileCheck");

	return(blRet);
};
//------------------------------------------------------------------------------------------//
bool32 MSG_Files::SendFile(CMD_ENV* env,MSG_ExternalFun exFun,bool32 blOW,const STDSTR& fn_src,const STDSTR& fn_des){
	bool32		blRet;
	IOSE		rcode;
	uint64		fileSize;
	IOS			ios;
	_FileStatus	fs;
	
	ClrSFlag(REG_blFileSaveERR);
	
	do{
		blRet = G_FALSE;
		if (Send_REQ_FileCHSetup(env,4,4,10000,10) == G_FALSE)
			break;
		if (cgT1FileCH == nullptr)
			break;
		if (Send_REQ_FileCheck(env, &fs, fn_src, fn_des) == G_FALSE)
			break;
		blRet = G_TRUE;
		if (fs.cfg == MSG_FILES_STATUS_FINISH)
			break;
		blRet = -1;//DIR exist
		if (fs.cfg == MSG_FILES_STATUS_DIR)
			break;
		blRet = -2;//file exist
		if (fs.cfg == MSG_FILES_STATUS_OVERWRITE){
			if (blOW == G_FALSE)//check whether allow overwrite
				break;
			fs.cfg = MSG_FILES_STATUS_WRITE;
		}
		blRet = G_FALSE;
		if (fs.cfg == MSG_FILES_STATUS_WRITE){
			fileSize = CFS_CheckFileSize(fn_src);
			do{
				if (fs.offset == 0){
					rcode = cgT1FileCH->Send(ACF(_IsClosed,cgTeam),IOS_clr(&ios),MSG_FILES_OVERWRITE,IUD_FILE(fn_src,fs.package,fs.offset));
				}
				else{
					rcode = cgT1FileCH->Send(ACF(_IsClosed,cgTeam),IOS_clr(&ios),MSG_FILES_APPEND,IUD_FILE(fn_src,fs.package,fs.offset));
				}
				fs.offset += ios.total_in;
				if (exFun(env,fileSize,fs.offset) != G_FALSE)
					break;
				if (CheckSFlag(REG_blFileSaveERR))
					break;
				if (rcode == IOS_NO_MEM)
					continue;
				if (rcode != IOS_FINISH)
					break;
			}while(fs.offset < fileSize);
			if (CheckSFlag(REG_blFileSaveERR))
				break;
			if (rcode != IOS_FINISH)
				break;
			if (fs.offset < fileSize)
				break;
		}
		if (Send_REQ_FileCheck(env, &fs, fn_src, fn_des) == G_FALSE)
			break;
		if (fs.cfg != MSG_FILES_STATUS_FINISH)
			break;
		blRet = G_TRUE;
	}while(0);
	return(blRet);
};
//------------------------------------------------------------------------------------------//
void MSG_Files::AppendFile(CMD_ENV* env,const STDSTR& recMsg){
	bool32		blRet = G_FALSE;
	IOSE		rcode;

	rcode = DS_IO_NODE::GetDSIOList().Save(nullptr, OUD_FILEApp(cgFileName), recMsg);

	if (rcode <= 0){
		MSGSendInFileCH3(MSG_FILES_STATUS_ERR,"ERR",blRet,"MSG_Files","ReceiveFile");
	}
};
//------------------------------------------------------------------------------------------//
void MSG_Files::OverWriteFile(CMD_ENV* env,const STDSTR& recMsg){
	bool32		blRet = G_FALSE;
	IOSE		rcode;
	
	rcode = DS_IO_NODE::GetDSIOList().Save(nullptr, OUD_FILEWR(cgFileName), recMsg);
	
	if (rcode <= 0){
		MSGSendInFileCH3(MSG_FILES_STATUS_ERR,"ERR",blRet,"MSG_Files","ReceiveFile");
	}
};
//------------------------------------------------------------------------------------------//
bool32 MSG_Files::T1FileRxThreadFunc(void* _team){
	ETLogThreadStart(t1FileRxThread);
	
	uint32	mID = 0;
	STDSTR	strMesg;
	
	while(t1FileRxThread.IsTerminated() == G_FALSE){
		cgT1FileCH->Read(ACF(_IsClosed,_team), nullptr, &mID, _EMPTY(&strMesg));
		MessageProcessing2(cgENV,mID,strMesg,_team);
		SYS_SleepMS(2);
	};
	ETLogThreadStop(t1FileRxThread);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
CMDID MSG_Files::MessageProcessing2(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* _team){
	bool32	blRet2;
	CMDID	retCode = mID;
	STDSTR	strMesg,*recMsg;

#ifdef LOGMSG_ENABLE
	switch (mID){
		case MSG_FILES_REQ_CHECK:;
		case MSG_FILES_APPEND:;
		case MSG_FILES_OVERWRITE:;
			ELogMSG(LogTitle(this,"MsgProcessing2","Rec","<<") << _GetFileMsgText(mID));
			break;
		case MSG_FILES_OFFSET:;
		case MSG_FILES_ANS_CHECK:;
			ELogMSG(LogTitle(this,"MsgProcessing2","Rec","<<") << _GetFileMsgText(mID) << ": " << msg);
		default:
			break;
	}
#endif
	blRet2 = G_TRUE;
	switch (mID){
		case MSG_FILES_REQ_CHECK:
			ANS_FileCheck(env,msg);
			break;
		case MSG_FILES_ANS_CHECK:
			strMesg = msg;
			recMsg = (STDSTR*)Str_HexToDec(Str_ReadSubItem(&strMesg, ","));
			*recMsg = strMesg;
			SetSFlag(REG_blFileCHCheckOK);
			break;
		case MSG_FILES_OFFSET:
			cgFileOffset = (uint32)Str_HexToDec(msg);
			break;
		case MSG_FILES_OVERWRITE:
			OverWriteFile(env,msg);
			break;
		case MSG_FILES_APPEND:
			AppendFile(env,msg);
			break;
		case MSG_FILES_STATUS_ERR:
			SetSFlag(REG_blFileSaveERR);
			break;
		default:
			retCode = MESG_NONE;
			break;
	};
	return(retCode);
}
//------------------------------------------------------------------------------------------//
CMDID MSG_Files::MessageProcessing(CMD_ENV* env,const uint32& mID,const STDSTR& msg,void* _team){
	bool32	blRet2;
	CMDID	retCode = mID;
	STDSTR	strMesg;
	uint32	port, maxPackage, maxPages, maxDlyMS, maxTimes;
	
#ifdef LOGMSG_ENABLE
	switch (mID){
		case MESG_REQ_FileCHSetup:;
			ELogMPRecMsg(mID);
			break;
		case MESG_ANS_FileCHSetup:;
			ELogMPRecMsg2(mID,msg);
		default:
			break;
	}
#endif
	blRet2 = G_TRUE;
	switch (mID){
		case MESG_REQ_FileCHSetup:
			cgTeam = _team;
			cgENV = env;
			strMesg = msg;
			maxPackage = (uint32)Str_HexToDec(Str_ReadSubItem(&strMesg, ","));
			maxPages = (uint32)Str_HexToDec(Str_ReadSubItem(&strMesg, ","));
			maxDlyMS = (uint32)Str_HexToDec(Str_ReadSubItem(&strMesg, ","));
			maxTimes = (uint32)Str_HexToDec(Str_ReadSubItem(&strMesg, ","));
			if (cgT1FileCH == nullptr)
				cgT1FileCH = cgMsgCenter->GetSSLDev()->AcquireT1MSG(maxPackage,maxPages,maxDlyMS,maxTimes);
			if (cgT1FileCH == nullptr){
				MPSend3(MESG_ANS_FileCHSetup,"ERR",blRet2);
			}
			else{
				t1FileRxThread.ThreadRun(_team);
				strMesg = Str_DecToHex(cgT1FileCH->GetPort());
				strMesg += ',';
				strMesg += msg;
				MPSend3(MESG_ANS_FileCHSetup,strMesg,blRet2);
			}
			break;
		case MESG_ANS_FileCHSetup:
			cgTeam = _team;
			cgENV = env;
			if (msg == "ERR"){
				SetSFlag(REG_blFileCHSetupERR);
			}
			else{
				strMesg = msg;
				port = (uint32)Str_HexToDec(Str_ReadSubItem(&strMesg, ","));
				maxPackage = (uint32)Str_HexToDec(Str_ReadSubItem(&strMesg, ","));
				maxPages = (uint32)Str_HexToDec(Str_ReadSubItem(&strMesg, ","));
				maxDlyMS = (uint32)Str_HexToDec(Str_ReadSubItem(&strMesg, ","));
				maxTimes = (uint32)Str_HexToDec(Str_ReadSubItem(&strMesg, ","));

				if (cgT1FileCH == nullptr)
					cgT1FileCH = cgMsgCenter->GetSSLDev()->AcquireT1MSG(maxPackage,maxPages,maxDlyMS,maxTimes);
				if (cgT1FileCH != nullptr){
					cgT1FileCH->SetPort(port);
					t1FileRxThread.ThreadRun(_team);
				}
				SetSFlag(REG_blFileCHSetupOK);
			}
			break;
		default:
			retCode = MESG_NONE;
			break;
	};
	return(retCode);
};
//------------------------------------------------------------------------------------------//
#endif
