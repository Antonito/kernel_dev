## My attempt to write an x86 unix-like kernel

This project has an educational-only purpose.

### Current State
- [x] Repo Architecture
- [x] Kernel start
  - [x] Multiboot
  - [x] x86 ASM to C
- [x] CPU Initialization
  - [x] GDT
  - [x] IRQ
  - [x] ISR
- [x] Logger
  - [x] Date / Time detection
  - [x] Serial
  - [x] VGA
- [ ] Memory Management Unit
  - [ ] PMM
  - [ ] VMM
  - [ ] Heap
  - [ ] Memory Map
- [ ] User-space code execution
  - [ ] Multithread
  - [ ] Ring3
  - [ ] Syscalls
- [ ] Filesystem
  - [ ] ATA / IDE driver
  - [ ] VFS
  - [ ] FAT16
  - [ ] EXT2
- [ ] PCI Device detection
- [ ] Network Stack
- [ ] Debug utilities
  - [x] UBSan
  - [ ] ASan

I won't accept any pull request for now, as I want this to be a personal work, but feel free to open issues !
