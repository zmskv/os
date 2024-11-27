#include <stdio.h>
#include <dlfcn.h>

typedef float (*func_t)(int);

int main()
{
    void *lib_handle;
    func_t func;
    char *error;
    int choice, arg;

    while (1)
    {
        printf("Введите: 1 для расчета Pi, 2 для расчета e, 0 для смены библиотеки: ");
        scanf("%d", &choice);

        if (choice == 0)
        {
            printf("Смена реализации: ");
            char lib[20];
            scanf("%s", lib);
            if (lib_handle)
                dlclose(lib_handle);
            lib_handle = dlopen(lib, RTLD_LAZY);
            if (!lib_handle)
            {
                fprintf(stderr, "%s\n", dlerror());
                return 1;
            }
        }
        else if (choice == 1 || choice == 2)
        {
            scanf("%d", &arg);
            func = dlsym(lib_handle, (choice == 1) ? "Pi_Leibniz" : "E_Series");
            if ((error = dlerror()) != NULL)
            {
                fprintf(stderr, "%s\n", error);
                return 1;
            }
            printf("Результат: %.6f\n", func(arg));
        }
    }

    if (lib_handle)
        dlclose(lib_handle);
    return 0;
}
