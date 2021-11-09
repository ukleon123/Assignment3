#include<linux/syscalls.h>
#include<linux/kernel.h>
#include<linux/likage.h>

SYSCALL_DEFINE1(ftrace, int, pid){
    return 0;
}
