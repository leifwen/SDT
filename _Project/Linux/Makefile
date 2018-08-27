# A generic template Makefile
# Author Red_Liu lli_njupt@163.com v0.1
#
# This file is a(part of) free software; you can redistribute it 
# and/or modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2, or 
# (at your option) any later version.
# modify by Leif wen@2013.11.02, used for C++.

.PHONY = all install clean

#compile option
ISSTATIC		?= no

#global directory defined
TOPDIR			= $(shell pwd)/../..
SRCDIR			= $(TOPDIR)
LW_LibSRCDIR	= $(SRCDIR)/LW_BaseLib/src
LW_LibINCDIR	= $(SRCDIR)/LW_BaseLib/inc
LW_ExLibINCDIR	= $(SRCDIR)/ExtraLib/zlib/1.2.11/include
LW_ExLibINCDIR	+= $(SRCDIR)/ExtraLib/Openssl/1.0.2o/include
kernelSRCDIR	= $(SRCDIR)/Code/kernel/src
kernelINCDIR	= $(SRCDIR)/Code/kernel/inc
UISRCDIR		= $(SRCDIR)/Code/UI/MacOS
UIINCDIR		= $(SRCDIR)/Code/UI/MacOS
LW_ExLIBDIR1	= $(SRCDIR)/ExtraLib/Openssl/1.0.2o/lib/Linux_3.18.118
LW_ExLIBDIR2	= $(SRCDIR)/ExtraLib/zlib/1.2.11/lib/Linux_3.18.118
OBJECTDIR		= $(shell pwd)/build

#cross compile tools defined 
ARCH			?= 
CROSS_COMPILE	?= 
AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)g++
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm
STRIP	= $(CROSS_COMPILE)strip
RANLIB	= $(CROSS_COMPILE)ranlib

#local host tools defined
CP		:= cp
RM		:= rm
MKDIR	:= mkdir
SED		:= sed
FIND	:= find
XARGS	:= xargs

#target name
TARGETMAIN	= sdt
TARGETLIBS	= libmk.a
TARGETSLIBS	= libmk.so

#FILE' INFOMATION COLLECT
LW_LibSRCPATCH		= $(shell ls -AxR $(LW_LibSRCDIR)|grep ":"|grep -v "\.svn"|tr -d ':')
LW_LibINCPATCH		= $(shell ls -AxR $(LW_LibINCDIR)|grep ":"|grep -v "\.svn"|tr -d ':')
kernelSRCPATCH		= $(shell ls -AxR $(kernelSRCDIR)|grep ":"|grep -v "\.svn"|tr -d ':')
kernelINCPATCH		= $(shell ls -AxR $(kernelINCDIR)|grep ":"|grep -v "\.svn"|tr -d ':')
UISRCPATCH			= $(shell ls -AxR $(UISRCDIR)|grep ":"|grep -v "\.svn"|tr -d ':')
UIINCPATCH			= $(shell ls -AxR $(UIINCDIR)|grep ":"|grep -v "\.svn"|tr -d ':')

SOURCEDIRS	= $(LW_LibSRCPATCH) + $(kernelSRCPATCH) + $(UISRCPATCH)
INCLUDEDIRS	= $(LW_LibINCPATCH) + $(kernelINCPATCH) + $(UIINCPATCH) + $(LW_ExLibINCDIR)

#search source file in the current dir
SOURCES		= $(foreach subdir,$(SOURCEDIRS),$(wildcard $(subdir)/*.cpp))
SRCOBJS		= $(patsubst %.cpp,%.o,$(SOURCES))
BUILDOBJS	= $(subst $(SRCDIR),$(OBJECTDIR),$(SRCOBJS))
DEPS		= $(patsubst %.o,%.d,$(BUILDOBJS))

#external include file define
CFLAGS	= -O2 -Wall -MD $(foreach dir,$(INCLUDEDIRS),-I$(dir))
ARFLAGS = rc

#special parameters for apps
CFLAGS	+=

#c file compile parameters and linked libraries
CPPFLAGS	= -Wall -std=c++0x -ggdb

ifeq ($(ISSTATIC), yes)
CPPFLAGS	+= -static
endif

LDFLAGS	= -lssl -lcrypto -ldl -lz
ifeq ($(ARCH), arm)
CPPFLAGS	+= -Wl,--whole-archive -lpthread -Wl,--no-whole-archive -lc
else
LDFLAGS		+= -lpthread
endif

ifeq ($(ARCH), arm)
LDFLAGS		+= -latomic
endif
XLDFLAGS	= -Xlinker "-(" $(LDFLAGS) -Xlinker "-)"
ifeq ($(ARCH), arm)
LDLIBS		= -L $(LW_ExLIBDIR1) -L $(LW_ExLIBDIR2) 
else
LDLIBS		?=
endif

#defaut target:compile the currrent dir file and sub dir 
all:  $(TARGETMAIN)

#for .h header files dependence
-include $(DEPS)

$(TARGETMAIN) :$(BUILDOBJS)
	@$(CC) $(subst $(SRCDIR),$(OBJECTDIR),$^) $(CPPFLAGS) $(CFLAGS) $(XLDFLAGS) -o $@ $(LDLIBS) 
	@$(STRIP)  --strip-unneeded $(TARGETMAIN)

$(TARGETLIBS) :$(BUILDOBJS)
	@$(AR) $(ARFLAGS) $@ $(BUILDOBJS)
	@$(RANLIB) $@

$(TARGETSLIBS) :$(BUILDOBJS)
	@$(CC) -shared $(subst $(SRCDIR),$(OBJECTDIR),$^) $(CPPFLAGS) $(CFLAGS) $(XLDFLAGS) -o $@ $(LDLIBS)

$(OBJECTDIR)%.o: $(SRCDIR)%.cpp
	@[ ! -d $(dir $(subst $(SRCDIR),$(OBJECTDIR),$@)) ] & $(MKDIR) -p $(dir $(subst $(SRCDIR),$(OBJECTDIR),$@))
	@$(CC) $(CPPFLAGS) $(CFLAGS) -o $(subst $(SRCDIR),$(OBJECTDIR),$@) -c $<
	
intall:

clean:
	@$(FIND) $(OBJECTDIR) -name "*.o" -o -name "*.d" | $(XARGS) $(RM) -f
	@$(RM) -f $(TARGETMAIN) $(TARGETLIBS) $(TARGETSLIBS)
