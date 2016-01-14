/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: ODEV_LIST.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.7.5
 *			: 2014.1.13 remove support BCB2012
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "ODEV_LIST.h"
#include "ODEV_FILE.h"
#include "ODEV_SDOUT.h"
#include "Commu_DBuf.h"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#ifdef LOGPRINT
	char Logbuffer[1024 * 1024];
	SYS_DateTime	logTime;
	SYS_Lock		logLock;
#endif
//------------------------------------------------------------------------------------------//
void MyLogPrint(const RTREE_NODE *treeNode1,const RTREE_NODE *treeNode2,const char *format, ...){
#ifdef LOGPRINT
	logLock.Lock();
	va_list arg_ptr;
	va_start(arg_ptr, format);
#ifdef CommonDefH_Unix
	vsnprintf(Logbuffer,sizeof(Logbuffer),format, arg_ptr);
#endif
#ifdef CommonDefH_VC
	vsnprintf_s(Logbuffer, sizeof(Logbuffer), format, arg_ptr);
#endif
	va_end(arg_ptr);
	logTime.Now();
	if (treeNode1 != nullptr){
		if (treeNode2 != nullptr){
			printf("%s  [%s][%s]::%s\n",logTime.FormatDateTime("[hh:mm:ss.zzz]").c_str()
				   ,treeNode1->selfName.c_str(),treeNode2->selfName.c_str()
				   ,Logbuffer);
		}
		else{
			printf("%s  [%s]::%s\n",logTime.FormatDateTime("[hh:mm:ss.zzz]").c_str(),treeNode1->selfName.c_str(),Logbuffer);
		}
	}
	else{
		if (treeNode2 != nullptr){
			printf("%s  [%s]::%s\n",logTime.FormatDateTime("[hh:mm:ss.zzz]").c_str(),treeNode2->selfName.c_str(),Logbuffer);
		}
		else{
			printf("%s  %s\n",logTime.FormatDateTime("[hh:mm:ss.zzz]").c_str(),Logbuffer);
		}
	}
	logLock.Unlock();
#endif
}
//------------------------------------------------------------------------------------------//
void MyLogPrint(const RTREE_NODE &treeNode1,const RTREE_NODE &treeNode2,const char *format, ...){
#ifdef LOGPRINT
	logLock.Lock();
	va_list arg_ptr;
	va_start(arg_ptr, format);
#ifdef CommonDefH_Unix
	vsnprintf(Logbuffer, sizeof(Logbuffer), format, arg_ptr);
#endif
#ifdef CommonDefH_VC
	vsnprintf_s(Logbuffer, sizeof(Logbuffer), format, arg_ptr);
#endif
	va_end(arg_ptr);
	logTime.Now();
	printf("%s  [%s][%s]::%s\n",logTime.FormatDateTime("[hh:mm:ss.zzz]").c_str()
		   ,treeNode1.selfName.c_str(),treeNode2.selfName.c_str()
		   ,Logbuffer);
	logLock.Unlock();
#endif
}
//------------------------------------------------------------------------------------------//
void MyLogPrint(const RTREE_NODE *treeNode,const char *format, ...){
#ifdef LOGPRINT
	logLock.Lock();
	va_list arg_ptr;
	va_start(arg_ptr, format);
#ifdef CommonDefH_Unix
	vsnprintf(Logbuffer,sizeof(Logbuffer),format, arg_ptr);
#endif
#ifdef CommonDefH_VC
	vsnprintf_s(Logbuffer, sizeof(Logbuffer), format, arg_ptr);
#endif
	va_end(arg_ptr);
	logTime.Now();
	if (treeNode != nullptr){
		printf("%s  [%s]::%s\n",logTime.FormatDateTime("[hh:mm:ss.zzz]").c_str(),treeNode->selfName.c_str(),Logbuffer);
	}
	else{
		printf("%s  %s\n",logTime.FormatDateTime("[hh:mm:ss.zzz]").c_str(),Logbuffer);
	}
	logLock.Unlock();
#endif
}
//------------------------------------------------------------------------------------------//
void MyLogPrint(const RTREE_NODE &treeNode,const char *format, ...){
#ifdef LOGPRINT
	logLock.Lock();
	va_list arg_ptr;
	va_start(arg_ptr, format);
#ifdef CommonDefH_Unix
	vsnprintf(Logbuffer, sizeof(Logbuffer), format, arg_ptr);
#endif
#ifdef CommonDefH_VC
	vsnprintf_s(Logbuffer, sizeof(Logbuffer), format, arg_ptr);
#endif
	va_end(arg_ptr);
	logTime.Now();
	printf("%s  [%s]::%s\n",logTime.FormatDateTime("[hh:mm:ss.zzz]").c_str(),treeNode.selfName.c_str(),Logbuffer);
	logLock.Unlock();
#endif
}
//------------------------------------------------------------------------------------------//
void MyLogPrint(const char *format, ...){
#ifdef LOGPRINT
	logLock.Lock();
	va_list arg_ptr;
	va_start(arg_ptr, format);
#ifdef CommonDefH_Unix
	vsnprintf(Logbuffer, sizeof(Logbuffer), format, arg_ptr);
#endif
#ifdef CommonDefH_VC
	vsnprintf_s(Logbuffer, sizeof(Logbuffer), format, arg_ptr);
#endif
	va_end(arg_ptr);
	logTime.Now();
	printf("%s  %s\n",logTime.FormatDateTime("[hh:mm:ss.zzz]").c_str(),Logbuffer);
	logLock.Unlock();
#endif
}
//------------------------------------------------------------------------------------------//
ODEV_LIST_POOL::ODEV_LIST_POOL(void) : RTREE_NODE(){
	cODevSDOUT = nullptr;
	cODevFileTXT = nullptr;
	cODevFileRTF = nullptr;
	CreateTrash(this);
}
//------------------------------------------------------------------------------------------//
int32 ODEV_LIST_POOL::Print(void){
	int32		blPrint;
	ODEV_NODE	*nextNode;
	
	blPrint = 0;
	nextNode = (ODEV_NODE*)GetFirstChild(this);

	while (nextNode != nullptr){
		#ifdef CommonDefH_VC
		if ((nextNode != cODevSDOUT) && (nextNode->Print() != 0))
			blPrint = 1;
		#endif
		#ifdef CommonDefH_Unix
		if (nextNode->Print() != 0)
			blPrint = 1;
		#endif
		nextNode = (ODEV_NODE*)GetNextBrother(nextNode);
	}
	return(blPrint);
}
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_VC
void ODEV_LIST_POOL::PrintSDOUT(void){
	if (cODevSDOUT != nullptr)
		cODevSDOUT->Print();
};
#endif
//------------------------------------------------------------------------------------------//
ODEV_NODE_FILE *ODEV_LIST_POOL::CreateNewODevFile(const std::string &tODEV,COLSTRING::COLType tCOLType){
	ODEV_NODE_FILE	*tNewODev;
	
	tNewODev = new ODEV_NODE_FILE(tODEV,tCOLType);
	if (tNewODev == nullptr)
		return(nullptr);
	AddNode(tNewODev);
	return(tNewODev);
}
//------------------------------------------------------------------------------------------//
ODEV_NODE_SDOUT *ODEV_LIST_POOL::CreateNewODevSDOUT(const void *tRichEdit,const void *tCFrm,COLSTRING::COLType tCOLType){
	ODEV_NODE_SDOUT	*tNewODev;
	
	tNewODev = new ODEV_NODE_SDOUT(tRichEdit,tCFrm,tCOLType);
	if (tNewODev == nullptr)
		return(nullptr);
	AddNode(tNewODev);
	return(tNewODev);
}
//------------------------------------------------------------------------------------------//
ODEV_NODE_FILE *ODEV_LIST_POOL::CreateODevFileTXT(const std::string &tODEV){
	if (cODevFileTXT == nullptr)
		cODevFileTXT = CreateNewODevFile(tODEV,COLSTRING::COLType_TXT);
	if (cODevFileTXT != nullptr)
		cODevFileTXT->Init(tODEV,COLSTRING::COLType_TXT);
	return(cODevFileTXT);
}
//------------------------------------------------------------------------------------------//
ODEV_NODE_FILE *ODEV_LIST_POOL::CreateODevFileRTF(const std::string &tODEV){
	if (cODevFileRTF == nullptr)
		cODevFileRTF = CreateNewODevFile(tODEV,COLSTRING::COLType_COL);
	if (cODevFileRTF != nullptr)
		cODevFileRTF->Init(tODEV,COLSTRING::COLType_COL);
	return(cODevFileRTF);
}
//------------------------------------------------------------------------------------------//
void ODEV_LIST_POOL::CreateODevFile(const std::string &tODEV){
	CreateODevFileRTF(tODEV);
	CreateODevFileTXT(tODEV);
}
//------------------------------------------------------------------------------------------//
ODEV_NODE_SDOUT *ODEV_LIST_POOL::CreateODevSDOUT(const void *tRichEdit,const void *tCFrm,COLSTRING::COLType tCOLType){
	if (cODevSDOUT == nullptr)
		cODevSDOUT = CreateNewODevSDOUT(tRichEdit,tCFrm,tCOLType);
	if (cODevSDOUT != nullptr)
		cODevSDOUT->Init(tRichEdit,tCFrm,tCOLType);
	return(cODevSDOUT);
}
//------------------------------------------------------------------------------------------//