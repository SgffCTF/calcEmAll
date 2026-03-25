#include <linux/atomic.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/types.h>

#include "calc.h"
#include "calculator/calculator.h"

static dev_t dev;
static struct cdev calc_cdev;
static struct class *cls;

static struct calculator_struct *calc_p;

static atomic_t already_open = ATOMIC_INIT(CDEV_NOT_USED);

static long calc_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
    switch (cmd) {
    case ALLOC_CMD:
        calc_p = create_calculator();
        return 0;
    case FREE_CMD:
        kfree(calc_p);
        return 0;
    case CALL_CMD:
        if (!calc_p)
            return 0;

        struct ioctl_arg arg_struct;
        if (copy_from_user(&arg_struct, (void *)arg, sizeof(arg_struct))) {
            return -EACCES;
        }

        int64_t res;
        switch (arg_struct.command) {
        case DIV_CMD:
            int res_code =
                calc_p->div_func(arg_struct.arg1, arg_struct.arg2, &res);
            if (res_code != 0)
                return res_code;
            break;
        case SUB_CMD:
            res = calc_p->sub_func(arg_struct.arg1, arg_struct.arg2);
            break;
        case SUM_CMD:
            res = calc_p->sum_func(arg_struct.arg1, arg_struct.arg2);
            break;
        case MUL_CMD:
            res = calc_p->mul_func(arg_struct.arg1, arg_struct.arg2);
            break;
        default:
            return -EINVAL;
        }

        if (calc_p->historyCount == HISTORY_SIZE) {
            for (int i = HISTORY_SIZE - 2; i >= 0; i--) {
                calc_p->history[i] = calc_p->history[i + 1];
            }
            calc_p->history[HISTORY_SIZE - 1] = res;
        } else {
            calc_p->history[calc_p->historyCount] = res;
            calc_p->historyCount++;
        }

        if (copy_to_user((int64_t __user *)arg_struct.out, &res, sizeof(res))) {
            return -EFAULT;
        }

        return 0;
    case HISTORY_CMD:
        if (!calc_p)
            return -EFAULT;

        struct ioctl_history_arg history_struct;
        if (copy_from_user(&history_struct, (void *)arg,
                           sizeof(history_struct))) {
            return -EACCES;
        }

        if (calc_p->historyCount < history_struct.index) {
            return -EINVAL;
        }

        int64_t val = calc_p->history[history_struct.index];
        if (copy_to_user((int64_t __user *)history_struct.out, &val,
                         sizeof(val))) {
            return -EFAULT;
        }

        return 0;
    }
    return -1;
}

static int calc_open(struct inode *nodp, struct file *filp) {
    if (atomic_cmpxchg(&already_open, CDEV_NOT_USED, CDEV_USED))
        return -EBUSY;

    try_module_get(THIS_MODULE);
    pr_info("%s: module open\n", DEVICE_NAME);

    return 0;
}

static int calc_release(struct inode *nodp, struct file *filp) {
    atomic_set(&already_open, CDEV_NOT_USED);

    module_put(THIS_MODULE);
    pr_info("%s: module released\n", DEVICE_NAME);

    return 0;
}

static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = calc_open,
    .release = calc_release,
    .unlocked_ioctl = calc_ioctl,
};

static int module_register(void) {
    pr_info("%s: registering module\n", DEVICE_NAME);

    if (alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME)) {
        return -1;
    }

    cdev_init(&calc_cdev, &fops);
    if (cdev_add(&calc_cdev, dev, 1)) {
        return -2;
    }

    cls = class_create(DEVICE_NAME);
    if (IS_ERR(cls)) {
        pr_err("%s: class create failed\n", DEVICE_NAME);
        return PTR_ERR(cls);
    }

    if (IS_ERR(device_create(cls, NULL, dev, NULL, DEVICE_NAME))) {
        pr_err("%s: device create failed\n", DEVICE_NAME);
        class_destroy(cls);
        return -EINVAL;
    }

    return 0;
}

static void module_delete(void) {
    pr_info("%s: deleting module\n", DEVICE_NAME);

    device_destroy(cls, dev);
    class_destroy(cls);
    cdev_del(&calc_cdev);
    unregister_chrdev_region(dev, 1);
}

module_init(module_register);
module_exit(module_delete);
MODULE_LICENSE("GPL");
