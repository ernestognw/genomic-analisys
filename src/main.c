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
int findInString(int start, int end, char *sequence, char *reference, size_t reference_length, size_t sequence_length)
{
   // Returns -1 if substring not found
   // Returns location if found

   //En español ---> Para verificar que la secuencia esxista en el genoma del texto referencia y su sposición
   // FATLTA PARALELIZAR. CADA FOR SE PUEDE PARALELIZAR.
   for (int i = start; i <= end; i++) //bloque de texto
   {
      for (int j = 0; j < sequence_length; j++) //secuence
      {
         if (sequence[j] == reference[i + j])
         {
            if (j == sequence_length - 1)
            {
               return i;
            }
            continue;
         }
         else
         {
            break;
         }
      }
      return -1;
   }
}

int findLocation(char *sequence, char *reference, size_t reference_length, size_t sequence_length)
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
      position = findInString(start, end, sequence, reference, reference_length, sequence_length);
      if (position != -1)
      {
         return position;
      }
   }
}
//TODO: sumar size de bloques anteriores, para la posicion
//TODO: Que pasa si la secuencia se queda entre dos bloques, hacer otra funcion que se le regresa la i (posicion inicial), la secuencia que se esta coomparando y otra posicion que sea donde se quedo de la secuencia larga para seguir la comparacion.

int main()
{

   size_t reference_length = readDNA("S._cerevisiae_processed.txt", reference);
   // Lo tenemos que leer linea por linea, tiene que ser otra funcion
   // size_t sequence_length = readDNA("s_cerevisia_2021_03.seq", sequence);
   printf("%ld \n", reference_length);

   printf("%c", reference[1]);

   return 0;
}
