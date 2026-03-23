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

void main() {
  int fd = open("/dev/calculator", O_RDWR);

  struct ioctl_arg *ioctl_input_struct;
  int64_t out;
  ioctl_input_struct->arg1 = 5;
  ioctl_input_struct->arg2 = 56;
  ioctl_input_struct->command = SUM_CMD;
  ioctl_input_struct->out = &out;
  ioctl(fd, ALLOC_CMD, 0);
  ioctl(fd, CALL_CMD, ioctl_input_struct);
  printf("sum result: %d\n", out);

  ioctl_input_struct->command = MUL_CMD;
  ioctl_input_struct->arg2 = 20;
  ioctl(fd, CALL_CMD, ioctl_input_struct);
  printf("mul result: %d\n", out);

  struct ioctl_history_arg *ioctl_history_struct;
  ioctl_history_struct->index = 0;
  ioctl_history_struct->out = &out;
  ioctl(fd, HISTORY_CMD, ioctl_history_struct);
  printf("history 0 index result: %d\n", out);

  ioctl_history_struct->index = 1;
  ioctl(fd, HISTORY_CMD, ioctl_history_struct);
  printf("history 1 index result: %d\n", out);
}
