//
//  SDT.cpp
//  SDT
//
//  Created by Leif Wen on 17/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//
/***************************************************************************************/
#include "Global.h"
#include <fcntl.h>
#include <termios.h>
#include <signal.h>
/***************************************************************************************/
class SDT{
	enum	{SDT_RUN,SDT_HELP,};
	public:
				 SDT(void);
		virtual ~SDT(void);
	private:
		KERNEL	*kernel;
		uint32	cgSizeCache;
		uint32	cgSizeBuffer;
		STDSTR	cgCMD;
		void	UIInit0(void);
		void	UIInit1(void);
		void	UIInit_Help(void);
		uint32	Decode(int argc,char *argv[]);
	public:
		void	Run(int argc,char *argv[]);
		void	Exit(void);
	public:
	public:
};
/***************************************************************************************/
SDT::SDT(void){
	kernel = nullptr;
}
SDT::~SDT(void){
	if (kernel != nullptr)
		delete kernel;
}
/***************************************************************************************/
void SDT::Run(int argc,char *argv[]){
	cgSizeCache = 1024 * 1024;
#ifdef CommonDefH_MAC
	cgSizeCache = 1024 * 1024 * 32;
#endif
	cgSizeBuffer = 1024 * 128;
	cgCMD = "";

	switch (Decode(argc,argv)){
		case SDT_RUN:;
			if (kernel == nullptr)
				kernel = new KERNEL(cgSizeCache,cgSizeBuffer);
			if (kernel != nullptr){
				UIInit0();
			
				kernel->Init(DEFAULT_INI_FILE);
				kernel->Run(cgCMD);
				kernel->Exit(DEFAULT_INI_FILE);
			
				UIInit1();
			}
			break;
		case SDT_HELP:;
			UIInit_Help();
		default:;
			break;
	}
}
/***************************************************************************************/
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
/***************************************************************************************/
void SDT::UIInit1(void){//print welcome information
	SYS_SleepMS(100);
	std::cout << std::endl << COL_STR_clDefault << "Quit safely." << std::endl;
	SYS_SleepMS(100);
}
/***************************************************************************************/
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
/***************************************************************************************/
void SDT::Exit(void){
	kernel->ExecBIC("\x1b\nexit");
}
/***************************************************************************************/
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
/***************************************************************************************/














/***************************************************************************************/
SDT		sdt;
void TestFun	(void);
void SetSignal	(void);
/***************************************************************************************/
int main(int argc,char *argv[]){
	TestFun();
	SetSignal();
	sdt.Run(argc,argv);
	exit(EXIT_SUCCESS);
	return 0;
}
/***************************************************************************************/
void father_sig_hander(int p){
	if (p == SIGINT){
		std::cout << std::endl << "End by Ctrl+C" << std::endl;
		sdt.Exit();
	}
	if (p == SIGQUIT){
		std::cout << std::endl << "End by Ctrl+\\" << std::endl;
		sdt.Exit();
	}
	else if(p == SIGTSTP){
		std::cout << "end by Ctrl+Z" << std::endl;
	}
	SYS_DelayMS(10, nullptr);
}
/***************************************************************************************/
void SetSignal(void){
	signal(SIGINT,father_sig_hander);
	signal(SIGQUIT,father_sig_hander);
	//signal(SIGTSTP,father_sig_hander);
}
/***************************************************************************************/
