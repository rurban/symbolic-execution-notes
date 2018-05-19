#include <assert.h>
#include "./klee_src/include/klee/klee.h"

int main(int argc, char* argv[]) 
{
  int x, y, z;

  klee_make_symbolic(&x, sizeof x, "x"); 
  klee_make_symbolic(&y, sizeof y, "y"); 
  klee_make_symbolic(&z, sizeof z, "z");

  if (x - 2 * y + 3 * z != 7) return 0;
  if (2 * x + y + z != 4) return 0;
  if (-3 * x + 2 * y - 2 * z != -10) return 0;

  klee_assert(0);
}
