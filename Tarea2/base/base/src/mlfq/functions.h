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
  int c;
  Process* processes[2048];
}Queue;

  InputFile* input_file;
  Queue** queueslist;
  int Q ;
  int q ;
  int S ;
  int processes_number;
  int ticks;
  Process** processes;
  Process* running_process;


Process* ProcessInit(char name,int pid, int tiempo_inicio, int cycles, int wait, int waiting_delay);
Queue* QueueInit(int Q, int q);
void read_input();
//Funcion que encola procesos cuando llega su tiempo_inicio
void process_to_sistem();
void cpu();
int return_pi();