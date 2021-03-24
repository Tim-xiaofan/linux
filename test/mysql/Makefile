# SPDX-License-Identifier: BSD-3-Clause
# Copyright(c) 2010-2014 Intel Corporation

# binary name
APP = mysql

# all source are stored in SRCS-y
SRCS-y := mysql.c

ifeq ($(OS),Windows_NT)
		PLATFORM="Windows"
		SHELL=powershell.exe
else 
ifeq ($(shell uname),Darwin)
		PLATFORM="MacOS"
else 
ifeq ($(shell uname),Linux)
		PLATFORM="Unix-Like"
endif
endif
endif

# Add flag 
CLAGS += -g -Wall
MYSQLFLAGS += $(shell mysql_config --cflags)
MYSQLLIBS += $(shell mysql_config --libs)

$(APP): $(SRCS-y) Makefile 
	$(CC) $(CLAGS) $(MYSQLFLAGS) $(SRCS-y) -o $@ $(MYSQLLIBS)


ifeq ($(shell uname),Linux)
.PHONY: clean
clean:
	echo "Linux rm"
	rm -f *.o
	rm -f *.exe
	rm -f $(APP)
else 
ifeq ($(OS),Windows_NT)
SHELL=powershell.exe
.PHONY: clean
clean:
	echo "Windows rm"
	rm -Force *.o
	rm -Force *.exe
	rm -Force $(APP)
endif
endif
