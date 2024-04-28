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

#define OFF 0
#define ON 1


/* global variables */
struct gpio_desc *green_desc, *red_desc, *target0_desc, *target1_desc, *target2_desc, *BT_desc;
int BT_irq;


static int myshape_init(void);
static void myshape_exit(void);


static irq_handler_t BT_irq_handler(unsigned int irq, void *dev_id, struct pt_regs *regs) {
	printk(KERN_ALERT "Button press handled\n");
	return (irq_handler_t) IRQ_HANDLED;
}

static int myshape_init(void) {
	gpio_request(GREEN, "green");
	gpio_request(RED, "red");
	gpio_request(TARGET0, "target0");
	gpio_request(TARGET1, "target1");
	gpio_request(TARGET2, "target2");
	gpio_request(BUTTON, "BT");

	// get descriptions for gpio
	green_desc = gpio_to_desc(GREEN);
	red_desc = gpio_to_desc(RED);
	target0_desc = gpio_to_desc(TARGET0);
	target1_desc = gpio_to_desc(TARGET1);
	target2_desc = gpio_to_desc(TARGET2);
	BT_desc = gpio_to_desc(BUTTON);

	// set LEDs as outputs
	if ((gpiod_direction_output(green_desc, OFF)) != 0) {
		printk(KERN_ALERT "Green failed\n");
	}
	if ((gpiod_direction_output(red_desc, OFF)) != 0) {
		printk(KERN_ALERT "Red failed\n");
	}
	if ((gpiod_direction_output(target0_desc, OFF)) != 0) {
		printk(KERN_ALERT "Target0 failed\n");
	}
	if ((gpiod_direction_output(target1_desc, OFF)) != 0) {
		printk(KERN_ALERT "Target1 failed\n");
	}
	if ((gpiod_direction_output(target2_desc, OFF)) != 0) {
		printk(KERN_ALERT "Target2 failed\n");
	}

	// add button to irq
	BT_irq = gpiod_to_irq(BT_desc);
	if (BT_irq < 0) {
		printk(KERN_ALERT "BT IRQ failed\n");
	}

	// set buttons as inputs
	if (gpiod_direction_input(BT_desc) != 0) {
		printk(KERN_ALERT "BT failed\n");
	}
	// turn everything on
	gpiod_set_value(green_desc, ON);
	gpiod_set_value(red_desc, ON);
	gpiod_set_value(target0_desc, ON);
	gpiod_set_value(target1_desc, ON);
	gpiod_set_value(target2_desc, ON);
	if (request_irq(BT_irq, (irq_handler_t)BT_irq_handler, IRQF_TRIGGER_RISING,  "BT_irq_handler", NULL) != 0) {
		printk(KERN_ALERT "BT request failed\n");
	}

	return 0;
}



static void myshape_exit(void) {
	gpiod_put(green_desc);
	gpiod_put(red_desc);
	gpiod_put(target0_desc);
	gpiod_put(target1_desc);
	gpiod_put(target2_desc);
	free_irq(BT_irq, NULL);
	gpiod_put(BT_desc);
}

module_init(myshape_init);
module_exit(myshape_exit);
