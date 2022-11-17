// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Louis Solofrizzo <louis@ne02ptzero.me>");
MODULE_DESCRIPTION("Useless module");

static ssize_t myfd_read(struct file *fp, char __user *user, size_t size, loff_t *offs);

static ssize_t myfd_write(struct file *fp, const char __user *user, size_t size, loff_t *offs);

static const struct file_operations myfd_fops = {
	.owner = THIS_MODULE,
	.read = &myfd_read,
	.write = &myfd_write,
};

static struct miscdevice myfd_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "reverse",
	.fops = &myfd_fops,
};

char str[PAGE_SIZE] = {0};

static int __init myfd_init(void)
{
	int retval;

	retval = misc_register(&myfd_device);
	return retval;
}

static void __exit myfd_cleanup(void)
{
	misc_deregister(&myfd_device);
}

ssize_t myfd_read(struct file *fp, char __user *user, size_t size, loff_t *offs)
{
	size_t i;
	ssize_t retval;
	char *tmp, *t, *s;

	tmp = kmalloc(sizeof(char) * PAGE_SIZE, GFP_KERNEL);
	s = str;
	while (*s)
		s++;
	i = s - str;
	t = tmp;
	while (s != str)
		*t++ = *--s;
	*t = 0x0;
	retval = simple_read_from_buffer(user, size, offs, tmp, i);
	kfree(tmp);
	return retval;
}

ssize_t myfd_write(struct file *fp, const char __user *user, size_t size, loff_t *offs)
{
	memset(str, 0, PAGE_SIZE);
	return simple_write_to_buffer(str, PAGE_SIZE - 1, offs, user, size);
}

module_init(myfd_init);
module_exit(myfd_cleanup)
