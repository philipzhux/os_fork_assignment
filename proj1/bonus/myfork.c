#include "project.h"
int main(int argc, char** argv) {
    int status,cnt=0,flag=0; pid_t pid;
    if(argc<2) {
        printf("Syntax Error. Usage: %s EXECUABLE1 [EXECUANLE2] [...] \n",argv[0]);
        return -1;
    }
    pid_t* pids = (pid_t*)(malloc(argc*sizeof(pid_t)));
    char* path = argv[1];
    //signal(SIGCHLD,sigchld_handler);
    printf("I'm the parent process, my pid = %d\n",(int)(getpid()));
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
        printf("the process tree: ");
        size_t i;
        for(i=0;i<argc-1;i++) printf("%d->",(int)(pids[i]));
        printf("%d\n",(int)(pids[i]));
        execve(argv[cnt],NULL,NULL);
        printf("\n====================== ERROR ======================\n");
        printf("\nCorrupted execuable [%s], abort myself (pid=%d)\n",argv[cnt],(int)(getpid()));
        printf("\n===================================================\n\n");
        abort();
    }
    waitpid(pid,&status,WUNTRACED);
    if(WIFSIGNALED(status)) signaled(WTERMSIG(status),getpid(),pid);
    if(WIFSTOPPED(status)) stopped(WSTOPSIG(status),getpid(),pid);
    if(WIFEXITED(status))  exited(WEXITSTATUS(status),getpid(),pid);
    if(cnt!=0) {
        execve(argv[cnt],NULL,NULL);
        printf("\n====================== ERROR ======================\n");
        printf("\nCorrupted execuable [%s], abort myself (pid=%d)\n",argv[cnt],(int)(getpid()));
        printf("\n===================================================\n\n");
        abort();
    }
    printf("\n%s process (pid=%d) executes normally\n",argv[0],(int)(getpid()));
}



void sigchld_handler(int sig) {
    printf("Parent process receiving %s SIGNAL\n","SIGCHLD");
}
