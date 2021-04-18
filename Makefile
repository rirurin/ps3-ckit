include userconfig.mk

# dirs
BASE_DIR = $(CURDIR)
LOADER_DIR = $(BASE_DIR)\loader
TOOLS_DIR = $(BASE_DIR)\tools
PRX_DIR = $(BASE_DIR)\prx
OUT_DIR = $(BASE_DIR)\prx\build\tmp
BUILD_IN_DIR = $(BASE_DIR)\prx\build\tmp
BUILD_TMP_DIR = $(BASE_DIR)\prx\build\tmp
BUILD_OUT_DIR = $(BASE_DIR)\prx\build\bin
BIN2RPCS3PATCH = $(TOOLS_DIR)\bin2rpcs3patch.py

# addresses
#mod name			first - last instruction addr				
#mod cpk:			B44A9C-B44AB8
#file access log:	B44ABC-B44B4C
#bgmorder:			B44B50-B44B94
#bgmrandom:			B44B50-B44B94
#fixscriptprint:	B44B98-B44BF4
#60 fps:			b44e00-b44e50

# NOTE: might be different between versions...?
TOC 						= 0xD01288
# NOTE: if you change these, also change them in the loader linker script
INJECT_ADDR 				= 0x10250
LOADER_START_ADDR 			= 0xA3BE70 # .sub_A3BAD0 + 4
LOADER_END_ADDR 			= 0xA3BEF0 # + 0x80

# 745 instructions, around ~200 hooks
HOOK_SHARED_TEXT_BEGIN_ADDR = 0xA3BEF0 # LOADER_START_ADDR + 0x80
HOOK_SHARED_TEXT_END_ADDR 	= 0xA3CA94 

# 0x1FC / (3 * 4) = 42
# TODO use this
#HOOK_SHARED_TEXT_2_BEGIN_ADDR = 0xB44E60
#HOOK_SHARED_TEXT_2_END_ADDR = 0xB4505C

# 0xA8 / 4 = 42
HOOK_SHARED_DATA_BEGIN_ADDR = 0xCE2CB8
HOOK_SHARED_DATA_END_ADDR 	= 0xCE2D60

# 0xA8 / 4 = 42
# TODO use this
#HOOK_SHARED_DATA_2_BEGIN_ADDR = 0xB46434
#HOOK_SHARED_DATA_2_END_ADDR = 0xB464DC

BIN2RPCS3PATCHARGS = \
	--input $(LOADER_DIR)\build\loader.text.inject.bin $(LOADER_DIR)\build\loader.text.bin --address $(INJECT_ADDR) $(LOADER_START_ADDR) \
	--output "$(PATCH_FILE)" --indent 3 --replace_patch shk_elf_loader

SHKGENARGS = \
	--tools_dir "$(TOOLS_DIR)" --out_dir "$(OUT_DIR)" --build_in_dir "$(BUILD_IN_DIR)" --build_tmp_dir "$(BUILD_TMP_DIR)" \
	--build_out_dir "$(BUILD_OUT_DIR)" --toc $(TOC) --hook_shared_text_range $(HOOK_SHARED_TEXT_BEGIN_ADDR) $(HOOK_SHARED_TEXT_END_ADDR) \
	--hook_shared_data_range $(HOOK_SHARED_DATA_BEGIN_ADDR) $(HOOK_SHARED_DATA_END_ADDR) --patch_file "$(PATCH_FILE)" --hooks $(HOOKS)

all:
# build loader
	cd $(LOADER_DIR) && $(MAKE) binary INJECT_ADDR=$(INJECT_ADDR) LOADER_START_ADDR=$(LOADER_START_ADDR)
	$(PYTHON) "$(BIN2RPCS3PATCH)" $(BIN2RPCS3PATCHARGS)

# generate shk files
	cd tools && $(PYTHON) shkgen.py $(SHKGENARGS)

# build injection patch
	cd $(BUILD_TMP_DIR) && $(MAKE) -f shk_elf.gen.mk patch

# build sprx
	cd $(PRX_DIR) && $(MAKE) sprx
	copy "$(BUILD_OUT_DIR)\mod.sprx" "$(GAME_DIR)" /Y

clean:
	cd $(LOADER_DIR) && $(MAKE) clean
	cd $(PRX_DIR) && $(MAKE) clean