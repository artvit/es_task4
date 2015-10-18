#include <linux/module.h>
#include <linux/string.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/kernel.h>  
#include <linux/fs.h>
#include <asm/uaccess.h>

MODULE_LICENSE("Dual BSD/GPL");

#define MAJOR_NUMBER 138


static int    majorNumber = MAJOR_NUMBER;                  ///< Stores the device number -- determined automatically

#define  FST_DEVICE_NAME "calc-1st"    ///< The device will appear at /dev/CalcModule using this value
static int     dev1_open(struct inode *, struct file *);
static int     dev1_release(struct inode *, struct file *);
static ssize_t dev1_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev1_write(struct file *, const char *, size_t, loff_t *);
static struct file_operations fops_1 =
{
	.open = dev1_open,
	.read = dev1_read,
	.write = dev1_write,
	.release = dev1_release,
};


#define  SND_DEVICE_NAME "calc-2nd"
static int     dev2_open(struct inode *, struct file *);
static int     dev2_release(struct inode *, struct file *);
static ssize_t dev2_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev2_write(struct file *, const char *, size_t, loff_t *);
static struct file_operations fops_2 =
{
	.open = dev2_open,
	.read = dev2_read,
	.write = dev2_write,
	.release = dev2_release,
};


#define  OP_DEVICE_NAME "calc-oper"
static int     dev_op_open(struct inode *, struct file *);
static int     dev_op_release(struct inode *, struct file *);
static ssize_t dev_op_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_op_write(struct file *, const char *, size_t, loff_t *);
static struct file_operations fops_op =
{
	.open = dev_op_open,
	.read = dev_op_read,
	.write = dev_op_write,
	.release = dev_op_release,
};


#define  RES_DEVICE_NAME "calc-res"
static int     dev_res_open(struct inode *, struct file *);
static int     dev_res_release(struct inode *, struct file *);
static ssize_t dev_res_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_res_write(struct file *, const char *, size_t, loff_t *);
static struct file_operations fops_r =
{
	.open = dev_res_open,
	.read = dev_res_read,
	.write = dev_res_write,
	.release = dev_res_release,
};


static char		buffer_1[30] = {0};
static short	read_pos_1 = 0;
static long int 		a;

static char		buffer_2[30] = {0};
static short	read_pos_2 = 0;
static long int 		b;

static char		buffer_op[5] = {0};
static short	read_pos_op = 0;
static char		op;

static short	res_size = 0;
static short	times = 0;


static int __init calc_init(void)
{
	int result;
	result = register_chrdev(majorNumber, FST_DEVICE_NAME, &fops_1);
	if (majorNumber<0) {
		printk(KERN_ALERT "CalcModule failed to register first device\n");
		return majorNumber;
	}
	
	result = register_chrdev(majorNumber + 1, SND_DEVICE_NAME, &fops_2);
	if (result < 0) {
		unregister_chrdev(majorNumber, FST_DEVICE_NAME);
		printk(KERN_ALERT "CalcModule failed to register second device\n");
		return result;
	}

	result = register_chrdev(majorNumber + 2, OP_DEVICE_NAME, &fops_op);
	if (result < 0) {
		unregister_chrdev(majorNumber, FST_DEVICE_NAME);
		unregister_chrdev(majorNumber + 1, SND_DEVICE_NAME);
		printk(KERN_ALERT "CalcModule failed to register third device\n");
		return result;
	}

	result = register_chrdev(majorNumber + 3, RES_DEVICE_NAME, &fops_r);
	if (result < 0) {
		unregister_chrdev(majorNumber, FST_DEVICE_NAME);
		unregister_chrdev(majorNumber + 1, SND_DEVICE_NAME);
		unregister_chrdev(majorNumber + 2, OP_DEVICE_NAME);
		printk(KERN_ALERT "CalcModule failed to register fourth device\n");
		return result;
	}
	
	printk(KERN_INFO "CalcModule: registered correctly with major number %d\n", majorNumber);

	return 0;
}

