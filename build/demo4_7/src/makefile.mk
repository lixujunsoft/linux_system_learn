#makefile.mk 公共头文件
ifndef TARGET
#  /root/make/src/test_include
# notdir
TARGET:=$(notdir $(shell pwd)) #test_include
endif
CXXFLAGS:=$(CXXFLAGS) -I../../include -std=c++17 #g++ -c 编译 自动推导
LDFLAGS:=$(LDFLAGS) #链接 可用于自动推导
LDLIBS:=$(LDLIBS) -lpthread #链接库 用于自动推导
#输出路径 /usr/bin  /usr/lib /usr/include
OUT:=/usr

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

#启动脚本
STARTSH=start_$(TARGET)
#停止脚本
STOPSH=stop_$(TARGET)

# $(1) TARGET ,$(2)  OUT
define Install 
	@echo "begin install "$(1)
	-mkdir -p $(2)
	cp $(1) $(2)
	@echo "end install "$(1)
endef

#生成启动停止脚本，并安装到$(OUT)
# $(1) TARGET ,$(2)  OUT,$(3) $PARAS
define InstallSH
	@echo "begin make start shell"
	echo "nohup $(1) $(3)  &" > $(STARTSH)
	chmod +x $(STARTSH)
	cp $(STARTSH) $(2)
	@echo "end make start shell"

	@echo "begin make stop shell"
	echo killall $(1) >$(STOPSH)
	cp $(STOPSH) $(2)
	@echo "end make stop shell"
endef
all:depend $(TARGET)
depend:
	@$(CXX) $(CXXFLAGS) -MM $(SRCS) >.depend
-include .depend

#目标生成
$(TARGET):$(OBJS)
ifeq ($(LIBTYPE),.a) #静态库 
	$(AR) -cvr $@ $^
else
	$(CXX) $(LDFLAGS) $^ -o $@  $(LDLIBS)
endif


#安装程序和库
install:$(TARGET)
ifdef LIBTYPE
	$(call Install,$(TARGET),$(OUT)/lib)
	$(call Install,*.h,$(OUT)/include)
else
	$(call Install,$(TARGET),$(OUT)/bin)
	$(call InstallSH,$(TARGET),$(OUT)/bin)
endif

#卸载程序和库
uninstall:clean
ifndef LIBTYPE
	-$(STOPSH)
	$(RM) $(OUT)/bin/$(TARGET)
	$(RM) $(OUT)/bin/$(STARTSH)
	$(RM) $(OUT)/bin/$(STOPSH)
else
	$(RM) $(OUT)/lib/$(TARGET)
endif
	

#rm -r test.o test
#目标清理
clean:
	$(RM) $(OBJS)  $(TARGET) .depend

.PHONY: clean  uninstall install  all depend #伪目标 没有对应的文件

