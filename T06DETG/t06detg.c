/* FILE NAME: t06detg.c
 * PROGRAMMER: TM5
 * DATE: 05.06.2026
 * PURPOSE: Write efficent determinant calculation.
 */
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <time.h>

/* Matrix structure for holding matrixes */
struct tagMatrix
{
  DOUBLE **Values;
  DOUBLE Determinant;
  INT Size;
};
typedef struct tagMatrix MATRIX;


/* Creates MATRIX instance */
MATRIX MatrixCreate( INT Size )
{
  INT i;
  MATRIX NewMatrix;

  NewMatrix.Size = Size;
  NewMatrix.Determinant = 1;

  if (Size > 0)
  {
    NewMatrix.Values = malloc(sizeof(DOUBLE *) * Size);
    if (NewMatrix.Values == NULL)
    {
      NewMatrix.Size = 0;
      printf("ERROR: Couldn't create matrix.");
    }

    for (i = 0; i < Size; i++)
    {
      NewMatrix.Values[i] = malloc(sizeof(DOUBLE) * Size);
      if (NewMatrix.Values[i] == NULL)
      {
        NewMatrix.Size = 0;
        printf("ERROR: Couldn't create matrix.");
      }
    }
  };

  return NewMatrix;
} /* End of 'MatrixCreate' function */


MATRIX MatrixCopy( MATRIX *Matrix )
{
  INT i, j;
  MATRIX NewMatrix = MatrixCreate(Matrix->Size);
  
  NewMatrix.Determinant = Matrix->Determinant;
  if (NewMatrix.Size > 0)
  {
    for (i = 0; i < Matrix->Size; i++)
      for (j = 0; j < Matrix->Size; j++)
        NewMatrix.Values[i][j] = Matrix->Values[i][j];
  }

  return NewMatrix;
} /* End of 'MatrixCreate' function */


/* Releases resources used by MATRIX instance */
VOID MatrixFree( MATRIX *Matrix )
{
  INT i;
  if (Matrix->Size == 0)
    return;

  for (i = 0; i < Matrix->Size; i++)
    free(Matrix->Values[i]);
  free(Matrix->Values);
  Matrix->Size = 0;
}


/* Loads values into MATRIX instance from file */
VOID MatrixLoad( MATRIX *Matrix, CHAR *FileName )
{
  INT Size, i, j;
  FILE *MatrixFile = fopen(FileName, "r");
  if (MatrixFile == NULL)
  {
    Matrix->Size = 0;
    printf("ERROR: Couldn't open file \"%s\".", FileName);
    return;
  }

  fscanf(MatrixFile, "%i", &Size);
  MatrixFree(Matrix);
  *Matrix = MatrixCreate(Size);

  for (i = 0; i < Size; i++)
    for (j = 0; j < Size; j++)
      fscanf(MatrixFile, "%lf", &Matrix->Values[i][j]);

  fclose(MatrixFile);
} /* End of 'MatrixLoad' function */


/* Prints MATRIX instance */
VOID MatrixPrint( MATRIX *Matrix )
{
  int i, j;

  for (i = 0; i < Matrix->Size; i++)
  {
    for (j = 0; j < Matrix->Size; j++)
      printf("%8.2f", Matrix->Values[i][j]);
    printf("\n");
  }
  printf("\n");
} /* End of 'MatrixPrint' function */


/* Swaps two double variables */
VOID Swap( DOUBLE *A, DOUBLE *B )
{
  DOUBLE T;
  
  T = *A;
  *A = *B;
  *B = T;
} /* End of 'Swap' function */


/* Swaps provided columns in MATRIX instance */
VOID MatrixSwapVertical( MATRIX *Matrix, INT A, INT B )
{
  INT i;

  for (i = 0; i < Matrix->Size; i++)
    Swap(&(Matrix->Values[i][A]), &(Matrix->Values[i][B]));
} /* End of 'MatrixSwapVertical' function */


/* Swaps provided rows in MATRIX instance */
VOID MatrixSwapHorizontal( MATRIX *Matrix, INT A, INT B )
{
  INT i;

  for (i = 0; i < Matrix->Size; i++)
    Swap(&(Matrix->Values[A][i]), &(Matrix->Values[B][i]));
} /* End of 'MatrixSwapHorizontal' function */


