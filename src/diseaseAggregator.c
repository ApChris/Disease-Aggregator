#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    long workers;
    long bufferSize;
    char * path = NULL;
    if (argc != 7)                          // Check if we have !=7 arguments
    {
      printf("ERROR INPUT!!\nGive for example : ./diseaseAggregator -w 4 -b 50 -i etc/input_dir\n");
      return -1;
    }

    for(int i = 1; i < argc; i++)           // Get arguments
    {
      if(strcmp(argv[i],"-w") == 0)         // Get number of workers
      {
          workers = atoi(argv[i+1]);
      }
      else if(strcmp(argv[i],"-b") == 0)    // Get buffer Size
      {
         bufferSize = atoi(argv[i+1]);

      }
      else if(strcmp(argv[i],"-i") == 0)    // Path of input _dir
      {
          path = (char *)malloc(sizeof(char)*strlen(argv[i+1]));
          strcpy(path,argv[i+1]);
      }

    }

    printf("workers:%lu\nbufferSize:%lu\npath:%s\n",workers,bufferSize,path);
    free(path);
    return 0;
}
