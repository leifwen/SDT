//
//  BIC_EXE.h
//  SDT
//
//  Created by Leif Wen on 16/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "BIC_Commu.h"
#include "Commu_AEXE.h"
#if defined BIC_Commu_h && defined Commu_AEXE_h
//------------------------------------------------------------------------------------------//
#ifndef BIC_EXE_h
#define BIC_EXE_h
#ifdef BIC_EXE_h
//------------------------------------------------------------------------------------------//
class BIC_EXEC : public BIC_COMMU{
	public:
				 BIC_EXEC(void) : BIC_COMMU() {cgCommandID = BIC_ID_EXECUTE;cgCommand = "ex/execute";};
		virtual ~BIC_EXEC(void){;};
	public:
		virtual	CMDID	Command				(CMD_ENV* env,const STDSTR& msg,void* p)const;
		virtual	CMDID	Help				(CMD_ENV* env,uint32 flag)const;
	private:
		virtual	AEXE*	ExecuteCMD			(CMD_ENV* env,const STDSTR& cmd)const;
};
//------------------------------------------------------------------------------------------//
class BIC_EXE_BASH : public BIC_EXEC{
	public:
				 BIC_EXE_BASH(void) : BIC_EXEC() {cgCommandID = BIC_ID_EXECUTE_BASH;cgCommand = "bash";};
		virtual ~BIC_EXE_BASH(void){;};
	public:
		virtual	CMDID	Help				(CMD_ENV* env,uint32 flag)const;
	private:
		virtual	AEXE*	ExecuteCMD			(CMD_ENV* env,const STDSTR& cmd)const;
};
//------------------------------------------------------------------------------------------//
class BIC_EXE_SH : public BIC_EXEC{
	public:
				 BIC_EXE_SH(void) : BIC_EXEC() {cgCommandID = BIC_ID_EXECUTE_SH;cgCommand = "exsh";};
		virtual ~BIC_EXE_SH(void){;};
	public:
		virtual	CMDID	Help				(CMD_ENV* env,uint32 flag)const;
	private:
		virtual	AEXE*	ExecuteCMD			(CMD_ENV* env,const STDSTR& cmd)const;
};
//------------------------------------------------------------------------------------------//
#endif /* BIC_EXE_h */
#endif /* BIC_EXE_h */
#endif /* BIC_Dev_h */
