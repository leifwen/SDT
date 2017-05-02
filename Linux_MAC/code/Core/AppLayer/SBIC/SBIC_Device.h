//
//  SBIC_Device.h
//  SDT
//
//  Created by Leif Wen on 04/04/2017.
//  Copyright © 2017 leif wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#include "SBIC_B.h"
//------------------------------------------------------------------------------------------//
#ifdef SBIC_BH
#ifndef SBIC_Device_hpp
#define SBIC_Device_hpp
#ifdef SBIC_Device_hpp
//------------------------------------------------------------------------------------------//
class SBIC_DTR : public SBIC_Node{
	public:
		SBIC_DTR(void) : SBIC_Node() {
			cgCommand = "dtr,=";
			cgReturnCode = SBI_RETCODE_DTR;
		};
		virtual ~SBIC_DTR(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = DTR=<H|L>[,<PAR>] -->Set DTR output high/low and delay PAR time, then goto send next \"Command\".");
			PrintB(env,"  Command = <'DTR=<H|L>[,<PAR>]>[//COMMENT]");
			PrintP(env,"   eg:");
			PrintP(env,"     Command = 'DTR=H,1.2    //Set DTR output high, delay 1.2s");
			PrintP(env,"     Command = 'DTR=L,1.2s   //Set DTR output low, delay 1.2s");
			PrintP(env,"     Command = 'DTR=L,500ms  //Set DTR output low, delay 500ms");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_RTS : public SBIC_Node{
	public:
		SBIC_RTS(void) : SBIC_Node() {
			cgCommand = "rts,=";
			cgReturnCode = SBI_RETCODE_RTS;
		};
		virtual ~SBIC_RTS(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = RTS=<H|L>[,<PAR>] -->Set RTS output high/low and delay PAR time, then goto send next \"Command\".");
			PrintB(env,"  Command = <'RTS=<H|L>[,<PAR>]>[//COMMENT]");
			PrintP(env,"   eg:");
			PrintP(env,"     Command = 'RTS=H,1.2    //Set RTS output high, delay 1.2s");
			PrintP(env,"     Command = 'RTS=L,1.2s   //Set RTS output low, delay 1.2s");
			PrintP(env,"     Command = 'RTS=L,500ms  //Set RTS output low, delay 500ms");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_OpenCom : public SBIC_Node{
	public:
		SBIC_OpenCom(void) : SBIC_Node() {
			cgCommand = "OpenCom,=/OCom,=";
			cgReturnCode = SBI_RETCODE_OPENCOM;
		};
		virtual ~SBIC_OpenCom(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = OpenCom=<PAR1>,<PAR2> -->Change COM to PAR1, baud rate to PAR2.");
			PrintB(env,"  Command = <'OpenCom=<PAR1>,<PAR2>>[//COMMENT]");
			PrintP(env,"   eg:");
			PrintP(env,"     Command = 'OpenCom=COM3,115200  //set COM is COM3, baud rate is 115200");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_ChangeBR : public SBIC_Node{
	public:
		SBIC_ChangeBR(void) : SBIC_Node() {
			cgCommand = "ChangeBR,=";
			cgReturnCode = SBI_RETCODE_CHANGEBR;
		};
		virtual ~SBIC_ChangeBR(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = ChangeBR=<PAR> -->Set baud rate.");
			PrintB(env,"   <PAR> = 1200,2400,9600,14400,19200,38400,56000,57600");
			PrintB(env,"          ,115200,128000,230400,256000,460800,912600.");
			PrintB(env,"  Command = <'ChangeBR=<PAR>>[//COMMENT]");
			PrintP(env,"   eg:");
			PrintP(env,"     Command = 'ChangeBR=115200  //set baud rate 115200");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_OpenTCPSocket : public SBIC_Node{
	public:
		SBIC_OpenTCPSocket(void) : SBIC_Node() {
			cgCommand = "OpenTCPSocket,=/OTCP,=";
			cgReturnCode = SBI_RETCODE_OPENTCPSOCKET;
		};
		virtual ~SBIC_OpenTCPSocket(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = OpenTCPSocket/OTCP=<PAR1>,<PAR2> -->Open TCP socket, PAR1 is IP, PAR2 is port.");
			PrintB(env,"  Command = <'OpenTCPSocket=<PAR1>,<PAR2>>[//COMMENT]");
			PrintP(env,"   eg:");
			PrintP(env,"     Command = 'OpenTCPSocket=127.0.0.1,9527  //Open TCP socket");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_CloseTCPSocket : public SBIC_Node{
	public:
		SBIC_CloseTCPSocket(void) : SBIC_Node() {
			cgCommand = "CloseTCPSocket,=/CTCP,=";
			cgReturnCode = SBI_RETCODE_CLOSETCPSOCKET;
		};
		virtual ~SBIC_CloseTCPSocket(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = CloseTCPSocket/CTCP -->Close socket.");
			PrintB(env,"  Command = <'CloseTCPSocket>[//COMMENT]");
			PrintP(env,"   eg:");
			PrintP(env,"     Command = 'CloseTCPSocket  //Close socket");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_OpenUDPSocket : public SBIC_Node{
	public:
		SBIC_OpenUDPSocket(void) : SBIC_Node() {
			cgCommand = "OpenUDPSocket,=/OUDP,=";
			cgReturnCode = SBI_RETCODE_OPENUDPSOCKET;
		};
		virtual ~SBIC_OpenUDPSocket(void){ ; };
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env, ".CMD = OpenUDPSocket/OUDP=<PAR1>,<PAR2> -->Open UDP socket, PAR1 is IP, PAR2 is port.");
			PrintB(env, "  Command = <'OpenUDPSocket=<PAR1>,<PAR2>>[//COMMENT]");
			PrintP(env, "   eg:");
			PrintP(env, "     Command = 'OpenUDPSocket=127.0.0.1,9527  //Open UDP socket");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_CloseUDPSocket : public SBIC_Node{
	public:
		SBIC_CloseUDPSocket(void) : SBIC_Node() {
			cgCommand = "CloseUDPSocket,=/CUDP,=";
			cgReturnCode = SBI_RETCODE_CLOSEUDPSOCKET;
		};
		virtual ~SBIC_CloseUDPSocket(void){ ; };
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env, ".CMD = CloseUDPSocket/CUDP -->Close socket.");
			PrintB(env, "  Command = <'CloseUDPSocket>[//COMMENT]");
			PrintP(env, "   eg:");
			PrintP(env, "     Command = 'CloseUDPSocket  //Close socket");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_SetRecvDataMode : public SBIC_Node{
	public:
		SBIC_SetRecvDataMode(void) : SBIC_Node() {
			cgCommand = "SetRecvDataMode,=";
			cgReturnCode = SBI_RETCODE_SETRECVDATAMODE;
		};
		virtual ~SBIC_SetRecvDataMode(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = SetRecvDataMode=<HEX|ASCII> -->Change receive data print mode, HEE or ASCII.");
			PrintB(env,"  Command = <'SetRecvDataMode=<HEX|ASCII>>[//COMMENT]");
			PrintP(env,"   eg:");
			PrintP(env,"     Command = 'SetRecvDataMode=HEX    //change to HEX mode");
			PrintP(env,"     Command = 'SetRecvDataMode=ASCII  //change to ASCII mode");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class SBIC_SetRecMsgReport : public SBIC_Node{
	public:
		SBIC_SetRecMsgReport(void) : SBIC_Node() {
			cgCommand = "SetRecMsgReport,=";
			cgReturnCode = SBI_RETCODE_SETRECMSGREPORT;
		};
		virtual ~SBIC_SetRecMsgReport(void){;};
	public:
		virtual	int32	Command	(SBIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(SBIC_ENV *env,int32 blDetail = 1)const{
			PrintB(env,".CMD = SetRecMsgReport=<Enable|Disable> -->Enable/Disable receive report.");
			PrintB(env,"  Command = <'SetRecMsgReport=<Enable|Disable>>[//COMMENT]");
			PrintP(env,"   eg:");
			PrintP(env,"     Command = 'SetRecMsgReport=Enable    //enable report");
			PrintP(env,"     Command = 'SetRecMsgReport=Disable   //disable report");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif /* SBIC_Device_hpp */
