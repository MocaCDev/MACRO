#ifndef READ
#define READ
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *read_file(char *filename)
{
  FILE* f = fopen(filename, "r");

  if(!f)
  {
    fclose(f);
    fprintf(stderr, "Error: No such file, %s", filename);
    exit(EXIT_FAILURE);
  }

  fseek(f, 0, SEEK_END);
  size_t size = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *data = (char *)calloc(size, sizeof(*data));
  int passed = fread(data, 1, size, f);

  if(!(strlen(data) == size))
  {
    fclose(f);
    fprintf(stderr, "Size of file(%ld) does not match the size when read(%ld)", size, strlen(data));
    exit(EXIT_FAILURE);
  }
  
  fclose(f);

  return data;
}

#endif