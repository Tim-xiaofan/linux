#Makefile for Makefile_test
#编译器选项
CC=gcc
CFLAGS=-Wall -g
#APP名字
APP=Makefile_test
#源文件
SRCS=main.c ./m_include/a.c ./m_include/b.c ./m_include/c.c
#头文件
INC=-I./m_include
#lib
LIBS=-lpthread
#中间文件
OBJS=$(SRCS:.c=.o)
#编译
$(APP):$(OBJS)
#	@echo APP:$@
#	@echo OBJECTS:$^
	$(CC) -o $(APP)  $^ $(LIBS)
%.o:%.c	
	$(CC) $(CFLAGS) $(INC) -o $@ -c $< 
clean:
	rm -rf $(TARGET) $(OBJS)
install:
	cp $(APP) /usr/bin
