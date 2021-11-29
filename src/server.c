#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
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

void processLine(  char *line, int interval[2])
{

 size_t n = strlen(line);
if (n > 0 && line[n] == '\0') {
 
    line[n-1] = 0;
}
 char * substringPointer;
  substringPointer = strstr(storedReference, line);
  int size = strlen(line);
 

  if (substringPointer != NULL)
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
    printf("was not found %s\n", line);
  }
}

int compareFirst(const void *a, const void *b)
{
  return *(int *)a - *(int *)b;
}

int intervalsCoverage(int intervals[][2], int intervalsAmount)
{
  qsort(intervals, intervalsAmount, sizeof(int *), compareFirst);

  int coverage = 0;
  int minStart = 0;

  int i = 0;
  while (i < intervalsAmount)
  {
    int start = fmax(minStart, intervals[i][0]);
    int end = intervals[i][1];

    int advance = 1;
    for (int j = i + 1; j < intervalsAmount; j++)
    {
      if (intervals[j][0] > end)
        break;
      end = fmax(end, intervals[j][1]);
      advance++;
    }

    i += advance;
    coverage += fmax(0, end - start);
    minStart = end;
  }

  return coverage;
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
  
#pragma parallel for
    for (int i = 0; *(splitted + i); i++)
      processLine(splitted[i], intervals[i]);

    int coverage = intervalsCoverage(intervals, linesCount);
    float percentage = (float)coverage / (float)strlen(storedReference) * 100;
    printf("%d lines covering %f%% of the genoma reference\n", coverage, percentage);
    printf("Line count is: %d\n", linesCount);
  }
}

// int main()
// {
//   return 0;
// }

// TODO: Daemonize
