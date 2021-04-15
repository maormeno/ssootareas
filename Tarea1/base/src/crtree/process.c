#include <time.h>
#include <stdlib.h>
#include "process.h"
#include <unistd.h>


void execute_process(char*** lines, int process)
{
    clock_t start, end;
    double time;

    start = clock();
    if (*lines[process][0] == 'W')
    {
        char* nombre = lines[process][1];
        int size = atoi(lines[process][2]);
        if (size > 0)
        {
            int* arguments_array[size];
            for (int i = 0; i < size; i++)
            {
                arguments_array[i] = lines[process][i + 3];
            }
        }
        //HAY QUE EJECUTAR EL PROGRAMA ACÁ
        end = clock();
        time = ((double) (end - start)) / CLOCKS_PER_SEC;
        //RETURN_RECIEVED = RECIBIR CÓDIGO DE RETORNO DE PROGRAMA EJECUTADO
        //INTERRPUTED = SI EL PROCESO FUE INTERRUMPIDO
        
    }
    else if (*lines[process][0] == 'M' || *lines[process][0] == 'R')
    {
        int timeout = atoi(lines[process][1]);
        int childsnumber = atoi(lines[process][2]);
        alarm(timeout);
        if (childsnumber>0)
        {
            for (int i = 0;i<childsnumber;i++)
            {
                pid_t child_pid = fork();
                if (child_pid == 0)
                {
                    int childindex = atoi(lines[process][i + 3]);
                    execute_process(lines,childindex);
                }
                else if (child_pid != 0)
                {
                    //Codigo padre
                }
                
            }
        }
    }
    
}