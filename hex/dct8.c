/*
 * jfdctfst.c
 *
 * Copyright (C) 1994-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains a fast, not so accurate integer implementation of the
 * forward DCT (Discrete Cosine Transform).
 *
 * A 2-D DCT can be done by 1-D DCT on each row followed by 1-D DCT
 * on each column.  Direct algorithms are also available, but they are
 * much more complex and seem not to be any faster when reduced to code.
 *
 * This implementation is based on Arai, Agui, and Nakajima's algorithm for
 * scaled DCT.  Their original paper (Trans. IEICE E-71(11):1095) is in
 * Japanese, but the algorithm is described in the Pennebaker & Mitchell
 * JPEG textbook (see REFERENCES section in file README).  The following code
 * is based directly on figure 4-8 in P&M.
 * While an 8-point DCT cannot be done in less than 11 multiplies, it is
 * possible to arrange the computation so that many of the multiplies are
 * simple scalings of the final outputs.  These multiplies can then be
 * folded into the multiplications or divisions by the JPEG quantization
 * table entries.  The AA&N method leaves only 5 multiplies and 29 adds
 * to be done in the DCT itself.
 * The primary disadvantage of this method is that with fixed-point math,
 * accuracy is lost due to imprecise representation of the scaled
 * quantization values.  The smaller the quantization table entry, the less
 * precise the scaled value, so this implementation does worse with high-
 * quality-setting files than with low-quality ones.
 */

/*#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"
#include "jdct.h"*/		/* Private declarations for DCT subsystem */

volatile int __attribute__((section(".mySection_par"))) startPar =  0x00 ;
volatile int __attribute__((section(".mySection_par_num"))) par_num =  0x03 ; //constant
volatile int __attribute__((section(".mySection_arr_str"))) arr_str =  0x00 ;
volatile int __attribute__((section(".mySection_arr_end"))) arr_end =  0x00 ;

/*
 * This module is specialized to the case DCTSIZE = 8.
 */
#define DCTSIZE  8
#if DCTSIZE != 8
  Sorry, this code only copes with 8x8 DCTs. // deliberate syntax err 
#endif


/* Scaling decisions are generally the same as in the LL&M algorithm;
 * see jfdctint.c for more details.  However, we choose to descale
 * (right shift) multiplication products as soon as they are formed,
 * rather than carrying additional fractional bits into subsequent additions.
 * This compromises accuracy slightly, but it lets us save a few shifts.
 * More importantly, 16-bit arithmetic is then adequate (for 8-bit samples)
 * everywhere except in the multiplications proper; this saves a good deal
 * of work on 16-bit-int machines.
 *
 * Again to save a few shifts, the intermediate results between pass 1 and
 * pass 2 are not upscaled, but are represented only to integral precision.
 *
 * A final compromise is to represent the multiplicative constants to only
 * 8 fractional bits, rather than 13.  This saves some shifting work on some
 * machines, and may also reduce the cost of multiplication (since there
 * are fewer one-bits in the constants).
 */

#define CONST_BITS  8


/* Some C compilers fail to reduce "FIX(constant)" at compile time, thus
 * causing a lot of useless floating-point operations at run time.
 * To get around this we use the following pre-calculated constants.
 * If you change CONST_BITS you may want to add appropriate values.
 * (With a reasonable C compiler, you can just rely on the FIX() macro...)
 */

#if CONST_BITS == 8
#define FIX_0_382683433    98		/* FIX(0.382683433) */
#define FIX_0_541196100    139		/* FIX(0.541196100) */
#define FIX_0_707106781    181		/* FIX(0.707106781) */
#define FIX_1_306562965    334		/* FIX(1.306562965) */
#else
#define FIX_0_382683433  FIX(0.382683433)
#define FIX_0_541196100  FIX(0.541196100)
#define FIX_0_707106781  FIX(0.707106781)
#define FIX_1_306562965  FIX(1.306562965)
#endif


/* We can gain a little more speed, with a further compromise in accuracy,
 * by omitting the addition in a descaling shift.  This yields an incorrectly
 * rounded result half the time...
 */
int RIGHT_SHIFT (int x, int n) {
	
	return x  >> n ;
}
//inaccurute descale 
#define DESCALE(x,n)  RIGHT_SHIFT(x, n)
//accurute descale 
//#define DESCALE(x,n)  RIGHT_SHIFT((x) + (ONE << ((n)-1)), n)

/* Multiply a DCTELEM variable by an INT32 constant, and immediately
 * descale to yield a DCTELEM result.
 */

#define MULTIPLY(var,const)  ((int) DESCALE((var) * (const), CONST_BITS))


