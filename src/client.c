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
    perror("Error while opening the file");
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

  printf("Type the path to the file with the reference: ");
  scanf("%s", filename);

  getFileContent(filename, reference);

  write(sockfd, UPLOAD_REFERENCE, sizeof(UPLOAD_REFERENCE));
  write(sockfd, reference, sizeof(reference));
}

void handleUploadSequence(int sockfd)
{
  char filename[BUFF_SIZE];
  char sequence[BUFF_SIZE];

  printf("Type the path to the file with the sequence: ");
  scanf("%s", filename);

  getFileContent(filename, sequence);

  write(sockfd, UPLOAD_SEQUENCE, sizeof(UPLOAD_SEQUENCE));
  write(sockfd, sequence, sizeof(sequence));
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
