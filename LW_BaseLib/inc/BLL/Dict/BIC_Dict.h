//
//  BIC_Dict.h
//  SDT
//
//  Created by Leif Wen on 25/02/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#include "Dict.h"
#include "BIC_Define.h"
#if defined Dict_h && defined BIC_Define_h && defined SWVERSION_DICT
//------------------------------------------------------------------------------------------//
#ifndef BIC_Dict_h
#define BIC_Dict_h
#ifdef BIC_Dict_h
//------------------------------------------------------------------------------------------//
BIC_CLASSTYPE(DICT_DO,		"do");
BIC_CLASSTYPE(DICT_READ,	"read");
BIC_CLASSTYPE(DICT_SAVE,	"save");
BIC_CLASSTYPE(DICT_FIND,	"find");
BIC_CLASSTYPE(DICT_UPDATE,	"update");
BIC_CLASSTYPE(DICT_EMPTY,	"empty");
//------------------------------------------------------------------------------------------//
class BIC_DICT : public BIC_BASE_S{
	public:
				 BIC_DICT(void);
		virtual ~BIC_DICT(void);
	public:
		static	DICTWORD_NODE&	GetDict	(void);
		static	COCAWORD_NODE&	GetCoca	(void);
		static	void			Empty	(void);
	public:
		BIC_DICT_READ		cgSub_read;
		BIC_DICT_SAVE		cgSub_save;
		BIC_DICT_FIND		cgSub_find;
		BIC_DICT_UPDATE		cgSub_update;
		BIC_DICT_DO			cgSub_do;
		BIC_DICT_EMPTY		cgSub_empty;
};
//------------------------------------------------------------------------------------------//
#endif /* BIC_Dict_h */
#endif /* BIC_Dict_h */
#endif /* Dict_h */
