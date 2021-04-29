#pragma once
#include <stdio.h>
#include "../file_manager/manager.h"



typedef struct Process {
  int pid;
  int p;
  //0:running, 1:ready, 2:waiting, 3:finished
  int status;
  int wait;
  int waiting_delay;
  int tiempo_inicio;
  int cycles;
  char name;
  int running_time;
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
void process_to_sistem(int Q, int ticks,Queue* queueslist[], Process *processes[input_file->len]);
void cpu(Queue* queueslist[], Process *processes[input_file->len], int Q);
int return_pi(Queue* queueslist[],Process* running_process, int Q, int processes_number);