static void __exit calc_exit(void)
{
	unregister_chrdev(majorNumber, FST_DEVICE_NAME);
	unregister_chrdev(majorNumber + 1, SND_DEVICE_NAME);
	unregister_chrdev(majorNumber + 2, OP_DEVICE_NAME);
	unregister_chrdev(majorNumber + 3, RES_DEVICE_NAME);
	printk(KERN_INFO "CalcModule: Goodbye from the LKM!\n");
}




static int dev1_open(struct inode *inodep, struct file *filep)
{
	printk(KERN_INFO "CalcModule: First operand device has been opened");
	return 0;
}

static ssize_t dev1_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
	short count = 0;
	while (len && buffer_1[read_pos_1] != 0) {
		put_user(buffer_1[read_pos_1], buffer++);
		count++;
		len--;
		read_pos_1++;
	}
	return count;
}

static ssize_t dev1_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
	short count = 0;
	memset(buffer_1, 0, 30);
	read_pos_1 = 0;
	while (len > 0) {
		buffer_1[count] = buffer[count++];
		len--;
	}
	short result = kstrtol(buffer_1, 0, &a);
	return count;
}

static int dev1_release(struct inode *inodep, struct file *filep)
{
	printk(KERN_INFO "CalcModule: First operand device successfully closed\n");
	return 0;
}



static int dev2_open(struct inode *inodep, struct file *filep)
{
	printk(KERN_INFO "CalcModule: Second operand device has been opened");
	return 0;
}

static ssize_t dev2_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
	short count = 0;
	while (len && buffer_2[read_pos_2] != 0) {
		put_user(buffer_2[read_pos_2], buffer++);
		count++;
		len--;
		read_pos_2++;
	}
	return count;
}

static ssize_t dev2_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
	short count = 0;
	memset(buffer_2, 0, 30);
	read_pos_2 = 0;
	while (len > 0) {
		buffer_2[count] = buffer[count++];
		len--;
	}
	short result = kstrtol(buffer_2, 0, &b);
	return count;
}

static int dev2_release(struct inode *inodep, struct file *filep)
{
	printk(KERN_INFO "CalcModule: Second operand device successfully closed\n");
	return 0;
}



static int dev_op_open(struct inode *inodep, struct file *filep)
{
	printk(KERN_INFO "CalcModule: Operator device has been opened");
	return 0;
}

static ssize_t dev_op_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
	short count = 0;
	while (len && buffer_op[read_pos_op] != 0) {
		put_user(buffer_op[read_pos_op], buffer++);
		count++;
		len--;
		read_pos_op++;
	}
	return count;
}

static ssize_t dev_op_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
	short count = 0;
	memset(buffer_op, 0, 5);
	read_pos_op = 0;
	while (len > 0) {
		buffer_op[count] = buffer[count++];
		len--;
	}
	op = buffer_op[0];
	return count;
}

static int dev_op_release(struct inode *inodep, struct file *filep)
{
	printk(KERN_INFO "CalcModule: Operator device successfully closed\n");
	return 0;
}



static int dev_res_open(struct inode *inodep, struct file *filep)
{
	printk(KERN_INFO "CalcModule: Result device has been opened");
	return 0;
}

static ssize_t dev_res_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
	long int res = 0;
	char buf[30];
	memset(buf, 0, 30);

	if (times != 0)
		return 0;
	switch (op) {
	case '+':
		res = a + b;
		break;
	case '-':
		res = a - b;
		break;
	case '*':
		res = a * b;
		break;
	case '/':
		if (b == 0) {
			printk(KERN_ALERT "Division by 0!!!");
			return -1;
		} else {
			res = a / b;
		}
		break;
	default:
		printk(KERN_ALERT "Unknown operation!!!");
		break;
	}
	
	sprintf(buf, "%ld\n", res);
	
	printk(KERN_ALERT "a:%ld b:%ld op:%c res:%ld", a, b, op, res);
	res_size = strlen(buf);
	copy_to_user(buffer, buf, res_size);
	times++;
	return res_size;
}

static ssize_t dev_res_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
	return -1;
}

static int dev_res_release(struct inode *inodep, struct file *filep)
{
	times = 0;
	res_size = 0;
	printk(KERN_INFO "CalcModule: Result device successfully closed\n");
	return 0;
}



module_init(calc_init);
module_exit(calc_exit);