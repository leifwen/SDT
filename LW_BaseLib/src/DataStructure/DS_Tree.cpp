//
//  DS_Tree.cpp
//  SDT
//
//  Created by Leif Wen on 18/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//
#include "stdafx.h"
#include "DS_Tree.h"
#include <sstream>
#include <iomanip>
#ifdef DS_Tree_h
//------------------------------------------------------------------------------------------//
std::atomic_uint TNF::cgID_NextNode = ATOMIC_VAR_INIT(0);
//------------------------------------------------------------------------------------------//
TNF::TREE_NODE_FRAME(void) : BASE_FLAG(){
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
TNF::~TREE_NODE_FRAME(void){
	cgHead = this;
	cgTail = this;
	cgPrior = nullptr;
	cgNext = nullptr;
	cgUp = nullptr;
	cgDown = nullptr;
}
//------------------------------------------------------------------------------------------//
STDSTR TNF::GetHEXID(const TNF* tnfNode){
	std::stringstream tStream;
	STDSTR	name;
	tStream << "" << std::setiosflags(std::ios::uppercase) << std::hex << std::setw(4) << std::setfill('0') << GetNodeID(tnfNode);
	tStream >> name;
	return(name);
};
//------------------------------------------------------------------------------------------//
void TNF::UpdateInstert_nolock(TNF* tInsertNode,TNF* tHeadNode,bool32 blUpdatedRNodeID){
	TNF *loopNode;
	
	if (blUpdatedRNodeID != G_FALSE){
		tHeadNode->cgID_NextDRNode ++;
		tInsertNode->dRNodeID.store(tHeadNode->cgID_NextDRNode.load());
	}
	tInsertNode->cgHead = tHeadNode;
	tInsertNode->cgTail = nullptr;
	tInsertNode->cgUp = tHeadNode->cgUp;
	
	loopNode = tInsertNode->cgNext;
	while(loopNode != nullptr){
		if (blUpdatedRNodeID != G_FALSE){
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
void TNF::UpdateHead_nolock(TNF* tHeadNode,bool32 blUpdatedRNodeID){
	TNF *loopNode;
	
	if (blUpdatedRNodeID != G_FALSE){
		tHeadNode->dRNodeID.store(0);
		tHeadNode->cgID_NextDRNode.store(0);
	}
	tHeadNode->cgHead = tHeadNode;
	tHeadNode->cgTail = tHeadNode;
	tHeadNode->cgPrior = nullptr;
	
	loopNode = tHeadNode->cgNext;
	while(loopNode != nullptr){
		if (blUpdatedRNodeID != G_FALSE){
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
TNF* TNF::InsertAfter(TNF* tOpNode,TNF* tInsertNode,bool32 blUpdatedRNodeID){
	TNF* f;
	f = tOpNode->cgUp;
	LockChild_set(f);
	InsertAfter_nolock(tOpNode,tInsertNode,blUpdatedRNodeID);
	LockChild_clr(f);
	return(tOpNode);
}
//------------------------------------------------------------------------------------------//
TNF* TNF::InsertAfter_nolock(TNF* tOpNode,TNF* tInsertNode,bool32 blUpdatedRNodeID){
	TNF	*head,*endNode,*next;
	
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
TNF* TNF::InsertBefore(TNF* tOpNode,TNF* tInsertNode,bool32 blUpdatedRNodeID){
	TNF *f;
	f = tOpNode->cgUp;
	LockChild_set(f);
	InsertBefore_nolock(tOpNode,tInsertNode,blUpdatedRNodeID);
	LockChild_clr(f);
	return(tOpNode);
}
//------------------------------------------------------------------------------------------//
TNF* TNF::InsertBefore_nolock(TNF* tOpNode,TNF* tInsertNode,bool32 blUpdatedRNodeID){
	TNF	*head,*endNode,*prior;
	
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
TNF* TNF::AddSubNode(TNF* tOpNode,TNF* tInsertNode){
	do{
		if (tOpNode == nullptr)
			break;
		LockChild_set(tOpNode);
		AddSubNode_nolock(tOpNode,tInsertNode);
		LockChild_clr(tOpNode);
	}while(0);
	return(tOpNode);
}
//------------------------------------------------------------------------------------------//
TNF* TNF::AddSubNode_nolock(TNF* tOpNode,TNF* tInsertNode){
	do{
		if ((tOpNode == nullptr) || (tInsertNode == nullptr) || (tOpNode == tInsertNode))
			break;
		
		if (tInsertNode->cgUp != nullptr)
			break;
		if (tOpNode->cgDown == nullptr){
			tOpNode->cgDown = tInsertNode;
			tInsertNode->cgUp = tOpNode;
			UpdateHead_nolock(tInsertNode,G_TRUE);
		}
		else{
			InsertAfter_nolock(tOpNode->cgDown->cgTail,tInsertNode);
		}
	}while(0);
	return(tOpNode);
}
//------------------------------------------------------------------------------------------//
TNF* TNF::Remove(TNF* tFirstNode,TNF* tEndNode,bool32 blUpdatedRNodeID){
	TNF *f;
	f = tFirstNode->cgUp;
	LockChild_set(f);
	Remove_nolock(tFirstNode,tEndNode,blUpdatedRNodeID);
	LockChild_clr(f);
	return(tFirstNode);
}
//------------------------------------------------------------------------------------------//
TNF* TNF::Remove_nolock(TNF* tFirstNode,TNF* tEndNode,bool32 blUpdatedRNodeID){
	TNF *head,*next,*prior,*upNode;
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
				UpdateHead_nolock(next,G_FALSE);
			}
		}
		tEndNode->cgNext = nullptr;
		tFirstNode->cgUp = nullptr;
		UpdateHead_nolock(tFirstNode,blUpdatedRNodeID);
	}while(0);
	return(tFirstNode);
}
//------------------------------------------------------------------------------------------//
void TNF::MoveUp(TNF* tFirstNode,TNF* tEndNode){
	TNF *f;
	f = tFirstNode->cgUp;
	LockChild_set(f);
	MoveUp_nolock(tFirstNode,tEndNode);
	LockChild_clr(f);
}
//------------------------------------------------------------------------------------------//
void TNF::MoveUp_nolock(TNF* tFirstNode,TNF* tEndNode){
	TNF	*prior;
	do{
		if (tFirstNode == nullptr)
			break;
		if (tEndNode == nullptr)
			tEndNode = tFirstNode;
		prior = tFirstNode->cgPrior;
		if (prior == nullptr)
			break;
		Remove_nolock(tFirstNode,tEndNode,G_FALSE);
		InsertBefore_nolock(prior,tFirstNode,G_FALSE);
	}while(0);
}
//------------------------------------------------------------------------------------------//
void TNF::MoveDown(TNF* tFirstNode,TNF* tEndNode){
	TNF *f;
	f = tFirstNode->cgUp;
	LockChild_set(f);
	MoveDown_nolock(tFirstNode,tEndNode);
	LockChild_clr(f);
}
//------------------------------------------------------------------------------------------//
void TNF::MoveDown_nolock(TNF* tFirstNode,TNF* tEndNode){
	TNF	*next;
	do{
		if (tFirstNode == nullptr)
			break;
		if (tEndNode == nullptr)
			tEndNode = tFirstNode;
		next = tEndNode->cgNext;
		if (next == nullptr)
			break;
		Remove_nolock(tFirstNode,tEndNode,G_FALSE);
		InsertAfter_nolock(next,tFirstNode,G_FALSE);
	}while(0);
}
//------------------------------------------------------------------------------------------//
void TNF::MoveAfter(TNF* tFirstNode,TNF* tEndNode,TNF* tAfterNode){
	TNF *f;
	f = tFirstNode->cgUp;
	LockChild_set(f);
	MoveAfter_nolock(tFirstNode,tEndNode,tAfterNode);
	LockChild_clr(f);
}
//------------------------------------------------------------------------------------------//
void TNF::MoveAfter_nolock(TNF* tFirstNode,TNF* tEndNode,TNF* tAfterNode){
	do{
		if (tFirstNode == nullptr)
			break;
		if (tEndNode == nullptr)
			tEndNode = tFirstNode;
		if (tAfterNode == nullptr)
			tAfterNode = tEndNode->cgNext;
		if (tAfterNode == nullptr)
			break;
		Remove_nolock(tFirstNode,tEndNode,G_FALSE);
		InsertAfter_nolock(tAfterNode,tFirstNode,G_FALSE);
	}while(0);
}
//------------------------------------------------------------------------------------------//
TNF* TNF::BreakChild(TNF* tnfNode){
	TNF	*child;
	child = nullptr;
	do{
		if (tnfNode == nullptr)
			break;
		LockChild_set(tnfNode);
		child = BreakChild_nolock(tnfNode);
		LockChild_clr(tnfNode);
	}while(0);
	return(child);
}
//------------------------------------------------------------------------------------------//
TNF* TNF::BreakChild_nolock(TNF* tnfNode){
	TNF	*child;
	child = nullptr;
	do{
		if (tnfNode == nullptr)
			break;
		child = tnfNode->cgDown;
		if (child == nullptr)
			break;
		tnfNode->cgDown = nullptr;
		child->cgUp = nullptr;
		UpdateHead_nolock(child,G_TRUE);
	}while(0);
	return(child);
}
//------------------------------------------------------------------------------------------//
TNF* TNF::BreakNext(TNF* tnfNode){
	TNF *next;
	TNF *f;
	f = tnfNode->cgUp;
	LockChild_set(f);
	next = BreakNext_nolock(tnfNode);
	LockChild_clr(f);
	return(next);
}
//------------------------------------------------------------------------------------------//
TNF* TNF::BreakNext_nolock(TNF* tnfNode){
	TNF	*next;
	next = nullptr;
	do{
		if (tnfNode == nullptr)
			break;
		next = tnfNode->cgNext;
		if (next == nullptr)
			break;
		Remove(next,tnfNode->cgHead->cgTail);
	}while(0);
	return(next);
}
//------------------------------------------------------------------------------------------//
TNF* TNF::FindInLChildRChainByDRNodeID(TNF* tnNode,uint32 tDRNodeID){
	TNF	*ret;
	ret = nullptr;
	if (tnNode != nullptr){
		TREE_LChildRChain_Find(TNF,tnNode,ret,(GetdRNodeID(_opNode) == tDRNodeID));
	}
	return(ret);
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
TNFP::TREE_NODE_FRAME_POOL(void) : TREE_NODE_FRAME(){
	STDSTR	name;
	cgTrash = nullptr;

	selfName = "TN" + GetHEXID(this);
	fatherName = "";
}
//------------------------------------------------------------------------------------------//
TNFP::~TREE_NODE_FRAME_POOL(void){
	InUse_set();
	DestroyTrash();
	InUse_clr();
};
//------------------------------------------------------------------------------------------//
TNFP* TNFP::SetSubNodeFatherName(TNFP* node){
	if (node != nullptr)
		node->SetFatherName(GetFullName(this));
	return(node);
};
//------------------------------------------------------------------------------------------//
void TNFP::DestroyTree(TNF* tnfNode){
	if (tnfNode == nullptr)
		return;
	
	TNF *oNode,*nextNode;
	
	Remove(tnfNode,GetTail(GetHead(tnfNode)),G_FALSE);
	
	oNode = tnfNode;
	do{
		oNode = GetNext(InsertAfter_nolock(oNode,BreakChild_nolock(oNode),G_FALSE));
	}while(oNode != nullptr);
	
	oNode = tnfNode;
	do{
		nextNode = GetNext(oNode);
		delete oNode;
		oNode = nextNode;
	}while(oNode != nullptr);
}
//------------------------------------------------------------------------------------------//
void TNFP::DestroySubTree(TNF* tnfNode){
	if (tnfNode == nullptr)
		return;
	
	TNF *child,*next;
	
	child = BreakChild(tnfNode);
	next = BreakNext(tnfNode);

	if (next != nullptr){
		InsertAfter_nolock(next,child,G_FALSE);
	}
	else{
		next = child;
	}
	DestroyTree(next);
}
//------------------------------------------------------------------------------------------//
void TNFP::MoveNodesToTrash(TNFP* trashOwner,TNF* tFirstNode,TNF* tEndNode){
	if (tFirstNode == nullptr)
		return;
	
	Remove(tFirstNode,tEndNode);
	if (trashOwner == nullptr){
		DestroyTree(tFirstNode);
	}
	else if (trashOwner->GetTrash() == nullptr){
		DestroyTree(tFirstNode);
	}
	else{
		AddSubNode(trashOwner->GetTrash(),tFirstNode);
	}
}
//------------------------------------------------------------------------------------------//
TNF* TNFP::GetNewNode(void){
	TNF* nNode;
	if (GetTrash() == nullptr)
		return(CreateNode());
	nNode = GetDown(GetTrash());
	if (nNode == nullptr)
		return(CreateNode());
	return(CleanChild(this,Remove(GetTail(nNode))));
}
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
TREE_NODE::TREE_NODE(void) : TNFP(){
	cgCNType = CN_None;
	cgCoupleNode = nullptr;
	Enable();
};
//------------------------------------------------------------------------------------------//
void TREE_NODE::LinkCoupleNode_nolock(TREE_NODE* slaveNode){
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
void TREE_NODE::LinkCoupleNode(TREE_NODE* slaveNode){
	if ((slaveNode == nullptr) || (slaveNode == this))
		return;
	
	do{
		InUse_set();
		if (cgCoupleNode != nullptr){
			if (cgCoupleNode->InUse_try() == G_FALSE){
				InUse_clr();
				continue;
			}
			cgCoupleNode->cgCoupleNode = nullptr;
			cgCoupleNode->cgCNType = CN_None;
			cgCoupleNode->InUse_clr();
			cgCoupleNode = nullptr;
			cgCNType = CN_None;
		}
		if (slaveNode->InUse_try() == G_FALSE){
			InUse_clr();
			continue;
		}
		if (slaveNode->cgCoupleNode != nullptr){
			if (slaveNode->cgCoupleNode->InUse_try() == G_FALSE){
				slaveNode->InUse_clr();
				InUse_clr();
				continue;
			}
			slaveNode->cgCoupleNode->cgCoupleNode = nullptr;
			slaveNode->cgCoupleNode->cgCNType = CN_None;
			slaveNode->cgCoupleNode->InUse_clr();
		}
		slaveNode->cgCoupleNode = this;
		slaveNode->cgCNType = CN_S;
		cgCoupleNode = slaveNode;
		cgCNType = CN_M;
		
		slaveNode->InUse_clr();
		InUse_clr();
		break;
	}while(1);
}
//------------------------------------------------------------------------------------------//
TREE_NODE* TREE_NODE::UnlinkCoupleNode(void){
	TREE_NODE *retCPNode = nullptr;
	do{
		InUse_set();
		if (cgCoupleNode != nullptr){
			if (cgCoupleNode->InUse_try() == G_FALSE){
				InUse_clr();
				continue;
			}
			retCPNode = cgCoupleNode;
			cgCoupleNode->cgCoupleNode = nullptr;
			cgCoupleNode->cgCNType = CN_None;
			cgCoupleNode->InUse_clr();
			cgCoupleNode = nullptr;
			cgCNType = CN_None;
		}
		InUse_clr();
		break;
	}while(1);
	return(retCPNode);
}
//------------------------------------------------------------------------------------------//
#endif
