TARGET=libxcom
OBJS=xcom.o
ifeq  ($(STATIC),1)
#静态库
TARGET:=$(TARGET).a
$(TARGET):$(OBJS)
	$(AR) -cvr $@ $^

else
#动态库
TARGET:=$(TARGET).so
LDFLAGS=-shared
CXXFLAGS=-fPIC 
$(TARGET):$(OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@
endif

clean:
	$(RM) $(TARGET) $(OBJS)

.PHONY: clean
