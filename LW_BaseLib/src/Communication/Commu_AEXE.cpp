//
//  Commu_AEXE.cpp
//  SDT
//
//  Created by Leif Wen on 13/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "Commu_AEXE.h"
#ifdef Commu_AEXE_h
//------------------------------------------------------------------------------------------//
#include <fcntl.h>
#include <signal.h>
#include <termios.h>
#ifdef CommonDefH_Linux
#include <sys/types.h>
#include <sys/wait.h>
#endif
//#define LOGPRINT_ENABLE
//#define LOGTHREAD_ENABLE
#include "SYS_Log.h"
//------------------------------------------------------------------------------------------//
AEXE::AEXE(uint32 size,const ODEV_SYSTEM* logSys) : COMMU_THREAD(size,logSys){
	monitorThread.ThreadInit(this, &AEXE::MonitorThreadFun,"monitor");
	cgThreadList < monitorThread;
	cgCommand = "";
	cgSH = "";
	childpid = -1;

	SetGetDataByRead();
	TNFP::SetSelfName("AEXE");
	SetSelfName(selfName);
	SetFatherName("");
};
//------------------------------------------------------------------------------------------//
AEXE::~AEXE(void){
	Close();
	monitorThread.RemoveSelf();
};
//------------------------------------------------------------------------------------------//
void AEXE::SetSelfName(const STDSTR& strName){
	COMMU_THREAD::SetSelfName(strName);
	monitorThread.SetFatherName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
void AEXE::SetFatherName(const STDSTR& strName){
	COMMU_THREAD::SetFatherName(strName);
	monitorThread.SetFatherName(GetFullName(this));
};
//------------------------------------------------------------------------------------------//
bool32 AEXE::ExecuteCommand(const STDSTR& cmd){
	char	**argv;
	STDSTR	strCMD,strArg;
	int32	num,ret;
	
	num = 0;
	strCMD = cmd;
	do{
		Str_TrimSelf(strCMD);
		strArg = Str_ReadSubItem(&strCMD, " ");
		if (strArg.length() > 0)
			++ num;
	}while(strCMD.length() > 0);
	
	if (num > 0){
		argv = new char*[num + 1];
		argv[num] = (char*)nullptr;
		
		num = 0;
		strCMD = cmd;
		do{
			Str_TrimSelf(strCMD);
			strArg = Str_ReadSubItem(&strCMD, " ");
			if (strArg.length() > 0){
				argv[num] = new char[strArg.length() + 1];
				memcpy(argv[num],strArg.c_str(),strArg.length());
				argv[num][strArg.length()] = 0;
				++ num;
			}
		}while(strCMD.length() > 0);
		ret = execvp(argv[0],argv);
		delete []argv;
		return(ret);
	}
	return G_FALSE;
}
//------------------------------------------------------------------------------------------//
bool32 AEXE::Execute(const STDSTR &name,const STDSTR& cmd){
	int32 ret;
	ret = 0;
	if (InDoing_try() != G_FALSE){
		cgCommand = cmd;
		cgSH = "";
		ret = COMMU_THREAD::Open(SetOpenPar(OPEN_None,name,0,0),G_LOCK_OFF);
		InDoing_clr();
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
bool32 AEXE::SH(const STDSTR &name,const STDSTR& cmd){
	int32 ret;
	ret = 0;
	if (InDoing_try() != G_FALSE){
		cgCommand = cmd;
		cgSH = "/bin/sh";
		ret = COMMU_THREAD::Open(SetOpenPar(OPEN_None,name,0,0),G_LOCK_OFF);
		InDoing_clr();
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
bool32 AEXE::BASH(const STDSTR &name,const STDSTR& cmd){
	int32 ret;
	ret = 0;
	if (InDoing_try() != G_FALSE){
		cgCommand = cmd;
		cgSH = "/bin/bash";
		ret = COMMU_THREAD::Open(SetOpenPar(OPEN_None,name,0,0),G_LOCK_OFF);
		InDoing_clr();
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
const STDSTR& AEXE::GetCommand(void)const{
	return(cgCommand);
};
//------------------------------------------------------------------------------------------//
bool32 AEXE::OpenDev(const OPEN_PAR& par){
	int32			oldf;
	struct termios	oldt, newt;
	int				status;
	
	status = pipe(fd_pipeChildOut);	//child SDOUT
	status = pipe(fd_pipeChildIn);	//child SDIN
	
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
			ExecuteCommand(cgCommand);
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
	COMMU_THREAD::CloseDev();
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
bool32 AEXE::ReadFromDevice(uint32* retNum,uint8* buffer,uint32 length){
	int64	retCode;
	*retNum = 0;
	
	retCode = read(fd_pipeChildOut[0],buffer,length);
	if (((retCode < 0) && (errno != EINTR) && (errno != EWOULDBLOCK) && (errno != EAGAIN)))
		return -1;
	if (retCode > 0){
		*retNum = (uint32)retCode;
		return G_TRUE;
	}
	return G_FALSE;
}
//------------------------------------------------------------------------------------------//
bool32 AEXE::SendToDevice(uint32* retNum,const uint8* buffer,uint32 length){
	int64		retCode = 0;
	uint32		alreadySend;
	
	alreadySend = 0;
	while((alreadySend < length) && (IsConnected())){
		retCode = write(fd_pipeChildIn[1], &buffer[alreadySend], length - alreadySend);
		if (retCode < 0)
			break;
		alreadySend += retCode;
	}
	*retNum = alreadySend;
	return(!((alreadySend < length) || retCode));
}
//------------------------------------------------------------------------------------------//
bool32 AEXE::MonitorThreadFun(void* commu){
	int	childRet,status;
	SYS_SleepMS(2);
	ETLogThreadStart(monitorThread);
	while(monitorThread.IsTerminated() == G_FALSE){
		childRet = waitpid(childpid,&status,WNOHANG);
		if (childRet == childpid || childRet == -1){
			childpid = -1;
			DoSelfClose();
			break;
		}
		SYS_SleepMS(50);
	}
	ETLogThreadStop(monitorThread);
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
AEXEPOOL::AEXEPOOL(uint32 size,const ODEV_SYSTEM* logSys) : COMMU_FRAME_LOGSYS(size,logSys){
	SetSFlag(CF_blNoInitSize);
	
	TNFP::SetSelfName("AEXEPOOL");
	SetSelfName(selfName);
	SetFatherName("");
};
//------------------------------------------------------------------------------------------//
AEXEPOOL::~AEXEPOOL(void){
	Close();
};
//------------------------------------------------------------------------------------------//
TNFP* AEXEPOOL::CreateNode(void){
	return(SetSubNodeFatherName(new AEXE(cgMaxSize,GetLogSystem())));
};
//------------------------------------------------------------------------------------------//
AEXE* AEXEPOOL::Execute(const STDSTR& name,const STDSTR& cmd){
	AEXE	*newExe;
	
	newExe = static_cast<AEXE*>(GetNewChild());
	if (newExe != nullptr){
		if (newExe->Execute(name, cmd))
			return(newExe);
		CloseChild(newExe);
	}
	return(nullptr);
}
//------------------------------------------------------------------------------------------//
AEXE* AEXEPOOL::SH(const STDSTR& name,const STDSTR& cmd){
	AEXE	*newExe;
	
	newExe = static_cast<AEXE*>(GetNewChild());
	if (newExe != nullptr){
		if (newExe->SH(name, cmd))
			return(newExe);
		CloseChild(newExe);
	}
	return(nullptr);
}
//------------------------------------------------------------------------------------------//
AEXE* AEXEPOOL::BASH(const STDSTR& name,const STDSTR& cmd){
	AEXE	*newExe;
	
	newExe = static_cast<AEXE*>(GetNewChild());
	if (newExe != nullptr){
		if (newExe->BASH(name, cmd))
			return(newExe);
		CloseChild(newExe);
	}
	return(nullptr);
}
//------------------------------------------------------------------------------------------//
AEXE* AEXEPOOL::Find(const STDSTR& name,const STDSTR& cmd){
	AEXE	*exe;
	exe = nullptr;
	TREE_LChildRChain_Find(AEXE,this,exe,((_opNode->GetOpenPar().name == name) && (_opNode->GetCommand() == cmd)));
	return(exe);
}
//------------------------------------------------------------------------------------------//
AEXE* AEXEPOOL::Find(const STDSTR& name){
	AEXE	*exe;
	exe = nullptr;
	TREE_LChildRChain_Find(AEXE,this,exe,(_opNode->GetOpenPar().name == name));
	return(exe);
}
//------------------------------------------------------------------------------------------//
void AEXEPOOL::DoClose(void){
	ELog(this << "AEXEPOOL::DoClose()");
	COMMU_FRAME_LOGSYS::DoClose();
	ChildClrSel(GetSelDB());
	CleanChild(this, this);
	SetblUpdate();
}
//------------------------------------------------------------------------------------------//
void AEXEPOOL::CloseChild(COMMU_FRAME *commu){
	COMMU_FRAME_LOGSYS::CloseChild(commu);
};
//------------------------------------------------------------------------------------------//
bool32 AEXEPOOL::CloseChild(const STDSTR& name,const STDSTR& cmd,uint32 timeoutMS){
	AEXE	*exe;
	
	exe = Find(name,cmd);
	
	if (exe != nullptr)
		return(exe->CloseSelf(timeoutMS));
	
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
bool32 AEXEPOOL::CloseChild(const STDSTR& name,uint32 timeoutMS){
	AEXE	*exe;
	
	exe = Find(name);
	
	if (exe != nullptr)
		return(exe->CloseSelf(timeoutMS));
	
	return G_TRUE;
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* Commu_AEXE_h */
