//
//  DS_Transform.cpp
//  SDT
//
//  Created by Leif Wen on 19/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

#include "stdafx.h"
//------------------------------------------------------------------------------------------//
#include "DS_Transform.h"
#include "DS_STRING.h"
#include "SYS_File.h"
//------------------------------------------------------------------------------------------//
#ifdef DS_Transform_h
//------------------------------------------------------------------------------------------//
STDSTR _GetIOSEMesg(IOSE code){
	STDSTR retStr;
	retStr = "(" + Str_ToStr(code)+ ")";

	switch(code){
		case IOS_TIMEOUT 	:retStr += "IOS_TIMEOUT";	break;
		case IOS_NO_MEM		:retStr += "IOS_NO_MEM";	break;
		case IOS_NO_OBJECT	:retStr += "IOS_NO_OBJECT";	break;
		case IOS_ERR		:retStr += "IOS_ERR";		break;
		case IOS_OK			:retStr += "IOS_OK";		break;
		case IOS_FINISH		:retStr += "IOS_FINISH";	break;
		default				:retStr += "IOS_UNDEFINE";	break;
	}
	return(retStr);
};
//------------------------------------------------------------------------------------------//
DS_IO_NODE::DS_IO_NODE(void) : TNFP(){
	ClrSFlag(DSTF_blStart);
	TNFP::SetSelfName("DS_IO_NODE");
};
//------------------------------------------------------------------------------------------//
IOSE DS_IO_NODE::DoConvert(DSIO* ioNode,IOS* _ios,const UVOut& _out,const UVIn& _in){
	IOSE rcode;
	rcode = BaseConvert(ioNode,_ios,_out,_in);
	if (rcode == IOS_NO_OBJECT){
		rcode = IOS_OK;
		TREE_DownChain_Traversal_LINE_nolock(DS_IO_NODE,this,
			rcode = _opNode->DoConvert(ioNode,_ios,_out,_in);
			if (rcode != IOS_NO_OBJECT)
				break;
			rcode = IOS_OK;
		);
	}
	return(IOS_update(_ios, rcode));
};
//------------------------------------------------------------------------------------------//
IOSE DS_IO_NODE::DoSave(IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	IOSE rcode;
	rcode = BaseSave(_ios,_out,data,length);
	if (rcode == IOS_NO_OBJECT){
		rcode = IOS_FINISH;
		TREE_DownChain_Traversal_LINE_nolock(DS_IO_NODE,this,
			rcode = _opNode->DoSave(_ios,_out,data,length);
			if (rcode != IOS_NO_OBJECT)
				break;
			rcode = IOS_FINISH;
		);
	}
	return(IOS_update(_ios, rcode));
};
//------------------------------------------------------------------------------------------//
IOSE DS_IO_NODE::DoGetUVInLength(uint64* num,const UVIn& _in){
	IOSE rcode;
	rcode = BaseGetUVInLen(num,_in);
	if (rcode == IOS_NO_OBJECT){
		rcode = IOS_OK;
		TREE_DownChain_Traversal_LINE_nolock(DS_IO_NODE,this,
			rcode = _opNode->DoGetUVInLength(num,_in);
			if (rcode != IOS_NO_OBJECT)
				break;
			rcode = IOS_OK;
		);
	}
	return(rcode);
};
//------------------------------------------------------------------------------------------//
IOSE DS_IO_NODE::DoAddUVInOffset(const UVIn& _in,const int64& length){
	IOSE rcode;
	rcode = BaseAddUVInOffset(_in,length);
	if (rcode == IOS_NO_OBJECT){
		rcode = IOS_OK;
		TREE_DownChain_Traversal_LINE_nolock(DS_IO_NODE,this,
			rcode = _opNode->DoAddUVInOffset(_in,length);
			if (rcode != IOS_NO_OBJECT)
				break;
			rcode = IOS_OK;
		);
	}
	return(rcode);
};
//------------------------------------------------------------------------------------------//
IOSE DS_IO_NODE::BaseConvert(DSIO* ioNode,IOS* _ios,const UVOut& _out,const UVIn& _in){
	IOSE	rcode = IOS_OK;
	
	if (_in.uvp == nullptr){
		rcode = ioNode->DoTransform(_ios,_out,nullptr,0);
	}
	else{
		switch (_in.uvid) {
			case UVID_NONE:
				break;
			case UVID_CHARS:{
				_UVCHARS* uvp = (_UVCHARS*)_in.uvp;
				uint8*	buf = (uint8*)uvp->p;
				buf += uvp->offset;
				rcode = ioNode->DoTransform(_ios,_out,buf,uvp->num - uvp->offset);
				break;
			}
			case UVID_STR:{
				UVIn* uvIn = (UVIn*)&_in;
				STDSTR* str = (STDSTR*)_in.uvp;
				uint8*	buf = (uint8*)str->c_str();
				buf += uvIn->uvOffset;
				rcode = ioNode->DoTransform(_ios,_out,buf,str->length() - uvIn->uvOffset);
				break;
			}
			case UVID_ARRAY0:{
				UVIn* uvIn = (UVIn*)&_in;
				ARRAY* array = (ARRAY*)_in.uvp;
				rcode = ArrayConvert(ioNode,_ios,_out,array,array->Used() - (uint32)uvIn->uvOffset,(uint32)uvIn->uvOffset);
				break;
			}
			case UVID_ARRAY1:{
				_UVARRAY* uvp = (_UVARRAY*)_in.uvp;
				rcode = ArrayConvert(ioNode,_ios,_out,(ARRAY*)uvp->p,uvp->num - uvp->offset,uvp->offset);
				break;
			}
			case UVID_FILE:{
				_UVFILE* uvp = (_UVFILE*)_in.uvp;
				rcode = FileConvert(ioNode,_ios,_out,*((STDSTR*)uvp->p),uvp->num - uvp->offset,uvp->offset);
				break;
			}
			default:;
				return IOS_NO_OBJECT;
		};
	}
	return(IOS_update(_ios, rcode));
};
//------------------------------------------------------------------------------------------//
IOSE DS_IO_NODE::ArrayConvert(DSIO* ioNode,IOS* _ios,const UVOut& _out,const ARRAY* _array,uint32 length,uint32 offset){
	uint32	slength;
	IOSE	rcode = IOS_OK;
	slength = _array->CalcOutLength(length,offset);
	if ((length > 0) || (_ios != nullptr)){
		IOS_update(&rcode,ioNode->DoTransform(_ios,_out,_array->GetPointer(offset),length));
		if (slength > 0)
			IOS_update(&rcode,ioNode->DoTransform(_ios,_out,_array->GetPointer(0),slength));
	}
	return(rcode);
};
//------------------------------------------------------------------------------------------//
IOSE DS_IO_NODE::FileConvert(DSIO* ioNode, IOS* _ios,const UVOut& _out,const STDSTR& name,uint64 length,const uint64& offset){
	uint32			num;
	uint8			data[1024];
	std::fstream	fileStream;
	IOSE			rcode;

	if (CFS_CheckFile(name) == G_FALSE)
		return(IOS_update(_ios,IOS_ERR));

	fileStream.open(name.c_str(),std::ios::in|std::ios::binary);
	if (offset > 0)
		fileStream.seekp(offset,std::ios::beg);
	rcode = IOS_OK;
	do{
		num = (sizeof(data) < length) ? sizeof(data) : (uint32)length;
		fileStream.read((char*)data,num);
		num = (uint32)fileStream.gcount();
		length -= num;
		IOS_update(&rcode,ioNode->DoTransform(_ios,_out,data,num));
	}while((rcode >= IOS_OK) && (!fileStream.eof()) && (length > 0));
	fileStream.close();
	return(rcode);
};
//------------------------------------------------------------------------------------------//
IOSE DS_IO_NODE::BaseSave(IOS* _ios,const UVOut& _out,const uint8* data,const uint64& length){
	G_SPACE	space;
	IOS		ios;
	IOS_clr(&ios);
	ios.rcode = IOS_FINISH;
	if (_out.uvid == UVID_UVVTRAN){
		DSIO* io = (DSIO*)((_UVTRAN*)_out.uvp)->iop;
		if (data != nullptr){
			ios.rcode = io->Transform(_ios,((_UVTRAN*)_out.uvp)->uvb,data,length);
		}
		else{
			ios.rcode = io->Final(_ios,((_UVTRAN*)_out.uvp)->uvb);
		}
	}
	else if (data != nullptr){
		ios.avail_in = length;
		space = G_SPACE_ON;
		switch (_out.uvid) {
			case UVID_NONE:
				ios.total_in = ios.avail_in;
				ios.avail_in -= ios.total_in;
				ios.avail_out = -1;
				ios.total_out = ios.total_in;
				break;
			case UVID_CHARS:{
				_UVCHARS	*array = (_UVCHARS*)_out.uvp;
				ios.total_in = (ios.avail_in < (array->num - array->offset)) ? ios.avail_in : (array->num - array->offset);
				if (ios.total_in > 0)
					memcpy((uint8*)array->p + array->offset,data,ios.total_in);
				ios.avail_in -= ios.total_in;
				ios.avail_out = array->num - array->offset - ios.total_in;
				ios.total_out = ios.total_in;
				array->offset += ios.total_out;
				ios.rcode = (ios.avail_in > 0) ? IOS_NO_MEM : IOS_OK;
				break;
			}
			case UVID_STR:{
				STDSTR* str = (STDSTR*)_out.uvp;
				if (ios.avail_in > 0)
					str->append((char*)data, ios.avail_in);
				ios.total_in = ios.avail_in;
				ios.avail_in -= ios.total_in;
				ios.avail_out = -1;
				ios.total_out = ios.total_in;
				break;
			}
			case UVID_STRHEX:
				space = G_SPACE_OFF;
			case UVID_STRHEXs:{
				STDSTR* str = (STDSTR*)_out.uvp;
				ios.total_out = str->length();
				if (ios.avail_in > 0)
					Str_CharToStr(str, data, ios.avail_in, G_HEX, space, G_ESCAPE_OFF);
				ios.total_in = ios.avail_in;
				ios.avail_in -= ios.total_in;
				ios.avail_out = -1;
				ios.total_out = str->length() - ios.total_out;
				break;
			}
			case UVID_ARRAY1:
				if ((_ios != nullptr) && (_ios->rcode == IOS_NO_MEM))
					break;
				ios.total_in = 0;
				if (ios.avail_in > 0)
					ios.total_in = ((ARRAY*)_out.uvp)->Put(data, (uint32)ios.avail_in);
				ios.avail_in -= ios.total_in;
				ios.avail_out = ((ARRAY*)_out.uvp)->Unused();
				ios.total_out = ios.total_in;
				ios.rcode = ((ARRAY*)_out.uvp)->IsNoMEM() ? IOS_NO_MEM : IOS_OK;
				break;
			case UVID_FILE_ADD:
				FileAdd(&ios,*((STDSTR*)((_UVFILE*)_out.uvp)->p),data,length);
				break;
			case UVID_FILE_WR:{
				_UVFILE	*uvfile = (_UVFILE*)_out.uvp;
				if (uvfile->cfg == 0){
					FileAdd(&ios,*((STDSTR*)uvfile->p),data,length);
				}
				else{
					FileWrite(&ios,*((STDSTR*)uvfile->p),data,length);
				}
				uvfile->cfg = 0;
				break;
			}
			case UVID_FILE:
				FileUpdate(&ios,*((STDSTR*)((_UVFILE*)_out.uvp)->p),data,length,((_UVFILE*)_out.uvp)->offset);
				break;
			default:;
				return IOS_NO_OBJECT;
		};
		IOS_update(_ios, ios);
	}
	return(IOS_update(_ios,ios.rcode));
};
//------------------------------------------------------------------------------------------//
IOSE DS_IO_NODE::FileAdd(IOS* _ios,const STDSTR& name,const uint8* data,const uint64& length){
	std::fstream	fileStream;
	
	if (length > 0){
		fileStream.open(name.c_str(),std::ios::out|std::ios::app|std::ios::binary);
		fileStream.write((char*)data,length);
		fileStream.flush();
		fileStream.close();
	}
	if (_ios != nullptr){
		_ios->total_in += length;
		_ios->avail_in = 0;
		_ios->avail_out = -1;
		_ios->total_out += length;
	}
	return(IOS_update(_ios,IOS_OK));
};
//------------------------------------------------------------------------------------------//
IOSE DS_IO_NODE::FileWrite(IOS* _ios,const STDSTR& name,const uint8* data,const uint64& length){
	std::fstream	fileStream;
	
	if (length > 0){
		fileStream.open(name.c_str(),std::ios::out|std::ios::trunc|std::ios::binary);
		fileStream.write((char*)data,length);
		fileStream.flush();
		fileStream.close();
	}
	if (_ios != nullptr){
		_ios->total_in += length;
		_ios->avail_in = 0;
		_ios->avail_out = -1;
		_ios->total_out += length;
	}
	return(IOS_update(_ios,IOS_OK));
};
//------------------------------------------------------------------------------------------//
IOSE DS_IO_NODE::FileUpdate(IOS* _ios,const STDSTR& name,const uint8* data,const uint64& length,const uint64& offset){
	std::fstream	fileStream;
	
	if (length > 0){
		fileStream.open(name.c_str(),std::ios::in|std::ios::out|std::ios::binary);
		fileStream.seekp(offset,std::ios::beg);
		fileStream.write((char*)data,length);
		fileStream.flush();
		fileStream.close();
	}
	if (_ios != nullptr){
		_ios->total_in += length;
		_ios->avail_in = 0;
		_ios->avail_out = -1;
		_ios->total_out += length;
	}
	return(IOS_update(_ios,IOS_OK));
};
//------------------------------------------------------------------------------------------//
IOSE DS_IO_NODE::BaseGetUVInLen(uint64* num,const UVIn& _in){
	*num = 0;
	if (_in.uvp != nullptr){
		switch (_in.uvid) {
			case UVID_NONE:
				break;
			case UVID_CHARS:{
				_UVCHARS* uvp = (_UVCHARS*)_in.uvp;
				*num = (uvp->num > uvp->offset) ? (uvp->num - uvp->offset) : 0;
				break;
			}
			case UVID_STR:{
				STDSTR* str = (STDSTR*)_in.uvp;
				*num = (str->length() > _in.uvOffset) ? (str->length() - _in.uvOffset) : 0;
				break;
			}
			case UVID_ARRAY0:{
				ARRAY* array = (ARRAY*)_in.uvp;
				*num = (array->Used() > _in.uvOffset) ? (array->Used() - _in.uvOffset) : 0;
				break;
			}
			case UVID_ARRAY1:{
				_UVARRAY* uvp = (_UVARRAY*)_in.uvp;
				*num = (uvp->num > uvp->offset) ? (uvp->num - uvp->offset) : 0;
				break;
			}
			case UVID_FILE:{
				_UVFILE* uvp = (_UVFILE*)_in.uvp;
				if (CFS_CheckFile(*((STDSTR*)uvp->p)))
					*num = CFS_CheckFileSize(*((STDSTR*)uvp->p));
				if (*num > uvp->num)
					*num = uvp->num;
				*num = (*num > uvp->offset) ? (*num - uvp->offset) : 0;
				break;
			}
			default:;
				return IOS_NO_OBJECT;
		};
	}
	return IOS_OK;
};
//------------------------------------------------------------------------------------------//
IOSE DS_IO_NODE::BaseAddUVInOffset(const UVIn& _in,const int64& length){
	if (_in.uvp != nullptr){
		switch (_in.uvid) {
			case UVID_NONE:
				break;
			case UVID_CHARS:{
				_UVCHARS* uvp = (_UVCHARS*)_in.uvp;
				uvp->offset += length;
				break;
			}
			case UVID_STR:
			case UVID_ARRAY0:{
				UVIn* uvIn = (UVIn*)&_in;
				uvIn->uvOffset += length;
				break;
			}
			case UVID_ARRAY1:{
				_UVARRAY* uvp = (_UVARRAY*)_in.uvp;
				uvp->offset += length;
				break;
			}
			case UVID_FILE:{
				_UVFILE* uvp = (_UVFILE*)_in.uvp;
				uvp->offset += length;
				break;
			}
			default:;
				return IOS_NO_OBJECT;
		};
	}
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
