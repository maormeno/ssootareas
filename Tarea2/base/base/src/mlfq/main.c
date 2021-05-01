#include <stdio.h>
#include "functions.h"
#include "../file_manager/manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv)
{

  input_file = read_file(argv[1]);
  output_name = argv[2];
  Q = atoi(argv[3]);
  q = atoi(argv[4]);
  S = atoi(argv[5]);
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
    ticks += 1;
    printf("%i\n\n", ticks);
    reset_ticks += 1;
  }

}