/*
 * Perform the forward DCT on one block of samples.
 */

#define SIZEX 150
int data [SIZEX][DCTSIZE] = {
		{8 ,1 ,0 ,7 ,6 ,9 ,2 ,3,}, 
		{5 ,1 ,2 ,1 ,3 ,8 ,6 ,7,},
		{9 ,2 ,5 ,0 ,2 ,5 ,9 ,7,},
		{4 ,4 ,9 ,6 ,1 ,7 ,4 ,3,},
		{8 ,8 ,7 ,0 ,1 ,1 ,9 ,5,},
		{3 ,6 ,2 ,5 ,3 ,4 ,2 ,6,},
		{2 ,0 ,0 ,6 ,8 ,5 ,7 ,7,},
		{4 ,4 ,5 ,0 ,0 ,8 ,5 ,9,},
		{4 ,1 ,9 ,2 ,5 ,2 ,2 ,0,},
		{9 ,1 ,0 ,0 ,3 ,2 ,6 ,3,},
		{3 ,8 ,9 ,3 ,1 ,9 ,8 ,0,},
		{6 ,2 ,0 ,4 ,5 ,5 ,0 ,0,},
		{2 ,4 ,1 ,4 ,2 ,1 ,4 ,2,},
		{4 ,9 ,5 ,4 ,2 ,3 ,8 ,6,},
		{3 ,9 ,4 ,8 ,1 ,5 ,2 ,5,},
		{9 ,8 ,5 ,0 ,8 ,7 ,8 ,3,},
		{3 ,0 ,8 ,2 ,8 ,3 ,6 ,5,},
		{9 ,2 ,3 ,2 ,8 ,1 ,6 ,3,},
		{2 ,9 ,2 ,6 ,1 ,6 ,1 ,5,},
		{9 ,1 ,3 ,8 ,1 ,8 ,5 ,3,},
		{7 ,9 ,9 ,8 ,9 ,3 ,9 ,6,},
		{9 ,6 ,6 ,4 ,5 ,7 ,1 ,4,},
		{5 ,7 ,3 ,1 ,6 ,1 ,7 ,5,},
		{7 ,6 ,8 ,6 ,4 ,1 ,1 ,2,},
		{8 ,9 ,0 ,9 ,4 ,2 ,5 ,5,},
		{2 ,6 ,7 ,0 ,4 ,4 ,3 ,9,},
		{0 ,0 ,1 ,8 ,3 ,6 ,2 ,5,},
		{8 ,5 ,1 ,5 ,1 ,6 ,0 ,1,},
		{3 ,5 ,7 ,1 ,9 ,4 ,0 ,1,},
		{9 ,6 ,8 ,1 ,4 ,1 ,6 ,6,},
		{5 ,9 ,8 ,3 ,9 ,5 ,1 ,1,},
		{7 ,6 ,9 ,0 ,8 ,9 ,5 ,4,},
		{8 ,9 ,9 ,9 ,4 ,9 ,7 ,7,},
		{3 ,5 ,9 ,8 ,0 ,7 ,0 ,6,},
		{8 ,2 ,9 ,6 ,3 ,1 ,2 ,5,},
		{9 ,8 ,9 ,2 ,3 ,8 ,5 ,4,},
		{2 ,1 ,7 ,5 ,9 ,9 ,6 ,9,},
		{9 ,2 ,6 ,8 ,6 ,2 ,2 ,1,},
		{3 ,6 ,4 ,5 ,1 ,9 ,7 ,6,},
		{7 ,6 ,2 ,0 ,8 ,2 ,8 ,3,},
		{5 ,0 ,9 ,4 ,7 ,4 ,3 ,6,},
		{0 ,2 ,2 ,0 ,8 ,4 ,1 ,5,},
		{3 ,9 ,2 ,0 ,0 ,2 ,7 ,1,},
		{5 ,8 ,5 ,2 ,0 ,5 ,5 ,0,},
		{4 ,8 ,1 ,5 ,6 ,2 ,8 ,8,},
		{3 ,9 ,6 ,4 ,6 ,0 ,4 ,8,},
		{2 ,1 ,5 ,6 ,4 ,8 ,1 ,7,},
		{4 ,1 ,4 ,8 ,4 ,7 ,0 ,8,},
		{7 ,1 ,0 ,6 ,3 ,2 ,4 ,4,},
		{7 ,8 ,6 ,8 ,3 ,9 ,1 ,0,},
		{3 ,0 ,6 ,4 ,2 ,4 ,2 ,6,},
		{6 ,1 ,0 ,9 ,5 ,0 ,6 ,1,},
		{8 ,4 ,2 ,4 ,9 ,8 ,3 ,6,},
		{9 ,0 ,8 ,6 ,4 ,4 ,4 ,2,},
		{3 ,0 ,9 ,8 ,7 ,5 ,4 ,5,},
		{1 ,8 ,8 ,6 ,7 ,0 ,4 ,8,},
		{3 ,8 ,4 ,5 ,3 ,8 ,7 ,4,},
		{2 ,4 ,5 ,8 ,9 ,4 ,2 ,6,},
		{6 ,8 ,0 ,4 ,6 ,8 ,5 ,5,},
		{1 ,8 ,3 ,4 ,3 ,7 ,7 ,7,},
		{0 ,8 ,6 ,2 ,8 ,4 ,6 ,0,},
		{4 ,9 ,9 ,1 ,6 ,7 ,0 ,7,},
		{7 ,2 ,8 ,5 ,3 ,4 ,2 ,2,},
		{9 ,8 ,9 ,1 ,3 ,4 ,6 ,3,},
		{0 ,8 ,7 ,3 ,7 ,7 ,6 ,2,},
		{5 ,0 ,6 ,8 ,0 ,7 ,1 ,3,},
		{8 ,4 ,8 ,2 ,7 ,9 ,0 ,0,},
		{0 ,2 ,5 ,9 ,9 ,9 ,3 ,5,},
		{8 ,5 ,7 ,7 ,5 ,3 ,9 ,8,},
		{8 ,7 ,7 ,2 ,8 ,9 ,0 ,7,},
		{4 ,4 ,1 ,3 ,4 ,0 ,7 ,4,},
		{5 ,5 ,7 ,5 ,4 ,3 ,1 ,7,},
		{1 ,4 ,5 ,6 ,2 ,4 ,4 ,7,},
		{2 ,7 ,6 ,2 ,1 ,1 ,3 ,3,},
		{8 ,8 ,1 ,2 ,1 ,9 ,1 ,2,},
		{0 ,7 ,3 ,3 ,9 ,3 ,5 ,4,},
		{8 ,2 ,7 ,7 ,5 ,3 ,0 ,7,},
		{4 ,6 ,7 ,3 ,9 ,4 ,0 ,5,},
		{6 ,2 ,9 ,7 ,9 ,3 ,2 ,0,},
		{8 ,5 ,3 ,1 ,1 ,5 ,4 ,7,},
		{8 ,8 ,7 ,9 ,7 ,7 ,8 ,0,},
		{3 ,1 ,1 ,1 ,7 ,2 ,0 ,7,},
		{3 ,8 ,3 ,6 ,2 ,3 ,9 ,3,},
		{7 ,6 ,2 ,3 ,7 ,3 ,2 ,0,},
		{4 ,3 ,4 ,6 ,2 ,8 ,7 ,6,},
		{5 ,1 ,9 ,2 ,3 ,7 ,9 ,2,},
		{6 ,3 ,9 ,0 ,1 ,1 ,3 ,9,},
		{1 ,4 ,9 ,2 ,3 ,9 ,5 ,1,},
		{2 ,7 ,0 ,6 ,9 ,7 ,3 ,5,},
		{2 ,4 ,1 ,0 ,6 ,2 ,1 ,4,},
		{6 ,2 ,3 ,4 ,6 ,2 ,9 ,5,},
		{4 ,4 ,4 ,8 ,4 ,7 ,0 ,5,},
		{1 ,0 ,9 ,7 ,0 ,3 ,2 ,0,},
		{2 ,4 ,3 ,6 ,4 ,2 ,3 ,2,},
		{8 ,5 ,6 ,0 ,2 ,5 ,4 ,6,},
		{4 ,2 ,9 ,8 ,2 ,1 ,9 ,3,},
		{9 ,6 ,7 ,9 ,6 ,7 ,2 ,3,},
		{2 ,1 ,5 ,6 ,6 ,5 ,5 ,9,},
		{0 ,5 ,0 ,7 ,3 ,8 ,9 ,6,},
		{4 ,3 ,1 ,4 ,5 ,3 ,9 ,7,},
		{2 ,2 ,7 ,3 ,8 ,4 ,7 ,8,},
		{4 ,5 ,8 ,0 ,9 ,4 ,1 ,5,},
		{5 ,8 ,6 ,8 ,1 ,2 ,5 ,7,},
		{3 ,0 ,4 ,0 ,6 ,6 ,5 ,5,},
		{6 ,4 ,0 ,2 ,3 ,9 ,0 ,0,},
		{0 ,9 ,0 ,1 ,0 ,8 ,4 ,0,},
		{8 ,8 ,0 ,4 ,5 ,9 ,1 ,1,},
		{2 ,8 ,4 ,8 ,0 ,5 ,0 ,3,},
		{0 ,8 ,2 ,1 ,7 ,5 ,4 ,2,},
		{2 ,9 ,3 ,5 ,5 ,0 ,4 ,2,},
		{2 ,4 ,3 ,3 ,6 ,1 ,5 ,7,},
		{8 ,4 ,2 ,7 ,9 ,3 ,2 ,2,},
		{2 ,0 ,2 ,3 ,7 ,7 ,5 ,7,},
		{9 ,6 ,4 ,5 ,9 ,8 ,6 ,1,},
		{3 ,2 ,7 ,8 ,1 ,1 ,4 ,2,},
		{9 ,1 ,4 ,6 ,8 ,5 ,8 ,4,},
		{0 ,6 ,2 ,6 ,1 ,8 ,0 ,9,},
		{5 ,1 ,6 ,4 ,9 ,3 ,8 ,0,},
		{5 ,0 ,2 ,5 ,4 ,2 ,2 ,4,},
		{4 ,5 ,4 ,8 ,6 ,9 ,8 ,7,},
		{7 ,0 ,8 ,8 ,7 ,9 ,2 ,8,},
		{9 ,4 ,1 ,4 ,6 ,4 ,9 ,1,},
		{7 ,0 ,2 ,3 ,0 ,4 ,8 ,2,},
		{0 ,1 ,2 ,8 ,1 ,1 ,6 ,7,},
		{4 ,8 ,2 ,0 ,1 ,6 ,0 ,6,},
		{4 ,4 ,6 ,6 ,3 ,1 ,2 ,2,},
		{6 ,2 ,6 ,1 ,0 ,4 ,9 ,8,},
		{5 ,4 ,3 ,6 ,8 ,0 ,3 ,9,},
		{3 ,6 ,3 ,6 ,6 ,4 ,2 ,8,},
		{3 ,3 ,1 ,6 ,3 ,2 ,5 ,7,},
		{7 ,0 ,8 ,1 ,3 ,5 ,0 ,7,},
		{7 ,8 ,6 ,6 ,8 ,3 ,1 ,3,},
		{6 ,9 ,8 ,5 ,2 ,0 ,8 ,3,},
		{0 ,4 ,5 ,6 ,9 ,1 ,5 ,5,},
		{8 ,5 ,4 ,8 ,7 ,0 ,9 ,3,},
		{6 ,5 ,7 ,4 ,0 ,4 ,5 ,4,},
		{5 ,3 ,2 ,0 ,2 ,0 ,8 ,4,},
		{0 ,2 ,4 ,9 ,9 ,0 ,6 ,9,},
		{0 ,7 ,8 ,7 ,2 ,4 ,0 ,7,},
		{9 ,0 ,4 ,1 ,1 ,6 ,0 ,2,},
		{6 ,6 ,2 ,3 ,9 ,9 ,3 ,5,},
		{4 ,1 ,7 ,3 ,0 ,2 ,5 ,7,},
		{8 ,2 ,6 ,9 ,0 ,2 ,6 ,1,},
		{2 ,0 ,2 ,8 ,6 ,3 ,5 ,3,},
		{7 ,5 ,6 ,1 ,4 ,9 ,7 ,1,},
		{0 ,3 ,1 ,4 ,7 ,7 ,5 ,7,},
		{4 ,8 ,4 ,5 ,0 ,2 ,4 ,5,},
		{4 ,9 ,7 ,7 ,4 ,6 ,6 ,4,},
		{0 ,6 ,9 ,2 ,8 ,5 ,3 ,8,},
		{5 ,5 ,0 ,2 ,5 ,2 ,8 ,8,}
    };



