#***************License************************************************
#
# Copyright 2000-2002.  SpeechWorks International, Inc.  All rights reserved.
#
# Use of this software is subject to certain restrictions and limitations
# set forth in a license agreement entered into between SpeechWorks
# International Inc. and the licensee of this software.  Please refer 
# to the license agreement for license use rights and restrictions. 
#
# SpeechWorks is a registered trademark, and SpeechWorks Here,
# DialogModules and the SpeechWorks logo are trademarks of SpeechWorks
# International, Inc. in the United States and other countries.
# 
#**********************************************************************
#
# VXIvalue
# UNIX make file 
#
#
#**********************************************************************/

#--------------------------------
# Required Environment Variables
#--------------------------------
SHELL = /bin/sh

PRODUCT_LIB_PREFIX=SB

ifndef SWISBSDK
$(error The environment variable SWISBSDK is not defined (it should point to your baseline directory))
endif

#--------------------
# Platform macros
#--------------------
PROJ_ROOT = ..

CPU = i386
OS = linux
PLATFORM = $(CPU)-$(OS)

MAKE_CPU = $(CPU)
MAKE_OS = $(OS)

PERL = $(shell which perl)
MAKEFILE = Makefile

CC = gcc
CXX = g++

#-----------------------------------------
# Set up macros for different config's
#-----------------------------------------
ifndef CFG
CFG = debug
endif

ifeq ("$(CFG)","debug")
CFG_SUFFIX = D
CFLAGS_CFG = $(CFLAGS_DEBUG)
else
ifeq ("$(CFG)","release")
CFG_SUFFIX =
CFLAGS_CFG = $(CFLAGS_RELEASE)
else
$(error CFG is not a legal type - should be debug or release)
endif
endif

#---------------------
# Directories
#---------------------
PWD := $(shell pwd)
BUILD_ROOT := build
BUILD_ROOT_PLT = $(BUILD_ROOT)/$(PLATFORM)
BUILDDIR = $(BUILD_ROOT_PLT)/$(CFG)
PLATFORMDIR = $(PROJ_ROOT)/$(PLATFORM)
SRCPATH = $(PLATFORMDIR)/src

#
# SWISBSDK Install Directories
#
bindir = $(SWISBSDK)/bin
libdir = $(SWISBSDK)/lib
incdir = $(SWISBSDK)/include

#
# Build Area directories
#
buildbindir = $(BUILDDIR)/bin
buildlibdir = $(BUILDDIR)/lib

#------------------------
# C/C++ compiler flags
#------------------------
CC = gcc
CXX = g++

CFLAGS_GENERAL = -D_$(CPU)_ \
	         -D_$(OS)_ \
	         -I$(PROJ_ROOT)/$(PLATFORM)/include \
	         -I$(PROJ_ROOT)/include \
	         -I$(SWISBSDK)/include \
	         -I./include \
	         -I./ \
	         -D_SIMPLE_R \
	         -D_REENTRANT \
	         -D_GNU_SOURCE \
	         -D__THREADED

ANSIFLAGS = -pedantic \
	    -Wall \
	    -Wcast-qual \
	    -Wcast-align \
	    -Wmissing-prototypes \
	    -Wstrict-prototypes \
	    -Wtraditional

C_ANSIFLAGS = $(ANSIFLAGS) \
	      -Wid-clash-31

CXX_ANSIFLAGS = $(ANSIFLAGS)

ifdef STRICT
CFLAGS_GENERAL += $(ANSIFLAGS)
endif

# -g     debug symbols
# -O0 	 No Optimization for debug
# -fPIC  Generate Position Independant code
CFLAGS_DEBUG = -g -O0 -fPIC

# -s     strip symbols
# -O2    Optimize for release
# -fPIC  Generate Position Independant code
CFLAGS_RELEASE = -DNDEBUG -s -O2 -fPIC

