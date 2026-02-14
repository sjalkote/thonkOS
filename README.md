# thonkOS
im bored so lets learn comp arch and make an OS

### Notes

The `Makefile` contains the bulk of the logic for automated builds and
runs. The scripts within it can be used to compile the kernel and create
a bootable ISO.

Each GitHub actions workflow run also generates an artifact of the ISO image that can be downloaded.

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

> [!TIP]
> I created a really useful run config for CLion that combines gdb and QEMU's built in gdb server with CLion for native debugging in the IDE.
> The run configuration is located at [`.run/GDB.run.xml`](./.run/GDB.run.xml), so take advantage of the goated CLion debugger UI.

<img width="1392" height="940" alt="firstscreenshot" src="https://github.com/user-attachments/assets/8b854efe-0e4f-4f6b-a5ea-0baca4fef5d4" />


Using `bear` you can generate a `compile_commands.json` file for the `clangd` language server to use.
```bash
make clean; bear -- make
```
