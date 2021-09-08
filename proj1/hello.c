#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
int main() {
    printf("------------CHILD PROCESS START------------\n");
    printf("THIS IS A NORMAL FUNCTION RETURNING 3!\n");
    printf("------------CHILD PROCESS START------------\n");
    return 3;
}
