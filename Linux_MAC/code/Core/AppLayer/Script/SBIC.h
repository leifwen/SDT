/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: SBIC.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.08.12
*/
#ifndef SBICH
#define SBICH
//------------------------------------------------------------------------------------------//
#include "SBIC_B.h"
#include "SBIC_Build.h"
#include "SBIC_Search.h"
#include "SBIC_Condition.h"
#include "SBIC_Expression.h"
#include "SBIC_RunExternal.h"
//------------------------------------------------------------------------------------------//
int32 SBIC_CreateHexCommand(SBICPAR *cSBICPAR,const std::string &inputCommand,int32 blHEX,int32 bl0x0D,G_ESCAPE_VAILD blEscape,std::string *retHexCommand,std::string *retPrintData);
//------------------------------------------------------------------------------------------//
#endif