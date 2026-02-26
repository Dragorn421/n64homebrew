ROMNAME := gl_mkmat_test
ROM_TITLE := "$(ROMNAME)"

BLENDER := /home/dragorn421/blender_collection/blender-4.2.11-linux-x64/blender

$(info Using N64_INST = $(N64_INST))

all: $(ROMNAME).z64
.PHONY: all

BUILD_DIR := build
include $(N64_INST)/include/n64.mk

C_FILES := $(shell find . -name '*.c')
OBJS := $(addprefix $(BUILD_DIR)/,$(C_FILES:.c=.o))

FILESYSTEM_DIR := $(BUILD_DIR)/filesystem
ASSETS_FILES := materials.mdb
FILESYSTEM_FILES := $(addprefix $(FILESYSTEM_DIR)/,$(ASSETS_FILES))

.SECONDARY: $(BUILD_DIR)/assets/materials.jmat

$(BUILD_DIR)/assets/materials.jmat: materials.blend
	@mkdir -p $(dir $@)
	@echo "    [JMAT] $@"
	$(BLENDER) --background $< --python matex.py -- Material $@

$(BUILD_DIR)/assets/materials.mdb: $(BUILD_DIR)/assets/materials.jmat
	@mkdir -p $(dir $@)
	@echo "    [MDB] $@"
	$(N64_BINDIR)/mkmaterial --output $(dir $@) $<

$(FILESYSTEM_DIR)/%.mdb: $(BUILD_DIR)/assets/%.mdb
	@mkdir -p $(dir $@)
	cp $< $@

$(BUILD_DIR)/file_system.dfs: $(FILESYSTEM_FILES)
$(BUILD_DIR)/file_system.dfs: N64_MKDFS_ROOT := $(FILESYSTEM_DIR)
$(ROMNAME).z64: $(BUILD_DIR)/file_system.dfs

$(ROMNAME).z64: N64_ROM_TITLE = $(ROM_TITLE)

$(BUILD_DIR)/$(ROMNAME).elf: $(OBJS)

clean:
	rm -rf $(BUILD_DIR) *.z64
.PHONY: clean

-include $(wildcard $(BUILD_DIR)/*.d)
