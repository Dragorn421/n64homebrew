ROMNAME := brew3d

$(info Using N64_INST = $(N64_INST))

all: $(ROMNAME).z64
.PHONY: all

BUILD_DIR = build
include $(N64_INST)/include/n64.mk

C_FILES = $(shell find . -name '*.c')
OBJS = $(addprefix $(BUILD_DIR)/,$(C_FILES:.c=.o))

# assets
FILESYSTEM_DIR = $(BUILD_DIR)/filesystem
ASSETS_MODEL64 = brewmodel.model64
FILESYSTEM_FILES = $(addprefix $(FILESYSTEM_DIR)/,$(ASSETS_MODEL64))

$(FILESYSTEM_DIR)/%.model64: %.model64
	mkdir -p $(dir $@)
	cp "$<" "$@"

$(BUILD_DIR)/file_system.dfs: $(FILESYSTEM_FILES)
$(ROMNAME).z64: $(BUILD_DIR)/file_system.dfs

$(ROMNAME).z64: N64_ROM_TITLE = "brew3d"

$(BUILD_DIR)/$(ROMNAME).elf: $(OBJS)

clean:
	rm -rf $(BUILD_DIR) *.z64
.PHONY: clean

-include $(wildcard $(BUILD_DIR)/*.d)
