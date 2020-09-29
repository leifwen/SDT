//
//  ImplicitFishhook.m
//  SDT
//
//  Created by Leif Wen on 2020/9/25.
//  Copyright © 2020 Leif Wen. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "fishhook.h"

static void (*orig_nslog) (NSString *format, ...);
static void (*orig_nslogv)(NSString *format,va_list args);

void Blank_nslog (NSString *format, ...)			{;};
void Blank_nslogv(NSString *format,va_list args)	{/*orig_nslogv(format,args);*/;};

struct rebinding nslog_rebinding = {"NSLog",  Blank_nslog, (void*)&orig_nslog};
struct rebinding nslogv_rebinding = {"NSLogv",Blank_nslogv,(void*)&orig_nslogv};

void Disable_NSLOG(void) {
	rebind_symbols((struct rebinding[1]){nslog_rebinding}, 1);
}

void Disable_NSLOGV(void) {
	rebind_symbols((struct rebinding[1]){nslogv_rebinding}, 1);
}

void Disable_NSLOG_ALL(void) {
	Disable_NSLOG();
	Disable_NSLOGV();
}

void Test_NSLOG(void) {
	NSLog(@"%@", [NSString stringWithFormat:@"test - %s","this is a test"]);
}
