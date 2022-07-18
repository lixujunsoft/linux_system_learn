# first_make 
# $^ 依赖 不重复
# $@ 目标
# @ 不显示命令执行 -失败不停止
# -@rm test 失败不停止
# #-@rm test  注释掉-@rm test
# @#-@rm test 不显示注释掉的语句，外部的注释不用加@，代码段中的注释要加@

#TARGET=first_make
#LIBS=-lpthread
#$(TARGET):first_make.cpp xdata.cpp
#	@#-@rm test
#	@echo "begin build $(TARGET)"
#	@$(CXX) $^ -o $@ $(LIBS)
#	@echo "$(TARGET) build success!"

#自动推导
TARGET=first_make
LIBS=-lpthread
$(TARGET):first_make.o xdata.o
	@#-@rm test
	@echo "begin build $(TARGET)"
	$(CXX) $^ -o $@ $(LIBS)
	@echo "$(TARGET) build success!"