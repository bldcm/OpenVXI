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
# VXIinterfaces, OpenSpeech Browser VXI interfaces
# UNIX make file 
#
#
#**********************************************************************/

#--------------------------------
# Required Environment Variables
#--------------------------------
SHELL = /bin/sh

ifndef SWISBSDK
$(error The environment variable SWISBSDK is not defined (it should point to your baseline directory))
endif

all: mkalldirs headers

clean:
	@echo "nothing to be cleaned!"

mkalldirs:
	mkdir -p $(SWISBSDK)/include
	mkdir -p $(SWISBSDK)/bin
	mkdir -p $(SWISBSDK)/lib
	mkdir -p $(SWISBSDK)/config
	  
headers:
	cp -f *.h $(SWISBSDK)/include
  