#----------------------------------------
# Link Flags
#----------------------------------------
LDFLAGS += -L$(buildlibdir) $(PROJ_LDFLAGS) $($(TARGET)_LDFLAGS) $(LDFLAGS_$(OS)) -L$(SWISBSDK)/lib
LDFLAGS_SHARED = -shared
LDLIBS += $($(TARGET)_LDLIBS)
ifdef STATIC
LDLIBS += $($(TARGET)_LDLIBS_STATIC)
endif

LDLIBS += -lstdc++ -lpthread -ldl
LDFLAGS += -Wl,-rpath-link=$(SWISBSDK)/lib:$(PROJ_RPATH_LINK)

#--------------------------------
# General Programs
#--------------------------------
CHMOD = chmod
CHMODFLAGS = go-w,a-x
CHMODFLAGSX = go-w,a+x
INSTALL = cp -p
INSTALL_PROGRAM = $(INSTALL)
INSTALL_DATA = $(INSTALL)
MKDIRS = ../VXIinterfaces/mkinstalldirs
RMDIR = rmdir
RMCLEAN = rm -rf

#-------------------------------------------
# Final Values
#-------------------------------------------
CFLAGS = $(CFLAGS_CFG) $(CFLAGS_GENERAL) $(MYCFLAGS)
CXXFLAGS = $(CFLAGS_CFG) $(CFLAGS_GENERAL) $(MYCFLAGS)
CPPFLAGS = $(PROJ_CFLAGS) $(CFLAGS_$(OS)) $($(TARGET)_CFLAGS)

PROJ_CFLAGS = \
	 -DCOMPANY_DOMAIN=L\"com.speechworks\" \
	 -DMODULE_PREFIX=L\"swi:\"

#============================================================================
# Build sources
#============================================================================
PUBLIC_HEADERS =

# Mask the exports of shared objects by explicitly listing public functions
EXPLICIT_VER_FILES = 1

# Set defaults for the library version and product name burnt into
# shared libraries
LIBVER = 3

# Define libaries to be build
LIBS = VXIvalue

# Define library sources
VXIvalue_SRC = \
        ValueBasic.cpp \
        ValueToString.cpp
ifdef NO_STL
VXIvalue_SRC += ValueNoSTL.cpp
else
VXIvalue_SRC += ValueSTL.cpp
endif

# Define any linked libraries
VXIvalue_LDLIBS =

# Define version file
VXIvalue_VER = VXIvalue.ver


#============================================================================
# Build Rules
#============================================================================
all_SRC := $(sort $(foreach target, $(PROGS) $(STATIC_LIBS) $(LIBS), $($(target)_SRC)))

progsubdirs := $(sort ./ $(foreach target, $(PROGS), $(dir $($(target)_SRC))))
libsubdirs := $(sort ./ $(foreach target, $(STATIC_LIBS) $(LIBS), $(dir $($(target)_SRC))))

#-------------------------------------------------
# Header file and Include Directories
#-------------------------------------------------
vpath lib%.a $(patsubst -L%, %, $(filter -L%, $(LDFLAGS))) \
	      $(subst :, , $(LD_LIBRARY_PATH)) $(STD_LIBRARY_PATH)

ifneq ($(SRCPATH),)
vpath %.c $(SRCPATH)
vpath %.cc $(SRCPATH)
vpath %.cpp $(SRCPATH)
vpath %.C $(SRCPATH)
vpath %.def $(SRCPATH)
vpath %.f $(SRCPATH)
vpath %.F $(SRCPATH)
vpath %.l $(SRCPATH)
vpath %.mod $(SRCPATH)
vpath %.p $(SRCPATH)
vpath %.r $(SRCPATH)
vpath %.s $(SRCPATH)
vpath %.sh $(SRCPATH)
vpath %.y $(SRCPATH)
endif

#--------------------------------------------------
# Default all target as all-shared
#--------------------------------------------------
all: all-shared

