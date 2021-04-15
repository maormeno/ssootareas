#include <time.h>
#include <stdlib.h>
#include "process.h"
#include <unistd.h>

pid_t* childs;
int childsnumber;
void send_sigabrt(int sig)
{
    pid_t currchild;
    for(int i = 0;i<childsnumber;i++)
    {
        currchild = childs[i];
        if(currchild > 0)
        {
        kill(currchild,SIGABRT);
        }
    }
}
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
        childsnumber = atoi(lines[process][2]);
        childs = calloc(childsnumber,sizeof(pid_t));
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
                    signal(SIGALARM, send_sigabrt);
                    signal(SIGABRT,send_sigabrt);
                    if (*lines[process][0] == 'R')
                    {
                        signal(SIGINT, send_sigabrt);
                    }

                }
                
            }
        }
    }
    
}