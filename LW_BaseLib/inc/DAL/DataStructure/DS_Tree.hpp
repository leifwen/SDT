//
//  DS_Tree.hpp
//  SDT
//
//  Created by Leif Wen on 18/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#ifndef DS_Tree_hpp
#define DS_Tree_hpp
//------------------------------------------------------------------------------------------//
#include "DS_Tree.h"
#ifdef DS_Tree_h
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#define	TREE_NextChain_Traversal_LINE_nolock(NODE_TYPE,NODE_Start,NODE_LINE)\
{\
	NODE_TYPE *_opNode,*_nextNode;\
	_opNode = static_cast<NODE_TYPE*>(NODE_Start);\
	while(_opNode != nullptr){\
		_nextNode = static_cast<NODE_TYPE*>(TNF::GetNext(_opNode));\
		{NODE_LINE;};\
		_opNode = _nextNode;\
	};\
};
//------------------------------------------------------------------------------------------//
#define	TREE_DownChain_Traversal_LINE_nolock(NODE_TYPE,NODE_Father,NODE_LINE)\
	TREE_NextChain_Traversal_LINE_nolock(NODE_TYPE,TNF::GetDown(NODE_Father),NODE_LINE)
//------------------------------------------------------------------------------------------//
#define	TREE_NextChain_Traversal_LINE(NODE_TYPE,NODE_Start,NODE_LINE)\
{\
	NODE_TYPE *_opNode,*_nextNode,*_upNode;\
	TNF::Traversal_set(NODE_Start);\
	_upNode = static_cast<NODE_TYPE*>(TNF::GetUp(NODE_Start));\
	TNF::Traversal_set(_upNode);\
	_opNode = static_cast<NODE_TYPE*>(NODE_Start);\
	while(_opNode != nullptr){\
		_nextNode = static_cast<NODE_TYPE*>(TNF::GetNext(_opNode));\
		{NODE_LINE;};\
		_opNode = _nextNode;\
	};\
	TNF::Traversal_clr(_upNode);\
	TNF::Traversal_clr(NODE_Start);\
};
//------------------------------------------------------------------------------------------//
#define	TREE_DownChain_Traversal_LINE(NODE_TYPE,NODE_Father,NODE_LINE)\
	TNF::Traversal_set(NODE_Father);\
	TREE_NextChain_Traversal_LINE_nolock(NODE_TYPE,TNF::GetDown(NODE_Father),NODE_LINE);\
	TNF::Traversal_clr(NODE_Father);\
//------------------------------------------------------------------------------------------//
#define	TREE_NextChain_Find_nolock(NODE_TYPE,NODE_Start,Node_Ret,NODE_IF)\
	Node_Ret = nullptr;\
	TREE_NextChain_Traversal_LINE_nolock(NODE_TYPE,NODE_Start,if (NODE_IF){Node_Ret = _opNode;break;});
//------------------------------------------------------------------------------------------//
#define	TREE_NextChain_Find(NODE_TYPE,NODE_Start,Node_Ret,NODE_IF)\
	Node_Ret = nullptr;\
	TREE_NextChain_Traversal_LINE(NODE_TYPE,NODE_Start,if (NODE_IF){Node_Ret = _opNode;break;});
//------------------------------------------------------------------------------------------//
#define	TREE_DownChain_Find(NODE_TYPE,NODE_Father,Node_Ret,NODE_IF)\
	TNF::Traversal_set(NODE_Father);\
	TREE_NextChain_Find_nolock(NODE_TYPE,TNF::GetDown(NODE_Father),Node_Ret,NODE_IF);\
	TNF::Traversal_clr(NODE_Father);
//------------------------------------------------------------------------------------------//
#define	TREE_DownChain_Traversal(NODE_TYPE,NODE_FUN) TREE_DownChain_Traversal_LINE(NODE_TYPE,this,_opNode->NODE_FUN)
//------------------------------------------------------------------------------------------//
inline void		TREE_FLAG::SetblUpdate		(TREE_FLAG* tf)		{if (tf != nullptr) tf->SetSFlag(TN_blUpdate);};
inline void		TREE_FLAG::ClrblUpdate		(TREE_FLAG* tf)		{if (tf != nullptr) tf->ClrSFlag(TN_blUpdate);};
inline bool32	TREE_FLAG::IsUpdate			(TREE_FLAG* tf)		{return((tf != nullptr) ? tf->CheckSFlag(TN_blUpdate):0);};
inline void		TREE_FLAG::Enable			(TREE_FLAG* tf)		{if (tf != nullptr) tf->SetSFlag(TN_blEnable);};
inline void		TREE_FLAG::Disable			(TREE_FLAG* tf)		{if (tf != nullptr) tf->ClrSFlag(TN_blEnable);};
inline int32	TREE_FLAG::IsEnable			(TREE_FLAG* tf)		{return((tf != nullptr) ? tf->CheckSFlag(TN_blEnable):0);};
//------------------------------------------------------------------------------------------//
inline uint32	TNF::GetUniqueID	(const TNF* tnfNode)	{return((tnfNode != nullptr) ? tnfNode->cgUniqueID.load():0);};
inline uint32	TNF::GetDRNodeID	(const TNF* tnfNode)	{return((tnfNode != nullptr) ? tnfNode->cgDRNodeID.load():0);};
//------------------------------------------------------------------------------------------//
inline TNF*		TNF::GetHead		(const TNF* tnfNode)	{return((tnfNode != nullptr) ? tnfNode->cgHead:	nullptr);};
inline TNF*		TNF::GetTail		(const TNF* tnfNode)	{return((tnfNode != nullptr) ? tnfNode->cgTail:	nullptr);};
inline TNF*		TNF::GetPrior		(const TNF* tnfNode)	{return((tnfNode != nullptr) ? tnfNode->cgPrior:nullptr);};
inline TNF*		TNF::GetNext		(const TNF* tnfNode)	{return((tnfNode != nullptr) ? tnfNode->cgNext:	nullptr);};
inline TNF*		TNF::GetUp			(const TNF* tnfNode)	{return((tnfNode != nullptr) ? tnfNode->cgUp:	nullptr);};
inline TNF*		TNF::GetDown		(const TNF* tnfNode)	{return((tnfNode != nullptr) ? tnfNode->cgDown:	nullptr);};
//------------------------------------------------------------------------------------------//
inline	void	TNF::Modify_set		(void){cgNodeLock.W_set();};
inline	void	TNF::Modify_clr		(void){cgNodeLock.W_clr();};
inline	bool32	TNF::Modify_try		(void){return(cgNodeLock.W_try());};

