#!/bin/sh

DATE=`date --date="now" "+%Y-%m-%d"`

grub-mkrescue -o tupai.iso grub/isodir
#cp tupai.iso tupai-${DATE}.iso
