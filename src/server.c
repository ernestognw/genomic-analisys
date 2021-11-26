#include <stdio.h>
#include <string.h>
#include "constants.c"

char storedReference[BUFF_SIZE];

void uploadReference(char uploadedReference[])
{
  strcpy(storedReference, uploadedReference);
}

char **split(char *toSplit, const char delimiter)
{
  int count = 0;
  char *tmp = toSplit;
  char *lastDelimiter = 0;

  // Count how many elements will be extracted.
  while (*tmp)
  {
    if (delimiter == *tmp)
    {
      count++;
      lastDelimiter = tmp;
    }
    tmp++;
  }

  char delim[] = { delimiter };
  char **result = malloc(sizeof(char *) * count);

  if (result)
  {
    int index = 0;
    char *token = strtok(toSplit, delim);

    while (token)
    {
      *(result + index++) = strdup(token);
      token = strtok(NULL, delim);
    }
    *(result + index) = 0;
  }

  return result;
}

void uploadSequence(char uploadedSequence[])
{
  if (strlen(storedReference) == 0)
    printf("No reference has been previously uploaded\n");
  else
  {
    char **splitted = split(uploadedSequence, '\n');

    for (int i = 0; *(splitted + i); i++)
      printf("%s\n", *(splitted + i));
    // TODO: Process each line and compare
  }
}

// TODO: Daemonize
