# MadOS | CassandraOS | CassiopeiaOS | devOS

Name undecided. Let me know what you think.

An operating system I am building mostly to get experience for low level development and to try something new other then web development. If I don't continue active development I atleast want to port Doom or a similar game. 

### Contributions
All contributions are welcomed so long as they come with a detailed descriptions of what they do as I am still learning lots.

### Current Goals:
  - [ ] Working Filesystem
  - [ ] Shell
  - [ ] Processes
  - [ ] Scheduling
  - [ ] Userspace
  - [x] C Standard Library(basic progress so far)
  - [ ] Play Original Doom for the first time

### Dependencies:
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

### Compiling
```sh
% ./scripts/compile.sh
```

### Running in Qemu
```sh
% ./scripts/run.sh
```

`Note 1:` If the scripts refuse to run because of permissions try using `chmod 777 <script_name>.sh`

`Note 2:` If you have different names for your cross compilers or other utilities check out `./scripts/config.sh` and change some of the exported variables to match what is on your system.

### Write to USB drive
The following command is an example for writing the resulting `.img` file to a flash drive. It would be likely that your device is not named `disk2` or be located in the same location depending on the host system you are using.
```sh
sudo dd if=MadOS.img of=/dev/disk2
```


  
### Supported Architectures:
x86_64 only for now. Maybe more when I learn a thing or two.

