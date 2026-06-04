/* FILE NAME: t04perm.c
 * PROGRAMMER: TM5
 * DATE: 04.06.2026
 * PURPOSE: Print all possible variants.
 */
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#define NUM_SIZE 7

INT Numbers[NUM_SIZE];

VOID Swap( INT *a, INT *b )
{
  INT t = *a;
  *a = *b;
  *b = t;
}

INT IsEven( VOID )
{
  INT i, j, Disorders = 0;

  for (i = 0; i < NUM_SIZE; i++)
    for (j = i + 1; j < NUM_SIZE; j++)
      Disorders += Numbers[i] > Numbers[j];

  return Disorders % 2;
}

VOID PrintNums( VOID )
{
  static Count = 0;
  INT i;

  FILE *File = fopen("Positions.txt", "a");
  if (File == NULL)
    return;

  Count++;
  fprintf(File, "%.2i: [", Count);
  printf("%.2i: [", Count);
  for (i = 0; i < NUM_SIZE - 1; i++)
  {
    fprintf(File, "%i, ", Numbers[i]);
    printf("%i, ", Numbers[i]);
  }
  fprintf(File, "%i] - %s\n", Numbers[i], IsEven() ? "not even" : "even");
  printf("%i] - %s\n", Numbers[i], IsEven() ? "not even" : "even");

  fclose(File);
}

VOID For( INT *pNumbers, INT size )
{
  INT i;

  if (size <= 1)
    return;

  for (i = 0; i < size; i++)
  {
    Swap(pNumbers, pNumbers + i);
    if (size < 3)
      PrintNums();
    For(pNumbers + 1, size - 1);
    Swap(pNumbers, pNumbers + i);
  }
}

VOID ClearFile( VOID )
{
  FILE *File = fopen("Positions.txt", "w");
  if (File != NULL)
    fclose(File);
}

VOID main( VOID )
{
  INT i;

  for (i = 0; i < NUM_SIZE; i++)
    Numbers[i] = i + 1;

  ClearFile();
  For(Numbers, NUM_SIZE);
  
  _getch();
}