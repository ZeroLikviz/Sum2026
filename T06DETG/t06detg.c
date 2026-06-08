/* FILE NAME: t06detg.c
 * PROGRAMMER: TM5
 * DATE: 05.06.2026
 * PURPOSE: Write efficent determinant calculation.
 */
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <time.h>

/* Matrix structure for holding matrixes */
struct tagMatrix
{
  DOUBLE **Values;
  DOUBLE Determinant;
  INT Rows;
  INT Cols;
};
typedef struct tagMatrix MATRIX;


/* Creates MATRIX instance */
MATRIX MatrixCreate( INT Rows, INT Cols )
{
  INT i;
  MATRIX NewMatrix;

  NewMatrix.Rows = Rows;
  NewMatrix.Cols = Cols;
  NewMatrix.Determinant = 1;

  if (Rows * Cols > 0)
  {
    NewMatrix.Values = malloc(sizeof(DOUBLE *) * Rows);
    if (NewMatrix.Values == NULL)
    {
      NewMatrix.Rows = 0;
      NewMatrix.Cols = 0;
      printf("ERROR: Couldn't create matrix.\n");
      return NewMatrix;
    }

    for (i = 0; i < Cols; i++)
    {
      NewMatrix.Values[i] = malloc(sizeof(DOUBLE) * Cols);
      if (NewMatrix.Values[i] == NULL)
      {
        NewMatrix.Rows = 0;
        NewMatrix.Cols = 0;
        printf("ERROR: Couldn't create matrix.\n");
      }
    }
  };

  return NewMatrix;
} /* End of 'MatrixCreate' function */


/* Releases resources used by MATRIX instance */
VOID MatrixFree( MATRIX *Matrix )
{
  INT i;
  if (Matrix->Rows * Matrix->Cols == 0)
    return;

  for (i = 0; i < Matrix->Rows; i++)
    free(Matrix->Values[i]);
  free(Matrix->Values);
  Matrix->Rows = 0;
  Matrix->Cols = 0;
} /* End of 'MatrixFree' function */


/* Copies one matrix to another */
VOID MatrixCopy( MATRIX *MDest, MATRIX *MSrc )
{
  INT i, j;
  
  if (MDest->Rows != MSrc->Rows || MDest->Cols != MSrc->Cols)
  {
  	MatrixFree(MDest);
  	*MDest = MatrixCreate(MSrc->Rows, MSrc->Cols);
  }
  
  MDest->Determinant = MSrc->Determinant;
  if (MSrc->Rows * MSrc->Cols > 0)
    for (i = 0; i < MSrc->Rows; i++)
      for (j = 0; j < MSrc->Cols; j++)
        MDest->Values[i][j] = MSrc->Values[i][j];
} /* End of 'MatrixCopy' function */


/* Loads values into MATRIX instance from file */
VOID MatrixLoadSquare( MATRIX *Matrix, CHAR *FileName )
{
  INT Size, i, j;
  FILE *MatrixFile = fopen(FileName, "r");
  if (MatrixFile == NULL)
  {
    Matrix->Rows = 0;
    Matrix->Cols = 0;
    printf("ERROR: Couldn't open file \"%s\".\n", FileName);
    return;
  }

  fscanf(MatrixFile, "%i", &Size);
  MatrixFree(Matrix);
  *Matrix = MatrixCreate(Size, Size);

  for (i = 0; i < Size; i++)
    for (j = 0; j < Size; j++)
      fscanf(MatrixFile, "%lf", &Matrix->Values[i][j]);

  fclose(MatrixFile);
} /* End of 'MatrixLoad' function */


/* Prints MATRIX instance */
VOID MatrixPrint( MATRIX *Matrix )
{
  int i, j;

  for (i = 0; i < Matrix->Rows; i++)
  {
    for (j = 0; j < Matrix->Cols; j++)
      printf("%8.4f", Matrix->Values[i][j]);
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

  for (i = 0; i < Matrix->Rows; i++)
    Swap(&(Matrix->Values[i][A]), &(Matrix->Values[i][B]));
} /* End of 'MatrixSwapVertical' function */


/* Swaps provided rows in MATRIX instance */
VOID MatrixSwapHorizontal( MATRIX *Matrix, INT A, INT B )
{
  INT i;

  for (i = 0; i < Matrix->Cols; i++)
    Swap(&(Matrix->Values[A][i]), &(Matrix->Values[B][i]));
} /* End of 'MatrixSwapHorizontal' function */


/* Finds max matrix element by absolute value */
POINT MatrixFindMax( MATRIX *Matrix, INT hOffset, INT vOffset )
{
  INT i, j;
  POINT Point = {hOffset, vOffset};

  for (i = vOffset; i < Matrix->Rows; i++)
    for (j = hOffset; j < Matrix->Cols; j++)
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

  for (i = 0; i < Matrix->Cols; i++)
  {
    Matrix->Values[to][i] = Matrix->Values[to][i] + Matrix->Values[from][i] * Coefficent;
  }
} /* End of 'MatrixAddLineToLine' function */


/* Calculates matrix determinant */
VOID MatrixCalculateDeterminant( MATRIX *Matrix )
{
  POINT MaxPt;
  INT i, j;

  if (Matrix->Rows != Matrix->Cols)
  {
    printf("ERROR: Trying to calculate determinant of nonsquare matrix.\n");
    return;
  }

  Matrix->Determinant = 1;
  for (i = 0; i < Matrix->Rows; i++)
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
    for (j = i + 1; j < Matrix->Rows; j++)
      MatrixAddLineToLine(Matrix, i, j, -Matrix->Values[j][i] / Matrix->Values[i][i]);
  }
} /* End of 'MatrixCalculateDeterminant' function */


