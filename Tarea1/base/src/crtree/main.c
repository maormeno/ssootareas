#include <stdio.h>
#include <stdlib.h>
#include "../file_manager/manager.h"
#include "process.h"


int main(int argc, char **argv)
{
  InputFile* input_file = read_file(argv[1]);
  int start = argv[2];
  execute_process(input_file->lines, start);
  /*for (int i = 0; i < input_file -> len; i++)
  {
    if (*input_file -> lines[i][0] == 'W'){
      printf("Proceso worker \n");
    }
  }*/

}
