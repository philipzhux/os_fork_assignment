#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
/** prototypes **/

// int sigemptyset(sigset_t *set);

// int sigfillset(sigset_t *set);

// int sigaddset(sigset_t *set, int signum);

// int sigdelset(sigset_t *set, int signum);

// int sigismember(const sigset_t *set, int signum);

typedef void (*sighandler_t)(int);

sighandler_t signal(int signum, sighandler_t handler);

char *strsignal(int sig);

const char *sigdescr_np(int sig);

const char *sigabbrev_np(int sig);

extern const char *const sys_siglist[];

pid_t wait(int *status);
pid_t waitpid(pid_t pid, int *status, int options);

pid_t fork(void);
pid_t getpid(void);

pid_t getppid(void);


void sigchld_handler(int sig);

void exited(int sig, pid_t parent, pid_t child);
void signaled(int sig, pid_t parent, pid_t child);
void stopped(int sig, pid_t parent, pid_t child);

/** utility macros **/

#define PCHILDSTART printf("Child process start to execute the program\n")



#define SIGNALLED(sig)\
    printf("child process get %s signal\n",strsignal(sig));\
    printf("child process is abort by abort signal\n");\
    printf("CHILD EXECUTION FAILED!!\n");\
    return -1

#define STOPPED(sig)\
    printf("child process get %s signal\n",strsignal(sig));\
    printf("child process stopped\n");\
    printf("CHILD PROCESS STOPPED\n");\
    return -1

#define EXITED(res) printf("Normal termination with EXIT STATUS = %d\n",res);\
    return -1

