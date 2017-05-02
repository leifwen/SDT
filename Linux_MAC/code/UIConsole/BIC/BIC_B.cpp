/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: BIC_B.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "BIC_B.h"
#include "Comm_FIFO.h"
#include "Comm_Convert.h"
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Unix
#include <fcntl.h>
#include <signal.h>
#include <termios.h>
#endif
//------------------------------------------------------------------------------------------//
#ifdef BIC_BH
//------------------------------------------------------------------------------------------//
int32 BIC_Node::Execute(BIC_ENV *env,const STDSTR &rawIn,void *p)const{
	STDSTR	 par;
	
	if (Check(env,rawIn,&par) == cgReturnCode){
		if ((par == "?") || (par == "-help"))
			return(Help(env));
		return(Command(env,par,p));
	}
	else if ((cgblTrySubCMD != 0) && (rawIn.length() > 0)){
		return(Command(env,rawIn,p));
	}
	return(BI_RETCODE_NO);
}
//------------------------------------------------------------------------------------------//
int32 BIC_Node::Check(BIC_ENV *env,const STDSTR &rawIn,STDSTR *retPar)const{
	STDSTR	lRawIn,strT;
	STDSTR	subCommand,oCommand;
	STDSTR	cmdIn,parIn;
	
	oCommand = cgCommand;
	lRawIn = Str_LTrim(rawIn);
	
	do{
		strT = lRawIn;
		SplitPar1(cmdIn, parIn, strT);
		subCommand = Str_ReadSubItem(&oCommand,"/");
		if (cmdIn == subCommand){
			*retPar = parIn;
			return(cgReturnCode);
		}
		subCommand += '?';
		if (cmdIn == subCommand){
			*retPar = '?';
			return(cgReturnCode);
		}
		subCommand += '?';
		if (strT == subCommand){
			*retPar = "??";
			return(cgReturnCode);
		}
	}while (oCommand.length() > 0);
	*retPar = "";
	return(BI_RETCODE_NO);
}
//------------------------------------------------------------------------------------------//
int32 BIC_Node::Command(BIC_ENV *env,const STDSTR &par,void *p)const{
	int32	retCode;
	
	retCode = ExecuteAsConsole(env,par,"",p);
	if (retCode == BI_RETCODE_RETURN)
		return(cgReturnCode);
	return(retCode);
}
//------------------------------------------------------------------------------------------//
int32 BIC_Node::ExecuteAsConsole(BIC_ENV *env,const STDSTR &cmd,const STDSTR &exTitle,void *p)const{
	STDSTR		command;
	int32		retCode;
	
	retCode = cgReturnCode;
	if (cmd.length() > 0){
		retCode = ExecuteTraversalChild(env, cmd, p);
	}
	else{
		while(env->blExit != -1){
			ClrblExit(env);
			PrintNL(env) << COL_clDefault << cgPrintTitle << exTitle << ">" << Endl;
			
			if (BI_ReadCommand(env,&command) == 0)
				return(BI_RETCODE_NO);
			
			Str_TrimSelf(command);
			
			retCode = ExecuteTraversalChild(env,command, p);
			if ((retCode == BI_RETCODE_RETURN) || (retCode == BI_RETCODE_EXIT))
				break;
		}
	}
	return(retCode);
}
//------------------------------------------------------------------------------------------//
uint8 BIC_Tools::BI_ReadChar(BIC_ENV *env,int32 blWait){
	uint8	charGet = 0;
	
	while(env->blExit == 0){
		if (env->cstdin ->Get(&charGet,1) > 0)
			break;
		charGet = 0;
		if (blWait == 0)
			break;
		SYS_SleepMS(10);
	}
	return(charGet);
}
//------------------------------------------------------------------------------------------//
int32 BIC_Tools::BI_ReadCommand(BIC_ENV *env,STDSTR *retCMD){
	uint8	charGet = 0;
	
	*retCMD = "";
	while(env->blExit == 0){
		charGet = BI_ReadChar(env,1);
		if ((charGet >= 0x20) && (charGet <= 0x7e)){
			*retCMD += charGet;
		}
		else if ((charGet == '\r') || (charGet == '\n')){
			return 1;
		}
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
uint8 BIC_Tools::PressAnyKey(BIC_ENV *env,ExpandDeviceAttr *eda,SYS_TIME_S *dly){
	uint8	retChar;
	retChar = 0;
	while(env->blExit == 0){
		env->blInPressKeyMode = 1;
		retChar = BI_ReadChar(env,0);
		if (retChar > 0)
			break;
		if (dly == nullptr)
			SYS_SleepMS(10);
		if (eda->IsConnected() == 0)
			break;
		if (SYS_Delay_CheckTS(dly) != 0)
			break;
	}
	env->blInPressKeyMode = 0;
	return(retChar);
}
//------------------------------------------------------------------------------------------//
uint8 BIC_Tools::PrintPressAnyKey(BIC_ENV *env){
	uint8	retChar = 0;
	if (env->blExit == 0){
		env->blInPressKeyMode = 1;
		PrintALine(env,"Press any key to continue.");
		retChar = BI_ReadChar(env);
	}
	env->blInPressKeyMode = 0;
	return(retChar);
}
//------------------------------------------------------------------------------------------//
void BIC_Tools::PrintStr(BIC_ENV *env,const COLSTR &colStr1,const COLSTR &colStr2,const COLSTR &colStr3,const COLSTR &colStr4
						  ,const COLSTR &colStr5,const COLSTR &colStr6,const COLSTR &colStr7,const COLSTR &colStr8){
	OUTPUT_NODE::PrintStr(env->cstdout,colStr1,colStr2,colStr3,colStr4,colStr5,colStr6,colStr7,colStr8);
}
//------------------------------------------------------------------------------------------//
void BIC_Tools::PrintStrNL(BIC_ENV *env,const COLSTR &colStr1,const COLSTR &colStr2,const COLSTR &colStr3,const COLSTR &colStr4
							,const COLSTR &colStr5,const COLSTR &colStr6,const COLSTR &colStr7,const COLSTR &colStr8){
	OUTPUT_NODE::PrintStrNL(env->cstdout,colStr1,colStr2,colStr3,colStr4,colStr5,colStr6,colStr7,colStr8);
}
//------------------------------------------------------------------------------------------//
void BIC_Tools::PrintWithTime(BIC_ENV *env,const COLSTR &colStr1,const COLSTR &colStr2,const COLSTR &colStr3,const COLSTR &colStr4
							   ,const COLSTR &colStr5,const COLSTR &colStr6,const COLSTR &colStr7,const COLSTR &colStr8){
	OUTPUT_NODE::PrintWithTime(env->cstdout,colStr1,colStr2,colStr3,colStr4,colStr5,colStr6,colStr7,colStr8);
}
//------------------------------------------------------------------------------------------//
void BIC_Tools::PrintWithDividingLine(BIC_ENV *env,const COLSTR &colStr1,const COLSTR &colStr2,const COLSTR &colStr3,const COLSTR &colStr4
									   ,const COLSTR &colStr5,const COLSTR &colStr6,const COLSTR &colStr7,const COLSTR &colStr8){
	OUTPUT_NODE::PrintWithDividingLine(env->cstdout,colStr1,colStr2,colStr3,colStr4,colStr5,colStr6,colStr7,colStr8);
}
//------------------------------------------------------------------------------------------//
void BIC_Tools::PrintMessage(BIC_ENV *env,const COLSTR &colStr1,const COLSTR &colStr2,const COLSTR &colStr3,const COLSTR &colStr4
							  ,const COLSTR &colStr5,const COLSTR &colStr6,const COLSTR &colStr7,const COLSTR &colStr8){
	OUTPUT_NODE::PrintMessage(env->cstdout,colStr1,colStr2,colStr3,colStr4,colStr5,colStr6,colStr7,colStr8);
}
//------------------------------------------------------------------------------------------//
void BIC_Tools::PrintNormalMessage(BIC_ENV *env,const STDSTR &strData1,const STDSTR &strData2,const STDSTR &strData3,const STDSTR &strData4
									,const STDSTR &strData5,const STDSTR &strData6,const STDSTR &strData7,const STDSTR &strData8){
	OUTPUT_NODE::PrintNormalMessage(env->cstdout,strData1,strData2,strData3,strData4,strData5,strData6,strData7,strData8);
}
//------------------------------------------------------------------------------------------//
void BIC_Tools::PrintWarningMessage(BIC_ENV *env,const STDSTR &strData1C1,const STDSTR &strData2C2,const STDSTR &strData3C2,const STDSTR &strData4C2
									 ,const STDSTR &strData5C2,const STDSTR &strData6C2,const STDSTR &strData7C2,const STDSTR &strData8C2){
	OUTPUT_NODE::PrintWarningMessage(env->cstdout,strData1C1,strData2C2,strData3C2,strData4C2,strData5C2,strData6C2,strData7C2,strData8C2);
}
//------------------------------------------------------------------------------------------//
void BIC_Tools::PrintALine(BIC_ENV *env,const COLSTR &colStr1,const COLSTR &colStr2,const COLSTR &colStr3,const COLSTR &colStr4
						   ,const COLSTR &colStr5,const COLSTR &colStr6,const COLSTR &colStr7,const COLSTR &colStr8){
	if (env->cstdout != nullptr){
		*env->cstdout << Start << NL
		<< COL_clDYellow
		<< ColStr(colStr1)
		<< ColStr(colStr2)
		<< ColStr(colStr3)
		<< ColStr(colStr4)
		<< ColStr(colStr5)
		<< ColStr(colStr6)
		<< ColStr(colStr7)
		<< ColStr(colStr8)
		<< NL << Endl;
	}
}
//------------------------------------------------------------------------------------------//
void BIC_Tools::PrintFail(BIC_ENV *env,const COLSTR &colStr1,const COLSTR &colStr2,const COLSTR &colStr3,const COLSTR &colStr4){
	if (env->cstdout != nullptr){
		*env->cstdout << Start << NL
		<< COL_Fail
		<< " [BIC] Execute fail"
		<< ((colStr1.p->length() == 0)?"":". Due to")
		<< ColStr(colStr1)
		<< ColStr(colStr2)
		<< ColStr(colStr3)
		<< ColStr(colStr4)
		<< "."
		<< NL << Endl;
	}
};
//------------------------------------------------------------------------------------------//
void BIC_Tools::PrintSuccess(BIC_ENV *env,const COLSTR &colStr1,const COLSTR &colStr2,const COLSTR &colStr3,const COLSTR &colStr4){
	if (env->cstdout != nullptr){
		*env->cstdout << Start << NL
		<< COL_Sucess
		<< " [BIC] Execute success"
		<< ((colStr1.p->length() == 0)?"":".")
		<< ColStr(colStr1)
		<< ColStr(colStr2)
		<< ColStr(colStr3)
		<< ColStr(colStr4)
		<< "."
		<< NL << Endl;
	}
};
//------------------------------------------------------------------------------------------//
void BIC_Tools::PrintResult(BIC_ENV *env,const COLSTR &colStr1,const COLSTR &colStr2,const COLSTR &colStr3,const COLSTR &colStr4
							,const COLSTR &colStr5,const COLSTR &colStr6,const COLSTR &colStr7,const COLSTR &colStr8){
	if (env->cstdout != nullptr){
		*env->cstdout << Start << NL
		<< COL_Result
		<< " [BIC]"
		<< ColStr(colStr1)
		<< ColStr(colStr2)
		<< ColStr(colStr3)
		<< ColStr(colStr4)
		<< ColStr(colStr5)
		<< ColStr(colStr6)
		<< ColStr(colStr7)
		<< ColStr(colStr8)
		<< "."
		<< NL << Endl;
	}
};
//------------------------------------------------------------------------------------------//
void BIC_Tools::PrintTitle(BIC_ENV *env,CSType tDevType,int32 blPrintTail){
	PrintNL(env);
	switch(tDevType){
		case CSType_COM:
		case CSType_COMV:
			STDout(env)
			<< ColData(COL_clBlue	,"COM")
			<< ColData(COL_clDefault,":")
			<< ColData(COL_clDYellow,env->eda->aCOM.name)
			<< ColData(COL_clDefault,"@")
			<< ColData(COL_clDYellow,Str_ToString(env->eda->aCOM.port));
			break;
		case CSType_TCP:
			STDout(env)
			<< ColData(COL_clBlue	,"TCP")
			<< ColData(COL_clDefault,":")
			<< ColData(COL_clDYellow,env->eda->aTCP.name)
			<< ColData(COL_clDefault,"@")
			<< ColData(COL_clDYellow,Str_ToString(env->eda->aTCP.port));
			break;
		case CSType_UDP:
			STDout(env)
			<< ColData(COL_clBlue	,"UDP")
			<< ColData(COL_clDefault,":")
			<< ColData(COL_clDYellow,env->eda->aUDP.name)
			<< ColData(COL_clDefault,"@")
			<< ColData(COL_clDYellow,Str_ToString(env->eda->aUDP.port));
			break;
		case CSType_TCPS:
			STDout(env)
			<< ColData(COL_clBlue	,"TCP Server")
			<< ColData(COL_clDefault,"@")
			<< ColData(COL_clDYellow,Str_ToString(env->eda->aTCPS.port));
			break;
		case CSType_UDPS:
			STDout(env)
			<< ColData(COL_clBlue	,"UDP Server")
			<< ColData(COL_clDefault,"@")
			<< ColData(COL_clDYellow,Str_ToString(env->eda->aUDPS.port));
			break;
		case CSType_None:
		default:
			STDout(env) << ColData(COL_clDefault,SWVERSION_SHORTNAME);
			break;
	}
	if (blPrintTail != 0)
		STDout(env) << ColData(COL_clDefault,">");
	STDout(env) << Endl;
	return;
}
//------------------------------------------------------------------------------------------//
void BIC_Tools::PrintHelpItem(BIC_ENV *env,const STDSTR &command,const STDSTR &desp1,const STDSTR &desp2,const STDSTR &desp3,const STDSTR &desp4){
	STDSTR		space;
	space = "";
	if (command.length() < 22)
		space.insert(space.length(),22 - command.length(),' ');

	PrintALine(env,Data(COL_clBlue,&command),space,Data(COL_clDefault,&desp1),desp2,desp3,desp4);
}
//------------------------------------------------------------------------------------------//
void BIC_Tools::PrintHelpSubItem(BIC_ENV *env,const STDSTR &command,const STDSTR &desp1,const STDSTR &desp2,const STDSTR &desp3,const STDSTR &desp4){
	STDSTR		space;
	space = "";
	if (command.length() < 17)
		space.insert(space.length(),17 - command.length(),' ');
	
	PrintALine(env,Data(COL_clBlue,"    "),command,space,Data(COL_clDefault,&desp1),desp2,desp3,desp4);
}
//------------------------------------------------------------------------------------------//
int32 BIC_Tools::BI_SetConnectPar(ExpandDeviceAttr *eda,const STDSTR &par,CSType tDevType){
	STDSTR	strPar1,strPar2;
	
	if (eda->device->IsConnected() == 0){
		eda->csType = tDevType;
		if(par.length() > 0){
			SplitPar1(strPar1,strPar2,par);
			switch(tDevType){
				case CSType_TCP:
					eda->aTCP.name = strPar1;
					BI_SetConnectPar2(eda,strPar2);
					break;
				case CSType_UDP:
					eda->aUDP.name = strPar1;
					BI_SetConnectPar2(eda,strPar2);
					break;
				case CSType_TCPS:
				case CSType_UDPS:
					BI_SetConnectPar2(eda,par);
					break;
				case CSType_COM:
				case CSType_COMV:
				default:
					eda->aCOM.name = strPar1;
					BI_SetConnectPar2(eda,strPar2);
					break;
			}
		}
		return 1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
int32 BIC_Tools::BI_SetConnectPar2(ExpandDeviceAttr *eda,const STDSTR &par){
	int32 baud;
	
	switch(eda->csType){
		case CSType_TCP: baud = eda->aTCP.port;break;
		case CSType_UDP: baud = eda->aUDP.port;break;
		case CSType_TCPS:baud = eda->aTCPS.port;break;
		case CSType_UDPS:baud = eda->aUDPS.port;break;
		case CSType_COM:;
		case CSType_COMV:;
		default:baud = eda->aCOM.port;break;
	}
	
	if (par.length() > 0){
		baud = atoi(par.c_str());
		if (baud < 0)
			baud = 0;
	}
	if (eda->device->IsConnected() == 0){
		switch(eda->csType){
			case CSType_TCP: eda->aTCP.port = baud;break;
			case CSType_UDP: eda->aUDP.port = baud;break;
			case CSType_TCPS:eda->aTCPS.port = baud;break;
			case CSType_UDPS:eda->aUDPS.port = baud;break;
			case CSType_COM:;
			case CSType_COMV:;
			default:eda->aCOM.port = baud;break;
		}
		return 1;
	}
	else if (eda->IsCom()){
		eda->ACom()->SetBaudrate(baud);
		return 1;
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
int32 BIC_Tools::OnlineMode(BIC_ENV *env,COMMU_DBUF_FRAME *cmmu,CMD_TAIL tail)const{
	uint8	chKey;
	STDSTR	sendData,recData,strTail;
	uint32	ret;
	
	ret = 0;
	strTail = "";
	switch (tail) {
		case CMD_R:	strTail = '\r';break;
		case CMD_N:	strTail = '\n';break;
		case CMD_RN:strTail = "\r\n";break;
		default:
			break;
	}
	//define ctrl^A~Z is 1~26,HEX is 01~1A. Escape ctrl^~ to 128
	
	PrintALine(env,Data(COL_clCyan,"In online mode:"));
	env->blInOnlineMode = 1;
	env->blUseSecondLH = 1;
	env->cstdout->PrintEnable();
	sendData = "";
	
	while((env->blExit == 0) && (cmmu->IsConnected() != 0) && (OnlineModeExit(env) == 0)){
		recData = "";
		if (cmmu->Read(&recData,G_ESCAPE_OFF).length() > 0)
			PrintStr(env, Data(COL_DB_RxText,&recData));
		
		SYS_SleepMS(10);
		chKey = BI_ReadChar(env,0);
		if ((chKey >= 32) && (chKey <= 126)){
			sendData += chKey;
		}
		else if (chKey == '\n'){
			env->cstdout->PrintEnable();
			PrintStr(env,"\n");
			sendData += strTail;
			cmmu->Send(sendData,G_ESCAPE_OFF);
			sendData = "";
		}
		else if (chKey == 27){
			ret = 1;
			break;
		}
		else{
			if (chKey == 0x80)
				chKey = 27;
			if ((chKey > 0) && (chKey <= 31))
				cmmu->Send(chKey);
		}
	}
	env->cstdout->PrintEnable();
	PrintStr(env,"\n");
	env->blInOnlineMode = 0;
	env->blUseSecondLH = 0;
	return(ret);
}
//------------------------------------------------------------------------------------------//
int32 BIC_Tools::InPressKeyMode(BIC_ENV *env)const{
	uint8	chKey;
	uint32	ret;
	env->blInPressKeyMode = 1;
	env->cstdout->PrintEnable();
	ret = 0;
	while((env->blExit == 0) && (InPressKeyModeExit(env) == 0)){
		SYS_SleepMS(10);
		chKey = BI_ReadChar(env,0);
		if (chKey == '\n'){
			env->cstdout->PrintEnable();
		}
		else if (chKey == 27){
			ret = 1;
			break;
		}
	}
	env->blInPressKeyMode = 0;
	return(ret);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
int32 BIC_HELP::Command(BIC_ENV *env,const STDSTR &par,void *p)const{
	int32		detail;
	BIC_Node	*father;
	
	detail = 0;
	
	if (par == "-d")
		detail = 3;
	
	PrintALine(env,"Build in command explain:");
	father = static_cast<BIC_Node*>(GetcgUp(const_cast<BIC_HELP *>(this)));
	
	if (father != nullptr)
		father->HelpTraversalChild(env,detail,0);
	
	return(cgReturnCode);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