/* Counts time needed to calculate determinant of matrix from file IN.TXT */
VOID TimeTest( VOID )
{
  MATRIX Matrix = MatrixCreate(0, 0);
  MATRIX MatrixBuffer = MatrixCreate(0, 0);
  INT Time1, Time2, i, TotalClocks = 0, Iterations = 10000;

  MatrixLoadSquare(&MatrixBuffer, "IN.TXT");
  for (i = 0; i < Iterations; i++)
  {
    MatrixCopy(&Matrix, &MatrixBuffer);
    Time1 = clock();
    MatrixCalculateDeterminant(&Matrix);
    Time2 = clock();
    TotalClocks += Time2 - Time1;
  }
  
  MatrixPrint(&MatrixBuffer);
  printf("Determinant: %.2f\n", Matrix.Determinant);
  printf("Time: %.8f microseconds", (DOUBLE)(TotalClocks) / CLOCKS_PER_SEC * 1000000 / Iterations);
  MatrixFree(&Matrix);
} /* End of 'TimeTest' function */


/* Checks if a string has provided substring */
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
} /* End of 'StringHas' function */


/* Tries to find a matrix with provided determinant searching through random matrixes */
VOID FindMatrix( INT Size, INT Determinant )
{
  MATRIX Matrix = MatrixCreate(Size, Size);
  MATRIX MatrixBuffer = MatrixCreate(Size, Size);
  INT i, j, HasSize;
  CHAR DeterminantString[256];
  CHAR HasString[256];
  HasSize = sprintf(HasString, "%i", Determinant);
  
  srand(clock());
  while (1)
  {
    /* Filling with random numbers */
    for (i = 0; i < Size; i++)
      for (j = 0; j < Size; j++)
        Matrix.Values[i][j] = (rand() % 11) - 5 + (rand() % 1000) / 1000.0;
    
    /* Calculating determinant */
    MatrixCopy(&MatrixBuffer, &Matrix);
    MatrixCalculateDeterminant(&Matrix);
    if (StringHas(DeterminantString, HasString, sprintf(DeterminantString, "%.0f", Matrix.Determinant), HasSize))
    {
      MatrixPrint(&MatrixBuffer);
      printf("Determinant: %.2f\n", Matrix.Determinant);
      _getch();
    }                                     
  }
  MatrixFree(&Matrix);
  MatrixFree(&MatrixBuffer);
} /* End of 'FindMatrix' function */


/* Approximates pow values for improved Matrix search */
DOUBLE PowApprox( DOUBLE X )
{
  return (-0.0027 * X * X * X) + (0.1070 * X * X) - (0.2421 * X) + 2.6650;
} /* End of 'PowApprox' function */


/* Tries to find a matrix with provided determinant through step by step improvements */
VOID FindMatrixApprox( INT Size, DOUBLE Determinant )
{
  MATRIX Matrix = MatrixCreate(Size, Size);
  MATRIX MatrixBuffer = MatrixCreate(Size, Size);
  INT GiveUpCounter = 20000 / Size;
  INT i, j;
  DOUBLE Rate = pow(10, -PowApprox(Size));
  
  /* Filling with random numbers */
  srand(clock());
  for (i = 0; i < Size; i++)
    for (j = 0; j < Size; j++)
      Matrix.Values[i][j] = (rand() % 11) - 5 + (rand() % 1000) / 1000.0;
  
  do
  {
    /* Getting every value closer to one needed for provided determinant */
    for (i = 0; i < Size; i++)
      for (j = 0; j < Size; j++)
      {
        MatrixCopy(&MatrixBuffer, &Matrix);
        MatrixCalculateDeterminant(&MatrixBuffer);
        Matrix.Determinant = MatrixBuffer.Determinant;
        Matrix.Values[i][j] -= (Determinant - Matrix.Determinant) * Rate;
      }
     
    GiveUpCounter--;
  } while (fabs(Determinant - Matrix.Determinant) > 10e-3 && GiveUpCounter > 0);
  
  if (GiveUpCounter == 0)
  {
    printf("Couldn't get to the Determinant %9.3f from matrix of size %ix%i\n", Determinant, Size, Size);
    printf("Determinant: %9.3f; Difference: %9.5f\n", Matrix.Determinant, fabs(Determinant - Matrix.Determinant));
  }
  else
  {
    printf("Determinant: %9.3f; Difference: %9.5f\n", Matrix.Determinant, fabs(Determinant - Matrix.Determinant));
    MatrixPrint(&Matrix);
  }
  MatrixFree(&Matrix);
  MatrixFree(&MatrixBuffer);
} /* End of 'FindMatrixApprox' function */


