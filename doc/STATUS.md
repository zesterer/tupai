# Tupai Status
---

## Implemented

### 0.1.0

	- [x] Simple i686 VGA driver
	- [x] Simple ring-buffer i686 keyboard driver
	- [x] i686 GDT manager
	- [x] i686 IDT manager
	- [x] i686 interrupt driver
	- [x] i686 I/O port functions
	- [x] i686 kernel panic screen
	- [x] Generic TTY
	- [x] Simple kernel prompt
	- [x] Simple dynamic memory manager
	- [x] Simple stack-smashing detection
	- [x] Basic Multiboot support

### 0.2.0

	- [x] i686 PIT driver
	- [x] Improved snake
	- [x] Improved libk implementation
	- [x] Improved kernel prompt
	- [x] Added timer test program
	- [x] Improved system status program
	- [x] Added basic text adventure program

## 0.3.0

	- [x] Partial ANSI terminal compatibility
	- [x] Basic kernel paging support
	- [x] Higher-half kernel
	- [x] Better kernel prompt (backspace)

## 0.4.0

	- [x] VBE graphics driver
	- [x] Backwards-compatible text-mode fallback
	- [x] New virtual tty implementation
	- [x] Better Multiboot support

## 0.5.0

	- [x] Co-operative multi-tasking
	- [x] Graphics blitting
	- [x] Virtual console implementation
	- [x] BMP image loading
	- [x] Independent VGA task

## Tasks
---

	- [-] PAE Paging
	- [-] Stream buffers / queues
	- [ ] RAM filesystem
	- [-] Advanced kernel prompt
	- [ ] Pre-emptive multi-tasking
	- [-] Atomic concurrency objects (mutex, semaphore, etc.) implementation
	- [ ] Properly implement keyboard scancode translation

## Planned
---

	- [ ] x86_64 support
