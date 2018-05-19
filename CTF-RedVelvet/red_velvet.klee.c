#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include "defs.h"
#include <assert.h>
#include "./klee_src/include/klee/klee.h"

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
  signed __int64 v2; // rtt@1

  LODWORD(v2) = (char)(a2 ^ a1 ^ a2);
  HIDWORD(v2) = (unsigned __int64)(char)(a2 ^ a1 ^ a2) >> 32;
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

int main(int argc, const char **argv, const char **envp)
{
  char buf[27];
  size_t v6; // rax@2
  signed int i; // [sp+8h] [bp-1B8h]@2
  SHA256_CTX v11; // [sp+50h] [bp-170h]@2
  unsigned char v38[32]; // [sp+E0h] [bp-E0h]@2

  char s1[80]; // [sp+100h] [bp-C0h]@3
  char s2[8]; // [sp+150h] [bp-70h]@1
  *(_QWORD *)s2 = 3905859155515433264LL;
  char v41[8];
  *(_QWORD *)v41 = 3990529441497888818LL;
  char v42[8];
  *(_QWORD *)v42 = 7017565014431380534LL;
  char v43[8];
  *(_QWORD *)v43 = 3977633058323522358LL;
  char v44[8];
  *(_QWORD *)v44 = 7364290724313116725LL;
  char v45[8];
   *(_QWORD *)v45 = 3991705742141175652LL;
  char v46[8];
   *(_QWORD *)v46 = 7363494672811320633LL;
  char v47[8];
   *(_QWORD *)v47 = 3847534474596595814LL;
  char v48[8];
  *(_QWORD *)v48 = 0LL;
  char v49[8];
  *(_QWORD *)v49 = 0LL;
  char v50[8];
  *(_QWORD *)v50 = 0LL;
  char v51[8];
  *(_QWORD *)v51 = 0LL;
  char v52[8];
  *(_QWORD *)v52 = 0;

#ifdef KLEE
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
  SHA256_Init(&v11);
  v6 = strlen(buf);
  SHA256_Update(&v11, buf, v6);
  SHA256_Final(v38, &v11);
  for ( i = 0; i <= 31; ++i )
    sprintf(&s1[2 * i], "%02x", (unsigned __int8)v38[i]);
  if ( strcmp(s1, s2) )
    exit(1);
#ifdef KLEE
  klee_assert(0);
#else
  printf("flag : {\" %s \"}\n", buf);
#endif
  return 0;
}
