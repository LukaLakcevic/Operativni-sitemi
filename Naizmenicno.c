#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int n, *array;

void *parni(){
    for(int i = 0; i < n; i++){
        if(i % 2 == 0)
            array[i] = i;
        //sleep(1); 
    }
}

void *neparni(){
    for(int i = 0; i < n; i++){
        if(i % 2 != 0)
            array[i] = i;
        //sleep(1);
    }
}

int main(int argc, char *argv[]){
    n = atoi(argv[1]);
    
    array = (int*)malloc(n*sizeof(int));

    pthread_t th1, th2;
    
    pthread_create(&th1, NULL, parni, NULL);
    pthread_create(&th2, NULL, neparni, NULL);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    for(int i = 0; i < n; i++)
        printf("%d ", array[i]);
    printf("\n");
    
    return 0;
}
