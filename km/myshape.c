#include <linux/module.h>
#include <linux/kernel.h>

static int myshape_init(void);
static void myshape_exit(void);

static int myshape_init(void) {
	printk(KERN_ALERT "Hello world\n");
	return 0;
}

static void myshape_exit(void) {
	printk(KERN_ALERT "Goodbye world\n");
}

module_init(myshape_init);
module_exit(myshape_exit);
