#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t m;
pthread_cond_t c;
int *array;
int stage = 0;

void *fun1(void *arg){
    int n = *(int*)arg;

    pthread_mutex_lock(&m);
    array = (int*)malloc(n*sizeof(int));
    stage = 1;
    pthread_cond_broadcast(&c);

    while(stage != 4)
        pthread_cond_wait(&c, &m);
    free(array);
    array = NULL;
    pthread_mutex_unlock(&m);
}

void *fun2(void *arg){
    int n = *(int*)arg;

    pthread_mutex_lock(&m);
    while(stage != 1)
        pthread_cond_wait(&c, &m);
        
    for(int i = 0; i < n; i++)
        scanf("%d", &array[i]);
    
    stage = 2;
    pthread_cond_broadcast(&c);
    pthread_mutex_unlock(&m);
}

void *fun3(void *arg){
    int n = *(int*)arg;

    pthread_mutex_lock(&m);
    while(stage != 2)
        pthread_cond_wait(&c, &m);

    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            if(array[i] < array[j]){
                int tmp = array[i];
                array[i] = array[j];
                array[j] = tmp;
            }
    stage = 3;
    pthread_cond_broadcast(&c);
    pthread_mutex_unlock(&m);
}

void *fun4(void *arg){
    int n = *(int*)arg;

    pthread_mutex_lock(&m);
    while(stage != 3)
        pthread_cond_wait(&c, &m);

    for(int i = 0; i < n; i++)
        printf("%d ", array[i]);
    printf("\n");

    stage = 4;
    pthread_cond_broadcast(&c);
    pthread_mutex_unlock(&m);
}

int main(int argc, char* argv[]){
    int n = atoi(argv[1]);

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
