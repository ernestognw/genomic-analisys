#include <stdio.h>

#define BUFF_SIZE 16000000

char reference[BUFF_SIZE + 1];
char sequence[BUFF_SIZE + 1];

//Returns size of sequence
//Esto sera funcion del cliente (leer el archivo para mandarlo al servidor)
size_t readDNA(char *name, char *buff)
{
   FILE *fp = fopen(name, "r");
   if (fp != NULL)
   {
      //file stored in memory
      size_t newLen = fread(buff, sizeof(char), BUFF_SIZE, fp);
      if (ferror(fp) != 0)
      {
         fputs("Error reading file", stderr);
      }
      else
      {
         buff[newLen++] = '\0'; /* Just to be safe. */
      }
      fclose(fp);
      return newLen;
   }
}

int main()
{

   size_t reference_length = readDNA("S._cerevisiae_processed.txt", reference);
   size_t sequence_length = readDNA("s_cerevisia_2021_03.seq", sequence);

   return 0;
}
