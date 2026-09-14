ROMNAME := test_rendering
ROM_TITLE := "$(ROMNAME)"
LIBDRAGON_PREVIEW := 2

$(info Using N64_INST = $(N64_INST))

all: $(ROMNAME).z64
.PHONY: all

BUILD_DIR = build
include $(N64_INST)/include/n64.mk

C_FILES := $(shell find src -name '*.c')
OBJS := $(addprefix $(BUILD_DIR)/,$(C_FILES:.c=.o))

PNGS := $(shell find assets -name '*.png')
SPRITES := $(PNGS:%.png=$(BUILD_DIR)/dfs/%.sprite)

$(BUILD_DIR)/dfs/assets/%.sprite: assets/%.png
	@mkdir -p $(@D)
	@echo "    [SPRITE] $@"
	$(N64_MKSPRITE) -o $(@D) $<

$(ROMNAME).z64: N64_ROM_TITLE = $(ROM_TITLE)

$(BUILD_DIR)/$(ROMNAME).elf: $(OBJS)

$(BUILD_DIR)/dfs.dfs: $(SPRITES)
$(BUILD_DIR)/dfs.dfs: N64_MKDFS_ROOT := $(BUILD_DIR)/dfs
$(ROMNAME).z64: $(BUILD_DIR)/dfs.dfs

clean:
	rm -rf $(BUILD_DIR) *.z64
.PHONY: clean

-include $(wildcard $(BUILD_DIR)/*.d)