/* Finds max matrix element by absolute value */
POINT MatrixFindMax( MATRIX *Matrix, INT hOffset, INT vOffset )
{
  INT i, j;
  POINT Point = {hOffset, vOffset};

  for (i = vOffset; i < Matrix->Size; i++)
    for (j = hOffset; j < Matrix->Size; j++)
      if (fabs(Matrix->Values[Point.y][Point.x]) < fabs(Matrix->Values[i][j]))
      {
        Point.y = i;
        Point.x = j;
      }

  return Point;
} /* End of 'MatrixFindMax' function */


/* Sums two rows using provided coefficent */
VOID MatrixAddLineToLine( MATRIX *Matrix, INT from, INT to, DOUBLE Coefficent )
{
  INT i;

  for (i = 0; i < Matrix->Size; i++)
  {
    Matrix->Values[to][i] = Matrix->Values[to][i] + Matrix->Values[from][i] * Coefficent;
  }
} /* End of 'MatrixAddLineToLine' function */


/* Calculates matrix determinant */
VOID MatrixCalculateDeterminant( MATRIX *Matrix )
{
  POINT MaxPt;
  INT i, j;

  Matrix->Determinant = 1;
  for (i = 0; i < Matrix->Size; i++)
  {
    /* Getting largest by absolute value elements on the diagonal */
    MaxPt = MatrixFindMax(Matrix, i, i);
    MatrixSwapVertical(Matrix, i, MaxPt.x);
    MatrixSwapHorizontal(Matrix, i, MaxPt.y);
    Matrix->Determinant *= Matrix->Values[i][i];
    Matrix->Determinant *= (i == MaxPt.x) * 2 - 1;
    Matrix->Determinant *= (i == MaxPt.y) * 2 - 1;

    /* Ending soon if the largest element is zero */
    if (Matrix->Values[i][i] == 0)
    {
      Matrix->Determinant = 0;
      return;
    }

    /* Zeroing columns to make a triangular matrix */
    for (j = i + 1; j < Matrix->Size; j++)
      MatrixAddLineToLine(Matrix, i, j, -Matrix->Values[j][i] / Matrix->Values[i][i]);
  }
} /* End of 'MatrixCalculateDeterminant' function */

VOID TimerTest( VOID )
{
  MATRIX Matrix = MatrixCreate(0);
  INT Time1, Time2, i, TotalClocks = 0, Iterations = 10000;

  for (i = 0; i < Iterations; i++)
  {
    MatrixLoad(&Matrix, "IN.TXT");
    Time1 = clock();
    MatrixCalculateDeterminant(&Matrix);
    Time2 = clock();
    TotalClocks += Time2 - Time1;
  }

  MatrixPrint(&Matrix);
  printf("Determinant: %.2f\n", Matrix.Determinant);
  printf("Time: %.8f microseconds", (DOUBLE)(TotalClocks) / CLOCKS_PER_SEC * 1000000 / Iterations);
}

BOOL StringHas( CHAR *String, CHAR *Has, INT StringSize, INT HasSize )
{
  INT i, j;
  BOOL IsHere;

  for (i = 0; i < StringSize - HasSize; i++)
  {
    for (j = 0, IsHere = TRUE; j < HasSize; j++)
      if (String[i + j] != Has[j])
        IsHere = FALSE;

    if (IsHere == TRUE)
      return TRUE;
  }

  return FALSE;
}

VOID FindMatrix( INT Size, INT Determinant )
{
  MATRIX Matrix = MatrixCreate(Size);
  MATRIX MatrixBuffer = MatrixCreate(0);
  INT i, j;
  CHAR DeterminantString[256];

  while (1)
  {
    for (i = 0; i < Size; i++)
      for (j = 0; j < Size; j++)
        Matrix.Values[i][j] = (rand() % 11) - 5 + (rand() % 1000) / 1000.0;
    
    MatrixFree(&MatrixBuffer);
    MatrixBuffer = MatrixCopy(&Matrix);
    MatrixCalculateDeterminant(&Matrix);
    if (StringHas(DeterminantString, "4730", sprintf(DeterminantString, "%.0f", Matrix.Determinant), 8))
    {
      MatrixPrint(&MatrixBuffer);
      _getch();
    }                                     
  }
  printf("Determinant: %.2f\n", Matrix.Determinant);
}

INT main( VOID )
{
  GenerateRandomMatrix(10);
}
/* END OF 't06detg.c' FILE */