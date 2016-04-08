/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: SBIC_RunExternal.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.08.12

*/
#ifndef SBIC_RunExternalH
#define SBIC_RunExternalH
//------------------------------------------------------------------------------------------//
#include "SBIC_B.h"
#include "SBIC_Expression.h"
#include "Comm_FIFO.h"
#include "SYS_Thread.h"
#ifdef CommonDefH_Unix
#include <sys/types.h>
#include <sys/wait.h>
//------------------------------------------------------------------------------------------//
class SBIC_RE_Node : public RTREE_NODE{
	public:
		enum{RFLAG_C = 1, RFLAG_S = RTREE_NODE::RFLAG_S + RTREE_NODE::RFLAG_C};
	public:
		SBIC_RE_Node(void) : RTREE_NODE() {
			cgCommand = "";
			cgLable = "";
			childpid = -1;
		};
		virtual ~SBIC_RE_Node(void){;};
	protected:
		pid_t           childpid;
		int             fd_pipeChildOut[2];
		int             fd_pipeChildIn[2];
		std::string		cgCommand;
		std::string		cgLable;
	public:
		int32		Run				(const std::string &lable,const std::string &cmd);
		int32		GetPIPEData		(uint8 *buffer,uint32 tsize)const{return((int32)read(fd_pipeChildOut[0],buffer,tsize));};
		int32		PutPIPEData		(uint8 *buffer,uint32 tsize)const{return((int32)write(fd_pipeChildIn[1],buffer,tsize));};
		int32		ClosePIPE		(void)const;
		int			GetChildStatus	(int *status)const	{return(waitpid(childpid,status,WNOHANG));};
		int			GetChildPID		(void)const			{return(childpid);};
		const std::string&	GetCommand(void)const		{return(cgCommand);};
		const std::string&	GetLable(void)const			{return(cgLable);};
		int32		Kill			(void)const;
		void		SetblKilled		(void){SetSFlag(RFLAG_CREATE(0));};
    	int32		CheckblKilled	(void){return(CheckSFlag(RFLAG_CREATE(0)));};
};
//------------------------------------------------------------------------------------------//
class SBIC_RE_LIST : public RTREE_NODE{
	public:
		enum{RFLAG_C = 0, RFLAG_S = RTREE_NODE::RFLAG_S + RTREE_NODE::RFLAG_C};
				 SBIC_RE_LIST(void);
		virtual ~SBIC_RE_LIST(void);
	private:
		SYS_ThreadEx<SBIC_RE_LIST>		eECPThread;
		int32	ECPThreadFun(void);
	
		DEVICE			*cgDevice;
    	FIFO_UINT8		cgPrintBuffer;
	private:
        void	WriteToPrintBuffer	(SBIC_RE_Node *tNode,uint8 *buffer,int32 size);
		void	PrintBuffer			(void);
	public:
		void	Start				(DEVICE *tDevice);
		void	Stop				(void);
		int32	Kill				(const std::string &lable,const std::string &cmd,int32 timeout);
		std::string	PrintBuffer		(SBIC_RE_Node *tNode,uint8 *buffer,int32 size);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class SBIC_RunExternal : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_RunExternal(void) : SBIC_Node() {
			cgCommand = "RunExternal,=";
			cgReturnCode = SBI_RETCODE_RUNEXTERNAL;
		};
		virtual ~SBIC_RunExternal(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
	protected:
		SBIC_Expression	cgSubC_Expression;
};
//------------------------------------------------------------------------------------------//
class SBIC_StopExternal : public SBIC_Node{
	public:
		enum{RFLAG_C = 0, RFLAG_S = SBIC_Node::RFLAG_S + SBIC_Node::RFLAG_C};
	public:
		SBIC_StopExternal(void) : SBIC_Node() {
			cgCommand = "StopExternal,=";
			cgReturnCode = SBI_RETCODE_STOPEXTERNAL;
		};
		virtual ~SBIC_StopExternal(void){;};
	public:
		virtual	int32	Command	(SBICPAR *tBICPAR,const std::string &par,std::string *ret)const;
		virtual	int32	Help	(SBICPAR *tBICPAR,int32 blDetail = 1)const;
	protected:
		SBIC_Expression	cgSubC_Expression;
};
//------------------------------------------------------------------------------------------//
#endif
#endif
