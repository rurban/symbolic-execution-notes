#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/opensslconf.h>
#include <openssl/sha.h>
#include "defs.h"
#include <assert.h>
#include "./klee_src/include/klee/klee.h"

/* openssl */
typedef void *(*memset_t)(void *, int, size_t);

static volatile memset_t memset_func = memset;

void OPENSSL_cleanse(void *ptr, size_t len)
{
    memset_func(ptr, 0, len);
}

#define HOST_l2c(l,c) ({ unsigned int r=(l);                  \
                                   asm ("bswapl %0":"=r"(r):"0"(r));    \
                                   *((unsigned int *)(c))=r; (c)+=4; r; })

static const SHA_LONG K256[64] = {
    0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL,
    0x3956c25bUL, 0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL,
    0xd807aa98UL, 0x12835b01UL, 0x243185beUL, 0x550c7dc3UL,
    0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL, 0xc19bf174UL,
    0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL,
    0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL,
    0x983e5152UL, 0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL,
    0xc6e00bf3UL, 0xd5a79147UL, 0x06ca6351UL, 0x14292967UL,
    0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL, 0x53380d13UL,
    0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
    0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL,
    0xd192e819UL, 0xd6990624UL, 0xf40e3585UL, 0x106aa070UL,
    0x19a4c116UL, 0x1e376c08UL, 0x2748774cUL, 0x34b0bcb5UL,
    0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL, 0x682e6ff3UL,
    0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL,
    0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL
};

#define HOST_c2l(c,l) ({ unsigned int r=*((const unsigned int *)(c)); \
        asm ("bswapl %0":"=r"(r):"0"(r));    \
        (c)+=4; (l)=r;                       })

#define ROTATE(a,n)  ({ register unsigned int ret;   \
                                asm (                   \
                                "roll %1,%0"            \
                                : "=r"(ret)             \
                                : "I"(n), "0"((unsigned int)(a))        \
                                : "cc");                \
                           ret;                         \
                        })

#define Ch(x,y,z)       (((x) & (y)) ^ ((~(x)) & (z)))
#define Maj(x,y,z)      (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define Sigma1(x)       (ROTATE((x),26) ^ ROTATE((x),21) ^ ROTATE((x),7))
#define Sigma0(x)       (ROTATE((x),30) ^ ROTATE((x),19) ^ ROTATE((x),10))
#define sigma1(x)       (ROTATE((x),15) ^ ROTATE((x),13) ^ ((x)>>10))
#define sigma0(x)       (ROTATE((x),25) ^ ROTATE((x),14) ^ ((x)>>3))
#define SHA_LONG unsigned int
#define SHA_LBLOCK      16
#define SHA_CBLOCK      (SHA_LBLOCK*4)
#define HASH_CBLOCK     SHA_CBLOCK
#define HASH_CTX        SHA256_CTX
#define HASH_LONG SHA_LONG

#define HASH_MAKE_STRING(c,s)   do {    \
        unsigned long ll;               \
        unsigned int  nn;               \
        switch ((c)->md_len)            \
        {   case SHA224_DIGEST_LENGTH:  \
                for (nn=0;nn<SHA224_DIGEST_LENGTH/4;nn++)       \
                {   ll=(c)->h[nn]; (void)HOST_l2c(ll,(s));   }  \
                break;                  \
            case SHA256_DIGEST_LENGTH:  \
                for (nn=0;nn<SHA256_DIGEST_LENGTH/4;nn++)       \
                {   ll=(c)->h[nn]; (void)HOST_l2c(ll,(s));   }  \
                break;                  \
            default:                    \
                if ((c)->md_len > SHA256_DIGEST_LENGTH) \
                    return 0;                           \
                for (nn=0;nn<(c)->md_len/4;nn++)                \
                {   ll=(c)->h[nn]; (void)HOST_l2c(ll,(s));   }  \
                break;                  \
        }                               \
        } while (0)

#define HASH_BLOCK_DATA_ORDER   sha256_block_data_order
#define MD32_REG_T int

