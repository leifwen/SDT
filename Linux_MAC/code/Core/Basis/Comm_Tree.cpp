/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: Comm_Tree.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2015.09.10
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Comm_Tree.h"
//------------------------------------------------------------------------------------------//
#ifdef Comm_TreeH
//------------------------------------------------------------------------------------------//
#include "Comm_Convert.h"
//------------------------------------------------------------------------------------------//
std::atomic_uint TREE_NODE_FRAME::cgID_NextNode = ATOMIC_VAR_INIT(0);
//------------------------------------------------------------------------------------------//
TREE_NODE_FRAME::TREE_NODE_FRAME(void) : BASIC_CFLAG(){
	cgID_NextNode ++;
	nodeID = cgID_NextNode.load();
	
	cgID_NextDRNode = 0;
	dRNodeID = 0;
	
	cgHead = this;
	cgTail = this;
	cgPrior = nullptr;
	cgNext = nullptr;
	cgUp = nullptr;
	cgDown = nullptr;
}
//------------------------------------------------------------------------------------------//
void TREE_NODE_FRAME::Spin_Child_set(TREE_NODE_FRAME *tTreeNode,G_LOCK blVaild){
	if (tTreeNode != nullptr)
		tTreeNode->Spin_Child_set(blVaild);
};
//------------------------------------------------------------------------------------------//
void TREE_NODE_FRAME::Spin_Child_clr(TREE_NODE_FRAME *tTreeNode,G_LOCK blVaild){
	if (tTreeNode != nullptr)
		tTreeNode->Spin_Child_clr(blVaild);
};
//------------------------------------------------------------------------------------------//
int32 TREE_NODE_FRAME::Spin_Child_try(TREE_NODE_FRAME *tTreeNode,G_LOCK blVaild){
	if (tTreeNode != nullptr)
		return(tTreeNode->Spin_Child_try(blVaild));
	return 1;
};
//------------------------------------------------------------------------------------------//
void TREE_NODE_FRAME::Spin_Father_set(TREE_NODE_FRAME *tTreeNode,G_LOCK blVaild){
	if (tTreeNode != nullptr)
		Spin_Child_set(tTreeNode->cgUp,blVaild);
};
//------------------------------------------------------------------------------------------//
void TREE_NODE_FRAME::Spin_Father_clr(TREE_NODE_FRAME *tTreeNode,G_LOCK blVaild){
	if (tTreeNode != nullptr)
		Spin_Child_clr(tTreeNode->cgUp,blVaild);
};
//------------------------------------------------------------------------------------------//
int32 TREE_NODE_FRAME::Spin_Father_try(TREE_NODE_FRAME *tTreeNode,G_LOCK blVaild){
	if (tTreeNode != nullptr)
		Spin_Child_try(tTreeNode->cgUp,blVaild);
	return 1;
};
//------------------------------------------------------------------------------------------//
void TREE_NODE_FRAME::UpdateInstert_nolock(TREE_NODE_FRAME *tInsertNode,TREE_NODE_FRAME *tHeadNode,int32 blUpdatedRNodeID){
	TREE_NODE_FRAME *loopNode;
	
	if (blUpdatedRNodeID != 0){
		tHeadNode->cgID_NextDRNode ++;
		tInsertNode->dRNodeID.store(tHeadNode->cgID_NextDRNode.load());
	}
	tInsertNode->cgHead = tHeadNode;
	tInsertNode->cgTail = nullptr;
	tInsertNode->cgUp = tHeadNode->cgUp;
	
	loopNode = tInsertNode->cgNext;
	while(loopNode != nullptr){
		if (blUpdatedRNodeID != 0){
			tHeadNode->cgID_NextDRNode ++;
			loopNode->dRNodeID.store(tHeadNode->cgID_NextDRNode.load());
		}
		loopNode->cgHead = tHeadNode;
		loopNode->cgTail = nullptr;
		loopNode->cgUp = tHeadNode->cgUp;
		loopNode = loopNode->cgNext;
	}
}
//------------------------------------------------------------------------------------------//
void TREE_NODE_FRAME::UpdateHead_nolock(TREE_NODE_FRAME *tHeadNode,int32 blUpdatedRNodeID){
	TREE_NODE_FRAME *loopNode;
	
	if (blUpdatedRNodeID != 0){
		tHeadNode->dRNodeID.store(0);
		tHeadNode->cgID_NextDRNode.store(0);
	}
	tHeadNode->cgHead = tHeadNode;
	tHeadNode->cgTail = tHeadNode;
	tHeadNode->cgPrior = nullptr;
	
	loopNode = tHeadNode->cgNext;
	while(loopNode != nullptr){
		if (blUpdatedRNodeID != 0){
			tHeadNode->cgID_NextDRNode ++;
			loopNode->dRNodeID.store(tHeadNode->cgID_NextDRNode.load());
		}
		loopNode->cgHead = tHeadNode;
		loopNode->cgTail = nullptr;
		loopNode->cgUp = tHeadNode->cgUp;
		tHeadNode->cgTail = loopNode;
		loopNode = loopNode->cgNext;
	}
}
//------------------------------------------------------------------------------------------//
TREE_NODE_FRAME* TREE_NODE_FRAME::InsertAfter(TREE_NODE_FRAME *tOpNode,TREE_NODE_FRAME *tInsertNode,int32 blUpdatedRNodeID){
	TREE_NODE_FRAME *f;
	f = tOpNode->cgUp;
	Spin_Child_set(f);
	InsertAfter_nolock(tOpNode,tInsertNode,blUpdatedRNodeID);
	Spin_Child_clr(f);
	return(tOpNode);
}
//------------------------------------------------------------------------------------------//
TREE_NODE_FRAME* TREE_NODE_FRAME::InsertAfter_nolock(TREE_NODE_FRAME *tOpNode,TREE_NODE_FRAME *tInsertNode,int32 blUpdatedRNodeID){
	TREE_NODE_FRAME	*head,*endNode,*next;
	
	do{
		if ((tOpNode == nullptr) || (tInsertNode == nullptr) || (tOpNode == tInsertNode))
			break;
		if (tInsertNode->cgPrior != nullptr)
			break;
		head = tOpNode->cgHead;
		endNode = tInsertNode->cgTail;
		UpdateInstert_nolock(tInsertNode,head,blUpdatedRNodeID);
		
		next = tOpNode->cgNext;
		tInsertNode->cgPrior = tOpNode;
		endNode->cgNext = next;
		if (next == nullptr){
			head->cgTail = endNode;
		}
		else{
			next->cgPrior = endNode;
		}
		tOpNode->cgNext = tInsertNode;
	}while(0);
	return(tOpNode);
}
//------------------------------------------------------------------------------------------//
TREE_NODE_FRAME* TREE_NODE_FRAME::InsertBefore(TREE_NODE_FRAME *tOpNode,TREE_NODE_FRAME *tInsertNode,int32 blUpdatedRNodeID){
	TREE_NODE_FRAME *f;
	f = tOpNode->cgUp;
	Spin_Child_set(f);
	InsertBefore_nolock(tOpNode,tInsertNode,blUpdatedRNodeID);
	Spin_Child_clr(f);
	return(tOpNode);
}
//------------------------------------------------------------------------------------------//
TREE_NODE_FRAME* TREE_NODE_FRAME::InsertBefore_nolock(TREE_NODE_FRAME *tOpNode,TREE_NODE_FRAME *tInsertNode,int32 blUpdatedRNodeID){
	TREE_NODE_FRAME	*head,*endNode,*prior;
	
	do{
		if ((tOpNode == nullptr) || (tInsertNode == nullptr) || (tOpNode == tInsertNode))
			break;
		if (tInsertNode->cgPrior != nullptr)
			break;
		endNode = tInsertNode->cgTail;
		
		head = tOpNode->cgHead;
		prior = tOpNode->cgPrior;
		if (prior == nullptr){
			if (tOpNode->cgUp != nullptr)
				tOpNode->cgUp->cgDown = tInsertNode;
			tInsertNode->cgUp = tOpNode->cgUp;
			tInsertNode->cgID_NextDRNode.store(head->cgID_NextDRNode.load());
			tInsertNode->cgTail = head->cgTail;
			head = tInsertNode;
		}
		UpdateInstert_nolock(tInsertNode,head,blUpdatedRNodeID);
		
		tInsertNode->cgPrior = prior;
		endNode->cgNext = tOpNode;
		tOpNode->cgPrior = endNode;
		if (prior != nullptr){
			prior->cgNext = tInsertNode;
		}
		else{
			UpdateHead_nolock(head,blUpdatedRNodeID);
		}
	}while(0);
	return(tOpNode);
}
//------------------------------------------------------------------------------------------//
TREE_NODE_FRAME* TREE_NODE_FRAME::AddSubNode(TREE_NODE_FRAME *tFatherNode,TREE_NODE_FRAME *tInsertNode){
	do{
		if (tFatherNode == nullptr)
			break;
		Spin_Child_set(tFatherNode);
		AddSubNode_nolock(tFatherNode,tInsertNode);
		Spin_Child_clr(tFatherNode);
	}while(0);
	return(tFatherNode);
}
//------------------------------------------------------------------------------------------//
TREE_NODE_FRAME* TREE_NODE_FRAME::AddSubNode_nolock(TREE_NODE_FRAME *tFatherNode,TREE_NODE_FRAME *tInsertNode){
	do{
		if ((tFatherNode == nullptr) || (tInsertNode == nullptr) || (tFatherNode == tInsertNode))
			break;
		
		if (tInsertNode->cgUp != nullptr)
			break;
		if (tFatherNode->cgDown == nullptr){
			tFatherNode->cgDown = tInsertNode;
			tInsertNode->cgUp = tFatherNode;
			UpdateHead_nolock(tInsertNode,1);
		}
		else{
			InsertAfter_nolock(tFatherNode->cgDown->cgTail,tInsertNode);
		}
	}while(0);
	return(tFatherNode);
}
//------------------------------------------------------------------------------------------//
TREE_NODE_FRAME *TREE_NODE_FRAME::Remove(TREE_NODE_FRAME *tFirstNode,TREE_NODE_FRAME *tEndNode,int32 blUpdatedRNodeID){
	TREE_NODE_FRAME *f;
	f = tFirstNode->cgUp;
	Spin_Child_set(f);
	Remove_nolock(tFirstNode,tEndNode,blUpdatedRNodeID);
	Spin_Child_clr(f);
	return(tFirstNode);
}
//------------------------------------------------------------------------------------------//
TREE_NODE_FRAME *TREE_NODE_FRAME::Remove_nolock(TREE_NODE_FRAME *tFirstNode,TREE_NODE_FRAME *tEndNode,int32 blUpdatedRNodeID){
	TREE_NODE_FRAME *head,*next,*prior,*upNode;
	do{
		if (tFirstNode == nullptr)
			break;
		if (tEndNode == nullptr)
			tEndNode = tFirstNode;
		
		prior = tFirstNode->cgPrior;
		if (prior != nullptr){//tFirstNode is not head
			next = tEndNode->cgNext;
			head = tFirstNode->cgHead;
			if (next != nullptr){
				next->cgPrior = prior;
			}
			else{
				head->cgTail = prior;
			}
			prior->cgNext = next;
		}
		else{
			next = tEndNode->cgNext;
			upNode = tFirstNode->cgUp;
			if (upNode != nullptr)
				upNode->cgDown = next;
			if (next != nullptr){
				next->cgID_NextDRNode.store(tFirstNode->cgID_NextDRNode.load());
				UpdateHead_nolock(next,0);
			}
		}
		tEndNode->cgNext = nullptr;
		tFirstNode->cgUp = nullptr;
		UpdateHead_nolock(tFirstNode,blUpdatedRNodeID);
	}while(0);
	return(tFirstNode);
}
//------------------------------------------------------------------------------------------//
void TREE_NODE_FRAME::MoveUp(TREE_NODE_FRAME *tFirstNode,TREE_NODE_FRAME *tEndNode){
	TREE_NODE_FRAME *f;
	f = tFirstNode->cgUp;
	Spin_Child_set(f);
	MoveUp_nolock(tFirstNode,tEndNode);
	Spin_Child_clr(f);
}
//------------------------------------------------------------------------------------------//
void TREE_NODE_FRAME::MoveUp_nolock(TREE_NODE_FRAME *tFirstNode,TREE_NODE_FRAME *tEndNode){
	TREE_NODE_FRAME	*prior;
	do{
		if (tFirstNode == nullptr)
			break;
		if (tEndNode == nullptr)
			tEndNode = tFirstNode;
		prior = tFirstNode->cgPrior;
		if (prior == nullptr)
			break;
		Remove_nolock(tFirstNode,tEndNode,0);
		InsertBefore_nolock(prior,tFirstNode,0);
	}while(0);
}
//------------------------------------------------------------------------------------------//
void TREE_NODE_FRAME::MoveDown(TREE_NODE_FRAME *tFirstNode,TREE_NODE_FRAME *tEndNode){
	TREE_NODE_FRAME *f;
	f = tFirstNode->cgUp;
	Spin_Child_set(f);
	MoveDown_nolock(tFirstNode,tEndNode);
	Spin_Child_clr(f);
}
//------------------------------------------------------------------------------------------//
void TREE_NODE_FRAME::MoveDown_nolock(TREE_NODE_FRAME *tFirstNode,TREE_NODE_FRAME *tEndNode){
	TREE_NODE_FRAME	*next;
	do{
		if (tFirstNode == nullptr)
			break;
		if (tEndNode == nullptr)
			tEndNode = tFirstNode;
		next = tEndNode->cgNext;
		if (next == nullptr)
			break;
		Remove_nolock(tFirstNode,tEndNode,0);
		InsertAfter_nolock(next,tFirstNode,0);
	}while(0);
}
//------------------------------------------------------------------------------------------//
void TREE_NODE_FRAME::MoveAfter(TREE_NODE_FRAME *tFirstNode,TREE_NODE_FRAME *tEndNode,TREE_NODE_FRAME *tAfterNode){
	TREE_NODE_FRAME *f;
	f = tFirstNode->cgUp;
	Spin_Child_set(f);
	MoveAfter_nolock(tFirstNode,tEndNode,tAfterNode);
	Spin_Child_clr(f);
}
//------------------------------------------------------------------------------------------//
void TREE_NODE_FRAME::MoveAfter_nolock(TREE_NODE_FRAME *tFirstNode,TREE_NODE_FRAME *tEndNode,TREE_NODE_FRAME *tAfterNode){
	do{
		if (tFirstNode == nullptr)
			break;
		if (tEndNode == nullptr)
			tEndNode = tFirstNode;
		if (tAfterNode == nullptr)
			tAfterNode = tEndNode->cgNext;
		if (tAfterNode == nullptr)
			break;
		Remove_nolock(tFirstNode,tEndNode,0);
		InsertAfter_nolock(tAfterNode,tFirstNode,0);
	}while(0);
}
//------------------------------------------------------------------------------------------//
TREE_NODE_FRAME* TREE_NODE_FRAME::BreakChild(TREE_NODE_FRAME *tFatherNode){
	TREE_NODE_FRAME	*child;
	child = nullptr;
	do{
		if (tFatherNode == nullptr)
			break;
		Spin_Child_set(tFatherNode);
		child = BreakChild_nolock(tFatherNode);
		Spin_Child_clr(tFatherNode);
	}while(0);
	return(child);
}
//------------------------------------------------------------------------------------------//
TREE_NODE_FRAME* TREE_NODE_FRAME::BreakChild_nolock(TREE_NODE_FRAME *tFatherNode){
	TREE_NODE_FRAME	*child;
	child = nullptr;
	do{
		if (tFatherNode == nullptr)
			break;
		child = tFatherNode->cgDown;
		if (child == nullptr)
			break;
		tFatherNode->cgDown = nullptr;
		child->cgUp = nullptr;
		UpdateHead_nolock(child,1);
	}while(0);
	return(child);
}
//------------------------------------------------------------------------------------------//
TREE_NODE_FRAME* TREE_NODE_FRAME::BreakNext(TREE_NODE_FRAME *tTreeNode){
	TREE_NODE_FRAME	*next;
	TREE_NODE_FRAME *f;
	f = tTreeNode->cgUp;
	Spin_Child_set(f);
	next = BreakNext_nolock(tTreeNode);
	Spin_Child_clr(f);
	return(next);
}
//------------------------------------------------------------------------------------------//
TREE_NODE_FRAME* TREE_NODE_FRAME::BreakNext_nolock(TREE_NODE_FRAME *tTreeNode){
	TREE_NODE_FRAME	*next;
	next = nullptr;
	do{
		if (tTreeNode == nullptr)
			break;
		next = tTreeNode->cgNext;
		if (next == nullptr)
			break;
		Remove(next,tTreeNode->cgHead->cgTail);
	}while(0);
	return(next);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
TREE_NODE_FRAME_POOL::TREE_NODE_FRAME_POOL(void) : TREE_NODE_FRAME(){
	cgTrash = nullptr;
	SetSelfName("TN" + Str_DecToHex(GetNodeID(this)));
}
//------------------------------------------------------------------------------------------//
TREE_NODE_FRAME_POOL* TREE_NODE_FRAME_POOL::SetSubNodeSelfName(TREE_NODE_FRAME_POOL *node){
	if (node != nullptr)
		node->SetSelfName(selfName + "->" + node->selfName + Str_DecToHex(GetNodeID(node)));
	return(node);
};
//------------------------------------------------------------------------------------------//
void TREE_NODE_FRAME_POOL::CreateTrash(TREE_NODE_FRAME_POOL *tTrashOwner){
	if (tTrashOwner == nullptr)
		return;
	if (tTrashOwner->cgTrash == nullptr)
		tTrashOwner->cgTrash = new TREE_NODE_FRAME_POOL;
}
//------------------------------------------------------------------------------------------//
void TREE_NODE_FRAME_POOL::DestroyTrash(TREE_NODE_FRAME_POOL *tTrashOwner){
	if (tTrashOwner == nullptr)
		return;
	DestroyTree(tTrashOwner->cgTrash);
	tTrashOwner->cgTrash = nullptr;
}
//------------------------------------------------------------------------------------------//
void TREE_NODE_FRAME_POOL::CleanTrash(TREE_NODE_FRAME_POOL *tTrashOwner){
	TREE_NODE_FRAME *deltree;
	if (tTrashOwner == nullptr)
		return;
	deltree = BreakChild(tTrashOwner->cgTrash);
	DestroyTree(deltree);
}
//------------------------------------------------------------------------------------------//
void TREE_NODE_FRAME_POOL::CleanChild(TNFP *tTrashOwner){
	if (tTrashOwner == nullptr){
		DestroyTree(BreakChild(this));
	}
	else if (tTrashOwner->cgTrash == nullptr){
		DestroyTree(BreakChild(this));
	}
	else{
		AddSubNode(tTrashOwner->cgTrash,BreakChild(this));
	}
};
//------------------------------------------------------------------------------------------//
TREE_NODE_FRAME* TREE_NODE_FRAME_POOL::Empty(TREE_NODE_FRAME_POOL *tTreeNode){
	if (tTreeNode != nullptr)
		tTreeNode->Empty();
	return (tTreeNode);
};
//------------------------------------------------------------------------------------------//
void TREE_NODE_FRAME_POOL::MoveNodeToTrash(TREE_NODE_FRAME *tTreeNode,TREE_NODE_FRAME_POOL *tTrashOwner){
	if (tTreeNode == nullptr)
		return;
	
	Remove(tTreeNode);
	if (tTrashOwner == nullptr){
		DestroyTree(tTreeNode);
	}
	else if (tTrashOwner->cgTrash == nullptr){
		DestroyTree(tTreeNode);
	}
	else{
		AddSubNode(tTrashOwner->cgTrash,tTreeNode);
	}
}
//------------------------------------------------------------------------------------------//
void TREE_NODE_FRAME_POOL::MoveNodesToTrash(TREE_NODE_FRAME *tFirstNode,TREE_NODE_FRAME *tEndNode,TREE_NODE_FRAME_POOL *tTrashOwner){
	if (tFirstNode == nullptr)
		return;
	
	Remove(tFirstNode,tEndNode);
	if (tTrashOwner == nullptr){
		DestroyTree(tFirstNode);
	}
	else if (tTrashOwner->cgTrash == nullptr){
		DestroyTree(tFirstNode);
	}
	else{
		AddSubNode(tTrashOwner->cgTrash,tFirstNode);
	}
}
//------------------------------------------------------------------------------------------//
void TREE_NODE_FRAME_POOL::DestroyTree(TREE_NODE_FRAME *tTreeNode){
	if (tTreeNode == nullptr)
		return;
	
	DestroyTree(BreakChild(tTreeNode));
	DestroyTree(BreakNext(tTreeNode));
	
	try{
		delete tTreeNode;
	}
	catch(...){}
	tTreeNode = nullptr;
}
//------------------------------------------------------------------------------------------//
void TREE_NODE_FRAME_POOL::DestroySubTree(TREE_NODE_FRAME *tTreeNode){
	if (tTreeNode == nullptr)
		return;
	
	DestroyTree(BreakChild(tTreeNode));
	DestroyTree(BreakNext(tTreeNode));
}
//------------------------------------------------------------------------------------------//
TREE_NODE_FRAME *TREE_NODE_FRAME_POOL::GetNewNodeFromTrash(TREE_NODE_FRAME_POOL *tTrashOwner){
	TREE_NODE_FRAME *nNode;
	if (tTrashOwner == nullptr)
		return(nullptr);
	if (tTrashOwner->cgTrash == nullptr)
		return(tTrashOwner->CreateNode());
	nNode = GetcgDown(tTrashOwner->cgTrash);
	if (nNode == nullptr)
		return(tTrashOwner->CreateNode());
	return(Empty((TNFP*)Remove(GetcgTail(nNode))));
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
void TREE_NODE::Init(void){
	cgCNType = CN_None;
	cgCoupleNode = nullptr;
	Enable();
}
//------------------------------------------------------------------------------------------//
void TREE_NODE::LinkCoupleNode_nolock(TREE_NODE *slaveNode){
	if ((slaveNode != nullptr) && (slaveNode != this)){
		slaveNode->cgCoupleNode = this;
		slaveNode->cgCNType = CN_S;
		cgCoupleNode = slaveNode;
		cgCNType = CN_M;
	}
	else{
		cgCoupleNode = nullptr;
		cgCNType = CN_None;
	}
}
//------------------------------------------------------------------------------------------//
void TREE_NODE::LinkCoupleNode(TREE_NODE *slaveNode){
	if ((slaveNode == nullptr) || (slaveNode == this))
		return;
	
	do{
		Spin_InUse_set();
		if (cgCoupleNode != nullptr){
			if (cgCoupleNode->Spin_InUse_try() ==0){
				Spin_InUse_clr();
				continue;
			}
			cgCoupleNode->cgCoupleNode = nullptr;
			cgCoupleNode->cgCNType = CN_None;
			cgCoupleNode->Spin_InUse_clr();
			cgCoupleNode = nullptr;
			cgCNType = CN_None;
		}
		if (slaveNode->Spin_InUse_try() ==0){
			Spin_InUse_clr();
			continue;
		}
		if (slaveNode->cgCoupleNode != nullptr){
			if (slaveNode->cgCoupleNode->Spin_InUse_try() ==0){
				slaveNode->Spin_InUse_clr();
				Spin_InUse_clr();
				continue;
			}
			slaveNode->cgCoupleNode->cgCoupleNode = nullptr;
			slaveNode->cgCoupleNode->cgCNType = CN_None;
			slaveNode->cgCoupleNode->Spin_InUse_clr();
		}
		slaveNode->cgCoupleNode = this;
		slaveNode->cgCNType = CN_S;
		cgCoupleNode = slaveNode;
		cgCNType = CN_M;
		
		slaveNode->Spin_InUse_clr();
		Spin_InUse_clr();
		break;
	}while(1);
}
//------------------------------------------------------------------------------------------//
TREE_NODE* TREE_NODE::UnlinkCoupleNode(void){
	TREE_NODE *retCPNode = nullptr;
	do{
		Spin_InUse_set();
		if (cgCoupleNode != nullptr){
			if (cgCoupleNode->Spin_InUse_try() ==0){
				Spin_InUse_clr();
				continue;
			}
			retCPNode = cgCoupleNode;
			cgCoupleNode->cgCoupleNode = nullptr;
			cgCoupleNode->cgCNType = CN_None;
			cgCoupleNode->Spin_InUse_clr();
			cgCoupleNode = nullptr;
			cgCNType = CN_None;
		}
		Spin_InUse_clr();
		break;
	}while(1);
	return(retCPNode);
}
//------------------------------------------------------------------------------------------//
TREE_NODE *TREE_NODE::FindInLChildRChainByDRNodeID(TREE_NODE *tTreeNode,uint32 tDRNodeID){
	TREE_NODE	*ret;
	ret = nullptr;
	if (tTreeNode != nullptr){
		TREE_LChildRChain_Traversal_LINE(TREE_NODE,tTreeNode,
			if (GetdRNodeID(operateNode_t) == tDRNodeID){
				ret = operateNode_t;
				break;
			}
		);
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
#endif
