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

void pop_process(Process* process)
{
    int pi = return_pi(process);
    int q_index = Q -pi -1;
    Queue* queue = queueslist[q_index];
    int index;
    for (int i = 0; i < queue -> c; i++)
    {
        if (queue -> processes[i] == process){index = i;}
    }
    if (index < queue -> c - 1)
    {
        for(int i = index; i < queue -> c - 1; i++)
        {
            queue -> processes[i] = queue -> processes[i + 1];
        }
    }
    queue -> processes[queue -> c - 1] = NULL;
    queue -> c = queue -> c - 1:
}


Queue* QueueInit(int Q, int q)
{
    Queue* queue = calloc(1,sizeof(Queue));
    queue->pi = 0;
    queue->c = 0;
    return queue;
}
void read_input()
{
    for (int i = 0;i<input_file->len;i++)
    {
        //Revisar nombre (posiblemente char name[])
        Process* process = ProcessInit(input_file->lines[i][0],atoi(input_file->lines[i][1]),atoi(input_file->lines[i][2]), atoi(input_file->lines[i][3]), atoi(input_file->lines[i][4]), atoi(input_file->lines[i][5]));
        processes[i] = process;
    }
}

void process_to_sistem()
{
    for (int i = 0;i<processes_number;i++)
    {
      if (ticks >= processes[i]->tiempo_inicio)
      {
        int last = queueslist[0]->c;
        processes[i]->status = 1;
        queueslist[0]->processes[last] = processes[i];
        queueslist[0]->c+=1;
      }      
    }
}

void cpu()
{
    if (running_process)
    {
        int pi = return_pi();
        int q_index = Q -pi -1;
        running_process->cycles -= 1;
        running_process->running_time +=1;
        if (running_process->wait == running_process->running_time)
        {
            running_process->status = 2;
            running_process = NULL;
        }
        if (running_process->cycles == 0)
        {
            running_process->status = 3;
            running_process = NULL;
        }
        if (queueslist[q_index]->quantum == ticks)
        {
            running_process->status = 1;
            //Mover  a la siguiente cola
        }
    }
    if (!running_process)
    {
     int a = 0;   
    }
}

int return_pi(Process* process)
{
    for (int i = 0;i<Q;i++)
    {
        for (int j = 0;j<processes_number;j++)
        {
            if (running_process == queueslist[i]->processes[j])
            {
                return queueslist[i]->pi;
            }
        }
    }
}