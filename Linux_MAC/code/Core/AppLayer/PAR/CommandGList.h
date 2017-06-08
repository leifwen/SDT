/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: CommandGList.h
 * File ID	:
 * Remark	:
 * Writer   : Leif Wen
 * Date     : 2013.12.31
*/

//------------------------------------------------------------------------------------------//
#include "Comm_Tree.h"
#include "SYS_Time.h"
//------------------------------------------------------------------------------------------//
#ifdef Comm_TreeH
#ifndef CommandGListH
#define CommandGListH
#ifdef CommandGListH
enum{
	GL_showSelect	= BD_FLAG32(0),
	GL_showCycle	= BD_FLAG32(1),
	GL_showInterval	= BD_FLAG32(2),
	GL_showCL		= BD_FLAG32(3),
	
	CL_showSelect	= BD_FLAG32(4),
	CL_showCycle	= BD_FLAG32(5),
	CL_showTimeout	= BD_FLAG32(6),
	CL_showDetail	= BD_FLAG32(7),
	
	GL_Select		= BD_FLAG32(8),
	GL_setSelect	= BD_FLAG32(9),
	GL_setCycle		= BD_FLAG32(10),
	GL_setInterval	= BD_FLAG32(11),
	
	CL_Select		= BD_FLAG32(12),
	CL_setSelect	= BD_FLAG32(13),
	CL_setCycle		= BD_FLAG32(14),
	CL_setTimeout	= BD_FLAG32(15),
	CL_setHEX		= BD_FLAG32(16),
	CL_setCR		= BD_FLAG32(17),
};
//------------------------------------------------------------------------------------------//
class COMMAND_NODE : public TREE_NODE{
	public:
				 COMMAND_NODE(void) : TREE_NODE(){Init();SetSelfName("COMMAND_NODE");};
		virtual ~COMMAND_NODE(void){;};
	public://need record
		STDSTR		StrCommand;
		int32		blEnableSend;	//enable send command,only valid in GC Spript
		CMD_TAIL	cmdTail;		//send "\r"
		STDSTR		StrShowName;
		int32		blSameAsCommand;//StrShowName is the same as StrCommand
		STDSTR		StrTimeout;		//unit is S after V0.3.
		STDSTR		StrCycle;
		STDSTR		StrContinue;
		STDSTR		StrResend;
		STDSTR		StrStop;
		STDSTR		StrCatch;
	public://no need to record,only used in runtime.
        int32		runTimes;		//
		int32		catchTimes;		//
		int32		timeoutTimes;	//no response times.
		int32		blFirstSynchronous;
		TIME		timeST0;		//
	public:
		void 		Init(void);
		STDSTR		&CreateNodeStrV0_2(STDSTR *retStr);
		STDSTR		&CreateNodeStrV0_3(STDSTR *retStr){return(CreateNodeStrV0_2(retStr));};
		STDSTR		&CreateNodeStrV0_4(STDSTR *retStr);
		STDSTR		&CreateNodeStrV0_5(STDSTR *retStr);
		STDSTR		&CreateNodeStrV0_6(STDSTR *retStr);
		void		SetNodeV0_2(STDSTR *strIn);
		void		SetNodeV0_3(STDSTR *strIn);
		void		SetNodeV0_4(STDSTR *strIn);
		void		SetNodeV0_5(STDSTR *strIn);
		void		SetNodeV0_6(STDSTR *strIn);
	public:
		static void	CopyCOMMAND_NODE(COMMAND_NODE *node1,const COMMAND_NODE *node2);//copy 2 to 1
	public:
		static	STDSTR&	GetTitle(STDSTR *retStr,uint32 flag);
				STDSTR&	Compose	(STDSTR *retStr,uint32 flag);
				STDSTR&	ComposeDetail(STDSTR *retStr);
};
//------------------------------------------------------------------------------------------//
class COMMAND_GROUP : public TREE_NODE{
	public:
				 COMMAND_GROUP(void) : TREE_NODE(){Init();SetSelfName("COMMAND_GROUP");}
		virtual ~COMMAND_GROUP(void){Empty();};
	protected:
		static	TREE_NODE		sgSpareOwner;
		inline	virtual	TNF*	CreateNode	(void){return(SetSubNodeSelfName(new COMMAND_NODE));};
	public:
		inline	virtual	void	MoveToTrash	(TNF *tFirstNode,TNF *tEndNode = nullptr){MoveNodesToTrash(tFirstNode,tEndNode,&sgSpareOwner);};
		inline			void	Empty		(void){CleanChild(&sgSpareOwner);};
				COMMAND_NODE*	GetNewNode	(void);
	public://need reocord
		STDSTR	name;
		int32	intervalTime;
	public:
		int32	blEnableAutoRun;//enable
    	int32	autoRunTimes;	//cycle
	public:
		void	Init(void);
		STDSTR	&CreateGroupStrV0_2(STDSTR *retStr);
		STDSTR	&CreateGroupStrV0_3(STDSTR *retStr){return(CreateGroupStrV0_2(retStr));};
		STDSTR	&CreateGroupStrV0_4(STDSTR *retStr);
		STDSTR	&CreateGroupStrV0_5(STDSTR *retStr);
		STDSTR	&CreateGroupStrV0_6(STDSTR *retStr);
		void	SetGroupV0_2(STDSTR *strIn);
		void	SetGroupV0_3(STDSTR *strIn);
		void	SetGroupV0_4(STDSTR *strIn);
		void	SetGroupV0_5(STDSTR *strIn);
		void	SetGroupV0_6(STDSTR *strIn);
	public:
		void			ClearResult(void);
		static void		CopyCOMMAND_GROUP(COMMAND_GROUP *group1,const COMMAND_GROUP *group2,int32 blClear = 1);//copy 2 to 1
	public:
		static	STDSTR&	GetTitle(STDSTR *retStr,uint32 flag);
				STDSTR&	Compose	(STDSTR *retStr,uint32 flag);
};
//------------------------------------------------------------------------------------------//
class GC_LIST : public TREE_NODE{
	public:
				 GC_LIST(void){CreateTrash(this);SetSelfName("GC_LIST");};
		virtual ~GC_LIST(void){DestroyAll();};
	protected:
		inline	virtual	TNF*	CreateNode	(void){return(SetSubNodeSelfName(new COMMAND_GROUP));};
	public:
						void	Empty		(void);
	public:
		STDSTR	&CreateGroupListStrV0_2(STDSTR *retStr);
		STDSTR	&CreateGroupListStrV0_3(STDSTR *retStr){return(CreateGroupListStrV0_2(retStr));};
		STDSTR	&CreateGroupListStrV0_4(STDSTR *retStr);
		STDSTR	&CreateGroupListStrV0_5(STDSTR *retStr);
		STDSTR	&CreateGroupListStrV0_6(STDSTR *retStr);
		void	SetGroupListV0_2(STDSTR *strIn);
		void	SetGroupListV0_3(STDSTR *strIn);
		void	SetGroupListV0_4(STDSTR *strIn);
		void	SetGroupListV0_5(STDSTR *strIn);
		void	SetGroupListV0_6(STDSTR *strIn);
	public:
		void			ClearTestResult(void);
		static void		CopyCOMMAND_GROUP_ENABLE(GC_LIST *tGroupList1,const GC_LIST *tGroupList2);//copy 2 to 1
};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif

