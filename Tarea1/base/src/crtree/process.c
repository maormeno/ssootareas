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
#include <sys/time.h>
#include "../file_manager/manager.h"

void send_sigabrt(int sig)
{
    pid_t currchild,wpid;
    int status;
    int length = 3;
    char line[length];
    sprintf(line, "%d", process_number);
    char txt[4] = ".txt";
    char filename[length+4];
    strncpy(filename, line, length);
    strncat(filename, txt, 4);
    FILE* output = fopen(filename, "w");
    for(int i = 0;i<childsnumber;i++)
    {
        currchild = childs[i];
        if(currchild > 0)
        {
        kill(currchild,SIGABRT);
        }
    }
    while ((wpid = wait(&status)) > 0);

    for (int i = 0;i<childsnumber;i++)
    {
        int childindex = atoi(lines[process_number][i + 3]);
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
            if(*lines[process_number][0] == 'M' || *lines[process_number][0] == 'W' ) 
            {fprintf(output,"\n");}                    

        }                
        fclose(filePointer);
    }
    fclose(output);
    input_file_destroy(handler_file);
    free(childs);
    exit(1);
}

void worker_handler(int sig)
{
    pid_t wpid;
    int status;
    //printf("Se detuvo worker de pid");
    if (child_pid > 0)
    {
        kill(child_pid, SIGABRT);
    }
    while ((wpid = wait(&status)) > 0);
    interrupted = 1;
    timer = time(NULL) - start;
    int length = 3;
    char line[length];
    sprintf(line, "%d", process_number);
    char txt[4] = ".txt";
    char filename[length + 4];
    strcpy(filename, line);
    strcat(filename, txt);
    FILE* output = fopen(filename, "w");
    fprintf(output, "%s, ", nombre);
    if (size > 0)
    {
        for (int i = 0; i < size; i++)
        {
            fprintf(output, "%d, ", atoi(arguments[i]));
        }
    }
    fprintf(output, "%d, ", timer);
    fprintf(output, "%d, ", WEXITSTATUS(status));
    fprintf(output, "%d", interrupted);
    fclose(output);
    free(arguments);
    input_file_destroy(handler_file);
    exit(1);
}


void execute_process(InputFile* file, int process)
{
    interrupted = 0;
    int status = 0;
    pid_t wpid;
    start = time(NULL);
    process_number = process;
    lines = file -> lines;
    handler_file = file;
    if (*lines[process][0] == 'W')
    {
        size = atoi(lines[process][2]);
        int status;
        char *args[size + 2];
        arguments = malloc((size+1)*sizeof(char*));
        args[0] = lines[process][1];
        nombre = lines[process][1];
        args[1] = lines[process][2];
        if (size > 0)
        {
            for (int i = 0; i < size; i++)
            {
                arguments[i] = lines[process][i + 3];
                args[i + 2] = lines[process][i + 3];
            }
        }
        arguments[size] = NULL;
        args[size + 2] = NULL;
        printf("%s\n", nombre);
        child_pid = fork();
        signal(SIGINT, SIG_IGN);
        if (child_pid == 0)
        {
            execvp(args[0], args);
            exit(1);
        }
        else if (child_pid != 0)
        {
            //return_code = WEXITSTATUS(status);
            signal(SIGABRT, worker_handler);
            wait(&status);
            free(arguments);
            //timer = ((double) (end - start)) / CLOCKS_PER_SEC;
            timer = time(NULL) - start;
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
            fprintf(output, "%d, ", timer);
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
        if (*lines[process][0] == 'R'){signal(SIGINT,send_sigabrt);}
        else {signal(SIGINT,SIG_IGN);}
        if (childsnumber>0)
        {
            childs = calloc(childsnumber,sizeof(pid_t));
            for (int i = 0;i<childsnumber;i++)
            {
                child_pid = fork();

                if (child_pid == 0)
                {
                    int childindex = atoi(lines[process][i + 3]);
                    signal(SIGABRT, send_sigabrt);
                    free(childs);
                    execute_process(file,childindex);
                    input_file_destroy(file);
                    exit(1);
                }
                else if (child_pid != 0)
                {
                    childs[i] = child_pid;
                    signal(SIGALRM, send_sigabrt);
                    signal(SIGABRT,send_sigabrt);
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
        }
        free(childs);
    }
}