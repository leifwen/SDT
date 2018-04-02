//
//  DS_Tree.hpp
//  SDT
//
//  Created by Leif Wen on 18/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#ifndef DS_Tree_hpp
#define DS_Tree_hpp
//------------------------------------------------------------------------------------------//
#include "DS_Tree.h"
#ifdef DS_Tree_h
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#define	TREE_RChain_Traversal_LINE_nolock(NODE_TYPE,NODE_Start,NODE_LINE)\
	NODE_TYPE *_opNode,*_nextNode;\
	_opNode = static_cast<NODE_TYPE*>(NODE_Start);\
	while(_opNode != nullptr){\
		_nextNode = static_cast<NODE_TYPE*>(TNF::GetNext(_opNode));\
		NODE_LINE;\
		_opNode = _nextNode;\
	};\
//------------------------------------------------------------------------------------------//
#define	TREE_LChildRChain_Traversal_LINE_nolock(NODE_TYPE,NODE_Father,NODE_LINE)\
	TREE_RChain_Traversal_LINE_nolock(NODE_TYPE,TNF::GetDown(NODE_Father),NODE_LINE)
//------------------------------------------------------------------------------------------//
#define	TREE_RChain_Traversal_LINE(NODE_TYPE,NODE_Start,NODE_LINE)\
	NODE_TYPE *_opNode,*_nextNode;\
	TNF::LockFather_set(NODE_Start);\
	_opNode = static_cast<NODE_TYPE*>(NODE_Start);\
	while(_opNode != nullptr){\
		_nextNode = static_cast<NODE_TYPE*>(TNF::GetNext(_opNode));\
		NODE_LINE;\
		_opNode = _nextNode;\
	};\
	TNF::LockFather_clr(NODE_Start);
//------------------------------------------------------------------------------------------//
#define	TREE_LChildRChain_Traversal_LINE(NODE_TYPE,NODE_Father,NODE_LINE)\
	NODE_Father->LockChild_set();\
	TREE_RChain_Traversal_LINE_nolock(NODE_TYPE,TNF::GetDown(NODE_Father),NODE_LINE);\
	NODE_Father->LockChild_clr();\
//------------------------------------------------------------------------------------------//
#define	TREE_RChain_Find_nolock(NODE_TYPE,NODE_Start,Node_Ret,NODE_IF)\
	Node_Ret = nullptr;\
	TREE_RChain_Traversal_LINE_nolock(NODE_TYPE,NODE_Start,if (NODE_IF){Node_Ret = _opNode;break;});
//------------------------------------------------------------------------------------------//
#define	TREE_RChain_Find(NODE_TYPE,NODE_Start,Node_Ret,NODE_IF)\
	Node_Ret = nullptr;\
	TREE_RChain_Traversal_LINE(NODE_TYPE,NODE_Start,if (NODE_IF){Node_Ret = _opNode;break;});
//------------------------------------------------------------------------------------------//
#define	TREE_LChildRChain_Find(NODE_TYPE,NODE_Father,Node_Ret,NODE_IF)\
	NODE_Father->LockChild_set();\
	TREE_RChain_Find_nolock(NODE_TYPE,TNF::GetDown(NODE_Father),Node_Ret,NODE_IF);\
	NODE_Father->LockChild_clr();
//------------------------------------------------------------------------------------------//
#define	TREE_LChildRChain_T(NODE_TYPE,NODE_FUN) TREE_LChildRChain_Traversal_LINE(NODE_TYPE,this,_opNode->NODE_FUN)
//------------------------------------------------------------------------------------------//
inline uint32	TNF::GetNodeID		(const TNF* tnfNode)	{return((tnfNode != nullptr) ? tnfNode->nodeID.load():0);};
inline uint32	TNF::GetdRNodeID	(const TNF* tnfNode)	{return((tnfNode != nullptr) ? tnfNode->dRNodeID.load():0);};
//------------------------------------------------------------------------------------------//
inline TNF*		TNF::GetHead		(const TNF* tnfNode)	{return((tnfNode != nullptr) ? tnfNode->cgHead:	nullptr);};
inline TNF*		TNF::GetTail		(const TNF* tnfNode)	{return((tnfNode != nullptr) ? tnfNode->cgTail:	nullptr);};
inline TNF*		TNF::GetPrior		(const TNF* tnfNode)	{return((tnfNode != nullptr) ? tnfNode->cgPrior:nullptr);};
inline TNF*		TNF::GetNext		(const TNF* tnfNode)	{return((tnfNode != nullptr) ? tnfNode->cgNext:	nullptr);};
inline TNF*		TNF::GetUp			(const TNF* tnfNode)	{return((tnfNode != nullptr) ? tnfNode->cgUp:	nullptr);};
inline TNF*		TNF::GetDown		(const TNF* tnfNode)	{return((tnfNode != nullptr) ? tnfNode->cgDown:	nullptr);};

inline void		TNF::LockChild_set	(G_LOCK blVaild)		{cgChildLock.Lock(blVaild);};
inline void		TNF::LockChild_clr	(G_LOCK blVaild)		{cgChildLock.Unlock(blVaild);};
inline bool32	TNF::LockChild_try	(G_LOCK blVaild)		{return(cgChildLock.TryLock(blVaild));};

