TARGET=xserver
OBJS=xserver.o
CXXFLAGS=-I../xthread
LDFLAGS=-L../xthread
LIBS=-lxthread -lpthread
$(TARGET):$(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS) $(LIBS)
clean:
	$(RM) $(TARGET) $(OBJS)