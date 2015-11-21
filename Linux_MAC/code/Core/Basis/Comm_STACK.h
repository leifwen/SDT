/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: Comm_STACK.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.7.3
 *
*/
#ifndef Comm_STACKH
#define Comm_STACKH
//------------------------------------------------------------------------------------------//
#include "BasicDef.h"
#include <string>
//------------------------------------------------------------------------------------------//
template <typename TYPE_STACK>
class	COMMM_STACK{
	public:
				 COMMM_STACK(TYPE_STACK *externalBuf,int32 bufferSize);
				 COMMM_STACK(int32 bufferSize);
				 COMMM_STACK(void);
		virtual ~COMMM_STACK(void);
	private:
		TYPE_STACK	*stackFrontPointer;
		TYPE_STACK	*dataAreaPointer;	//used for external buffer
		int32	size;
		int32	maxSize;
		int32	blCreated;		//flag if create buffer
		int32	blInit;
	public:
		void	Init	(TYPE_STACK *buf,int32 bufferSize);
		void	Init	(int32 bufferSize);
		void	Clear	(void);
		
		int32	IsEmpty	(void)const{return(size == 0);};
		int32	IsFull	(void)const{return(size == maxSize);};
		int32	Push	(const TYPE_STACK &data);
		TYPE_STACK	*Pop		(void);
		TYPE_STACK	*StackTop	(void);
};
//---------------------------------------------------------------------------
template <typename TYPE_STACK>
COMMM_STACK<TYPE_STACK>::COMMM_STACK(void){
	blCreated = 0;
	blInit = 0;
	stackFrontPointer = NULL;
	dataAreaPointer = NULL;
}
//---------------------------------------------------------------------------
template <typename TYPE_STACK>
COMMM_STACK<TYPE_STACK>::COMMM_STACK(int32 bufferSize){
	blCreated = 0;
	blInit = 0;
	Init(bufferSize);
}
//---------------------------------------------------------------------------
template <typename TYPE_STACK>
COMMM_STACK<TYPE_STACK>::COMMM_STACK(TYPE_STACK *externalBuf,int32 bufferSize){
	blCreated = 0;
	blInit = 0;
	Init(externalBuf,bufferSize);
}
//---------------------------------------------------------------------------
template <typename TYPE_STACK>
COMMM_STACK<TYPE_STACK>::~COMMM_STACK(void){
	if (blCreated != 0){
		try{
			delete []dataAreaPointer;
		}
		catch(...){}
	}
	blInit  = 0;
	blCreated = 0;
	dataAreaPointer = NULL;
	stackFrontPointer = NULL;
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_STACK>
void COMMM_STACK<TYPE_STACK>::Init(TYPE_STACK *buf,int32 bufferSize){
	if (blInit == 0){
		dataAreaPointer = buf;
		stackFrontPointer = buf;
		size = 0;
		maxSize = bufferSize;
		blInit = 1;
	}
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_STACK>
void COMMM_STACK<TYPE_STACK>::Init(int32 bufferSize){
	if (blCreated == 0){
		try{
			dataAreaPointer = new TYPE_STACK[bufferSize];
			blCreated = 1;
		}
		catch(...){
			dataAreaPointer = NULL;
			stackFrontPointer = NULL;
		}
		if (dataAreaPointer != NULL)
			Init(dataAreaPointer,bufferSize);
	}
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_STACK>
void COMMM_STACK<TYPE_STACK>::Clear(void){
	stackFrontPointer = dataAreaPointer;
	size = 0;
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_STACK>
int32 COMMM_STACK<TYPE_STACK>::Push(const TYPE_STACK &data){
	if (dataAreaPointer == NULL)
		return 0;
	if (size == maxSize)
		return 0;
	if (size > 0)
		++ stackFrontPointer;
	*stackFrontPointer = data;
	++ size;
	return 1;
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_STACK>
TYPE_STACK	*COMMM_STACK<TYPE_STACK>::Pop(void){
	TYPE_STACK	*data;

	if (dataAreaPointer == NULL)
		return(dataAreaPointer);
	data = stackFrontPointer;
	-- size;
	if (size > 0)
		-- stackFrontPointer;

	return(data);
}
//------------------------------------------------------------------------------------------//
template <typename TYPE_STACK>
TYPE_STACK	*COMMM_STACK<TYPE_STACK>::StackTop(void){
	if (dataAreaPointer == NULL)
		return(dataAreaPointer);
	return(stackFrontPointer);
}
//------------------------------------------------------------------------------------------//
#endif