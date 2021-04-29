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

void process_to_sistem(int Q, int ticks,Queue* queueslist[], Process *processes[input_file->len])
{
    int c = 0;
    for (int i = 0;i<Q;i++)
    {
      if (ticks >= processes[i]->tiempo_inicio)
      {
        processes[i]->status = 1;
        queueslist[0]->processes[c] = processes[i];
        c+=1;
      }
      
    }
}

void cpu(int ticks, Queue* queueslist[], Process *processes[], Process* running_process,int Q, int processes_number)
{
    if (running_process)
    {
        int pi = return_pi(Queue* queueslist[],Process* running_process, int Q, int processes_number);
        int q_index = Q -pi -1;
        running_process->cycles -= 1;
        running_process->running_time +=1;
        if (running_process->wait == ticks)
        {
            running_process->status = 3;
            running_process = NULL;
        }
        if (running_process->cycles == 0)
        {
            running_process->status = 4;
            running_process = NULL;
        }
        if (queueslist[q_index]->quantum == ticks)
        {
            running_process->status = 1
            //Mover  a la siguiente cola
        }
    }
    if !(running_process)
    {
        
    }
}

int return_pi(Queue* queueslist[],Process* running_process, int Q, int processes_number)
{
    for (int i = 0;i<Q;i++)
    {
        for (j = 0;j<processes_number;j++)
        {
            if (running_process == queueslist[i]->processes[j])
            {
                return queueslist[i]->pi;
            }
        }
    }
}