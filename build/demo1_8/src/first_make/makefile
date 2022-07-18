# first_make 
# $^ 依赖 不重复
# $@ 目标
# @ 不显示命令执行 -失败不停止
TARGET=first_make
LIBS=-lpthread
OBJS=first_make.o xdata.o
CXXFLAGS=-I../test_gcc

$(TARGET):$(OBJS)
	@#-@rm test
	@echo "begin build $(TARGET)"
	$(CXX) $^ -o $@ $(LIBS)
	@echo "$(TARGET) build success!"

clean:
	$(RM) $(OBJS) $(TARGET)
.PHONY: clean *clean 

	
