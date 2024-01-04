ROMNAME := test_gdb

$(info Using N64_INST = $(N64_INST))

all: $(ROMNAME).z64
.PHONY: all

BUILD_DIR = build
include $(N64_INST)/include/n64.mk

C_FILES = $(shell find . -name '*.c')
OBJS = $(addprefix $(BUILD_DIR)/,$(C_FILES:.c=.o))

$(ROMNAME).z64: N64_ROM_TITLE = "$(ROMNAME)"

$(BUILD_DIR)/$(ROMNAME).elf: $(OBJS)

$(OBJS): CFLAGS += -ggdb3

clean:
	rm -rf $(BUILD_DIR) *.z64
.PHONY: clean

-include $(wildcard $(BUILD_DIR)/*.d)
