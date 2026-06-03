/* FILE NAME: t00guess.c
 * PROGRAMMER: TM5
 * DATE: 01.06.2026
 * PURPOSE: WinAPI first sample.
 */
#include <windows.h>
#include <stdio.h>

void main( void )
{
  int a = 0, b = 100, m = 50, answer = 0, q = 1;
  char s[255];
  
  MessageBox(NULL, "Come up with a number from 0 to 100", "Number Guesser", MB_OK | MB_ICONEXCLAMATION);
  
  while (1)
  {
    if (rand() % 2)
    {
      sprintf(s, "Is it bigger than %i?", m);
      q = 1;
    }
    else
    {
      sprintf(s, "Is it lesser than %i?", m);
      q = -1;
    }
      
    switch (MessageBox(NULL, s, "Number Guesser", MB_YESNOCANCEL | MB_ICONQUESTION))
    {
    case IDYES:
      answer = 1 * q;
      break;
    case IDNO:
      answer = -1 * q;
      break;
    case IDCANCEL:
      MessageBox(NULL, "Haha, I won!", "Glorious Number Guesser", MB_OK | MB_ICONINFORMATION);
      return;
    }
    if (answer > 0)
       a = m + 1;
    else
      b = m - 1;

    m = (a + b) / 2;
    
    if (a > b)
    {
      MessageBox(NULL, "You're a dirty lier!", "Angry Number Guesser", MB_OK | MB_ICONSTOP);
      return;
    }
  }
}