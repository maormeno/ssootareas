#include <stdio.h>
#include "functions.h"
#include "../file_manager/manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv)
{

  input_file = read_file(argv[1]);
  Q = atoi(argv[2]);
  q = atoi(argv[3]);
  S = atoi(argv[4]);
  processes_number = input_file->len;
  ticks = 0;
  //Crear y llenar lista de Queues
  queueslist = calloc(Q, sizeof(Queue*));
  for (int i = 0;i<Q;i++)
  {
    Queue* queue = QueueInit(Q,q);
    queue->pi = (Q-i)-1;
    queue->quantum = (Q-queue->pi)*q;
    queueslist[i] = queue;
  }
  processes = calloc(input_file->len,sizeof(Process*));
  read_input();
  running_process = NULL;
  ticks = 0;
  reset_ticks = 0;
  while (ticks>=0)
  {
    process_to_sistem();
    cpu();
    if (reset_ticks == S)
    {
      reset_queues();
      reset_ticks = 0;
      }
    ticks += 1;
    reset_ticks += 1;
    //finish();
  }
  free_mem();
  input_file_destroy(input_file);
}


