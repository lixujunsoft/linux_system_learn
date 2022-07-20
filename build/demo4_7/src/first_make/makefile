# first_make 
# $^ 依赖 不重复
# $@ 目标
# @ 不显示命令执行 -失败不停止
TARGET=first_make
LIBS=-lpthread
OBJS=first_make.o xdata.o
CXXFLAGS=-I../test_gcc
SRCS=first_make.cpp xdata.cpp
all:depend $(TARGET)
depend:
	@$(CXX) $(CXXFLAGS) -MM $(SRCS) >.depend
-include .depend

$(TARGET):$(OBJS)
	@#-@rm test
	@echo "begin build $(TARGET)"
	$(CXX) $^ -o $@ $(LIBS)
	@echo "$(TARGET) build success!"
#first_make.o: first_make.cpp xdata.h ../test_gcc/test.h
#xdata.o: xdata.cpp xdata.h
clean:
	$(RM) $(OBJS) $(TARGET)
.PHONY: clean *clean  depend all

	
