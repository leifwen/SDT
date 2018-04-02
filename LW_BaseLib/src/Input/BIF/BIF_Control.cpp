//
//  BIF_Control.cpp
//  SDT
//
//  Created by Leif Wen on 19/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "BIF_Control.h"
#ifdef BIF_Control_h
#include "BIF_Expression.h"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_DELAY::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = Delay=<PAR> -->Delay PAR time, then goto send next \"Command\". Default unit is second.");
	PrintB(env,"  Command = <'Delay=<PAR>>[//COMMENT]");
	PrintP(env,"   eg:");
	PrintP(env,"     Command = 'Delay=1.2      //delay 1.2s");
	PrintP(env,"     Command = 'Delay=1200ms   //delay 1200ms");
	PrintP(env,"     Command = 'Delay=1.2s     //delay 1.2s");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_DELAY::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	int32 		timeout;
	STDSTR		strPar1,strPar2;
	
	SplitPar1(strPar1, strPar2, msg, " ");
	timeout = GetMS(strPar1);
	
	PrintExecute(env,"Delay",GetMSSTR(timeout));
	
	DelayMS(env,timeout);
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_WAIT::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = Wait=<timeout>,<Expression> -->Wait until receive data contains expression,.");
	PrintB(env,"  Command = <'Wait=<timeout>,<Expression>>[//COMMENT]");
	PrintP(env,"  Notes:1.Expression is the same as Condition Expression.");
	PrintP(env,"   eg:");
	PrintP(env,"     Command = 'Wait=10.5s,'cts==H");
	PrintP(env,"               //if CTS is high then send next \"Command\". Waiting timeout is 10.5s.");
	PrintP(env,"     Command = 'Wait=500ms,'cts==L");
	PrintP(env,"               //if CTS is low then send next \"Command\". Waiting timeout is 500ms.");
	PrintP(env,"     Command = 'Wait=10.5s,'cts==H && 'dsr==H");
	PrintP(env,"               //if CTS is high and DSR is high, then send next \"Command\".");
	PrintP(env,"     Command = 'Wait=10.5s,'cts==H && OK");
	PrintP(env,"               //if CTS is high and receive OK, then send next \"Command\".");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_WAIT::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	int32 		timeout;
	STDSTR		strPar2,strRet;
	SYS_TIME_S	timeS;
	
	if (env != nullptr){
		strPar2 = msg;
		timeout = GetMS(Str_Trim(Str_ReadSubItem(&strPar2,",")));
		Str_TrimSelf(strPar2);
		
		PrintExecute(env,"Wait, until {", strPar2, "} is true.\n Timeout is", GetMSSTR(timeout));
		SYS_Delay_SetTS(&timeS,timeout);
		while(!(ChkblExit(env) || SYS_Delay_CheckTS(&timeS) || BIF_Expression(env,strPar2)));
	}
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_SYNCHRONOUS::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = Synchronous=<PAR1>,<PAR2> -->Time synchronization, PAR1 is offset of zone second,");
	PrintB(env,"                                    PAR2 is synchronization interval, uint is second.");
	PrintB(env,"  Command = <'Synchronous=<PAR1>,<PAR2>>[//COMMENT]");
	PrintP(env,"  Notes:");
	PrintP(env,"     1.If this is the first implementation, T0=TIME(HOUR(Tnow),MINUTE(Tnow),0)+PAR1.");
	PrintP(env,"        eg:Time is 10:12:45, record T0=10:12:00 + PAR1.");
	PrintP(env,"     2.According to PAR2 to calculate the execution time of next \"Command\".");
	PrintP(env,"       Tnext=T0+INT((Tnow-T0)/Timeout(ms)+1)*Timeout(ms).");
	PrintP(env,"     3.If this is not the first time implementation, T0 has no changed, re-calculate Tnext.");
	PrintP(env,"   eg:Time is 10:12:14");
	PrintP(env,"     Command = 'Synchronous=0,10");
	PrintP(env,"               //T0=10:12:00, the execution time of next \"Command\" is 10:12:20, 10:12:30, 10:12:40,...");
	PrintP(env,"     Command = 'Synchronous=5,10");
	PrintP(env,"               //T0=10:12:05, the execution time of next \"Command\" is 10:12:15, 10:12:25, 10:12:35,...");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_SYNCHRONOUS::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
#ifdef GList_h
	double 	T1,T2,synchronousDelay;
	DTIME	currentTime,dT;
	int32	hour, min, sec, msec;
	uint64	msecT2,dTime;
	STDSTR	strPar1,strPar2;

	if ((env != nullptr) && (BIF_ENV::GetCommandNode(env) != nullptr)){
		SplitPar1(strPar1,strPar2,msg,",");
		
		T1 = atof(strPar1.c_str());
		T2 = atof(strPar2.c_str());
		
		currentTime.Now();
		if (BIF_ENV::GetCommandNode(env)->blFirstSynchronous == G_FALSE){
			BIF_ENV::GetCommandNode(env)->blFirstSynchronous = G_TRUE;
			currentTime.DecodeTime1970(&hour,&min,&sec,&msec);
			dT.EncodeTimeABS(0,0,sec,msec);
			BIF_ENV::GetCommandNode(env)->timeST0 = currentTime - dT;
			msec = (int32)(T1 * 1000) % 1000;
			dT.EncodeTimeABS(0,0,(int32)T1,msec);
			BIF_ENV::GetCommandNode(env)->timeST0 += dT;
			BIF_ENV::GetCommandNode(env)->timeST0 -= (60 * 60 * 24 * 2);
		}
		dT = currentTime - BIF_ENV::GetCommandNode(env)->timeST0;
		
		dTime = (uint64)(dT.GetSec() * 1000 * 1000);
		msecT2 = (uint64)(T2 * 1000 * 1000);
		dTime %= msecT2;
		synchronousDelay = (double)(msecT2 - dTime) / 1000;
#ifdef Output_h
		if (CheckEnvFlag(env,BIF_ENV::ENV_blPrintBIFInfo) && (GetSTDOUT(env) != nullptr)){
			*GetSTDOUT(env) << Begin() << NL()
			<< COL_DivLine_Maroon << COL_Time << SYS_MakeTime(currentTime)
			<< COL_NormalMessage << " Synchronous from "
			<< COL_ImportantMessage << BIF_ENV::GetCommandNode(env)->timeST0.Format(DEFAULT_TIMEFORMATE)
			<< COL_NormalMessage << " per "
			<< COL_clBlue << Str_FloatToStr(T2) << "s"
			<< COL_NormalMessage << ". \n Next command will be send after "
			<< COL_clRed << Str_FloatToStr(synchronousDelay / 1000) << "s\n"
			<< Endl();
		}
#endif
		DelayMS(env,(uint32)synchronousDelay);
	}