inline	void	TNF::Traversal_set	(void){cgNodeLock.R_set();};
inline	void	TNF::Traversal_clr	(void){cgNodeLock.R_clr();};
inline	bool32	TNF::Traversal_try	(void){return(cgNodeLock.R_try());};
//------------------------------------------------------------------------------------------//
inline void TNF::Modify_set(TNF* tnfNode){
	if (tnfNode != nullptr)
		tnfNode->Modify_set();
};
//------------------------------------------------------------------------------------------//
inline void TNF::Modify_clr(TNF* tnfNode){
	if (tnfNode != nullptr)
		tnfNode->Modify_clr();
};
//------------------------------------------------------------------------------------------//
inline bool32 TNF::Modify_try(TNF* tnfNode){
	if (tnfNode != nullptr)
		return(tnfNode->Modify_try());
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
inline void TNF::Traversal_set(TNF* tnfNode){
	if (tnfNode != nullptr)
		tnfNode->Traversal_set();
};
//------------------------------------------------------------------------------------------//
inline void TNF::Traversal_clr(TNF* tnfNode){
	if (tnfNode != nullptr)
		tnfNode->Traversal_clr();
};
//------------------------------------------------------------------------------------------//
inline bool32 TNF::Traversal_try(TNF* tnfNode){
	if (tnfNode != nullptr)
		return(tnfNode->Traversal_try());
	return G_TRUE;
};
//------------------------------------------------------------------------------------------//
inline TNF&		TNF::AppendDown		(TNF& tnfNode)			{return(*AppendDownNode(&tnfNode));};
inline TNF*		TNF::AppendDownNode	(TNF* tnfNode)			{return(InsertDownTail(this,tnfNode));};
inline void		TNF::RemoveSelf		(void)					{DetachUpPriorNext(this,this);};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
inline void	TNFP::SetUpName		(const STDSTR& strName)		{fatherName = strName;};;
inline void	TNFP::SetSelfName	(const STDSTR& strName)		{selfName = strName + GetHEXUniqueID(this);};
//------------------------------------------------------------------------------------------//
inline STDSTR TNFP::GetFullName(const TNFP* tnfpNode){
	return((tnfpNode->fatherName.length() == 0) ? (tnfpNode->selfName) : (tnfpNode->fatherName + "." + tnfpNode->selfName));
};
//------------------------------------------------------------------------------------------//
inline TNF* TNFP::CreateNode(void){
	
	return(SetNodeUpName(new TNFP));
};
//------------------------------------------------------------------------------------------//
inline TNFP* TNFP::CreateTrash(void){
	if (cgTrash == nullptr){
		cgTrashLock.Set();
		if (cgTrash == nullptr)
			cgTrash = new TNFP;
		cgTrashLock.Clr();
	}
	return(cgTrash);
};
//------------------------------------------------------------------------------------------//
inline void TNFP::DestroyTrash(void){
	cgTrashLock.Set();
	DestroyTree(cgTrash);
	cgTrash = nullptr;
	cgTrashLock.Clr();
};
//------------------------------------------------------------------------------------------//
inline TNFP* TNFP::GetTrash(void){
	
	return(CreateTrash());
};
//------------------------------------------------------------------------------------------//
inline void TNFP::CleanTrash(TNFP* tTrashOwner){
	if (tTrashOwner == nullptr)
		return;
	DestroyTree(DetachDown(tTrashOwner->cgTrash));
};
//------------------------------------------------------------------------------------------//
inline TNF* TNFP::CleanDownTree(TNFP* trashOwner,TNF* tnfNode){
	TNF *child;
	child = DetachDown(tnfNode);
	MoveNodesToTrash(trashOwner,child, GetTail(GetHead(child)));
	return(tnfNode);
};
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif /* DS_Tree_h */
#endif /* DS_Tree_hpp */
