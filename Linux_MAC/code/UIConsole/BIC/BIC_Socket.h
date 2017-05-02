/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: BIC_Socket.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
*/
//------------------------------------------------------------------------------------------//
#include "BIC_B.h"
//------------------------------------------------------------------------------------------//
#ifdef BIC_BH
#ifndef BIC_SocketH
#define BIC_SocketH
#ifdef BIC_SocketH
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
class BIC_TCP : public BIC_Node{
	public:
				 BIC_TCP(void) : BIC_Node() {cgCommand = "tcp";cgReturnCode = BI_RETCODE_TCP;};
		virtual ~BIC_TCP(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *eda)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Set as", Str_LowerCase(selfName), "TCP mode.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"[IP][PORT]","Destination IP and port.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_UDP : public BIC_Node{
	public:
				 BIC_UDP(void) : BIC_Node() {cgCommand = "udp";cgReturnCode = BI_RETCODE_UDP;};
		virtual ~BIC_UDP(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *eda)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Set as", Str_LowerCase(selfName), "UDP mode.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"[IP][PORT]","Destination IP and port.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_TCPS : public BIC_Node{
	public:
				 BIC_TCPS(void) : BIC_Node() {cgCommand = "tcps";cgReturnCode = BI_RETCODE_TCPS;};
		virtual ~BIC_TCPS(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *eda)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Set as", Str_LowerCase(selfName), "TCP server mode.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"[PORT]","Local listen port.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_UDPS : public BIC_Node{
	public:
				 BIC_UDPS(void) : BIC_Node() {cgCommand = "udps";cgReturnCode = BI_RETCODE_UDPS;};
		virtual ~BIC_UDPS(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *eda)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Set as", Str_LowerCase(selfName), "UDP server mode.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"[PORT]","Local listen port.");
			return(cgReturnCode);
	};
};
//------------------------------------------------------------------------------------------//
class BIC_PORT : public BIC_Node{
	public:
				 BIC_PORT(void) : BIC_Node() {cgCommand = "port";cgReturnCode = BI_RETCODE_PORT;};
		virtual ~BIC_PORT(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *eda)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Set port.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<n>","Port.");
			return(cgReturnCode);
		};
};
class ASOCKET;
//------------------------------------------------------------------------------------------//
class BIC_SI : public BIC_Node{
	public:
				 BIC_SI(void) : BIC_Node() {cgCommand = "si";cgReturnCode = BI_RETCODE_SI;};
		virtual ~BIC_SI(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *eda)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Socket information.");
			return(cgReturnCode);
		};
	    static	int32	BIC_SI_LS_PrintSocket(BIC_ENV *env,ASOCKET *socket);
};
//------------------------------------------------------------------------------------------//
class BIC_SS : public BIC_Node{
	public:
				 BIC_SS(void) : BIC_Node() {cgCommand = "ss";cgReturnCode = BI_RETCODE_SS;};
		virtual ~BIC_SS(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *eda)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Socket select.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<num>","Socket number.");
			return(cgReturnCode);
		}
};
//------------------------------------------------------------------------------------------//
class BIC_SSD : public BIC_Node{
	public:
				 BIC_SSD(void) : BIC_Node() {cgCommand = "ssd";cgReturnCode = BI_RETCODE_SSD;};
		virtual ~BIC_SSD(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *eda)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Socket shutdown.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<num>","Socket number.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_ASOCKET : public BIC_Node_S{
	public:
				 BIC_ASOCKET(void) : BIC_Node_S() {cgblTrySubCMD = 1;cgCommand = "socket";cgTitle = cgCommand;cgReturnCode = BI_RETCODE_ASOCKET;Init();};
		virtual ~BIC_ASOCKET(void){;};
	public:
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			if (blDetail == 1){
				HelpTraversalChild(env,0);
				return(cgReturnCode);
			}
			PrintHelpItem(env, cgCommand, "->", selfName, "Socket setting.");
			if (blDetail == 0)
				return(cgReturnCode);
			return(cgReturnCode);
		};
	private:
		void	Init(void){
			*this
			#ifdef	SWVERSION_TCPClient
			< cgSub_tcp
			#endif
			#ifdef	SWVERSION_UDPCLient
			< cgSub_udp
			#endif
			#ifdef	SWVERSION_TCPServer
			< cgSub_tcps
			#endif
			#ifdef	SWVERSION_UDPServer
			< cgSub_udps
			#endif
			#ifdef SWVERSION_PORT
			< cgSub_port
			#endif
			#ifdef SWVERSION_Server
			< cgSub_si < cgSub_ss < cgSub_ssd
			#endif
			;
		}
	private:
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
};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif
