#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

extern const char *const sys_siglist[];
void sigchld_handler(int sig);
void exited(int sig, pid_t parent, pid_t child);
void signaled(int sig, pid_t parent, pid_t child);
void stopped(int sig, pid_t parent, pid_t child);

const char * const sigprompt[]={"INVALID","hung up","interupted","quitted","stopped by ill-formed instruction",
"trapped","aborted","exited by bus error","exited by computation error","killed",
"exited by user defined signal","exited by segmentation fault","exited by user defined signal",
"trying to access a broken pipe","exited by an alarm","terminated","exited as coprocessor experiences a stack fault",
"receiving a SIGCHLD signal","receiving a coninue signal"};
const char * const signame[]={"INVALID", "SIGHUP", "SIGINT", "SIGQUIT", "SIGILL", "SIGTRAP",
 "SIGABRT", "SIGBUS", "SIGFPE", "SIGKILL", "SIGUSR1", "SIGSEGV", "SIGUSR2", "SIGPIPE",
  "SIGALRM", "SIGTERM", "SIGSTKFLT", "SIGCHLD", "SIGCONT", "SIGSTOP", "SIGTSTP", "SIGTTIN",
   "SIGTTOU", "SIGURG", "SIGXCPU", "SIGXFSZ", "SIGVTALRM", "SIGPROF", "SIGWINCH", "SIGPOLL",
    "SIGPWR", "SIGSYS", NULL};

int main(int argc, char** argv) {
    int status;
    pid_t pid;
    if(argc!=2) {
        printf("Syntax Error. Usage: %s EXECUABLEPATH\n",argv[0]);
        return -1;
    }
    printf("Process starts to work\n");
    char* path = argv[1];
    signal(SIGCHLD,sigchld_handler);
    printf("I'm the parent process, my pid = %d\n",(int)(getpid()));
    if((pid=fork())==0){
        printf("I'm the child process, my pid = %d\n",(int)(getpid()));
        printf("Child process start to execute the program\n");
        char *argv_new[] = {path, 0};
        char *envp_new[] = {0};
        execve(argv_new[0],&argv_new[0],&envp_new[0]);
        printf("\n====================== ERROR ======================\n");
        printf("\nCorrupted execuable [%s], abort myself (pid=%d)\n",path,(int)(getpid()));
        printf("\n===================================================\n\n");
        abort();
    }
    waitpid(pid,&status,WUNTRACED);
	int sig;
    if(WIFSIGNALED(status)) {
		sig = WTERMSIG(status);
		printf("child process get %s signal\n",signame[sig]);
		if(sig<=18) printf("child process is %s.\n",sigprompt[sig]);
		else printf("child process quits because the %s signal.\n",strsignal(sig));
		printf("CHILD EXECUTION FAILED!!\n");
		return -1;
	}

    if(WIFSTOPPED(status)) {
		sig = WSTOPSIG(status);
		printf("child process get %s signal\n",signame[sig]);\
		if(sig<=18) printf("child process is %s.\n",sigprompt[sig]);
		else printf("child process is %s.\n",strsignal(sig));
		printf("CHILD EXECUTION STOPPED!!\n");
		return -1;
	}

    if(WIFEXITED(status)) {
		printf("Normal termination with EXIT STATUS = %d\n",WEXITSTATUS(status));
	}
}



void sigchld_handler(int sig) {
    printf("Parent process receiving %s SIGNAL\n",signame[sig]);
}
