//
//  DS_Tree.h
//  SDT
//
//  Created by Leif Wen on 18/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#ifndef DS_Tree_h
#define DS_Tree_h
#ifdef DS_Tree_h
//------------------------------------------------------------------------------------------//
#include "BasicDefine.h"
#include "DS_BaseClass.h"
//------------------------------------------------------------------------------------------//
class	TREE_NODE_FRAME;
typedef TREE_NODE_FRAME TNF;
class TREE_NODE_FRAME : public BASE_FLAG{
	public:
				 TREE_NODE_FRAME(void);
		virtual	~TREE_NODE_FRAME(void);
	private:
		static	std::atomic_uint	cgID_NextNode;
				std::atomic_uint	nodeID;			//no changed,be setted when create.
				std::atomic_uint	cgID_NextDRNode;
				std::atomic_uint	dRNodeID;		//Right dynamic ID
	public:
		static	inline	uint32	GetNodeID		(const TNF* tnfNode);
		static			STDSTR	GetHEXID		(const TNF* tnfNode);
		static	inline	uint32	GetdRNodeID		(const TNF* tnfNode);
	private:
		TNF*	cgHead;
		TNF*	cgTail;
		TNF*	cgPrior;
		TNF*	cgNext;
		TNF*	cgUp;
		TNF*	cgDown;
	public:
		static	inline 	TNF*	GetHead			(const TNF* tnfNode);
		static	inline 	TNF*	GetTail			(const TNF* tnfNode);
		static	inline 	TNF*	GetPrior		(const TNF* tnfNode);
		static	inline 	TNF*	GetNext			(const TNF* tnfNode);
		static	inline 	TNF*	GetUp			(const TNF* tnfNode);
		static	inline 	TNF*	GetDown			(const TNF* tnfNode);
	private:
		DS_Lock			cgChildLock;
	public:
		inline			void	LockChild_set	(G_LOCK blVaild = G_LOCK_ON);
		inline			void	LockChild_clr	(G_LOCK blVaild = G_LOCK_ON);
		inline			bool32	LockChild_try	(G_LOCK blVaild = G_LOCK_ON);
		
		static	inline	void	LockChild_set	(TNF* tnfNode,G_LOCK blVaild = G_LOCK_ON);
		static	inline	void	LockChild_clr	(TNF* tnfNode,G_LOCK blVaild = G_LOCK_ON);
		static	inline	bool32	LockChild_try	(TNF* tnfNode,G_LOCK blVaild = G_LOCK_ON);
		
		static	inline	void	LockFather_set	(TNF* tnfNode,G_LOCK blVaild = G_LOCK_ON);
		static	inline	void	LockFather_clr	(TNF* tnfNode,G_LOCK blVaild = G_LOCK_ON);
		static	inline	bool32	LockFather_try	(TNF* tnfNode,G_LOCK blVaild = G_LOCK_ON);
	private:
		static	void	UpdateInstert_nolock	(TNF* tInsertNode,TNF* rChainHead,bool32 blUpdatedRNodeID);
		static	void	UpdateHead_nolock		(TNF* tHeadNode,bool32 blUpdatedRNodeID);
	public:
		static	TNF*	InsertAfter_nolock		(TNF* tOpNode,TNF* tInsertNode,bool32 blUpdatedRNodeID = G_TRUE);
		static	TNF*	InsertBefore_nolock		(TNF* tOpNode,TNF* tInsertNode,bool32 blUpdatedRNodeID = G_TRUE);
		static	TNF*	AddSubNode_nolock		(TNF* tOpNode,TNF* tInsertNode);
		static	TNF*	Remove_nolock			(TNF* tFirstNode,TNF* tEndNode = nullptr,bool32 blUpdatedRNodeID = G_TRUE);
		static	void	MoveUp_nolock			(TNF* tFirstNode,TNF* tEndNode = nullptr);
		static	void	MoveDown_nolock			(TNF* tFirstNode,TNF* tEndNode = nullptr);
		static	void	MoveAfter_nolock		(TNF* tFirstNode,TNF* tEndNode = nullptr,TNF* tAfterNode = nullptr);
		static	TNF*	BreakChild_nolock		(TNF* tnfNode);
		static	TNF*	BreakNext_nolock		(TNF* tnfNode);
		
