#test_install/makefile
TARGET=test_install
OBJS=test_install.o
CC:=g++
OUT=/usr
#make install OUT=../out
STARTSH=start_$(TARGET)
STOPSH=stop_$(TARGET)
define Install # $(1) TARGET $(2) $(OUT) $(3) para1 $(4) para2 ...
	@echo "begin install "$(1)
	-mkdir -p $(2)
	cp $(1) $(2)

	@echo "begin make start shell"
	echo "nohup $(1) $(3) $(4) $(5) $(6) &" > $(STARTSH)
	chmod +x $(STARTSH)
	cp $(STARTSH) $(2)
	@echo "end make start shell"

	@echo "begin make stop shell"
	echo killall $(1) >$(STOPSH)
	cp $(STOPSH) $(2)
	@echo "end make stop shell"
	
	@echo $(1) " install success!"
endef
# start_test_install
#  nohup test_install 3 &

#stop_test_install
# killall test_install


$(TARGET):$(OBJS)
install:$(TARGET)
	$(call Install,$(TARGET),$(OUT)/bin/,5)
uninstall:
	@echo "begin uninstall "$(TARGET)
	-$(STOPSH)
	$(RM) $(OUT)/bin/$(TARGET) $(OUT)/bin/$(STOPSH) $(OUT)/bin/$(STARTSH)
	@echo " uninstall "$(TARGET) " success!"
.PHONY:install uninstall
