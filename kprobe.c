//sysbench --test=CPU --cpu-max-prime=200000sysbench --test=cpu --cpu-max-prime=20000 run &
// SPDX-License-Identifier: GPL-2.0-only
/*
 * NOTE: This example is works on x86 and powerpc.
 * Here's a sample kernel module showing the use of kprobes to dump a
 * stack trace and selected registers when _do_fork() is called.
 *
 * For more information on theory of operation of kprobes, see
 * Documentation/kprobes.txt
 *
 * You will see the trace data in /var/log/messages and on the console
 * whenever _do_fork() is invoked to create a new process.
 */

#include <linux/kernel.h> 
#include <linux/module.h>
#include <linux/kprobes.h> //needed for kprobes

#include <linux/moduleparam.h> //needed for module arguments
#include <linux/init.h> //needed for module arguments
#include <linux/stat.h> //needed for module arguments

#define MAX_SYMBOL_LEN	64
//static char symbol[MAX_SYMBOL_LEN] = "_do_fork";
static char symbol[MAX_SYMBOL_LEN] = "handle_mm_fault"; //This is where the handle_mm_fault function is specified to be trapped instead of "_do_fork"
//static char symbol[MAX_SYMBOL_LEN] = "invalid_fake_xyzmakeup_instructiondwe";
module_param_string(symbol, symbol, sizeof(symbol), 0644);
//handle error for argument "symbol" greater than 64
//
static int arg1 = 0; // creating a default argument holder as 'static' for module interaction
//static int process = 0; // creating a parameter to hold the process number
module_param(arg1, int, 0);


/* For each probe you need to allocate a kprobe structure */
static struct kprobe kp = {
	.symbol_name	= symbol,
};

/* kprobe pre_handler: called just before the probed instruction is executed */

static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
/*
*/
	// A dump_stack() here will give a stack backtrace

/*	if(current->pid > 5 && current->pid < 10000 ){
		if(current->pid != 487){
			printk(KERN_ALERT "Pre-Handler has been called for current->pid of: %d", current->pid);	
		}
		
	}
*/
	if(current->pid == arg1 ){
//		printk(KERN_ALERT "This is the handler for handle_mm_fault() calls on process: %d", arg1);
//		printk(KERN_ALERT "The page fault is on the page of long address: %lu", regs->si);
//		printk(KERN_ALERT "Page fault logged for process %d at long address: %lu",arg1, regs->si);//Also prints to Kernel Alert
//		printk("Page fault logged for process %d at long address: %lu",arg1, regs->si); //Only prints to log

//		printk("%lu from process %d", regs->si, arg1); //Only prints to log
//		printk("%lu from process %d", regs->si, arg1); //Only prints to log
		printk(KERN_ALERT "%lu from process %d", regs->si, arg1); //Prints KERN_ALERT and prints to log
	}


	return 0;
}


static int __init kprobe_init(void)
{
	int ret;
//	struct task_struct * task_list;
//	int index = 0; //counter for the for_each_process loop to index the analagous area  in data_list


	kp.pre_handler = handler_pre;
//	kp.post_handler = handler_post;
//	kp.fault_handler = handler_fault;
	printk(KERN_ALERT "Searching for handle_mm_faults of process designated by arg1, which is: %d", arg1);

//	char * new_buff = kmalloc(sizeof(char*)*6400, GFP_KERNEL);
/*	
	for_each_process(task_list){
//		if(task_list[index].pid < arg1){
//		printk(KERN_ALERT "current->pid is: %d", current->pid); 
		if(task_list[index].state == 0){ //if state ==0 then it is TASK_RUNNING
			//kp.pre_handler = handler_pre;
			//kp.post_handler = handler_post;
			printk(KERN_ALERT "Conditional handler can be invoked here");

		}
		index++;

	}
*/
//	printk(KERN_ALERT "mymodule: Process count is: -- %d", index);
//	printk(KERN_ALERT "mymodule: The value for kp.fault_handler is: ");	
	ret = register_kprobe(&kp);
	if (ret < 0) {
		pr_err("register_kprobe failed, returned %d\n", ret);
		return ret;
	}
	pr_info("Planted kprobe at %p\n", kp.addr);

	return 0;
}

static void __exit kprobe_exit(void)
{
	unregister_kprobe(&kp);
	pr_info("kprobe at %p unregistered\n", kp.addr);
}

module_init(kprobe_init)
module_exit(kprobe_exit)
MODULE_LICENSE("GPL");
