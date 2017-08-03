#!/bin/sh

qemu-system-i386 -cdrom build/tupai.iso -d guest_errors -d cpu_reset --no-reboot --no-shutdown -m 256M #-s -S
