#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
/** prototypes **/
void sigchld_handler(int sig);
void exited(int sig, pid_t parent, pid_t child);
void signaled(int sig, pid_t parent, pid_t child);
void stopped(int sig, pid_t parent, pid_t child);
char *strsignal(int sig);
const char *sigdescr_np(int sig);
const char *sigabbrev_np(int sig);
extern const char *const sys_siglist[];
pid_t wait(int *status);
pid_t waitpid(pid_t pid, int *status, int options);
pid_t fork(void);
pid_t getpid(void);
pid_t getppid(void);