int main ()
{
  int tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
  int tmp10, tmp11, tmp12, tmp13;
  int z1, z2, z3, z4, z5, z11, z13;
  //SHIFT_TEMPS

  /* Pass 1: process rows. */

  //dataptr = data;
arr_str =  &data[0] ;
arr_end =  &data[149] + (&data[149] -&data[148]  ) ;


  	startPar = 0x1;
  for (int ctr = 0 ; ctr < SIZEX; ctr++) {
    tmp0 = data[ctr][0] + data[ctr][7];
    tmp7 = data[ctr][0] - data[ctr][7];
    tmp1 = data[ctr][1] + data[ctr][6];
    tmp6 = data[ctr][1] - data[ctr][6];
    tmp2 = data[ctr][2] + data[ctr][5];
    tmp5 = data[ctr][2] - data[ctr][5];
    tmp3 = data[ctr][3] + data[ctr][4];
    tmp4 = data[ctr][3] - data[ctr][4];
    
    /* Even part */
    
    tmp10 = tmp0 + tmp3;	/* phase 2 */
    tmp13 = tmp0 - tmp3;
    tmp11 = tmp1 + tmp2;
    tmp12 = tmp1 - tmp2;
    
    data[ctr][0] = tmp10 + tmp11; /* phase 3 */
    data[ctr][4] = tmp10 - tmp11;
    
    z1 = MULTIPLY(tmp12 + tmp13, FIX_0_707106781); /* c4 */
    data[ctr][2] = tmp13 + z1;	/* phase 5 */
    data[ctr][6] = tmp13 - z1;
    
    /* Odd part */

    tmp10 = tmp4 + tmp5;	/* phase 2 */
    tmp11 = tmp5 + tmp6;
    tmp12 = tmp6 + tmp7;

    /* The rotator is modified from fig 4-8 to avoid extra negations. */
    z5 = MULTIPLY(tmp10 - tmp12, FIX_0_382683433); /* c6 */
    z2 = MULTIPLY(tmp10, FIX_0_541196100) + z5; /* c2-c6 */
    z4 = MULTIPLY(tmp12, FIX_1_306562965) + z5; /* c2+c6 */
    z3 = MULTIPLY(tmp11, FIX_0_707106781); /* c4 */

    z11 = tmp7 + z3;		/* phase 5 */
    z13 = tmp7 - z3;

    data[ctr][5] = z13 + z2;	/* phase 6 */
    data[ctr][3] = z13 - z2;
    data[ctr][1] = z11 + z4;
    data[ctr][7] = z11 - z4;

    //dataptr += DCTSIZE;		/* advance pointer to next row */
  }
	startPar = 0x0;
  /* Pass 2: process columns. */

  /*dataptr = data;
  for (ctr = DCTSIZE-1; ctr >= 0; ctr--) {
    tmp0 = dataptr[DCTSIZE*0] + dataptr[DCTSIZE*7];
    tmp7 = dataptr[DCTSIZE*0] - dataptr[DCTSIZE*7];
    tmp1 = dataptr[DCTSIZE*1] + dataptr[DCTSIZE*6];
    tmp6 = dataptr[DCTSIZE*1] - dataptr[DCTSIZE*6];
    tmp2 = dataptr[DCTSIZE*2] + dataptr[DCTSIZE*5];
    tmp5 = dataptr[DCTSIZE*2] - dataptr[DCTSIZE*5];
    tmp3 = dataptr[DCTSIZE*3] + dataptr[DCTSIZE*4];
    tmp4 = dataptr[DCTSIZE*3] - dataptr[DCTSIZE*4];
    
    // Even part 
    
    tmp10 = tmp0 + tmp3;	// phase 2 
    tmp13 = tmp0 - tmp3;
    tmp11 = tmp1 + tmp2;
    tmp12 = tmp1 - tmp2;
    
    dataptr[DCTSIZE*0] = tmp10 + tmp11; // phase 3 
    dataptr[DCTSIZE*4] = tmp10 - tmp11;
    
    z1 = MULTIPLY(tmp12 + tmp13, FIX_0_707106781); // c4 
    dataptr[DCTSIZE*2] = tmp13 + z1; // phase 5 
    dataptr[DCTSIZE*6] = tmp13 - z1;
    
    // Odd part 

    tmp10 = tmp4 + tmp5;	// phase 2 
    tmp11 = tmp5 + tmp6;
    tmp12 = tmp6 + tmp7;

    // The rotator is modified from fig 4-8 to avoid extra negations. 
    z5 = MULTIPLY(tmp10 - tmp12, FIX_0_382683433); // c6 
    z2 = MULTIPLY(tmp10, FIX_0_541196100) + z5; // c2-c6 
    z4 = MULTIPLY(tmp12, FIX_1_306562965) + z5; // c2+c6 
    z3 = MULTIPLY(tmp11, FIX_0_707106781); // c4 

    z11 = tmp7 + z3;		// phase 5 
    z13 = tmp7 - z3;

    dataptr[DCTSIZE*5] = z13 + z2; // phase 6 
    dataptr[DCTSIZE*3] = z13 - z2;
    dataptr[DCTSIZE*1] = z11 + z4;
    dataptr[DCTSIZE*7] = z11 - z4;

    dataptr++;			// advance pointer to next column 
  }*/
  
  return 1;
}

 /* DCT_IFAST_SUPPORTED */
