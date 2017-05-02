//
//  SBIC_CMD.h
//  SDT
//
//  Created by Leif Wen on 03/04/2017.
//  Copyright © 2017 leif wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#include "SBIC.h"
//------------------------------------------------------------------------------------------//
#ifdef SBIC_BH
#ifndef SBIC_CMD_hpp
#define SBIC_CMD_hpp
#ifdef SBIC_CMD_hpp
//------------------------------------------------------------------------------------------//
class SBIC_CMD : public SBIC_Node{
	public:
		virtual ~SBIC_CMD(void){;};
		SBIC_CMD(void){
			*this
			#ifdef SBIC_BuildH
			< cgSubC_PRINT
			#endif
			#ifdef SBIC_SearchH
			< cgSubC_Search
			#endif
			#ifdef SBIC_Device_hpp
			< cgSubC_OpenCom < cgSubC_ChangeBR < cgSubC_DTR < cgSubC_RTS
			< cgSubC_OpenTCPSocket < cgSubC_CloseTCPSocket < cgSubC_OpenUDPSocket < cgSubC_CloseUDPSocket
			< cgSubC_SetRecvDataMode < cgSubC_SetRecMsgReport
			#endif
			#ifdef SBIC_ExpressionH
			< cgSubC_Delay < cgSubC_Synchronous
			< cgSubC_Wait < cgSubC_Lable <cgSubC_GOTO <cgSubC_STOP < cgSubC_BREAK
			#endif
			#ifdef SBIC_RunEXEH
			< cgSubC_RunEXE
			< cgSubC_StopEXE
			#endif
			;
		};
	public:
		#ifdef SBIC_BuildH
		SBIC_Print				cgSubC_PRINT;
		#endif
		#ifdef SBIC_SearchH
		SBIC_Search				cgSubC_Search;
		#endif
		#ifdef SBIC_Device_hpp
		SBIC_DTR				cgSubC_DTR;
		SBIC_RTS				cgSubC_RTS;
		SBIC_OpenCom			cgSubC_OpenCom;
		SBIC_ChangeBR			cgSubC_ChangeBR;
		SBIC_OpenTCPSocket		cgSubC_OpenTCPSocket;
		SBIC_CloseTCPSocket		cgSubC_CloseTCPSocket;
		SBIC_OpenUDPSocket		cgSubC_OpenUDPSocket;
		SBIC_CloseUDPSocket		cgSubC_CloseUDPSocket;
	
		SBIC_SetRecvDataMode	cgSubC_SetRecvDataMode;
		SBIC_SetRecMsgReport	cgSubC_SetRecMsgReport;
		#endif
		#ifdef SBIC_ExpressionH
		SBIC_Expression			cgSubC_Expression;
	
		SBIC_Delay				cgSubC_Delay;
		SBIC_Synchronous		cgSubC_Synchronous;
	
		SBIC_Wait				cgSubC_Wait;
		SBIC_Lable				cgSubC_Lable;
		SBIC_GOTO				cgSubC_GOTO;
		SBIC_STOP				cgSubC_STOP;
		SBIC_BREAK				cgSubC_BREAK;
		#endif
		#ifdef SBIC_RunEXEH
			AEXEPOOL			cgAExePool;
			SBIC_RunEXE			cgSubC_RunEXE;
			SBIC_StopEXE		cgSubC_StopEXE;
		#endif
};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif /* SBIC_CMD_hpp */

