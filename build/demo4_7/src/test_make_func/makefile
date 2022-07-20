TARGET=test_make_func
SRC:=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRC))
$(TARGET):$(OBJS)
	$(CXX) $^ -o $@
#all:
#	echo $(SRC)

