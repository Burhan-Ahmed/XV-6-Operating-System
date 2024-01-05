#include "types.h"
#include "stat.h"
#include "user.h"

int main(void)
{
    int i;
    for (i = 0; i < 5; i++)
    {
        int pid = fork();
        if (pid < 0)
        {
            printf(1, "Fork failed\n");
            exit();
        }
        else if (pid == 0)
        {
            // Mixed workload: Alternate between CPU-bound and I/O-bound work
            if (i % 2 == 0)
            {
                printf(1, "Process %d running with PID %d\n", i, getpid());
                volatile int x = 0;
                for (int j = 0; j < 20000000; j++)
                {

                    x = x + 1;
                }
                printf(1, "Process %d ended\n", i);
            }
            else
            {
                printf(1, "Current Process Interrupted...\n");
                sleep(5); // Simulate I/O work
            }
            exit();
        }
    }

    for (i = 0; i < 5; i++)
    {
        wait();
    }

    exit();
}
