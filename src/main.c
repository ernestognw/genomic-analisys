#include <stdio.h>
#include <pthread.h>

#define BUFF_SIZE 16000000
#define NTHREADS 10

char reference[BUFF_SIZE];
//Esto es la listas de listas que son las secuencias
// Cambiar a lista enlazada?
// char sequence[2000][BUFF_SIZE / 2000];

//Returns size of sequence
//Esto sera funcion del cliente (leer el archivo para mandarlo al servidor)
size_t readDNA(char name[], char buff[])
{
   FILE *fp = fopen(name, "r");
   if (fp != NULL)
   {
      //file stored in memory
      size_t newLen = fread(buff, sizeof(char), BUFF_SIZE - 1, fp);
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
int findInString(int start, int end, char *sequence, char *reference, size_t reference_length)
{
   // Returns -1 if substring not found
   // Returns location if found
   printf("en el findString");

   for (int i = start; i <= end)
   {
      for (int j = 0; j)
   }
}

int findLocation(char *sequence, char *reference, size_t reference_length)
{
   int position;
   int block_size = reference_length / NTHREADS;
   for (int i = 0; i < NTHREADS; i++)
   {
      //block position
      int start = i * block_size;
      int end = (i + 1) * block_size - 1;
      if (i == NTHREADS)
      {
         end = reference_length;
      }
      position = findInString(start, end, sequence, reference, reference_length);
      if (position != -1)
      {
         return position;
      }
   }
}

int main()
{

   size_t reference_length = readDNA("S._cerevisiae_processed.txt", reference);
   // Lo tenemos que leer linea por linea, tiene que ser otra funcion
   // size_t sequence_length = readDNA("s_cerevisia_2021_03.seq", sequence);

   return 0;
}
