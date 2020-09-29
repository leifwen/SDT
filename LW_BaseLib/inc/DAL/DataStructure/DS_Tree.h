//
//  DS_Tree.h
//  SDT
//
//  Created by Leif Wen on 18/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#include "BasicDefine.h"
#include "DS_BaseClass.h"
#include "DS_Lock.h"
//------------------------------------------------------------------------------------------//
#if defined DS_Lock_h
#ifndef DS_Tree_h
#define DS_Tree_h
#ifdef DS_Tree_h
//------------------------------------------------------------------------------------------//
class TREE_FLAG : public BASE_FLAG{
	protected:
		enum	{RFLAG_C = 2, RFLAG_S = BASE_FLAG::RFLAG_S + BASE_FLAG::RFLAG_C};
	private:
		enum	{TN_blEnable = RFLAG_CREATE(0),TN_blUpdate = RFLAG_CREATE(1)};
	public:
				 TREE_FLAG(void) : BASE_FLAG(){;};
		virtual	~TREE_FLAG(void){;};
	public:
		static	inline	void		SetblUpdate	(TREE_FLAG* tf);
		static	inline	void		ClrblUpdate	(TREE_FLAG* tf);
		static	inline	bool32		IsUpdate	(TREE_FLAG* tf);
		static	inline	void		Enable		(TREE_FLAG* tf);
		static	inline	void		Disable		(TREE_FLAG* tf);
		static	inline	bool32		IsEnable	(TREE_FLAG* tf);
};
//------------------------------------------------------------------------------------------//
class	TREE_NODE_FRAME;
typedef TREE_NODE_FRAME TNF;
class TREE_NODE_FRAME : public TREE_FLAG{
	/*
		FN
		||\
		|| \
		||  -<------<-------<-------<-------<----
		|| /	^		^		^		^		^
		||/		|		|		|		|		|
		CH<---->CN<---->N<----->N<----->N<----->T
		  \		||								|
		   ->------->------->------->------->----
		 		||
		 		||
		 		CH<----->N<----->N<----->N<----->T
	*/
	public:
				 TREE_NODE_FRAME(void);
		virtual	~TREE_NODE_FRAME(void);
	private:
		static	std::atomic_uint	cgNextUniqueID;
				std::atomic_uint	cgUniqueID;		//no changed,be setted when create.
				std::atomic_uint	cgNextDRNodeID;
				std::atomic_uint	cgDRNodeID;		//Right dynamic ID
	public:
		static	inline	uint32	GetUniqueID		(const TNF* tnfNode);
		static			STDSTR	GetHEXUniqueID	(const TNF* tnfNode);
		static	inline	uint32	GetDRNodeID		(const TNF* tnfNode);
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
		DS_RWLock		cgNodeLock;
	protected:
				inline	void	Modify_set		(void);
				inline	void	Modify_clr		(void);
				inline	bool32	Modify_try		(void);
		static	inline	void	Modify_set		(TNF* tnfNode);
		static	inline	void	Modify_clr		(TNF* tnfNode);
		static	inline	bool32	Modify_try		(TNF* tnfNode);
	public:
				inline	void	Traversal_set	(void);
				inline	void	Traversal_clr	(void);
				inline	bool32	Traversal_try	(void);
		static	inline	void	Traversal_set	(TNF* tnfNode);
		static	inline	void	Traversal_clr	(TNF* tnfNode);
		static	inline	bool32	Traversal_try	(TNF* tnfNode);
	private:
		static	void	UpdateInstert_nolock	(TNF* tInsertNode,TNF* rChainHead,bool32 blUpdatedRNodeID);
		static	void	UpdateHead_nolock		(TNF* tHeadNode,bool32 blUpdatedRNodeID);
	public:
		static	TNF*	InsertAfter_nolock		(TNF* tOpNode,TNF* tInsertNode,bool32 blUpdatedRNodeID = G_TRUE);
		static	TNF*	InsertBefore_nolock		(TNF* tOpNode,TNF* tInsertNode,bool32 blUpdatedRNodeID = G_TRUE);
		static	TNF*	InsertDownTail_nolock	(TNF* tOpNode,TNF* tInsertNode);
		static	void	MovePrior_nolock		(TNF* tFirstNode,TNF* tEndNode);//tEndNode = nullptr means tEndNode = tail
		static	void	MoveNext_nolock			(TNF* tFirstNode,TNF* tEndNode);//tEndNode = nullptr means tEndNode = tail
		static	void	MoveAfter_nolock		(TNF* tFirstNode,TNF* tEndNode,TNF* tAfterNode);//tEndNode/tAfterNode = nullptr means tEndNode/tAfterNode = tail
		static	TNF*	DetachUpPriorNext_nolock(TNF* tFirstNode,TNF* tEndNode,bool32 blUpdatedRNodeID = G_TRUE);//tEndNode = nullptr means tEndNode = tail
		static	TNF*	DetachDown_nolock		(TNF* tnfNode);
		static	TNF*	DetachNext_nolock		(TNF* tnfNode);
		
