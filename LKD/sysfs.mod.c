#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x2f552587, "module_layout" },
	{ 0x4800a4da, "cdev_del" },
	{ 0xf9411386, "device_destroy" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x4659ab65, "class_destroy" },
	{ 0x14507a23, "sysfs_remove_file_ns" },
	{ 0xf513389e, "kobject_put" },
	{ 0x6cda41e6, "sysfs_create_file_ns" },
	{ 0x95ce7032, "kobject_create_and_add" },
	{ 0x7b888f1e, "kernel_kobj" },
	{ 0x42f87f37, "device_create" },
	{ 0xecd84fbf, "__class_create" },
	{ 0xe9fc1dec, "cdev_add" },
	{ 0x2e4bc77e, "cdev_init" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0xb245fc58, "kmem_cache_alloc_trace" },
	{ 0xd7509006, "kmalloc_caches" },
	{ 0x37a0cba, "kfree" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0xbcab6ee6, "sscanf" },
	{ 0x92997ed8, "_printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "BB1EAAD8D59BD7C38259D66");
