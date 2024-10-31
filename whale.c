/*
 * whale.c - Kernel module with whales
 *        .
 *       ":"
 *     ___:____     |"\/"|
 *   ,'        `.    \  /
 *   |  O        \___/  |
 * ~^~^~^~^~^~^~^~^~^~^~^~^~
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/version.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/uaccess.h>

#include "whale.h"

MODULE_AUTHOR("Alexey Lunev");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_DESCRIPTION("Kernel module with whales");

static int major;
static struct class *cls;

static struct file_operations whale_fops = {
	.read 		= whale_read,
	.write 		= whale_write,
	.open 		= whale_open,
	.release 	= whale_release
};

static int __init whale_init(void) {
	major = register_chrdev(0, DEVICE_NAME, &whale_fops);
	if (major < 0) {
		pr_alert("Registering char device failed with %d\n", major);
		return major;
	};

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 4, 0)
	cls = class_create(DEVICE_NAME);
#else
	cls = class_create(THIS_MODULE, DEVICE_NAME);
#endif
	cls->devnode = whale_devnode;

	device_create(cls, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);

	pr_info(WHALE);
	return SUCCESS;
}

static void __exit whale_exit(void) {
	device_destroy(cls, MKDEV(major, 0));
	class_destroy(cls);

	unregister_chrdev(major, DEVICE_NAME);
}

static int whale_open(struct inode *inode, struct file *file) {
	try_module_get(THIS_MODULE);

	return SUCCESS;
}

static int whale_release(struct inode *inode, struct file *file) {
	module_put(THIS_MODULE);

	return SUCCESS;
}

static ssize_t whale_read(
		struct file *filp,
		char __user *buffer,
		size_t length,
		loff_t *offset
) {
	int bytes_read = 0;
	const char *msg = WHALE;

	if (!*(msg + *offset)) {
		*offset = 0;
		return 0;
	}

	while (length && *msg) {
		put_user(*(msg++), buffer++);

		length--;
		bytes_read++;
	}

	*offset += bytes_read;

	return bytes_read;
}

static ssize_t whale_write(
		struct file *filp,
		const char __user *buffer,
		size_t length,
		loff_t *offset		
) {
	return -EINVAL;
}

static char *whale_devnode(const struct device *dev, umode_t *mode) {
	if (!mode)
		return NULL;
	if (dev->devt == MKDEV(major, 0))
		*mode = 0444;
	return NULL;
}

module_init(whale_init);
module_exit(whale_exit);
