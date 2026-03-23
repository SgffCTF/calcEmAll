#ifndef TEST_H
#define TEST_H

#include <linux/ioctl.h>

#define MAJOR_NUM 100
#define DEVICE_NAME "calculator"

#define ALLOC_CMD _IO(MAJOR_NUM, 0)
#define FREE_CMD _IO(MAJOR_NUM, 1)
#define CALL_CMD _IOR(MAJOR_NUM, 2, int64_t *)
#define HISTORY_CMD _IOR(MAJOR_NUM, 3, int64_t *)

enum {
  CDEV_NOT_USED = 0,
  CDEV_USED = 1,
};

enum {
  DIV_CMD = 0,
  SUB_CMD = 1,
  SUM_CMD = 2,
  MUL_CMD = 3,
};

struct ioctl_arg {
  int64_t arg1;
  int64_t arg2;
  uint8_t command;
  int64_t *out;
};

struct ioctl_history_arg {
  int64_t index;
  int64_t *out;
};

#endif
