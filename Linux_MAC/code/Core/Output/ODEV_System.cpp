/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: ODEV_System.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.7.5
 *			: 2014.1.13 remove support BCB2012
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "ODEV_System.h"
//------------------------------------------------------------------------------------------//
#ifdef ODEV_SystemH
//------------------------------------------------------------------------------------------//
void ODEV_CACHE::Init(void){
	defSTDOUT = nullptr;
	defFile = nullptr;
}
//------------------------------------------------------------------------------------------//
ODEV_STDOUT *ODEV_CACHE::CreateG1_STDOUT(OUTPUT_NODE::COLType tColType){
	if (defSTDOUT == nullptr)
		defSTDOUT = new ODEV_STDOUT(tColType,this,CRD_G1);
	return(defSTDOUT);
}
//------------------------------------------------------------------------------------------//
ODEV_STDOUT *ODEV_CACHE::AddG1_STDOUT(ODEV_STDOUT *oG1D){
	ODEV_STDOUT *ret;
	ret = nullptr;
	Spin_InUse_set();
	if (defSTDOUT == nullptr){
		Register(oG1D, CRD_G1);
		ret = oG1D;
		defSTDOUT = oG1D;
		SetSFlag(blAdd_defSTDOUT);
	}
	Spin_InUse_clr();
	return(ret);
}
//------------------------------------------------------------------------------------------//
ODEV_FILE *ODEV_CACHE::CreateG2_FILE(const STDSTR &fName,uint64 tColType){
	if (defFile == nullptr){
		defFile = new ODEV_FILE(this,CRD_G2);
		if (defFile != nullptr)
			defFile->Init(fName,tColType);
	}
	else{
		defFile->Init(fName,tColType);
	}
	return(defFile);
}
//------------------------------------------------------------------------------------------//
void ODEV_CACHE::UnregisterAll(void){
	if (CheckSFlag(blAdd_defSTDOUT))
		defSTDOUT->RemoveSelf();
}
//------------------------------------------------------------------------------------------//
void ODEV_CACHE::Unregister(OUTPUT_NODE *oG1D){
	Spin_InUse_set();
	if (defSTDOUT == oG1D){
		defSTDOUT = nullptr;
		ClrSFlag(blAdd_defSTDOUT);
	}
	Spin_InUse_clr();
	OUTPUT_CACHE::Unregister(oG1D);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
void ODEV_VG3D::Init(void){
	defODEV = nullptr;
	defSTDOUT = nullptr;
	defFile = nullptr;
};
//------------------------------------------------------------------------------------------//
void ODEV_VG3D::UnregisterAll(void){
	if (CheckSFlag(blAdd_defODEV))
		defODEV->RemoveSelf();
	if (CheckSFlag(blAdd_defSTDOUT))
		defSTDOUT->RemoveSelf();
	VG3D_POOL::UnregisterAll();
}
//------------------------------------------------------------------------------------------//
void ODEV_VG3D::Unregister(OUTPUT_NODE *oG3D){
	Spin_InUse_set();
	if (defODEV == oG3D){
		defODEV = nullptr;
		ClrSFlag(blAdd_defODEV);
	}
	else if (defSTDOUT == oG3D){
		defSTDOUT = nullptr;
		ClrSFlag(blAdd_defSTDOUT);
	}
	Spin_InUse_clr();
	VG3D_POOL::Unregister(oG3D);
};
//------------------------------------------------------------------------------------------//
OUTPUT_NODE *ODEV_VG3D::AddG3D_ODEV(OUTPUT_NODE *oG3D,int32 blManage){
	OUTPUT_NODE *ret;
	ret = nullptr;
	Spin_InUse_set();
	if (defODEV == nullptr){
		AddG3D(oG3D,OUTPUT_CACHE::OC_G3D1,G_LOCK_OFF);
		ret = oG3D;
		defODEV = oG3D;
		if (blManage != 0)
			SetSFlag(blAdd_defODEV);
	}
	Spin_InUse_clr();
	return(ret);
};
//------------------------------------------------------------------------------------------//
ODEV_STDOUT *ODEV_VG3D::AddG3D_STDOUT(ODEV_STDOUT *oG3D,int32 blManage){
	ODEV_STDOUT *ret;
	ret = nullptr;
	Spin_InUse_set();
	if (defSTDOUT == nullptr){
		AddG3D(oG3D,OUTPUT_CACHE::OC_G3D2,G_LOCK_OFF);
		ret = oG3D;
		defSTDOUT = oG3D;
		if (blManage != 0)
			SetSFlag(blAdd_defSTDOUT);
	}
	Spin_InUse_clr();
	return(ret);
};
//------------------------------------------------------------------------------------------//
ODEV_FILE *ODEV_VG3D::CreateG3D_FILE(const STDSTR &fName,uint64 tColType){
	Spin_InUse_set();
	if (defFile == nullptr){
		defFile = new ODEV_FILE();
		if (defFile != nullptr)
			defFile->Init(fName,tColType);
		AddG3D(defFile,OUTPUT_CACHE::OC_G3D3,G_LOCK_OFF);
	}
	else{
		defFile->Init(fName,tColType);
	}
	Spin_InUse_clr();
	return(defFile);
}
//------------------------------------------------------------------------------------------//
#endif
