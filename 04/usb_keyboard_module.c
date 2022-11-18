// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init main_init(void)
{
	pr_info("glafond-: Module Loaded!\n");
	return 0;
}

static void __exit main_exit(void)
{
	pr_info("glafond-: Module Unloaded!\n");
}

module_init(main_init);
module_exit(main_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("glafond-");
MODULE_DESCRIPTION("module Hello world !");
MODULE_VERSION("0.1");
