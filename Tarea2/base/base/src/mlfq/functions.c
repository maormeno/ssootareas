#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//Revisar wait y waiting Delay
//Revisar el status por default
//Asignar prioridad al proceso segun la cola en la que sae encuentre (Issue)
Process* ProcessInit(char name,int pid, int tiempo_inicio, int cycles, int wait, int waiting_delay)
{
  Process* process = calloc(1,sizeof(Process));
  process->pid = pid;
  process->name = name;
  process->p = 0;
  process->wait = wait;
  process->waiting_delay = waiting_delay;
  process->tiempo_inicio = tiempo_inicio;
  process->cycles = cycles;
  process->status = 0;
  return process;

}


Queue* QueueInit(int Q, int q)
{
    Queue* queue = calloc(1,sizeof(Queue));
    queue->pi = 0;
    return queue;
}
void read_input(InputFile* input_file, Process* processes[])
{
    InputFile* file = input_file;
    for (int i = 0;i<file->len;i++)
    {
        //Revisar nombre (posiblemente char name[])
        Process* process = ProcessInit(file->lines[i][0],atoi(file->lines[i][1]),atoi(file->lines[i][2]), atoi(file->lines[i][3]), atoi(file->lines[i][4]), atoi(file->lines[i][5]));
        processes[i] = process;
    }
}

void process_to_sistem(InputFile* input_file, int ticks,Queue* queueslist[], Process *processes[input_file->len])
{
    int c = 0;
    for (int i = 0;i<input_file->len;i++)
    {
      if (ticks >= processes[i]->tiempo_inicio)
      {
        queueslist[0]->processes[c] = processes[i];
        c+=1;
      }
      
    }
}
