#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

MODULE_DESCRIPTION("Happy Birthday Module");
MODULE_AUTHOR("L. Alberto Giménez <alberto.gimenez@capside.com>");
MODULE_LICENSE("GPL");

static int happyb_init(void)
{
        pr_debug("Hi\n");
        return 0;
}

static void happyb_exit(void)
{
        pr_debug("Bye\n");
}

module_init(happyb_init);
module_exit(happyb_exit);
