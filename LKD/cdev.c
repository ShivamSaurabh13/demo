
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>

dev_t dev =0;
static struct class *sh_class;
static struct cdev sh_cdev;

//function definition
int sh_open(struct inode *inode, struct file *filp){
	pr_info("opening the the cdev\n");
	return 0;
}

int sh_release(struct inode *inode, struct file *filp){
	pr_info("releasing the file\n");
	return 0;
}

ssize_t sh_read(struct file *filp, char __user *buf, size_t len, loff_t *off){
	pr_info("reading file opened\n");
	return 0;
}

ssize_t sh_write(struct file *filp, const char __user *buf, size_t len, loff_t *off){
	pr_info("Writting to the file\n");
	return len;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = sh_read,
	.write = sh_write,
	.open = sh_open,
	.release = sh_release,
};


static int __init hello_init(void){
	if((alloc_chrdev_region(&dev,0,1,"Shiva"))<0){
		pr_err("allocation of major and minor allocation");
		return -1;
	}
	pr_info("Major = %d Minor = %d\n",MAJOR(dev),MAJOR(dev));
	
	cdev_init(&sh_cdev,&fops);
	
	if((cdev_add(&sh_cdev,dev,1)) < 0){
		pr_err("Cannot add the device to the system\n");
		goto err_class;
	}
	
	if(IS_ERR(sh_class=class_create(THIS_MODULE,"shiva_ka_class"))){
		pr_err("Cannot create the struct class\n");
		goto err_class;
	}
	if((IS_ERR(device_create(sh_class,NULL,dev,NULL,"shiva_ka_device")))){
		pr_err("Cannot create the device class\n");
		goto err_device;
	}
	pr_info("The init function is working\n");
	return 0;
err_device:class_destroy(sh_class);
err_class:unregister_chrdev_region(dev,1);
return -1;
}

static void __exit hello_exit(void){

	device_destroy(sh_class,dev);
	class_destroy(sh_class);
	cdev_del(&sh_cdev);
	unregister_chrdev_region(dev,1);
	pr_info("The exit function is called\n");
	
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("shiva");
MODULE_DESCRIPTION("Simple");
MODULE_VERSION("1.1");

