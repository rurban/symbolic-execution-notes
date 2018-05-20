//http://deniable.org/reversing/symbolic-execution
//usage: cbmc -DCBMC velvet.c --cover assertion

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
//#include "defs.h"
#include <assert.h>
#ifdef KLEE
# include "./klee_src/include/klee/klee.h"
#endif
#ifdef CBMC
# define exit(n)  __CPROVER_assert(0, "fail");
#endif

#define LODWORD(x) x
#define HIDWORD(x) x

int  func1(char a1, char a2)
{
  if ( a1 * 2 * (char)(a2 ^ a1) - a2 != 10858 )
    exit(1);
  if ( a1 <= 85 || a1 > 95 || a2 <= 96 || a2 > 111 )
    exit(1);
  return puts("HAPPINESS:)");
}

int  func2(char a1, char a2)
{
  if ( a1 % a2 != 7 )
    exit(1);
  if ( a2 <= 90 )
    exit(1);
  return puts("HAPPINESS:)");
}

int  func3(char a1, char a2)
{
  if ( a1 / a2 + (char)(a2 ^ a1) != 21 || a1 > 99 || a2 > 119 )
    exit(1);
  return puts("HAPPINESS:)");
}

int  func4(char a1, char a2)
{
  int64_t v2; // rtt@1

  LODWORD(v2) = (char)(a2 ^ a1 ^ a2);
  HIDWORD(v2) |= (uint64_t)(char)(a2 ^ a1 ^ a2) >> 32;
  if ( (unsigned int)(v2 % a2) + a1 != 137 || a1 <= 115 || a2 > 99 || a2 != 95 )
    exit(1);
  return puts("HAPPINESS:)");
}

int  func5(char a1, char a2)
{
  if ( ((a2 + a1) ^ (char)(a1 ^ a2 ^ a1)) != 225 || a1 <= 90 || a2 > 89 )
    exit(1);
  return puts("HAPPINESS:)");
}

int  func6(char a1, char a2, char a3)
{
  if ( a1 > a2 )
    exit(1);
  if ( a2 > a3 )
    exit(1);
  if ( a1 <= 85 || a2 <= 110 || a3 <= 115 || ((a2 + a3) ^ (a1 + a2)) != 44 || (a2 + a3) % a1 + a2 != 161 )
    exit(1);
  return puts("HAPPINESS:)");
}

int  func7(char a1, char a2, char a3)
{
  if ( a1 < a2 )
    exit(1);
  if ( a2 < a3 )
    exit(1);
  if ( a1 > 119 || a2 <= 90 || a3 > 89 || ((a1 + a3) ^ (a2 + a3)) != 122 || (a1 + a3) % a2 + a3 != 101 )
    exit(1);
  return puts("HAPPINESS:)");
}

int  func8(char a1, char a2, char a3)
{
  if ( a1 > a2 )
    exit(1);
  if ( a2 > a3 )
    exit(1);
  if ( a3 > 114 || (a1 + a2) / a3 * a2 != 97 || (a3 ^ (a1 - a2)) * a2 != -10088 || a3 > 114 )
    exit(1);
  return puts("HAPPINESS:)");
}

int  func9(char a1, char a2, char a3)
{
  if ( a1 != a2 )
    exit(1);
  if ( a2 < a3 )
    exit(1);
  if ( a3 > 99 || a3 + a1 * (a3 - a2) - a1 != -1443 )
    exit(1);
  return puts("HAPPINESS:)");
}

int  func10(char a1, char a2, char a3)
{
  if ( a1 < a2 )
    exit(1);
  if ( a2 < a3 )
    exit(1);
  if ( a2 * (a1 + a3 + 1) - a3 != 15514 || a2 <= 90 || a2 > 99 )
    exit(1);
  return puts("HAPPINESS:)");
}

