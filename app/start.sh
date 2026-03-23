#!/bin/sh


qemu-system-x86_64 \
    -s \
    -smp 1 \
    -m 128M \
    -cpu host,+smep,+smap \
    -enable-kvm \
    -kernel linux/bzImage \
    -initrd initramfs.cpio.gz \
    -nographic \
    -no-reboot \
    -monitor /dev/null \
    -append "console=ttyS0 kpti=1 quiet panic=0 init=/init"
