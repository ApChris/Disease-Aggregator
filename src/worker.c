#include <stdio.h>
#include <stdbool.h>
#include "../include/list.h"


int main(int argc, char *argv[])
{
    printf("WORKER HEREE!!!!\n");
    for (size_t i = 0; i < argc; i++)
    {
        printf("-->%s\n", argv[i]);
    }

    return 0;
}
