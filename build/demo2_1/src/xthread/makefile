# -fPIC 产生位置无关代码(PIC),一般创建共享库时用到

TARGET=libxthread.so
OBJS=xthread.o
LDFLAGS=-shared
CXXFLAGS=-fPIC 
$(TARGET):$(OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@
clean:
	$(RM) $(TARGET) $(OBJS)
	
.PHONY: clean