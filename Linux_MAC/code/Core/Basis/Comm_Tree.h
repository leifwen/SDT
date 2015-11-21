/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: Comm_Tree.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2014.01.04
 * @2015.10.01 : rewrite
*/
#ifndef Comm_TreeH
#define Comm_TreeH
//------------------------------------------------------------------------------------------//
#include "BasicDef.h"
#include "BasicClass.h"
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#define	RTREE_RChain_Traversal_LINE_nolock(NODE_TYPE,NODE_Start,NODE_LINE)\
	NODE_TYPE *operateNode_t,*nextNode_t;\
	operateNode_t = (NODE_TYPE*)NODE_Start;\
	while(operateNode_t != nullptr){\
		nextNode_t = (NODE_TYPE*)RTREE_NODE::GetcgRChild(operateNode_t);\
		NODE_LINE;\
		operateNode_t = nextNode_t;\
	};\
//------------------------------------------------------------------------------------------//
#define	RTREE_LChildRChain_Traversal_LINE_nolock(NODE_TYPE,NODE_Father,NODE_LINE)\
	RTREE_RChain_Traversal_LINE_nolock(NODE_TYPE,RTREE_NODE::GetcgLChild(NODE_Father),NODE_LINE)
//------------------------------------------------------------------------------------------//
#define	RTREE_RChain_Traversal_LINE(NODE_TYPE,NODE_Start,NODE_LINE)\
	NODE_TYPE *rootNode_t,*operateNode_t,*nextNode_t;\
	if (NODE_Start != nullptr){\
	RESTART_RTREE_RChain_Traversal_LINE:;\
		NODE_Start->Spin_Link_Lock();\
		rootNode_t = (NODE_TYPE*)NODE_Start;\
		if (RTREE_NODE::GetcgblVirtualHead(NODE_Start) == 0){\
			rootNode_t = (NODE_TYPE*)RTREE_NODE::GetcgVirtualRoot(NODE_Start);\
			if (rootNode_t->Spin_Link_Try() == 0){\
				NODE_Start->Spin_Link_Lock();\
				goto RESTART_RTREE_RChain_Traversal_LINE;\
			}\
		}\
		operateNode_t = (NODE_TYPE*)NODE_Start;\
		while(operateNode_t != nullptr){\
			if (operateNode_t != NODE_Start)\
				operateNode_t->Spin_Link_Lock();\
			nextNode_t = (NODE_TYPE*)RTREE_NODE::GetcgRChild(operateNode_t);\
			if (operateNode_t != rootNode_t)\
				operateNode_t->Spin_Link_Unlock();\
			NODE_LINE;\
			operateNode_t = nextNode_t;\
		}\
		rootNode_t->Spin_Link_Unlock();\
	};
//------------------------------------------------------------------------------------------//
#define	RTREE_LChildRChain_Traversal_LINE(NODE_TYPE,NODE_Father,NODE_LINE)\
	NODE_Father->Spin_Link_Lock();\
	RTREE_RChain_Traversal_LINE(NODE_TYPE,RTREE_NODE::GetcgLChild(NODE_Father),NODE_LINE);\
	NODE_Father->Spin_Link_Unlock();\
//------------------------------------------------------------------------------------------//
#define	RTREE_RChain_Find(NODE_TYPE,NODE_Start,Node_Ret,NODE_IF)\
	Node_Ret = nullptr;\
	RTREE_RChain_Traversal_LINE(NODE_TYPE,NODE_Start,if (NODE_IF){Node_Ret = operateNode_t;break;});
//------------------------------------------------------------------------------------------//
#define	RTREE_LChildRChain_Find(NODE_TYPE,NODE_Father,Node_Ret,NODE_IF)\
	NODE_Father->Spin_Link_Lock();\
	RTREE_RChain_Find(NODE_TYPE,RTREE_NODE::GetcgLChild(NODE_Father),Node_Ret,NODE_IF);\
	NODE_Father->Spin_Link_Unlock();
