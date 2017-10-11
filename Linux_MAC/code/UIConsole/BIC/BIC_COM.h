/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: BIC_COM.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
*/
//------------------------------------------------------------------------------------------//
#include "BIC_B.h"
//------------------------------------------------------------------------------------------//
#ifdef BIC_BH
#ifndef BIC_COMH
#define BIC_COMH
#ifdef BIC_COMH
//------------------------------------------------------------------------------------------//
class BIC_COM : public BIC_Node{
	public:
				 BIC_COM(void) : BIC_Node() {cgCommand = "com";cgReturnCode = BI_RETCODE_COM;};
		virtual ~BIC_COM(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *eda)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Set as",Str_LowerCase(selfName),"COM mode.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"[name][br]",selfName,"COM name and baudrate.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_COM_BR : public BIC_Node{
	public:
				 BIC_COM_BR(void) : BIC_Node() {cgCommand = "br";cgReturnCode = BI_RETCODE_COM_BR;};
		virtual ~BIC_COM_BR(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *eda)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Set", Str_LowerCase(selfName),"COM baudrate.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<n>","Baudrate.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_COM_DTR : public BIC_Node{
	public:
			     BIC_COM_DTR(void) : BIC_Node() {cgCommand = "dtr";cgReturnCode = BI_RETCODE_COM_DTR;};
	    virtual ~BIC_COM_DTR(void){;};
	public:
	    virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *eda)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Set",Str_LowerCase(selfName),"COM DTR.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"[-H[t]]","Set to High, t is time, unit is S.");
			PrintHelpSubItem(env,"[-L[t]]","Set to Low , t is time, unit is S.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_COM_RTS : public BIC_Node{
	public:
		    	 BIC_COM_RTS(void) : BIC_Node() {cgCommand = "rts";cgReturnCode = BI_RETCODE_COM_RTS;};
	    virtual ~BIC_COM_RTS(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *eda)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Set",Str_LowerCase(selfName),"COM RTS.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"[-H[t]]","Set to High, t is time, unit is S.");
			PrintHelpSubItem(env,"[-L[t]]","Set to Low , t is time, unit is S.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_COM_MS : public BIC_Node{
	public:
			     BIC_COM_MS(void) : BIC_Node() {cgCommand = "ms";cgReturnCode = BI_RETCODE_COM_MS;};
	    virtual ~BIC_COM_MS(void){;};
	public:
	    virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *eda)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Modem status report");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"[on|off]"	,"Enable/disable",Str_LowerCase(selfName),"modem status report.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_COM_LS : public BIC_Node{
	public:
			     BIC_COM_LS(void) : BIC_Node() {cgCommand = "lscom";cgReturnCode = BI_RETCODE_COM_LS;};
	    virtual ~BIC_COM_LS(void){;};
	public:
	    virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"List valid COM.");
			if (blDetail == 0)
				return(cgReturnCode);
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_COM_OPEN : public BIC_COM{
	public:
				 BIC_COM_OPEN(void) : BIC_COM() {cgCommand = "open";cgReturnCode = BI_RETCODE_COM_OPEN;};
		virtual ~BIC_COM_OPEN(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Open",Str_LowerCase(selfName),"COM.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"[-v]","Open as virtual COM.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_ACOM : public BIC_Node_S{
	public:
				 BIC_ACOM(void) : BIC_Node_S() {cgblTrySubCMD = 1;cgCommand = "acom";cgTitle = cgCommand;cgReturnCode = BI_RETCODE_ACOM;Init();};
		virtual ~BIC_ACOM(void){;};
	public:
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			if (blDetail == 1){
				HelpTraversalChild(env,0);
				return(cgReturnCode);
			}
			PrintHelpItem(env, cgCommand, "->",selfName,"COM setting.");
			if (blDetail == 0)
				return(cgReturnCode);
			return(cgReturnCode);
		};
	private:
		void	Init(void){
			#ifdef SWVERSION_COM
			Add(cgSub_com_ls) < cgSub_com < cgSub_com_br < cgSub_com_open< cgSub_com_ms < cgSub_com_dtr < cgSub_com_rts;
			#endif
		};
	private:
		#ifdef SWVERSION_COM
		BIC_COM_LS		cgSub_com_ls;
		BIC_COM			cgSub_com;
		BIC_COM_BR		cgSub_com_br;
		BIC_COM_OPEN	cgSub_com_open;
		BIC_COM_MS		cgSub_com_ms;
		BIC_COM_DTR		cgSub_com_dtr;
		BIC_COM_RTS		cgSub_com_rts;
		#endif
};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif
