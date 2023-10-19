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
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include "whale.h"

static int whale_major;
static int whalestream_major;

static char whale_msg[BUF_LEN];
static char *whale_msg_ptr;

static char whalestream_msg[BUF_LEN];
static char *whalestream_msg_ptr;

static struct class *whale_cls;
static struct class *whalestream_cls;

static struct file_operations whale_fops = {
    .read    = whale_device_read,
    .write   = whale_device_write,
    .open    = whale_device_open,
    .release = whale_device_release
};

static struct file_operations whalestream_fops = {
    .read    = whalestream_device_read,
    .write   = whalestream_device_write,
    .open    = whalestream_device_open,
    .release = whalestream_device_release
};

int init_module(void)
{
    whale_major = register_chrdev(0, WHALE_DEVICE_NAME, &whale_fops);
    if (whale_major < 0) {
        printk(KERN_ALERT "Registering char device whale failed with %d\n", whale_major);
        return whale_major;
    }

    whalestream_major = register_chrdev(0, WHALESTREAM_DEVICE_NAME, &whalestream_fops);
    if (whale_major < 0) {
        printk(KERN_ALERT "Registering char device whalestream failed with %d\n", whalestream_major);
        return whalestream_major;
    }

    whale_cls = class_create(WHALE_DEVICE_NAME);
    whalestream_cls = class_create(WHALESTREAM_DEVICE_NAME);

    whale_cls->dev_uevent = whale_uevent;
    whalestream_cls->dev_uevent = whalestream_uevent;

    device_create(whale_cls, NULL, MKDEV(whale_major, 0), NULL, WHALE_DEVICE_NAME);
    device_create(whalestream_cls, NULL, MKDEV(whalestream_major, 0), NULL, WHALESTREAM_DEVICE_NAME);

    printk(KERN_INFO WHALE);
    return SUCCESS;
}

void cleanup_module(void)
{
    unregister_chrdev(whale_major, WHALE_DEVICE_NAME);
    unregister_chrdev(whalestream_major, WHALESTREAM_DEVICE_NAME);

    class_destroy(whale_cls);
    class_destroy(whalestream_cls);

    unregister_chrdev(whale_major, WHALE_DEVICE_NAME);
    unregister_chrdev(whalestream_major, WHALESTREAM_DEVICE_NAME);
}

static int whale_device_open(struct inode *inode, struct file *file)
{
    sprintf(whale_msg, WHALE);
    whale_msg_ptr = whale_msg;

    try_module_get(THIS_MODULE);

    return SUCCESS;
}

static int whalestream_device_open(struct inode *inode, struct file *file)
{
    sprintf(whalestream_msg, WHALE);
    whalestream_msg_ptr = whalestream_msg;

    try_module_get(THIS_MODULE);

    return SUCCESS;
}

static int whale_device_release(struct inode *inode, struct file *file)
{
    module_put(THIS_MODULE);

    return 0;
}

static int whalestream_device_release(struct inode *inode, struct file *file)
{
    module_put(THIS_MODULE);

    return 0;
}

static ssize_t whale_device_read(struct file *filp,
        char *buffer,
        size_t length,
        loff_t * offset)
{
    int bytes_read = 0;

    if (*whale_msg_ptr == 0)
        return 0;

    while (length && *whale_msg_ptr) {
        put_user(*(whale_msg_ptr++), buffer++);

        length--;
        bytes_read++;
    }

    return bytes_read;
}

static ssize_t whalestream_device_read(struct file *filp,
        char *buffer,
        size_t length,
        loff_t * offset)
{
    int bytes_read = 0;

    if (*whalestream_msg_ptr == 0)
        whalestream_msg_ptr = whalestream_msg;

    while (length && *whalestream_msg_ptr) {
        put_user(*(whalestream_msg_ptr++), buffer++);

        length--;
        bytes_read++;
    }

    return bytes_read;
}

static ssize_t whale_device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{
    return -EINVAL;
}

static ssize_t whalestream_device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{
    return -EINVAL;
}

static int whale_uevent(const struct device *dev, struct kobj_uevent_env *env)
{
    add_uevent_var(env, "DEVMODE=%#o", 0666);
    return SUCCESS;
}

static int whalestream_uevent(const struct device *dev, struct kobj_uevent_env *env)
{
    add_uevent_var(env, "DEVMODE=%#o", 0666);
    return SUCCESS;
}

MODULE_LICENSE("GPL");
