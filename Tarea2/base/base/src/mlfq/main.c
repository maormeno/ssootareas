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
  ticks += 3;
  while (ticks>=0)
  {
    process_to_sistem(input_file, ticks, queueslist, processes);
    //printf("%i",queueslist[0]->processes[0]->pid);
    ticks = -1;
  }
  input_file_destroy(input_file);
}


