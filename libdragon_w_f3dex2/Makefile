ROMNAME := libdragon_w_f3dex2
ROM_TITLE := "$(ROMNAME)"

$(info Using N64_INST = $(N64_INST))

all: $(ROMNAME).z64
.PHONY: all

BUILD_DIR = build
include $(N64_INST)/include/n64.mk

C_FILES = $(shell find . -name '*.c')
S_FILES = $(shell find . -name '*.S')
OBJS = $(addprefix $(BUILD_DIR)/,$(C_FILES:.c=.o) $(S_FILES:.S=.o))

$(ROMNAME).z64: N64_ROM_TITLE = $(ROM_TITLE)

$(BUILD_DIR)/$(ROMNAME).elf: $(OBJS)

clean:
	rm -rf $(BUILD_DIR) *.z64
.PHONY: clean

-include $(wildcard $(BUILD_DIR)/*.d)
