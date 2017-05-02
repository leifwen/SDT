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
TREE_NODE_FRAME *TREE_NODE_FRAME_POOL::GetNewNode(TREE_NODE_FRAME_POOL *tTrashOwner){
	TREE_NODE_FRAME *nNode;
	if (tTrashOwner == nullptr)
		return(nullptr);
	if (tTrashOwner->cgTrash == nullptr)
		return(tTrashOwner->CreateNode());
	nNode = GetcgDown(tTrashOwner->cgTrash);
	if (nNode == nullptr)
		return(tTrashOwner->CreateNode());
	return(Remove(GetcgTail(nNode)));
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





































/*
//------------------------------------------------------------------------------------------//
TREE_NODE::TREE_NODE(void) : BASIC_CFLAG(){
	cgID_NextNode ++;
	nodeID = cgID_NextNode.load();
	
	cgID_NextDRNode = 0;
	dRNodeID = 0;
	
	cgTrash = nullptr;
	
	cgblVirtualHead = 1;
	cgVirtualRoot = this;
	cgFather = nullptr;
	cgLChild = nullptr;
	cgRChild = nullptr;
	
	cgCNType = CN_None;
	cgCoupleNode = nullptr;
	Enable();
	SetSelfName("TN" + Str_DecToHex(nodeID));
}
//------------------------------------------------------------------------------------------//
TREE_NODE* TREE_NODE::SetSubNodeSelfName(TREE_NODE *node){
	if (node != nullptr)
		node->SetSelfName(selfName + "->" + node->selfName + Str_DecToHex(GetNodeID(node)));
	return(node);
};
//------------------------------------------------------------------------------------------//
TREE_NODE	*TREE_NODE::GetFather_nolock(TREE_NODE *tTreeNode){
	TREE_NODE	*ret;
	
	if (tTreeNode == nullptr)
		return(nullptr);
	
	ret = tTreeNode->cgFather;
	if (tTreeNode->cgblVirtualHead == 0)
		ret = tTreeNode->cgVirtualRoot->cgFather;
	return(ret);
}
//------------------------------------------------------------------------------------------//
TREE_NODE	*TREE_NODE::GetFather(TREE_NODE *tTreeNode){
	TREE_NODE	*ret,*node;
	
	if (tTreeNode == nullptr)
		return(nullptr);
	
	ret = nullptr;
	tTreeNode->Spin_Link_set();
	ret = tTreeNode->cgFather;
	
	if (tTreeNode->cgblVirtualHead == 0){
		node = tTreeNode->cgVirtualRoot;
		node->Spin_Link_set();
		ret = node->cgFather;
		node->Spin_Link_clr();
	}
	tTreeNode->Spin_Link_clr();
	return(ret);
}
//------------------------------------------------------------------------------------------//
TREE_NODE	*TREE_NODE::GetFirstChild(TREE_NODE *tTreeNode){
	TREE_NODE	*ret;
	
	if (tTreeNode == nullptr)
		return(nullptr);
	
	ret = nullptr;
	tTreeNode->Spin_Link_set();
	ret = tTreeNode->cgLChild;
	tTreeNode->Spin_Link_clr();
	return(ret);
}
//------------------------------------------------------------------------------------------//
TREE_NODE	*TREE_NODE::GetLastChild_nolock(TREE_NODE *tTreeNode){
	TREE_NODE	*ret,*child;
	
	if (tTreeNode == nullptr)
		return(nullptr);
	
	ret = nullptr;
	try{
		child = tTreeNode->cgLChild;
		if (child != nullptr)
			ret = child->cgVirtualRoot;
	}
	catch(...){
		ret = nullptr;
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
TREE_NODE	*TREE_NODE::GetLastChild(TREE_NODE *tTreeNode){
	TREE_NODE	*ret,*child;
	
	if (tTreeNode == nullptr)
		return(nullptr);
	
	ret = nullptr;
	tTreeNode->Spin_Link_set();
	child = tTreeNode->cgLChild;
	if (child != nullptr){
		child->Spin_Link_set();
		ret = child->cgVirtualRoot;
		child->Spin_Link_clr();
	}
	tTreeNode->Spin_Link_clr();
	return(ret);
}
//------------------------------------------------------------------------------------------//
TREE_NODE	*TREE_NODE::GetFirstBrother_nolock(TREE_NODE *tTreeNode){
	TREE_NODE	*ret;
	
	if (tTreeNode == nullptr)
		return(nullptr);
	
	ret = tTreeNode;
	if (tTreeNode->cgblVirtualHead == 0)
		ret = tTreeNode->cgVirtualRoot;
	return(ret);
}
//------------------------------------------------------------------------------------------//
TREE_NODE	*TREE_NODE::GetFirstBrother(TREE_NODE *tTreeNode){
	TREE_NODE	*ret;
	
	if (tTreeNode == nullptr)
		return(nullptr);
	
	ret = nullptr;
	tTreeNode->Spin_Link_set();
	ret = tTreeNode;
	if (tTreeNode->cgblVirtualHead == 0)
		ret = tTreeNode->cgVirtualRoot;
	tTreeNode->Spin_Link_clr();
	return(ret);
}
//------------------------------------------------------------------------------------------//
TREE_NODE	*TREE_NODE::GetLastBrother_nolock(TREE_NODE *tTreeNode){
	TREE_NODE	*ret,*node;
	
	if (tTreeNode == nullptr)
		return(nullptr);
	
	ret = nullptr;
	node = tTreeNode->cgVirtualRoot;
	ret = node;
	if (tTreeNode->cgblVirtualHead == 0)
		ret = node->cgVirtualRoot;
	return(ret);
}
//------------------------------------------------------------------------------------------//
TREE_NODE	*TREE_NODE::GetLastBrother(TREE_NODE *tTreeNode){
	TREE_NODE	*ret,*node;
	
	if (tTreeNode == nullptr)
		return(nullptr);
	
	ret = nullptr;
	tTreeNode->Spin_Link_set();
	node = tTreeNode->cgVirtualRoot;
	ret = node;
	if (tTreeNode->cgblVirtualHead == 0){
		node->Spin_Link_set();
		ret = node->cgVirtualRoot;
		node->Spin_Link_clr();
	}
	tTreeNode->Spin_Link_clr();
	return(ret);
}
//------------------------------------------------------------------------------------------//
TREE_NODE	*TREE_NODE::GetNextBrother(TREE_NODE *tTreeNode){
	TREE_NODE	*ret;
	
	if (tTreeNode == nullptr)
		return(nullptr);
	
	ret = nullptr;
	tTreeNode->Spin_Link_set();
	ret = tTreeNode->cgRChild;
	tTreeNode->Spin_Link_clr();
	return(ret);
}
//------------------------------------------------------------------------------------------//
TREE_NODE	*TREE_NODE::GetPriorBrother_nolock(TREE_NODE *tTreeNode){
	TREE_NODE	*ret;
	
	if (tTreeNode == nullptr)
		return(nullptr);
	
	ret = nullptr;
	if (tTreeNode->cgblVirtualHead == 0)
		ret = tTreeNode->cgFather;
	return(ret);
}
//------------------------------------------------------------------------------------------//
TREE_NODE	*TREE_NODE::GetPriorBrother(TREE_NODE *tTreeNode){
	TREE_NODE	*ret;
	
	if (tTreeNode == nullptr)
		return(nullptr);
	
	ret = nullptr;
	tTreeNode->Spin_Link_set();
	if (tTreeNode->cgblVirtualHead == 0)
		ret = tTreeNode->cgFather;
	tTreeNode->Spin_Link_clr();
	return(ret);
}
//------------------------------------------------------------------------------------------//
void TREE_NODE::UpdateSubInstertTree_nolock(TREE_NODE *tInsertNode,TREE_NODE *tVirtualRoot){
	TREE_NODE *loopNode,*opNode;
	
	tVirtualRoot->cgID_NextDRNode ++;
	tInsertNode->cgblVirtualHead = 0;
	tInsertNode->dRNodeID.store(tVirtualRoot->cgID_NextDRNode.load());
	tInsertNode->cgVirtualRoot = tVirtualRoot;
	
	loopNode = tInsertNode->cgRChild;
	while(loopNode != nullptr){
		opNode = loopNode;
		opNode->Spin_Link_set();
		
		tVirtualRoot->cgID_NextDRNode ++;
		opNode->cgblVirtualHead = 0;
		opNode->dRNodeID.store(tVirtualRoot->cgID_NextDRNode.load());
		opNode->cgVirtualRoot = tVirtualRoot;
		
		loopNode = opNode->cgRChild;
		opNode->Spin_Link_clr();
	}
}
//------------------------------------------------------------------------------------------//
int32 TREE_NODE::InsertRChild(TREE_NODE *tFatherNode,TREE_NODE *tInsertNode){
	TREE_NODE	*rootNode,*endNode,*rChild;
	int32 blok;
	
	if ((tFatherNode == nullptr) || (tInsertNode == nullptr) || (tFatherNode == tInsertNode))
		return 0;
	
	blok = 0;
	
	tInsertNode->Spin_Link_set();
	if (tInsertNode->cgFather == nullptr){
		do{
			tFatherNode->Spin_Link_set();
			if (tFatherNode->cgblVirtualHead != 0){
				rootNode = tFatherNode;
			}
			else{
				rootNode = tFatherNode->cgVirtualRoot;
				if (rootNode->Spin_Link_try() == 0){
					tFatherNode->Spin_Link_clr();
					continue;
				}
			}
			
			endNode = tInsertNode->cgVirtualRoot;
			UpdateSubInstertTree_nolock(tInsertNode,rootNode);
			
			{//insert
				tInsertNode->cgFather = tFatherNode;
				rChild = tFatherNode->cgRChild;
				if (rChild == nullptr){
					rootNode->cgVirtualRoot = endNode;
				}
				else{
					if (endNode != tInsertNode)
						endNode->Spin_Link_set();
					endNode->cgRChild = rChild;
					if (endNode != tInsertNode)
						endNode->Spin_Link_clr();
					
					rChild->Spin_Link_set();
					rChild->cgFather = endNode;
					rChild->Spin_Link_clr();
				}
				tFatherNode->cgRChild = tInsertNode;
				blok = 1;
			}
			tFatherNode->Spin_Link_clr();
			break;
		}while(1);
		if (tFatherNode->cgblVirtualHead == 0)
			rootNode->Spin_Link_clr();
	}
	tInsertNode->Spin_Link_clr();
	return(blok);
}
//------------------------------------------------------------------------------------------//
int32 TREE_NODE::InsertLChild(TREE_NODE *tFatherNode,TREE_NODE *tInsertNode,G_LOCK blLock){
	TREE_NODE	*lChild,*locateNode,*endNode;
	int32		blok;
	
	if ((tFatherNode == nullptr) || (tInsertNode == nullptr) || (tFatherNode == tInsertNode))
		return 0;
	
	blok = 0;
	lChild = nullptr;
	
	tInsertNode->Spin_Link_set();
	if (tInsertNode->cgFather == nullptr){
		tFatherNode->Spin_Link_set(blLock);
		lChild = tFatherNode->cgLChild;
		
		if (lChild == nullptr){
			tInsertNode->cgFather = tFatherNode;
			tFatherNode->cgLChild = tInsertNode;
		}
		else{
			lChild->Spin_Link_set(blLock);
			locateNode = lChild->cgVirtualRoot;
			endNode = tInsertNode->cgVirtualRoot;
			
			UpdateSubInstertTree_nolock(tInsertNode,lChild);
			
			{//insert
				tInsertNode->cgFather = locateNode;
				lChild->cgVirtualRoot = endNode;
				if (locateNode != lChild)
					locateNode->Spin_Link_set();
				locateNode->cgRChild = tInsertNode;
				if (locateNode != lChild)
					locateNode->Spin_Link_clr();
			}
			lChild->Spin_Link_clr(blLock);
		}
		blok = 1;
		tFatherNode->Spin_Link_clr(blLock);
	}
	tInsertNode->Spin_Link_clr();
	return(blok);
}
//------------------------------------------------------------------------------------------//
void TREE_NODE::UpdateSubRChain_nolock(TREE_NODE *tSubRootNode){
	TREE_NODE *loopNode,*opNode;
	
	tSubRootNode->cgblVirtualHead = 1;
	tSubRootNode->cgID_NextDRNode = 0;
	tSubRootNode->dRNodeID = 0;
	tSubRootNode->cgFather = nullptr;
	opNode = tSubRootNode;
	loopNode = tSubRootNode->cgRChild;
	while(loopNode != nullptr){
		opNode = loopNode;
		opNode->Spin_Link_set();
		
		tSubRootNode->cgID_NextDRNode ++;
		opNode->cgblVirtualHead = 0;
		opNode->dRNodeID.store(tSubRootNode->cgID_NextDRNode.load());
		opNode->cgVirtualRoot = tSubRootNode;
		
		loopNode = opNode->cgRChild;
		opNode->Spin_Link_clr();
	};
	tSubRootNode->cgVirtualRoot = opNode;
}
//------------------------------------------------------------------------------------------//
TREE_NODE *TREE_NODE::BreakRChild(TREE_NODE *tFatherNode){
	TREE_NODE	*rootNode,*rChild;
	
	if (tFatherNode == nullptr)
		return(nullptr);
	
	do{
		tFatherNode->Spin_Link_set();
		rChild = tFatherNode->cgRChild;
		if (rChild != nullptr){
			if (tFatherNode->cgblVirtualHead != 0){
				rootNode = tFatherNode;
			}
			else{
				rootNode = tFatherNode->cgVirtualRoot;
				if (rootNode->Spin_Link_try() == 0){
					tFatherNode->Spin_Link_clr();
					continue;
				}
			}
			
			rootNode->cgVirtualRoot = tFatherNode;
			tFatherNode->cgRChild = nullptr;
			rChild->Spin_Link_set();
			UpdateSubRChain_nolock(rChild);
			rChild->Spin_Link_clr();
			if (tFatherNode->cgblVirtualHead == 0)
				rootNode->Spin_Link_clr();
		}
		tFatherNode->Spin_Link_clr();
		break;
	}while(1);
	return(rChild);
}
//------------------------------------------------------------------------------------------//
TREE_NODE *TREE_NODE::BreakLChild(TREE_NODE *tFatherNode){
	TREE_NODE	*lChild;
	
	if (tFatherNode == nullptr)
		return(nullptr);
	
	lChild = nullptr;
	tFatherNode->Spin_Link_set();
	if (tFatherNode->cgLChild != nullptr){
		lChild = tFatherNode->cgLChild;
		tFatherNode->cgLChild = nullptr;
		lChild->Spin_Link_set();
		lChild->cgFather = nullptr;
		lChild->Spin_Link_clr();
	}
	tFatherNode->Spin_Link_clr();
	
	return(lChild);
}
//------------------------------------------------------------------------------------------//
void TREE_NODE::RemoveFromFather(TREE_NODE *tTreeNode){
	TREE_NODE	*rootNode,*fatherNode;
	
	if (tTreeNode == nullptr)
		return;
	
	do{
		tTreeNode->Spin_Link_set();
		fatherNode = tTreeNode->cgFather;
		if (fatherNode != nullptr){
			if (fatherNode->Spin_Link_try() == 0){
				tTreeNode->Spin_Link_clr();
				continue;
			}
			
			if (tTreeNode->cgblVirtualHead != 0){
				fatherNode->cgLChild = nullptr;
				tTreeNode->cgFather = nullptr;
			}
			else{
				if (fatherNode->cgblVirtualHead != 0){
					rootNode = fatherNode;
				}
				else{
					rootNode = fatherNode->cgVirtualRoot;
					if (rootNode->Spin_Link_try() == 0){
						fatherNode->Spin_Link_clr();
						tTreeNode->Spin_Link_clr();
						continue;
					}
				}
				rootNode->cgVirtualRoot = fatherNode;
				fatherNode->cgRChild = nullptr;
				UpdateSubRChain_nolock(tTreeNode);
				if (fatherNode->cgblVirtualHead == 0)
					rootNode->Spin_Link_clr();
			}
			fatherNode->Spin_Link_clr();
		}
		tTreeNode->Spin_Link_clr();
		break;
	}while(1);
}
//------------------------------------------------------------------------------------------//
TREE_NODE *TREE_NODE::RemoveNodesInRChain(TREE_NODE *tFirstNode,TREE_NODE *tEndNode,G_LOCK blLock){
	TREE_NODE	*retSubRoot,*rootNode,*fatherNode,*priorNode,*nextNode,*opNode,*loopNote;
	
	if (tFirstNode == nullptr)
		return(nullptr);
	
	if (tEndNode == nullptr)
		tEndNode = tFirstNode;
	
	retSubRoot = nullptr;
	
	do{
		tFirstNode->Spin_Link_set();
		
		if (tFirstNode->cgblVirtualHead == 0){
			rootNode = tFirstNode->cgVirtualRoot;
			if (rootNode->Spin_Link_try(blLock) == 0){
				tFirstNode->Spin_Link_clr();
				continue;
			}
			
			if (tEndNode != tFirstNode)
				tEndNode->Spin_Link_set();
			nextNode = tEndNode->cgRChild;
			
			priorNode = tFirstNode->cgFather;
			if (priorNode != rootNode)
				priorNode->Spin_Link_set();
			priorNode->cgRChild = nextNode;
			if (priorNode != rootNode)
				priorNode->Spin_Link_clr();
			
			if (nextNode != nullptr){
				nextNode->Spin_Link_set();
				nextNode->cgFather = priorNode;
				nextNode->Spin_Link_clr();
			}
			else{
				rootNode->cgVirtualRoot = priorNode;
			}
			tEndNode->cgRChild = nullptr;
			if (tEndNode != tFirstNode)
				tEndNode->Spin_Link_clr();
			UpdateSubRChain_nolock(tFirstNode);
			rootNode->Spin_Link_clr(blLock);
		}
		else{
			if (tEndNode != tFirstNode)
				tEndNode->Spin_Link_set();
			nextNode = tEndNode->cgRChild;
			
			fatherNode = tFirstNode->cgFather;
			if (fatherNode != nullptr){
				if (fatherNode->Spin_Link_try(blLock) == 0){
					if (tEndNode != tFirstNode)
						tEndNode->Spin_Link_clr();
					tFirstNode->Spin_Link_clr();
					continue;
				}
				fatherNode->cgLChild = nextNode;
				fatherNode->Spin_Link_clr(blLock);
			}
			
			tFirstNode->cgFather = nullptr;
			tFirstNode->cgVirtualRoot = tEndNode;
			tEndNode->cgRChild = nullptr;
			if (tEndNode != tFirstNode)
				tEndNode->Spin_Link_clr();
			
			if (nextNode != nullptr){
				nextNode->Spin_Link_set();
				if (fatherNode == nullptr){
					UpdateSubRChain_nolock(nextNode);
					retSubRoot = nextNode;
				}
				else{
					nextNode->cgblVirtualHead = 1;
					nextNode->cgID_NextDRNode.store(tFirstNode->cgID_NextDRNode.load());
					nextNode->cgFather = fatherNode;
					opNode = nextNode;
					loopNote = nextNode->cgRChild;
					while(loopNote != nullptr){
						opNode = loopNote;
						opNode->Spin_Link_set();
						opNode->cgblVirtualHead = 0;
						opNode->cgVirtualRoot = nextNode;
						loopNote = opNode->cgRChild;
						opNode->Spin_Link_clr();
					};
					nextNode->cgVirtualRoot = opNode;
				}
				nextNode->Spin_Link_clr();
			}
		}
		tFirstNode->Spin_Link_clr();
		break;
	}while(1);
	return(retSubRoot);
}
//------------------------------------------------------------------------------------------//
void TREE_NODE::MoveNodesUpInRChain(TREE_NODE *tFirstNode,TREE_NODE *tEndNode){
	TREE_NODE	*rootNode,*priorNodefather,*priorNode,*nextNode;
	TREE_NODE	*loopNode,*opNode;
	int32		blret;
	
	if (tFirstNode == nullptr)
		return;
	if (tEndNode == nullptr)
		tEndNode = tFirstNode;

	do{
		blret = 1;
		tFirstNode->Spin_Link_set();
		if (tFirstNode->cgblVirtualHead == 0){
			blret = 0;
			priorNode = tFirstNode->cgFather;
			if (priorNode->Spin_Link_try() > 0){
				do{
					rootNode = tFirstNode->cgVirtualRoot;
					if (rootNode != priorNode){
						if (rootNode->Spin_Link_try() == 0)
							break;
					}
					{
						if (rootNode->cgVirtualRoot == tEndNode)
							rootNode->cgVirtualRoot = priorNode;
						
						if (tEndNode != tFirstNode)
							tEndNode->Spin_Link_set();
						{
							nextNode = tEndNode->cgRChild;
							if (nextNode != nullptr){
								nextNode->Spin_Link_set();
								nextNode->cgFather = priorNode;
								nextNode->Spin_Link_clr();
							}
							
							priorNodefather = priorNode->cgFather;
							priorNode->cgFather = tEndNode;
							priorNode->cgRChild = nextNode;
							tFirstNode->cgFather = priorNodefather;
							tEndNode->cgRChild = priorNode;
						}
						if (tEndNode != tFirstNode)
							tEndNode->Spin_Link_clr();
						
						if (priorNode->cgblVirtualHead == 0){
							if (priorNodefather != rootNode)
								priorNodefather->Spin_Link_set();
							priorNodefather->cgRChild = tFirstNode;
							if (priorNodefather != rootNode)
								priorNodefather->Spin_Link_clr();
						}
						else{
							if (priorNodefather != nullptr){
								priorNodefather->Spin_Link_set();
								priorNodefather->cgLChild = tFirstNode;
								priorNodefather->Spin_Link_clr();
							}
							tFirstNode->cgblVirtualHead = 1;
							tFirstNode->cgID_NextDRNode.store(priorNode->cgID_NextDRNode.load());
							
							opNode = tFirstNode;
							loopNode = tFirstNode->cgRChild;
							while(loopNode != nullptr){
								opNode = loopNode;
								opNode->Spin_Link_set(((opNode == priorNode) || (opNode == rootNode))?G_LOCK_OFF:G_LOCK_ON);
								opNode->cgblVirtualHead = 0;
								opNode->cgVirtualRoot = tFirstNode;
								loopNode = opNode->cgRChild;
								opNode->Spin_Link_clr(((opNode == priorNode) || (opNode == rootNode))?G_LOCK_OFF:G_LOCK_ON);
							};
							tFirstNode->cgVirtualRoot = opNode;
						}
					}
					if (rootNode != priorNode)
						rootNode->Spin_Link_clr();
					blret = 1;
				}while(0);
				priorNode->Spin_Link_clr();
			}
		}
		tFirstNode->Spin_Link_clr();
	}while(blret == 0);
}
//------------------------------------------------------------------------------------------//
void TREE_NODE::MoveNodesDownInRChain(TREE_NODE *tFirstNode,TREE_NODE *tEndNode){
	TREE_NODE	*rootNode,*priorNode,*nextNode,*nextNodeRChild;
	TREE_NODE	*loopNode,*opNode;
	int32		blret;

	if (tFirstNode == nullptr)
		return;
	if (tEndNode == nullptr)
		tEndNode = tFirstNode;
	
	do{
		blret = 1;
		tEndNode->Spin_Link_set();
		nextNode = tEndNode->cgRChild;
		if (nextNode != nullptr){
			blret = 0;
			if (nextNode->Spin_Link_try() > 0){
				do{
					rootNode = nextNode->cgVirtualRoot;
					if (rootNode != tEndNode){
						if (rootNode->Spin_Link_try() == 0)
							break;
					}
					{
						if (rootNode->cgVirtualRoot == nextNode)
							rootNode->cgVirtualRoot = tEndNode;
						
						if ((tFirstNode != tEndNode) && (tFirstNode != rootNode))
							tFirstNode->Spin_Link_set();
						{
							nextNodeRChild = nextNode->cgRChild;
							if (nextNodeRChild != nullptr){
								nextNodeRChild->Spin_Link_set();
								nextNodeRChild->cgFather = tEndNode;
								nextNodeRChild->Spin_Link_clr();
							}
							
							priorNode = tFirstNode->cgFather;
							
							nextNode->cgFather = priorNode;
							nextNode->cgRChild = tFirstNode;
							tFirstNode->cgFather = nextNode;
							tEndNode->cgRChild = nextNodeRChild;
							
							if (tFirstNode->cgblVirtualHead == 0){
								if (priorNode != rootNode)
									priorNode->Spin_Link_set();
								priorNode->cgRChild = nextNode;
								if (priorNode != rootNode)
									priorNode->Spin_Link_clr();
							}
							else{
								if (priorNode != nullptr){
									priorNode->Spin_Link_set();
									priorNode->cgLChild = nextNode;
									priorNode->Spin_Link_clr();
								}
								nextNode->cgblVirtualHead = 1;
								nextNode->cgID_NextDRNode.store(tFirstNode->cgID_NextDRNode.load());
								
								opNode = nextNode;
								loopNode = nextNode->cgRChild;
								while(loopNode != nullptr){
									opNode = loopNode;
									opNode->Spin_Link_set(((opNode == rootNode) || (opNode == tFirstNode) || (opNode == tEndNode))?G_LOCK_OFF:G_LOCK_ON);
									opNode->cgblVirtualHead = 0;
									opNode->cgVirtualRoot = nextNode;
									loopNode = opNode->cgRChild;
									opNode->Spin_Link_clr(((opNode == rootNode) || (opNode == tFirstNode) || (opNode == tEndNode))?G_LOCK_OFF:G_LOCK_ON);
								};
								nextNode->cgVirtualRoot = opNode;
							}
						}
						if ((tFirstNode != tEndNode) && (tFirstNode != rootNode))
							tFirstNode->Spin_Link_clr();
					}
					if (rootNode != tEndNode)
						rootNode->Spin_Link_clr();
					blret = 1;
				}while(0);
				nextNode->Spin_Link_clr();
			}
		}
		tEndNode->Spin_Link_clr();
	}while(blret == 0);
}
//------------------------------------------------------------------------------------------//
void TREE_NODE::MoveNodesAfterInRChain(TREE_NODE *tFirstNode,TREE_NODE *tEndNode,TREE_NODE *tAfterNode){
//	TREE_NODE	*rootNode,*priorNode,tFirstNode,tEndNode,*nextNode,tAfterNode,*afterNodeRChild;
//	TREE_NODE	*rootNode,tAfterNode,*afterNodeRChild,*priorNode,tFirstNode,tEndNode,*nextNode;

	TREE_NODE	*rootNode,*priorNode,*nextNode,*afterNodeRChild;
	TREE_NODE	*loopNode,*opNode;
	int32		blret;
	if (tFirstNode == nullptr)
		return;
	if (tEndNode == nullptr)
		tEndNode = tFirstNode;
	do{
		blret = 1;
		tEndNode->Spin_Link_set();
		nextNode = tEndNode->cgRChild;
		if (tAfterNode == nullptr)
			tAfterNode = nextNode;
		if ((nextNode != nullptr) && (tAfterNode != tFirstNode) && (tAfterNode != tEndNode)){
			blret = 0;
			if (nextNode->Spin_Link_try() > 0){
				if (tAfterNode->Spin_Link_try((tAfterNode == nextNode)?G_LOCK_OFF:G_LOCK_ON) > 0){
					do{
						rootNode = nextNode->cgVirtualRoot;
						if (rootNode->Spin_Link_try((rootNode == tEndNode)?G_LOCK_OFF:G_LOCK_ON) == 0)
							break;
						{
							if (rootNode->cgVirtualRoot == tAfterNode)
								rootNode->cgVirtualRoot = tEndNode;
							
							tFirstNode->Spin_Link_set(((tFirstNode == tEndNode) || (tFirstNode == rootNode))?G_LOCK_OFF:G_LOCK_ON);
							{
								priorNode = tFirstNode->cgFather;
								
								if (priorNode != tAfterNode){
									afterNodeRChild = tAfterNode->cgRChild;
									if (afterNodeRChild != nullptr){
										afterNodeRChild->Spin_Link_set();
										afterNodeRChild->cgFather = tEndNode;
										afterNodeRChild->Spin_Link_clr();
									}
									
									tAfterNode->cgRChild = tFirstNode;
									tFirstNode->cgFather = tAfterNode;
									tEndNode->cgRChild = afterNodeRChild;
									
									nextNode->cgFather = priorNode;
									
									if (tFirstNode->cgblVirtualHead == 0){
										if (priorNode != rootNode)
											priorNode->Spin_Link_set();
										priorNode->cgRChild = nextNode;
										if (priorNode != rootNode)
											priorNode->Spin_Link_clr();
									}
									else{
										if (priorNode != nullptr){
											priorNode->Spin_Link_set();
											priorNode->cgLChild = nextNode;
											priorNode->Spin_Link_clr();
										}
										nextNode->cgblVirtualHead = 1;
										nextNode->cgID_NextDRNode.store(tFirstNode->cgID_NextDRNode.load());
										
										opNode = nextNode;
										loopNode = nextNode->cgRChild;
										while(loopNode != nullptr){
											opNode = loopNode;
											opNode->Spin_Link_set(((opNode == rootNode) || (opNode == tFirstNode) || (opNode == tEndNode) || (opNode == tAfterNode))
																   ?G_LOCK_OFF:G_LOCK_ON);
											opNode->cgblVirtualHead = 0;
											opNode->cgVirtualRoot = nextNode;
											loopNode = opNode->cgRChild;
											opNode->Spin_Link_clr(((opNode == rootNode) || (opNode == tFirstNode) || (opNode == tEndNode) || (opNode == tAfterNode))
																	 ?G_LOCK_OFF:G_LOCK_ON);
										};
										nextNode->cgVirtualRoot = opNode;
									}
								}
							}
							tFirstNode->Spin_Link_clr(((tFirstNode == tEndNode) || (tFirstNode == rootNode))?G_LOCK_OFF:G_LOCK_ON);
						}
						rootNode->Spin_Link_clr((rootNode == tEndNode)?G_LOCK_OFF:G_LOCK_ON);
						blret = 1;
					}while(0);
					tAfterNode->Spin_Link_clr((tAfterNode == nextNode)?G_LOCK_OFF:G_LOCK_ON);
				}
				nextNode->Spin_Link_clr();
			}
		}
		tEndNode->Spin_Link_clr();
	}while(blret == 0);
}
//------------------------------------------------------------------------------------------//
void TREE_NODE::DestroyTree(TREE_NODE *tTreeNode){
	if (tTreeNode == nullptr)
		return;
	
	DestroyTree(BreakRChild(tTreeNode));
	DestroyTree(BreakLChild(tTreeNode));
	
	try{
		delete tTreeNode;
	}
	catch(...){}
	tTreeNode = nullptr;
}
//------------------------------------------------------------------------------------------//
void TREE_NODE::DestroySubTree(TREE_NODE *tTreeNode){
	if (tTreeNode == nullptr)
		return;
	
	DestroyTree(BreakRChild(tTreeNode));
	DestroyTree(BreakLChild(tTreeNode));
}
//------------------------------------------------------------------------------------------//
TREE_NODE *TREE_NODE::GetNewNode(TREE_NODE *tTrashOwner){
	TREE_NODE *nNode,*cNode;
	if (tTrashOwner == nullptr)
		return(nullptr);
	if (tTrashOwner->cgTrash == nullptr)
		return(tTrashOwner->CreateNode());
	nNode = GetLastChild(tTrashOwner->cgTrash);
	if (nNode == nullptr)
		return(tTrashOwner->CreateNode());
	RemoveNodesInRChain(nNode);
	cNode = BreakLChild(nNode);
	InsertLChild(tTrashOwner->cgTrash,cNode);
	return(nNode);
}
//------------------------------------------------------------------------------------------//
void TREE_NODE::CreateTrash(TREE_NODE *tTrashOwner){
	if (tTrashOwner == nullptr)
		return;
	if (tTrashOwner->cgTrash == nullptr)
		tTrashOwner->cgTrash = new TREE_NODE;
}
//------------------------------------------------------------------------------------------//
void TREE_NODE::CleanTrash(TREE_NODE *tTrashOwner){
	TREE_NODE *deltree;
	if (tTrashOwner == nullptr)
		return;
	deltree = BreakLChild(tTrashOwner->cgTrash);
	DestroyTree(deltree);
}
//------------------------------------------------------------------------------------------//
void TREE_NODE::DestroyTrash(TREE_NODE *tTrashOwner){
	if (tTrashOwner == nullptr)
		return;
	DestroyTree(tTrashOwner->cgTrash);
	tTrashOwner->cgTrash = nullptr;
}
//------------------------------------------------------------------------------------------//
TREE_NODE *TREE_NODE::MoveNodeToTrash(TREE_NODE *tTreeNode,TREE_NODE *tTrashOwner){
	TREE_NODE *ret;
	if (tTreeNode == nullptr)
		return(nullptr);
	
	ret = RemoveNodesInRChain(tTreeNode);
	if (tTrashOwner == nullptr){
		DestroyTree(tTreeNode);
	}
	else{
		InsertLChild(tTrashOwner->cgTrash,tTreeNode);
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
TREE_NODE *TREE_NODE::MoveNodesToTrash(TREE_NODE *tFirstNode,TREE_NODE *tEndNode,TREE_NODE *tTrashOwner){
	TREE_NODE *ret;
	if (tFirstNode == nullptr)
		return(nullptr);
	
	ret = RemoveNodesInRChain(tFirstNode,tEndNode);
	if (tTrashOwner == nullptr){
		DestroyTree(tFirstNode);
	}
	else{
		InsertLChild(tTrashOwner->cgTrash,tFirstNode);
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
void TREE_NODE::MoveTreeToTrash(TREE_NODE *tTreeNode,TREE_NODE *tTrashOwner){
	if (tTreeNode == nullptr)
		return;
	RemoveFromFather(tTreeNode);
	if (tTrashOwner == nullptr){
		DestroyTree(tTreeNode);
	}
	else{
		InsertLChild(tTrashOwner->cgTrash,tTreeNode);
	}
}
//------------------------------------------------------------------------------------------//
TREE_NODE *FindByNodeID_lock(TREE_NODE *tTreeNode,uint32 tNodeID){
	uint32	id;
	TREE_NODE *ret;
	
	if (tTreeNode == nullptr)
		return(nullptr);
	
	ret = nullptr;
	tTreeNode->Spin_Link_set();
	
	id = TREE_NODE::GetNodeID(tTreeNode);
	
	if (id == tNodeID){
		ret = tTreeNode;
	}
	else{
		ret = FindByNodeID_lock(TREE_NODE::GetcgRChild(tTreeNode),tNodeID);
		if (ret == nullptr)
			ret = FindByNodeID_lock(TREE_NODE::GetcgLChild(tTreeNode),tNodeID);
	}
	
	tTreeNode->Spin_Link_clr();
	return(ret);
}
//------------------------------------------------------------------------------------------//
TREE_NODE *TREE_NODE::FindInTreeByNodeID(TREE_NODE *tTreeNode,uint32 tNodeID){
	uint32	id;
	TREE_NODE *ret,*rootNode;
	
	if (tTreeNode == nullptr)
		return(nullptr);
	
	ret = nullptr;
	rootNode = nullptr;
	id = tTreeNode->nodeID.load();
	
	if (id == tNodeID){
		ret = tTreeNode;
	}
	else{
		do{
			tTreeNode->Spin_Link_set();
			if (tTreeNode->cgblVirtualHead == 0){
				rootNode = tTreeNode->cgVirtualRoot;
				if (rootNode->Spin_Link_try() == 0){
					tTreeNode->Spin_Link_clr();
					continue;
				}
			}
			
			ret = FindByNodeID_lock(tTreeNode->cgRChild,tNodeID);
			if (ret == nullptr)
				ret = FindByNodeID_lock(tTreeNode->cgLChild,tNodeID);
			tTreeNode->Spin_Link_clr();
			break;
		}while (1);
		
		if (tTreeNode->cgblVirtualHead == 0)
			rootNode->Spin_Link_clr();
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
TREE_NODE *TREE_NODE::FindInLChildRChainByDRNodeID(TREE_NODE *tTreeNode,uint32 tDRNodeID){
	TREE_NODE *ret;
	ret = nullptr;
	if (tTreeNode != nullptr){
		TREE_LChildRChain_Find(TREE_NODE,tTreeNode,ret,(operateNode_t->dRNodeID.load() == tDRNodeID));
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
TREE_NODE *TREE_NODE::FindInLChildRChainByDRNodeID_nolock(TREE_NODE *tTreeNode,uint32 tDRNodeID){
	TREE_NODE	*ret;
	ret = nullptr;
	if (tTreeNode != nullptr){
		TREE_LChildRChain_Traversal_LINE_nolock(TREE_NODE,tTreeNode,
			if (operateNode_t->dRNodeID.load() == tDRNodeID){
				ret = operateNode_t;
				break;
			}
		);
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//
TREE_NODE *TREE_NODE::GetSelectedInLChildRChain(TREE_NODE *tTreeNode){
	TREE_NODE	*tSelect;
	tSelect = nullptr;
	if (tTreeNode != nullptr){
		TREE_LChildRChain_Find(TREE_NODE,tTreeNode,tSelect,(operateNode_t->CheckSelected() != 0));
	}
	return(tSelect);
}
//------------------------------------------------------------------------------------------//
TREE_NODE *TREE_NODE::GetSelectedInLChildRChain_nolock(TREE_NODE *tTreeNode){
	TREE_NODE	*tSelect;
	tSelect = nullptr;
	if (tTreeNode != nullptr){
		TREE_LChildRChain_Traversal_LINE_nolock(TREE_NODE,tTreeNode->cgLChild,
			if (operateNode_t->CheckSelected() != 0){
				tSelect = operateNode_t;
				break;
			}
		);
	}
	return(tSelect);
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
}*/
//------------------------------------------------------------------------------------------//
#endif
