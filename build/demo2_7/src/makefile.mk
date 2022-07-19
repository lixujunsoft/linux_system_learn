#makefile.mk
ifndef TARGET
TARGET:=test
endif
CXXFLAGS:=-I../../include -std=c++17 #g++ -c 编译 自动推导
LDFLAGS:=-L../xcom -L../xthread #链接 可用于自动推导
LDLIBS:=-lpthread #链接库 用于自动推导
SRCS:=$(wildcard *.cpp *.cc *.c) #test_include.cpp testcpp.cc testc.c
OBJS:=$(patsubst %.cpp,%.o,$(SRCS)) #test_include.o testcpp.cc testc.c
OBJS:=$(patsubst %.cc,%.o,$(OBJS))
OBJS:=$(patsubst %.c,%.o,$(OBJS)) #test_include.o testcpp.o testc.o

$(TARGET):$(OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@  $(LDLIBS)
#rm -r test.o test
clean:
	$(RM) $(OBJS)  $(TARGET) 

.PHONY: clean  #伪目标 没有对应的文件

