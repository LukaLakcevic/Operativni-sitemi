#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

//Napisati C program koji vrsi fonukciju sledeceg izraza u SHELL-u: ls | grep "exec" | sort.

int main(int argc, char* argv[]){

    int fd1[2], fd2[2];
    pipe(fd1);
    pipe(fd2);

    int pid1 = fork();
    if(pid1 == 0){
        close(fd1[0]);
        dup2(fd1[1], STDOUT_FILENO);
        close(fd1[1]);

        execlp("ls", "ls", (char*)NULL);
        perror("Exec1 faild\n");    
        exit(1);
    }

    int pid2 = fork();
    if(pid2 == 0){
        close(fd1[1]);
        dup2(fd1[0], STDIN_FILENO);
        close(fd1[0]);

        close(fd2[0]);
        dup2(fd2[1], STDOUT_FILENO);
        close(fd2[1]);

        execlp("grep", "grep", "exec", (char*)NULL);
        perror("Exec2 faild!\n");
        exit(2);
    }

    close(fd1[0]);
    close(fd1[1]);

    int pid3 = fork();
    if(pid3 == 0){
        close(fd2[1]);
        dup2(fd2[0], STDIN_FILENO);
        close(fd2[0]);

        execlp("sort", "sort", (char*)NULL);
        perror("Exec3 faild!\n");
        exit(3);
    }

    close(fd2[0]);
    close(fd2[1]);
    return 0;
}
