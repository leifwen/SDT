//
//  MSG_ID.cpp
//  SDT
//
//  Created by Leif Wen on 15/03/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "MSG_ID.h"
#ifdef MSG_ID_h
//------------------------------------------------------------------------------------------//
#include "DS_STRING.h"
//------------------------------------------------------------------------------------------//
STDSTR _GetMesgText(uint32 mID){
	STDSTR retStr;
	retStr = "(" + Str_ToStr(mID)+ ")";
	switch(mID){
		case MESG_NONE					:retStr += "MESG_NONE ";				break;
		case MESG_ID_TEST				:retStr += "MESG_ID_TEST ";				break;
		case MESG_INI_Handshake			:retStr += "MESG_INI_Handshake ";		break;
		case MESG_REQ_Handshake			:retStr += "MESG_REQ_Handshake ";		break;
		case MESG_REQ_RSAPuk			:retStr += "MESG_REQ_RSAPuk ";			break;
		case MESG_ANS_RSAPuk			:retStr += "MESG_ANS_RSAPuk ";			break;
		case MESG_REQ_UpdateAESKey		:retStr += "MESG_REQ_UpdateAESKey ";	break;
		case MESG_ANS_UpdateAESKey		:retStr += "MESG_ANS_UpdateAESKey ";	break;
		case MESG_REQ_Test				:retStr += "MESG_REQ_Test ";			break;
		case MESG_ANS_Test				:retStr += "MESG_ANS_Test ";			break;
		case MESG_REQ_Close				:retStr += "MESG_REQ_Close ";			break;
		case MESG_ANS_Close				:retStr += "MESG_ANS_Close ";			break;
			
		case MESG_REQ_SetupMServer		:retStr += "MESG_REQ_SetupMServer ";	break;
		case MESG_ANS_SetupMServer		:retStr += "MESG_ANS_SetupMServer ";	break;
		case MESG_REQ_CloseMServer		:retStr += "MESG_REQ_CloseMServer ";	break;
		case MESG_ANS_CloseMServer		:retStr += "MESG_ANS_CloseMServer ";	break;
		case MESG_REQ_ListMServer		:retStr += "MESG_REQ_ListMServer ";		break;
		case MESG_ANS_ListMServer		:retStr += "MESG_ANS_ListMServer ";		break;
		case MESG_REQ_SetupDataChannel	:retStr += "MESG_REQ_SetupDataChannel ";break;
		case MESG_ANS_SetupDataChannel	:retStr += "MESG_ANS_SetupDataChannel ";break;
		case MESG_REQ_Link				:retStr += "MESG_REQ_Link ";			break;
		case MESG_ANS_Link				:retStr += "MESG_ANS_Link ";			break;
			
		case MESG_REQ_SetupTerminal		:retStr += "MESG_REQ_SetupTerminal ";	break;
		case MESG_ANS_SetupTerminal		:retStr += "MESG_ANS_SetupTerminal ";	break;
		case MESG_REQ_CloseTerminal		:retStr += "MESG_REQ_CloseTerminal ";	break;
		case MESG_ANS_CloseTerminal		:retStr += "MESG_ANS_CloseTerminal ";	break;

		case MESG_REQ_Registration		:retStr += "MESG_REQ_Registration ";	break;
		case MESG_ANS_Registration		:retStr += "MESG_ANS_Registration ";	break;
		case MESG_REQ_WriteLicense		:retStr += "MESG_REQ_WriteLicense ";	break;
		case MESG_ANS_WriteLicense		:retStr += "MESG_ANS_WriteLicense ";	break;
			
		case MESG_REQ_License			:retStr += "MESG_REQ_License ";			break;
		case MESG_ANS_Approve			:retStr += "MESG_ANS_Approve ";			break;
		case MESG_ANS_Reject			:retStr += "MESG_ANS_Reject ";			break;
		default							:retStr = "";							break;
	}
	return(retStr);
}
//------------------------------------------------------------------------------------------//
#endif /* MSG_ID_h */
