//
//  SYS_File.h
//  SDT
//
//  Created by Leif Wen on 31/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#ifndef SYS_File_h
#define SYS_File_h
//------------------------------------------------------------------------------------------//
#include "BasicDefine.h"
#include <fstream>
//------------------------------------------------------------------------------------------//
bool32	CFS_CheckFile		(const STDSTR& fName);
uint64	CFS_CheckFileSize	(const STDSTR& fName);
uint64	CFS_ReadFile		(STDSTR* retStr,const STDSTR &fName);
void	CFS_WriteFile		(const STDSTR& fName,const STDSTR& strContent);
void	CFS_AddToFile		(const STDSTR& fName,const STDSTR& strContent);
//------------------------------------------------------------------------------------------//
#endif /* SYS_File_h */
