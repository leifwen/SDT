/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: SDT.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.6.27
 */
/***************************************************************************************/
#include "Global.h"
#include "AppLayer.h"
#include "SDT.h"
#ifdef  CommonDefH_Unix
#include <fcntl.h>
#include <termios.h>
#endif
/***************************************************************************************/
void MainConsole(void);
void SetSignal(void);
void father_sig_hander(int p);
/***************************************************************************************/
int main(void){	
	SDTInit();
	SetSignal();
	MainConsole();
	SDTExit();
	exit(EXIT_SUCCESS);
}
/***************************************************************************************/
void SDTInit(void){
	UIInit();
	GSDTApp.Init();
}
/***************************************************************************************/
void SDTExit(void){
	GSDTApp.Exit();
	SYS_SleepMS(100);
	std::cout << std::endl << COL_STR_clDefault << "Quit safely." << std::endl;
	SYS_SleepMS(100);
}
/***************************************************************************************/
void UIInit(void){//print welcome information
	std::cout << COL_STR_clBlue << DEV_LINE_START << COL_STR_clDefault << std::endl;
	std::cout << " " << SWVERSION_NAME << std::endl;
	std::cout << std::endl;
	std::cout << " Wish you have a nice day." << std::endl;
	std::cout << std::endl;
	std::cout << " Best regards." << std::endl;
	std::cout << " " << SWVERSION_SHORTNAME << "," SWVERSION_VER << std::endl;
	std::cout << " " << SWVERSION_WR << "." << std::endl << std::endl;
	std::cout << COL_STR_clBlue << DEV_LINE_START << COL_STR_clDefault << std::endl;
}
/***************************************************************************************/
void MainConsole(void){
	int32			oldf;
	struct termios	oldt, newt;
	
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
	
	#ifdef ConsoleH
	GSDTApp.m_Console.LoadDefault("console.ini");
	GSDTApp.m_Console.RunWithBIC();
	while ((GSDTApp.m_blExit != -1) && (GSDTApp.m_Console.ReceiveKey(nullptr) != -1))
		SYS_SleepMS(10);
	#endif
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
}
/***************************************************************************************/
void SetSignal(void){
#ifdef CommonDefH_Linux
	sigset_t son_set;
	struct sigaction father_pact;
	
	father_pact.sa_handler = father_sig_hander;
	sigemptyset(&son_set);
	sigaddset(&son_set,SIGTSTP);
	sigaddset(&son_set,SIGINT);
	
	sigprocmask(SIG_BLOCK,&son_set,nullptr);
	sigaction(SIGTSTP,&father_pact,nullptr);
#endif
	signal(SIGINT,father_sig_hander);
}
/***************************************************************************************/
void father_sig_hander(int p){
	if(p == SIGINT){
		std::cout << std::endl << "End by Ctrl+C" << std::endl;
		GSDTApp.m_blExit = -1;
		SYS_DelayMS(10, nullptr);
	}
	else if(p == SIGTSTP){
		//std::cout << "end by Ctrl+Z" << std::endl;
		//GSDTApp.m_blExit = -1;
	}
}
/***************************************************************************************/
