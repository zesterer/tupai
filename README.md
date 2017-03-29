# Tupai

Copyright 2016-2017, Joshua Barretto

---

A simple monolithic operating system created for educational purposes.


![Tupai 0.6.0](doc/images/tupai-0-6-0-dev.png)


## What is Tupai?
---

Tupai is a modular operating system kernel. It is designed, implemented and
maintained by myself, Joshua Barretto. Tupai does not aim for compliance with
any particular standard, although borrows many ideas and paradigms from Linux,
Minix and POSIX. Tupai is a personal venture into operating system development
and is intended as a project to teach myself more about the field. That said, I
tend to adhere to a rather strict coding convention and style, so the code may
be beneficial to others wishing to learn from it.

## Projects Goals
---

* Modular multi-tasking kernel
* Ports for amd64, i386 and ARM (Raspberry Pi 2) architectures
* Generic, cross-platform design that makes future porting relatively trivial
* Correct use of memory-protection systems such as paging / MMUs
* Interrupt-driven architecture
* Preemptive, multi-process POSIX-like userland
* Simple filesystem
* File descriptor / streaming IPC and I/O

## Project Status
---

_See `docs/STATUS.md`_

## Pre-built ISOs
---

I regularly release pre-built ISO images of Tupai on every feature change
release. You can find them here:
[Zesterer's Projects](http://zesterer.homenet.org/projects.shtml)

## Building
---

It is recommended that Tupai is built on a UNIX-like system. All other operating
systems are untested. The build system is dependant on basic UNIX utilities like
'cp', 'mkdir' and 'tar' and also a cross-compiling version of the GNU Compiler
Collection, so this may somewhat limit the range of operating system hosts
capable of building Tupai. My own build system is an x86_64 Arch Linux machine.

_See `docs/BUILDING.md`_

## Licensing
---

Tupai is currently licensed under the GNU project's General Public License
version 3 and is copyrighted 2016-2017.

_See `docs/LICENSE` or [GNU General Public License version 3](https://www.gnu.org/licenses/gpl-3.0.html)_

![GNU GPLv3](https://www.gnu.org/graphics/gplv3-127x51.png)

## Contributors
---

* Programming
	* Joshua Barretto (joshua.s.barretto@gmail.com)

* Guidance and Advice
	* Numerous members of the #osdev Freenode IRC channel (irc.freenode.net)
	* Numerous members of the OSDev Forums (forum.osdev.org)
		* sortie
		* thePowersGang
		* doug16k

* Resources
	* The OSDev Wiki (wiki.osdev.org)
	* Operating System Concepts, Sixth Edition (Silberschatz, Galvin, Gagne)
