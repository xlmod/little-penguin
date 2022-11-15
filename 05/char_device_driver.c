// SPDX-License-Identifier: GPL-2.0
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>

#define LOGIN "glafond-"

static ssize_t misc_write(struct file *file, const char __user *buf,
			  size_t len, loff_t *ppos);
static ssize_t misc_read(struct file *file, char __user *buf,
			 size_t count, loff_t *ppos);

static const struct file_operations fops = {
	.write	= misc_write,
	.read	= misc_read,
};

struct miscdevice device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "fortytwo",
	.fops = &fops,
};

static ssize_t misc_write(struct file *file, const char __user *buf,
			  size_t len, loff_t *ppos)
{
	char tmp[sizeof(LOGIN)] = {0};

	if (len != sizeof(LOGIN) - 1)
		goto misc_write_err;
	simple_write_to_buffer(tmp, sizeof(tmp) - 1, ppos, buf, len);
	if (!memcmp(tmp, LOGIN, sizeof(LOGIN))) {
		pr_info("[/dev/%s] written!\n", device.name);
		return len;
	}
misc_write_err:
	pr_info("[/dev/%s] write error: invalid value!\n", device.name);
	return -EINVAL;
}

static ssize_t misc_read(struct file *file, char __user *buf,
			 size_t count, loff_t *ppos)
{
	pr_info("[/dev/%s] read!\n", device.name);
	return simple_read_from_buffer(buf, count, ppos, LOGIN, sizeof(LOGIN));
}

static int __init main_init(void)
{
	int error;

	error = misc_register(&device);
	if (error) {
		pr_err("misc_char_device: [/dev/%s] registration failed!\n", device.name);
		return error;
	}
	pr_info("misc_char_device: [/dev/%s] registered!\n", device.name);
	return 0;
}

static void __exit main_exit(void)
{
	misc_deregister(&device);
	pr_info("misc_char_device: [/dev/%s] deregistered!\n", device.name);
}

module_init(main_init);
module_exit(main_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("glafond-");
MODULE_DESCRIPTION("assignment 05");
MODULE_VERSION("0.1");
