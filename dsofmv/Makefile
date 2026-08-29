ROMNAME := dsofmv
ROM_TITLE := "$(ROMNAME)"

$(info Using N64_INST = $(N64_INST))

all: $(ROMNAME).z64
.PHONY: all

BUILD_DIR = build
include $(N64_INST)/include/n64.mk

C_FILES = main.c things.c
OBJS = $(addprefix $(BUILD_DIR)/,$(C_FILES:.c=.o))

$(ROMNAME).z64: N64_ROM_TITLE = $(ROM_TITLE)

$(BUILD_DIR)/$(ROMNAME).elf: $(OBJS)

MAIN_ELF_EXTERNS := $(BUILD_DIR)/$(ROMNAME).externs
DSOS := $(BUILD_DIR)/filesystem/fmv.dso

$(MAIN_ELF_EXTERNS): $(DSOS)
$(BUILD_DIR)/filesystem/fmv.dso: N64_DSOLDFLAGS += --gc-sections --gc-keep-exported
$(BUILD_DIR)/filesystem/fmv.dso: $(BUILD_DIR)/dso.o \
 $(BUILD_DIR)/libdragon/fmv.o \
 $(BUILD_DIR)/libdragon/h264.o \
 $(BUILD_DIR)/libdragon/yuv.o \
 $(BUILD_DIR)/libdragon/h264_decoder.o \
 $(BUILD_DIR)/libdragon/rsph264_inter.o \
 $(BUILD_DIR)/libdragon/rsph264_intra.o \
 $(BUILD_DIR)/libdragon/subtitles.o \
 $(BUILD_DIR)/libdragon/video.o \
 $(BUILD_DIR)/libdragon/video_sync.o

LIBDRAGON_SRC := /home/dragorn421/Documents/libragon

$(BUILD_DIR)/filesystem/fmv.dso: CFLAGS += -I $(LIBDRAGON_SRC)/src -fvisibility=hidden

# next two recipes copypasted from n64.mk

$(BUILD_DIR)/libdragon/%.o: $(LIBDRAGON_SRC)/src/video/%.c
	@mkdir -p $(dir $@)
	@echo "    [CC] $<"
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD_DIR)/libdragon/%.o: $(LIBDRAGON_SRC)/src/video/%.S
	@mkdir -p $(dir $@)
	set -e; \
	FILENAME="$(notdir $(basename $@))"; \
	if case "$$FILENAME" in "rsp"*) true;; *) false;; esac; then \
		SYMPREFIX="$(subst .,_,$(subst /,_,$(basename $@)))"; \
		TEXTSECTION="$(basename $@).text"; \
		DATASECTION="$(basename $@).data"; \
		METASECTION="$(basename $@).meta"; \
		BINARY="$(basename $@).elf"; \
		echo "    [RSP] $<"; \
		$(N64_CC) $(RSPASFLAGS) -L$(N64_LIBDIR) -nostartfiles -Wl,-Trsp.ld -Wl,--gc-sections  -Wl,-Map=$(BUILD_DIR)/$(notdir $(basename $@)).map,--cref -o $@ $<; \
		mv "$@" $$BINARY; \
		$(N64_OBJCOPY) -O binary -j .text $$BINARY $$TEXTSECTION.bin; \
		$(N64_OBJCOPY) -O binary -j .data $$BINARY $$DATASECTION.bin; \
		$(N64_OBJCOPY) -O binary -j .meta $$BINARY $$METASECTION.bin --set-section-flags .meta=alloc,load; \
		[ -s $$METASECTION.bin ] || printf '\0' > $$METASECTION.bin; \
		$(N64_OBJCOPY) -I binary -O elf32-bigmips -B mips4300 \
				--redefine-sym _binary_$${SYMPREFIX}_text_bin_start=$${FILENAME}_text_start \
				--redefine-sym _binary_$${SYMPREFIX}_text_bin_end=$${FILENAME}_text_end \
				--redefine-sym _binary_$${SYMPREFIX}_text_bin_size=$${FILENAME}_text_size \
				--set-section-alignment .data=16 \
				--rename-section .text=.data $$TEXTSECTION.bin $$TEXTSECTION.o; \
		$(N64_OBJCOPY) -I binary -O elf32-bigmips -B mips4300 \
				--redefine-sym _binary_$${SYMPREFIX}_data_bin_start=$${FILENAME}_data_start \
				--redefine-sym _binary_$${SYMPREFIX}_data_bin_end=$${FILENAME}_data_end \
				--redefine-sym _binary_$${SYMPREFIX}_data_bin_size=$${FILENAME}_data_size \
				--set-section-alignment .data=16 \
				--rename-section .text=.data $$DATASECTION.bin $$DATASECTION.o; \
		$(N64_OBJCOPY) -I binary -O elf32-bigmips -B mips4300 \
				--redefine-sym _binary_$${SYMPREFIX}_meta_bin_start=$${FILENAME}_meta_start \
				--redefine-sym _binary_$${SYMPREFIX}_meta_bin_end=$${FILENAME}_meta_end \
				--redefine-sym _binary_$${SYMPREFIX}_meta_bin_size=$${FILENAME}_meta_size \
				--set-section-alignment .data=16 \
				--rename-section .text=.data $$METASECTION.bin $$METASECTION.o; \
		$(N64_SIZE) -G $$BINARY; \
		$(N64_LD) -relocatable $$TEXTSECTION.o $$DATASECTION.o $$METASECTION.o -o $@; \
		rm $$TEXTSECTION.bin $$DATASECTION.bin $$METASECTION.bin $$TEXTSECTION.o $$DATASECTION.o $$METASECTION.o; \
	else \
		echo "    [AS] $<"; \
		$(CC) -c $(ASFLAGS) -o $@ $<; \
	fi

$(BUILD_DIR)/video.avi:
	@mkdir -p $(@D)
	ffmpeg -i frame.png $@

$(BUILD_DIR)/filesystem/video.h264: $(BUILD_DIR)/video.avi
	@mkdir -p $(@D)
	$(N64_VIDEOCONV) -o $(@D) --codec h264 $<

$(BUILD_DIR)/dfs.dfs: $(BUILD_DIR)/filesystem/video.h264 $(DSOS)
$(BUILD_DIR)/dfs.dfs: N64_MKDFS_ROOT := $(BUILD_DIR)/filesystem
$(ROMNAME).z64: $(BUILD_DIR)/dfs.dfs $(BUILD_DIR)/$(ROMNAME).msym
$(BUILD_DIR)/$(ROMNAME).msym: $(BUILD_DIR)/$(ROMNAME).elf $(MAIN_ELF_EXTERNS)

clean:
	rm -rf $(BUILD_DIR) *.z64
.PHONY: clean

-include $(wildcard $(BUILD_DIR)/*.d)