//------------------------------------------------------------------------------------------//
#define	RTREE_LChildRChain_Excute_S_R(NODE_TYPE,NODE_Father,Node_Ret,NODE_FUN)\
	NODE_TYPE	*tSelect_t;\
	tSelect_t = (NODE_TYPE*)GetSelectedInLChildRChain(NODE_Father);\
	if (tSelect_t != nullptr)\
		Node_Ret = tSelect_t->NODE_FUN;
//------------------------------------------------------------------------------------------//
#define	RTREE_LChildRChain_Excute_S(NODE_TYPE,NODE_Father,NODE_FUN)\
	NODE_TYPE	*tSelect_t;\
	tSelect_t = (NODE_TYPE*)GetSelectedInLChildRChain(NODE_Father);\
	if (tSelect_t != nullptr)\
		tSelect_t->NODE_FUN;
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#define	RTREE_LChildRChain_T(NODE_TYPE,NODE_FUN) RTREE_LChildRChain_Traversal_LINE(NODE_TYPE,this,operateNode_t->NODE_FUN)
//------------------------------------------------------------------------------------------//
#define	RTREE_LChildRChain_E_S_R(NODE_TYPE,NODE_FUN,RET_Type)\
	RET_Type	ret_RET_Type_t;\
	ret_RET_Type_t = 0;\
	RTREE_LChildRChain_Excute_S_R(NODE_TYPE,this,ret_RET_Type_t,NODE_FUN);\
	return(ret_RET_Type_t);
//------------------------------------------------------------------------------------------//
#define	RTREE_LChildRChain_E_S(NODE_TYPE,NODE_FUN) RTREE_LChildRChain_Excute_S(NODE_TYPE,this,NODE_FUN);
//------------------------------------------------------------------------------------------//
class RTREE_NODE : public BASIC_CFLAG{
	public:
		enum{RFLAG_C = 3, RFLAG_S = BASIC_CFLAG::RFLAG_S + BASIC_CFLAG::RFLAG_C};
		enum CNType	{CN_None = 0,CN_1,CN_0};
				 RTREE_NODE(void);
		virtual	~RTREE_NODE(void){ DestroyTrash(this); };
	public:
		std::string	selfName;
	private:
		static	std::atomic_uint	cgID_NextNode;	//the ID of next the same class in system.
				std::atomic_uint	nodeID;			//no changed,be setted when create.
				std::atomic_uint	cgID_NextDRNode;
				std::atomic_uint	dRNodeID;		//Right dynamic ID
	public:
		inline	static 	const std::atomic_uint&	GetnodeID	(const RTREE_NODE *mySelf){return(mySelf->nodeID);};
		inline	static 	const std::atomic_uint&	GetdRNodeID	(const RTREE_NODE *mySelf){return(mySelf->dRNodeID);};
	private:
		SYS_Lock		cgInLinking;
		SYS_Lock		cgInDoing;
	public:
		inline	void	Spin_Link_Lock	(G_LOCK_VAILD blVaild = G_LOCK_ON){cgInLinking.Lock(blVaild);};
		inline	void	Spin_Link_Unlock(G_LOCK_VAILD blVaild = G_LOCK_ON){cgInLinking.Unlock(blVaild);};
		inline	int32	Spin_Link_Try	(G_LOCK_VAILD blVaild = G_LOCK_ON){return(cgInLinking.TryLock(blVaild));};

