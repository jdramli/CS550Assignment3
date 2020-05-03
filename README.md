# CS550Assignment3

Part A:  Completed using kprobe example at: https://elixir.bootlin.com/linux/latest/source/samples/kprobes
This example was modified to only use the "handler_pre" portion of the kprobe trap.  
The structure of the kprobe that receives data from handle_mm_fault can be found at: https://elixir.bootlin.com/linux/latest/source/arch/x86/include/asm/ptrace.h#L56
Each parameter of handle_mm_fault is passed into the unsigned long registers of the kprobe, starting with arg1=di, arg2=si, and arg3=dx, etc.
If the current->pid matches the process ID passed on the "sudo insmod kprobe.ko arg1=" call, then the regs->si will print, denoting the address of the page-fault
The parameter for pid is just named "arg1" for the process ID to track via kprobe.

Part B: Using the piped command "dmesg | tail -x" where x is the number of entries to display, a log of the address with microsecond time-stamps were recorded.
The standard output was redirected for multiple commands to generate three process reports.  Those text files are:

sample2300fromprocess491.txt -  PID 491 referring to:  /lib/systemd/systemd-journald
sample2000fromprocess1213.txt - PID 1213 referring to: /usr/bin/perl /usr/sbin/x2gocleansessions 
sample165sysbenchProc31850.txt - PID 31850 referring to: A run of command ---  "sysbench --test=CPU --cpu-max-prime=200000sysbench --test=cpu --cpu-max-prime=20000 run &"

Each text file was used as a data source for an Excel Spreadsheet to obtain a scatter plot.  It can be seen from the scatter plots that there are definitely periods of extreme density
of points.  This density signifies repeated reports of the same or nearby address locations of the page-faults.   



