#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int *array;
int num_elements;
int max_threads;
int current_threads = 0;

typedef struct
{
    int left;
    int right;
} ThreadData;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *quicksort(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    int left = data->left;
    int right = data->right;

    if (left >= right)
        return NULL;

    int pivot = array[(left + right) / 2];
    int i = left, j = right;

    while (i <= j)
    {
        while (array[i] < pivot)
            i++;
        while (array[j] > pivot)
            j--;
        if (i <= j)
        {
            int temp = array[i];
            array[i] = array[j];
            array[j] = temp;
            i++;
            j--;
        }
    }

    pthread_t thread1, thread2;
    ThreadData leftData = {left, j};
    ThreadData rightData = {i, right};

    pthread_mutex_lock(&mutex);
    if (current_threads < max_threads)
    {
        current_threads++;
        pthread_mutex_unlock(&mutex);
        pthread_create(&thread1, NULL, quicksort, (void *)&leftData);
    }
    else
    {
        pthread_mutex_unlock(&mutex);
        quicksort((void *)&leftData);
    }

    pthread_mutex_lock(&mutex);
    if (current_threads < max_threads)
    {
        current_threads++;
        pthread_mutex_unlock(&mutex);
        pthread_create(&thread2, NULL, quicksort, (void *)&rightData);
    }
    else
    {
        pthread_mutex_unlock(&mutex);
        quicksort((void *)&rightData);
    }

    // Ожидание завершения потоков
    if (current_threads < max_threads)
    {
        pthread_join(thread1, NULL);
        pthread_mutex_lock(&mutex);
        current_threads--;
        pthread_mutex_unlock(&mutex);
    }

    if (current_threads < max_threads)
    {
        pthread_join(thread2, NULL);
        pthread_mutex_lock(&mutex);
        current_threads--;
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void generate_array(int size)
{
    for (int i = 0; i < size; i++)
    {
        array[i] = rand() % 1000;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Использование: %s <количество элементов> <максимальное количество потоков>\n", argv[0]);
        return 1;
    }

    num_elements = atoi(argv[1]);
    max_threads = atoi(argv[2]);

    array = malloc(num_elements * sizeof(int));
    if (array == NULL)
    {
        perror("Ошибка выделения памяти");
        return 1;
    }

    generate_array(num_elements);

    clock_t start_time = clock();

    ThreadData initialData = {0, num_elements - 1};
    quicksort((void *)&initialData);

    clock_t end_time = clock();
    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("Время выполнения: %f секунд\n", time_taken);

    free(array);
    return 0;
}
