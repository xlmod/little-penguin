// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/nsproxy.h>
#include <../fs/mount.h>
#include <linux/delay.h>

#define MNAME "mymounts"

char path_list[PATH_MAX] = {0};

static void get_size(struct mnt_namespace *mnt_ns, int *size)
{
	struct mount *mnt;
	struct path mnt_path;
	char *path;
	int count = 0;
	size_t total_len = 0;
	size_t len;

	list_for_each_entry(mnt, &mnt_ns->list, mnt_list) {
		if (mnt->mnt_id != 1) {
			mnt_path.mnt = &mnt->mnt;
			mnt_path.dentry = mnt->mnt.mnt_root;
			path = d_path(&mnt_path, path_list, PATH_MAX);
			if (IS_ERR(path)) {
				return ;
			}
			total_len += strlen(path);
			count++;
			len = strlen(mnt->mnt_devname);
			if (len > size[1])
				size[1] = len;
		}
	}
	size[1] += 8 - (size[1] % 8);
	size[0] = size[1] * count + total_len + count;
}

static ssize_t mfile_read(struct file *file, char __user *buf,
			  size_t count, loff_t *ppos)
{
	struct mnt_namespace *mnt_ns = current->nsproxy->mnt_ns;
	struct mount *mnt;
	struct path mnt_path;
	char *path;
	int size[2] = {0};
	ssize_t retval;
	off_t offset = 0;
	char *output;

	get_size(mnt_ns, size);
	output = kzalloc(size[0], GFP_KERNEL);
	if (!output)
		return -ENOMEM;
	memset(path_list, 0, PATH_MAX);
	list_for_each_entry(mnt, &mnt_ns->list, mnt_list) {
		if (mnt->mnt_id != 1) {
			mnt_path.mnt = &mnt->mnt;
			mnt_path.dentry = mnt->mnt.mnt_root;
			path = d_path(&mnt_path, path_list, PATH_MAX);
			if (IS_ERR(path)) {
				kfree(output);
				return -1;
			}
			offset += sprintf(output + offset, "%-*s%s\n", size[1],
					 mnt->mnt_devname, path);
		}
	}
	retval = simple_read_from_buffer(buf, count, ppos, output, size[0]);
	kfree(output);
	return retval;
}

static const struct proc_ops mfile_proc_ops = {
	.proc_read  = mfile_read,
};

struct proc_dir_entry *mfile;

static int __init main_init(void)
{
	mfile = proc_create(MNAME, 0444, NULL, &mfile_proc_ops);
	if (!mfile) {
		pr_err(MNAME ": proc_create failled!\n");
		return -1;
	}
	pr_info(MNAME ": proc file created!\n");
	return 0;
}

static void __exit main_exit(void)
{
	proc_remove(mfile);
	pr_info(MNAME ": proc file removed!\n");
}

module_init(main_init);
module_exit(main_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("glafond-");
MODULE_DESCRIPTION("assignment 09");
MODULE_VERSION("0.1");
