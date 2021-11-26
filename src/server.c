#include <stdio.h>
#include <string.h>
#include "constants.c"

char storedReference[BUFF_SIZE];

void uploadReference(char uploadedReference[])
{
  strcpy(storedReference, uploadedReference);
}

char **split(char *toSplit, const char delimiter, int *count)
{
  *(count) = 0;
  char *tmp = toSplit;
  char *lastDelimiter = 0;

  // Count how many elements will be extracted.
  while (*tmp)
  {
    if (delimiter == *tmp)
    {
      *(count) += 1;
      lastDelimiter = tmp;
    }
    tmp++;
  }

  char delim[] = {delimiter};
  char **result = malloc(sizeof(char *) * *(count));

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

void processLine(const char *line, int interval[2])
{
  char *substringPointer = strstr(storedReference, line);

  if (substringPointer)
  {
    int position = substringPointer - storedReference;
    interval[0] = position;
    interval[1] = position + strlen(line);
    printf("%s found from character %d to character %d\n", line, interval[0], interval[1]);
  }
  else
  {
    interval[0] = 0;
    interval[1] = 0;
    printf("%s was not found\n", line);
  }
}

void uploadSequence(char uploadedSequence[])
{
  if (strlen(storedReference) == 0)
    printf("No reference has been previously uploaded\n");
  else
  {
    int linesCount = 0;
    char **splitted = split(uploadedSequence, '\n', &linesCount);
    int intervals[linesCount][2];

    for (int i = 0; *(splitted + i); i++)
      processLine(*(splitted + i), intervals[i]);

    for (int i = 0; i < linesCount; i++)
    {
      printf("[%d,%d]\n", intervals[i][0], intervals[i][1]);
    }
  }
}

// TODO: Daemonize
