#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Aryan Dhull");
MODULE_DESCRIPTION("A kernel module that prints information about a process");

static int pid ;
module_param(pid, int, 0644);

static int __init process_info_init(void)
{
    struct task_struct *task;

    task = pid_task(find_vpid(pid), PIDTYPE_PID);
    if (!task) {
        printk(KERN_ERR "Invalid PID\n");
        return -EINVAL;
    }
    printk("Process Details:\n");
    printk(KERN_INFO "Process name: %s\n", task->comm);
    printk(KERN_INFO "Process ID: %d\n", task->pid);
    printk(KERN_INFO "User ID: %ld\n", task->cred->uid.val);
    printk(KERN_INFO "Group ID: %d\n", task->group_leader->pid);

    return 0;
}

static void __exit process_info_exit(void){}

module_init(process_info_init);
module_exit(process_info_exit);

//sudo insmod Q3.ko pid=3