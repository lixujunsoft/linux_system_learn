# wildcard 展开为已经存在的、使用空格分开的、匹配此模式的所有文件列表
#          SRC=$(wildcard *.cpp *.cc)
# patsubst 
# 格式：$(patsubst<pattern>,<replacement>,<text>)
# 名称：模式字符串替换函数---patsubst
# TMP=$(patsubst %.cpp,%.o,$(SRC))  将字符串SRC中所有的.cpp替换成.o，并输出成另外一个字符串
TARGET=test_make_func
SRC:=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRC))
$(TARGET):$(OBJS)
	$(CXX) $^ -o $@
#all:
#	@echo $(SRC)

clean:
	rm $(TARGET) $(OBJS)
.PHONY: clean *clean
