#pragma once
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <math.h>
#include "../file_manager/manager.h"

pid_t* childs;
time_t start, end;
int process_number;
int timer;
pid_t child_pid;
int childsnumber;
int interrupted;
int size;
char* nombre;
char** arguments;
char*** lines;
InputFile* handler_file;
void execute_process(InputFile* file, int process);
void send_sigabrt(int sig);
void worker_handler(int sig);