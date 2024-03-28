#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/device.h>
#include <linux/cdev.h>
//#include <linux/kdev_t.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>


#define mem_size 1024

#define WR_VALUE _IOW('a','a',int32_t*)
#define RD_VALUE _IOR('a','b',int32_t*)

int32_t value = 0;

volatile int shi_value = 0;

dev_t dev=0;
static struct class *re_class;
static struct cdev re_cdev;
uint8_t *kernel_buffer;
struct kobject *kobj_ref;


static int __init main_init(void);
static void __exit main_exit(void);
static int re_open(struct inode *inode, struct file *file);
static int re_release(struct inode *inode, struct file *file);
static ssize_t re_write(struct file *filp,const char *buf,size_t len, loff_t *off);
static ssize_t re_read(struct file *filp,char *buf, size_t len, loff_t *off);
static long re_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

static ssize_t sysfs_show(struct kobject *kobj,struct kobj_attribute *attr, char *buf);
static ssize_t sysfs_store(struct kobject *kobj,struct kobj_attribute *attr,const char *buf,size_t count);

struct kobj_attribute etx_attr = __ATTR(etx_value,0660,sysfs_show,sysfs_store);

struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = re_open,
	.write = re_write,
	.read = re_read,
	.unlocked_ioctl = re_ioctl,
	.release = re_release,
};

static ssize_t sysfs_show(struct kobject *kobj,struct kobj_attribute *attr,char *buf){
	pr_info("Sysfs read\n");
	//sscanf(buf,"%d",&shi_value);
	
	return sprintf(buf,"%d",shi_value);
}

static ssize_t sysfs_store(struct kobject *kobj,struct kobj_attribute *attr, const char *buf,size_t count){
	pr_info("Sysfs write\n");
	sscanf(buf,"%d",&shi_value);
	return count;
}

static int re_open(struct inode *inode, struct file *file){
	
	if((kernel_buffer = kmalloc(mem_size,GFP_KERNEL)) == 0){
		pr_info("kmalloc");
		return -1;
	}
	pr_info("Opening the device file\n");
	return 0;
}
static ssize_t re_write(struct file *filp,const char __user *buf,size_t len, loff_t *off){
	
	
	if(copy_from_user(kernel_buffer,buf,len)){
		pr_err("copy_from_user");
	}
	printk(KERN_INFO "The data is %s\n",kernel_buffer);
	pr_info("Write called:DONE\n");
	return len;
}
static ssize_t re_read(struct file *filp,char __user *buf, size_t len, loff_t *off){
	
	if(copy_to_user(buf,kernel_buffer,mem_size)){
		pr_err("copy_to_user");
	}
	pr_info("Read called \n");
	printk(KERN_INFO "Data from user space: %s\n",kernel_buffer);
	return mem_size;
}

static int re_release(struct inode *inode, struct file *filp){
	kfree(kernel_buffer);
	pr_info("Released called\n");
	
	return 0;
}

static long re_ioctl(struct file *file, unsigned int cmd, unsigned long arg){
	switch(cmd){
		case WR_VALUE:
			if(copy_from_user(&value,(int32_t*)arg, sizeof(value))){
				pr_err("IOCTL copy_from_user\n");
				
			}
			pr_info("Value = %d\n",value);
			break;
		case RD_VALUE:
			if(copy_to_user((int32_t*)arg,&value,sizeof(value))){
				pr_err("IOCTL copy_to_user");
			}
			pr_info("Data send to user space\n");
			break;
		default: pr_info("Default\n");
			break;
	}
	return 0;
}


static int __init main_init(void){
	if((alloc_chrdev_region(&dev,0,1,"real_major"))<0){
		pr_err("alloc_chrdev_region");
		return -1;
	}
	pr_info("Major = %d Minor = %d\n",MAJOR(dev),MINOR(dev));
	
	cdev_init(&re_cdev,&fops);
	
	if((cdev_add(&re_cdev,dev,1))<0){
		pr_info("cdev_add\n");
		goto err_class;
	}
	
	if(IS_ERR(re_class = class_create(THIS_MODULE,"real_class"))){
		pr_err("class_create");
		//return -1;
		goto err_class;
	}
	
	if((IS_ERR(device_create(re_class,NULL,dev,NULL,"real_device")))){
		pr_err("device_create");
		goto err_device;
	}
	
	kobj_ref = kobject_create_and_add("shivam_sysfs",kernel_kobj);
	
	if(sysfs_create_file(kobj_ref,&etx_attr.attr)){
		pr_err("Cannot create sysfs file\n");
		goto err_sysfs;
	}
	
	pr_info("Module successfully inserted\n")
;	return 0;	
err_sysfs:	kobject_put(kobj_ref);
		sysfs_remove_file(kernel_kobj, &etx_attr.attr);
		
err_device: class_destroy(re_class);
err_class : unregister_chrdev_region(dev,1);
return -1;
}

static void __exit main_exit(void){
	kobject_put(kobj_ref);
	sysfs_remove_file(kernel_kobj,&etx_attr.attr);
	device_destroy(re_class,dev);
	class_destroy(re_class);
	cdev_del(&re_cdev);
	unregister_chrdev_region(dev,1);
	pr_info("Module removed\n");
	
}

module_init(main_init);
module_exit(main_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SHIVAM");
MODULE_DESCRIPTION("REAL CHARECTER DRIVER");


