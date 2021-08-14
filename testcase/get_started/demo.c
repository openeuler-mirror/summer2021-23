#include <linux/module.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <asm/atomic.h>
#include <linux/slab.h>				// kzalloc和kfree的头文件

typedef struct 
{
	dev_t dev_no;
	char devname[20];
	char classname[20];
	struct cdev demo_cdev;
	struct class *cls;
	struct device *dev;
	atomic_t lock_status;			// 定义设备使用状态，1表示可用，0表示不可用
	spinlock_t my_lock;				// 定义一个自旋锁
}demo_struct;
demo_struct *my_demo_dev = NULL;	// 定义一个设备结构体指针，指向NULL

static int demo_open(struct inode *inode, struct file *filp)
{
	unsigned long flags;
	
	/* 首先判断设备是否可用 */
	spin_lock_irqsave(&my_demo_dev->my_lock, flags);		// 上锁，保存中断屏蔽状态到flags中，并禁止本地中断
	if (atomic_read(&my_demo_dev->lock_status) == 0)		// 设备不可用
	{
		printk("device busy");
		spin_unlock_irqrestore(&my_demo_dev->my_lock, flags);
		goto err0;
	}
	atomic_dec(&my_demo_dev->lock_status);
	spin_unlock_irqrestore(&my_demo_dev->my_lock, flags);	// 解锁，将中断状态恢复到之前的状态，并激活本地中断

	printk("%s -- %d.\n", __FUNCTION__, __LINE__);
	
	return 0;

err0:
	return -EBUSY;	/* Device or resource busy */
}

static int demo_release(struct inode *inode, struct file *filp)
{
	unsigned long flags;

	/* 首先判断设备是否可用 */
	spin_lock_irqsave(&my_demo_dev->my_lock, flags);		// 上锁，保存中断屏蔽状态到flags中，并禁止本地中断
	if (atomic_read(&my_demo_dev->lock_status) == 0) 		// 设备不可用
	{
		atomic_inc(&my_demo_dev->lock_status);				// 自加1，将其恢复到初始状态，一攻其他应用程序使用
	}
	spin_unlock_irqrestore(&my_demo_dev->my_lock, flags);	// 解锁，将中断状态恢复到之前的状态，并激活本地中断

	printk("%s -- %d.\n", __FUNCTION__, __LINE__);

	
	return 0;
}

struct file_operations demo_ops = {
	.open = demo_open,
	.release=  demo_release,
};

static int __init demo_init(void)
{
	int ret;

	printk("%s -- %d.\n", __FUNCTION__, __LINE__);

	/* 开辟空间 */
	my_demo_dev =  kzalloc(sizeof(demo_struct), GFP_KERNEL);
	if ( IS_ERR(my_demo_dev) )
	{
		printk("kzalloc failed.\n");
		ret = PTR_ERR(my_demo_dev);
		goto kzalloc_err;
	}
	strcpy(my_demo_dev->devname, "demo_chrdev");	// 给设备名字赋值
	strcpy(my_demo_dev->classname, "demo_class");	// 给设备类的名字赋值

	atomic_set(& my_demo_dev->lock_status , 1);		// 初始化锁的状态为1，表示此时可以用
	spin_lock_init(&my_demo_dev->my_lock);			// 初始化自旋锁
	
	ret = alloc_chrdev_region(&my_demo_dev->dev_no, 0, 0, my_demo_dev->devname);
	if (ret)
	{
		printk("alloc_chrdev_region failed.\n");
		goto region_err;
	}

	cdev_init(&my_demo_dev->demo_cdev, &demo_ops);

	ret = cdev_add(&my_demo_dev->demo_cdev, my_demo_dev->dev_no, 1);
	if (ret < 0)
	{
		printk("cdev_add failed.\n");
		goto add_err;
	}

	my_demo_dev->cls = class_create(THIS_MODULE, my_demo_dev->classname);	/* 在目录/sys/class/.. */
	if ( IS_ERR(my_demo_dev->cls) )
	{
		ret = PTR_ERR(my_demo_dev->cls);
		printk("class_create failed.\n");
		goto cls_err;
	}

	my_demo_dev->dev = device_create(my_demo_dev->cls, NULL, my_demo_dev->dev_no, NULL, "chrdev%d", 0);	/* 在目录/dev/.. */
	if ( IS_ERR(my_demo_dev->dev) )
	{
		ret = PTR_ERR(my_demo_dev->dev);
		printk("device_create failed.\n");
		goto dev_err;
	}
	
	return 0;

dev_err:
	class_destroy(my_demo_dev->cls);	
cls_err:
	cdev_del(&my_demo_dev->demo_cdev);
add_err:
	unregister_chrdev_region(my_demo_dev->dev_no, 1);
region_err:
	kfree(my_demo_dev);		// 释放空间，避免内存泄漏
kzalloc_err:
	return ret;
}


static void __exit demo_exit(void)
{
	printk("%s -- %d.\n", __FUNCTION__, __LINE__);

	device_destroy(my_demo_dev->cls, my_demo_dev->dev_no);
	class_destroy(my_demo_dev->cls);
	cdev_del(&my_demo_dev->demo_cdev);
	unregister_chrdev_region(my_demo_dev->dev_no, 1);
	kfree(my_demo_dev);		// 释放空间，避免内存泄漏
}

module_init(demo_init);
module_exit(demo_exit);
MODULE_LICENSE("GPL");
