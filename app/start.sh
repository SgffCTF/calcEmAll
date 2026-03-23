#!/bin/sh


qemu-system-x86_64 \
    -s \
    -smp 1 \
    -m 128M \
    -cpu qemu64,+smep,+smap \
    -kernel linux/bzImage \
    -initrd initramfs.cpio.gz \
    -nographic \
    -no-reboot \
    -serial stdio \
    -monitor /dev/null \
    -netdev user,id=net0 \
    -device e1000,netdev=net0 \
    -append "console=ttyS0 kpti=1 quiet panic=0 init=/init"
