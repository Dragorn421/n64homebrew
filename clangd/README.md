# N64 homebrew with libdragon and clangd

This folder demonstrates how to configure and use
[clangd](https://clangd.llvm.org) for use with libdragon N64 homebrew.

See `.clangd` for the relevant configuration bit. It mainly deals with tweaking
the flags passed to gcc so that clangd can process the compile commands.

If you have the toolchain installed to a different location from
`/opt/n64/libdragon`, you will need to change the
`-I/opt/n64/libdragon/mips64-elf/include` argument accordingly.

## `compile_commands.json`

clangd requires a `compile_commands.json` file.

On Linux you can generate it easily using bear (Ubuntu: `apt install bear`):

```sh
bear -- make --always-make -j$(nproc)
```

Windows instructions (native, MSYS2, ...): contributions welcome!
