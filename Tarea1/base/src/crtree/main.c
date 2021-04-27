#include <stdio.h>
#include <stdlib.h>
#include "../file_manager/manager.h"
#include "process.h"


int main(int argc, char **argv)
{
  InputFile* input_file = read_file(argv[1]);
  int start = atoi(argv[2]);
  execute_process(input_file->lines, start);
  input_file_destroy(input_file);
}
