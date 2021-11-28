#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <omp.h>
#include "constants.c"

char storedReference[BUFF_SIZE];

void uploadReference(char uploadedReference[])
{
  bzero(storedReference, BUFF_SIZE);
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

  count++;

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
    printf("%s was not found\n", line);
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

    int i = 0;
#pragma omp parallel default(none) private(i) shared(splitted, intervals, linesCount)
    {
#pragma omp for
      for (int i = 0; i < linesCount; i++)
      {
        processLine(*(splitted + i), intervals[i]);
      }
    }

    int coverage = intervalsCoverage(intervals, linesCount);
    float percentage = (float)coverage / (float)strlen(storedReference) * 100;
    printf("%d lines covering %f%% of the genoma reference\n", coverage, percentage);
  }
}

void receiver(int sockfd)
{
  char buff[BUFF_SIZE];
  while (1)
  {
    bzero(buff, BUFF_SIZE);
    read(sockfd, buff, sizeof(buff));
    if (strcmp(buff, UPLOAD_REFERENCE) == 0)
    {
      bzero(buff, BUFF_SIZE);
      read(sockfd, buff, sizeof(buff));
      uploadReference(buff);
    }
    else if (strcmp(buff, UPLOAD_SEQUENCE) == 0)
    {
      bzero(buff, BUFF_SIZE);
      read(sockfd, buff, sizeof(buff));
      uploadSequence(buff);
    }
  }
}

int main()
{
  int sockfd, connfd;
  SA_IN servaddr, client;

  // Socket create and verification
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1)
    exit(EXIT_FAILURE);
  bzero(&servaddr, sizeof(servaddr));

  // Assign IP, PORT
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);

  // Binding newly created socket to given IP and verification
  if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
    exit(EXIT_FAILURE);

  // Listen
  if ((listen(sockfd, 5)) != 0)
    exit(EXIT_FAILURE);

  // Accept the data packet from client and verification
  int len = sizeof(client);
  connfd = accept(sockfd, (SA *)&client, (socklen_t *)&len);
  if (connfd < 0)
    exit(EXIT_FAILURE);

  omp_set_num_threads(NUM_THREADS);
  receiver(connfd);
  close(sockfd);
}
