#include    "booga.h"

static boogaPtr booga_device;
static struct proc_dir_entry *booga_proc_file;
static int booga_major = BOOGA_MAJOR;
static int booga_nr_devs = BOOGA_NR_DEVS;
module_param(booga_major, int, 0);
module_param(booga_nr_devs, int, 0);
MODULE_AUTHOR("Lam Nguyen"); 
MODULE_LICENSE("GPL v2");


static int booga_proc_open(struct inode *inode, struct file *file);


static struct file_operations booga_fops = {
    .read = booga_read,
    .write = booga_write, 
    .unlocked_ioctl = NULL,
    .open = booga_open,
    .release = booga_release,
};


static struct file_operations booga_proc_fops = {
    .owner = THIS_MODULE,
    .open = booga_proc_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release, 
};


static int booga_proc_show(struct seq_file *m, void *v) {
    seq_printf(m, "bytes read = %ld\n", booga_device -> num_read);
    seq_printf(m, "bytes written = %ld\n", booga_device -> num_write);
    seq_printf(m, "number of opens:\n");
    seq_printf(m, "  /dev/booga0 = %ld times\n", booga_device -> booga0_opens);
    seq_printf(m, "  /dev/booga1 = %ld times\n", booga_device -> booga1_opens);
    seq_printf(m, "  /dev/booga2 = %ld times\n", booga_device -> booga2_opens);
    seq_printf(m, "  /dev/booga3 = %ld times\n", booga_device -> booga3_opens);
    seq_printf(m, "strings output:\n");
    seq_printf(m, "  booga! booga!   = %ld times\n", booga_device -> string0_output);
    seq_printf(m, "  googoo! gaga!   = %ld times\n", booga_device -> string1_output);
    seq_printf(m, "  wooga! wooga!   = %ld times\n", booga_device -> string2_output);
    seq_printf(m, "  neka! maka!     = %ld times\n", booga_device -> string3_output);

    return 0;
}


static int booga_proc_open(struct inode *inode, struct file *file) {
    return single_open(file, booga_proc_show, NULL);
}


static int booga_open (struct inode *inode, struct file *filp) {
    unsigned int number = NUM(inode -> i_rdev);

    if(number >= booga_nr_devs) {
        return -ENODEV;
    }
    printk("<1> booga_open device opened\n");
    filp -> f_op = &booga_fops;
    if(down_interruptible(&booga_device -> sem)) {
        return (-ERESTARTSYS);
    }
    booga_device -> num = number;
    if(number == 0) {
        (booga_device -> booga0_opens)++;
    }
    if(number == 1) {
        (booga_device -> booga1_opens)++;
    }
    if(number == 2) {
        (booga_device -> booga2_opens)++;
    }
    if(number == 3) {
        (booga_device -> booga3_opens)++;
    }
    up(&booga_device -> sem);
    try_module_get(THIS_MODULE);
    return 0;
}


static int booga_release (struct inode *inode, struct file *filp) {
    if(down_interruptible(&booga_device -> sem)) {
        return (-ERESTARTSYS);
    }
    booga_device -> num_close++; 
	up(&booga_device -> sem);
    module_put(THIS_MODULE);
    return 0;
}


static ssize_t booga_read (struct file *filp, char *buf, size_t count, loff_t *f_pos) {
    unsigned int choice;
    unsigned int outputIndex = 0;
    unsigned int stringIndex = 0;

    char *option;
    char *outputString = (char *) kmalloc(sizeof(char) *count, GFP_KERNEL);

    char randval;
    get_random_bytes(&randval, 1);
    choice = (randval & 0x7F) % 4;

    printk("<1> booga_read invoked at: %d\n", choice);

    if(down_interruptible(&booga_device -> sem)) {
        return -ERESTARTSYS;
    }
    if(choice == 0) {
        option = "booga! booga! ";
        (booga_device -> string0_output)++;
    }
    if(choice == 1) {
        option = "googoo! gaga! ";
        (booga_device -> string1_output)++;
    }
    if(choice == 2) {
        option = "wooga! wooga! ";
        (booga_device -> string2_output)++;
    }
    if(choice == 3) {
        option = "neka! maka! ";
        (booga_device -> string3_output)++;
    }
    booga_device -> num_read += count;
    up(&booga_device -> sem);
    while(outputIndex < count) {
        outputString[outputIndex++] = option[stringIndex++];
        if(option[stringIndex] == '\0') {
            stringIndex = 0;
        }
    }
    if(__copy_to_user(buf, outputString, count)) {
        buf = NULL;
        return 0;
    }
    return count;
}


static ssize_t booga_write (struct file *filp, const char *buf, size_t count, loff_t *f_pos) {
    char *sWrite = (char *) kmalloc(sizeof(char) * count, GFP_KERNEL);
    unsigned int numWrite = count;
    int minor = iminor(filp -> f_path.dentry -> d_inode);
    
    printk("<1> booga_write invoked.\n");

    numWrite -= __copy_from_user(sWrite, buf, count);

    if(down_interruptible(&booga_device -> sem)) {
        return (-ERESTARTSYS);
    }

    if(minor == 3) {
	send_sig(SIGTERM, current, 0);
	numWrite =0;
    }

    booga_device -> num_write += numWrite;
    up(&booga_device -> sem);
    
    return numWrite;
}


static void init_booga_device_stats(void) {
    booga_device -> num_read = 0;
    booga_device -> num_write = 0;
    booga_device -> booga0_opens = 0;
    booga_device -> booga1_opens = 0;
    booga_device -> booga2_opens = 0;
    booga_device -> booga3_opens = 0;
    booga_device -> string0_output = 0;
    booga_device -> string1_output = 0;
    booga_device -> string2_output = 0;
    booga_device -> string3_output = 0;
    sema_init(&booga_device -> sem, 1);
}


static int __init booga_init(void) {
    int result;
    result = register_chrdev(booga_major, "booga", &booga_fops);

    if(result < 0) {
        printk(KERN_WARNING "booga: can't get major %d\n", booga_major);
        return result;
    }
    if(booga_major == 0) {
        booga_major = result;
    }
    printk("<1> booga device driver version 3: loaded at major number %d\n", booga_major);
    booga_device = (boogaPtr) kmalloc(sizeof(booga), GFP_KERNEL);
    if(!booga_device) {
        result = -ENOMEM;
        unregister_chrdev(booga_major, "booga");
        return result;
    }
    init_booga_device_stats();
    booga_proc_file = proc_create("driver/booga", 0, NULL, &booga_proc_fops);
    if(!booga_proc_file) {
        result = -ENOMEM;
        unregister_chrdev(booga_major, "booga");
        return result;
    }

    return 0;
}


static void __exit booga_exit(void) {
    remove_proc_entry("driver/booga", NULL/* parent dir*/);
    kfree(booga_device);
    unregister_chrdev(booga_major, "booga");
    printk("<1> booga device driver version 4: unloaded\n");
}

module_init(booga_init);
module_exit(booga_exit);