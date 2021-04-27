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
#include "process.h"
#include "../file_manager/manager.h"

void send_sigabrt(int sig)
{
    pid_t currchild,wpid;
    int status;
    for(int i = 0;i<childsnumber;i++)
    {
        currchild = childs[i];
        if(currchild > 0)
        {
        kill(currchild,SIGABRT);
        }
    }
    while ((wpid = wait(&status)) > 0);
    exit(1);
}


void execute_process(char*** lines, int process)

{
    clock_t start, end;
    double time;
    //int return_code;
    int interrupted = 0;
    int status = 0;
    pid_t wpid, child_pid;
    start = clock();
    if (*lines[process][0] == 'W')
    {
        int size = atoi(lines[process][2]);
        char *args[size + 3];
        int status;
        args[0] = lines[process][1];
        args[1] = lines[process][2];
        if (size > 0)
        {
            for (int i = 0; i < size; i++)
            {
                args[i + 2] = lines[process][i + 3];
            }
        }
        args[size + 2] = NULL;
        pid_t child_pid = fork();
        if (child_pid == 0)
        {
            execvp(args[0], args);
        }
        else
        {
            wait(&status);
            //return_code = WEXITSTATUS(status);
            end = clock();
            time = ((double) (end - start)) / CLOCKS_PER_SEC;
            //int length = (int)((ceil(log10(process))+1));
            int length = 3;
            char line[length];
            sprintf(line, "%d", process);
            char txt[4] = ".txt";
            char filename[length + 4];
            strcpy(filename, line);
            strcat(filename, txt);
            FILE* output = fopen(filename, "w");
            fprintf(output, "%s, ", args[0]);
            if (size > 0)
            {
                for (int i = 2; i < size + 2; i++)
                {
                    fprintf(output, "%d, ", atoi(args[i]));
                }
            }
            fprintf(output, "%f, ", time);
            fprintf(output, "%d, ", WEXITSTATUS(status));
            fprintf(output, "%d", interrupted);
            fclose(output);
        }
    }
    else if (*lines[process][0] == 'M' || *lines[process][0] == 'R')
    {
        int timeout = atoi(lines[process][1]);
        childsnumber = atoi(lines[process][2]);
        alarm(timeout);
        if (childsnumber>0)
        {
            childs = calloc(childsnumber,sizeof(pid_t));
            for (int i = 0;i<childsnumber;i++)
            {
                child_pid = fork();

                if (child_pid == 0)
                {
                    int childindex = atoi(lines[process][i + 3]);
                    signal(SIGABRT,send_sigabrt);
                    free(childs);
                    execute_process(lines,childindex);
                    free(childs);
                    exit(1);
                }
                else if (child_pid != 0)
                {
                    childs[i] = child_pid;
                    signal(SIGALRM, send_sigabrt);
                    //signal(SIGABRT,send_sigabrt);
                }
            }
            
            //free(childs);
            while ((wpid = wait(&status)) > 0);
            //free(childs);
            
            //int length = (int)((ceil(log10(process))+1));
            int length = 3;
            char line[length];
            sprintf(line, "%d", process);
            char txt[4] = ".txt";
            char filename[length+4];
            strncpy(filename, line, length);
            strncat(filename, txt, 4);
            FILE* output = fopen(filename, "w");
            for (int i = 0;i<childsnumber;i++)
            {
                int childindex = atoi(lines[process][i + 3]);
                //int length = (int)((ceil(log10(childindex))+1));
                int length = 3;
                char line[length];
                sprintf(line, "%d", childindex);
                char txt[4] = ".txt";
                char filename[length + 4];
                strncpy(filename, line,length);
                strncat(filename, txt, 4);
                FILE* filePointer;
                int bufferLength = 255;
                char buffer[bufferLength];
                filePointer = fopen(filename, "r");
                while(fgets(buffer, bufferLength, filePointer)) 
                {
                    fprintf(output,"%s", buffer);
                    if(*lines[process][0] == 'M' || *lines[process][0] == 'W' ) 
                    {fprintf(output,"\n");}                    

                }
                
                fclose(filePointer);
            }
            fclose(output);
            free(childs);

        }
    }
}