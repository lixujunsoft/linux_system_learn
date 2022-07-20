#makefile.mk
ifndef TARGET
#  /root/make/src/test_include
# notdir
TARGET:=$(notdir $(shell pwd)) #test_include
endif
CXXFLAGS:=$(CXXFLAGS) -I../../include -std=c++17 #g++ -c 编译 自动推导
LDFLAGS:=$(LDFLAGS) #链接 可用于自动推导
LDLIBS:=$(LDLIBS) -lpthread #链接库 用于自动推导


SRCS:=$(wildcard *.cpp *.cc *.c) #test_include.cpp testcpp.cc testc.c
OBJS:=$(patsubst %.cpp,%.o,$(SRCS)) #test_include.o testcpp.cc testc.c
OBJS:=$(patsubst %.cc,%.o,$(OBJS))
OBJS:=$(patsubst %.c,%.o,$(OBJS)) #test_include.o testcpp.o testc.o

#区分动态库 静态库 和执行程序
ifeq ($(LIBTYPE),.so) #动态库 $(strip $(TARGET)) 去掉前后空格\t 
	TARGET:=lib$(strip $(TARGET)).so  
	LDLIBS:=$(LDLIBS) -shared
	CXXFLAGS:=$(CXXFLAGS) -fPIC
endif
ifeq ($(LIBTYPE),.a) #静态库 
	TARGET:=lib$(strip $(TARGET)).a  
endif


#目标生成
$(TARGET):$(OBJS)
ifeq ($(LIBTYPE),.a) #静态库 
	$(AR) -cvr $@ $^
else
	$(CXX) $(LDFLAGS) $^ -o $@  $(LDLIBS)
endif

#rm -r test.o test
#目标清理
clean:
	$(RM) $(OBJS)  $(TARGET) 

.PHONY: clean  #伪目标 没有对应的文件