#--------------------------------------------------
# .PHONY target declares all possible rules
#--------------------------------------------------
.PHONY: $(PROGS) $(STATIC_LIBS) $(LIBS) \
	$(addsuffix -shared, $(STATIC_LIBS) $(LIBS)) \
	$(addsuffix -static, $(STATIC_LIBS) $(LIBS)) \
	$(addsuffix -install, $(STATIC_LIBS) $(LIBS) $(PROGS)) \
	$(addsuffix -install-static, $(STATIC_LIBS) $(LIBS)) \
	$(addsuffix -install-shared, $(STATIC_LIBS) $(LIBS)) \
	$(addsuffix -install-progs, $(PROGS)) \
	$(buildbindir)/installprogsubdirs $(buildlibdir)/installlibsubdirs \
	installbuildbindir installbuildlibdir \
	installbindir installlibdir \
	all progs libs-static libs-shared \
	all-libs all-static all-shared \
	all-install headers clean

all-static: $(addsuffix -install-static, $(STATIC_LIBS) $(LIBS)) \
	    $(addsuffix -install, $(PROGS))

all-shared: $(addsuffix -install-static, $(STATIC_LIBS)) \
	    $(addsuffix -install-shared, $(LIBS)) \
	    $(addsuffix -install, $(PROGS))

progs: $(addsuffix -progs, $(PROGS))

libs: libs-static libs-shared

libs-static: $(addsuffix -static, $(STATIC_LIBS) $(LIBS))

libs-shared: $(addsuffix -static, $(STATIC_LIBS)) $(addsuffix -shared, $(LIBS))

#---------------------------------------------------------------------------
# headers copies all the public headers and scripts to the build area
#         (normally automatically done when a library, DLL, or program is 
#         successfully build)
#---------------------------------------------------------------------------
headers: installincdir
ifneq ($(PUBLIC_HEADERS),)
	$(INSTALL) $(PUBLIC_HEADERS) $(incdir)
endif
ifneq ($(PUBLIC_HEADER_TREES),)
	$(INSTALL) -r $(PUBLIC_HEADER_TREES) "$(SWISBSDK)/include"
endif
ifneq ($(PUBLIC_SCRIPTS),)
	$(INSTALL) $(PUBLIC_SCRIPTS) "$(SWISBSDK)/bin"
endif
ifneq ($(PUBLIC_CONFIG_FILES),)
	$(INSTALL) $(PUBLIC_CONFIG_FILES) "$(SWISBSDK)/config"
endif
ifneq ($(PUBLIC_ERROR_FILES),)
	$(INSTALL) $(PUBLIC_ERROR_FILES) "$(SWISBSDK)/config"
endif

#---------------------------------------------------------------------
# clean deletes the entire directory tree for the current build config
#---------------------------------------------------------------------
clean: $(PROJ_CLEAN)
	$(RMCLEAN) $(BUILDDIR)
	@echo Cleaning up $(BUILDDIR)

#--------------------------------------------
# Rules to create (install) build subdirs
#--------------------------------------------
$(buildbindir)/installbinsubdirs:
	$(MKDIRS) $(addprefix $(dir $@), $(progsubdirs))
	-$(CHMOD) $(CHMODFLAGSX) $(addprefix $(dir $@), $(progsubdirs))

$(buildlibdir)/installlibsubdirs:
	$(MKDIRS) $(addprefix $(dir $@), $(libsubdirs))
	-$(CHMOD) $(CHMODFLAGSX) $(addprefix $(dir $@), $(libsubdirs))

installbuildbindir:
	$(MKDIRS) $(buildbindir)
	-$(CHMOD) $(CHMODFLAGSX) $(buildbindir)

installbuildlibdir:
	$(MKDIRS) $(buildlibdir)
	-$(CHMOD) $(CHMODFLAGSX) $(buildlibdir)

installbindir:
	$(MKDIRS) $(bindir)
	-$(CHMOD) $(CHMODFLAGSX) $(bindir)

installlibdir:
	$(MKDIRS) $(libdir)
	-$(CHMOD) $(CHMODFLAGSX) $(libdir)

installincdir:
	$(MKDIRS) $(incdir)
	-$(CHMOD) $(CHMODFLAGSX) $(incdir)

#-----------------------------------------------
# Targets 
#-----------------------------------------------
$(addsuffix -install, $(PROGS)): %-install: installbindir %-progs
	$(RM) $(bindir)/$*$(CFG_SUFFIX)
	$(INSTALL_PROGRAM) $(buildbindir)/$*$(CFG_SUFFIX) $(bindir)

