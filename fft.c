#include <math.h>
#include <complex.h>
#include "fft.h"
#define SWAP(a, b, type) \
  do{type tmp = (a); (a) = (b); (b) = tmp; }while(0)


void bitreverse2(cmplx *restrict z, int n)
{
    int u, d, rev;
    for (int i = 0; i < n; i++)
    {
        u = 1;
        d = n >> 1;
        rev = 0;
        while (u < n)
        {
            rev += d*((i&u)/u);
            u <<= 1;
            d >>= 1;
        }
        if (rev >= i)
        {
            SWAP(z[i], z[rev], cmplx);
        }
    }
}


void inplace_fft(cmplx *restrict z, int n)
{

    bitreverse2(z, n);

    real cos_arr[n*sizeof(real)/2];
    _fill_cosarr(cos_arr, n);

    cmplx even, odd;
    cmplx exp;
    real cosval, sinval;
    int numberofblocks;
    for (int blocksize = 2; blocksize <= n; blocksize *= 2)
    {
        numberofblocks = n/blocksize;
        for (int j = 0; j < n; j += blocksize)
        {
            for (int i = 0; i < blocksize/2; i++)
            {

                cosval = cos_arr[i*numberofblocks];
                sinval = (i*numberofblocks < n/4)?
                         (-cos_arr[i*numberofblocks + n/4]):
                         ( cos_arr[i*numberofblocks - n/4]);

                /*Get even and odd elements*/
                even = z[j + i];
                odd = z[blocksize/2 + j + i];

                exp = cosval*creal(odd) - cimag(odd)*sinval
                      + (cosval*cimag(odd) + creal(odd)*sinval)*I;

                /* Butterfly */
                z[j + i] = even + exp;
                z[blocksize/2 + j + i] = even - exp;

            }
        }
    }
}


void frequencies (real *freq, int n)
{
    int half_n = (n % 2)? n/2 + 1: n/2;
    for (int i = 0; i < half_n; i++)
    {
        freq[i] = (real)i;
    }
    int k=-1;
    for (int j = n - 1; j >= half_n; j--)
    {
        freq[j] = (real)k;
        k--;
    }
}

