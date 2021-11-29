#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/socket.h>
#include <omp.h>
#include "constants.c"

char storedReference[BUFF_SIZE];

void uploadReference(char uploadedReference[])
{
  bzero(storedReference, BUFF_SIZE);
  printf("Received reference of %lu length\n", strlen(uploadedReference));
  strcpy(storedReference, uploadedReference);
  bzero(uploadedReference, BUFF_SIZE);
}

char **split(char *toSplit, const char delimiter, int *length)
{
  char **result = 0;
  int count = 0;
  char *tmp = toSplit;
  char *lastDelimiter = 0;
  char delim[2];
  delim[0] = delimiter;
  delim[1] = 0;

  /* Count how many elements will be extracted. */
  while (*tmp)
  {
    if (delimiter == *tmp)
    {
      count++;
      lastDelimiter = tmp;
    }
    tmp++;
  }

  /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
  count++;

  result = malloc(sizeof(char *) * count);

  if (result)
  {
    int index = 0;
    char *token = strtok(toSplit, delim);

    while (token)
    {
      *(result + index++) = strdup(token);
      token = strtok(0, delim);
    }
    *(result + index) = 0;
  }

  *(length) = count;

  return result;
}

char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while (isspace((unsigned char)*str))
    str++;

  if (*str == 0) // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while (end > str && isspace((unsigned char)*end))
    end--;

  // Write new null terminator character
  end[1] = '\0';

  return str;
}

void processLine(char *line, int interval[2], int *mappedSequences)
{
  line = trimwhitespace(line);
  char *substringPointer = strstr(storedReference, line);

  if (substringPointer != NULL)
  {
    int position = substringPointer - storedReference;
    interval[0] = position;
    interval[1] = position + strlen(line);
    *(mappedSequences) += 1;
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
    printf("Received sequence of %lu length\n", strlen(uploadedSequence));
    int linesCount = 0;
    char **splitted = split(uploadedSequence, '\n', &linesCount);
    int intervals[linesCount][2];
    bzero(uploadedSequence, BUFF_SIZE);

    int i = 0;
    int mappedSequences = 0;
#pragma omp parallel default(none) shared(i, splitted, intervals, linesCount, mappedSequences)
    {
#pragma omp for
      for (int i = 0; i < linesCount; i++)
      {
        processLine(*(splitted + i), intervals[i], &mappedSequences);
      }
    }

    int coverage = intervalsCoverage(intervals, linesCount);
    float percentage = (float)coverage / (float)strlen(storedReference) * 100;
    printf("%d characters covering %f%% of the genoma reference\n", coverage, percentage);
    printf("%d sequencias mapeadas\n", mappedSequences);
    printf("%d sequencias no mapeadas\n", linesCount - mappedSequences);
  }
}

void receiver(int sockfd)
{
  while (1)
  {
    char chunk[CHUNK_SIZE];
    bzero(chunk, CHUNK_SIZE);
    recv(sockfd, chunk, sizeof(chunk), 0);

    if (strcmp(chunk, UPLOAD_REFERENCE) == 0)
    {
      char reference[BUFF_SIZE];
      bzero(reference, BUFF_SIZE);
      bzero(chunk, CHUNK_SIZE);
      int finished = 0;
      while (!finished)
      {
        bzero(chunk, CHUNK_SIZE);
        recv(sockfd, chunk, sizeof(chunk), 0);
        if (strcmp(chunk, FINISHED_BUFFER) == 0)
          finished = 1;
        else
          strcat(reference, chunk);
      }
      uploadReference(reference);
    }
    else if (strcmp(chunk, UPLOAD_SEQUENCE) == 0)
    {
      char sequence[BUFF_SIZE];
      bzero(sequence, BUFF_SIZE);
      bzero(chunk, CHUNK_SIZE);
      int finished = 0;
      while (!finished)
      {
        bzero(chunk, CHUNK_SIZE);
        recv(sockfd, chunk, sizeof(chunk), 0);
        if (strcmp(chunk, FINISHED_BUFFER) == 0)
          finished = 1;
        else
          strcat(sequence, chunk);
      }
      uploadSequence(sequence);
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
