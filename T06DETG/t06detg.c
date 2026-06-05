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

struct tagMatrix
{
  DOUBLE **Values;
  DOUBLE Determinant;
  INT Size;
};
typedef struct tagMatrix MATRIX;

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
}

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
}

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
}

VOID Swap( DOUBLE *a, DOUBLE *b )
{
  DOUBLE t = *a;
  *a = *b;
  *b = t;
}

VOID MatrixSwapVertical( MATRIX *Matrix, INT a, INT b )
{
  INT i;

  for (i = 0; i < Matrix->Size; i++)
    Swap(&(Matrix->Values[i][a]), &(Matrix->Values[i][b]));
}

VOID MatrixSwapHorizontal( MATRIX *Matrix, INT a, INT b )
{
  INT i;

  for (i = 0; i < Matrix->Size; i++)
    Swap(&(Matrix->Values[a][i]), &(Matrix->Values[b][i]));
}

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
}

VOID MatrixAddLineToLine( MATRIX *Matrix, INT from, INT to, DOUBLE Coefficent )
{
  INT i;

  for (i = 0; i < Matrix->Size; i++)
  {
    Matrix->Values[to][i] = Matrix->Values[to][i] + Matrix->Values[from][i] * Coefficent;
  }
}

VOID MatrixCalculateDeterminant( MATRIX *Matrix )
{
  POINT MaxPt;
  INT i, j;

  Matrix->Determinant = 1;
  for (i = 0; i < Matrix->Size; i++)
  {
    MaxPt = MatrixFindMax(Matrix, i, i);
    MatrixSwapVertical(Matrix, i, MaxPt.x);
    MatrixSwapHorizontal(Matrix, i, MaxPt.y);
    Matrix->Determinant *= Matrix->Values[i][i];
    Matrix->Determinant *= (i == MaxPt.x) * 2 - 1;
    Matrix->Determinant *= (i == MaxPt.y) * 2 - 1;

    if (Matrix->Values[i][i] == 0)
    {
      Matrix->Determinant = 0;
      return;
    }

    for (j = i + 1; j < Matrix->Size; j++)
      MatrixAddLineToLine(Matrix, i, j, -Matrix->Values[j][i] / Matrix->Values[i][i]);
  }
}

INT main( VOID )
{
  MATRIX Matrix = MatrixCreate(0);
  INT Time1, Time2, i, TotalClocks = 0, Iterations = 100000;

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
  _getch();
}