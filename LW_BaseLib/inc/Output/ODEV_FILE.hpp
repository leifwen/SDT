//
//  ODEV_FILE.hpp
//  SDT
//
//  Created by Leif Wen on 31/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#ifndef ODEV_FILE_hpp
#define ODEV_FILE_hpp
//------------------------------------------------------------------------------------------//
#include "ODEV_FILE.h"
#ifdef ODEV_FILE_h
//------------------------------------------------------------------------------------------//
inline void	ODEV_FILE::SetFileName(const STDSTR& filename){
	cgfileName = filename;
};
//------------------------------------------------------------------------------------------//
inline const STDSTR& ODEV_FILE::GetFileName(void)const{
	return(cgfileName);
};
//------------------------------------------------------------------------------------------//
#endif /* ODEV_FILE_h */
#endif /* ODEV_FILE_hpp */
