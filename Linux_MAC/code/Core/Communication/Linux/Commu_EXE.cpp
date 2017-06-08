//
//  Commu_Exe.cpp
//  SDT
//
//  Created by Leif Wen on 23/04/2017.
//  Copyright © 2017 leif wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Commu_EXE.h"
#include "Comm_Convert.h"
//------------------------------------------------------------------------------------------//
#ifdef Commu_EXE_hpp
#include <fcntl.h>
#include <signal.h>
#include <termios.h>
#define LOGPRINT_ENABLE
#define LOGPRINT_ENABLE2
#include "Comm_Log.h"
//------------------------------------------------------------------------------------------//
int32 AEXE::ExecuteConsoleCommand(const STDSTR &strCommand){
	char	**argv;
	STDSTR	cmd,strArg;
	int32	num,ret;
	
	num = 0;
	cmd = strCommand;
	do{
		Str_TrimSelf(cmd);
		strArg = Str_ReadSubItem(&cmd, " ");
		if (strArg.length() > 0)
			++ num;
	}while(cmd.length() > 0);
	
	if (num > 0){
		argv = new char*[num + 1];
		argv[num] = (char*)nullptr;
		
		num = 0;
		cmd = strCommand;
		do{
			Str_TrimSelf(cmd);
			strArg = Str_ReadSubItem(&cmd, " ");
			if (strArg.length() > 0){
				argv[num] = new char[strArg.length() + 1];
				memcpy(argv[num],strArg.c_str(),strArg.length());
				argv[num][strArg.length()] = 0;
				++ num;
			}
		}while(cmd.length() > 0);
		ret = execvp(argv[0],argv);
		delete []argv;
		return(ret);
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
void AEXE::Init(void){
	monitorThread.ThreadInit(this, &AEXE::monitorThreadFun,"monitor");
	cgThreadList < monitorThread;
	cgCommand = "";
	cgSH = "";
	childpid = -1;
}
//------------------------------------------------------------------------------------------//
int32 AEXE::Execute(const STDSTR &tCDBufName,const STDSTR &cmd){
	int32 ret;
	ret = 0;
	if (InDoing_try() != 0){
		cgCommand = cmd;
		cgSH = "";
		ret = COMMU_DBUF::OpenD(tCDBufName,0,CSType_None,0,G_LOCK_OFF);
		InDoing_clr();
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
int32 AEXE::SH(const STDSTR &tCDBufName,const STDSTR &cmd){
	int32 ret;
	ret = 0;
	if (InDoing_try() != 0){
		cgCommand = cmd;
		cgSH = "/bin/sh";
		ret = COMMU_DBUF::OpenD(tCDBufName,0,CSType_None,0,G_LOCK_OFF);
		InDoing_clr();
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
int32 AEXE::BASH(const STDSTR &tCDBufName,const STDSTR &cmd){
	int32 ret;
	ret = 0;
	if (InDoing_try() != 0){
		cgCommand = cmd;
		cgSH = "/bin/bash";
		ret = COMMU_DBUF::OpenD(tCDBufName,0,CSType_None,0,G_LOCK_OFF);
		InDoing_clr();
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
int32 AEXE::OpenDev(const STDSTR &tCDBufName,int32 tCDBufPar,CSType tCSType,int32 blEnEcho){
	int32			oldf;
	struct termios	oldt, newt;
	int				status;
	
	pipe(fd_pipeChildOut);	//child SDOUT
	pipe(fd_pipeChildIn);	//child SDIN
	
	tcgetattr(fd_pipeChildOut[0], &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(fd_pipeChildOut[0], TCSANOW, &newt);
	oldf = fcntl(fd_pipeChildOut[0], F_GETFL, 0);
	fcntl(fd_pipeChildOut[0], F_SETFL, oldf | O_NONBLOCK);
	
	childpid = fork();
	if (childpid == 0){				//enter to child process.
		status = 0;
		dup2(fd_pipeChildIn[0],0);	//redirect standard input to read pipe.
		dup2(fd_pipeChildOut[1],1);	//redirect write pipe to standard output.
		
		//close(fd_pipeChildIn[0]);	//close read pipe.
		close(fd_pipeChildIn[1]);	//close write pipe.
		close(fd_pipeChildOut[0]);	//close read pipe.
		//close(fd_pipeChildOut[1]);	//close write pipe.
		if (cgSH.length() == 0){
			ExecuteConsoleCommand(cgCommand);
		}
		else{
			status = execlp(cgSH.c_str(),cgSH.c_str(),"-c",cgCommand.c_str(),(char*)nullptr);
		}
		exit(status);
	}
	else if (childpid > 0){			//enter to fater process.
		close(fd_pipeChildIn[0]);
		close(fd_pipeChildOut[1]);
		return 1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
void AEXE::CloseDev(void){
	int	status;
	COMMU_DBUF::CloseDev();
	cgCommand = "";
	close(fd_pipeChildIn[0]);	//close read pipe.
	close(fd_pipeChildIn[1]);	//close write pipe.
	close(fd_pipeChildOut[0]);	//close read pipe.
	close(fd_pipeChildOut[1]);	//close write pipe.
	
	if (childpid > 0){
		kill(childpid,SIGINT);
		waitpid(childpid,&status,0);
	}
	childpid = -1;
}
//------------------------------------------------------------------------------------------//
int32 AEXE::ReadFromDevice(uint32 *retNum,uint8 *buffer,uint32 length){
	int64	retCode;
	*retNum = 0;

	retCode = read(fd_pipeChildOut[0],buffer,length);
	if (((retCode == -1) && (errno != EINTR) && (errno != EWOULDBLOCK) && (errno != EAGAIN)))
		return -1;
	if (retCode > 0){
		*retNum = (uint32)retCode;
		return 1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
int32 AEXE::SendToDevice(uint32 *retNum,const uint8 *buffer,uint32 length){
	int64		retCode;
	uint32		i;
	
	i = 0;
	while((i < length) && (IsConnected() != 0)){
		retCode = write(fd_pipeChildIn[1], &buffer[i], length - i);
		if (retCode == -1){
			*retNum = i;
			return -1;
		}
		i += retCode;
	}
	*retNum = (uint32)i;
	return 1;
}
//------------------------------------------------------------------------------------------//
int32 AEXE::monitorThreadFun(void *p){
	int	childRet,status;
	SYS_SleepMS(2);
	while(monitorThread.IsTerminated() == 0){
		childRet = waitpid(childpid,&status,WNOHANG);
		if (childRet == childpid || childRet == -1){
			childpid = -1;
			SelfClose();
			break;
		}
		SYS_SleepMS(50);
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void AEXEPOOL::Init(uint32 tSize,const ODEV_SYSTEM *logSys){
	COMMU_DBUF_FRAME_FW::Init(0,logSys);
	cgBufMaxSize = tSize;
}
//------------------------------------------------------------------------------------------//
AEXE* AEXEPOOL::Execute(const STDSTR &tCDBufName,const STDSTR &cmd){
	AEXE	*newExe;
	
	newExe = static_cast<AEXE*>(GetNewSon());
	if (newExe != nullptr){
		if (newExe->Execute(tCDBufName, cmd) > 0)
			return(newExe);
		ChildClose(newExe);
	}
	return(nullptr);
}
//------------------------------------------------------------------------------------------//
AEXE* AEXEPOOL::SH(const STDSTR &tCDBufName,const STDSTR &cmd){
	AEXE	*newExe;
	
	newExe = static_cast<AEXE*>(GetNewSon());
	if (newExe != nullptr){
		if (newExe->SH(tCDBufName, cmd) > 0)
			return(newExe);
		ChildClose(newExe);
	}
	return(nullptr);
}
//------------------------------------------------------------------------------------------//
AEXE* AEXEPOOL::BASH(const STDSTR &tCDBufName,const STDSTR &cmd){
	AEXE	*newExe;
	
	newExe = static_cast<AEXE*>(GetNewSon());
	if (newExe != nullptr){
		if (newExe->BASH(tCDBufName, cmd) > 0)
			return(newExe);
		ChildClose(newExe);
	}
	return(nullptr);
}
//------------------------------------------------------------------------------------------//
AEXE* AEXEPOOL::Find(const STDSTR &tCDBufName,const STDSTR &cmd){
	AEXE	*retExe;
	retExe = nullptr;
	TREE_LChildRChain_Find(AEXE,this,retExe,((operateNode_t->GetBufName() == tCDBufName) && (operateNode_t->GetCommand() == cmd)));
	return(retExe);
}
//------------------------------------------------------------------------------------------//
void AEXEPOOL::DoClose(void){
	ELog(this << "AEXEPOOL::DoClose()");
	COMMU_DBUF_FRAME_FW::DoClose();
	DestroyAll();
	SetblUpdate();
}
//------------------------------------------------------------------------------------------//
int32 AEXEPOOL::ChildClose(const STDSTR &tCDBufName,const STDSTR &cmd,int32 timeout){
	AEXE		*retExe;
	SYS_TIME_S	timeS;
	
	retExe = Find(tCDBufName,cmd);
	
	if (retExe != nullptr){
		SYS_Delay_SetTS(&timeS,timeout);
		retExe->SelfClose();
		while(SYS_Delay_CheckTS(&timeS) == 0){
			SYS_SleepMS(2);
			if (retExe->IsConnected() == 0)
				return 1;
		};
		return 0;
	}
	return 1;
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif

