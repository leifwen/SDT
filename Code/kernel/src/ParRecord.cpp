//
//  ParRecord.cpp
//  SDT
//
//  Created by Leif Wen on 23/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "ParRecord.h"
//------------------------------------------------------------------------------------------//
#ifdef ParRecord_h
#include "DS_STRING.h"
#include "SYS_File.h"
//------------------------------------------------------------------------------------------//
PR_Record::PR_Record(void){
	cgIPComList = nullptr;
	cgSCList = nullptr;
	cgGCList = nullptr;
}
//------------------------------------------------------------------------------------------//
void PR_Record::Init(IPCOMLIST *iplist,SC_LIST *sclist,GC_LIST *gclist){
	cgIPComList = iplist;
	cgSCList = sclist;
	cgGCList = gclist;
}
//------------------------------------------------------------------------------------------//
void PR_Record::Load(const STDSTR& fileName,uint32 cfg){
	STDSTR	strResult,strLine;
	uint32	ver;
	
	if (CFS_CheckFile(fileName)){
		if (cfg & READINITYPE_CoverDeviceList)
			cgIPComList->Empty();
		if (cfg & READINITYPE_CoverSingleCommand)
			cgSCList->Empty();
		if (cfg & READINITYPE_CoverGroupCommand)
			cgGCList->Empty();

		CFS_ReadFile(&strResult,fileName);
		
		strLine = Str_LowerCase(Str_Trim(Str_ReadSubItem(&strResult,"\n")));
		
		ver = 6;
		if (strLine == "[v0.1]"){
			ver = 1;
		}
		else if (strLine == "[v0.2]"){
			ver = 2;
		}
		else if (strLine == "[v0.3]"){
			ver = 3;
		}
		else if (strLine == "[v0.4]"){
			ver = 4;
		}
		else if (strLine == "[v0.5]"){
			ver = 5;
		}

		switch (ver) {
			case 0:;
			case 1:;
			case 2:;
			case 3:;
				break;
			case 4:;
			default:
				ImportV0_4(ver,&strResult,cfg);
				break;
		}
		return;
	}
}
//------------------------------------------------------------------------------------------//
void PR_Record::Save(const STDSTR& fileName,uint32 cfg){
	STDSTR	strResult = "[V0.6]\n";

	CFS_WriteFile(fileName,ExportV0_4(6,&strResult,cfg));
}
//------------------------------------------------------------------------------------------//
void PR_Record::ImportV0_4(uint32 ver,STDSTR* strIn,uint32 cfg){
	STDSTR		strLine;
	
	while(strIn->length() > 0){
		strLine = Str_LowerCase(Str_Trim(Str_ReadSubItem(strIn,"\n")));
		if (strLine == "[ipcomlist]"){
			if (cfg & READINITYPE_AddDeviceList)
				cgIPComList->Import(ver,strIn);
		}
		else if (strLine == "[singlecommand]"){
			if (cfg & READINITYPE_AddSingleCommand)
				cgSCList->Import(ver,strIn);
		}
		else if (strLine == "[grouplist]"){
			if (cfg & READINITYPE_AddGroupCommand)
				cgGCList->Import(ver,strIn);
		}
	}
}
//------------------------------------------------------------------------------------------//
STDSTR& PR_Record::ExportV0_4(uint32 ver,STDSTR* strOut,uint32 cfg){
	if (cfg & READINITYPE_AddDeviceList)
		cgIPComList->Export(ver,strOut);
	if (cfg & READINITYPE_AddSingleCommand)
		cgSCList->Export(ver,strOut);
	if (cfg & READINITYPE_AddGroupCommand)
		cgGCList->Export(ver,strOut);
	return(*strOut);
}
//------------------------------------------------------------------------------------------//
#endif /* ParRecord_h */
