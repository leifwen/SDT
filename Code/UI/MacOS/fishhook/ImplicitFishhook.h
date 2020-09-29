//
//  ImplicitFishhook.h
//  SDT
//
//  Created by Leif Wen on 2020/9/25.
//  Copyright © 2020 Leif Wen. All rights reserved.
//

#ifndef ImplicitFishhook_h
#define ImplicitFishhook_h

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

extern void Disable_NSLOG(void);
extern void Disable_NSLOGV(void);
extern void Disable_NSLOG_ALL(void);
extern void Disable_printf(void);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif /* ImplicitFishhook_h */
