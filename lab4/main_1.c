#include <stdio.h>
#include "lib_pi.h"
#include "lib_e.h"

int main()
{
    int choice, arg;
    printf("Введите: 1 для расчета Pi, 2 для расчета e: ");
    scanf("%d %d", &choice, &arg);

    if (choice == 1)
    {
        printf("Pi: %.6f\n", Pi_Leibniz(arg));
    }
    else if (choice == 2)
    {
        printf("e: %.6f\n", E_Series(arg));
    }
    return 0;
}
