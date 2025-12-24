#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int n, tiket = 0, counter = 0;
pthread_mutex_t m;
pthread_cond_t c;

void *fun(void *arg){
    int index = *(int*)arg;

    while(1){
        int my_ticket = tiket++;

        pthread_mutex_lock(&m);
        if(n <= 0){
            pthread_mutex_unlock(&m);
            break;        
        }

        printf("Nit %d uzima tiket %d\n", index, my_ticket);
        pthread_mutex_unlock(&m);

        pthread_mutex_lock(&m);
        while(my_ticket != counter)
            pthread_cond_wait(&c, &m);

        printf("Nit %d radi neki posao\n", index);
        n--; counter++;
        pthread_cond_broadcast(&c);
        sleep(2);
        pthread_mutex_unlock(&m);
    }
}

int main(int argc, char *argv[]){
    n = atoi(argv[1]);

    int index[n];
    pthread_t th[n];
    pthread_mutex_init(&m, NULL);
    pthread_cond_init(&c, NULL);

    for(int i = 0; i < n; i++)
        index[i] = i;

    for(int i = 0; i < n; i++)
        pthread_create(&th[i], NULL, fun, &index[i]);

    for(int i = 0; i < n; i++)
        pthread_join(th[i], NULL);

    pthread_mutex_destroy(&m);
    pthread_cond_destroy(&c);

    return 0;
}
