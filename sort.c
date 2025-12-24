#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Globalni niz koji se sortira
int *array;

// Struktura za podatke koje svaka nit dobija
typedef struct {
    int start;
    int end;
} ThreadData;

// Funkcija za sortiranje jednog dela niza (Bubble sort)
void bubble_sort(int *arr, int start, int end) {
    for (int i = start; i < end - 1; i++) {
        for (int j = start; j < end - (i - start) - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Funkcija koju izvodi svaka nit
void *sort_part(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    printf("Nit sortira deo [%d - %d)\n", data->start, data->end);
    bubble_sort(array, data->start, data->end);
    pthread_exit(NULL);
}

// Funkcija za spajanje dve sortirane polovine u jednu
void merge(int *arr, int left, int mid, int right) {
    int n1 = mid - left;
    int n2 = right - mid;
    int *L = malloc(n1 * sizeof(int));
    int *R = malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    free(L);
    free(R);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Upotreba: %s <velicina_niza> <broj_niti>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int k = atoi(argv[2]);
    array = malloc(n * sizeof(int));
    pthread_t threads[k];
    ThreadData data[k];

    // Popuni niz slučajnim brojevima
    for (int i = 0; i < n; i++)
        array[i] = rand() % 100;

    printf("Pocetni niz:\n");
    for (int i = 0; i < n; i++) printf("%d ", array[i]);
    printf("\n\n");

    // Odredi delove niza koje svaka nit sortira
    int chunk = n / k;
    for (int i = 0; i < k; i++) {
        data[i].start = i * chunk;
        data[i].end = (i == k - 1) ? n : (i + 1) * chunk;  // poslednja nit uzima ostatak
        pthread_create(&threads[i], NULL, sort_part, &data[i]);
    }

    // Sačekaj da sve niti završe
    for (int i = 0; i < k; i++)
        pthread_join(threads[i], NULL);

    // Spajanje delova jedan po jedan
    int step = chunk;
    while (step < n) {
        for (int i = 0; i < n; i += 2 * step) {
            int mid = i + step;
            int right = (i + 2 * step < n) ? (i + 2 * step) : n;
            if (mid < right)
                merge(array, i, mid, right);
        }
        step *= 2;
    }

    printf("\nSortiran niz:\n");
    for (int i = 0; i < n; i++) printf("%d ", array[i]);
    printf("\n");

    free(array);
    return 0;
}

