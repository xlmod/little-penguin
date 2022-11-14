// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");

MODULE_AUTHOR("glafond-");

MODULE_DESCRIPTION("module Hello world !");

MODULE_VERSION("0.1");

static int __init main_start(void)
{
	pr_info("Hello world !\n");
	return 0;
}

static void __exit main_cleanup(void)
{
	pr_info("Cleaning up module.\n");
}

module_init(main_start);
module_exit(main_cleanup);
