//
//  BIF_CMD.cpp
//  SDT
//
//  Created by Leif Wen on 22/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "BIF_CMD.h"
#ifdef BIF_CMD_h
#include "BIF_Expression.h"
//------------------------------------------------------------------------------------------//
BIF_CMD::BIF_CMD(void) : BIF_BASE(){
	cgCommandID = BIF_ID_CMD;
	cgCommand = "CMD,=";

	SetSFlag(CMD_blTrySubCMD);
	
	*this
	< cgSubC_BIC
#ifdef BIF_Transform_h
	< cgSubC_Print
	< cgSubC_Search
#endif
#ifdef BIF_Control_h
	< cgSubC_Delay < cgSubC_Wait < cgSubC_Synchronous
	< cgSubC_Lable < cgSubC_Goto < cgSubC_Stop < cgSubC_Break
#endif
#ifdef BIF_Dev_h
	< cgSubC_Open < cgSubC_Close < cgSubC_BR < cgSubC_DTR < cgSubC_RTS
	< cgSubC_SetRecvDataMode < cgSubC_SetRecMsgReport
#endif
#ifdef BIF_EXE_h
	< cgSubC_RunEXE < cgSubC_StopEXE < cgSubC_WaitEXE < cgSubC_GetEXE
#endif
	;
};
//------------------------------------------------------------------------------------------//
CMDID BIF_CMD::Help(CMD_ENV* env,uint32 flag)const{
	STDSTR		strTemp;
	
	strTemp  = "Written by Leif Wen,Began in 2012-02-09\n\n";
	strTemp += SWVERSION_SHORTNAME;
	strTemp += " Help ";
	strTemp += SWVERSION_VER;
	PrintM(env,strTemp);
	
	PrintP(env,"----------------------------------------------------------------------");
	PrintM(env,"Script Introduction:"													  );
	PrintM(env,"Command            : ","Data or build-in commands."					  );
	PrintM(env,"Timeout(s)         : ","Receive timeout."								  );
	PrintM(env,"Cycle              : ","Cycle times,if blank,Cycle = 1 as default."	  );
	PrintM(env,"Send<CR>           : ","Add \\r(0x0D) in the end of command."			  );
	PrintM(env,"HEX                : ","Command is HEX format."						  );
	PrintM(env,"Continue Condition : ","Continue expression. If blank, do not check it.");
	PrintM(env,"Stop Condition     : ","Stop expression. If blank, do not check it."	  );
	PrintM(env,"Resend Condition   : ","Resend expression. If blank, do not check it."  );
	PrintM(env,"Catch Condition    : ","Log expression. If blank, do not check it."	  );
	PrintP(env,"----------------------------------------------------------------------");
	
	PrintM(env,"How does \"Condition\" work:");
	PrintM(env," A. After send \"Command\",");
	PrintM(env,"   1. If no receive timeout:");
	PrintP(env,"     1.1  If recevie some data:");
	PrintB(env,"        1.1.1  Check \"Catch Condition\".");
	PrintB(env,"        1.1.2  If received data contains \"Catch Condition\", the build-in \"catch counter\" is");
	PrintB(env,"               incremented by one and report the result once the scprit stop. It's only valid");
	PrintB(env,"               in \"Execute Automatically Command Group List\" test.");
	PrintB(env,"        1.1.3  Check \"Continue Condition\".");
	PrintB(env,"        1.1.4  If received data contains \"Continue Condition\", then send next \"Command\".");
	PrintB(env,"        1.1.5  Else check \"Resend Condition\".");
	PrintB(env,"        1.1.6  If received data contains \"Resend Condition\", then resend this \"Command\".");
	PrintB(env,"        1.1.7  Else check \"Stop Condition\".");
	PrintB(env,"        1.1.8  If received data contains \"Stop Condition\", the script execution will be stopped.");
	PrintB(env,"        1.1.9  Else check whether is timeout.");
	PrintB(env,"        1.1.10 If timeout, \"Cycle\" minus one, and resend this \"Command\" until \"Cycle\" = 0,");
	PrintB(env,"               	then send next \"Command\".");
	PrintB(env,"        1.1.11 Else if no timeout, waitting to receive data.");
	PrintP(env,"     1.2  Else waitting to receive data.");
	PrintM(env,"   2. If timeout:");
	PrintP(env,"     2.1  If no recevie any data, the build-in \"no response counter\" is incremented by one and");
	PrintP(env,"          report the result when the scprit stop. It's only valid in \"Execute Automatically");
	PrintP(env,"          Command Group List\" test.");
	PrintP(env,"     2.2  \"Cycle\" minus one, and resend this \"Command\" until \"Cycle\" = 0, then send next");
	PrintP(env,"          \"Command\".");
	PrintM(env," B. \"Condition\"'s arithmetic expression:");
	PrintM(env,"   1. Support AND, OR, NOR, [, ] operators.");
	PrintP(env,"     1.1  AND operators is : && or &.");
	PrintP(env,"     1.2  OR operators is  : || or |.");
	PrintP(env,"     1.3  NOR operators is  :!.");
	PrintM(env,"   2. Automatic delete spaces character(0x20) before and after the \"Condition\" expression.");
	PrintP(env,"     2.1  Eg. The expression is ABCD && DEF,");
	PrintP(env,"              if the received data contains the ABCD and DEF, the expression is true.");
	PrintP(env,"     2.2  Eg. The expression is \"ABCD \" && DEF,");
	PrintP(env,"              if the received data contains 'ABCD '(5bytes) and DEF, the expression is true.");
	PrintM(env,"   3. if use &, |,! , [, ], \\, need use \\&, \\|, \\!, \\[, \\], \\\\ to escape.");
	PrintM(env,"   4. Support hexadecimal escape input,Format:\\0xhh or \\0Xhh.");
	PrintM(env,"   5. Support these escape character: \\a, \\b, \\f, \\n, \\r, \\t, \\v, \\\\, \\', \\\", \\0, \\/, \\*, \\?.");
	PrintM(env,"   6. Support build-in command:");
	PrintM(env,"      'Receive, 'CTS, 'DSR, 'RING, 'DCD, 'Timeout, 'nullptr, 'RX, 'LCS.");
	PrintM(env,"   7. Support * and ? compare.");
	
	PrintP(env,"----------------------------------------------------------------------");
	PrintM(env,"Build-in Command Format:");
	PrintM(env,"   A.Command = <'CMD>[//COMMENT].");
	PrintM(env,"   B.Command = [<data>][//COMMENT].");
	PrintP(env, "Notes:");
	PrintP(env,"   1.Only accept visible characters(To use escape character instead of invisible characters).");
	PrintP(env,"   2.<> means: delete spaces character(0x20) before and after CMD. [] means: options.");
	PrintP(env,"   3.Support hexadecimal escape input,Format:\\0xhh or \\0Xhh.");
	PrintP(env,"   4.Support these escape character: \\a, \\b, \\f, \\n, \\r, \\t, \\v, \\\\, \\', \\\", \\0, \\/, \\*, \\?.");
	PrintP(env,"----------------------------------------------------------------------");
	PrintM(env,"CMD List:(CMD Ignore case)");
	
	TraversalChildHelp(env,1);
	BIF_COMBINE::GetCombine().Help(env,1);
	BIF_CONDITION::GetCondition().Help(env,1);
	
	PrintP(env,"----------------------------------------------------------------------");
	PrintM(env,"End");
	return(cgCommandID);
};
//------------------------------------------------------------------------------------------//
#endif /* BIF_CMD_h */
