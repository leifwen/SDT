//
//  DS_Tree.cpp
//  SDT
//
//  Created by Leif Wen on 18/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "DS_Tree.h"
#include <sstream>
#include <iomanip>
//------------------------------------------------------------------------------------------//
#ifdef DS_Tree_h
//------------------------------------------------------------------------------------------//
std::atomic_uint TNF::cgNextUniqueID = ATOMIC_VAR_INIT(0);
//------------------------------------------------------------------------------------------//
TNF::TREE_NODE_FRAME(void) : TREE_FLAG(){
	cgNextUniqueID ++;
	cgUniqueID = cgNextUniqueID.load();
	
	cgNextDRNodeID = 0;
	cgDRNodeID = 0;
	
	cgHead = this;
	cgTail = this;
	cgPrior = nullptr;
	cgNext = nullptr;
	cgUp = nullptr;
	cgDown = nullptr;
};
//------------------------------------------------------------------------------------------//
TNF::~TREE_NODE_FRAME(void){
	cgHead = this;
	cgTail = this;
	cgPrior = nullptr;
	cgNext = nullptr;
	cgUp = nullptr;
	cgDown = nullptr;
};
//------------------------------------------------------------------------------------------//
STDSTR TNF::GetHEXUniqueID(const TNF* tnfNode){
	std::stringstream tStream;
	STDSTR	name;
	tStream << "" << std::setiosflags(std::ios::uppercase) << std::hex << std::setw(4) << std::setfill('0') << GetUniqueID(tnfNode);
	tStream >> name;
	return(name);
};
//------------------------------------------------------------------------------------------//
void TNF::UpdateInstert_nolock(TNF* tInsertNode,TNF* tHeadNode,bool32 blUpdatedRNodeID){
	TNF *loopNode;
	
	if (blUpdatedRNodeID != G_FALSE){
		tHeadNode->cgNextDRNodeID ++;
		tInsertNode->cgDRNodeID.store(tHeadNode->cgNextDRNodeID.load());
	}
	tInsertNode->cgHead = tHeadNode;
	tInsertNode->cgTail = nullptr;
	tInsertNode->cgUp = tHeadNode->cgUp;
	
	loopNode = tInsertNode->cgNext;
	while(loopNode != nullptr){
		if (blUpdatedRNodeID != G_FALSE){
			tHeadNode->cgNextDRNodeID ++;
			loopNode->cgDRNodeID.store(tHeadNode->cgNextDRNodeID.load());
		}
		loopNode->cgHead = tHeadNode;
		loopNode->cgTail = nullptr;
		loopNode->cgUp = tHeadNode->cgUp;
		loopNode = loopNode->cgNext;
	}
};
//------------------------------------------------------------------------------------------//
void TNF::UpdateHead_nolock(TNF* tHeadNode,bool32 blUpdatedRNodeID){
	TNF *loopNode;
	
	if (blUpdatedRNodeID != G_FALSE){
		tHeadNode->cgDRNodeID.store(0);
		tHeadNode->cgNextDRNodeID.store(0);
	}
	tHeadNode->cgHead = tHeadNode;
	tHeadNode->cgTail = tHeadNode;
	tHeadNode->cgPrior = nullptr;
	
	loopNode = tHeadNode->cgNext;
	while(loopNode != nullptr){
		if (blUpdatedRNodeID != G_FALSE){
			tHeadNode->cgNextDRNodeID ++;
			loopNode->cgDRNodeID.store(tHeadNode->cgNextDRNodeID.load());
		}
		loopNode->cgHead = tHeadNode;
		loopNode->cgTail = nullptr;
		loopNode->cgUp = tHeadNode->cgUp;
		tHeadNode->cgTail = loopNode;
		loopNode = loopNode->cgNext;
	}
};
//------------------------------------------------------------------------------------------//
TNF* TNF::InsertAfter(TNF* tOpNode,TNF* tInsertNode,bool32 blUpdatedRNodeID){
	TNF* f;
	do{
		Modify_set(tOpNode);
		f = tOpNode->cgUp;
		if (Modify_try(f) == G_FALSE){
			Modify_clr(tOpNode);
			continue;
		}
		else{
			Modify_clr(tOpNode);
			InsertAfter_nolock(tOpNode,tInsertNode,blUpdatedRNodeID);
			Modify_clr(f);
		}
		break;
	}while(1);
	return(tOpNode);
};
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
};
//------------------------------------------------------------------------------------------//
TNF* TNF::InsertBefore(TNF* tOpNode,TNF* tInsertNode,bool32 blUpdatedRNodeID){
	TNF *f;
	do{
		Modify_set(tOpNode);
		f = tOpNode->cgUp;
		if (Modify_try(f) == G_FALSE){
			Modify_clr(tOpNode);
			continue;
		}
		else{
			Modify_clr(tOpNode);
			InsertBefore_nolock(tOpNode,tInsertNode,blUpdatedRNodeID);
			Modify_clr(f);
		}
		break;
	}while(1);
	return(tOpNode);
};
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
			tInsertNode->cgNextDRNodeID.store(head->cgNextDRNodeID.load());
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
};
//------------------------------------------------------------------------------------------//
TNF* TNF::InsertDownTail(TNF* tOpNode,TNF* tInsertNode){
	do{
		if (tOpNode == nullptr)
			break;
		Modify_set(tOpNode);
		InsertDownTail_nolock(tOpNode,tInsertNode);
		Modify_clr(tOpNode);
	}while(0);
	return(tOpNode);
};
//------------------------------------------------------------------------------------------//
TNF* TNF::InsertDownTail_nolock(TNF* tOpNode,TNF* tInsertNode){
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
};
//------------------------------------------------------------------------------------------//
void TNF::MovePrior(TNF* tFirstNode,TNF* tEndNode){
	TNF *f;
	do{
		Modify_set(tFirstNode);
		f = tFirstNode->cgUp;
		if (Modify_try(f) == G_FALSE){
			Modify_clr(tFirstNode);
			continue;
		}
		else{
			Modify_clr(tFirstNode);
			MovePrior_nolock(tFirstNode,tEndNode);
			Modify_clr(f);
		}
		break;
	}while(1);
};
//------------------------------------------------------------------------------------------//
void TNF::MovePrior_nolock(TNF* tFirstNode,TNF* tEndNode){
	TNF	*prior;
	do{
		if (tFirstNode == nullptr)
			break;
		if (tEndNode == nullptr)
			tEndNode = tFirstNode->cgHead->cgTail;
		prior = tFirstNode->cgPrior;
		if (prior == nullptr)
			break;
		DetachUpPriorNext_nolock(tFirstNode,tEndNode,G_FALSE);
		InsertBefore_nolock(prior,tFirstNode,G_FALSE);
	}while(0);
};
//------------------------------------------------------------------------------------------//
void TNF::MoveNext(TNF* tFirstNode,TNF* tEndNode){
	TNF *f;
	do{
		Modify_set(tFirstNode);
		f = tFirstNode->cgUp;
		if (Modify_try(f) == G_FALSE){
			Modify_clr(tFirstNode);
			continue;
		}
		else{
			Modify_clr(tFirstNode);
			MoveNext_nolock(tFirstNode,tEndNode);
			Modify_clr(f);
		}
		break;
	}while(1);
};
//------------------------------------------------------------------------------------------//
void TNF::MoveNext_nolock(TNF* tFirstNode,TNF* tEndNode){
	TNF	*next;
	do{
		if (tFirstNode == nullptr)
			break;
		if (tEndNode == nullptr)
			tEndNode = tFirstNode->cgHead->cgTail;
		next = tEndNode->cgNext;
		if (next == nullptr)
			break;
		DetachUpPriorNext_nolock(tFirstNode,tEndNode,G_FALSE);
		InsertAfter_nolock(next,tFirstNode,G_FALSE);
	}while(0);
};
//------------------------------------------------------------------------------------------//
void TNF::MoveAfter(TNF* tFirstNode,TNF* tEndNode,TNF* tAfterNode){
	TNF *f;
	do{
		Modify_set(tFirstNode);
		f = tFirstNode->cgUp;
		if (Modify_try(f) == G_FALSE){
			Modify_clr(tFirstNode);
			continue;
		}
		else{
			Modify_clr(tFirstNode);
			MoveAfter_nolock(tFirstNode,tEndNode,tAfterNode);
			Modify_clr(f);
		}
		break;
	}while(1);
};
//------------------------------------------------------------------------------------------//
void TNF::MoveAfter_nolock(TNF* tFirstNode,TNF* tEndNode,TNF* tAfterNode){
	do{
		if (tFirstNode == nullptr)
			break;
		if (tEndNode == nullptr)
			tEndNode = tFirstNode->cgHead->cgTail;
		if (tAfterNode == nullptr)
			tAfterNode = tFirstNode->cgHead->cgTail;
		if (tAfterNode == nullptr)
			break;
		DetachUpPriorNext_nolock(tFirstNode,tEndNode,G_FALSE);
		InsertAfter_nolock(tAfterNode,tFirstNode,G_FALSE);
	}while(0);
};
//------------------------------------------------------------------------------------------//
TNF* TNF::DetachUpPriorNext(TNF* tFirstNode,TNF* tEndNode,bool32 blUpdatedRNodeID){
	TNF *f;
	do{
		Modify_set(tFirstNode);
		f = tFirstNode->cgUp;
		if (Modify_try(f) == G_FALSE){
			Modify_clr(tFirstNode);
			continue;
		}
		else{
			Modify_clr(tFirstNode);
			DetachUpPriorNext_nolock(tFirstNode,tEndNode,blUpdatedRNodeID);
			Modify_clr(f);
		}
		break;
	}while(1);
	return(tFirstNode);
};
//------------------------------------------------------------------------------------------//
TNF* TNF::DetachUpPriorNext_nolock(TNF* tFirstNode,TNF* tEndNode,bool32 blUpdatedRNodeID){
	TNF *head,*next,*prior,*upNode;
	do{
		if (tFirstNode == nullptr)
			break;
		if (tEndNode == nullptr)
			tEndNode = tFirstNode->cgHead->cgTail;
		
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
				next->cgNextDRNodeID.store(tFirstNode->cgNextDRNodeID.load());
				UpdateHead_nolock(next,G_FALSE);
			}
		}
		tEndNode->cgNext = nullptr;
		tFirstNode->cgUp = nullptr;
		UpdateHead_nolock(tFirstNode,blUpdatedRNodeID);
	}while(0);
	return(tFirstNode);
};
//------------------------------------------------------------------------------------------//
TNF* TNF::DetachDown(TNF* tnfNode){
	TNF	*child;
	child = nullptr;
	do{
		if (tnfNode == nullptr)
			break;
		Modify_set(tnfNode);
		child = DetachDown_nolock(tnfNode);
		Modify_clr(tnfNode);
	}while(0);
	return(child);
};
//------------------------------------------------------------------------------------------//
TNF* TNF::DetachDown_nolock(TNF* tnfNode){
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
};
//------------------------------------------------------------------------------------------//
TNF* TNF::DetachNext(TNF* tnfNode){
	TNF *next;
	TNF *f;
	do{
		Modify_set(tnfNode);
		f = tnfNode->cgUp;
		if (Modify_try(f) == G_FALSE){
			Modify_clr(tnfNode);
			continue;
		}
		else{
			Modify_clr(tnfNode);
			next = DetachNext_nolock(tnfNode);
			Modify_clr(f);
		}
		break;
	}while(1);
	return(next);
};
//------------------------------------------------------------------------------------------//
TNF* TNF::DetachNext_nolock(TNF* tnfNode){
	TNF	*next;
	next = nullptr;
	do{
		if (tnfNode == nullptr)
			break;
		next = tnfNode->cgNext;
		if (next == nullptr)
			break;
		DetachUpPriorNext_nolock(next,tnfNode->cgHead->cgTail);
	}while(0);
	return(next);
};
//------------------------------------------------------------------------------------------//
TNF* TNF::FindInDownChainByDRNodeID(TNF* tnNode,uint32 tDRNodeID){
	TNF	*ret;
	ret = nullptr;
	if (tnNode != nullptr){
		TREE_DownChain_Find(TNF,tnNode,ret,(GetDRNodeID(_opNode) == tDRNodeID));
	}
	return(ret);
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
TNFP::TREE_NODE_FRAME_POOL(void) : TREE_NODE_FRAME(){
	STDSTR	name;
	cgTrash = nullptr;

	selfName = "TN" + GetHEXUniqueID(this);
	fatherName = "";
};
//------------------------------------------------------------------------------------------//
TNFP::~TREE_NODE_FRAME_POOL(void){
	DestroyTrash();
};
//------------------------------------------------------------------------------------------//
TNFP* TNFP::SetNodeUpName(TNFP* node){
	if (node != nullptr)
		node->SetUpName(GetFullName(this));
	return(node);
};
//------------------------------------------------------------------------------------------//
void TNFP::DestroyTree(TNF* tnfNode){
	if (tnfNode == nullptr)
		return;
	
	TNF *oNode,*nextNode;

	DetachUpPriorNext(tnfNode,nullptr,G_FALSE);
	
	oNode = tnfNode;
	do{
		oNode = GetNext(InsertAfter_nolock(oNode,DetachDown_nolock(oNode),G_FALSE));
	}while(oNode != nullptr);
	
	oNode = tnfNode;
	do{
		nextNode = GetNext(oNode);
		delete oNode;
		oNode = nextNode;
	}while(oNode != nullptr);
};
//------------------------------------------------------------------------------------------//
void TNFP::DestroyDownNextTree(TNF* tnfNode){
	if (tnfNode == nullptr)
		return;
	
	TNF *child,*next;
	
	child = DetachDown(tnfNode);
	next = DetachNext(tnfNode);

	if (next != nullptr){
		InsertAfter_nolock(next,child,G_FALSE);
	}
	else{
		next = child;
	}
	DestroyTree(next);
};
//------------------------------------------------------------------------------------------//
void TNFP::MoveNodesToTrash(TNFP* trashOwner,TNF* tFirstNode,TNF* tEndNode){
	if (tFirstNode == nullptr)
		return;
	
	DetachUpPriorNext(tFirstNode,tEndNode);
	if (trashOwner == nullptr){
		DestroyTree(tFirstNode);
	}
	else if (trashOwner->GetTrash() == nullptr){
		DestroyTree(tFirstNode);
	}
	else{
		InsertDownTail(trashOwner->GetTrash(),tFirstNode);
	}
};
//------------------------------------------------------------------------------------------//
TNF* TNFP::GetNewNode(void){
	TNF* nNode,*tNode;
	tNode = GetTrash();
	if (tNode == nullptr){
		nNode = CreateNode();
	}
	else{
		Modify_set(tNode);
		nNode = GetDown(tNode);
		if (nNode == nullptr){
			nNode = CreateNode();
		}
		else{
			nNode = CleanDownTree(this,DetachUpPriorNext_nolock(GetTail(nNode),GetTail(nNode)));
		}
		Modify_clr(tNode);
	}
	return(nNode);
};
//------------------------------------------------------------------------------------------//
#endif
