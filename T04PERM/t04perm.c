/* FILE NAME: t04perm.c
 * PROGRAMMER: TM5
 * DATE: 04.06.2026
 * PURPOSE: Print all possible variants.
 */
#include <stdio.h>
#include <math.h>
#include <conio.h>
#define NUM_SIZE 4

int real_nums[NUM_SIZE];
int relocations_count = 0;

void Swap( int *a, int *b )
{
  int t = *a;
  *a = *b;
  *b = t;
}

void PrintNums( void )
{
  static count = 0;
  int i;

  FILE *file = fopen("Positions.txt", "a");
  if (file == NULL)
    return;

  count++;
  fprintf(file, "%.2i: [", count);
  printf("%.2i: [", count);
  for (i = 0; i < NUM_SIZE - 1; i++)
  {
    fprintf(file, "%i, ", real_nums[i]);
    printf("%i, ", real_nums[i]);
  }
  fprintf(file, "%i] - %s\n", real_nums[i], relocations_count % 2 == 0 ? "not even" : "even");
  printf("%i] - %s\n", real_nums[i], relocations_count % 2 == 0 ? "not even" : "even");

  fclose(file);
}

void For( int *nums, int size )
{
  int i;

  if (size <= 1)
    return;

  for (i = 0; i < size; i++)
  {
    Swap(nums, nums + i);
    relocations_count++;
    PrintNums();
    For(nums + 1, size - 1);
    Swap(nums, nums + i);
    relocations_count++;
  }
}

void main( void )
{
  int i;

  for (i = 0; i < NUM_SIZE; i++)
    real_nums[i] = i + 1;

  For(real_nums, NUM_SIZE);
  
  _getch();
}