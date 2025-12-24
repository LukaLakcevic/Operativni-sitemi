#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int fib(int n){
    if(n == 1)
        return 1;

    if(n == 2)
        return 1;

    int fd[2];
    pipe(fd);
    int pid = fork();
    if(pid == 0){
        close(fd[0]);
        int res = fib(n-1);
        write(fd[1], &res, sizeof(int));
        close(fd[1]);
        exit(0);
    }

    int fd1[2];
    pipe(fd1);
    int pid1 = fork();
    if(pid1 == 0){
        close(fd[0]);
        int res = fib(n-2);
        write(fd1[1], &res, sizeof(int));
        close(fd1[1]);
        exit(0);
    }
    int res1, res2;
    read(fd[0], &res1, sizeof(int));
    read(fd1[0], &res2, sizeof(int));
    close(fd[0]);
    close(fd1[0]);
    wait(NULL);

    return res1 + res2;
}

int main(int argc, char* argv[]){
    int n = atoi(argv[1]);

    int res = fib(n);
    printf("%d\n", res);
}