#endif
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_LABLE::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = Lable:[PAR] -->Set a label. No execution command, used with CMD Goto.");
	PrintB(env,"  Command = <'Lable:[PAR]>[//COMMENT]");
	PrintP(env,"   eg:");
	PrintP(env,"     Command = 'Lable:001      //used with CMD Goto, scrpit will jump to here");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_LABLE::Command(CMD_ENV* env,const STDSTR& msg,void* retStr)const{
	STDSTR *ret = static_cast<STDSTR*>(retStr);
	if (retStr != nullptr)
		*ret = Str_Trim(msg);
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
namespace BIF {
	static BIF_LABLE	gLable;
}
//------------------------------------------------------------------------------------------//
CMDID BIF_GOTO::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = Goto=<Lable>[,<Expression>] -->if receive data contains expression, then jump to Lable.");
	PrintB(env,"  Command = <'Goto=<Lable>[,<Expression>]>[//COMMENT]");
	PrintP(env,"  Notes:Expression is the same as Condition Expression.");
	PrintP(env,"   eg:");
	PrintP(env,"     Command = 'Goto=001,Leif Wen  //if receive data contains Leif Wen, then goto 001");
	PrintP(env,"     Command = 'Goto=001,'cts==h   //if CTS is high, then goto 001");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_GOTO::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
#ifdef GList_h
	STDSTR			strPar1,strPar2,retCMDLable;
	COMMAND_NODE	*cNode;
	bool32			blJump;
	bool32			blPrint;
	
	if ((env != nullptr) && (BIF_ENV::GetCommandNode(env) != nullptr)){
		blPrint = CheckEnvFlag(env,BIF_ENV::ENV_blPrintBIFInfo) && CheckEnvFlag(env,BIF_ENV::ENV_blPrintBIFExplain);
		SplitPar1(strPar1,strPar2,msg,",");
		if (strPar2.length() == 0 || BIF_Expression(env,strPar2)){
			if (blPrint)
				PrintWithDividingLine(env,"Try to execute:",DelComment(BIF_ENV::GetCommandNode(env)->StrCommand));
			blJump = G_FALSE;
			cNode = (COMMAND_NODE*)(GetDown(GetUp(BIF_ENV::GetCommandNode(env))));
			while(cNode != nullptr){
				if (cNode->blEnableSend){
					if ((BIF_ID_LABLE == BIF::gLable.Dispose(env,DelComment(cNode->StrCommand),&retCMDLable))
						&& (retCMDLable == strPar1)){
						if (blPrint)
							PrintStrNL(env,"Goto Lable:", retCMDLable);
						BIF_ENV::SetCommandNode(env,cNode);
						blJump = G_TRUE;
						break;
					}
				}
				cNode = (COMMAND_NODE*)(GetNext(cNode));
			}
			
			if ((blJump == G_FALSE) && blPrint)
				PrintStrNL(env,"No find suitable label, script will continue, no jump");
		}
	}
#endif
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_STOP::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = Stop=<Expression> -->if receive data contains expression, then stop to run.");
	PrintB(env,"  Command = <'Stop=<Expression>>[//COMMENT]");
	PrintP(env,"  Notes:Expression is the same as Condition Expression.");
	PrintP(env,"   eg:");
	PrintP(env,"     Command = 'Stop=Leif Wen  //if receive data contains Leif Wen, then stop");
	PrintP(env,"     Command = 'Stop='cts==h   //if CTS is high, then stop");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_STOP::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	if (env != nullptr){
		if (msg.length() == 0 || BIF_Expression(env,msg)){
			PrintExecute(env,"Expression matched, stop");
			BIF_ENV::RetCMD(env) = 6;
		}
	}
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
CMDID BIF_BREAK::Help(CMD_ENV* env,uint32 flag)const{
	PrintB(env,".CMD = Break=<Expression> -->if receive data contains expression, then break to run.");
	PrintB(env,"  Command = <'Break=<Expression>>[//COMMENT]");
	PrintP(env,"  Notes:Expression is the same as Condition Expression.");
	PrintP(env,"   eg:");
	PrintP(env,"     Command = 'Break=Leif Wen  //if receive data contains Leif Wen, then break");
	PrintP(env,"     Command = 'Break='cts==h   //if CTS is high, then break");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
CMDID BIF_BREAK::Command(CMD_ENV* env,const STDSTR& msg,void* p)const{
	if (env != nullptr){
		if (msg.length() == 0 || BIF_Expression(env,msg)){
			PrintExecute(env,"Expression matched, break");
			BIF_ENV::RetCMD(env) = 3;
		}
	}
	return(cgCommandID);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* BIF_Control_h */
