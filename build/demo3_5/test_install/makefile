#test_install/makefile
TARGET=test_install
OBJS=test_install.o
CC:=g++
OUT=/usr
#make install OUT=../out

define Install # $(1) TARGET $(2) $(OUT)
	@echo "begin install "$(1)
	-mkdir -p $(2)
	cp $(1) $(2)
	@echo $(1) " install success!"
endef

$(TARGET):$(OBJS)
install:$(TARGET)
	$(call Install,$(TARGET),$(OUT)/bin/)
#	@echo "begin install "$(TARGET)
#	mkdir -p $(OUT)/bin/
#	cp $(TARGET) $(OUT)/bin/
#	@echo $(TARGET) " install success!"

.PHONY:install