inline TNF&		TNF::Add			(TNF& tnfNode)			{return(*AddNode(&tnfNode));};
inline TNF*		TNF::AddNode		(TNF* tnfNode)			{return(AddSubNode(this,tnfNode));};
inline void		TNF::RemoveSelf		(void)					{Remove(this,this);};
//------------------------------------------------------------------------------------------//
inline void TNF::LockChild_set(TNF* tnfNode,G_LOCK blVaild){
	if (tnfNode != nullptr)
		tnfNode->LockChild_set(blVaild);
};
//------------------------------------------------------------------------------------------//
inline void TNF::LockChild_clr(TNF* tnfNode,G_LOCK blVaild){
	if (tnfNode != nullptr)
		tnfNode->LockChild_clr(blVaild);
};
//------------------------------------------------------------------------------------------//
inline bool32 TNF::LockChild_try(TNF* tnfNode,G_LOCK blVaild){
	if (tnfNode != nullptr)
		return(tnfNode->LockChild_try(blVaild));
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
inline void TNF::LockFather_set(TNF* tnfNode,G_LOCK blVaild){
	if (tnfNode != nullptr)
		LockChild_set(tnfNode->cgUp,blVaild);
};
//------------------------------------------------------------------------------------------//
inline void TNF::LockFather_clr(TNF* tnfNode,G_LOCK blVaild){
	if (tnfNode != nullptr)
		LockChild_clr(tnfNode->cgUp,blVaild);
};
//------------------------------------------------------------------------------------------//
inline bool32 TNF::LockFather_try(TNF* tnfNode,G_LOCK blVaild){
	if (tnfNode != nullptr)
		LockChild_try(tnfNode->cgUp,blVaild);
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
inline void		TNFP::SetFatherName	(const STDSTR& strName)		{fatherName = strName;};;
inline void		TNFP::SetSelfName	(const STDSTR& strName)		{selfName = strName + GetHEXID(this);};
//------------------------------------------------------------------------------------------//
inline STDSTR TNFP::GetFullName(const TNFP* tnfpNode){
	return((tnfpNode->fatherName.length() == 0) ? (tnfpNode->selfName) : (tnfpNode->fatherName + "." + tnfpNode->selfName));
}
//------------------------------------------------------------------------------------------//
inline TNF* TNFP::CreateNode(void){
	
	return(SetSubNodeFatherName(new TNFP));
};
//------------------------------------------------------------------------------------------//
inline TNFP* TNFP::CreateTrash(void){
	if (cgTrash == nullptr)
		cgTrash = new TNFP;
	return(cgTrash);
}
//------------------------------------------------------------------------------------------//
inline void TNFP::DestroyTrash(void){
	DestroyTree(cgTrash);
	cgTrash = nullptr;
}
//------------------------------------------------------------------------------------------//
inline TNFP* TNFP::GetTrash(void){
	
	return(CreateTrash());
}
//------------------------------------------------------------------------------------------//
inline void TNFP::CleanTrash(TNFP* tTrashOwner){
	if (tTrashOwner == nullptr)
		return;
	DestroyTree(BreakChild(tTrashOwner->cgTrash));
}
//------------------------------------------------------------------------------------------//
inline TNF* TNFP::CleanChild(TNFP* trashOwner,TNF* tnfNode){
	TNF *child;
	child = BreakChild(tnfNode);
	MoveNodesToTrash(trashOwner,child, GetTail(GetHead(child)));
	return(tnfNode);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
inline CNType		TREE_NODE::GetCNType		(const TREE_NODE* tnNode)	{return((tnNode != nullptr)?tnNode->cgCNType:CN_None);};
inline TREE_NODE*	TREE_NODE::GetCoupleNode	(const TREE_NODE* tnNode)	{return((tnNode != nullptr)?tnNode->cgCoupleNode:nullptr);};

inline void			TREE_NODE::Enable			(void)						{SetSFlag(TN_blEnable);};
inline void			TREE_NODE::Disable			(void)						{ClrSFlag(TN_blEnable);};
inline int32		TREE_NODE::CheckEnable		(void)const					{return(CheckSFlag(TN_blEnable));};

inline void			TREE_NODE::SetblSelected	(void)						{SetSFlag(TN_blSelected);};
inline void			TREE_NODE::ClrblSelected	(void)						{ClrSFlag(TN_blSelected);};
inline int32		TREE_NODE::CheckSelected	(void)const					{return(CheckSFlag(TN_blSelected));};

inline void			TREE_NODE::SetblUpdate		(void)						{SetSFlag(TN_blUpdate);};
inline void			TREE_NODE::ClrblUpdate		(void)						{ClrSFlag(TN_blUpdate);};
inline int32		TREE_NODE::CheckUpdate		(void)const					{return(CheckSFlag(TN_blUpdate));};
//------------------------------------------------------------------------------------------//
#endif /* DS_Tree_h */
#endif /* DS_Tree_hpp */
