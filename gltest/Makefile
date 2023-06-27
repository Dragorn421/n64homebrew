ROMNAME := gltest

$(info Using N64_INST = $(N64_INST))

all: $(ROMNAME).z64
.PHONY: all

BUILD_DIR = build
include $(N64_INST)/include/n64.mk

C_FILES = $(shell find . -name '*.c')
OBJS = $(addprefix $(BUILD_DIR)/,$(C_FILES:.c=.o))

# assets
FILESYSTEM_DIR = $(BUILD_DIR)/filesystem
ASSETS_RGBA16_PNG = n64brew_32x64.png
ASSETS_RGBA16_SPRITE = $(addprefix $(FILESYSTEM_DIR)/,$(ASSETS_RGBA16_PNG:.png=.sprite))
ASSETS_SPRITE = $(ASSETS_RGBA16_SPRITE)
FILESYSTEM_FILES = $(ASSETS_SPRITE)

$(FILESYSTEM_DIR)/%.sprite: %.png
	mkdir -p $(dir $@)
	$(N64_MKSPRITE) -f RGBA16 --compress -o "$(dir $@)" "$<"

$(BUILD_DIR)/file_system.dfs: $(FILESYSTEM_FILES)
$(ROMNAME).z64: $(BUILD_DIR)/file_system.dfs
#

$(ROMNAME).z64: N64_ROM_TITLE = "GL Test"

$(BUILD_DIR)/$(ROMNAME).elf: $(OBJS)

clean:
	rm -rf $(BUILD_DIR) *.z64
.PHONY: clean

-include $(wildcard $(BUILD_DIR)/*.d)
