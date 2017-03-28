#!/bin/sh

qemu-system-x86_64 -cdrom build-default/tupai.iso -d guest_errors -m 256Mb
