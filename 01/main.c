
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");

MODULE_AUTHOR("glafond-");

MODULE_DESCRIPTION("module Hello world !");

MODULE_VERSION("0.1");

static int __init main_start(void) {
	printk(KERN_INFO "Hello world !\n");
	return 0;
}

static void __exit main_cleanup(void) {
	printk(KERN_INFO "Cleaning up module.\n");
}

module_init(main_start);
module_exit(main_cleanup);
