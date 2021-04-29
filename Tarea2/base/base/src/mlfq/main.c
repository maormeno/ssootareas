#include <stdio.h>
#include "functions.h"
#include "../file_manager/manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv)
{
  int ticks = 0;
  InputFile* input_file = read_file(argv[1]);
  int Q = atoi(argv[2]);
  int q = atoi(argv[3]);
  int S = atoi(argv[4]);
  int processes_number = input_file->len;
  //Crear y llenar lista de Queues
  Queue* queueslist[Q];
  for (int i = 0;i<Q;i++)
  {
    Queue* queue = QueueInit(Q,q);
    queue->pi = (Q-i)-1;
    queue->quantum = (Q-queue->pi)*q;
    queueslist[i] = queue;
  }
  Process *processes[input_file->len];
  read_input(input_file, processes);
  Process* running_process = NULL;
  while (ticks>=0)
  {
    process_to_sistem(Q, ticks, queueslist, processes);
    cpu(int ticks, Queue* queueslist[], Process *processes[], Process* running_process,int Q, int processes_number);
    ticks = -1;
  }
  input_file_destroy(input_file);
}


