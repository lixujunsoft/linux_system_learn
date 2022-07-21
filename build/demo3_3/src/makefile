# src/makefile
TARGET=xcom xthread xserver #注意，有次序
LDIR=$(shell pwd)
CTARGET=$(foreach n,$(TARGET),$(LDIR)/$n) #/make/src/xcom /make/src/xthread 
all:$(TARGET)
$(TARGET): 
	$(MAKE) -C $(LDIR)/$@/ -f $(LDIR)/$@/makefile
clean:$(CTARGET)
$(CTARGET):
	$(MAKE) -C $@ -f $@/makefile clean
.PHONY: $(TARGET) all clean $(CTARGET)