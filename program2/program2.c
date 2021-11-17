#include <linux/module.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/kthread.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/printk.h>
#include <linux/jiffies.h>
#include <linux/kmod.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
#define DFORK(stack_start) _do_fork(SIGCHLD, (unsigned long)stack_start, 0, NULL, NULL, 0)

#define kernel_pid current->pid

#define __WEXITSTATUS(status) (((status) & 0xff00) >> 8)

/* If WIFSIGNALED(STATUS), the terminating signal.  */
#define __WTERMSIG(status) ((status) & 0x7f)

/* If WIFSTOPPED(STATUS), the signal that stopped the child.  */
#define __WSTOPSIG(status) __WEXITSTATUS(status)

/* Nonzero if STATUS indicates normal termination.  */
#define __WIFEXITED(status) (__WTERMSIG(status) == 0)

/* Nonzero if STATUS indicates termination by a signal.  */
#define __WIFSIGNALED(status) \
  (((signed char) (((status) & 0x7f) + 1) >> 1) > 0)

/* Nonzero if STATUS indicates the child is stopped.  */
#define __WIFSTOPPED(status) (((status) & 0xff) == 0x7f)

/* Nonzero if STATUS indicates the child continued after a stop.  We only
   define this if <bits/waitflags.h> provides the WCONTINUED flag bit.  */

	const char * sigprompt[]={"INVALID","hung up","interupted","quitted","stopped by ill-formed instruction",
"trapped","aborted","exited by bus error","exited by computation error","killed",
"exited by user defined signal","exited by segmentation fault","exited by user defined signal",
"trying to access a broken pipe","exited by an alarm","terminated","exited as coprocessor experiences a stack fault",
"receiving a SIGCHLD signal","receiving a coninue signal"};
const char * const signame[]={"INVALID", "SIGHUP", "SIGINT", "SIGQUIT", "SIGILL",
	 "SIGTRAP", "SIGABRT", "SIGBUS", "SIGFPE", "SIGKILL", "SIGUSR1", "SIGSEGV",
	  "SIGUSR2", "SIGPIPE", "SIGALRM", "SIGTERM", "SIGSTKFLT", "SIGCHLD", "SIGCONT",
	   "SIGSTOP", "SIGTSTP", "SIGTTIN", "SIGTTOU", "SIGURG", "SIGXCPU", "SIGXFSZ",
	    "SIGVTALRM", "SIGPROF", "SIGWINCH", "SIGPOLL", "SIGPWR", "SIGSYS", NULL};

struct wait_opts {
	enum pid_type wo_type;
	int	wo_flags;
	struct pid	*wo_pid;
	struct siginfo __user *wo_info;
	int __user *wo_stat;
	struct rusage __user *wo_rusage;
	wait_queue_t child_wait;
	int	notask_error;
};

extern long do_wait(struct wait_opts *wo);
extern struct filename *getname(const char __user *);

void sigchld_handler(int sig) {
    printk("[program2] : Parent process receiving %s SIGNAL\n",signame[sig]);
}

int my_exec(void){
	const char* path = "/opt/test";
	struct filename *path_file;
	const char __user *const __user argv[] = {path,NULL};
	const char __user *const __user envp[] = {"HOME=/","PATH=/sbin:/usr/sbin:/bin:/usr/bin",NULL};
	printk("[program2] : child process\n");
	path_file = getname(path);
	return do_execve(path_file,argv,envp);
}

//implement fork function
void my_wait(pid_t pid,int* status) {
	struct wait_opts wo;
	struct pid* pid_objptr;
	enum pid_type type = PIDTYPE_PID;
	pid_objptr = find_get_pid(pid);
	wo.wo_type = type;
	wo.wo_pid = pid_objptr;
	wo.wo_flags = WEXITED|WUNTRACED;
	wo.wo_info = NULL;
	wo.wo_stat = (int __user *)status;
	wo.wo_rusage = NULL;
	do_wait(&wo);
	//you are free to prink blablabla here tho I didn't
	put_pid(pid_objptr);
	return;
}
int my_fork(void *argc){
	//set default sigaction for current process
	int i,status,sig; long pid;
	struct k_sigaction *k_action = &current->sighand->action[0];
	//struct k_sigaction *chld_action;
	printk("[program2] : module_init kthread start\n");
	for(i=0;i<_NSIG;i++){
		if(i==SIGCHLD) k_action->sa.sa_handler = &sigchld_handler;
		else k_action->sa.sa_handler = SIG_DFL;
		k_action->sa.sa_flags = 0;
		k_action->sa.sa_restorer = NULL;
		sigemptyset(&k_action->sa.sa_mask);
		k_action++;
	}
	// chld_action = (struct k_sigaction *)kmalloc(sizeof(struct k_sigaction),GFP_KERNEL);
	// chld_action->sa.sa_handler = &sigchld_handler;
	// chld_action->sa.sa_flags = 0;
	// chld_action->sa.sa_restorer = NULL;
	// sigemptyset(&chld_action->sa.sa_mask);
	// do_sigaction(SIGCHLD,chld_action,NULL);
	if((pid=DFORK(&my_exec))){ 
		printk("[program2] : The child process has pid = %d\n",(int)pid);
		printk("[program2] : This is the parent process, pid = %d\n",(int)kernel_pid);
	}
	else {
		/* execute a test program in child process */
		my_exec();
	}
	/* wait until child process terminates */
	my_wait((pid_t)pid,&status);
	if(__WIFSIGNALED(status)) {
		sig = __WTERMSIG(status);
		printk("[program2] : get %s signal\n",signame[sig]);
		if(sig<=18) printk("[program2] : child process is %s.\n",sigprompt[sig]);
		else printk("[program2] : the return signal is %d\n",sig);
		printk("[program2] : the return signal is %d\n",sig);
		return -1;
	}

    if(__WIFSTOPPED(status)) {
		sig = __WSTOPSIG(status);
		printk("[program2] : child process get %s signal\n",signame[sig]);
		if(sig<=18) printk("[program2] : child process is %s.\n",sigprompt[sig]);
		else printk("[program2] : the return signal is %d\n",sig);
		return -1;
	}

    if(__WIFEXITED(status)) {
		printk("[program2] : Normal termination with EXIT STATUS = %d\n",__WEXITSTATUS(status));
	}
	return 0;
}

static int __init program2_init(void){
	static struct task_struct *etx_thread; 
	printk("[program2] : module_init\n");
	etx_thread = kthread_create(my_fork,NULL,"prog2 Thread"); 
	if(etx_thread) 
	{
		printk("[program2] : module_init create kthread start\n");
		wake_up_process(etx_thread); 
	} 
	else 
	{
	printk(KERN_ERR "[program2] : Cannot create kthread\n"); 
	}
	
	return 0;
}

static void __exit program2_exit(void){
	printk("[program2] : Module_exit\n");
}

module_init(program2_init);
module_exit(program2_exit);
