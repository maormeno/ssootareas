#pragma once
#include <stdio.h>
#include "../file_manager/manager.h"



typedef struct Process {
  int pid;
  int p;
  int status;
  int wait;
  int waiting_delay;
  int tiempo_inicio;
  int cycles;
  char name;
} Process;

typedef struct Queue{
  int pi;
  int quantum;
  Process* processes[];
}Queue;

Process* ProcessInit(char name,int pid, int tiempo_inicio, int cycles, int wait, int waiting_delay);
Queue* QueueInit(int Q, int q);
void read_input(InputFile* input_file, Process* processes[]);
//Funcion que encola procesos cuando llega su tiempo_inicio
void process_to_sistem(InputFile* input_file, int ticks,Queue* queueslist[], Process *processes[input_file->len]);


