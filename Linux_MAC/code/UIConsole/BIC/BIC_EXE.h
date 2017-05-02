//
//  BIC_EXE.h
//  SDT
//
//  Created by Leif Wen on 04/04/2017.
//  Copyright © 2017 leif wen. All rights reserved.
//
//------------------------------------------------------------------------------------------//
#include "BIC_B.h"
#include "Commu_EXE.h"
//------------------------------------------------------------------------------------------//
#if defined Commu_EXE_hpp && defined BIC_BH
#ifndef BIC_EXE_hpp
#define BIC_EXE_hpp
#ifdef BIC_EXE_hpp
//------------------------------------------------------------------------------------------//
class BIC_EXEC : public BIC_Node{
	public:
				 BIC_EXEC(void) : BIC_Node() {cgCommand = "ex/execute";cgReturnCode = BI_RETCODE_EXECUTE;};
		virtual ~BIC_EXEC(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Execute console command.");
			return(cgReturnCode);
		};
	public:
		virtual	AEXE *ExecuteCMD(BIC_ENV *env,const STDSTR &cmd)const;
};
//------------------------------------------------------------------------------------------//
class BIC_EXE_BASH : public BIC_EXEC{
	public:
				 BIC_EXE_BASH(void) : BIC_EXEC() {cgCommand = "bash";cgReturnCode = BI_RETCODE_EXECUTE_BASH;};
		virtual ~BIC_EXE_BASH(void){;};
	public:
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Execute console command with using bash(default).");
			return(cgReturnCode);
		};
	public:
		virtual	AEXE *ExecuteCMD(BIC_ENV *env,const STDSTR &cmd)const;
};
//------------------------------------------------------------------------------------------//
class BIC_EXE_SH : public BIC_EXEC{
	public:
				 BIC_EXE_SH(void) : BIC_EXEC() {cgCommand = "exsh";cgReturnCode = BI_RETCODE_EXECUTE_SH;};
		virtual ~BIC_EXE_SH(void){;};
	public:
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Execute console command with using sh.");
			return(cgReturnCode);
		};
	public:
		virtual	AEXE *ExecuteCMD(BIC_ENV *env,const STDSTR &cmd)const;
};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif /* BIC_EXE_hpp */
