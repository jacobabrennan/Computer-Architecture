#include <stdlib.h>
#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
    //
    if(2 != argc)
    {
        fprintf(stdout, "Usage: ls8 FilePath");
        exit(1);
    }
    char *file_path = argv[1];
    //
    struct cpu cpu;
    cpu_init(&cpu);
    cpu_load(&cpu, file_path);
    cpu_run(&cpu);
    cpu_cleanup(&cpu);
    //
    return 0;
}