		inline	void	InDoing_set		(G_LOCK_VAILD blVaild = G_LOCK_ON){cgInDoing.Lock(blVaild);};
		inline	void	InDoing_clr		(G_LOCK_VAILD blVaild = G_LOCK_ON){cgInDoing.Unlock(blVaild);};
		inline	int32	InDoing_try		(G_LOCK_VAILD blVaild = G_LOCK_ON){return(cgInDoing.TryLock(blVaild));};
	private:
		RTREE_NODE		*cgTrash;
		int32			cgblVirtualHead;
		RTREE_NODE		*cgVirtualRoot;
		RTREE_NODE		*cgFather;
		RTREE_NODE		*cgLChild;
		RTREE_NODE		*cgRChild;
	protected:
		inline	static 	RTREE_NODE*	GetcgTrash			(const RTREE_NODE *mySelf){return(mySelf->cgTrash);};
	public:
		inline	static 	int32		GetcgblVirtualHead	(const RTREE_NODE *mySelf){return(mySelf->cgblVirtualHead);};
		inline	static 	RTREE_NODE*	GetcgVirtualRoot	(const RTREE_NODE *mySelf){return(mySelf->cgVirtualRoot);};
		inline	static 	RTREE_NODE*	GetcgFather			(const RTREE_NODE *mySelf){return(mySelf->cgFather);};
		inline	static 	RTREE_NODE*	GetcgLChild			(const RTREE_NODE *mySelf){return(mySelf->cgLChild);};
		inline	static 	RTREE_NODE*	GetcgRChild			(const RTREE_NODE *mySelf){return(mySelf->cgRChild);};
	private:
		CNType			cgCNType;
		RTREE_NODE		*cgCoupleNode;
	public:
		inline	static 	CNType		GetcgCNType			(const RTREE_NODE *mySelf){return(mySelf->cgCNType);};
		inline	static 	RTREE_NODE*	GetcgCoupleNode		(const RTREE_NODE *mySelf){return(mySelf->cgCoupleNode);};
						void		LinkCoupleNode		(RTREE_NODE *slaveNode,G_LOCK_VAILD blLock = G_LOCK_ON);
						void		UnlinkCoupleNode	(G_LOCK_VAILD blLock = G_LOCK_ON);
	private:
		static	void		UpdateSubInstertTree_nolock	(RTREE_NODE *tInsertNode,RTREE_NODE *tVirtualRoot);
		static	void		UpdateSubRChain_nolock		(RTREE_NODE *tSubRootNode);
	public:
		static	int32		InsertRChild			(RTREE_NODE *tFatherNode,RTREE_NODE *tInsertNode);
		static	int32		InsertLChild			(RTREE_NODE *tFatherNode,RTREE_NODE *tInsertNode,G_LOCK_VAILD blLock = G_LOCK_ON);
		static	RTREE_NODE*	BreakRChild				(RTREE_NODE *tFatherNode);
		static	RTREE_NODE*	BreakLChild				(RTREE_NODE *tFatherNode);
		static	RTREE_NODE*	FindInTreeByNodeID		(RTREE_NODE *tTreeNode, uint32 tNodeID);
	
		static	void 		MoveNodesUpInRChain		(RTREE_NODE *tFirstNode,RTREE_NODE *tEndNode = nullptr);
		static	void 		MoveNodesDownInRChain	(RTREE_NODE *tFirstNode,RTREE_NODE *tEndNode = nullptr);
		static	void 		MoveNodesAfterInRChain	(RTREE_NODE *tFirstNode,RTREE_NODE *tEndNode = nullptr,RTREE_NODE *tAfterNode = nullptr);
		static	RTREE_NODE*	RemoveNodesInRChain		(RTREE_NODE *tFirstNode,RTREE_NODE *tEndNode = nullptr,G_LOCK_VAILD blLock = G_LOCK_ON);
		static	void		RemoveFromFather		(RTREE_NODE *tTreeNode);
		inline	RTREE_NODE*	RemoveSelf				(void){return(RemoveNodesInRChain(this,nullptr));};
	protected:
		static	void		CreateTrash				(RTREE_NODE *tTrashOwner);
		static	void		DestroyTrash			(RTREE_NODE *tTrashOwner);
	public:
		static	void		CleanTrash				(RTREE_NODE *tTrashOwner);
		static	RTREE_NODE*	MoveNodesToTrash		(RTREE_NODE *tFirstNode,RTREE_NODE *tEndNode,RTREE_NODE *tTrashOwner);
		static	RTREE_NODE*	MoveNodeToTrash			(RTREE_NODE *tTreeNode,RTREE_NODE *tTrashOwner);
		static	void		MoveTreeToTrash			(RTREE_NODE *tTreeNode,RTREE_NODE *tTrashOwner);
		inline	void		DestroyAll(void)		{ Spin_InUse_set(); MoveTreeToTrash(cgLChild, this); CleanTrash(this); Spin_InUse_clr(); };
	public:
		virtual	void	Enable			(void)		{SetSFlag(RFLAG_CREATE(0));};
		virtual	void	Disable			(void)		{ClrSFlag(RFLAG_CREATE(0));};
		inline		int32	CheckblEnabled	(void)const	{return(CheckSFlag(RFLAG_CREATE(0)));};
	
