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

//------------------------------------------------------------------------------------------//
#include "BasicDef.h"
#include "BasicClass.h"
#ifndef Comm_TreeH
#define Comm_TreeH
#ifdef Comm_TreeH
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#define CreateOperatorSet(x) \
struct _##x {uint32 value;};\
static inline _##x Set##x(uint32 value){return{value};};

#define CreateOperatorClr(x) \
struct _c##x {uint32 value;};\
static inline _c##x Clr##x(uint32 value){return{value};};


#define CreateOperatorDSet(x,y) \
struct _##x {uint32 value;};\
static inline _##x Set##x(uint32 value = y){return{value};};

#define CreateOperatorFun(x) \
	template <typename T>static inline T& x(T& _tn){_tn._##x();return(_tn);};
//------------------------------------------------------------------------------------------//
#define	TREE_RChain_Traversal_LINE_nolock(NODE_TYPE,NODE_Start,NODE_LINE)\
	NODE_TYPE *operateNode_t,*nextNode_t;\
	operateNode_t = static_cast<NODE_TYPE*>(NODE_Start);\
	while(operateNode_t != nullptr){\
		nextNode_t = static_cast<NODE_TYPE*>(TREE_NODE::GetcgNext(operateNode_t));\
		NODE_LINE;\
		operateNode_t = nextNode_t;\
	};\
//------------------------------------------------------------------------------------------//
#define	TREE_LChildRChain_Traversal_LINE_nolock(NODE_TYPE,NODE_Father,NODE_LINE)\
	TREE_RChain_Traversal_LINE_nolock(NODE_TYPE,TREE_NODE::GetcgDown(NODE_Father),NODE_LINE)
//------------------------------------------------------------------------------------------//
#define	TREE_RChain_Traversal_LINE(NODE_TYPE,NODE_Start,NODE_LINE)\
	NODE_TYPE *operateNode_t,*nextNode_t;\
	TREE_NODE::Spin_Father_set(NODE_Start);\
	operateNode_t = static_cast<NODE_TYPE*>(NODE_Start);\
	while(operateNode_t != nullptr){\
		nextNode_t = static_cast<NODE_TYPE*>(TREE_NODE::GetcgNext(operateNode_t));\
		NODE_LINE;\
		operateNode_t = nextNode_t;\
	};\
	TREE_NODE::Spin_Father_clr(NODE_Start);
//------------------------------------------------------------------------------------------//
#define	TREE_LChildRChain_Traversal_LINE(NODE_TYPE,NODE_Father,NODE_LINE)\
	NODE_Father->Spin_Child_set();\
	TREE_RChain_Traversal_LINE_nolock(NODE_TYPE,TREE_NODE::GetcgDown(NODE_Father),NODE_LINE);\
	NODE_Father->Spin_Child_clr();\
//------------------------------------------------------------------------------------------//
#define	TREE_RChain_Find_nolock(NODE_TYPE,NODE_Start,Node_Ret,NODE_IF)\
	Node_Ret = nullptr;\
	TREE_RChain_Traversal_LINE_nolock(NODE_TYPE,NODE_Start,if (NODE_IF){Node_Ret = operateNode_t;break;});
//------------------------------------------------------------------------------------------//
#define	TREE_RChain_Find(NODE_TYPE,NODE_Start,Node_Ret,NODE_IF)\
	Node_Ret = nullptr;\
	TREE_RChain_Traversal_LINE(NODE_TYPE,NODE_Start,if (NODE_IF){Node_Ret = operateNode_t;break;});
//------------------------------------------------------------------------------------------//
#define	TREE_LChildRChain_Find(NODE_TYPE,NODE_Father,Node_Ret,NODE_IF)\
	NODE_Father->Spin_Child_set();\
	TREE_RChain_Find_nolock(NODE_TYPE,TREE_NODE::GetcgDown(NODE_Father),Node_Ret,NODE_IF);\
	NODE_Father->Spin_Child_clr();
//------------------------------------------------------------------------------------------//
#define	TREE_LChildRChain_T(NODE_TYPE,NODE_FUN) TREE_LChildRChain_Traversal_LINE(NODE_TYPE,this,operateNode_t->NODE_FUN)
//------------------------------------------------------------------------------------------//
namespace {
	CreateOperatorFun(Start)
	CreateOperatorFun(Endl)
};
//------------------------------------------------------------------------------------------//
class TREE_NODE_FRAME : public BASIC_CFLAG{
	protected:
		typedef TREE_NODE_FRAME TNF;
	public:
				 TREE_NODE_FRAME(void);
		virtual	~TREE_NODE_FRAME(void){;};
	private:
		static	std::atomic_uint	cgID_NextNode;
				std::atomic_uint	nodeID;			//no changed,be setted when create.
				std::atomic_uint	cgID_NextDRNode;
				std::atomic_uint	dRNodeID;		//Right dynamic ID
	public:
		inline	static  uint32	GetNodeID	(const TNF *mySelf){return((mySelf != nullptr)?mySelf->nodeID.load():0);};
		inline	static 	uint32	GetdRNodeID	(const TNF *mySelf){return((mySelf != nullptr)?mySelf->dRNodeID.load():0);};
	private:
		TNF		*cgHead;
		TNF		*cgTail;
		TNF		*cgPrior;
		TNF		*cgNext;
		TNF		*cgUp;
		TNF		*cgDown;
	public:
		inline	static 	TNF*	GetcgHead	(const TNF *mySelf){return((mySelf != nullptr)?mySelf->cgHead:nullptr);};
		inline	static 	TNF*	GetcgTail	(const TNF *mySelf){return((mySelf != nullptr)?mySelf->cgTail:nullptr);};
		inline	static 	TNF*	GetcgPrior	(const TNF *mySelf){return((mySelf != nullptr)?mySelf->cgPrior:nullptr);};
		inline	static 	TNF*	GetcgNext	(const TNF *mySelf){return((mySelf != nullptr)?mySelf->cgNext:nullptr);};
		inline	static 	TNF*	GetcgUp		(const TNF *mySelf){return((mySelf != nullptr)?mySelf->cgUp:nullptr);};
		inline	static 	TNF*	GetcgDown	(const TNF *mySelf){return((mySelf != nullptr)?mySelf->cgDown:nullptr);};
	private:
		SYS_Lock		cgChildLock;
	public:
		inline	void	Spin_Child_set	(G_LOCK blVaild = G_LOCK_ON){cgChildLock.Lock(blVaild);};
		inline	void	Spin_Child_clr	(G_LOCK blVaild = G_LOCK_ON){cgChildLock.Unlock(blVaild);};
		inline	int32	Spin_Child_try	(G_LOCK blVaild = G_LOCK_ON){return(cgChildLock.TryLock(blVaild));};
	
		static	void	Spin_Child_set	(TNF *tTreeNode,G_LOCK blVaild = G_LOCK_ON);
		static	void	Spin_Child_clr	(TNF *tTreeNode,G_LOCK blVaild = G_LOCK_ON);
		static	int32	Spin_Child_try	(TNF *tTreeNode,G_LOCK blVaild = G_LOCK_ON);
	
		static	void	Spin_Father_set	(TNF *tTreeNode,G_LOCK blVaild = G_LOCK_ON);
		static	void	Spin_Father_clr	(TNF *tTreeNode,G_LOCK blVaild = G_LOCK_ON);
		static	int32	Spin_Father_try	(TNF *tTreeNode,G_LOCK blVaild = G_LOCK_ON);
	private:
		static	void	UpdateInstert_nolock(TNF *tInsertNode,TREE_NODE_FRAME *rChainHead,int32 blUpdatedRNodeID);
		static	void	UpdateHead_nolock	(TNF *tHeadNode,int32 blUpdatedRNodeID);
	public:
		static	TNF*	InsertAfter_nolock	(TNF *tOpNode,TNF *tInsertNode,int32 blUpdatedRNodeID = 1);
		static	TNF*	InsertBefore_nolock	(TNF *tOpNode,TNF *tInsertNode,int32 blUpdatedRNodeID = 1);
		static	TNF*	AddSubNode_nolock	(TNF *tFatherNode,TNF *tInsertNode);
		static	TNF*	Remove_nolock		(TNF *tFirstNode,TNF *tEndNode = nullptr,int32 blUpdatedRNodeID = 1);
		static	void	MoveUp_nolock		(TNF *tFirstNode,TNF *tEndNode = nullptr);
		static	void	MoveDown_nolock		(TNF *tFirstNode,TNF *tEndNode = nullptr);
		static	void	MoveAfter_nolock	(TNF *tFirstNode,TNF *tEndNode = nullptr,TNF *tAfterNode = nullptr);
		static	TNF*	BreakChild_nolock	(TNF *tFatherNode);
		static	TNF*	BreakNext_nolock	(TNF *tTreeNode);
	
		static	TNF*	InsertAfter			(TNF *tOpNode,TNF *tInsertNode,int32 blUpdatedRNodeID = 1);
		static	TNF*	InsertBefore		(TNF *tOpNode,TNF *tInsertNode,int32 blUpdatedRNodeID = 1);
		static	TNF*	AddSubNode			(TNF *tFatherNode,TNF *tInsertNode);
		static	TNF*	Remove				(TNF *tFirstNode,TNF *tEndNode = nullptr,int32 blUpdatedRNodeID = 1);
		static	void	MoveUp				(TNF *tFirstNode,TNF *tEndNode = nullptr);
		static	void	MoveDown			(TNF *tFirstNode,TNF *tEndNode = nullptr);
		static	void	MoveAfter			(TNF *tFirstNode,TNF *tEndNode = nullptr,TNF *tAfterNode = nullptr);
		static	TNF*	BreakChild			(TNF *tFatherNode);
		static	TNF*	BreakNext			(TNF *tTreeNode);
	public:
		inline			TNF&	Add			(TNF &tTreeNode){return(*AddNode(&tTreeNode));};
		inline	virtual	TNF*	AddNode		(TNF *tTreeNode){return(AddSubNode(this,tTreeNode));};
		inline	virtual	void	RemoveSelf	(void)			{Remove(this,nullptr);};
	public:
		inline	virtual void _Start(void){;};
		inline	virtual void _Endl (void){;};
	public:
		inline TNF& operator < (TNF& tTreeNode)		{return(Add(tTreeNode));};
		inline TNF& operator << (TNF&(*fun)(TNF&))	{return((*fun)(*this));};
};
//------------------------------------------------------------------------------------------//
class TREE_NODE_FRAME_POOL : public TREE_NODE_FRAME{
	protected:
		typedef TREE_NODE_FRAME_POOL TNFP;
	public:
				 TREE_NODE_FRAME_POOL(void);
		virtual	~TREE_NODE_FRAME_POOL(void){Spin_InUse_set();DestroyTrash(this); Spin_InUse_clr();};
	public:
		STDSTR	selfName;
	public:
		inline	virtual void	SetSelfName			(const STDSTR &strName){selfName = strName;};
				virtual TNFP*	SetSubNodeSelfName	(TNFP *node);
	private:
		TNFP	*cgTrash;
	private:
		static	void	DestroyTrash		(TNFP *tTrashOwner);
	public:
		static	TNFP*	GetcgTrash			(TNFP *tTrashOwner){return(tTrashOwner->cgTrash);};
		static	void	CreateTrash			(TNFP *tTrashOwner);
		static	void	CleanTrash			(TNFP *tTrashOwner);
		static	void	MoveNodesToTrash	(TNF *tFirstNode,TNF *tEndNode,TNFP *tTrashOwner);
		static	void	MoveNodeToTrash		(TNF *tTreeNode,TNFP *tTrashOwner);
		static	void	DestroyTree			(TNF *tTreeNode);
		static	void	DestroySubTree		(TNF *tTreeNode);
				void	CleanChild			(TNFP *tTrashOwner);
		inline	void	DestroyAll			(void){CleanChild(this);CleanTrash(this);};
		static	TNF*	GetNewNodeFromTrash	(TNFP *tTrashOwner);
		static	TNF*	Empty				(TNFP *tTreeNode);
	
		inline	virtual	void	Empty		(void){CleanChild(this);};
		inline	virtual	TNF*	GetNewNode	(void){return(GetNewNodeFromTrash(this));};
		inline	virtual	TNF*	CreateNode	(void){return(SetSubNodeSelfName(new TNFP));};
		inline	virtual	void	MoveToTrash	(TNF *tFirstNode,TNF *tEndNode = nullptr){MoveNodesToTrash(tFirstNode,tEndNode,this);};
};
//------------------------------------------------------------------------------------------//
struct TRASH{;};
//------------------------------------------------------------------------------------------//
class TREE_NODE : public TREE_NODE_FRAME_POOL{
	public:
		enum		{RFLAG_C = 3, RFLAG_S = TREE_NODE_FRAME_POOL::RFLAG_S + TREE_NODE_FRAME_POOL::RFLAG_C};
		enum CNType	{CN_None = 0,CN_M,CN_S};
	private:
		enum{tn_blEnable = RFLAG_CREATE(0),tn_blSelected = RFLAG_CREATE(1),tn_blUpdate = RFLAG_CREATE(2),};
	public:
				 TREE_NODE(void) : TREE_NODE_FRAME_POOL(){Init();};
				 TREE_NODE(const TRASH *X) : TREE_NODE_FRAME_POOL(){Init();CreateTrash(this);};
		virtual	~TREE_NODE(void){;};
	public:
		inline	virtual	TREE_NODE*	AddNode		(TNF *tTreeNode){return(static_cast<TREE_NODE*>(AddSubNode(this,tTreeNode)));};
	private:
		void Init(void);
	private:
		CNType			cgCNType;
		TREE_NODE		*cgCoupleNode;
	public:
		inline	static 	CNType		GetCNType		(const TREE_NODE *mySelf){return((mySelf != nullptr)?mySelf->cgCNType:CN_None);};
		inline	static 	TREE_NODE*	GetCoupleNode	(const TREE_NODE *mySelf){return((mySelf != nullptr)?mySelf->cgCoupleNode:nullptr);};
		void		LinkCoupleNode					(TREE_NODE *slaveNode);
		void		LinkCoupleNode_nolock			(TREE_NODE *slaveNode);
		TREE_NODE*	UnlinkCoupleNode				(void);
	public:
		inline	virtual	void	Enable			(void)		{SetSFlag(tn_blEnable);};
		inline	virtual	void	Disable			(void)		{ClrSFlag(tn_blEnable);};
		inline			int32	CheckEnable		(void)const	{return(CheckSFlag(tn_blEnable));};
		
		inline	virtual	void	SetblSelected	(void)		{SetSFlag(tn_blSelected);};
		inline	virtual	void	ClrblSelected	(void)		{ClrSFlag(tn_blSelected);};
		inline			int32	CheckSelected	(void)const	{return(CheckSFlag(tn_blSelected));};
		
		inline	virtual	void	SetblUpdate		(void)		{SetSFlag(tn_blUpdate);};
		inline	virtual	void	ClrblUpdate		(void)		{ClrSFlag(tn_blUpdate);};
		inline			int32	CheckUpdate		(void)const	{return(CheckSFlag(tn_blUpdate));};
	
		static	TREE_NODE*	FindInLChildRChainByDRNodeID		(TREE_NODE *tTreeNode,uint32 tDRNodeID);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
#endif
