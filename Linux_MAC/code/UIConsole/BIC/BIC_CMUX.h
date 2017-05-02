/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: BIC_CMUX.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.25
*/
//------------------------------------------------------------------------------------------//
#include "BIC_B.h"
#include "CMUX.h"
//------------------------------------------------------------------------------------------//
#if defined CMUXH && defined BIC_BH && defined SWVERSION_CMUX
#ifndef BIC_CMUXH
#define BIC_CMUXH
#ifdef BIC_CMUXH
//------------------------------------------------------------------------------------------//
class BIC_CMUX_ON : public BIC_Node{
	public:
			     BIC_CMUX_ON(void) : BIC_Node() {cgCommand = "on";cgReturnCode = BI_RETCODE_CMUX_ON;};
	    virtual ~BIC_CMUX_ON(void){;};
	public:
	    virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Start CMUX.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"[n]"		,"Start CMUX with external group initialization.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_CMUX_OFF : public BIC_Node{
	public:
		    	 BIC_CMUX_OFF(void) : BIC_Node() {cgCommand = "off";cgReturnCode = BI_RETCODE_CMUX_OFF;};
	    virtual ~BIC_CMUX_OFF(void){;};
	public:
	    virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Stop CMUX.");
			if (blDetail == 0)
				return(cgReturnCode);
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_CMUX_CLD : public BIC_Node{
	public:
			     BIC_CMUX_CLD(void) : BIC_Node() {cgCommand = "cld";cgReturnCode = BI_RETCODE_CMUX_CLD;};
	    virtual ~BIC_CMUX_CLD(void){;};
	public:
	    virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Send CLD.");
			if (blDetail == 0)
				return(cgReturnCode);
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_CMUX_PSC : public BIC_Node{
	public:
			     BIC_CMUX_PSC(void) : BIC_Node() {cgCommand = "psc";cgReturnCode = BI_RETCODE_CMUX_PSC;};
	    virtual ~BIC_CMUX_PSC(void){;};
	public:
	    virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Send PSC.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"[n]"		,"num.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_CMUX_FC : public BIC_Node{
	public:
			     BIC_CMUX_FC(void) : BIC_Node() {cgCommand = "fc";cgReturnCode = BI_RETCODE_CMUX_FC;};
	    virtual ~BIC_CMUX_FC(void){;};
	public:
	    virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Send FC.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<on|off>","On/Off.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_CMUX_DTR : public BIC_Node{
	public:
				 BIC_CMUX_DTR(void) : BIC_Node() {cgCommand = "dtr";cgReturnCode = BI_RETCODE_CMUX_DTR;};
		virtual ~BIC_CMUX_DTR(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Set DTR.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<num>"	,"CUX virtual COM num(1-4).");
			PrintHelpSubItem(env,"<[-H[t]]"	,"Set to High, t is time, unit is S.");
			PrintHelpSubItem(env," [-L[t]]>","Set to Low, t is time, unit is S.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_CMUX_RTS : public BIC_Node{
	public:
				 BIC_CMUX_RTS(void) : BIC_Node() {cgCommand = "rts";cgReturnCode = BI_RETCODE_CMUX_RTS;};
		virtual ~BIC_CMUX_RTS(void){;};
	public:
		virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Set RTS.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"<num>"	,"CUX virtual COM num(1-4).");
			PrintHelpSubItem(env,"<[-H[t]]"	,"Set to High, t is time, unit is S.");
			PrintHelpSubItem(env," [-L[t]]>","Set to Low, t is time, unit is S.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_DLCI : public BIC_Node{
	public:
			     BIC_DLCI(void) : BIC_Node() {cgCommand = "dlci";cgReturnCode = BI_RETCODE_DLCI;};
	    virtual ~BIC_DLCI(void){;};
	public:
	    virtual	int32	Command (BIC_ENV *env,const STDSTR &par,void *p)const;
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			PrintHelpItem(env,cgCommand,"Set/list CMUX virtual COM status.");
			if (blDetail == 0)
				return(cgReturnCode);
			PrintHelpSubItem(env,"[num"				,"CUX virtual COM num(1-4).");
			PrintHelpSubItem(env," <-H|-A>"			,"Receive mode: Hex/ASCII.");
			PrintHelpSubItem(env," |-E<on|off>>]"	,"Enable/disable escape.");
			return(cgReturnCode);
		};
};
//------------------------------------------------------------------------------------------//
class BIC_CMUX : public BIC_Node_S{
	public:
				 BIC_CMUX(void) : BIC_Node_S() {cgCommand = "cmux";cgTitle = cgCommand;cgReturnCode = BI_RETCODE_CMUX;Init();};
		virtual ~BIC_CMUX(void){;};
	public:
		virtual	int32	Help	(BIC_ENV *env,int32 blDetail = 1)const{
			if (blDetail == 1){
				HelpTraversalChild(env,0);
				return(cgReturnCode);
			}
			PrintHelpItem(env, cgCommand, "-> CMUX.");
			if (blDetail == 0)
				return(cgReturnCode);
			return(cgReturnCode);
		};
	    void	Init(void){Add(cgSub_on) < cgSub_off < cgSub_cld < cgSub_psc < cgSub_fc < cgSub_dtr < cgSub_rts < cgSub_dlci;}
	    BIC_CMUX_ON		cgSub_on;
	    BIC_CMUX_OFF	cgSub_off;
	    BIC_CMUX_CLD	cgSub_cld;
	    BIC_CMUX_PSC	cgSub_psc;
	    BIC_CMUX_FC		cgSub_fc;
		BIC_CMUX_DTR	cgSub_dtr;
		BIC_CMUX_RTS	cgSub_rts;
	    BIC_DLCI		cgSub_dlci;
};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif

