#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_THREADS 8 // Максимальное количество потоков

typedef struct
{
    int *array;
    int left;
    int right;
    int maxThreads;
} ThreadData;

void swap(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(int arr[], int low, int high)
{
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void *quickSort(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    int *arr = data->array;
    int low = data->left;
    int high = data->right;
    int maxThreads = data->maxThreads;

    if (low < high)
    {
        int pi = partition(arr, low, high);

        // Создание структуры для левого и правого подмассива
        ThreadData leftData = {arr, low, pi - 1, maxThreads};
        ThreadData rightData = {arr, pi + 1, high, maxThreads};

        // Используем потоки только если их меньше, чем maxThreads
        if (maxThreads > 1)
        {
            pthread_t leftThread, rightThread;
            pthread_create(&leftThread, NULL, quickSort, &leftData);
            pthread_create(&rightThread, NULL, quickSort, &rightData);

            pthread_join(leftThread, NULL);
            pthread_join(rightThread, NULL);
        }
        else
        {
            // Если достигли максимального количества потоков, сортируем последовательно
            quickSort(&leftData);
            quickSort(&rightData);
        }
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Использование: %s <количество_потоков> <размер_массива>\n", argv[0]);
        return 1;
    }

    int numThreads = atoi(argv[1]);
    int arraySize = atoi(argv[2]);
    if (numThreads <= 0 || arraySize <= 0 || numThreads > MAX_THREADS)
    {
        fprintf(stderr, "Количество потоков должно быть положительным числом и не превышать %d\n", MAX_THREADS);
        return 1;
    }

    int *arr = (int *)malloc(arraySize * sizeof(int));
    if (!arr)
    {
        perror("Не удалось выделить память для массива");
        return 1;
    }

    srand(time(NULL));
    for (int i = 0; i < arraySize; i++)
    {
        arr[i] = rand() % 100;
    }

    printf("Исходный массив: ");
    for (int i = 0; i < arraySize; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    ThreadData data = {arr, 0, arraySize - 1, numThreads};
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    quickSort(&data);

    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Отсортированный массив: ");
    for (int i = 0; i < arraySize; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    printf("Время выполнения сортировки: %f секунд\n", time_taken);

    free(arr);
    return 0;
}
