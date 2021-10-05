#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
const char * sigprompt[]={"INVALID","hung up","interupted","quitted","stopped by ill-formed instruction",
"trapped","aborted","exited by bus error","exited by computation error","killed",
"exited by user defined signal","exited by segmentation fault","exited by user defined signal",
"trying to access a broken pipe","exited by an alarm","terminated","exited as coprocessor experiences a stack fault",
"receiving a SIGCHLD signal","receiving a coninue signal"};
const char * signame[]={"INVALID", "SIGHUP", "SIGINT", "SIGQUIT", "SIGILL", "SIGTRAP",
 "SIGABRT", "SIGBUS", "SIGFPE", "SIGKILL", "SIGUSR1", "SIGSEGV", "SIGUSR2", "SIGPIPE",
  "SIGALRM", "SIGTERM", "SIGSTKFLT", "SIGCHLD", "SIGCONT", "SIGSTOP", "SIGTSTP", "SIGTTIN",
   "SIGTTOU", "SIGURG", "SIGXCPU", "SIGXFSZ", "SIGVTALRM", "SIGPROF", "SIGWINCH", "SIGPOLL",
    "SIGPWR", "SIGSYS", NULL};

/** prototypes **/
void sigchld_handler(int sig);

void exited(int sig, pid_t parent, pid_t child);

void signaled(int sig, pid_t parent, pid_t child);

void stopped(int sig, pid_t parent, pid_t child);

char *strsignal(int sig);

extern const char *const sys_siglist[];

pid_t wait(int *status);

pid_t waitpid(pid_t pid, int *status, int options);

pid_t fork(void);

pid_t getpid(void);

pid_t getppid(void);



int main(int argc, char** argv) {
    int status,cnt=0,flag=0; pid_t pid;
    if(argc<2) {
        printf("Syntax Error. Usage: %s EXECUABLE1 [EXECUANLE2] [...] \n",argv[0]);
        return -1;
    }
    pid_t* pids = (pid_t*)(mmap(NULL, argc*sizeof(pid_t), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0));
    int* statuses = (int*)(mmap(NULL, argc*sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0));
    char* path = argv[1];
    //signal(SIGCHLD,sigchld_handler);
    //printf("I'm the root process, my pid = %d\n",(int)(getpid()));
    while(cnt<argc-1) {
        pids[cnt] = getpid();
        //printf("%d\n",cnt);
        if((pid = fork())>0) break;
        if(pid<0) {
            perror("FORK ERROR");
            return -1;
        }
        cnt++;
    }
    //printf("%d\n",cnt);
    if(cnt==argc-1) {
        pids[cnt] = getpid();
        char *argv_new[] = {argv[cnt], 0};
        char *envp_new[] = {0};
        execve(argv_new[0],&argv_new[0],&envp_new[0]);
        printf("\n====================== ERROR ======================\n");
        printf("\nCorrupted execuable [%s], abort myself (pid=%d)\n",argv[cnt],(int)(getpid()));
        printf("\n===================================================\n\n");
        abort();
    }
    waitpid(pid,&status,WUNTRACED);
    statuses[cnt] = status;
    if(cnt!=0) {
        char *argv_new[] = {argv[cnt], 0};
        char *envp_new[] = {0};
        execve(argv_new[0],&argv_new[0],&envp_new[0]);
        printf("\n====================== ERROR ======================\n");
        printf("\nCorrupted execuable [%s], abort myself (pid=%d)\n",argv[cnt],(int)(getpid()));
        printf("\n===================================================\n\n");
        abort();
    }
    int i;
    printf("\nProcess tree: ");
    for(i=0;i<argc-1;i++) printf("%d->",(int)(pids[i]));
    printf("%d\n",(int)(pids[i]));
    for(i=argc-2;i>=0;i--) {
        if(WIFSIGNALED(statuses[i])) signaled(WTERMSIG(statuses[i]),pids[i],pids[i+1]);
        else if(WIFSTOPPED(statuses[i])) stopped(WSTOPSIG(statuses[i]),pids[i],pids[i+1]);
        else if(WIFEXITED(statuses[i]))  exited(WEXITSTATUS(statuses[i]),pids[i],pids[i+1]);
    }
    printf("\n%s process (%d) executes normally\n",argv[0],(int)(getpid()));
}



void sigchld_handler(int sig) {
    printf("Parent process receiving %s SIGNAL\n","SIGCHLD");
}

void exited(int code, pid_t parent, pid_t child) {
    printf("Child process %d of parent process %d terminated normally with exit code %d\n",
    (int)child,(int)parent,code);
}
void signaled(int sig, pid_t parent, pid_t child){
    printf("Child process %d of parent process %d is terminated by signal %d (%s)\n",
    (int)(child),(int)(parent),sig,strsignal(sig));
}
void stopped(int sig, pid_t parent, pid_t child){
    printf("Child process %d of parent process %d is terminated by signal %d (%s)\n",
    (int)(child),(int)(parent),sig,strsignal(sig));
}