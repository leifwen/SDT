//
//  ODEV_System.hpp
//  SDT
//
//  Created by Leif Wen on 31/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#ifndef ODEV_BUS_hpp
#define ODEV_BUS_hpp
//------------------------------------------------------------------------------------------//
#include "ODEV_BUS.h"
#ifdef ODEV_BUS_h
//------------------------------------------------------------------------------------------//
inline ODEV_STDOUT*	ODEV_CACHE::GetG1_STDOUT	(void)const {return(defSTDOUT);};
inline ODEV_FILE*	ODEV_CACHE::GetG2_File		(void)const {return(defFile);};

inline OUTPUT_NODE*	ODEV_VG3D::GetDefODEV		(void)const {return(defODEV);};
inline ODEV_STDOUT*	ODEV_VG3D::GetDefSTDOUT		(void)const {return(defSTDOUT);};
inline ODEV_FILE*	ODEV_VG3D::GetDefFile		(void)const {return(defFile);};
//------------------------------------------------------------------------------------------//
#endif /* ODEV_BUS_h */
#endif /* ODEV_BUS_hpp */
