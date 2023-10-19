/*
 * whale.c: Outputs whale ASCII art at init and creates
 * block device returning only whales
 * 
 *        .
 *       ":"
 *     ___:____     |"\/"|
 *   ,'        `.    \  /
 *   |  O        \___/  |
 * ~^~^~^~^~^~^~^~^~^~^~^~^~
 *
 */

// #ifdef CONFIG_WHALE
#include <linux/atomic.h>
#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/uaccess.h>

#include <asm/errno.h>


#define WHALE1 "       .                  \n"
#define WHALE2 "      \":\"               \n"
#define WHALE3 "    ___:____     |\"\\/\"|\n"
#define WHALE4 "  ,'        `.    \\  /   \n"
#define WHALE5 "  |  O        \\___/  |   \n"
#define WHALE6 "~^~^~^~^~^~^~^~^~^~^~^~^~ \n"
#define WHALE WHALE1 WHALE2 WHALE3 WHALE4 WHALE5 WHALE6
#define WHALESIZE sizeof(WHALE)


static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char __user *, size_t, loff_t *); 
static ssize_t device_write(struct file *, const char __user *, size_t, loff_t *);
static int whale_uevent(const struct device *, struct kobj_uevent_env *);

#define SUCCESS     0
#define DEVICE_NAME "whale"
#define BUF_LEN     WHALESIZE

static int major;

enum {
    CDEV_NOT_USED       = 0,
    CDEV_EXCLUSIVE_OPEN = 1,
};

static atomic_t already_open = ATOMIC_INIT(CDEV_NOT_USED);
static char msg[BUF_LEN+1];

static struct class *cls;
static struct file_operations chardev_fops = {
    .read  =   device_read,
    .write =   device_write,
    .open  =   device_open,
    .release = device_release,
};

static int __init chardev_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &chardev_fops);

    if (major < 0) {
        pr_alert("Registeing whale device failed with %d\n", major);
        return major;
    }

    cls = class_create(DEVICE_NAME);
    cls->dev_uevent = whale_uevent;

    device_create(cls, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
    pr_info(WHALE);

    return SUCCESS;
}

static void __exit chardev_exit(void)
{
    device_destroy(cls, MKDEV(major, 0));
    class_destroy(cls);

    unregister_chrdev(major, DEVICE_NAME);
}

static int device_open(struct inode *inode, struct file *file)
{
    if (atomic_cmpxchg(&already_open, CDEV_NOT_USED, CDEV_EXCLUSIVE_OPEN))
        return -EBUSY;

    sprintf(msg, WHALE);
    try_module_get(THIS_MODULE);

    return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
    atomic_set(&already_open, CDEV_NOT_USED);
    module_put(THIS_MODULE);

    return SUCCESS;
}

static ssize_t device_read(struct file *filp,
                           char __user *buffer,
                           size_t length,
                           loff_t *offset)
{
    int bytes_read = 0;
    const char *msg_ptr = msg;

    if (!*(msg_ptr + *offset)) {
        *offset = 0;
        return 0;
    }

    msg_ptr += *offset;

    while (length && *msg_ptr) {
        put_user(*(msg_ptr++), buffer++);
        length--;
        bytes_read++;
    }

    *offset += bytes_read;

    return bytes_read;
}

static ssize_t device_write(struct file *filp, const char __user *buff,
                            size_t len,        loff_t            *off)
{
    return -EINVAL;
}

static int whale_uevent(const struct device *dev, struct kobj_uevent_env *env)
{
    add_uevent_var(env, "DEVMODE=%#o", 0666);
    return SUCCESS;
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL");
// #endif // CONFIG_WHALE
