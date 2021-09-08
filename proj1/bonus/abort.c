#include "project.h"
void exited(int code, pid_t parent, pid_t child) {
    printf("The child process(pid=%d) of parent process (pid=%d) has normal execution\n",
    (int)(child),(int)(parent));
    printf("Its exit status = %d\n\n",code);
}
void signaled(int sig, pid_t parent, pid_t child){
    printf("The child process(pid=%d) of parent process (pid=%d) is terminated by signal\n",
    (int)(child),(int)(parent));
    printf("Its signal number = %d\n",sig);
    printf("child process get %s signal\n\n",strsignal(sig));
}
void stopped(int sig, pid_t parent, pid_t child){
    printf("The child process(pid=%d) of parent process (pid=%d) is stopped by signal\n",
    (int)(child),(int)(parent));
    printf("Its signal number = %d\n",sig);
    printf("child process get %s signal\n\n",strsignal(sig));
}