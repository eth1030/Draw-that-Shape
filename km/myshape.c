#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched/signal.h>
#include <linux/jiffies.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <asm/uaccess.h>
#include <linux/types.h>
#include <linux/proc_fs.h>
#include <linux/fcntl.h>
#include <asm/system_misc.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/gpio/consumer.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Emika Hammond <eth@bu.edu>");
MODULE_AUTHOR("Shamir Legaspi <slegaspi@bu.edu>");

// gpio definitions
#define GREEN 67
#define RED 68
#define TARGET0 44
#define TARGET1 26
#define TARGET2 46
#define BUTTON 65


/* global variables */
struct gpio_desc *green_desc, *red_desc, *target0_desc, *target1_desc, *target2_desc, *bt_desc;
int BT_irq;


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
