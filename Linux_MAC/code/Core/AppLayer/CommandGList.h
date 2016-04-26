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
#ifndef CommandGListH
#define CommandGListH
//------------------------------------------------------------------------------------------//
#include "Comm_Tree.h"
#include "SYS_Time.h"
//------------------------------------------------------------------------------------------//
class COMMAND_NODE : public RTREE_NODE{
	public:
				 COMMAND_NODE(void) : RTREE_NODE(){Init();};
		virtual ~COMMAND_NODE(void){;};
	private:
	public://need record
		std::string		StrCommand;
		int32			blEnableSend;	//enable send command,only valid in GC Spript
		int32			blEnableSendCR;	//send "\r"
		int32			blEnableHEX;	//StrCommand is HEX
		std::string		StrShowName;
		int32			blSameAsCommand;//StrShowName is the same as StrCommand
		std::string		StrTimeout;		//unit is S after V0.3.
		std::string		StrCycle;
		std::string		StrContinue;
		std::string		StrResend;
		std::string		StrStop;
		std::string		StrCatch;
	public://no need to record,only used in runtime.
        int32			runTimes;		//
		int32			catchTimes;		//
		int32			timeoutTimes;	//no response times.
		int32			blFirstSynchronous;
		SYS_DateTime	timeST0;		//
	public:
		void 		Init(G_LOCK_VAILD blLock = G_LOCK_ON);
		std::string	CreateNodeStrV0_2(void);
		std::string	CreateNodeStrV0_3(void){return(CreateNodeStrV0_2());};
		std::string	CreateNodeStrV0_4(void);
		std::string	CreateNodeStrV0_5(void);
		void		SetNodeV0_2(std::string *strInput);
		void		SetNodeV0_3(std::string *strInput);
		void		SetNodeV0_4(std::string *strInput);
		void		SetNodeV0_5(std::string *strInput);
		static void	CopyCOMMAND_NODE(const COMMAND_NODE *node2,COMMAND_NODE *node1);//copy 2 to 1
};
//------------------------------------------------------------------------------------------//
class COMMAND_GROUP : public RTREE_NODE{
	public:
				 COMMAND_GROUP(void);
		virtual ~COMMAND_GROUP(void){DestroyAll();};
	public://need reocord
		std::string		name;
		int32			intervalTime;
	public:
		int32			blEnableAutoRun;//enable
    	int32			autoRunTimes;	//cycle

		std::string	CreateGroupStrV0_2(void);
		std::string	CreateGroupStrV0_3(void){return(CreateGroupStrV0_2());};
		std::string	CreateGroupStrV0_4(void);
		std::string	CreateGroupStrV0_5(void);
		void			SetGroupV0_2(std::string *strInput);
		void			SetGroupV0_3(std::string *strInput);
		void			SetGroupV0_4(std::string *strInput);
		void			SetGroupV0_5(std::string *strInput);
	public:
		void			ClearResult(void);
		static void		CopyCOMMAND_GROUP(const COMMAND_GROUP *group2,COMMAND_GROUP *group1,int32 blClear = 1);//copy 2 to 1
};
//------------------------------------------------------------------------------------------//
class GC_LIST : public RTREE_NODE{
	public:
				 GC_LIST(void){CreateTrash(this);};
		virtual ~GC_LIST(void){DestroyAll();};
	private:
	public:
		std::string		CreateGroupListStrV0_2(void);
		std::string		CreateGroupListStrV0_3(void){return(CreateGroupListStrV0_2());};
		std::string		CreateGroupListStrV0_4(void);
		std::string		CreateGroupListStrV0_5(void);
		void			SetGroupListV0_2(std::string *strInput);
		void			SetGroupListV0_3(std::string *strInput);
		void			SetGroupListV0_4(std::string *strInput);
		void			SetGroupListV0_5(std::string *strInput);
	public:
		void			ClearTestResult(void);
		static void		CopyCOMMAND_GROUP_ENABLE(const GC_LIST *tGroupList2, GC_LIST *tGroupList1);//copy 2 to 1
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif