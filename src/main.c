#include <stdio.h>

#define BUFF_SIZE 16000000

char source[BUFF_SIZE + 1];

//Returns size of sequence
size_t readFile()
{
   FILE *fp = fopen("S._cerevisiae_processed.txt", "r");
   if (fp != NULL)
   {
      //file stored in memory
      size_t newLen = fread(source, sizeof(char), BUFF_SIZE, fp);
      if (ferror(fp) != 0)
      {
         fputs("Error reading file", stderr);
      }
      else
      {
         source[newLen++] = '\0'; /* Just to be safe. */
      }
      fclose(fp);
      return newLen;
   }
}

int main()
{

   printf("Hello, World!\n");

   return 0;
}
