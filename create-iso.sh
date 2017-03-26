#!/bin/sh

DATE=`date --date="now" "+%Y-%m-%d"`

cp tupai.bin grub/isodir/boot/tupai.bin
cp grub/grub.cfg grub/isodir/boot/grub/grub.cfg
grub-mkrescue -o tupai.iso grub/isodir
#cp tupai.iso tupai-${DATE}.iso