static void sha256_block_data_order(SHA256_CTX *ctx, const void *in,
                                    size_t num)
{
    unsigned MD32_REG_T a, b, c, d, e, f, g, h, s0, s1, T1, T2;
    SHA_LONG X[16], l;
    int i;
    const unsigned char *data = in;

    while (num--) {

        a = ctx->h[0];
        b = ctx->h[1];
        c = ctx->h[2];
        d = ctx->h[3];
        e = ctx->h[4];
        f = ctx->h[5];
        g = ctx->h[6];
        h = ctx->h[7];

        for (i = 0; i < 16; i++) {
            (void)HOST_c2l(data, l);
            T1 = X[i] = l;
            T1 += h + Sigma1(e) + Ch(e, f, g) + K256[i];
            T2 = Sigma0(a) + Maj(a, b, c);
            h = g;
            g = f;
            f = e;
            e = d + T1;
            d = c;
            c = b;
            b = a;
            a = T1 + T2;
        }

        for (; i < 64; i++) {
            s0 = X[(i + 1) & 0x0f];
            s0 = sigma0(s0);
            s1 = X[(i + 14) & 0x0f];
            s1 = sigma1(s1);

            T1 = X[i & 0xf] += s0 + s1 + X[(i + 9) & 0xf];
            T1 += h + Sigma1(e) + Ch(e, f, g) + K256[i];
            T2 = Sigma0(a) + Maj(a, b, c);
            h = g;
            g = f;
            f = e;
            e = d + T1;
            d = c;
            c = b;
            b = a;
            a = T1 + T2;
        }

        ctx->h[0] += a;
        ctx->h[1] += b;
        ctx->h[2] += c;
        ctx->h[3] += d;
        ctx->h[4] += e;
        ctx->h[5] += f;
        ctx->h[6] += g;
        ctx->h[7] += h;

    }
}

int SHA256_Init(SHA256_CTX *c)
{
    memset(c, 0, sizeof(*c));
    c->h[0] = 0x6a09e667UL;
    c->h[1] = 0xbb67ae85UL;
    c->h[2] = 0x3c6ef372UL;
    c->h[3] = 0xa54ff53aUL;
    c->h[4] = 0x510e527fUL;
    c->h[5] = 0x9b05688cUL;
    c->h[6] = 0x1f83d9abUL;
    c->h[7] = 0x5be0cd19UL;
    c->md_len = SHA256_DIGEST_LENGTH;
    return 1;
}

int SHA256_Final(unsigned char *md, HASH_CTX *c)
{
    unsigned char *p = (unsigned char *)c->data;
    size_t n = c->num;

    p[n] = 0x80;                /* there is always room for one */
    n++;

    if (n > (HASH_CBLOCK - 8)) {
        memset(p + n, 0, HASH_CBLOCK - n);
        n = 0;
        HASH_BLOCK_DATA_ORDER(c, p, 1);
    }
    memset(p + n, 0, HASH_CBLOCK - 8 - n);

    p += HASH_CBLOCK - 8;
#if   defined(DATA_ORDER_IS_BIG_ENDIAN)
    (void)HOST_l2c(c->Nh, p);
    (void)HOST_l2c(c->Nl, p);
#elif defined(DATA_ORDER_IS_LITTLE_ENDIAN)
    (void)HOST_l2c(c->Nl, p);
    (void)HOST_l2c(c->Nh, p);
#endif
    p -= HASH_CBLOCK;
    HASH_BLOCK_DATA_ORDER(c, p, 1);
    c->num = 0;
    OPENSSL_cleanse(p, HASH_CBLOCK);

#ifndef HASH_MAKE_STRING
# error "HASH_MAKE_STRING must be defined!"
#else
    HASH_MAKE_STRING(c, md);
#endif

    return 1;
}

int SHA256_Update(HASH_CTX *c, const void *data_, size_t len)
{
    const unsigned char *data = data_;
    unsigned char *p;
    HASH_LONG l;
    size_t n;

    if (len == 0)
        return 1;

    l = (c->Nl + (((HASH_LONG) len) << 3)) & 0xffffffffUL;
    if (l < c->Nl)              /* overflow */
        c->Nh++;
    c->Nh += (HASH_LONG) (len >> 29); /* might cause compiler warning on
                                       * 16-bit */
    c->Nl = l;

    n = c->num;
    if (n != 0) {
        p = (unsigned char *)c->data;

        if (len >= HASH_CBLOCK || len + n >= HASH_CBLOCK) {
            memcpy(p + n, data, HASH_CBLOCK - n);
            HASH_BLOCK_DATA_ORDER(c, p, 1);
            n = HASH_CBLOCK - n;
            data += n;
            len -= n;
            c->num = 0;
            /*
             * We use memset rather than OPENSSL_cleanse() here deliberately.
             * Using OPENSSL_cleanse() here could be a performance issue. It
             * will get properly cleansed on finalisation so this isn't a
             * security problem.
             */
            memset(p, 0, HASH_CBLOCK); /* keep it zeroed */
        } else {
            memcpy(p + n, data, len);
            c->num += (unsigned int)len;
            return 1;
        }
    }

    n = len / HASH_CBLOCK;
    if (n > 0) {
        HASH_BLOCK_DATA_ORDER(c, data, n);
        n *= HASH_CBLOCK;
        data += n;
        len -= n;
    }

    if (len != 0) {
        p = (unsigned char *)c->data;
        c->num = (unsigned int)len;
        memcpy(p, data, len);
    }
    return 1;
}
/* end openssl */

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
