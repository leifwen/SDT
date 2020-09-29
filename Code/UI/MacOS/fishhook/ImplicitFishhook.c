//
//  ImplicitFishhook.c
//  SDT
//
//  Created by Leif Wen on 2020/9/25.
//  Copyright © 2020 Leif Wen. All rights reserved.
//

#include <stdio.h>
#include "fishhook.h"

static void (*orig_printf)(const char *path, ...);

void Blank_printf(const char *path,...) {;};

struct rebinding printf_rebinding = {"printf",Blank_printf,(void*)&orig_printf};

void Disable_printf(void) {
	rebind_symbols((struct rebinding[1]){printf_rebinding}, 1);
}
