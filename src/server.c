#include <stdio.h>
#include <string.h>
#include "constants.c"

char storedReference[BUFF_SIZE];

void uploadReference(char uploadedReference[])
{
  strcpy(storedReference, uploadedReference);
}

void uploadSequence(char uploadedSequence[])
{
  if (strlen(storedReference) == 0)
    printf("No reference has been previously uploaded\n");
  else
  {
    // TODO: Separe in array of lines
    // TODO: Process each line and compare
    puts(uploadedSequence);
  }
}

// TODO: Daemonize
