#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/moduleparam.h>
#include<linux/unistd.h>
#include<linux/types.h>
#include<linux/sched.h>
#include<linux/signal.h>
#include<linux/highmem.h>
#include<linux/kallsyms.h>
#include<linux/syscalls.h>
#include<linux/proc_fs.h>
#include<asm/syscall_wrapper.h>
#include<asm/uaccess.h>

#define __NR_fork 57
#define __NR_ftrace 336

void** syscall_table;
void* original_ftrace;
asmlinkage long (*original_fork)();

asmlinkage long ftrace_fork(){
	return original_fork();
}
asmlinkage long process_trace(int pid){
    char state[30];
	struct list_head* s_list;
	struct list_head* c_list;
	struct task_struct *task;
	struct task_struct *sibling;
	for_each_process(task){
		if (task->pid == pid)
			break;
	}
	switch(task->state){
		case 0:
			state = "Running or ready";
			break;
		case 1:
			state = "Wait";
			break;
		case 2:
			state = "Wait with ignoring all signals";
			break;
		case 4:
			state = "Stopped";
			break;
		case 128:
			state = "Dead";
			break;
		default:
			if (task->exit_state == 32)
				state = "Zombie process";
			else state = "etc";
			break;
	}

	printk(KERN_INFO,"##### TASK INFORMATION of '[%d] %s'", pid, task->comm);
	printk(KERN_INFO," - task state : %s", state);
	printk(KERN_INFO," - Process Group leader : [%d] %s ", task->group_leader->pid, task->group_leader->comm);
	printk(KERN_INFO," - Number of context switches : %ld", task->nvcsw + task->nivcsw);
	printk(KERN_INFO," - Number of calling fork() : %d", task->fork_num);
	printk(KERN_INFO," - it's parent process : [%d] %s", task->real_parent->pid, task->real_parent->comm);
	printk(KERN_INFO," - it's sibling process(es) :");
	list_for_each_entry(sibling, &task->sibling, s_list){
		printk(KERN_INFO,"\t > [%d] %s", sibling->pid, sibling->comm);
	}
	printk(KERN_INFO,"\t > This process has %d child process(es)", sibling->pid, sibling->comm);
}

void make_rw(void* addr){
	unsigned int level;
	pte_t* pte = lookup_address((u64)addr, &level);
	
	if(pte->pte &~ _PAGE_RW)
		pte->pte |= _PAGE_RW;
}

void make_ro(void* addr){
	unsigned int level;
	pte_t* pte = lookup_address((u64)addr, &level);
	
	pte->pte = pte->pte &~ _PAGE_RW; 
}

static int __init hooking_init(void){
	syscall_table = (void**) kallsyms_lookup_name("sys_call_table");	
	make_rw(syscall_table);
	//save original table index of each syscall
	original_open = syscall_table[__NR_fork];
    original_ftrace = syscall_table[__NR_ftrace];
	//allocate new function to syscall table
	syscall_table[__NR_fork] = ftrace_fork;
    syscall_table[__NR_ftrace] = process_trace;
	
	return 0;
}

static void __exit hooking_exit(void){
	syscall_table[__NR_fork] = original_fork;
    syscall_table[__NR_ftrace] = original_ftrace;
	make_ro(syscall_table);
}

module_init(hooking_init);
module_exit(hooking_exit);
MODULE_LICENSE("GPL");