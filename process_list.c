#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h> //needed for miscellaneous device
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/sched/signal.h>
#include <linux/fcntl.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
//#include <linux/string.h>

MODULE_LICENSE("Dual BSD/GPL");

static int my_open(struct inode * inode_ptr, struct file * file_ptr){

	printk(KERN_ALERT "mymodule: The file/device has been opened\n");
	return 0;
}
static int my_close(struct inode * inode_ptr, struct file * file_ptr){

	printk(KERN_ALERT "mymodule: The file/device has been closed\n");
	return 0;
}
static ssize_t my_read(struct file * my_file, char __user * buff, size_t cnt, loff_t * loff){
//	printk(KERN_ALERT "mymodule: The 'read' function has been called\n");
	struct task_struct * task_list;
	struct k_info{
		int ch_pid;
		int parent_pid;
		int cpu;
		int state;
	};
	struct k_info temp_struct; //holder for size of k_info struct of size 1
	struct k_info * data_list = kmalloc(sizeof(temp_struct)*1000, GFP_KERNEL);
	int index = 0; //counter for the for_each_process loop to index the analagous area  in data_list
//	char * new_buff = kmalloc(sizeof(char*)*6400, GFP_KERNEL);
	
	for_each_process(task_list){

		data_list[index].ch_pid = task_list->pid;
		data_list[index].parent_pid = task_list->parent->pid;
		data_list[index].cpu = task_list->cpu;
		data_list[index].state = task_list->state;
//		printk(KERN_ALERT "mymodule: next data_list entry is now -- %d, %d, %d, %d", data_list[index].ch_pid, data_list[index].parent_pid, data_list[index].cpu, data_list[index].state);

		index++;
		if(index == 999){
			break; //breaks if the number of running processes is higher than allocated capacity
		}
	}
	data_list[index].ch_pid = -99999;
	data_list[index].parent_pid = -99999;
	data_list[index].cpu= -99999;
	data_list[index].state = -99999;
	copy_to_user(buff, data_list, sizeof(temp_struct)*1000);
	kfree(data_list);
	return 0;

}

static const struct file_operations my_fops = {

	.owner = THIS_MODULE,
	.open  = my_open,
	.release = my_close,
	.read = my_read,
};


struct miscdevice mcd = {
	.minor = MISC_DYNAMIC_MINOR,
	.name  = "process_list",
	.fops = &my_fops,

};

//called when module is installed
int __init init_module(void)
{
	printk(KERN_ALERT "mymodule: Init success--This kernel module will read a character device\n");
	printk(KERN_ALERT "mymodule: All devices are treated as files in Linux. The misc_device is a 'file'\n");
	misc_register(&mcd);

	return 0;
}



// called when module is removed
void __exit cleanup_module(void)
{
	printk(KERN_ALERT "mymodule: This is the exit function for cleanup--module removed.\n");
	misc_deregister(&mcd); 
}

//module_init(init_module);
//module_init(cleanup_module);

