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
      return NewMatrix;
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
} /* End of 'MatrixFree' function */


/* Copies one matrix to another */
VOID MatrixCopy( MATRIX *MDest, MATRIX *MSrc )
{
  INT i, j;
  
  if (MDest->Size != MSrc->Size)
  {
  	MatrixFree(MDest);
  	*MDest = MatrixCreate(MSrc->Size);
  }
  
  MDest->Determinant = MSrc->Determinant;
  if (MSrc->Size > 0)
    for (i = 0; i < MSrc->Size; i++)
      for (j = 0; j < MSrc->Size; j++)
        MDest->Values[i][j] = MSrc->Values[i][j];
} /* End of 'MatrixCopy' function */


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
      printf("%8.3f", Matrix->Values[i][j]);
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


/* Counts time needed to calculate determinant of matrix from file IN.TXT */
VOID TimeTest( VOID )
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
  MATRIX Matrix = MatrixCreate(Size);
  MATRIX MatrixBuffer = MatrixCreate(Size);
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
  MATRIX Matrix = MatrixCreate(Size);
  MATRIX MatrixBuffer = MatrixCreate(Size);
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
  MATRIX Product = MatrixCreate(0);
  
  if (A->Size != B->Size)
    return Product;
  
  Product = MatrixCreate(A->Size);
  for (i = 0; i < A->Size; i++)
    for (j = 0; j < A->Size; j++)
    {
      Product.Values[i][j] = 0;
      for (k = 0; k < A->Size; k++)
        Product.Values[i][j] += A->Values[i][k] * B->Values[k][j];
    }
  
  return Product;
} /* End of 'MatrixMultiply' function */


/* Calculates algebraic complement and minor */
DOUBLE MatrixCalculateMinorAndSign( MATRIX *Matrix, INT i, INT j )
{
  MATRIX sMatrix = MatrixCreate(Matrix->Size - 1);
  INT si, sj, ni, nj;
  DOUBLE Determinant;
  
  for (ni = 0, si = 0; ni < Matrix->Size; si += ni != i, ni++)
    for (nj = 0, sj = 0; nj < Matrix->Size; sj += nj != j, nj++)
      if (ni != i && nj != j)
        sMatrix.Values[si][sj] = Matrix->Values[ni][nj];
  MatrixCalculateDeterminant(&sMatrix);
  Determinant = sMatrix.Determinant;
  MatrixFree(&sMatrix);
  
  return Determinant * pow(-1, i + j);
} /* End of 'MatrixCalculateMinorAndSign' function */


/* Calculates reverse matrix for provided one */
MATRIX MatrixFindReverse( MATRIX *Matrix )
{
  INT i, j, si, sj;
  MATRIX Reverse = MatrixCreate(Matrix->Size);
  MATRIX MatrixBuffer = MatrixCreate(Matrix->Size);
  
  MatrixCopy(&MatrixBuffer, Matrix);
  MatrixCalculateDeterminant(&MatrixBuffer);
  for (i = 0; i < Matrix->Size; i++)
    for (j = 0; j < Matrix->Size; j++)
      Reverse.Values[j][i] = MatrixCalculateMinorAndSign(Matrix, i, j) / MatrixBuffer.Determinant;
  MatrixFree(&MatrixBuffer);
  
  return Reverse;
} /* End of 'MatrixFindReverse' function */


/* Generates matrix filled with random values */
MATRIX MatrixGenerateRandomMatrix( INT Size )
{
  MATRIX Matrix = MatrixCreate(Size);
  INT i, j;
  
  srand(clock());
  for (i = 0; i < Size; i++)
    for (j = 0; j < Size; j++)
      Matrix.Values[i][j] = (rand() % 11) - 5 + (rand() % 1000) / 1000.0;
  
  return Matrix;
} /* End of 'MatrixGenerateRandom' function */


INT main( VOID )
{
	/* FindMatrix(12, 3740374037); */
	/* TimeTest(); */
	MATRIX Matrix = MatrixGenerateRandomMatrix(4);
	MATRIX Reverse = MatrixFindReverse(&Matrix);
	MATRIX Product = MatrixMultiply(&Matrix, &Reverse);
	
	MatrixPrint(&Matrix);
	MatrixPrint(&Reverse);
	MatrixPrint(&Product);
	
	MatrixFree(&Matrix);
	MatrixFree(&Reverse);
	MatrixFree(&Product);
	
	return 0;
}
/* END OF 't06detg.c' FILE */