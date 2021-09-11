#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define VALUES_SIZE 7

static int *max, *min;
static double *ave;
int main()
{

    min = mmap(NULL, sizeof *min, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    max = mmap(NULL, sizeof *max, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    ave = mmap(NULL, sizeof *ave, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *max = 0;
    *min = 0;
    *ave = 0;

    int values[VALUES_SIZE] = {90, 81, 78, 95, 79, 72, 85};

    if (fork() == 0)
    {
        for (int i = 0; i < VALUES_SIZE; i++)
            *ave += values[i];

        *ave /= VALUES_SIZE;
    }
    else
    {
        if (fork() == 0)
        {
            *min = values[0];
            for (int i = 0; i < VALUES_SIZE; i++)
                if (*min > values[i])
                    *min = values[i];
        }
        else
        {
            if (fork() == 0)
            {
                *max = values[0];
                for (int i = 0; i < VALUES_SIZE; i++)
                    if (*max < values[i])
                        *max = values[i];
            }
            else
            {
                while (wait(NULL) > 0)
                {
                }

                printf("The Average Value Is %lf", *ave);
                printf("\nThe Minimum Value Is %d", *min);
                printf("\nThe Maximum Value Is %d", *max);
                printf("\n");


                munmap(max, sizeof *max);
                munmap(max, sizeof *min);
                munmap(max, sizeof *ave);
            }
        }
    }

    return 0;
}