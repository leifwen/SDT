//
//  DS_Transform.cpp
//  SDT
//
//  Created by Leif Wen on 19/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
#include "DS_Transform.h"
//------------------------------------------------------------------------------------------//
#ifdef DS_Transform_h
#include "DS_STRING.h"
#include "SYS_File.h"
//------------------------------------------------------------------------------------------//
DS_IO_NODE::DS_IO_NODE(void) : TNFP(){
	ClrSFlag(DSTF_blStart);
	TNFP::SetSelfName("DS_IO_NODE");
};
//------------------------------------------------------------------------------------------//
ioss DS_IO_NODE::DoConvert(DSIO* ioNode,IOSTATUS* _ios,const UVOut& _out,const UVIn& _in){
	ioss iossta;
	iossta = BaseConvert(ioNode,_ios,_out,_in);
	if (iossta == IOS_NOCONVERT){
		TREE_LChildRChain_Traversal_LINE_nolock(DS_IO_NODE,this,
			iossta = _opNode->DoConvert(ioNode,_ios,_out,_in);
			if (iossta != IOS_NOCONVERT)
				break;
		);
	}
	return(iossta);
};
//------------------------------------------------------------------------------------------//
ioss DS_IO_NODE::DoSave(IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	ioss iossta;
	iossta = BaseSave(_ios,_out,data,length);
	if (iossta == IOS_NOCONVERT){
		TREE_LChildRChain_Traversal_LINE_nolock(DS_IO_NODE,this,
			iossta = _opNode->DoSave(_ios,_out,data,length);
			if (iossta != IOS_NOCONVERT)
				break;
		);
	}
	return(iossta);
};
//------------------------------------------------------------------------------------------//
ioss DS_IO_NODE::DoGetInLength(uint64* num,const UVIn& _in){
	ioss iossta;
	iossta = BaseGetInLength(num,_in);
	if (iossta == G_FALSE){
		TREE_LChildRChain_Traversal_LINE_nolock(DS_IO_NODE,this,
			iossta = _opNode->DoGetInLength(num,_in);
			if (iossta != IOS_NOCONVERT)
				break;
		);
	}
	return(iossta);;
};
//------------------------------------------------------------------------------------------//
ioss DS_IO_NODE::BaseConvert(DSIO* ioNode,IOSTATUS* _ios,const UVOut& _out,const UVIn& _in){
	ioss iossta;
	iossta = IOS_OK;
	if (_in.uvp == nullptr){
		iossta = ioNode->DoTransform(_ios,_out,nullptr,0);
	}
	else{
		switch (_in.uvid) {
			case UVID_NONE:
				break;
			case UVID_CHARS:{
				_UVCHARS* uvp = (_UVCHARS*)_in.uvp;
				iossta = ioNode->DoTransform(_ios,_out,(uint8*)uvp->p,uvp->num);
				break;
			}
			case UVID_STR:{
				STDSTR* str = (STDSTR*)_in.uvp;
				iossta = ioNode->DoTransform(_ios,_out,(uint8*)str->c_str(),str->length());
				break;
			}
			case UVID_ARRAY:{
				_UVARRAY* uvp = (_UVARRAY*)_in.uvp;
				iossta = ArrayConvert(ioNode,_ios,_out,(ARRAY*)uvp->p,uvp->num,uvp->offset);
				break;
			}
			case UVID_FILE:{
				_UVFILE* uvp = (_UVFILE*)_in.uvp;
				iossta = FileConvert(ioNode,_ios,_out,*((STDSTR*)uvp->p),uvp->num,uvp->offset);
				break;
			}
			default:;
				iossta = IOS_NOCONVERT;
		};
	}
	return(iossta);
};
//------------------------------------------------------------------------------------------//
ioss DS_IO_NODE::ArrayConvert(DSIO* ioNode,IOSTATUS* _ios,const UVOut& _out,const ARRAY* _array,uint32 length,uint32 offset){
	uint32	slength;
	ioss iossta;
	iossta = IOS_OK;
	slength = _array->CalcOutLength(length,offset);
	if (length > 0){
		iossta = ioNode->DoTransform(_ios,_out,_array->GetPointer(offset),length);
		if (slength > 0)
			iossta = ioNode->DoTransform(_ios,_out,_array->GetPointer(0),slength);
	}
	return(iossta);
}
//------------------------------------------------------------------------------------------//
ioss DS_IO_NODE::FileConvert(DSIO* ioNode, IOSTATUS* _ios,const UVOut& _out,const STDSTR& name,uint64 length,uint64 offset){
	uint32			num;
	uint8			data[1024];
	std::fstream	fileStream;
	ioss iossta;

	if (CFS_CheckFile(name) == G_FALSE){
		if (_ios != nullptr)
			_ios->status = IOS_ERR;
		return IOS_ERR;
	}

	fileStream.open(name.c_str(),std::ios::in|std::ios::binary);
	if (offset > 0)
		fileStream.seekp(offset,std::ios::beg);
	do{
		num = (sizeof(data) < length) ? sizeof(data) : (uint32)length;
		fileStream.read((char*)data,num);
		num = (uint32)fileStream.gcount();
		length -= num;
		iossta = ioNode->DoTransform(_ios,_out,data,num);
	}while((iossta >= IOS_OK) && (!fileStream.eof()) && (length > 0));
	fileStream.close();
	return(iossta);
}
//------------------------------------------------------------------------------------------//
ioss DS_IO_NODE::BaseSave(IOSTATUS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	IOSTATUS	ioStatus;
	G_SPACE		space;
	
	IOSTATUS_Clr(&ioStatus);
	
	if (_out.uvid == UVID_UVVTRAN){
		DSIO* io = (DSIO*)((_UVTRAN*)_out.uvp)->iop;
		if (data != nullptr){
			ioStatus.avail_in = length;
			io->Transform(&ioStatus,((_UVTRAN*)_out.uvp)->uvb,data,length);
		}
		else{
			io->Final(&ioStatus,((_UVTRAN*)_out.uvp)->uvb);
		}
		IOSTATUS_Add(_ios,ioStatus);
	}
	else if (data != nullptr){
		ioStatus.avail_in = length;
		space = G_SPACE_ON;
		switch (_out.uvid) {
			case UVID_NONE:
				ioStatus.total_in = ioStatus.avail_in;
				ioStatus.avail_in -= ioStatus.total_in;
				ioStatus.total_out = ioStatus.total_in;
				break;
			case UVID_CHARS:{
				_UVCHARS	*array = (_UVCHARS*)_out.uvp;
				ioStatus.total_in = (ioStatus.avail_in < (array->num - array->offset)) ? ioStatus.avail_in : (array->num - array->offset);
				if (ioStatus.total_in > 0)
					memcpy((uint8*)array->p + array->offset,data,ioStatus.total_in);
				ioStatus.avail_in -= ioStatus.total_in;
				ioStatus.avail_out = array->num - array->offset - ioStatus.total_in;
				ioStatus.total_out = ioStatus.total_in;
				array->offset += ioStatus.total_out;
				ioStatus.status = (ioStatus.avail_in > 0) ? IOS_NOMEM : IOS_OK;
				break;
			}
			case UVID_STR:{
				STDSTR* str = (STDSTR*)_out.uvp;
				str->append((char*)data, length);
				ioStatus.total_in = ioStatus.avail_in;
				ioStatus.avail_in -= ioStatus.total_in;
				ioStatus.avail_out = -1;
				ioStatus.total_out = ioStatus.total_in;
				break;
			}
			case UVID_STRHEX:
				space = G_SPACE_OFF;
			case UVID_STRHEXs:{
				STDSTR* str = (STDSTR*)_out.uvp;
				ioStatus.total_in = ioStatus.avail_in;
				ioStatus.avail_in -= ioStatus.total_in;
				ioStatus.total_out = str->length();
				Str_CharToStr(str, data, length, G_HEX, space, G_ESCAPE_OFF);
				ioStatus.avail_out = -1;
				ioStatus.total_out = str->length() - ioStatus.total_out;
				break;
			}
			case UVID_ARRAY:
				ioStatus.total_in = ((ARRAY*)_out.uvp)->Put(data, (uint32)length);
				ioStatus.avail_in -= ioStatus.total_in;
				ioStatus.avail_out = ((ARRAY*)_out.uvp)->Unused();
				ioStatus.total_out = ioStatus.total_in;
				ioStatus.status = ((ARRAY*)_out.uvp)->IsNoMEM() ? IOS_NOMEM : IOS_OK;
				break;
			case UVID_FILE_ADD:
				FileAdd(&ioStatus,*((STDSTR*)((_UVFILE*)_out.uvp)->p),data,length);
				break;
			case UVID_FILE_WR:{
				_UVFILE	*uvfile = (_UVFILE*)_out.uvp;
				if (uvfile->cfg == 0){
					FileAdd(&ioStatus,*((STDSTR*)uvfile->p),data,length);
				}
				else{
					FileWrite(&ioStatus,*((STDSTR*)uvfile->p),data,length);
				}
				uvfile->cfg = 0;
				break;
			}
			default:;
				IOSTATUS_Add(_ios,ioStatus);
				return IOS_NOCONVERT;
		};
		IOSTATUS_Add(_ios,ioStatus);
	}
	return(ioStatus.status);
};
//------------------------------------------------------------------------------------------//
ioss DS_IO_NODE::FileAdd(IOSTATUS* _ios,const STDSTR& name,const uint8* data,const uint64& length){
	std::fstream	fileStream;
		
	fileStream.open(name.c_str(),std::ios::out|std::ios::app|std::ios::binary);
	fileStream.write((char*)data,length);
	fileStream.flush();
	fileStream.close();
	
	if (_ios != nullptr){
		_ios->total_in += length;
		_ios->avail_in = 0;
		_ios->avail_out = -1;
		_ios->total_out += length;
		_ios->status = IOS_OK;
	}
	return IOS_OK;
}
//------------------------------------------------------------------------------------------//
ioss DS_IO_NODE::FileWrite(IOSTATUS* _ios,const STDSTR& name,const uint8* data,const uint64& length){
	std::fstream	fileStream;
	
	fileStream.open(name.c_str(),std::ios::out|std::ios::trunc|std::ios::binary);
	fileStream.write((char*)data,length);
	fileStream.flush();
	fileStream.close();
	
	if (_ios != nullptr){
		_ios->total_in += length;
		_ios->avail_in = 0;
		_ios->avail_out = -1;
		_ios->total_out += length;
		_ios->status = IOS_OK;
	}
	return IOS_OK;
}
//------------------------------------------------------------------------------------------//
ioss DS_IO_NODE::BaseGetInLength(uint64* num,const UVIn& _in){
	*num = 0;
	if (_in.uvp == nullptr)
		return IOS_OK;
	
	switch (_in.uvid) {
		case UVID_NONE:
			break;
		case UVID_CHARS:{
			_UVCHARS* uvp = (_UVCHARS*)_in.uvp;
			*num = uvp->num;
			break;
		}
		case UVID_STR:{
			STDSTR* str = (STDSTR*)_in.uvp;
			*num = str->length();
			break;
		}
		case UVID_ARRAY:{
			uint32 length,offset,slength;
			_UVARRAY* uvp = (_UVARRAY*)_in.uvp;
			length = uvp->num;
			offset = uvp->offset;
			slength = ((ARRAY*)uvp->p)->CalcOutLength(length,offset);
			*num = length + slength;
			break;
		}
		case UVID_FILE:{
			_UVFILE* uvp = (_UVFILE*)_in.uvp;
			*num = 0;
			if (CFS_CheckFile(*((STDSTR*)uvp->p)))
				*num = CFS_CheckFileSize(*((STDSTR*)uvp->p));
			break;
		}
		default:;
			return IOS_NOCONVERT;
	};
	return IOS_OK;
};
//------------------------------------------------------------------------------------------//










//------------------------------------------------------------------------------------------//
DSTF::DS_TRANSFORM_FRAME(void) : DS_IO_NODE(){
	cgStartup.ios	= nullptr;
	cgStartup.uvOut	= nullptr;
	TNFP::SetSelfName("DSTF");
};
//------------------------------------------------------------------------------------------//
#endif /* DS_Transform_h */
