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