		static	TNF*	InsertAfter				(TNF* tOpNode,TNF* tInsertNode,bool32 blUpdatedRNodeID = G_TRUE);
		static	TNF*	InsertBefore			(TNF* tOpNode,TNF* tInsertNode,bool32 blUpdatedRNodeID = G_TRUE);
		static	TNF*	InsertDownTail			(TNF* tOpNode,TNF* tInsertNode);
		static	void	MovePrior				(TNF* tFirstNode,TNF* tEndNode);//tEndNode = nullptr means tEndNode = tail
		static	void	MoveNext				(TNF* tFirstNode,TNF* tEndNode);//tEndNode = nullptr means tEndNode = tail
		static	void	MoveAfter				(TNF* tFirstNode,TNF* tEndNode,TNF* tAfterNode);//tEndNode/tAfterNode = nullptr means tEndNode/tAfterNode = tail
		static	TNF*	DetachUpPriorNext		(TNF* tFirstNode,TNF* tEndNode,bool32 blUpdatedRNodeID = G_TRUE);//tEndNode = nullptr means tEndNode = tail
		static	TNF*	DetachDown				(TNF* tnfNode);
		static	TNF*	DetachNext 				(TNF* tnfNode);
	public:
		inline			TNF&	AppendDown		(TNF& tnfNode);
		inline	virtual	TNF*	AppendDownNode	(TNF* tnfNode);
		inline	virtual	void	RemoveSelf		(void);
	public:
		template <typename T_TNF> inline friend	T_TNF& operator < (T_TNF& _tnf,TNF& tnfNode){
			_tnf.AppendDown(tnfNode);
			return(_tnf);
		};
	
		static	TNF*	FindInDownChainByDRNodeID(TNF* tnNode,uint32 tDRNodeID);
		static	TNF*	FindInDownChainByUniqueID(TNF* tnNode,uint32 tUniqueID);
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
		inline	virtual void	SetUpName			(const STDSTR& strName);
		inline	virtual void	SetSelfName			(const STDSTR& strName);
		static	inline	STDSTR	GetFullName			(const TNFP* tnfpNode);
				virtual TNFP*	SetNodeUpName		(TNFP* tnfpNode);
		static	TNFP*	FindInDownChainBySelfName	(TNFP* tnfpNode,const STDSTR& strName);
	private:
		DS_SpinLock	cgTrashLock;
		TNFP*		cgTrash;
	private:
				inline	TNFP*	CreateTrash			(void);
				inline	void	DestroyTrash		(void);
	public:
		static			void	DestroyTree			(TNF* tnfNode);
		static			void	DestroyDownNextTree	(TNF* tnfNode);
	public:
		virtual	inline	TNFP*	GetTrash			(void);
		static			void	CleanTrash			(TNFP* trashOwner);
		static	inline	TNF*	CleanDownTree		(TNFP* trashOwner,TNF* tnfNode);
		static			void	MoveNodesToTrash	(TNFP* trashOwner,TNF* tFirstNode,TNF* tEndNode);//tEndNode = nullptr means tEndNode = tail
	public:
						TNF*	GetNewNode			(void);
		inline	virtual	TNF*	CreateNode			(void);
};
//------------------------------------------------------------------------------------------//
#include "DS_Tree.hpp"
#endif /* DS_Tree_h */
#endif /* DS_Tree_h */
#endif
