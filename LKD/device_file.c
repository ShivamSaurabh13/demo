#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kdev_t.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/device.h>

dev_t dev = 0;
struct class *dev_class;

int __init hello_world_init(void){
	
	dev = alloc_chrdev_region(&dev,0,1,"Shivam_major");
	if(dev < 0){
		pr_err("allocation major");
		return -1;
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
	unregister_chrdev_region(dev,1);
	pr_info("Kernel Module removed successfully");

}

module_init(hello_world_init);
module_exit(hello_world_exit);




MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shivam Saurabh");
MODULE_DESCRIPTION("CREATING THE DEVICE FILE");









