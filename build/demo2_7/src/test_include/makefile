#test_include.cpp testcpp.cc testc.c
# clean
TARGET:=test_include
include ../makefile.mk
CXXFLAGS:=$(CXXFLAGS) -I../test_gcc -I../xcom -I../xthread
LDLIBS:=-lpthread -lxcom -lxthread