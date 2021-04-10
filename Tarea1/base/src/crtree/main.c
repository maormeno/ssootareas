#include <stdio.h>
#include <stdlib.h>
#include "../file_manager/manager.h"


int main(int argc, char **argv)
{
  InputFile* input_file = read_file(argv[1]);
  for(int i = 0; i < input_file->len; i++)
  {
    printf(input_file->lines[i][0]);
    printf("\n");
  }
}
