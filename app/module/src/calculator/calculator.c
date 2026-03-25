#include <linux/slab.h>
#include <linux/types.h>

#include "calculator.h"

int64_t sum(int64_t arg1, int64_t arg2) { return arg1 + arg2; }

int64_t mul(int64_t arg1, int64_t arg2) { return arg1 * arg2; }

int div(int64_t arg1, int64_t arg2, int64_t *out) {
    if (arg2 == 0) {
        return -EINVAL;
    }
    *out = arg1 / arg2;
    return 0;
}

int64_t sub(int64_t arg1, int64_t arg2) { return arg1 - arg2; }

struct calculator_struct *create_calculator(void) {
    struct calculator_struct *calc;

    calc = kmalloc(sizeof(*calc), GFP_KERNEL);
    if (!calc)
        return NULL;

    calc->sub_func = sub;
    calc->sum_func = sum;
    calc->mul_func = mul;
    calc->div_func = div;
    for (int i = 0; i < HISTORY_SIZE; i++) {
    	calc->history[i] = 0;
    }
    calc->historyCount = 0;

    return calc;
}
