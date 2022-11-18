// SPDX-License-Identifier: GPL-2.0
#include <linux/debugfs.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#define MNAME "fortytwo"
#define LOGIN "glafond-"

static ssize_t id_write(struct file *file, const char __user *buf,
			size_t len, loff_t *ppos)
{
	char tmp[sizeof(LOGIN)] = {0};

	if (len == sizeof(LOGIN) - 1) {
		simple_write_to_buffer(tmp, sizeof(tmp) - 1, ppos, buf, len);
		if (!memcmp(tmp, LOGIN, sizeof(LOGIN)))
			return len;
	}
	return -EINVAL;
}

static ssize_t id_read(struct file *file, char __user *buf,
		       size_t count, loff_t *ppos)
{
	return simple_read_from_buffer(buf, count, ppos, LOGIN, sizeof(LOGIN));
}

static const struct file_operations id_fops = {
	.owner = THIS_MODULE,
	.write = id_write,
	.read  = id_read,
};

static ssize_t jiffies_read(struct file *file, char __user *buf,
			    size_t count, loff_t *ppos)
{
	char tmp[20];

	size_t size = snprintf(tmp, sizeof(tmp), "%llu", get_jiffies_64());
	return simple_read_from_buffer(buf, count, ppos, tmp, size);
}

static const struct file_operations jiffies_fops = {
	.owner = THIS_MODULE,
	.read  = jiffies_read,
};

DEFINE_MUTEX(foo_lock);
char *data;
ssize_t data_size;

static ssize_t foo_write(struct file *file, const char __user *buf,
			 size_t len, loff_t *ppos)
{
	mutex_lock(&foo_lock);
	memset(data, 0, PAGE_SIZE);
	pr_info(MNAME ": len: %lu  page_size: %lu\n", len, PAGE_SIZE);
	data_size = simple_write_to_buffer(data, PAGE_SIZE, ppos, buf, len);
	pr_info(MNAME ": size: %lu\n", data_size);
	mutex_unlock(&foo_lock);
	return data_size;
}

static ssize_t foo_read(struct file *file, char __user *buf,
			size_t count, loff_t *ppos)
{
	ssize_t size;

	mutex_lock(&foo_lock);
	size = simple_read_from_buffer(buf, count, ppos, data, data_size);
	mutex_unlock(&foo_lock);
	return size;
}

static const struct file_operations foo_fops = {
	.owner = THIS_MODULE,
	.write = foo_write,
	.read  = foo_read,
};

struct dentry *mdir;

static int __init main_init(void)
{
	mdir = debugfs_create_dir(MNAME, NULL);
	if (mdir == ERR_PTR(-ENODEV)) {
		pr_err(MNAME ": debugfs_create_dir failled!\n");
		return -ENODEV;
	} else if (mdir == ERR_PTR(-EPERM)) {
		pr_err(MNAME ": debugfs_create_dir: Permission denied!\n");
		return -EPERM;
	}
	pr_info(MNAME ": debugfs dir created!\n");

	debugfs_create_file("id", 0666, mdir, NULL, &id_fops);
	pr_info(MNAME ": " MNAME "/id debugfs file created!\n");

	debugfs_create_file("jiffies", 0444, mdir, NULL, &jiffies_fops);
	pr_info(MNAME ": " MNAME "/jiffies debugfs file created!\n");

	debugfs_create_file("foo", 0644, mdir, NULL, &foo_fops);
	pr_info(MNAME ": " MNAME "/foo debugfs file created!\n");

	data = kzalloc(PAGE_SIZE, GFP_KERNEL);

	return 0;
}

static void __exit main_exit(void)
{
	kfree(data);
	debugfs_remove_recursive(mdir);
	pr_info(MNAME ": debugfs dir removed!\n");
}

module_init(main_init);
module_exit(main_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("glafond-");
MODULE_DESCRIPTION("assignment 07");
MODULE_VERSION("0.1");
