#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched/signal.h>
#include <linux/jiffies.h>
#include <linux/slab.h>
#include <linux/random.h>
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
#define FALSE 0
#define TRUE 1
#define CIRCLE 0
#define TRIANGLE 1
#define RECTANGLE 2
#define MAJOR_NUM 61


#define BUF_SIZE 256


/* function declarations */
static ssize_t my_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
static ssize_t my_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);

/* define structs */
struct file_operations my_fops = {
	read: my_read,
	write: my_write
};

/* global variables */
struct gpio_desc *green_desc, *red_desc, *target0_desc, *target1_desc, *target2_desc, *BT_desc;
int BT_irq;
int READY_FOR_USER = FALSE;
int shape;
char shape_buf[BUF_SIZE];
struct timer_list timer;

static int myshape_init(void);
static void myshape_exit(void);


// generates a random number 0 to 2 inclusive
int rand3(void) {
	int i;
	get_random_bytes(&i, sizeof(i));
	return i%3;
}

static irq_handler_t BT_irq_handler(unsigned int irq, void *dev_id, struct pt_regs *regs) {
	shape = rand3();
	gpiod_set_value(red_desc, OFF);
	gpiod_set_value(green_desc, OFF);
	switch (shape) {
		case CIRCLE:
		gpiod_set_value(target0_desc, ON);
		gpiod_set_value(target1_desc, OFF);
		gpiod_set_value(target2_desc, OFF);	
		break;
		case TRIANGLE:
		gpiod_set_value(target0_desc, ON);
		gpiod_set_value(target1_desc, ON);
		gpiod_set_value(target2_desc, OFF);	
		break;
		case RECTANGLE:
		gpiod_set_value(target0_desc, ON);
		gpiod_set_value(target1_desc, ON);
		gpiod_set_value(target2_desc, ON);	
		break;
	}
	// set timer for 10 seconds
	mod_timer(&timer, jiffies + 10 * HZ);
	return (irq_handler_t) IRQ_HANDLED;
}

void mytimer_callback(struct timer_list* timer) {
	READY_FOR_USER = TRUE;
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
	if (request_irq(BT_irq, (irq_handler_t)BT_irq_handler, IRQF_TRIGGER_RISING,  "BT_irq_handler", NULL) != 0) {
		printk(KERN_ALERT "BT request failed\n");
	}

	// set up character device
	if ((register_chrdev(MAJOR_NUM, "myshape", &my_fops)) < 0) {
		printk(KERN_ALERT "character device registration failed\n");
	}

	// setup timer
	timer_setup(&timer, mytimer_callback, 0);



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
	del_timer(&timer);
	unregister_chrdev(MAJOR_NUM, "myshape");
}

// user reads kernel writes
static ssize_t my_read(struct file *filp, char *buf, size_t count, loff_t *f_pos) {
	int read_len = 0;
	if (*f_pos > 0 || !READY_FOR_USER) {
		return 0;
	}
	// alert user prog to write detection results to char dev
	memset(shape_buf, 0, BUF_SIZE);
	read_len += sprintf(shape_buf, "r");
	*f_pos += read_len;
	return read_len;
}

static ssize_t my_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos) {
	int classified_shape;
	if (copy_from_user(shape_buf, buf, count)) {
		return -EFAULT;
	}
	if (kstrtouint(shape_buf, 10, &classified_shape)) {
		return 0;
	}
	// turn on lights
	READY_FOR_USER = FALSE;
	if (classified_shape == shape){
		gpiod_set_value(red_desc, OFF);
		gpiod_set_value(green_desc, ON);
	} else {
		gpiod_set_value(red_desc, ON);
		gpiod_set_value(green_desc, OFF);
	}
	memset(shape_buf, 0, BUF_SIZE);
	return count;
}

module_init(myshape_init);
module_exit(myshape_exit);
