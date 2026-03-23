#!/bin/sh

if [ ! -f "./linux/vmlinux" ]; then
    echo "vmlinux not found, unpack bzImage..."
    vmlinux-to-elf ./linux/bzImage ./linux/vmlinux
fi
gdb -x script.gdb ./linux/vmlinux

