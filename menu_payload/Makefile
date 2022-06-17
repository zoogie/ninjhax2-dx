ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif
export CTRULIB=$(shell pwd)/libctru
ifeq ($(strip $(CTRULIB)),)
$(error "Please set CTRULIB in your environment. export DEVKITARM=<path to>ctrulib/libctru")
endif

ifeq ($(filter $(DEVKITARM)/bin,$(PATH)),)
export PATH:=$(DEVKITARM)/bin:$(PATH)
endif

ROPBIN_FILENAME_SUFFIX	:=	

ifeq ($(FIRMVERSION),N3DS)
	ROPBIN_FILENAME_SUFFIX	:=	_new3ds.bin
else
	ROPBIN_FILENAME_SUFFIX	:=	_old3ds.bin
endif

ifneq ($(strip $(LOADROPBIN)),)
	ROPBIN_CMD	:=	
endif

SCRIPTS = "../scripts"

all:  menu_payload_loadropbin.bin $(ROPBIN_CMD)

clean:
	@rm -f menu_payload_loadropbin.bin
	@echo "all cleaned up !"

%.bin: %.s
	@armips $<
