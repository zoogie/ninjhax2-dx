ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

ifeq ($(filter $(DEVKITARM)/bin,$(PATH)),)
export PATH:=$(DEVKITARM)/bin:$(PATH)
endif

ifeq ($(REGION), J)
CNVERSION = JPN
else
CNVERSION = WEST
endif

export FIRMVERSION
export CNVERSION
export REGION
export ROVERSION
export MSETVERSION
export MENUVERSION
export LOADROPBIN
export OTHERAPP
export UDSPLOIT
export QRINSTALLER

ROPDB_VERSIONS = 11272 12288 13330 14336 15360 16404 17415 19456 20480 21504 22528 23554 24576 25600 26624 20480_usa 21504_usa 22528_usa 23552_usa 24578_usa 25600_usa 26624_usa 27648_usa 6166_kor 7175_kor 8192_kor 9216_kor 10240_kor 11266_kor 12288_kor 13312_kor 14336_kor 15361_kor
ROPDB_TARGETS = $(addsuffix _ropdb.txt, $(addprefix menu_ropdb/, $(ROPDB_VERSIONS)))

OUTNAME = $(FIRMVERSION)_$(REGION)_$(MENUVERSION)_$(MSETVERSION)

QRCODE_TARGET0	:=	q/$(OUTNAME).png

SCRIPTS = "scripts"

.PHONY: directories all menu_ropdb build/constants firm_constants/constants.txt cn_constants/constants.txt region_constants/constants.txt menu_ropdb/ropdb.txt menu_payload/menu_payload_loadropbin.bin cn_save_initial_loader/cn_save_initial_loader.bin cn_qr_installer/cn_qr_installer.bin.png

all: directories build/constants build/cn_save_initial_loader.bin $(QRCODE_TARGET0)
directories:
	@mkdir -p build
	@mkdir -p q

menu_ropdb: $(ROPDB_TARGETS)

menu_ropdb/%_ropdb.txt: menu_ropdb/17415_ropdb_proto.txt
	@echo building ropDB for menu version $*...
	@python scripts/portRopDb.py menu_17415_code.bin menu_$*_code.bin 0x00100000 menu_ropdb/17415_ropdb_proto.txt menu_ropdb/$*_ropdb.txt

firm_constants/constants.txt:
	@cd firm_constants && make
cn_constants/constants.txt:
	@cd cn_constants && make
region_constants/constants.txt:
	@cd region_constants && make
menu_ropdb/ropdb.txt: menu_ropdb/$(MENUVERSION)_ropdb.txt
	@cd menu_ropdb && make
	
build/constants: firm_constants/constants.txt cn_constants/constants.txt region_constants/constants.txt menu_ropdb/ropdb.txt
	@python $(SCRIPTS)/makeHeaders.py $(FIRMVERSION) $(CNVERSION) $(MSETVERSION) $(ROVERSION) $(MENUVERSION) $(REGION) $(OUTNAME) build/constants $^
	@cd menu_payload && make
	@cp menu_payload/menu_payload_loadropbin.bin cn_save_initial_loader/cn_initial/data/

q/$(OUTNAME).png: build/cn_qr_installer.bin.png
	@cp build/cn_qr_installer.bin.png q/$(OUTNAME).png

build/cn_save_initial_loader.bin: cn_save_initial_loader/cn_save_initial_loader.bin
	@cp cn_save_initial_loader/cn_save_initial_loader.bin build
	@cp cn_save_initial_loader/cn_save_initial_loader.bin cn_qr_installer/
cn_save_initial_loader/cn_save_initial_loader.bin:
	@cd cn_save_initial_loader && make
	@cp cn_save_initial_loader/cn_save_initial_loader.bin cn_qr_installer/data.bin
	
build/cn_qr_installer.bin.png: cn_qr_installer/cn_qr_installer.bin.png
	@cd cn_qr_installer && make
	@cp cn_qr_installer/cn_qr_installer.bin.png build

clean:
	@rm -rf build/*
	@cd firm_constants && make clean
	@cd cn_constants && make clean
	@cd region_constants && make clean
	@cd menu_ropdb && make clean
	@cd cn_qr_installer && make clean
	@cd cn_save_initial_loader && make clean
	@cd menu_payload && make clean
	@echo "all cleaned up !"
