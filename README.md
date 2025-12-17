# thonkOS
im bored so lets learn comp arch and make an OS

### Notes

The `Makefile` contains the bulk of the logic for automated builds and
runs. The scripts within it can be used to compile the kernel and create
a bootable ISO.

> [!IMPORTANT]  
> I'm on macOS where the toolchain is missing the ELF binaries for Limine, so I set the Makefile
> to use the `TOOLCHAIN_PREFIX := x86_64-elf-` by default (you can install the GCC cross-compiler 
> with `brew install x86_64-elf-gcc`).
> 
> If you are not using macOS you can change this variable or specify the empty prefix in your make commands
> (e.g. `make TOOLCHAIN_PREFIX="" iso`).

To create the bootable ISO image:
1. Run `make -C limine` if you haven't before to build the limine utility.
2. Run `make iso` which will handle the rest and generate the file in `out/thonkOS.iso`.

To **run** the OS using QEMU you can use the `make run` script (automatically rebuilds ISO when necessary).