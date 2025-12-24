#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int *array, stage = 0, n;
pthread_mutex_t m;
pthread_cond_t c;

void *fun1(void *arg){
    array = (int*)malloc(n*sizeof(int));

    stage = 1;
    pthread_cond_broadcast(&c);
}

void *fun2(void *arg){
    pthread_mutex_lock(&m);
    while(stage != 1)
        pthread_cond_wait(&c, &m);
    pthread_mutex_unlock(&m);

    for(int i = 0; i < n; i++)
        array[i] = rand()%9;

    printf("Originalni niz: ");
    for(int i = 0; i < n; i++)
        printf("%d ", array[i]);
    printf("\n");  

    stage = 2;
    pthread_cond_broadcast(&c);
}

void *fun3(void *arg){
    pthread_mutex_lock(&m);
    while(stage != 2)
        pthread_cond_wait(&c, &m);
    pthread_mutex_unlock(&m);

    int k = 0;
    for(int i = 0; i < n; i++)
        if(array[i] % 5 == 0 && array[i] != 0){
            for(int j = i; j < n-1; j++)
                array[j] = array[j+1];
            k++;
            array = (int*)realloc(array, (n-k)*sizeof(int));   
        }
    n -= k;
    stage = 4;
    pthread_cond_broadcast(&c);
}

void *fun4(void *arg){
    pthread_mutex_lock(&m);
    while(stage != 4)
        pthread_cond_wait(&c, &m);
    pthread_mutex_unlock(&m);

    for(int i = 0; i < n; i++)
        printf("%d ", array[i]);
    printf("\n");
}

int main(int argc, char *argv[]){
    n = atoi(argv[1]);

    pthread_t th1;
    pthread_t th2;
    pthread_t th3;
    pthread_t th4;

    pthread_mutex_init(&m, NULL);
    pthread_cond_init(&c, NULL);

    pthread_create(&th1, NULL, fun1, &n);
    pthread_create(&th2, NULL, fun2, &n);
    pthread_create(&th3, NULL, fun3, &n);
    pthread_create(&th4, NULL, fun4, &n);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL);
    pthread_join(th4, NULL);

    pthread_mutex_destroy(&m);
    pthread_cond_destroy(&c);
}