$(addsuffix -install, $(STATIC_LIBS) $(LIBS)): %-install: %-install-static %-install-shared

$(addsuffix -install-static, $(STATIC_LIBS) $(LIBS)): %-install-static: installlibdir \
	                               %-static headers
	$(RM) $(libdir)/lib$*$(CFG_SUFFIX).a
	$(INSTALL_DATA) $(buildlibdir)/lib$*$(CFG_SUFFIX).a $(libdir)

$(addsuffix -install-shared, $(LIBS)): %-install-shared: installlibdir \
	                               %-shared headers
	$(RM) $(libdir)/lib$*$(CFG_SUFFIX).so.$(LIBVER)
	$(INSTALL_DATA) $(buildlibdir)/lib$*$(CFG_SUFFIX).so.$(LIBVER) $(libdir)
	$(RM) $(libdir)/lib$*$(CFG_SUFFIX).so
	ln -s lib$*$(CFG_SUFFIX).so.$(LIBVER) $(libdir)/lib$*$(CFG_SUFFIX).so

#-------------------------------------------
# Program Targets
#--------------------------------------------
$(PROGS): %: %-progs

$(addsuffix -progs, $(PROGS)): %-progs: installbuildbindir \
	            $(buildbindir)/installbinsubdirs
	$(MAKE) -f $(MAKEFILE) $(buildbindir)/$*$(CFG_SUFFIX) TARGET=$* \
	DEPS=$*_SRC

#--------------------------------------------
# Library Targets
#--------------------------------------------
$(STATIC_LIBS): %: %-static %-shared

$(LIBS): %: %-static %-shared

$(addsuffix -static, $(STATIC_LIBS) $(LIBS)): %-static: installbuildlibdir \
		     $(buildlibdir)/installlibsubdirs
	$(MAKE) -f $(MAKEFILE) $(buildlibdir)/lib$*$(CFG_SUFFIX).a TARGET=$* \
	DEPS=$*_SRC

LDEXPORTS =
ifdef EXPLICIT_VER_FILES
LDEXPORTS = --version-script $($(TARGET)_VER)
endif
ifndef BUILDING_STUB_SO
LDFOLLOWLINKS = -Wl,-h,$(notdir $@).$(LIBVER)
endif
LDFLAGS_SHARED +=  $(LDEXPORTS) $(LDFOLLOWLINKS)

$(addsuffix -shared, $(LIBS)): %-shared: installbuildlibdir \
		     $(buildlibdir)/installlibsubdirs 
	$(MAKE) -f $(MAKEFILE) $(buildlibdir)/lib$*$(CFG_SUFFIX).so TARGET=$* \
	DEPS=$*_SRC

#-----------------------------------------
# Recursive rules
#------------------------------------------
$(addprefix $(buildbindir)/, $(addsuffix $(CFG_SUFFIX), $(PROGS))): \
	    $(addprefix $(buildbindir)/, \
	      $(patsubst %.s, %$(CFG_SUFFIX).o, $(filter %.s, $($(DEPS)))) \
	      $(patsubst %.c, %$(CFG_SUFFIX).o, $(filter %.c, $($(DEPS)))) \
	      $(patsubst %.cc, %$(CFG_SUFFIX).o, $(filter %.cc, $($(DEPS)))) \
	      $(patsubst %.cpp, %$(CFG_SUFFIX).o, $(filter %.cpp, $($(DEPS)))) \
	      $(patsubst %.C, %$(CFG_SUFFIX).o, $(filter %.C, $($(DEPS)))))
	      $(LINK.cc) $(filter %$(CFG_SUFFIX).o, $^) -o $@ $(LDLIBS) $(PROJ_LDLIBS) $(LDLIBS_$(OS))
	      -$(CHMOD) $(CHMODFLAGSX) $@

