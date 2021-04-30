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
      if (ticks == processes[i]->tiempo_inicio)
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
        int pi = return_pi(running_process);
        int q_index = Q -pi -1;
        running_process->cycles -= 1;
        running_process->running_time +=1;
        if (running_process->wait == running_process->running_time)
        {
            running_process->status = 2;
            increase_queue(running_process);
            running_process = NULL;
        }
        if (running_process->cycles == 0)
        {
            running_process->status = 3;
            pop_process(running_process);
            running_process = NULL;
        }
        if (queueslist[q_index]->quantum == running_process->running_time)
        {
            running_process->status = 1;
            decrease_queue(running_process);
            running_process = NULL;

        }
    }
    if (!running_process)
    {
        for (int i = 0;i<Q;i++)
        {
            Queue* queue = queueslist[i];
            for (int j = 0;j<queue->c, j++)
            {
                Process* process = queue[i].processes[j];
                if (process->status == 1)
                {
                    running_process = process;
                    process->status =0;
                }
            }
        }
    }
    waiting_processes();
}

int return_pi(Process* process)
{
    for (int i = 0;i<Q;i++)
    {
        for (int j = 0;j<processes_number;j++)
        {
            if (process == queueslist[i]->processes[j])
            {
                return queueslist[i]->pi;
            }
        }
    }
}

void increase_queue(Process* process)
{
    int pi = return_pi(process);
    int q_index = Q -pi -1;
    if (q_index > 0)
    {
        int last = queueslist[q_index-1]->c;
        pop_process(process);
        queueslist[q_index-1]->processes[last] = process;
        queueslist[q_index-1]->c += 1;
    }
}

void decrease_queue(Process* process)
{
    int pi = return_pi(process);
    int q_index = Q -pi -1;
    if (q_index < Q-1)
    {
        int last = queueslist[q_index+1]->c;
        pop_process(process);
        queueslist[q_index+1]->processes[last] = process;
        queueslist[q_index+1]->c += 1;
    }
}
void waiting_processes()
{
    for (int i = 0;i<processes_number;i++)
    {
        if (processes[i]->status == 2)
        {
            processes[i]->waiting_time += 1;
            if (processes[i]->waiting_time == processes[i]->waiting_delay)
            {
                processes[i]->status = 1;
            }
        }
    }
}