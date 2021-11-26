#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.c"
#include "server.c"

void getFileContent(char filename[], char content[])
{
  FILE *filePointer = fopen(filename, "r");

  if (filePointer == NULL)
  {
    perror("Error while opening the file.\n");
    exit(EXIT_FAILURE);
  }

  int i = 0;
  char character;
  while ((character = fgetc(filePointer)) != EOF)
    content[i++] = character;
  
  content[i] = '\0'; // End string

  fclose(filePointer);
}

void handleUploadReference()
{
  char filename[BUFF_SIZE];
  char reference[BUFF_SIZE];

  printf("Type the path to the file with the reference: ");
  scanf("%s", filename);

  getFileContent(filename, reference);

  // TODO: Replace with message sent via sockets when server is ready
  uploadReference(reference);
}

void handleUploadSequence()
{
  char filename[BUFF_SIZE];
  char sequence[BUFF_SIZE];

  printf("Type the path to the file with the sequence: ");
  scanf("%s", filename);

  getFileContent(filename, sequence);

  // TODO: Replace with message sent via sockets when server is ready
  uploadSequence(sequence);
}

int main()
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
      handleUploadReference();
      break;
    case 2:
      handleUploadSequence();
      break;
    default:
      finished = 1;
    }
  }

  printf("============== Thanks! ==============\n");

  return 0;
}
