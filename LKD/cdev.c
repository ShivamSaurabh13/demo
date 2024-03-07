#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kdev_t.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/device.h>
#include <linux/cdev.h>


dev_t dev = 0;
struct class *dev_class;
struct cdev cdev_hai;

static int __init hello_world_init(void);
static void __exit hello_world_exit(void);
static int open_kro(struct inode *inode,struct file *file);
static int release_kro(struct inode *inode, struct file *file);
static ssize_t read_kro(struct file *filp, char __user *buff, size_t len, loff_t *off);
static ssize_t write_kro(struct file *filp,const char __user *buff, size_t len, loff_t *off);


struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = read_kro,
	.write = write_kro,
	.open = open_kro,
	.release = release_kro,
};

static int open_kro(struct inode *inode, struct file *file){
	pr_info("Driver open : Shivam re");
	return 0;
}

static int release_kro(struct inode *inode, struct file *file){
	pr_info("Driver close: shivam ja");	
	return 0;
}

ssize_t read_kro(struct file *filp,char __user *buff,size_t len,loff_t *off){
	pr_info("Driver read call: shivam sunna");
	return 0;
}

ssize_t write_kro(struct file *filp,const char __user *buff, size_t len, loff_t *off){
	pr_info("Driver write call: shivam kuch sunna");
	return len;
}






int __init hello_world_init(void){
	
	dev = alloc_chrdev_region(&dev,0,1,"Shivam_major");
	if(dev < 0){
		pr_err("allocation major");
		return -1;
	}

	cdev_init(&cdev_hai,&fops);
	
	if(cdev_add(&cdev_hai,dev,1) < 0){
		pr_err("Cannot add the device  to the system\n");
		goto err_class;	
	}

	dev_class = class_create(THIS_MODULE,"SHIVAM_CLASS");
	if(IS_ERR(dev_class)){
		pr_err("Class creation error");
		goto err_class;
	}
		
	if(IS_ERR((device_create(dev_class,NULL,dev,NULL,"shivam_device")))){
		pr_err("device creation error");
		goto err_device;	
	}
	pr_info("Kernel module INSERTED successfully\n");
	return 0;
err_device:
	class_destroy(dev_class);
err_class:
	unregister_chrdev_region(dev,1);

	return -1;	
}




static void __exit hello_world_exit(void){
	device_destroy(dev_class,dev);
	class_destroy(dev_class);
	cdev_del(&cdev_hai);
	unregister_chrdev_region(dev,1);
	pr_info("Kernel Module removed successfully");

}

module_init(hello_world_init);
module_exit(hello_world_exit);




MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shivam Saurabh");
MODULE_DESCRIPTION("CREATING THE DEVICE FILE");









