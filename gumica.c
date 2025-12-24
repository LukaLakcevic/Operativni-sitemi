#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int *array, n, k, ticket = 0, counter = 0;
pthread_mutex_t m;
pthread_cond_t c;

void *fun(void *arg){
    int index = *(int*)arg;
    
    while(1){
        int my_ticket = ticket++;
        
        pthread_mutex_lock(&m);
        if(n <= 0){
            pthread_mutex_unlock(&m);
            break;
        }

        while(my_ticket != counter){
            if(n <= 0){
                pthread_mutex_unlock(&m);
                break;
            }
            pthread_cond_wait(&c, &m);
        }

        printf("Nit %d je uzela tiket %d i barata sledecim nizom: ", index, my_ticket);
        for(int i = 0; i < n; i++)
            printf("%d ", array[i]);


        for(int j = index; j < n - 1; j++)
            array[j] = array[j+1];

        array = (int*)realloc(array, (--n)*sizeof(int));
        counter++;

        printf("\n Niz nakon obrade: ");
        for(int i = 0; i < n; i++)
             printf("%d ", array[i]);
        printf("\n");

        pthread_cond_broadcast(&c);
        pthread_mutex_unlock(&m);
    }
}

int main(int argc, char *argv[]){
    n = atoi(argv[1]);
    k = atoi(argv[2]);
    int index[k];

    pthread_t th[k];
    pthread_mutex_init(&m, NULL);
    pthread_cond_init(&c, NULL);

    array = (int*)malloc(n*sizeof(int));
    for(int i = 0; i < n; i++)
        array[i] = rand()%9;

    for(int i = 0; i < k; i++)
        index[i] = i;

    for(int i = 0; i < k; i++)
        pthread_create(&th[i], NULL, fun, &index[i]);

    for(int i = 0; i < k; i++)
        pthread_join(th[i], NULL);

    pthread_mutex_destroy(&m);
    pthread_cond_destroy(&c);
}
