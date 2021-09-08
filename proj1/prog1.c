#include "project.h"

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
        PCHILDSTART;
        execve(path,NULL,NULL);
        printf("\n====================== ERROR ======================\n");
        printf("\nCorrupted execuable [%s], abort myself (pid=%d)\n",path,(int)(getpid()));
        printf("\n===================================================\n\n");
        abort();
    }
    waitpid(pid,&status,WUNTRACED);
    if(WIFSIGNALED(status)) {SIGNALLED(WTERMSIG(status));}
    if(WIFSTOPPED(status)) {STOPPED(WSTOPSIG(status));}
    if(WIFEXITED(status))  {EXITED(WEXITSTATUS(status));}
}



void sigchld_handler(int sig) {
    printf("Parent process receiving %s SIGNAL\n","SIGCHLD");
}