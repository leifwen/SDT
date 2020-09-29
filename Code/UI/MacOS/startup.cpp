//
//  SDT.cpp
//  SDT
//
//  Created by Leif Wen on 12/09/2020.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#include "Config.h"
#include <string>
#include "ImplicitFishhook.h"
//------------------------------------------------------------------------------------------//
extern "C" void TestFun(void);
extern "C" void SDT_Run(int argc,char *argv[]);
extern "C" void SDT_RunInThread(int argc,char *argv[]);
extern "C" void Swift_MainLoop(void);
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_MAC
extern "C" int _main(int argc,char *argv[]){
	Disable_NSLOG();
	TestFun();
	SDT_RunInThread(argc,argv);
	Swift_MainLoop();
	return 0;
}
#endif
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Linux
int main(int argc,char *argv[]){
	TestFun();
	SDT_Run(argc,argv);
	exit(EXIT_SUCCESS);
	return 0;
}
#endif
//------------------------------------------------------------------------------------------//
