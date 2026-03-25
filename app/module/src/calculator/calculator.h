#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <linux/types.h>

#define HISTORY_SIZE 8

struct calculator_struct {
    int64_t history[HISTORY_SIZE];
    int historyCount;

    int64_t (*sum_func)(int64_t, int64_t);
    int64_t (*mul_func)(int64_t, int64_t);
    int64_t (*sub_func)(int64_t, int64_t);
    int (*div_func)(int64_t, int64_t, int64_t *);
};

int64_t sum(int64_t arg1, int64_t arg2);
int64_t mul(int64_t arg1, int64_t arg2);
int div(int64_t arg1, int64_t arg2, int64_t *out);
int64_t sub(int64_t arg1, int64_t arg2);
struct calculator_struct *create_calculator(void);

#endif