$(addprefix $(buildlibdir)/, $(patsubst %, lib%$(CFG_SUFFIX).a, $(STATIC_LIBS) $(LIBS))): \
	    $(addprefix $(buildlibdir)/, \
	      $(patsubst %.s, %$(CFG_SUFFIX).o, $(filter %.s, $($(DEPS)))) \
	      $(patsubst %.c, %$(CFG_SUFFIX).o, $(filter %.c, $($(DEPS)))) \
	      $(patsubst %.cc, %$(CFG_SUFFIX).o, $(filter %.cc, $($(DEPS)))) \
	      $(patsubst %.cpp, %$(CFG_SUFFIX).o, $(filter %.cpp, $($(DEPS)))) \
	      $(patsubst %.C, %$(CFG_SUFFIX).o, $(filter %.C, $($(DEPS)))))
	$(RM) $@
	$(AR) $(ARFLAGS) $@ $^
	-$(CHMOD) $(CHMODFLAGS) $@

$(addprefix $(buildlibdir)/, $(patsubst %, lib%$(CFG_SUFFIX).so, $(LIBS))): \
	    $(addprefix $(buildlibdir)/, \
	      $(patsubst %.s, %$(CFG_SUFFIX).o, $(filter %.s, $($(DEPS)))) \
	      $(patsubst %.c, %$(CFG_SUFFIX).o, $(filter %.c, $($(DEPS)))) \
	      $(patsubst %.cc, %$(CFG_SUFFIX).o, $(filter %.cc, $($(DEPS)))) \
	      $(patsubst %.cpp, %$(CFG_SUFFIX).o, $(filter %.cpp, $($(DEPS)))) \
	      $(patsubst %.C, %$(CFG_SUFFIX).o, $(filter %.C, $($(DEPS)))))

	$(LINK.cc) $(LDFLAGS_SHARED) $(filter %$(CFG_SUFFIX).o, $^) -o $@.$(LIBVER) $(LDLIBS)
	-$(CHMOD) $(CHMODFLAGSX) $@.$(LIBVER)
	$(RM) $@
	ln -s $(patsubst $(buildlibdir)/%,%, $@).$(LIBVER) $@

#---------------------------------------
# Object File compilation Rules
#----------------------------------------

$(buildbindir)/%$(CFG_SUFFIX).o: %.c
	$(COMPILE.c)  $(addprefix ${PWD}/,$<) -o $@
	-$(CHMOD) $(CHMODFLAGS) $@

$(buildbindir)/%$(CFG_SUFFIX).o: %.cc
	$(COMPILE.cc)  $(addprefix ${PWD}/, $<) -o $@
	-$(CHMOD) $(CHMODFLAGS) $@

$(buildbindir)/%$(CFG_SUFFIX).o: %.cpp
	$(COMPILE.cc)  $(addprefix ${PWD}/, $<) -o $@
	-$(CHMOD) $(CHMODFLAGS) $@

$(buildbindir)/%$(CFG_SUFFIX).o: %.C
	$(COMPILE.C)  $(addprefix ${PWD}/, $<) -o $@
	-$(CHMOD) $(CHMODFLAGS) $@

$(buildlibdir)/%$(CFG_SUFFIX).o: %.c
	$(COMPILE.c)  $(addprefix ${PWD}/, $<) -o $@ 
	-$(CHMOD) $(CHMODFLAGS) $@

$(buildlibdir)/%$(CFG_SUFFIX).o: %.cc
	$(COMPILE.cc) $(addprefix ${PWD}/, $<) -o $@
	-$(CHMOD) $(CHMODFLAGS) $@

$(buildlibdir)/%$(CFG_SUFFIX).o: %.cpp
	$(COMPILE.cc) $(addprefix ${PWD}/, $<) -o $@
	-$(CHMOD) $(CHMODFLAGS) $@

$(buildlibdir)/%$(CFG_SUFFIX).o: %.C
	$(COMPILE.C)  $(addprefix ${PWD}/, $<) -o $@
	-$(CHMOD) $(CHMODFLAGS) $@

$(buildlibdir)/%$(CFG_SUFFIX).o: %.s
	$(AS) $(ASFLAGS) -o $@ $<
	-$(CHMOD) $(CHMODFLAGS) $@