/* Multiplies two matrixes */
MATRIX MatrixMultiply( MATRIX *A, MATRIX *B )
{
  INT i, j, k;
  MATRIX Product = MatrixCreate(0, 0);
  
  if (A->Cols != B->Rows)
    return Product;
  
  Product = MatrixCreate(A->Rows, B->Cols);
  for (i = 0; i < A->Rows; i++)
    for (j = 0; j < B->Cols; j++)
    {
      Product.Values[i][j] = 0;
      for (k = 0; k < A->Cols; k++)
        Product.Values[i][j] += A->Values[i][k] * B->Values[k][j];
    }
  
  return Product;
} /* End of 'MatrixMultiply' function */


/* Calculates algebraic complement and minor */
DOUBLE MatrixCalculateMinor( MATRIX *Matrix, INT i, INT j )
{
  MATRIX sMatrix = MatrixCreate(Matrix->Rows - 1, Matrix->Cols - 1);
  INT si, sj, ni, nj;
  DOUBLE Determinant;
  
  for (ni = 0, si = 0; ni < Matrix->Rows; si += ni != i, ni++)
    for (nj = 0, sj = 0; nj < Matrix->Cols; sj += nj != j, nj++)
      if (ni != i && nj != j)
        sMatrix.Values[si][sj] = Matrix->Values[ni][nj];
  MatrixCalculateDeterminant(&sMatrix);
  Determinant = sMatrix.Determinant;
  MatrixFree(&sMatrix);
  
  return Determinant;
} /* End of 'MatrixCalculateMinorAndSign' function */


/* Calculates reverse matrix for provided one */
MATRIX MatrixFindReverse( MATRIX *Matrix )
{
  INT i, j;
  MATRIX Reverse = MatrixCreate(Matrix->Rows, Matrix->Cols);
  MATRIX MatrixBuffer = MatrixCreate(Matrix->Rows, Matrix->Cols);
  
  if (Matrix->Rows != Matrix->Cols)
  {
    printf("ERROR: Trying to calculate reverse matrix of nonsquare matrix.\n");
    return Reverse;
  }

  MatrixCopy(&MatrixBuffer, Matrix);
  MatrixCalculateDeterminant(&MatrixBuffer);
  for (i = 0; i < Matrix->Rows; i++)
    for (j = 0; j < Matrix->Cols; j++)
      Reverse.Values[j][i] = MatrixCalculateMinor(Matrix, i, j) * pow(-1, i + j) / MatrixBuffer.Determinant;
  MatrixFree(&MatrixBuffer);
  
  return Reverse;
} /* End of 'MatrixFindReverse' function */


/* Generates matrix filled with random values */
MATRIX MatrixGenerateRandomMatrix( INT Size )
{
  MATRIX Matrix = MatrixCreate(Size, Size);
  INT i, j;
  
  srand(clock());
  for (i = 0; i < Size; i++)
    for (j = 0; j < Size; j++)
      Matrix.Values[i][j] = (rand() % 11) - 5 + (rand() % 1000) / 1000.0;
  
  return Matrix;
} /* End of 'MatrixGenerateRandom' function */


INT main( VOID )
{
  /* FindMatrix(12, 1234); */
  /* FindMatrixApprox(15, 4730473047.5); */
  /*
  MATRIX Matrix = MatrixGenerateRandomMatrix(4);
  MATRIX Reverse = MatrixFindReverse(&Matrix);
  MATRIX Product = MatrixMultiply(&Matrix, &Reverse);

  MatrixPrint(&Matrix);
  MatrixPrint(&Reverse);
  MatrixPrint(&Product);

  MatrixFree(&Matrix);
  MatrixFree(&Reverse);
  MatrixFree(&Product);
  */

  TimeTest();
  _getch();

  return 0;
}
/* END OF 't06detg.c' FILE */