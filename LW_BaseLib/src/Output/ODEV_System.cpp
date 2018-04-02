//
//  ODEV_System.cpp
//  SDT
//
//  Created by Leif Wen on 31/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "ODEV_System.h"
//------------------------------------------------------------------------------------------//
#ifdef ODEV_System_h
//------------------------------------------------------------------------------------------//
ODEV_CACHE::ODEV_CACHE(uint32 size) : OUTPUT_CACHE(size){
	defSTDOUT = nullptr;
	defFile = nullptr;
	SetSelfName("ODEV_VG3D");
};
//------------------------------------------------------------------------------------------//
ODEV_STDOUT* ODEV_CACHE::CreateG1_STDOUT(OUTPUT_NODE::COLType colType){
	if (defSTDOUT == nullptr){
		defSTDOUT = new ODEV_STDOUT(colType,this,CRD_G1);
		defSTDOUT->SetDestoryByCache();
	}
	return(defSTDOUT);
}
//------------------------------------------------------------------------------------------//
ODEV_STDOUT* ODEV_CACHE::AddG1_STDOUT(ODEV_STDOUT* oG1D){
	ODEV_STDOUT *ret;
	ret = nullptr;
	InUse_set();
	if (defSTDOUT == nullptr){
		Register(oG1D, CRD_G1);
		ret = oG1D;
		defSTDOUT = oG1D;
	}
	InUse_clr();
	return(ret);
}
//------------------------------------------------------------------------------------------//
ODEV_FILE* ODEV_CACHE::CreateG2_FILE(const STDSTR& fName,uint64 colType){
	if (defFile == nullptr){
		defFile = new ODEV_FILE(this,CRD_G2);
		if (defFile != nullptr)
			defFile->Init(fName,colType);
		defFile->SetDestoryByCache();
	}
	else{
		defFile->Init(fName,colType);
	}
	return(defFile);
}
//------------------------------------------------------------------------------------------//
void ODEV_CACHE::Unregister(OUTPUT_NODE* oG1D){
	InUse_set();
	if (defSTDOUT == oG1D){
		defSTDOUT = nullptr;
	}
	else if (defFile == oG1D){
		defFile = nullptr;
	}

	InUse_clr();
	OUTPUT_CACHE::Unregister(oG1D);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
ODEV_VG3D::ODEV_VG3D(OUTPUT_CACHE* cache) : VG3D_POOL(cache){
	defODEV = nullptr;
	defSTDOUT = nullptr;
	defFile = nullptr;
	SetSelfName("ODEV_VG3D");
};
//------------------------------------------------------------------------------------------//
void ODEV_VG3D::UnregisterChild(OUTPUT_NODE* oG3D){
	InUse_set();
	if (defODEV == oG3D){
		defODEV = nullptr;
	}
	else if (defSTDOUT == oG3D){
		defSTDOUT = nullptr;
	}
	else if (defFile == oG3D){
		defFile = nullptr;
	}
	InUse_clr();
	VG3D_POOL::UnregisterChild(oG3D);
};
//------------------------------------------------------------------------------------------//
OUTPUT_NODE* ODEV_VG3D::AddG3D_ODEV(OUTPUT_NODE* oG3D){
	OUTPUT_NODE *ret;
	ret = nullptr;
	InUse_set();
	if (defODEV == nullptr){
		AddG3D(oG3D,G_LOCK_OFF);
		ret = oG3D;
		defODEV = oG3D;
	}
	InUse_clr();
	return(ret);
};
//------------------------------------------------------------------------------------------//
ODEV_STDOUT* ODEV_VG3D::AddG3D_STDOUT(ODEV_STDOUT* oG3D){
	ODEV_STDOUT *ret;
	ret = nullptr;
	InUse_set();
	if (defSTDOUT == nullptr){
		AddG3D(oG3D,G_LOCK_OFF);
		ret = oG3D;
		defSTDOUT = oG3D;
	}
	InUse_clr();
	return(ret);
};
//------------------------------------------------------------------------------------------//
ODEV_FILE* ODEV_VG3D::CreateG3D_FILE(const STDSTR& fName,uint64 colType){
	InUse_set();
	if (defFile == nullptr){
		defFile = new ODEV_FILE();
		if (defFile != nullptr)
			defFile->Init(fName,colType);
		AddG3D(defFile,G_LOCK_OFF);
		defFile->SetDestoryByCache();
	}
	else{
		defFile->Init(fName,colType);
	}
	InUse_clr();
	return(defFile);
}
//------------------------------------------------------------------------------------------//
#endif /* ODEV_System_h */
