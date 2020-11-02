# MadOS | CassandraOS | CassiopeiaOS | devOS

Name undecided. Let me know what you think.

An operating system I am building mostly to get experience for low level development and to try something new other then web development. If I don't continue active development I atleast want to port Doom or similar game. 

### Contributions
All contributions are welcomed so long as they come with a detailed descriptions of what they do as I am still learning lots. Any pull request I do not accept I will provide a description why.

### Current Goals:
  - [ ] Working Filesystem
  - [ ] Shell
  - [ ] Processes
  - [ ] Scheduling
  - [x] C Standard Library(basic progress so far)
  - [ ] Play Original Doom for the first time

### Compiling Dependencies:
I sourced everything from `Homebrew` and edited the formulas that did not have the initial desired configurations

nasm:
```sh
% brew install nasm
```
binutils:
```sh
% export EDITOR=nano
% brew edit x86_64-elf-binutils
# Add "--with-sysroot" option in config section
% brew install --build-from-source x86_64-elf-binutils
```

gcc:
```sh
% brew install x86_64-elf-gcc
```

  
### Supported Architectures:
x86_64 only for now. Maybe more when I learn a thing or two.

