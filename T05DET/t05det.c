/* FILE NAME: t04perm.c
 * PROGRAMMER: TM5
 * DATE: 04.06.2026
 * PURPOSE: Print all possible variants.
 */
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#define MATRIX_MAX_SIZE 6

INT Numbers[MATRIX_MAX_SIZE];
INT Matrix[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE];
INT Determinant = 0;
INT MatrixSize;

VOID PrintNums( VOID )
{
  INT i;

  printf("[");
  for (i = 0; i < MatrixSize - 1; i++)
    printf("%i, ", Numbers[i]);
  printf("%i]\n", Numbers[i]);
}

VOID Swap( INT *a, INT *b )
{
  INT t = *a;
  *a = *b;
  *b = t;
}

INT IsEven( VOID )
{
  INT i, j, Disorders = 0;

  for (i = 0; i < MatrixSize; i++)
    for (j = i + 1; j < MatrixSize; j++)
      Disorders += Numbers[i] > Numbers[j];

  return !(Disorders % 2);
}

VOID For( INT *pNumbers, INT size )
{
  INT i, j, PDeterminant;

  if (size <= 1)
    return;

  for (i = 0; i < size; i++)
  {
    Swap(pNumbers, pNumbers + i);
    if (size < 3)
    {
      PDeterminant = 1;
      for (j = 0; j < MatrixSize; j++)
        PDeterminant *= Matrix[j][Numbers[j] - 1];
      Determinant += PDeterminant * IsEven();
    }
    For(pNumbers + 1, size - 1);
    Swap(pNumbers, pNumbers + i);
  }
}

INT main( VOID )
{
  INT i, j;
  FILE *fMatrix = fopen("IN.TXT", "r");
  if (fMatrix == NULL)
    return 0;

  for (i = 0; i < MATRIX_MAX_SIZE; i++)
    Numbers[i] = i + 1;

  fscanf(fMatrix, "%i", &MatrixSize);
  if (MatrixSize > MATRIX_MAX_SIZE)
    return 0;

  for (i = 0; i < MatrixSize; i++)
    for (j = 0; j < MatrixSize; j++)
      fscanf(fMatrix, "%i", &Matrix[i][j]);

  for (i = 0; i < MatrixSize; i++)
  {
    for (j = 0; j < MatrixSize; j++)
      printf("%i ", Matrix[i][j]);
    printf("\n");
  }

  fclose(fMatrix);  

  For(Numbers, MatrixSize);
  printf("Det: %i", Determinant);

  _getch();
}