#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/interrupt.h>

MODULE_LICENSE("GPL");
static int irq = 1,dev = 0xaa, count =0;

static irqreturn_t keyboard_handler(int irq,void *dev){
    //pr_info("%s: In init\n",__func__);
    pr_info("Keyboard counter %d\n",count++);
    //return request_irq(irq,keyboard_handler,IRQF_SHARED,"my_handler_of_keyboard");
    return IRQ_NONE;
}


static int test_init(void){
   
    pr_info("%s: In init\n",__func__);
    //pr_info("Keyboard counter %d\n",count)
    return request_irq(irq,keyboard_handler,IRQF_SHARED,"my_keyboard_handler",&dev);
}


static void test_exit(void){
    pr_info("%s:In exit\n",__func__);
    synchronize_irq(irq);
    free_irq(irq,&dev);

}


module_init(test_init);
module_exit(test_exit);
