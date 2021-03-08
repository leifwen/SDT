//
//  SDT.cpp
//  SDT
//
//  Created by Leif Wen on 17/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#include "Global.h"
#include <fcntl.h>
#include <termios.h>
#include <signal.h>
//------------------------------------------------------------------------------------------//
class SDT{
	enum	{SDT_RUN,SDT_HELP,};
	public:
				 SDT(void);
		virtual ~SDT(void);
	public:
#ifdef AppLayer_h
		KERNEL			*kernel;
#endif
		uint32			cgSizeCache;
		uint32			cgSizeBuffer;
		STDSTR			cgCMD;
		SYS_Thread<SDT>	cgThread;
		int 			argc;
		char 			**argv;
		void	UIInit0		(void);
		void	UIInit1		(void);
		void	UIInit_Help	(void);
		uint32	Decode		(int argc,char *argv[]);
		bool32	ThreadFunc	(void* p);
	public:
		void	Run			(int argc,char *argv[]);
		void	RunInThread	(int argc,char *argv[]);
		bool32	IsExit		(void){return(cgThread.IsTerminated());};
		void	Exit		(void);
		bool32	BICDispose	(const STDSTR& cmd,bool32* exeResult);
};
//------------------------------------------------------------------------------------------//
SDT::SDT(void){
#ifdef AppLayer_h
	kernel = nullptr;
#endif
}
SDT::~SDT(void){
#ifdef AppLayer_h
	if (kernel != nullptr)
		delete kernel;
#endif
}
//------------------------------------------------------------------------------------------//
bool32 SDT::ThreadFunc(void* p){
	Run(argc,argv);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
void SDT::RunInThread(int _argc,char *_argv[]){
	cgThread.ThreadInit(this, &SDT::ThreadFunc,"SDT");
	argc = _argc;
	argv = _argv;
	cgThread.ThreadRun();
}
//------------------------------------------------------------------------------------------//
void SDT::Run(int argc,char *argv[]){
	STDSTR filename = CFS_FormatFileName(CFS_GetWorkDIR() + "/" + DEFAULT_INI_FILE);
	cgSizeCache = 1024 * 1024;
#ifdef CommonDefH_MAC
	cgSizeCache = 1024 * 1024 * 32;
#endif
	cgSizeBuffer = 1024 * 128;
	cgCMD = "";

	switch (Decode(argc,argv)){
		case SDT_RUN:;
#ifdef AppLayer_h
			if (kernel == nullptr)
				kernel = new KERNEL(cgSizeCache,cgSizeBuffer);
			if (kernel != nullptr){
				UIInit0();
			
				kernel->Init(filename);
				kernel->Run(cgCMD);
				kernel->Exit(filename);

				UIInit1();
			}
#endif
			break;
		case SDT_HELP:;
			UIInit_Help();
		default:;
			break;
	}
}
//------------------------------------------------------------------------------------------//
void SDT::UIInit0(void){//print welcome information
	std::cout << std::endl;
	std::cout << COL_STR_clBlue << DEV_LINE_STAR << COL_STR_clDefault << std::endl;
	std::cout << " " << SWVERSION_NAME << std::endl;
	std::cout << std::endl;
	std::cout << " Wish you have a nice day." << std::endl;
	std::cout << std::endl;
	std::cout << " Best regards." << std::endl;
	std::cout << " " << SWVERSION_SHORTNAME << "," SWVERSION_VER << std::endl;
	std::cout << " " << SWVERSION_WR << "." << std::endl << std::endl;
	std::cout << COL_STR_clBlue << DEV_LINE_STAR << COL_STR_clDefault << std::endl;
}
//------------------------------------------------------------------------------------------//
void SDT::UIInit1(void){//print welcome information
	/*	SYS_SleepMS(100);*/
	std::cout << std::endl << COL_STR_clDefault << "Quit safely." << std::endl;
	/*	SYS_SleepMS(100);*/
}
//------------------------------------------------------------------------------------------//
void SDT::UIInit_Help(void){//print welcome information
	std::cout << std::endl;
	std::cout << COL_STR_clBlue << DEV_LINE_STAR << COL_STR_clDefault << std::endl;
	std::cout << " -help/-h      " << std::endl;
	std::cout << " -bic          " << std::endl;
	std::cout << " -cachesize    " << std::endl;
	std::cout << " -buffersize   " << std::endl;
	std::cout << std::endl;
	std::cout << COL_STR_clBlue << DEV_LINE_STAR << COL_STR_clDefault << std::endl;
}
//------------------------------------------------------------------------------------------//
void SDT::Exit(void){
#ifdef AppLayer_h
	kernel->ToConsole("\x1b\nexit");
#endif
}
//------------------------------------------------------------------------------------------//
uint32 SDT::Decode(int argc,char *argv[]){
	enum{D_CMD,D_CACHESIZE,D_BUFFERSIZE,D_NONE};
	
	STDSTR	subCMD;
	uint32	bltype = D_NONE;
	int i = 0;
	
	while(++i < argc){
		subCMD = argv[i];
		if ((subCMD == "-h") || (subCMD == "-help"))
			return SDT_HELP;
		if (subCMD == "-bic"){
			bltype = D_CMD;
			continue;
		}
		if (subCMD == "-cachesize"){
			bltype = D_CACHESIZE;
			continue;
		}
		if (subCMD == "-buffersize"){
			bltype = D_BUFFERSIZE;
			continue;
		}
		switch (bltype) {
			case D_CMD:;
				cgCMD += subCMD;
				cgCMD += "\n";
				break;
			case D_CACHESIZE:;
				cgSizeCache = atoi(argv[i]);
				break;
			case D_BUFFERSIZE:;
				cgSizeBuffer = atoi(argv[i]);
				break;
			case D_NONE:;
			default:;
    		break;
		}
	};
	
	cgCMD = Str_Replace(cgCMD, "_", " ");

	return SDT_RUN;
}
//------------------------------------------------------------------------------------------//
bool32 SDT::BICDispose(const STDSTR& cmd,bool32* exeResult){
	if (kernel != nullptr)
		return(kernel->BICDispose(cmd,exeResult));
	return G_FALSE;
};
//------------------------------------------------------------------------------------------//
extern "C"{
//------------------------------------------------------------------------------------------//
void SDT_Exit(void);
//------------------------------------------------------------------------------------------//
static void father_sig_hander(int p){
	if (p == SIGINT){
		std::cout << std::endl << "End by Ctrl+C" << std::endl;
		SDT_Exit();
	}
	if (p == SIGQUIT){
		std::cout << std::endl << "End by Ctrl+\\" << std::endl;
		SDT_Exit();
	}
	else if(p == SIGTSTP){
		std::cout << "end by Ctrl+Z" << std::endl;
	}
	/*SYS_DelayMS(10, nullptr);*/
}
//------------------------------------------------------------------------------------------//
static void SetSignal(void){
	signal(SIGINT,father_sig_hander);
	signal(SIGQUIT,father_sig_hander);
	/*signal(SIGTSTP,father_sig_hander);*/
}
//------------------------------------------------------------------------------------------//
static SDT& GetSDT(void){
	static	SDT		sdt;
	return(sdt);
};
//------------------------------------------------------------------------------------------//
void SDT_Run(int argc,char *argv[]){
	SetSignal();
	GetSDT().Run(argc, argv);
}
//------------------------------------------------------------------------------------------//
void SDT_RunInThread(int argc,char *argv[]){
	SetSignal();
	GetSDT().RunInThread(argc, argv);
	SYS_SleepMS(50);
}
//------------------------------------------------------------------------------------------//
int SDT_IsExit(void){
	return(GetSDT().IsExit() != 0 ? G_TRUE : G_FALSE);
}
//------------------------------------------------------------------------------------------//
void SDT_Exit(void){
	GetSDT().Exit();
}
//------------------------------------------------------------------------------------------//
}
//------------------------------------------------------------------------------------------//
bool32 ExecuteBIC(const STDSTR& cmd,bool32* exeResult){
	return(GetSDT().BICDispose(cmd, exeResult));
}
//------------------------------------------------------------------------------------------//