		inline		void	SetblSelected	(void)		{SetSFlag(RFLAG_CREATE(1));};
		inline		void	ClrblSelected	(void)		{ClrSFlag(RFLAG_CREATE(1));};
		inline		int32	CheckblSelected	(void)const	{return(CheckSFlag(RFLAG_CREATE(1)));};
	
		inline		void	SetblUpdate		(void)		{SetSFlag(RFLAG_CREATE(2));};
		inline		void	ClrblUpdate		(void)		{ClrSFlag(RFLAG_CREATE(2));};
		inline		int32	CheckblUpdate	(void)const	{return(CheckSFlag(RFLAG_CREATE(2)));};
	public://used as linklist
		static	RTREE_NODE*	GetFather				(RTREE_NODE *tTreeNode);
		static	RTREE_NODE*	GetFirstChild			(RTREE_NODE *tTreeNode);
		static	RTREE_NODE*	GetLastChild			(RTREE_NODE *tTreeNode);
		static	RTREE_NODE*	GetFirstBrother			(RTREE_NODE *tTreeNode);
		static	RTREE_NODE*	GetLastBrother			(RTREE_NODE *tTreeNode);
		static	RTREE_NODE*	GetNextBrother			(RTREE_NODE *tTreeNode);
		static	RTREE_NODE*	GetPriorBrother			(RTREE_NODE *tTreeNode);
	
		static	RTREE_NODE*	GetFather_nolock		(RTREE_NODE *tTreeNode);
		static	RTREE_NODE*	GetFirstChild_nolock	(RTREE_NODE *tTreeNode);
		static	RTREE_NODE*	GetLastChild_nolock		(RTREE_NODE *tTreeNode);
		static	RTREE_NODE*	GetFirstBrother_nolock	(RTREE_NODE *tTreeNode);
		static	RTREE_NODE*	GetLastBrother_nolock	(RTREE_NODE *tTreeNode);
		static	RTREE_NODE*	GetNextBrother_nolock	(RTREE_NODE *tTreeNode);
		static	RTREE_NODE*	GetPriorBrother_nolock	(RTREE_NODE *tTreeNode);

		static	RTREE_NODE*	FindInLChildRChainByDRNodeID		(RTREE_NODE *tTreeNode,uint32 tDRNodeID);
		static	RTREE_NODE*	FindInLChildRChainByDRNodeID_nolock	(RTREE_NODE *tTreeNode,uint32 tDRNodeID);
		static	RTREE_NODE*	GetSelectedInLChildRChain			(RTREE_NODE *tTreeNode);
		static	RTREE_NODE*	GetSelectedInLChildRChain_nolock	(RTREE_NODE *tTreeNode);
	public:
		virtual	int32	AddNode			(RTREE_NODE *tTreeNode){return(InsertLChild(this,tTreeNode));};
		virtual	void	LSetblSelected	(void){RTREE_LChildRChain_T(RTREE_NODE,SetblSelected());};
		virtual	void	LClrblSelected	(void){RTREE_LChildRChain_T(RTREE_NODE,ClrblSelected());};
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif