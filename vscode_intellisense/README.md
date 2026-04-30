# N64 homebrew with libdragon and VSCode Intellisense

This folder demonstrates how to configure VSCode Intellisense for use with
libdragon N64 homebrew.

See `.vscode/c_cpp_properties.json` for the relevant bit.

You will need to change the `browse.path` path to point to your local libdragon
clone, if you want Intellisense to analyze it. It is useful because it provides
documentation that lies in the .c source files, otherwise you only get
documentation from .h header files.

If you have the toolchain installed to `$N64_GCCPREFIX` instead of `$N64_INST`,
you also need to change the `compilerPath` path accordingly.
