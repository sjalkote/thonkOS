# thonkOS
im bored so lets learn comp arch and make an OS

### Notes

The `Makefile` contains the bulk of the logic for automated builds and
runs. The scripts within it can be used to compile the kernel and create
a bootable ISO.

> [!IMPORTANT]  
> On macOS the toolchain is missing the ELF binaries for Limine,
> so we need to use a GCC cross-compiler (`brew install x86_64-elf-gcc`)
> and then specify the prefix for make commands: `make TOOLCHAIN_PREFIX=x86_64-elf-`
