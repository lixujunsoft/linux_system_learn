# src/makefile 所有项目统一编译
TARGET=xcom xthread xserver
LDIR=$(shell pwd)
CTARGET=$(foreach n,$(TARGET),$(LDIR)/$n) #/make/src/xcom /make/src/xthread 
ITAG=_install
UTAG=_uninstall
ITARGET=$(foreach n,$(TARGET),$(LDIR)/$n$(ITAG)) # /make/src/xcom_install xthead_install
UTARGET=$(foreach n,$(TARGET),$(LDIR)/$n$(UTAG)) # /make/src/xcom_uninstall 
all:$(TARGET)

#多目标编译
$(TARGET): 
	$(MAKE) -C $(LDIR)/$@/ -f $(LDIR)/$@/makefile depend
	$(MAKE) -C $(LDIR)/$@/ -f $(LDIR)/$@/makefile

#多目标清理
clean:$(CTARGET)
$(CTARGET):
	$(MAKE) -C $@ -f $@/makefile clean

#多目标install
install:$(ITARGET) # /make/src/xcom_install
	@echo $(ITARGET)
#$(subst $(ITAG),,$@) 替换 _install 为空
$(ITARGET):$(TARGET)# /make/src/xcom_install
	@echo $@
	$(MAKE) -C $(subst $(ITAG),,$@) -f $(subst $(ITAG),,$@)/makefile install

#多目标uninstall
uninstall:$(UTARGET)
$(UTARGET): #/make/src/xcom_uninstall  /make/src/xthread_uninstall
	$(MAKE) -C $(subst $(UTAG),,$@) -f $(subst $(UTAG),,$@)/makefile uninstall

.PHONY: $(TARGET) all clean $(CTARGET) $(ITARGET)

