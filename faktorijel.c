#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

int fak(int n){
    if(n < 1){
        return 1;
    }

    int fd[2];
    pipe(fd);
    
    int pid = fork();
    if(pid == 0){
        close(fd[0]);
        int res = n * fak(n-1);
        write(fd[1], &res, sizeof(int));
        close(fd[1]);
        exit(1);
    }

    close(fd[1]);
    int res;
    read(fd[0], &res, sizeof(int));
    close(fd[0]);

    return res;
    
}

int main(int argc, char* argv[]){
    if(argc != 2){
        printf("Pogresan broj aegumenata!\n");
        exit(0);
    }

    int n = atoi(argv[1]);

    int fd1[2], fd2[2];
    if(pipe(fd1) != 0 || pipe(fd2) != 0){
        printf("Pipe faild!\n");
        exit(1);
    }

    int pid1 = fork();
    if(pid1 < 0){
        printf("Fork 1 faild!\n");
        exit(1);
    }else if(pid1 == 0){
        close(fd1[0]);
        int res = 1;
        for(int i = 1; i <= n; i++)
            if(i % 2 == 0)
                res *= i;
        write(fd1[1], &res, sizeof(int));
        close(fd1[1]);
        exit(1);
    }

    int pid2 = fork();
    if(pid2 < 0){
        printf("Fork 2 faild!\n");
        exit(1);
    }else if(pid2 == 0){
        close(fd2[0]);
        int res = 1;
        for(int i = 1; i <= n; i++)
            if(i % 2 != 0)
                res *= i;
        write(fd2[1], &res, sizeof(int));
        close(fd2[1]);
        exit(1);
    }

    close(fd1[1]);
    close(fd2[1]);
    int res1, res2;
    read(fd1[0], &res1, sizeof(int));
    read(fd2[0], &res2, sizeof(int));
    
    close(fd1[0]);
    close(fd2[0]);
    wait(NULL);
    int res = res1*res2; 
    printf("Faktorijel broja %d: %d\n", n, res);

    int res3 = fak(n);

    printf("Faktorijel broja %d: %d\n", n, res3);
    return 0;
}