int  func11(char a1, char a2, char a3)
{
  if ( a2 < a1 )
    exit(1);
  if ( a1 < a3 )
    exit(1);
  if ( a2 <= 100 || a2 > 104 || a1 + (a2 ^ (a2 - a3)) - a3 != 70 || (a2 + a3) / a1 + a1 != 68 )
    exit(1);
  return puts("HAPPINESS:)");
}

int  func12(char a1, char a2, char a3)
{
  if ( a1 < a2 )
    exit(1);
  if ( a2 < a3 )
    exit(1);
  if ( a2 > 59 || a3 > 44 || a1 + (a2 ^ (a3 + a2)) - a3 != 111 || (a2 ^ (a2 - a3)) + a2 != 101 )
    exit(1);
  return puts("HAPPINESS:)");
}

int  func13(char a1, char a2, char a3)
{
  if ( a1 > a2 )
    exit(1);
  if ( a2 > a3 )
    exit(1);
  if ( a1 <= 40 || a2 <= 90 || a3 > 109 || a3 + (a2 ^ (a3 + a1)) - a1 != 269 || (a3 ^ (a2 - a1)) + a2 != 185 )
    exit(1);
  return puts("HAPPINESS:)");
}

int  func14(char a1, char a2, char a3)
{
  if ( a1 < a3 )
    exit(1);
  if ( a2 < a3 )
    exit(1);
  if ( a2 > 99 || a3 <= 90 || a1 + (a2 ^ (a2 + a1)) - a3 != 185 )
    exit(1);
  return puts("HAPPINESS:)");
}

int  func15(char a1, char a2, char a3)
{
  if ( a2 < a3 )
    exit(1);
  if ( a2 < a1 )
    exit(1);
  if ( a3 <= 95 || a2 > 109 || ((a2 - a1) * a2 ^ a3) - a1 != 1214 || ((a3 - a2) * a3 ^ a1) + a2 != -1034 )
    exit(1);
  return puts("HAPPINESS:)");
}

int main(void)
{
  char buf[27];

#ifdef CBMC
  for (int i=0; i<27; i++) {
    buf[i] = nondet_uchar();
    __CPROVER_assume(buf[i] >= 26 && buf[i] <= 125);
    __CPROVER_input(i, buf[i]);
  }
#elif defined KLEE
  klee_make_symbolic(buf, sizeof(buf), "buf");
#else
  printf("Your flag : ");
  fgets(buf, 27, stdin);
#endif
  func1((unsigned int)buf[0], (unsigned int)buf[1]);
  func2((unsigned int)buf[1], (unsigned int)buf[2]);
  func3((unsigned int)buf[2], (unsigned int)buf[3]);
  func4((unsigned int)buf[3], (unsigned int)buf[4]);
  func5((unsigned int)buf[4], (unsigned int)buf[5]);
  func6((unsigned int)buf[5], (unsigned int)buf[6], (unsigned int)buf[7]);
  func7((unsigned int)buf[7], (unsigned int)buf[8], (unsigned int)buf[9]);
  func8((unsigned int)buf[9], (unsigned int)buf[10], (unsigned int)buf[11]);
  func9((unsigned int)buf[11], (unsigned int)buf[12], (unsigned int)buf[13]);
  func10((unsigned int)buf[13], (unsigned int)buf[14], (unsigned int)buf[15]);
  func11((unsigned int)buf[15], (unsigned int)buf[16], (unsigned int)buf[17]);
  func12((unsigned int)buf[17], (unsigned int)buf[18], (unsigned int)buf[19]);
  func13((unsigned int)buf[19], (unsigned int)buf[20], (unsigned int)buf[21]);
  func14((unsigned int)buf[21], (unsigned int)buf[22], (unsigned int)buf[23]);
  func15((unsigned int)buf[23], (unsigned int)buf[24], (unsigned int)buf[25]);

#ifdef CBMC
  __CPROVER_assert(1, "found");
#elif defined KLEE
  klee_assert(0);
#else
  printf("flag : {\" %s \"}\n", buf);
#endif
  return 0;
}
