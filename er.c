#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void er(int fd){
    int prime;
    if(read(fd, &prime, sizeof(int)) == 0){
        close(fd);     
       return;
    }

    printf("%d ", prime);
    fflush(stdout);

    int fd_next[2];
    pipe(fd_next);

    int pid = fork();
    if(pid == 0){
        close(fd_next[1]);
        er(fd_next[0]);
        exit(0);
    }

    close(fd_next[0]);
    int num;
    while(read(fd, &num, sizeof(int))> 0)
        if(num % prime != 0)
            write(fd_next[1], &num, sizeof(int));
    close(fd_next[1]);
    close(fd);
    wait(NULL);   
}

int main(int argc, char* argv[]){
    int n = atoi(argv[1]);

    int fd[2];
    pipe(fd);

    for(int i = 2; i <= n; i++)
        write(fd[1], &i, sizeof(int));
    close(fd[1]);
    
    int prime;
    while(read(fd[0], &prime, sizeof(int)) > 0){
        printf("%d ", prime);
        fflush(stdout);

        int fd_next[2];
        pipe(fd_next);
        if(fork() == 0){
            close(fd_next[0]);
            int num;
            while(read(fd[0], &num, sizeof(int)) > 0)
                if(num % prime != 0)
                    write(fd_next[1], &num, sizeof(int));

            close(fd_next[1]);
            close(fd[0]);
            exit(0);
        }
        close(fd_next[1]);
        close(fd[0]);
        fd[0] = fd_next[0]; 
        wait(NULL);
    }
    close(fd[0]);
    printf("\n");

    int fd1[2];
    pipe(fd1);

    for(int i = 2; i <= n; i++)
        write(fd1[1], &i, sizeof(int));
    close(fd1[1]);

    er(fd1[0]);
    close(fd1[0]);
    printf("\n");
    return 0;
}