		static	TNF*	InsertAfter				(TNF* tOpNode,TNF* tInsertNode,bool32 blUpdatedRNodeID = G_TRUE);
		static	TNF*	InsertBefore			(TNF* tOpNode,TNF* tInsertNode,bool32 blUpdatedRNodeID = G_TRUE);
		static	TNF*	AddSubNode				(TNF* tOpNode,TNF* tInsertNode);
		static	TNF*	Remove					(TNF* tFirstNode,TNF* tEndNode = nullptr,bool32 blUpdatedRNodeID = G_TRUE);
		static	void	MoveUp					(TNF* tFirstNode,TNF* tEndNode = nullptr);
		static	void	MoveDown				(TNF* tFirstNode,TNF* tEndNode = nullptr);
		static	void	MoveAfter				(TNF* tFirstNode,TNF* tEndNode = nullptr,TNF* tAfterNode = nullptr);
		static	TNF*	BreakChild				(TNF* tnfNode);
		static	TNF*	BreakNext				(TNF* tnfNode);
	public:
		inline			TNF&	Add				(TNF& tnfNode);
		inline	virtual	TNF*	AddNode			(TNF* tnfNode);
		inline	virtual	void	RemoveSelf		(void);
	public:
		template <typename T_TNF> inline friend	T_TNF& operator < (T_TNF& _tnf,TNF& tnfNode){
			_tnf.Add(tnfNode);
			return(_tnf);
		};
	
		static	TNF*	FindInLChildRChainByDRNodeID	(TNF* tnNode,uint32 tDRNodeID);
};
//------------------------------------------------------------------------------------------//
class	TREE_NODE_FRAME_POOL;
typedef TREE_NODE_FRAME_POOL TNFP;
class TREE_NODE_FRAME_POOL : public TNF{
	public:
				 TREE_NODE_FRAME_POOL(void);
		virtual	~TREE_NODE_FRAME_POOL(void);
	public:
		STDSTR	fatherName;
		STDSTR	selfName;
	public:
		inline	virtual void	SetFatherName		(const STDSTR& strName);
		inline	virtual void	SetSelfName			(const STDSTR& strName);
		static	inline	STDSTR	GetFullName			(const TNFP* tnfpNode);
				virtual TNFP*	SetSubNodeFatherName(TNFP* tnfpNode);
	private:
		TNFP	*cgTrash;
	private:
				inline	TNFP*	CreateTrash			(void);
				inline	void	DestroyTrash		(void);
	public:
		static			void	DestroyTree			(TNF* tnfNode);
		static			void	DestroySubTree		(TNF* tnfNode);
	public:
		virtual	inline	TNFP*	GetTrash			(void);
		static			void	CleanTrash			(TNFP* trashOwner);
		static	inline	TNF*	CleanChild			(TNFP* trashOwner,TNF* tnfNode);
		static			void	MoveNodesToTrash	(TNFP* trashOwner,TNF* tFirstNode,TNF* tEndNode = nullptr);
	
						TNF*	GetNewNode			(void);
		inline	virtual	TNF*	CreateNode			(void);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
class TREE_NODE : public TNFP{
	protected:
		enum		{RFLAG_C = 3, RFLAG_S = TNFP::RFLAG_S + TNFP::RFLAG_C};
	private:
		enum		{TN_blEnable = RFLAG_CREATE(0),TN_blSelected = RFLAG_CREATE(1),TN_blUpdate = RFLAG_CREATE(2),};
	public:
		enum CNType	{CN_None = 0,CN_M,CN_S};
	public:
				 TREE_NODE(void);
		virtual	~TREE_NODE(void){;};
	private:
		CNType			cgCNType;
		TREE_NODE		*cgCoupleNode;
	public:
		static	inline 	CNType		GetCNType				(const TREE_NODE* tnNode);
		static	inline 	TREE_NODE*	GetCoupleNode			(const TREE_NODE* tnNode);
	
						void		LinkCoupleNode			(TREE_NODE* slaveNode);
						void		LinkCoupleNode_nolock	(TREE_NODE* slaveNode);
						TREE_NODE*	UnlinkCoupleNode		(void);
	public:
		inline	virtual	void		Enable					(void);
		inline	virtual	void		Disable					(void);
		inline			bool32		CheckEnable				(void)const;
		
		inline	virtual	void		SetblSelected			(void);
		inline	virtual	void		ClrblSelected			(void);
		inline			bool32		CheckSelected			(void)const;
		
		inline	virtual	void		SetblUpdate				(void);
		inline	virtual	void		ClrblUpdate				(void);
		inline			bool32		CheckUpdate				(void)const;
};
typedef TREE_NODE::CNType CNType;
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#include "DS_Tree.hpp"
#endif /* DS_Tree_h */
#endif /* DS_Tree_h */
