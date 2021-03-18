# devOS
---

devOS is a hobby operating system I am building on my free time. The operating system is unix based and will support x86_64 platforms with both UEFI and Legacy BIOS boot methods. 

### Goals
devOS will in the future support a full featured libc and userspace. Applications I plan to write will include many small games but also software to pull radio signals and construct images from weather satellites. A TCP/IP stack is also planned. 

### Dependencies

All easily sourced from homebrew

- x86_64-elf GCC
- x86_64-elf Binutils
- nasm
- qemu

### Compiling
_Only tested on macOS x86_64 systems so far._

macOS
- Navigate to the root of the workspace/individual projects and run `./scripts/build.sh`. Run './scripts/run.sh' to build and run the project in qemu. Configure as needed in './scripts/config.sh'.