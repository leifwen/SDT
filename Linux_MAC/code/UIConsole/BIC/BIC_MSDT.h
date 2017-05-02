/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: BIC_MSDT.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
*/

//------------------------------------------------------------------------------------------//
#include "BIC_B.h"
#include "License_Checker.h"
#include "License_SSLSocket.h"
//------------------------------------------------------------------------------------------//
#if defined BIC_BH && defined License_CheckerH && defined License_SSLSocketH
#ifndef BIC_MSDTH
#define BIC_MSDTH
#ifdef BIC_MSDTH
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_MAC
#define BIC_MSDT_ENABLE
//------------------------------------------------------------------------------------------//
class BIC_PATCHSDT : public BIC_Node{
	public:
				 BIC_PATCHSDT(void) : BIC_Node() {cgCommand = "patch";cgReturnCode = BI_RETCODE_PATCHSDT;};
		virtual ~BIC_PATCHSDT(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Add Patch Code to SDT");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<filename>"		,"SDT file name.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_APPROVE : public BIC_Node{
	public:
				 BIC_APPROVE(void) : BIC_Node() {cgCommand = "approve";cgReturnCode = BI_RETCODE_APPROVE;};
		virtual ~BIC_APPROVE(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Approve registration file");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<hours>"	,"720H = 1M,8760H = 1Y,867240H = 99Y");
			PrintHelpSubItem(env,"<filename>"		,"PreRegistration.key");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_LSERVER : public BIC_Node{
	public:
				 BIC_LSERVER(void) : BIC_Node() {cgCommand = "lserver";cgReturnCode = BI_RETCODE_LSERVER;};
		virtual ~BIC_LSERVER(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Setup License Server");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"[port]"		,"Server port, default is 16385.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_MSDT : public BIC_Node_S{
	public:
				 BIC_MSDT(void) : BIC_Node_S() {cgCommand = "msdt";cgTitle = cgCommand;cgReturnCode = BI_RETCODE_MSDT;Init();};
		virtual ~BIC_MSDT(void){;};
	public:
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			if (blDetail == 1){
				HelpTraversalChild(env,0);
				return(cgReturnCode);
			}
			PrintHelpItem(env, cgCommand, "-> SDT manage.");
			if (blDetail == 0)
				return(cgReturnCode);
			return(cgReturnCode);
		};
	private:
		void	Init(void){Add(cgC_PATCHSDT) < cgC_APPROVE < cgC_LSERVER;};
	private:
		BIC_PATCHSDT	cgC_PATCHSDT;
		BIC_APPROVE		cgC_APPROVE;
		BIC_LSERVER		cgC_LSERVER;
};
//------------------------------------------------------------------------------------------//
#endif//CommonDefH_MAC
//------------------------------------------------------------------------------------------//
#if defined CommonDefH_EnableSMC && defined CommonDefH_VC
#define BIC_CREGH
//------------------------------------------------------------------------------------------//
class BIC_CREGREPORT : public BIC_Node{
	public:
				 BIC_CREGREPORT(void) : BIC_Node() {cgCommand = "creg";cgReturnCode = BI_RETCODE_CREGREPORT;};
		virtual ~BIC_CREGREPORT(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Create \"PreRegistration.key\" file.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"[IP]"		,"Server address.");
			PrintHelpSubItem(env,"[PORT]"	,"Server port, default is 16385.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
#endif//CommonDefH_EnableSMC
//------------------------------------------------------------------------------------------//
#endif//BIC_MSDTH
#endif//BIC_MSDTH
#endif//defined BIC_BH && defined License_CheckerH

