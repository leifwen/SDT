//
//  GList.h
//  SDT
//
//  Created by Leif Wen on 17/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "DS_Tree.h"
#include "SYS_Time.h"
//------------------------------------------------------------------------------------------//
#ifndef GList_h
#define GList_h
#ifdef GList_h
//------------------------------------------------------------------------------------------//
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
				 COMMAND_NODE(void);
		virtual ~COMMAND_NODE(void){;};
	public:							//need record
		STDSTR		StrCommand;
		bool32		blEnableSend;	//enable send command,only valid in GC Spript
		CMD_TAIL	cmdTail;		//send "\r"
		STDSTR		StrShowName;
		bool32		blSameAsCommand;//StrShowName is the same as StrCommand
		STDSTR		StrTimeout;		//unit is S after V0.3.
		STDSTR		StrCycle;
		STDSTR		StrContinue;
		STDSTR		StrResend;
		STDSTR		StrStop;
		STDSTR		StrCatch;
	public:							//no need to record,only used in runtime.
		int32		runTimes;		//
		int32		catchTimes;		//
		int32		timeoutTimes;	//no response times.
		bool32		blFirstSynchronous;
		DTIME		timeST0;		//
	private:
				STDSTR&		ExportV0_4			(uint32 ver,STDSTR* strOut);
				STDSTR&		ExportV0_5			(uint32 ver,STDSTR* strOut);
				STDSTR&		ExportV0_6			(uint32 ver,STDSTR* strOut);
				void		ImportV0_4			(uint32 ver,STDSTR* strIn);
				void		ImportV0_5			(uint32 ver,STDSTR* strIn);
				void		ImportV0_6			(uint32 ver,STDSTR* strIn);
	public:
				void 		Init(void);
				STDSTR&		Export				(uint32 ver,STDSTR* strOut);
				void		Import				(uint32 ver,STDSTR* strIn);
	public:
		static	void		CopyCOMMAND_NODE	(COMMAND_NODE* node1,const COMMAND_NODE* node2);//copy 2 to 1
	public:
		static	STDSTR&		GetTitle			(STDSTR* retStr,uint32 flag);
				STDSTR&		Compose				(STDSTR* retStr,uint32 flag);
				STDSTR&		ComposeDetail		(STDSTR* retStr);
};
//------------------------------------------------------------------------------------------//
class COMMAND_GROUP : public TREE_NODE{
	public:
				 COMMAND_GROUP(void);
		virtual ~COMMAND_GROUP(void);
	protected:
		static	TNFP&			GetTrashOwer	(void);
		virtual	TNFP*			GetTrash		(void);
		virtual	TNF*			CreateNode		(void);
	public:						//need reocord
		STDSTR	name;
		int32	intervalTime;
	public:
		bool32	blEnableAutoRun;//enable
		int32	autoRunTimes;	//cycle
	private:
				STDSTR&		ExportV0_4			(uint32 ver,STDSTR* strOut);
				void		ImportV0_4			(uint32 ver,STDSTR* strIn);
	public:
				void		Init(void);
				STDSTR&		Export				(uint32 ver,STDSTR* strOut);
				void		Import				(uint32 ver,STDSTR* strIn);
	public:
		void				ClearResult(void);
		static void			CopyCOMMAND_GROUP	(COMMAND_GROUP* group1,const COMMAND_GROUP* group2,bool32 blClear = G_TRUE);//copy 2 to 1
	public:
		static	STDSTR&		GetTitle			(STDSTR* retStr,uint32 flag);
				STDSTR&		Compose				(STDSTR* retStr,uint32 flag);
};
//------------------------------------------------------------------------------------------//
class GC_LIST : public TREE_NODE{
	public:
				 GC_LIST(void);
		virtual ~GC_LIST(void);
	protected:
		virtual	TNF*		CreateNode				(void);
	private:
				STDSTR&		ExportV0_4				(uint32 ver,STDSTR* strOut);
				void		ImportV0_4				(uint32 ver,STDSTR* strIn);
	public:
				STDSTR&		Export					(uint32 ver,STDSTR* strOut);
				void		Import					(uint32 ver,STDSTR* strIn);
	
				void		Save					(const STDSTR& fileName);
				void		Load					(const STDSTR& fileName);
	public:
				void		MoveToTrash				(TNF* tFirstNode,TNF* tEndNode = nullptr);
				void		Empty					(void);
	public:
				void		ClearTestResult			(void);
		static	void		CopyCOMMAND_GROUP_ENABLE(GC_LIST* tGroupList1,const GC_LIST* tGroupList2);//copy 2 to 1
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* GList_h */
#endif /* GList_h */
