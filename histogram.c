#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

char c[1000];
char s[3];
int counter[3];

void *fun(){
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 1000; j++)    
            if(s[i] == c[j])
               counter[i]++;
    for(int i = 0; i < 3; i++)
        printf("Karakter %c se pojavljuje %d puta\n ", s[i], counter[i]); 
}

int main(int argc, char* argv[]){

    for(int i = 0; i < 1000; i++)
        c[i] = 'a'+rand()%26;
    for(int i = 0; i < 3; i++){
        printf("Unesi specijalan karakter broj %d: ", i+1);
        scanf(" %c", &s[i]);
    }    

    pthread_t th;
    pthread_create(&th, NULL, fun, NULL);
    pthread_join(th, NULL);
}
