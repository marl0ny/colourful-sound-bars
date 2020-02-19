#include <math.h>
#include <complex.h>

#ifndef _NUMBER_TYPEDEFS
#define _NUMBER_TYPEDEFS

typedef double __complex__ cmplx;
typedef double real;

#endif

#ifndef _FFT_H
#define _FFT_H

/*
* Bit reverse the order in which the elements of a
* an array with type complex is arranged.
* Its length must be of power two.
*
* For example, an array with elements in binary form
* [0b00, 0b01, 0b10, 0b11]
* becomes
* [0b00, 0b10, 0b01, 0b11].
*
* z: the pointer to the complex array.
* n: the number of elements in the complex array.
*/
void bitreverse2(cmplx *restrict z, int n);

/*
* Iterative in Place Radix-2
* Cooley-Turkey Fast Fourier Transform Algorithm.
* Please note that the input array size must be a
* a power of two - there are no checks on if it isn't.
*
* z: the pointer to the complex array. This
* function replaces each element of z with its
* fft counterpart.
* n: the size of the complex array, which must be a 
* power of two.
* ---------------------------------------------------------------------------
* References:
*
* Wikipedia contributors. (2020). Cooley–Tukey FFT algorithm.
* In Wikipedia, The Free Encyclopedia.
* https://en.wikipedia.org/wiki/Cooley%E2%80%93Tukey_FFT_algorithm
*
* Press W. et al. (1992). Fast Fourier Transform.
* In Numerical Recipes in Fortran 77, chapter 12. Cambridge University Press.
* https://websites.pmc.ucsc.edu/~fnimmo/eart290c_17/NumericalRecipesinF77.pdf
*
* Weisstein, Eric W. (2020). Fast Fourier Transform. 
* In MathWorld--A Wolfram Web Resource. 
* http://mathworld.wolfram.com/FastFourierTransform.html 
*
*/
void inplace_fft(cmplx *restrict z, int n);

/*
* Obtain the corresponding frequencies of the fft output
*
* freq: an array of each frequency that correspond
* to each element of the fft output.
* n: the size of the fft output and therefore the
* size of the frequencies array as well.
*/
void frequencies (real *freq, int n);

static inline void _fill_cosarr(real *restrict cos_arr, int n)
{
    real tau = 6.2831853071795864;
    real invsqrt2 = 0.70710678118654752;
    real angle=tau/n;
    real c, s;
    cos_arr[0] = 1.0;
    cos_arr[n/8] = invsqrt2;
    cos_arr[n/4] = 0.0;
    cos_arr[3*n/8] = -invsqrt2;
    for (int i = 1; i < n/8; i++)
    {
        c = cos(i*angle);
        s = sin(i*angle);
        cos_arr[i] = c;
        cos_arr[n/4 - i] = s;
        cos_arr[n/4 + i] = -s;
        cos_arr[n/2 - i] = -c;
    }
}

#endif