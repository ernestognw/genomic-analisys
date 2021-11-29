#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "constants.c"

void getFileContent(char filename[], char content[])
{
  FILE *filePointer = fopen(filename, "r");

  if (filePointer == NULL)
  {
    printf("Error while opening the file");
    fclose(filePointer);
    return;
  }

  int i = 0;
  char character;
  while ((character = fgetc(filePointer)) != EOF)
    content[i++] = character;

  content[i] = '\0'; // End string

  fclose(filePointer);
}

void handleUploadReference(int sockfd)
{
  char filename[BUFF_SIZE];
  char reference[BUFF_SIZE];
  bzero(filename, sizeof(filename));
  bzero(reference, sizeof(reference));

  printf("Type the path to the file with the reference: ");
  scanf("%s", filename);

  getFileContent(filename, reference);

  if (strlen(reference) != 0)
  {
    write(sockfd, UPLOAD_REFERENCE, sizeof(UPLOAD_REFERENCE));
    sleep(1);
    char chunk[CHUNK_SIZE];
    bzero(chunk, sizeof(chunk));
    int length = strlen(reference);

    int i = 0;
    for (int j = 0; j < length; j++, i++)
    {
      if (i == CHUNK_SIZE)
      {
        puts("Sending...");
        write(sockfd, chunk, sizeof(chunk));
        i = 0;
      }
      else
        chunk[i] = reference[j];
    }

    if (i != 0)
      write(sockfd, chunk, sizeof(chunk));

    write(sockfd, FINISHED_BUFFER, sizeof(FINISHED_BUFFER));
  }
}

void handleUploadSequence(int sockfd)
{
  char filename[BUFF_SIZE];
  char sequence[BUFF_SIZE];
  bzero(filename, sizeof(filename));
  bzero(sequence, sizeof(sequence));

  printf("Type the path to the file with the sequence: ");
  scanf("%s", filename);

  getFileContent(filename, sequence);
  if (strlen(sequence) != 0)
  {
    write(sockfd, UPLOAD_SEQUENCE, sizeof(UPLOAD_SEQUENCE));
    sleep(1);
    char chunk[CHUNK_SIZE];
    bzero(chunk, sizeof(chunk));
    int length = strlen(sequence);

    int i = 0;
    for (int j = 0; j < length; j++, i++)
    {
      if (i == CHUNK_SIZE)
      {
        puts("Sending...");
        write(sockfd, chunk, sizeof(chunk));
        i = 0;
      }
      else
        chunk[i] = sequence[j];
    }

    if (i != 0)
      write(sockfd, chunk, sizeof(chunk));

    write(sockfd, FINISHED_BUFFER, sizeof(FINISHED_BUFFER));
  }
}

int menu(int sockfd)
{
  int finished = 0;
  int operation;

  while (!finished)
  {
    printf("============== Genomic Analysis ==============\n");
    printf("Type an option: \n");
    printf("1) Upload reference\n");
    printf("2) Upload sequence\n");
    printf("Press any other number to exit\n");
    printf("Option: ");
    scanf("%d", &operation);

    switch (operation)
    {
    case 1:
      handleUploadReference(sockfd);
      break;
    case 2:
      handleUploadSequence(sockfd);
      break;
    default:
      finished = 1;
    }
  }

  printf("============== Thanks! ==============\n");

  return 0;
}

int main()
{
  int sockfd, connfd;
  SA_IN servaddr, cli;

  // Socket create and verification
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1)
    exit(EXIT_FAILURE);
  bzero(&servaddr, sizeof(servaddr));

  // Assign IP, PORT
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  servaddr.sin_port = htons(PORT);

  // Connect the client socket to server socket
  if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
    exit(EXIT_FAILURE);

  menu(sockfd);
  close(sockfd);
}
