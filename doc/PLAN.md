# Plan Of Action

What needs to be done, in order of priority:

### Memory Management

- Page frame allocator
- Virtual memory manager
- Virtual memory context switcher

### The Road To Userland

- Improve syscall API
- Create an elf loader for userland programs & modules
- Create a TSS and handle use-space code

### Drivers And Devices

- Fix x86 keyboard driver
- Fix x86 serial driver
- Fix TTY handling things

### Codebase

- Switch to Make as the primary build system
- Clean up code, use result<> and error<> more for error checking
- Add vector_t<>-based exceptions to kernel
