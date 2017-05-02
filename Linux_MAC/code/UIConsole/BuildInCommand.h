//
//  BuildInCommand.hpp
//  SDT
//
//  Created by Leif Wen on 01/05/2017.
//  Copyright © 2017 leif wen. All rights reserved.
//

//------------------------------------------------------------------------------------------//
#include "BIC_CMD.h"
//------------------------------------------------------------------------------------------//
#ifdef BIC_CMDH
#ifndef BuildInCommandH
#define BuildInCommandH
#ifdef BuildInCommandH
//------------------------------------------------------------------------------------------//
class BuildInCommand : public BIC_CMD{
	public:
			 BuildInCommand(void) : BIC_CMD(){;}
	virtual ~BuildInCommand(void){;};
	public:
		int32	Do		(BIC_ENV *env);
};
//------------------------------------------------------------------------------------------//
#endif
#endif
#endif
