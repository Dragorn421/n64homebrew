# DSO fmv

This demonstrates a pile of hacks to put parts of libdragon (in this case FMV playing code and direct backing libraries) in DSOs.

This is done by recompiling parts of libdragon from source and linking those into the DSO.

`things[0] = 0;`, `things.c` and `missing_symbols.h` are a hack to get ld to link parts of libdragon into the main elf that are needed by the fmv player. these parts could obviously be moved to the DSO too but since they have general use beyond FMVs it seems reasonable to just include them in the main elf.

The DSO is contrarily to typical libdragon DSOs linked with --gc-sections passed to ld, which is needed to garbage collect unused code in (at least) the h264 decoder that would otherwise not link due to missing functions. However --gc-sections without an entrypoint means ld would garbage collect everything, except KEEP() sections as defined in the linker script dso.ld. Only constructors and destructors are marked as such so we add another hack to the list by declaring the function we want to use as a constructor. And we skip the constructor call thanks to a static variable.

## other approaches tried

my first approach was force linking libdragon code into the dso, but 1) n64.mk doesn't allow it easily (`N64_DSOLDFLAGS += -L$(N64_LIBDIR) -ldragon` doesn't do it because -ldragon appearing before the input .o files makes ld think it doesn't need to link anything from there) and 2) even fixing 1 by copying the ld command and moving `-ldragon` to the end pulls in all of the libdragon stuff required, like rspq and rdpq

my second approach tried yanking .o files from the libdragon.a archive and linking those
```mk
$(BUILD_DIR)/filesystem/fmv.dso: $(BUILD_DIR)/dso.o $(BUILD_DIR)/libdragon/fmv.o
$(BUILD_DIR)/libdragon/%.o: $(N64_LIBDIR)/libdragon.a
    @mkdir -p $(@D)
    cd $(@D) && $(N64_AR) x $<
```
but then n64dso fails with
```
Tried to use defined symbol as GP-Relative
Compile with -G 0 or -mno-gpopt to fix
```
which is understandable, libdragon is compiled with small data after all (not `-G 0`)
so this would at least need recompiling libdragon with `-G 0`, no way around that

this led to my third approach presented in this folder.
