//
//  BIC_Socket.h
//  SDT
//
//  Created by Leif Wen on 13/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "BIC_Dev.h"
#if defined BIC_Dev_h && defined Commu_Socket_h
//------------------------------------------------------------------------------------------//
#ifndef BIC_Socket_h
#define BIC_Socket_h
#ifdef BIC_Socket_h
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
BIC_CLASSTYPE	(TCP,	"tcp");
BIC_CLASSTYPE	(UDP,	"udp");
BIC_CLASSTYPE	(TCPS,	"tcps");
BIC_CLASSTYPE	(UDPS,	"udps");
BIC_CLASSTYPE	(PORT,	"port");
BIC_CLASSTYPE	(SI,	"si");
BIC_CLASSTYPE	(SS,	"ss");
BIC_CLASSTYPE	(SD,	"sd");
//------------------------------------------------------------------------------------------//
class BIC_ASOCKET : public BIC_BASE_S{
	public:
				 BIC_ASOCKET(void);
		virtual ~BIC_ASOCKET(void){;};
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
		BIC_SD			cgSub_sd;
#endif
};
//------------------------------------------------------------------------------------------//
#else
	#undef SWVERSION_TCPClient
	#undef SWVERSION_UDPCLient
	#undef SWVERSION_TCPServer
	#undef SWVERSION_UDPServer
	#undef SWVERSION_PORT
	#undef SWVERSION_Server
#endif /* BIC_Socket_h */
#endif /* BIC_Socket_h */
#else
	#undef SWVERSION_TCPClient
	#undef SWVERSION_UDPCLient
	#undef SWVERSION_TCPServer
	#undef SWVERSION_UDPServer
	#undef SWVERSION_PORT
	#undef SWVERSION_Server
#endif /* BIC_Dev_h */
