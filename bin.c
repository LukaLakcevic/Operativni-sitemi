#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t m;
pthread_cond_t c;
int *array;
int j = 0;
int stage = 0;

void *fun1(void *arg){
    int n = *(int*)arg;
    pthread_mutex_lock(&m);
    
    array = (int*)malloc(n*sizeof(int));
    for(int i = 0; i < n; i++){
        array[i] = i*3;
    }
    stage = 1;
    pthread_cond_broadcast(&c);
    while(j < n-1)
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

    printf("%d ", 2*array[j] + 1);
    j++;
    if(j == n-1)
        pthread_cond_signal(&c);
    pthread_mutex_unlock(&m);
}

int main(int argc, char* argv[]){
    int n = atoi(argv[1]);

    pthread_t th[n];
    pthread_mutex_init(&m, NULL);
    pthread_cond_init(&c, NULL);

    pthread_create(&th[0], NULL, fun1, &n);

    for(int i = 1; i < n; i++)
        pthread_create(&th[i], NULL, fun2, &n);

    for(int i = 0; i < n; i++)
        pthread_join(th[i], NULL);
   printf("\n");
     pthread_mutex_destroy(&m);
     pthread_cond_destroy(&c);
}
