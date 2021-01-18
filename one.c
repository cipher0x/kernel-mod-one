#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h> 
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Eric Taylor");
MODULE_DESCRIPTION("A char device that returns all ones");
MODULE_VERSION("0.1");

#define DEVICE_NAME "one"
#define CLASS_NAME "one"

static struct class*  oneClass  = NULL;
static struct device* oneDevice = NULL; 
static int major;

static int dev_open(struct inode*, struct file*);
static int dev_release(struct inode*, struct file*);
static ssize_t dev_read(struct file*, char*, size_t, loff_t*);
static ssize_t dev_write(struct file*, const char*, size_t, loff_t*);

static struct file_operations fops = {
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release,
};

static int __init one_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);
    
    if (major < 0) {
        printk(KERN_ALERT "one failed to register a major number\n");
        return major;
    }
    
    printk(KERN_INFO "one module has registered a major number: %d\n", major);
    
    oneClass = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(oneClass)) {
        printk(KERN_ALERT "Failed to register device class\n");
        return PTR_ERR(oneClass);
        
    }
    printk(KERN_INFO "one: device class registered correctly\n");
   
    oneDevice = device_create(oneClass, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
    if (IS_ERR(oneDevice)){
        class_destroy(oneClass);
        unregister_chrdev(major, DEVICE_NAME);
        printk(KERN_ALERT "Failed to create the device\n");
        return PTR_ERR(oneDevice);
    }
   printk(KERN_INFO "one: device class created correctly\n");
   
    return 0;
}

static void __exit one_exit(void) {
    device_destroy(oneClass, MKDEV(major, 0));     // remove the device
    class_unregister(oneClass);                          // unregister the device class
    class_destroy(oneClass);                             // remove the device class
    unregister_chrdev(major, DEVICE_NAME);               //unregister Major number
    printk(KERN_INFO "one module has been unloaded\n");
}

static int dev_open(struct inode *inodep, struct file *filep) {
   printk(KERN_INFO "one device opened\n");
   return 0;
}

static ssize_t dev_write(struct file *filep, const char *buffer,
                         size_t len, loff_t *offset) {

   printk(KERN_INFO "Sorry, one is read only\n");
   return -EFAULT;
}

static int dev_release(struct inode *inodep, struct file *filep) {
   printk(KERN_INFO "one device closed\n");
   return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
    int errors = 0;
    char allOnes = 255;

    errors = copy_to_user(buffer, &allOnes, 1);

    return errors == 0 ? 1 : -EFAULT;
}

module_init(one_init);
module_exit(one_exit);
