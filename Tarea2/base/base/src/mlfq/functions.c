#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//Revisar wait y waiting Delay
//Revisar el status por default
//Asignar prioridad al proceso segun la cola en la que sae encuentre (Issue)
Process* ProcessInit(char* name,int pid, int tiempo_inicio, int cycles, int wait, int waiting_delay)
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
  process->chosen_times = 0;
  process -> interruptions = 0;
  process->turnaround_time = 0;
  process->response_time = 0;
  process->waiting_time = 0;
  process->first_ejec = 0;
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
    queue -> c = queue -> c - 1;
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
        printf("añadiendo proceso %s con status %i en ticks %i\n",queueslist[0]->processes[last]->name,queueslist[0]->processes[last]->status, ticks );
      }      
    }
}



void cpu()
{
    waiting_processes();
    if (running_process)
    {
        printf("corriendo proceso %s\n", running_process->name);
        int pi = return_pi(running_process);
        int q_index = Q -pi -1;
        running_process->cycles -= 1;
        running_process->running_time +=1;
        //printf("Revisando proceso %i, ticks %i\n", running_process->pid, ticks);
        if (running_process->cycles == 0)
        {
            printf("terminando proceso %s en ticks %i\n", running_process->name, ticks);
            running_process->turnaround_time = ticks - running_process->tiempo_inicio;
            running_process->response_time = running_process->first_ejec - running_process->tiempo_inicio;
            printf("calculando response%i como %i -%i\n", running_process->response_time, running_process->first_ejec,running_process->tiempo_inicio);
            running_process->status = 3;
            pop_process(running_process);
            running_process = NULL;
        }

       if (running_process && queueslist[q_index]->quantum == running_process->running_time)
       {
           printf("proceso cediendo cpu por quantum\n");
           running_process->status = 1;
           running_process->interruptions += 1;
           decrease_queue(running_process);
           running_process = NULL;
       }
       if (running_process && running_process !=0 &&running_process->wait == running_process->running_time)
       {
           printf("proceso cediendo cpu por llegar a su tiempo wait/n");
           running_process->status = 2;
           increase_queue(running_process);
           running_process = NULL;
       }
       review_S();




    }
    if (!running_process)
    {
        //printf("revisando si hay que correr otro proceso, rp %i\n", running_process->pid);
        for (int i = 0;i<Q;i++)
        {
            Queue* queue = queueslist[i];
            printf("revisando cola %i\n", i);
            for (int j = 0; j<queue->c; j++)
            {
                Process* process = queue->processes[j];
                printf("revisando proceso %s, cuyo status es %i\n", process->name, process->status);
                if (process->status == 1)
                {
                    running_process = process;
                    if (running_process->first_ejec == 0)
                    {
                        running_process->first_ejec = ticks;
                        printf("asignando al proceso %s el first ejec %i",running_process->name, ticks );
                    }
                    process->status = 0;
                    process->chosen_times +=1;
                    printf("empezando a correr %s en ticks %i \n", running_process->name, ticks);
                    break;

                }
            }
            if(running_process)
            {
                break;
            }
        }
    }
    finish();
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
    return 0;
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
        if (processes[i]->status == 1)
        {
            processes[i]->waiting_time += 1;

        }
    }
    for (int i = 0;i<processes_number;i++)
    {
        if (processes[i]->status == 2)
        {
            processes[i]->waited_time += 1;
            processes[i]->waiting_time += 1;
            if (processes[i]->waited_time == processes[i]->waiting_delay)
            {
                processes[i]->status = 1;
                processes[i]->waited_time = 0;
            }
        }
    }

}

void reset_queues()
{
    printf("S ticks %i\n", ticks);
    for (int i = 1; i < Q ; i++)
    {
        for (int j = queueslist[i] -> c - 1; j >= 0; j--)
        {
            queueslist[0] -> processes[queueslist[0] -> c] = queueslist[i] -> processes[j];
            queueslist[0] -> c += 1;
            pop_process(queueslist[i] -> processes[j]);
        }
    }
}

void free_mem()
{
  for (int i = 0; i<processes_number;i++)
  {
    free(processes[i]);

  }
  free(processes);
  for (int i = 0; i<Q;i++)
  {
    free(queueslist[i]);

  }
  free(queueslist);
}
void finish()
{
    int cond = 0;
    for (int i = 0;i<processes_number;i++)
    {
        if (processes[i]->status != 3)
        {
            cond +=1;
        }
    }
    if (cond == 0)
    {
        ticks = -1;
        write_output();
        free_mem();
        input_file_destroy(input_file);
        exit(1);
    }
}
void review_S()
{
    if (reset_ticks == S)
    {
      reset_queues();
      reset_ticks = 0;
    }
}

void write_output()
{
    fpt = fopen(output_name, "w+");
    for (int i = 0; i < processes_number;i++)
    {
        Process* process = processes[i];
        printf("response:%i\n",process->response_time);
        fprintf(fpt,"%s, %i, %i, %i, %i, %i\n",process->name, process->chosen_times, process->interruptions, process->turnaround_time, process->response_time, process->waiting_time);
    }
    fclose(fpt);
}