/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: BIC_Connect.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
*/
#ifndef BIC_ConnectH
#define BIC_ConnectH
//------------------------------------------------------------------------------------------//
#ifdef	SWVERSION_TCPClient
#define SWVERSION_PORT
#endif
#ifdef	SWVERSION_UDPCLient
#ifndef SWVERSION_PORT
#define SWVERSION_PORT
#endif
#endif
#ifdef	SWVERSION_TCPServer
#define SWVERSION_Server
#endif
#ifdef	SWVERSION_UDPServer
#ifndef SWVERSION_Server
#define SWVERSION_Server
#endif
#endif
//------------------------------------------------------------------------------------------//
#include "BIC_B.h"
#include "BIC_COM.h"
#include "BIC_Socket.h"
#include "BIC_Script.h"
//------------------------------------------------------------------------------------------//
class BIC_CONNECT : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_CONNECT(void) : BIC_Node() {cgCommand = "c/connect";cgReturnCode = BI_RETCODE_CONNECT;};
		virtual ~BIC_CONNECT(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_DISCONNECT : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_DISCONNECT(void) : BIC_Node() {cgCommand = "d/disconnect";cgReturnCode = BI_RETCODE_DISCONNECT;};
		virtual ~BIC_DISCONNECT(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_DISPLAY : public BIC_Node{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
			     BIC_DISPLAY(void) : BIC_Node() {cgCommand = "display";cgReturnCode = BI_RETCODE_DISPLAY;};
	    virtual ~BIC_DISPLAY(void){;};
	public:
	    virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
	    virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_CR : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_CR(void) : BIC_Node() {cgCommand = "cr";cgReturnCode = BI_RETCODE_CR;};
		virtual ~BIC_CR(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_ECHO : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_ECHO(void) : BIC_Node() {cgCommand = "echo";cgReturnCode = BI_RETCODE_ECHO;};
		virtual ~BIC_ECHO(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_REC : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_REC(void) : BIC_Node() {cgCommand = "rec";cgReturnCode = BI_RETCODE_REC;};
		virtual ~BIC_REC(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};
//------------------------------------------------------------------------------------------//
class BIC_DEV : public BIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = BIC_Node::RFLAG_S + BIC_Node::RFLAG_C};
	public:
				 BIC_DEV(void) : BIC_Node() {cgCommand = "dev";cgReturnCode = BI_RETCODE_DEV;};
		virtual ~BIC_DEV(void){;};
	public:
		virtual	int32	Command	(BICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const;
};

//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class BIC_MAIN : public BIC_Node_S{
	public:
	    enum{RFLAG_C = 0, RFLAG_S = BIC_Node_S::RFLAG_S + BIC_Node_S::RFLAG_C};
	public:
	    		 BIC_MAIN(void) : BIC_Node_S() {cgCommand = "main";cgTitle = cgCommand;cgReturnCode = BI_RETCODE_MAIN;Init();};
	    virtual ~BIC_MAIN(void){;};
	public:
		virtual	int32	Help	(BICPAR *tBICPAR,int32 blDetail = 1)const{
			PrintHelpItem(tBICPAR, cgCommand, "->Main connection setting.");
			if (blDetail == 0)
				return(cgReturnCode);
			return(cgReturnCode);
		};
	private:
	    void	Init(void){
			
				AddNode(&cgC_CONNECT);
				AddNode(&cgC_DISCONNECT);
				AddNode(&cgC_CR);
			
				AddNode(&cgC_DEV);
	            AddNode(&cgC_ECHO);
	            AddNode(&cgC_REC);

        	#ifdef SWVERSION_COM
	            AddNode(&cgSub_com_ls);
        	    AddNode(&cgSub_com);
        	    AddNode(&cgSub_com_br);
	            AddNode(&cgSub_com_ms);
	            AddNode(&cgSub_com_dtr);
	            AddNode(&cgSub_com_rts);
			#endif
			#ifdef	SWVERSION_TCPClient
        	    AddNode(&cgSub_tcp);
			#endif
			#ifdef	SWVERSION_UDPCLient
        	    AddNode(&cgSub_udp);
			#endif
			#ifdef	SWVERSION_TCPServer
        	    AddNode(&cgSub_tcps);
			#endif
			#ifdef	SWVERSION_UDPServer
        	    AddNode(&cgSub_udps);
			#endif
			#ifdef SWVERSION_PORT
        	    AddNode(&cgSub_port);
			#endif
			#ifdef SWVERSION_Server
        	    AddNode(&cgSub_si);
        	    AddNode(&cgSub_ss);
        	    AddNode(&cgSub_ssd);
			#endif
			#ifdef SWVERSION_SCRIPT
				AddNode(&cgC_STOP);
				AddNode(&cgC_RUN);
			
				AddNode(&cgC_SEND);
				AddNode(&cgC_SENDA);
				AddNode(&cgC_SENDFILE);
			#endif
	    }
		BIC_CONNECT		cgC_CONNECT;
		BIC_DISCONNECT	cgC_DISCONNECT;
		BIC_CR			cgC_CR;
	
	    BIC_ECHO		cgC_ECHO;
	    BIC_REC			cgC_REC;
	    BIC_DEV			cgC_DEV;
	#ifdef SWVERSION_COM
	    BIC_COM_LS		cgSub_com_ls;
	    BIC_COM			cgSub_com;
	    BIC_COM_BR		cgSub_com_br;
	    BIC_COM_MS		cgSub_com_ms;
	    BIC_COM_DTR		cgSub_com_dtr;
	    BIC_COM_RTS		cgSub_com_rts;
	#endif
	#ifdef	SWVERSION_TCPClient
	    BIC_TCP			cgSub_tcp;
	#endif
	#ifdef	SWVERSION_UDPCLient
	    BIC_UDP			cgSub_udp;
	#endif
	#ifdef	SWVERSION_TCPServer
	    BIC_TCPS		cgSub_tcps;
	#endif
	#ifdef	SWVERSION_UDPServer
	    BIC_UDPS		cgSub_udps;
	#endif
	#ifdef SWVERSION_PORT
	    BIC_PORT		cgSub_port;
	#endif
	#ifdef SWVERSION_Server
	    BIC_SI			cgSub_si;
	    BIC_SS			cgSub_ss;
	    BIC_SSD			cgSub_ssd;
	#endif
	#ifdef SWVERSION_SCRIPT
		BIC_STOP		cgC_STOP;
		BIC_RUN			cgC_RUN;
	
		BIC_SEND		cgC_SEND;
		BIC_SENDA		cgC_SENDA;
		BIC_SENDFILE	cgC_SENDFILE;
	#endif
};
//------------------------------------------------------------------------------------------//
#endif
