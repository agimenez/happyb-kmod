#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define MAX_NAME 20

MODULE_DESCRIPTION("Happy Birthday Module");
MODULE_AUTHOR("L. Alberto Giménez <alberto.gimenez@capside.com>");
MODULE_LICENSE("GPL");

static char *param_name = "my friend";
module_param(param_name, charp, S_IRUGO);

static char name[MAX_NAME];

static ssize_t happyb_read(struct file *file, char *buf, size_t count, loff_t *f_pos)
{
	char msg[128];
	int len;

	len = snprintf(msg, sizeof msg, "Happy birthday, %s!\n", name);
	if (*f_pos >= len) {
		return 0;
	}

	if (count > len) {
		count = len;
	}

	copy_to_user(buf, msg, count);
	*f_pos += count;

	return count;
}

static ssize_t happyb_write(struct file *file, const char *buf, size_t count, loff_t *f_pos)
{
	int len = count;
	int i;

	if (len > MAX_NAME) {
		len = MAX_NAME;
	}

	copy_from_user(&name, buf, len);
	for (i = 0; i < len; i++) {
		if (name[i] == '\n') {
			name[i] = '\0';
			break;
		}
	}
	name[MAX_NAME-1] = '\0';

	*f_pos += count;

	return count;
}

struct file_operations happyb_fops = {
	read: happyb_read,
	write: happyb_write,
};

struct miscdevice happyb_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "happy-birthday",
	.fops = &happyb_fops,
	.mode = 0666,
};


static int __init happyb_init(void)
{
	int res;

	strncpy(name, param_name, MAX_NAME);
	pr_info("Happy birthday, %s!\n", name);

	res = misc_register(&happyb_dev);
	if (res < 0) {
		pr_info("Could not register happyb device");
		return res;
	}


	return 0;
}

static void __exit happyb_exit(void)
{
	misc_deregister(&happyb_dev);
	pr_info("Have a nice day, %s\n", name);
}

module_init(happyb_init);
module_exit(happyb_exit);
