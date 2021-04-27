#pragma once

pid_t* childs;
int childsnumber;
void execute_process(char*** lines, int process);
void send_sigabrt(int sig);