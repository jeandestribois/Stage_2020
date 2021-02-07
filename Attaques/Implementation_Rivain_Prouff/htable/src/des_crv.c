// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License version 2 as published
// by the Free Software Foundation.

/* This source file implements the Coron_Roy_Vivek (CRV) polynomial evaluation technique, from CHES 2014, with shared inputs. This code was written by Srinivas Vivek on Oct 15, 2014. */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "share.h"
#include "des.h"
#include "des_carlet.h"
#include "des_crv.h"
#include "des_htable.h"
#include "des_share.h"

extern byte CRV[760];
extern byte LinP[7680]; 
byte polyL(byte *,int ,byte );
byte polyCRV(byte *,byte );
byte polyCRV_tab(byte *,byte *,byte );
void testlagrangeCRV();
void testlagrangeCRV_tab();
void polyCRV_share(int ,byte *,int );
void testlagrangeCRV_share();


/*Encodings for the CRV method. For each S-box, there are p1,q1,p2,q2,p3. Irreducible polynomial for F_64: a^6 + a^4 + a^3 + a + 1.
L={0, 1, 2, 3, 4, 6, 7, 8, 12, 14, 16, 24, 28, 32, 33, 35, 48, 49, 56} */

byte CRV[760] = {1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 43, 33, 20, 54, 50, 43, 45, 38, 32, 30, 29, 55, 52, 13, 0, 4, 50, 61, 8, 29, 48, 25, 61, 54, 10, 31, 45, 29, 19, 15, 9, 52, 1, 34, 38, 40, 10, 48, 61, 52, 49, 63, 24, 0, 40, 13, 21, 0, 27, 56, 36, 18, 38, 35, 29, 36, 15, 25, 58, 5, 31, 42, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18, 16, 2, 50, 4, 35, 30, 2, 22, 55, 12, 1, 45, 26, 32, 33, 61, 17, 34, 59, 45, 17, 17, 20, 41, 10, 45, 41, 26, 36, 20, 42, 20, 42, 16, 58, 19, 35, 9, 10, 49, 48, 62, 49, 0, 43, 41, 51, 7, 17, 62, 6, 41, 48, 4, 28, 38, 4, 4, 58, 27, 21, 34, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 9, 39, 26, 15, 4, 55, 33, 29, 47, 17, 8, 53, 35, 25, 32, 55, 63, 21, 1, 33, 5, 40, 27, 63, 0, 21, 18, 28, 2, 63, 51, 17, 55, 63, 17, 54, 2, 52, 32, 4, 48, 44, 61, 0, 13, 5, 22, 28, 21, 4, 41, 6, 34, 24, 8, 44, 6, 51, 48, 44, 6, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 50, 56, 20, 2, 11, 45, 25, 3, 7, 38, 25, 26, 55, 48, 30, 54, 24, 36, 43, 63, 40, 63, 18, 5, 23, 19, 24, 30, 14, 44, 57, 36, 23, 41, 55, 28, 45, 15, 35, 0, 45, 34, 4, 0, 17, 21, 4, 31, 13, 33, 20, 39, 22, 20, 51, 49, 7, 42, 22, 50, 49, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 38, 2, 1, 55, 63, 50, 40, 54, 13, 54, 28, 52, 52, 3, 48, 43, 0, 54, 54, 62, 44, 24, 11, 43, 44, 53, 2, 41, 9, 29, 4, 46, 0, 45, 54, 42, 25, 32, 17, 11, 40, 54, 56, 9, 0, 62, 40, 0, 0, 42, 10, 4, 59, 14, 5, 59, 5, 5, 11, 12, 24, 3, 4, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 63, 35, 12, 24, 63, 6, 5, 24, 9, 28, 41, 53, 43, 8, 6, 15, 55, 41, 4, 9, 38, 51, 60, 61, 41, 6, 50, 12, 49, 19, 9, 1, 20, 59, 33, 43, 58, 40, 60, 57, 10, 7, 17, 31, 0, 58, 21, 43, 34, 62, 25, 44, 23, 26, 39, 60, 14, 5, 37, 29, 44, 53, 13, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 40, 21, 9, 48, 48, 25, 2, 59, 32, 11, 14, 52, 58, 34, 51, 62, 26, 12, 52, 41, 14, 35, 47, 42, 14, 22, 37, 0, 60, 54, 38, 26, 55, 19, 55, 37, 18, 10, 31, 58, 22, 41, 20, 0, 4, 56, 7, 10, 47, 39, 15, 26, 57, 55, 58, 20, 7, 14, 57, 61, 19, 30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 0, 9, 22, 8, 5, 23, 32, 9, 7, 42, 42, 17, 63, 58, 26, 32, 33, 34, 0, 29, 43, 29, 32, 62, 44, 1, 50, 63, 11, 52, 54, 10, 31, 49, 25, 29, 1, 6, 45, 50, 33, 59, 54, 0, 6, 33, 34, 1, 51, 38, 4, 45, 12, 58, 52, 8, 3, 50, 11, 48, 33, 47, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/* Look up tables for the linear polynomials corresponding to each of p1, q1, p2, q2, p3 for each S-box. For each polynomial pi or qi, first is the linear polynomial of x, then that of x^3, and finally that of x^7. */

byte LinP[7680] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 0, 59, 48, 11, 37, 30, 21, 46, 3, 56, 51, 8, 38, 29, 22, 45, 45, 22, 29, 38, 8, 51, 56, 3, 46, 21, 30, 37, 11, 48, 59, 0, 48, 11, 0, 59, 21, 46, 37, 30, 51, 8, 3, 56, 22, 45, 38, 29, 29, 38, 45, 22, 56, 3, 8, 51, 30, 37, 46, 21, 59, 0, 11, 48, 0, 20, 4, 16, 54, 34, 50, 38, 37, 49, 33, 53, 19, 7, 23, 3, 30, 10, 26, 14, 40, 60, 44, 56, 59, 47, 63, 43, 13, 25, 9, 29, 29, 9, 25, 13, 43, 63, 47, 59, 56, 44, 60, 40, 14, 26, 10, 30, 3, 23, 7, 19, 53, 33, 49, 37, 38, 50, 34, 54, 16, 4, 20, 0, 0, 51, 34, 17, 29, 46, 63, 12, 39, 20, 5, 54, 58, 9, 24, 43, 21, 38, 55, 4, 8, 59, 42, 25, 50, 1, 16, 35, 47, 28, 13, 62, 52, 7, 22, 37, 41, 26, 11, 56, 19, 32, 49, 2, 14, 61, 44, 31, 33, 18, 3, 48, 60, 15, 30, 45, 6, 53, 36, 23, 27, 40, 57, 10, 0, 40, 33, 9, 37, 13, 4, 44, 46, 6, 15, 39, 11, 35, 42, 2, 9, 33, 40, 0, 44, 4, 13, 37, 39, 15, 6, 46, 2, 42, 35, 11, 49, 25, 16, 56, 20, 60, 53, 29, 31, 55, 62, 22, 58, 18, 27, 51, 56, 16, 25, 49, 29, 53, 60, 20, 22, 62, 55, 31, 51, 27, 18, 58, 0, 42, 8, 34, 59, 17, 51, 25, 38, 12, 46, 4, 29, 55, 21, 63, 59, 17, 51, 25, 0, 42, 8, 34, 29, 55, 21, 63, 38, 12, 46, 4, 4, 46, 12, 38, 63, 21, 55, 29, 34, 8, 42, 0, 25, 51, 17, 59, 63, 21, 55, 29, 4, 46, 12, 38, 25, 51, 17, 59, 34, 8, 42, 0, 0, 30, 17, 15, 53, 43, 36, 58, 19, 13, 2, 28, 38, 56, 55, 41, 38, 56, 55, 41, 19, 13, 2, 28, 53, 43, 36, 58, 0, 30, 17, 15, 9, 23, 24, 6, 60, 34, 45, 51, 26, 4, 11, 21, 47, 49, 62, 32, 47, 49, 62, 32, 26, 4, 11, 21, 60, 34, 45, 51, 9, 23, 24, 6, 0, 58, 9, 51, 25, 35, 16, 42, 8, 50, 1, 59, 17, 43, 24, 34, 3, 57, 10, 48, 26, 32, 19, 41, 11, 49, 2, 56, 18, 40, 27, 33, 9, 51, 0, 58, 16, 42, 25, 35, 1, 59, 8, 50, 24, 34, 17, 43, 10, 48, 3, 57, 19, 41, 26, 32, 2, 56, 11, 49, 27, 33, 18, 40, 0, 14, 11, 5, 61, 51, 54, 56, 40, 38, 35, 45, 21, 27, 30, 16, 24, 22, 19, 29, 37, 43, 46, 32, 48, 62, 59, 53, 13, 3, 6, 8, 3, 13, 8, 6, 62, 48, 53, 59, 43, 37, 32, 46, 22, 24, 29, 19, 27, 21, 16, 30, 38, 40, 45, 35, 51, 61, 56, 54, 14, 0, 5, 11, 0, 50, 43, 25, 15, 61, 36, 22, 56, 10, 19, 33, 55, 5, 28, 46, 38, 20, 13, 63, 41, 27, 2, 48, 30, 44, 53, 7, 17, 35, 58, 8, 45, 31, 6, 52, 34, 16, 9, 59, 21, 39, 62, 12, 26, 40, 49, 3, 11, 57, 32, 18, 4, 54, 47, 29, 51, 1, 24, 42, 60, 14, 23, 37, 0, 60, 29, 33, 8, 52, 21, 41, 43, 23, 54, 10, 35, 31, 62, 2, 54, 10, 43, 23, 62, 2, 35, 31, 29, 33, 0, 60, 21, 41, 8, 52, 34, 30, 63, 3, 42, 22, 55, 11, 9, 53, 20, 40, 1, 61, 28, 32, 20, 40, 9, 53, 28, 32, 1, 61, 63, 3, 34, 30, 55, 11, 42, 22, 0, 47, 20, 59, 55, 24, 35, 12, 18, 61, 6, 41, 37, 10, 49, 30, 14, 33, 26, 53, 57, 22, 45, 2, 28, 51, 8, 39, 43, 4, 63, 16, 2, 45, 22, 57, 53, 26, 33, 14, 16, 63, 4, 43, 39, 8, 51, 28, 12, 35, 24, 55, 59, 20, 47, 0, 30, 49, 10, 37, 41, 6, 61, 18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 0, 2, 51, 49, 44, 46, 31, 29, 25, 27, 42, 40, 53, 55, 6, 4, 1, 3, 50, 48, 45, 47, 30, 28, 24, 26, 43, 41, 52, 54, 7, 5, 35, 33, 16, 18, 15, 13, 60, 62, 58, 56, 9, 11, 22, 20, 37, 39, 34, 32, 17, 19, 14, 12, 61, 63, 59, 57, 8, 10, 23, 21, 36, 38, 0, 27, 2, 25, 62, 37, 60, 39, 10, 17, 8, 19, 52, 47, 54, 45, 58, 33, 56, 35, 4, 31, 6, 29, 48, 43, 50, 41, 14, 21, 12, 23, 37, 62, 39, 60, 27, 0, 25, 2, 47, 52, 45, 54, 17, 10, 19, 8, 31, 4, 29, 6, 33, 58, 35, 56, 21, 14, 23, 12, 43, 48, 41, 50, 0, 22, 0, 22, 29, 11, 29, 11, 0, 22, 0, 22, 29, 11, 29, 11, 61, 43, 61, 43, 32, 54, 32, 54, 61, 43, 61, 43, 32, 54, 32, 54, 53, 35, 53, 35, 40, 62, 40, 62, 53, 35, 53, 35, 40, 62, 40, 62, 8, 30, 8, 30, 21, 3, 21, 3, 8, 30, 8, 30, 21, 3, 21, 3, 0, 53, 26, 47, 11, 62, 17, 36, 24, 45, 2, 55, 19, 38, 9, 60, 7, 50, 29, 40, 12, 57, 22, 35, 31, 42, 5, 48, 20, 33, 14, 59, 13, 56, 23, 34, 6, 51, 28, 41, 21, 32, 15, 58, 30, 43, 4, 49, 10, 63, 16, 37, 1, 52, 27, 46, 18, 39, 8, 61, 25, 44, 3, 54, 0, 21, 11, 30, 7, 18, 12, 25, 32, 53, 43, 62, 39, 50, 44, 57, 37, 48, 46, 59, 34, 55, 41, 60, 5, 16, 14, 27, 2, 23, 9, 28, 35, 54, 40, 61, 36, 49, 47, 58, 3, 22, 8, 29, 4, 17, 15, 26, 6, 19, 13, 24, 1, 20, 10, 31, 38, 51, 45, 56, 33, 52, 42, 63, 0, 26, 31, 5, 56, 34, 39, 61, 52, 46, 43, 49, 12, 22, 19, 9, 26, 0, 5, 31, 34, 56, 61, 39, 46, 52, 49, 43, 22, 12, 9, 19, 40, 50, 55, 45, 16, 10, 15, 21, 28, 6, 3, 25, 36, 62, 59, 33, 50, 40, 45, 55, 10, 16, 21, 15, 6, 28, 25, 3, 62, 36, 33, 59, 0, 47, 56, 23, 22, 57, 46, 1, 41, 6, 17, 62, 63, 16, 7, 40, 2, 45, 58, 21, 20, 59, 44, 3, 43, 4, 19, 60, 61, 18, 5, 42, 9, 38, 49, 30, 31, 48, 39, 8, 32, 15, 24, 55, 54, 25, 14, 33, 11, 36, 51, 28, 29, 50, 37, 10, 34, 13, 26, 53, 52, 27, 12, 35, 0, 20, 52, 32, 0, 20, 52, 32, 8, 28, 60, 40, 8, 28, 60, 40, 44, 56, 24, 12, 44, 56, 24, 12, 36, 48, 16, 4, 36, 48, 16, 4, 43, 63, 31, 11, 43, 63, 31, 11, 35, 55, 23, 3, 35, 55, 23, 3, 7, 19, 51, 39, 7, 19, 51, 39, 15, 27, 59, 47, 15, 27, 59, 47, 0, 7, 32, 39, 61, 58, 29, 26, 18, 21, 50, 53, 47, 40, 15, 8, 36, 35, 4, 3, 25, 30, 57, 62, 54, 49, 22, 17, 11, 12, 43, 44, 10, 13, 42, 45, 55, 48, 23, 16, 24, 31, 56, 63, 37, 34, 5, 2, 46, 41, 14, 9, 19, 20, 51, 52, 60, 59, 28, 27, 1, 6, 33, 38, 0, 43, 49, 26, 43, 0, 26, 49, 3, 40, 50, 25, 40, 3, 25, 50, 52, 31, 5, 46, 31, 52, 46, 5, 55, 28, 6, 45, 28, 55, 45, 6, 0, 43, 49, 26, 43, 0, 26, 49, 3, 40, 50, 25, 40, 3, 25, 50, 52, 31, 5, 46, 31, 52, 46, 5, 55, 28, 6, 45, 28, 55, 45, 6, 0, 57, 8, 49, 34, 27, 42, 19, 58, 3, 50, 11, 24, 33, 16, 41, 13, 52, 5, 60, 47, 22, 39, 30, 55, 14, 63, 6, 21, 44, 29, 36, 36, 29, 44, 21, 6, 63, 14, 55, 30, 39, 22, 47, 60, 5, 52, 13, 41, 16, 33, 24, 11, 50, 3, 58, 19, 42, 27, 34, 49, 8, 57, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 0, 50, 29, 47, 32, 18, 61, 15, 19, 33, 14, 60, 51, 1, 46, 28, 37, 23, 56, 10, 5, 55, 24, 42, 54, 4, 43, 25, 22, 36, 11, 57, 50, 0, 47, 29, 18, 32, 15, 61, 33, 19, 60, 14, 1, 51, 28, 46, 23, 37, 10, 56, 55, 5, 42, 24, 4, 54, 25, 43, 36, 22, 57, 11, 0, 37, 61, 24, 55, 18, 10, 47, 51, 22, 14, 43, 4, 33, 57, 28, 38, 3, 27, 62, 17, 52, 44, 9, 21, 48, 40, 13, 34, 7, 31, 58, 47, 10, 18, 55, 24, 61, 37, 0, 28, 57, 33, 4, 43, 14, 22, 51, 9, 44, 52, 17, 62, 27, 3, 38, 58, 31, 7, 34, 13, 40, 48, 21, 0, 39, 3, 36, 35, 4, 32, 7, 10, 45, 9, 46, 41, 14, 42, 13, 1, 38, 2, 37, 34, 5, 33, 6, 11, 44, 8, 47, 40, 15, 43, 12, 51, 20, 48, 23, 16, 55, 19, 52, 57, 30, 58, 29, 26, 61, 25, 62, 50, 21, 49, 22, 17, 54, 18, 53, 56, 31, 59, 28, 27, 60, 24, 63, 0, 57, 37, 28, 53, 12, 16, 41, 35, 26, 6, 63, 22, 47, 51, 10, 17, 40, 52, 13, 36, 29, 1, 56, 50, 11, 23, 46, 7, 62, 34, 27, 57, 0, 28, 37, 12, 53, 41, 16, 26, 35, 63, 6, 47, 22, 10, 51, 40, 17, 13, 52, 29, 36, 56, 1, 11, 50, 46, 23, 62, 7, 27, 34, 0, 28, 38, 58, 53, 41, 19, 15, 58, 38, 28, 0, 15, 19, 41, 53, 31, 3, 57, 37, 42, 54, 12, 16, 37, 57, 3, 31, 16, 12, 54, 42, 43, 55, 13, 17, 30, 2, 56, 36, 17, 13, 55, 43, 36, 56, 2, 30, 52, 40, 18, 14, 1, 29, 39, 59, 14, 18, 40, 52, 59, 39, 29, 1, 0, 36, 16, 52, 28, 56, 12, 40, 15, 43, 31, 59, 19, 55, 3, 39, 4, 32, 20, 48, 24, 60, 8, 44, 11, 47, 27, 63, 23, 51, 7, 35, 12, 40, 28, 56, 16, 52, 0, 36, 3, 39, 19, 55, 31, 59, 15, 43, 8, 44, 24, 60, 20, 48, 4, 32, 7, 35, 23, 51, 27, 63, 11, 47, 0, 48, 46, 30, 25, 41, 55, 7, 0, 48, 46, 30, 25, 41, 55, 7, 31, 47, 49, 1, 6, 54, 40, 24, 31, 47, 49, 1, 6, 54, 40, 24, 15, 63, 33, 17, 22, 38, 56, 8, 15, 63, 33, 17, 22, 38, 56, 8, 16, 32, 62, 14, 9, 57, 39, 23, 16, 32, 62, 14, 9, 57, 39, 23, 0, 3, 53, 54, 40, 43, 29, 30, 33, 34, 20, 23, 9, 10, 60, 63, 4, 7, 49, 50, 44, 47, 25, 26, 37, 38, 16, 19, 13, 14, 56, 59, 36, 39, 17, 18, 12, 15, 57, 58, 5, 6, 48, 51, 45, 46, 24, 27, 32, 35, 21, 22, 8, 11, 61, 62, 1, 2, 52, 55, 41, 42, 28, 31, 0, 20, 23, 3, 4, 16, 19, 7, 62, 42, 41, 61, 58, 46, 45, 57, 63, 43, 40, 60, 59, 47, 44, 56, 1, 21, 22, 2, 5, 17, 18, 6, 29, 9, 10, 30, 25, 13, 14, 26, 35, 55, 52, 32, 39, 51, 48, 36, 34, 54, 53, 33, 38, 50, 49, 37, 28, 8, 11, 31, 24, 12, 15, 27, 0, 5, 43, 46, 39, 34, 12, 9, 10, 15, 33, 36, 45, 40, 6, 3, 35, 38, 8, 13, 4, 1, 47, 42, 41, 44, 2, 7, 14, 11, 37, 32, 27, 30, 48, 53, 60, 57, 23, 18, 17, 20, 58, 63, 54, 51, 29, 24, 56, 61, 19, 22, 31, 26, 52, 49, 50, 55, 25, 28, 21, 16, 62, 59, 0, 30, 38, 56, 36, 58, 2, 28, 50, 44, 20, 10, 22, 8, 48, 46, 13, 19, 43, 53, 41, 55, 15, 17, 63, 33, 25, 7, 27, 5, 61, 35, 63, 33, 25, 7, 27, 5, 61, 35, 13, 19, 43, 53, 41, 55, 15, 17, 50, 44, 20, 10, 22, 8, 48, 46, 0, 30, 38, 56, 36, 58, 2, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 0, 0, 31, 31, 46, 46, 49, 49, 39, 39, 56, 56, 9, 9, 22, 22, 58, 58, 37, 37, 20, 20, 11, 11, 29, 29, 2, 2, 51, 51, 44, 44, 51, 51, 44, 44, 29, 29, 2, 2, 20, 20, 11, 11, 58, 58, 37, 37, 9, 9, 22, 22, 39, 39, 56, 56, 46, 46, 49, 49, 0, 0, 31, 31, 0, 3, 22, 21, 41, 42, 63, 60, 15, 12, 25, 26, 38, 37, 48, 51, 57, 58, 47, 44, 16, 19, 6, 5, 54, 53, 32, 35, 31, 28, 9, 10, 14, 13, 24, 27, 39, 36, 49, 50, 1, 2, 23, 20, 40, 43, 62, 61, 55, 52, 33, 34, 30, 29, 8, 11, 56, 59, 46, 45, 17, 18, 7, 4, 0, 18, 4, 22, 26, 8, 30, 12, 50, 32, 54, 36, 40, 58, 44, 62, 29, 15, 25, 11, 7, 21, 3, 17, 47, 61, 43, 57, 53, 39, 49, 35, 27, 9, 31, 13, 1, 19, 5, 23, 41, 59, 45, 63, 51, 33, 55, 37, 6, 20, 2, 16, 28, 14, 24, 10, 52, 38, 48, 34, 46, 60, 42, 56, 0, 60, 42, 22, 36, 24, 14, 50, 21, 41, 63, 3, 49, 13, 27, 39, 27, 39, 49, 13, 63, 3, 21, 41, 14, 50, 36, 24, 42, 22, 0, 60, 14, 50, 36, 24, 42, 22, 0, 60, 27, 39, 49, 13, 63, 3, 21, 41, 21, 41, 63, 3, 49, 13, 27, 39, 0, 60, 42, 22, 36, 24, 14, 50, 0, 46, 16, 62, 6, 40, 22, 56, 61, 19, 45, 3, 59, 21, 43, 5, 41, 7, 57, 23, 47, 1, 63, 17, 20, 58, 4, 42, 18, 60, 2, 44, 34, 12, 50, 28, 36, 10, 52, 26, 31, 49, 15, 33, 25, 55, 9, 39, 11, 37, 27, 53, 13, 35, 29, 51, 54, 24, 38, 8, 48, 30, 32, 14, 0, 40, 29, 53, 40, 0, 53, 29, 30, 54, 3, 43, 54, 30, 43, 3, 15, 39, 18, 58, 39, 15, 58, 18, 17, 57, 12, 36, 57, 17, 36, 12, 55, 31, 42, 2, 31, 55, 2, 42, 41, 1, 52, 28, 1, 41, 28, 52, 56, 16, 37, 13, 16, 56, 13, 37, 38, 14, 59, 19, 14, 38, 19, 59, 0, 27, 26, 1, 33, 58, 59, 32, 10, 17, 16, 11, 43, 48, 49, 42, 8, 19, 18, 9, 41, 50, 51, 40, 2, 25, 24, 3, 35, 56, 57, 34, 42, 49, 48, 43, 11, 16, 17, 10, 32, 59, 58, 33, 1, 26, 27, 0, 34, 57, 56, 35, 3, 24, 25, 2, 40, 51, 50, 41, 9, 18, 19, 8, 0, 2, 53, 55, 14, 12, 59, 57, 9, 11, 60, 62, 7, 5, 50, 48, 62, 60, 11, 9, 48, 50, 5, 7, 55, 53, 2, 0, 57, 59, 12, 14, 28, 30, 41, 43, 18, 16, 39, 37, 21, 23, 32, 34, 27, 25, 46, 44, 34, 32, 23, 21, 44, 46, 25, 27, 43, 41, 30, 28, 37, 39, 16, 18, 0, 49, 38, 23, 35, 18, 5, 52, 63, 14, 25, 40, 28, 45, 58, 11, 29, 44, 59, 10, 62, 15, 24, 41, 34, 19, 4, 53, 1, 48, 39, 22, 7, 54, 33, 16, 36, 21, 2, 51, 56, 9, 30, 47, 27, 42, 61, 12, 26, 43, 60, 13, 57, 8, 31, 46, 37, 20, 3, 50, 6, 55, 32, 17, 0, 13, 30, 19, 5, 8, 27, 22, 16, 29, 14, 3, 21, 24, 11, 6, 20, 25, 10, 7, 17, 28, 15, 2, 4, 9, 26, 23, 1, 12, 31, 18, 62, 51, 32, 45, 59, 54, 37, 40, 46, 35, 48, 61, 43, 38, 53, 56, 42, 39, 52, 57, 47, 34, 49, 60, 58, 55, 36, 41, 63, 50, 33, 44, 0, 62, 15, 49, 8, 54, 7, 57, 19, 45, 28, 34, 27, 37, 20, 42, 42, 20, 37, 27, 34, 28, 45, 19, 57, 7, 54, 8, 49, 15, 62, 0, 63, 1, 48, 14, 55, 9, 56, 6, 44, 18, 35, 29, 36, 26, 43, 21, 21, 43, 26, 36, 29, 35, 18, 44, 6, 56, 9, 55, 14, 48, 1, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 0, 21, 8, 29, 26, 15, 18, 7, 29, 8, 21, 0, 7, 18, 15, 26, 1, 20, 9, 28, 27, 14, 19, 6, 28, 9, 20, 1, 6, 19, 14, 27, 8, 29, 0, 21, 18, 7, 26, 15, 21, 0, 29, 8, 15, 26, 7, 18, 9, 28, 1, 20, 19, 6, 27, 14, 20, 1, 28, 9, 14, 27, 6, 19, 0, 12, 1, 13, 9, 5, 8, 4, 45, 33, 44, 32, 36, 40, 37, 41, 35, 47, 34, 46, 42, 38, 43, 39, 14, 2, 15, 3, 7, 11, 6, 10, 44, 32, 45, 33, 37, 41, 36, 40, 1, 13, 0, 12, 8, 4, 9, 5, 15, 3, 14, 2, 6, 10, 7, 11, 34, 46, 35, 47, 43, 39, 42, 38, 0, 1, 56, 57, 44, 45, 20, 21, 49, 48, 9, 8, 29, 28, 37, 36, 51, 50, 11, 10, 31, 30, 39, 38, 2, 3, 58, 59, 46, 47, 22, 23, 7, 6, 63, 62, 43, 42, 19, 18, 54, 55, 14, 15, 26, 27, 34, 35, 52, 53, 12, 13, 24, 25, 32, 33, 5, 4, 61, 60, 41, 40, 17, 16, 0, 0, 19, 19, 55, 55, 36, 36, 62, 62, 45, 45, 9, 9, 26, 26, 24, 24, 11, 11, 47, 47, 60, 60, 38, 38, 53, 53, 17, 17, 2, 2, 60, 60, 47, 47, 11, 11, 24, 24, 2, 2, 17, 17, 53, 53, 38, 38, 36, 36, 55, 55, 19, 19, 0, 0, 26, 26, 9, 9, 45, 45, 62, 62, 0, 13, 45, 32, 20, 25, 57, 52, 33, 44, 12, 1, 53, 56, 24, 21, 28, 17, 49, 60, 8, 5, 37, 40, 61, 48, 16, 29, 41, 36, 4, 9, 6, 11, 43, 38, 18, 31, 63, 50, 39, 42, 10, 7, 51, 62, 30, 19, 26, 23, 55, 58, 14, 3, 35, 46, 59, 54, 22, 27, 47, 34, 2, 15, 0, 29, 57, 36, 19, 14, 42, 55, 37, 56, 28, 1, 54, 43, 15, 18, 14, 19, 55, 42, 29, 0, 36, 57, 43, 54, 18, 15, 56, 37, 1, 28, 43, 54, 18, 15, 56, 37, 1, 28, 14, 19, 55, 42, 29, 0, 36, 57, 37, 56, 28, 1, 54, 43, 15, 18, 0, 29, 57, 36, 19, 14, 42, 55, 0, 33, 12, 45, 40, 9, 36, 5, 34, 3, 46, 15, 10, 43, 6, 39, 55, 22, 59, 26, 31, 62, 19, 50, 21, 52, 25, 56, 61, 28, 49, 16, 57, 24, 53, 20, 17, 48, 29, 60, 27, 58, 23, 54, 51, 18, 63, 30, 14, 47, 2, 35, 38, 7, 42, 11, 44, 13, 32, 1, 4, 37, 8, 41, 0, 3, 48, 51, 31, 28, 47, 44, 37, 38, 21, 22, 58, 57, 10, 9, 45, 46, 29, 30, 50, 49, 2, 1, 8, 11, 56, 59, 23, 20, 39, 36, 9, 10, 57, 58, 22, 21, 38, 37, 44, 47, 28, 31, 51, 48, 3, 0, 36, 39, 20, 23, 59, 56, 11, 8, 1, 2, 49, 50, 30, 29, 46, 45, 0, 58, 9, 51, 53, 15, 60, 6, 23, 45, 30, 36, 34, 24, 43, 17, 10, 48, 3, 57, 63, 5, 54, 12, 29, 39, 20, 46, 40, 18, 33, 27, 36, 30, 45, 23, 17, 43, 24, 34, 51, 9, 58, 0, 6, 60, 15, 53, 46, 20, 39, 29, 27, 33, 18, 40, 57, 3, 48, 10, 12, 54, 5, 63, 0, 4, 22, 18, 3, 7, 21, 17, 33, 37, 55, 51, 34, 38, 52, 48, 59, 63, 45, 41, 56, 60, 46, 42, 26, 30, 12, 8, 25, 29, 15, 11, 37, 33, 51, 55, 38, 34, 48, 52, 4, 0, 18, 22, 7, 3, 17, 21, 30, 26, 8, 12, 29, 25, 11, 15, 63, 59, 41, 45, 60, 56, 42, 46, 0, 29, 48, 45, 23, 10, 39, 58, 51, 46, 3, 30, 36, 57, 20, 9, 35, 62, 19, 14, 52, 41, 4, 25, 16, 13, 32, 61, 7, 26, 55, 42, 44, 49, 28, 1, 59, 38, 11, 22, 31, 2, 47, 50, 8, 21, 56, 37, 15, 18, 63, 34, 24, 5, 40, 53, 60, 33, 12, 17, 43, 54, 27, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 0, 41, 0, 41, 56, 17, 56, 17, 27, 50, 27, 50, 35, 10, 35, 10, 18, 59, 18, 59, 42, 3, 42, 3, 9, 32, 9, 32, 49, 24, 49, 24, 33, 8, 33, 8, 25, 48, 25, 48, 58, 19, 58, 19, 2, 43, 2, 43, 51, 26, 51, 26, 11, 34, 11, 34, 40, 1, 40, 1, 16, 57, 16, 57, 0, 19, 25, 10, 26, 9, 3, 16, 41, 58, 48, 35, 51, 32, 42, 57, 63, 44, 38, 53, 37, 54, 60, 47, 22, 5, 15, 28, 12, 31, 21, 6, 22, 5, 15, 28, 12, 31, 21, 6, 63, 44, 38, 53, 37, 54, 60, 47, 41, 58, 48, 35, 51, 32, 42, 57, 0, 19, 25, 10, 26, 9, 3, 16, 0, 16, 41, 57, 9, 25, 32, 48, 58, 42, 19, 3, 51, 35, 26, 10, 47, 63, 6, 22, 38, 54, 15, 31, 21, 5, 60, 44, 28, 12, 53, 37, 35, 51, 10, 26, 42, 58, 3, 19, 25, 9, 48, 32, 16, 0, 57, 41, 12, 28, 37, 53, 5, 21, 44, 60, 54, 38, 31, 15, 63, 47, 22, 6, 0, 29, 41, 52, 43, 54, 2, 31, 7, 26, 46, 51, 44, 49, 5, 24, 32, 61, 9, 20, 11, 22, 34, 63, 39, 58, 14, 19, 12, 17, 37, 56, 49, 44, 24, 5, 26, 7, 51, 46, 54, 43, 31, 2, 29, 0, 52, 41, 17, 12, 56, 37, 58, 39, 19, 14, 22, 11, 63, 34, 61, 32, 20, 9, 0, 0, 38, 38, 21, 21, 51, 51, 40, 40, 14, 14, 61, 61, 27, 27, 62, 62, 24, 24, 43, 43, 13, 13, 22, 22, 48, 48, 3, 3, 37, 37, 46, 46, 8, 8, 59, 59, 29, 29, 6, 6, 32, 32, 19, 19, 53, 53, 16, 16, 54, 54, 5, 5, 35, 35, 56, 56, 30, 30, 45, 45, 11, 11, 0, 5, 41, 44, 23, 18, 62, 59, 48, 53, 25, 28, 39, 34, 14, 11, 44, 41, 5, 0, 59, 62, 18, 23, 28, 25, 53, 48, 11, 14, 34, 39, 16, 21, 57, 60, 7, 2, 46, 43, 32, 37, 9, 12, 55, 50, 30, 27, 60, 57, 21, 16, 43, 46, 2, 7, 12, 9, 37, 32, 27, 30, 50, 55, 0, 22, 9, 31, 35, 53, 42, 60, 31, 9, 22, 0, 60, 42, 53, 35, 54, 32, 63, 41, 21, 3, 28, 10, 41, 63, 32, 54, 10, 28, 3, 21, 53, 35, 60, 42, 22, 0, 31, 9, 42, 60, 35, 53, 9, 31, 0, 22, 3, 21, 10, 28, 32, 54, 41, 63, 28, 10, 21, 3, 63, 41, 54, 32, 0, 3, 46, 45, 22, 21, 56, 59, 28, 31, 50, 49, 10, 9, 36, 39, 47, 44, 1, 2, 57, 58, 23, 20, 51, 48, 29, 30, 37, 38, 11, 8, 8, 11, 38, 37, 30, 29, 48, 51, 20, 23, 58, 57, 2, 1, 44, 47, 39, 36, 9, 10, 49, 50, 31, 28, 59, 56, 21, 22, 45, 46, 3, 0, 0, 26, 10, 16, 54, 44, 60, 38, 57, 35, 51, 41, 15, 21, 5, 31, 19, 9, 25, 3, 37, 63, 47, 53, 42, 48, 32, 58, 28, 6, 22, 12, 1, 27, 11, 17, 55, 45, 61, 39, 56, 34, 50, 40, 14, 20, 4, 30, 18, 8, 24, 2, 36, 62, 46, 52, 43, 49, 33, 59, 29, 7, 23, 13, 0, 13, 55, 58, 14, 3, 57, 52, 57, 52, 14, 3, 55, 58, 0, 13, 37, 40, 18, 31, 43, 38, 28, 17, 28, 17, 43, 38, 18, 31, 37, 40, 25, 20, 46, 35, 23, 26, 32, 45, 32, 45, 23, 26, 46, 35, 25, 20, 60, 49, 11, 6, 50, 63, 5, 8, 5, 8, 50, 63, 11, 6, 60, 49, 0, 35, 23, 52, 14, 45, 25, 58, 28, 63, 11, 40, 18, 49, 5, 38, 1, 34, 22, 53, 15, 44, 24, 59, 29, 62, 10, 41, 19, 48, 4, 39, 47, 12, 56, 27, 33, 2, 54, 21, 51, 16, 36, 7, 61, 30, 42, 9, 46, 13, 57, 26, 32, 3, 55, 20, 50, 17, 37, 6, 60, 31, 43, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 0, 62, 59, 5, 31, 33, 36, 26, 17, 47, 42, 20, 14, 48, 53, 11, 9, 55, 50, 12, 22, 40, 45, 19, 24, 38, 35, 29, 7, 57, 60, 2, 33, 31, 26, 36, 62, 0, 5, 59, 48, 14, 11, 53, 47, 17, 20, 42, 40, 22, 19, 45, 55, 9, 12, 50, 57, 7, 2, 60, 38, 24, 29, 35, 0, 16, 48, 32, 11, 27, 59, 43, 42, 58, 26, 10, 33, 49, 17, 1, 6, 22, 54, 38, 13, 29, 61, 45, 44, 60, 28, 12, 39, 55, 23, 7, 23, 7, 39, 55, 28, 12, 44, 60, 61, 45, 13, 29, 54, 38, 6, 22, 17, 1, 33, 49, 26, 10, 42, 58, 59, 43, 11, 27, 48, 32, 0, 16, 0, 40, 42, 2, 19, 59, 57, 17, 23, 63, 61, 21, 4, 44, 46, 6, 39, 15, 13, 37, 52, 28, 30, 54, 48, 24, 26, 50, 35, 11, 9, 33, 30, 54, 52, 28, 13, 37, 39, 15, 9, 33, 35, 11, 26, 50, 48, 24, 57, 17, 19, 59, 42, 2, 0, 40, 46, 6, 4, 44, 61, 21, 23, 63, 0, 56, 8, 48, 23, 47, 31, 39, 33, 25, 41, 17, 54, 14, 62, 6, 4, 60, 12, 52, 19, 43, 27, 35, 37, 29, 45, 21, 50, 10, 58, 2, 13, 53, 5, 61, 26, 34, 18, 42, 44, 20, 36, 28, 59, 3, 51, 11, 9, 49, 1, 57, 30, 38, 22, 46, 40, 16, 32, 24, 63, 7, 55, 15, 0, 26, 39, 61, 14, 20, 41, 51, 56, 34, 31, 5, 54, 44, 17, 11, 41, 51, 14, 20, 39, 61, 0, 26, 17, 11, 54, 44, 31, 5, 56, 34, 48, 42, 23, 13, 62, 36, 25, 3, 8, 18, 47, 53, 6, 28, 33, 59, 25, 3, 62, 36, 23, 13, 48, 42, 33, 59, 6, 28, 47, 53, 8, 18, 0, 12, 43, 39, 58, 54, 17, 29, 3, 15, 40, 36, 57, 53, 18, 30, 41, 37, 2, 14, 19, 31, 56, 52, 42, 38, 1, 13, 16, 28, 59, 55, 13, 1, 38, 42, 55, 59, 28, 16, 14, 2, 37, 41, 52, 56, 31, 19, 36, 40, 15, 3, 30, 18, 53, 57, 39, 43, 12, 0, 29, 17, 54, 58, 0, 13, 40, 37, 19, 30, 59, 54, 23, 26, 63, 50, 4, 9, 44, 33, 56, 53, 16, 29, 43, 38, 3, 14, 47, 34, 7, 10, 60, 49, 20, 25, 45, 32, 5, 8, 62, 51, 22, 27, 58, 55, 18, 31, 41, 36, 1, 12, 21, 24, 61, 48, 6, 11, 46, 35, 2, 15, 42, 39, 17, 28, 57, 52, 0, 56, 55, 15, 21, 45, 34, 26, 63, 7, 8, 48, 42, 18, 29, 37, 19, 43, 36, 28, 6, 62, 49, 9, 44, 20, 27, 35, 57, 1, 14, 54, 13, 53, 58, 2, 24, 32, 47, 23, 50, 10, 5, 61, 39, 31, 16, 40, 30, 38, 41, 17, 11, 51, 60, 4, 33, 25, 22, 46, 52, 12, 3, 59, 0, 55, 5, 50, 59, 12, 62, 9, 41, 30, 44, 27, 18, 37, 23, 32, 62, 9, 59, 12, 5, 50, 0, 55, 23, 32, 18, 37, 44, 27, 41, 30, 43, 28, 46, 25, 16, 39, 21, 34, 2, 53, 7, 48, 57, 14, 60, 11, 21, 34, 16, 39, 46, 25, 43, 28, 60, 11, 57, 14, 7, 48, 2, 53, 0, 36, 18, 54, 55, 19, 37, 1, 62, 26, 44, 8, 9, 45, 27, 63, 17, 53, 3, 39, 38, 2, 52, 16, 47, 11, 61, 25, 24, 60, 10, 46, 54, 18, 36, 0, 1, 37, 19, 55, 8, 44, 26, 62, 63, 27, 45, 9, 39, 3, 53, 17, 16, 52, 2, 38, 25, 61, 11, 47, 46, 10, 60, 24, 0, 35, 2, 33, 35, 0, 33, 2, 55, 20, 53, 22, 20, 55, 22, 53, 43, 8, 41, 10, 8, 43, 10, 41, 28, 63, 30, 61, 63, 28, 61, 30, 46, 13, 44, 15, 13, 46, 15, 44, 25, 58, 27, 56, 58, 25, 56, 27, 5, 38, 7, 36, 38, 5, 36, 7, 50, 17, 48, 19, 17, 50, 19, 48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 0, 52, 25, 45, 5, 49, 28, 40, 37, 17, 60, 8, 32, 20, 57, 13, 33, 21, 56, 12, 36, 16, 61, 9, 4, 48, 29, 41, 1, 53, 24, 44, 63, 11, 38, 18, 58, 14, 35, 23, 26, 46, 3, 55, 31, 43, 6, 50, 30, 42, 7, 51, 27, 47, 2, 54, 59, 15, 34, 22, 62, 10, 39, 19, 0, 42, 6, 44, 48, 26, 54, 28, 14, 36, 8, 34, 62, 20, 56, 18, 24, 50, 30, 52, 40, 2, 46, 4, 22, 60, 16, 58, 38, 12, 32, 10, 35, 9, 37, 15, 19, 57, 21, 63, 45, 7, 43, 1, 29, 55, 27, 49, 59, 17, 61, 23, 11, 33, 13, 39, 53, 31, 51, 25, 5, 47, 3, 41, 0, 24, 56, 32, 3, 27, 59, 35, 34, 58, 26, 2, 33, 57, 25, 1, 49, 41, 9, 17, 50, 42, 10, 18, 19, 11, 43, 51, 16, 8, 40, 48, 54, 46, 14, 22, 53, 45, 13, 21, 20, 12, 44, 52, 23, 15, 47, 55, 7, 31, 63, 39, 4, 28, 60, 36, 37, 61, 29, 5, 38, 62, 30, 6, 0, 22, 23, 1, 14, 24, 25, 15, 16, 6, 7, 17, 30, 8, 9, 31, 58, 44, 45, 59, 52, 34, 35, 53, 42, 60, 61, 43, 36, 50, 51, 37, 17, 7, 6, 16, 31, 9, 8, 30, 1, 23, 22, 0, 15, 25, 24, 14, 43, 61, 60, 42, 37, 51, 50, 36, 59, 45, 44, 58, 53, 35, 34, 52, 0, 35, 61, 30, 20, 55, 41, 10, 52, 23, 9, 42, 32, 3, 29, 62, 53, 22, 8, 43, 33, 2, 28, 63, 1, 34, 60, 31, 21, 54, 40, 11, 27, 56, 38, 5, 15, 44, 50, 17, 47, 12, 18, 49, 59, 24, 6, 37, 46, 13, 19, 48, 58, 25, 7, 36, 26, 57, 39, 4, 14, 45, 51, 16, 0, 8, 62, 54, 51, 59, 13, 5, 41, 33, 23, 31, 26, 18, 36, 44, 43, 35, 21, 29, 24, 16, 38, 46, 2, 10, 60, 52, 49, 57, 15, 7, 50, 58, 12, 4, 1, 9, 63, 55, 27, 19, 37, 45, 40, 32, 22, 30, 25, 17, 39, 47, 42, 34, 20, 28, 48, 56, 14, 6, 3, 11, 61, 53, 0, 39, 18, 53, 48, 23, 34, 5, 34, 5, 48, 23, 18, 53, 0, 39, 7, 32, 21, 50, 55, 16, 37, 2, 37, 2, 55, 16, 21, 50, 7, 32, 41, 14, 59, 28, 25, 62, 11, 44, 11, 44, 25, 62, 59, 28, 41, 14, 46, 9, 60, 27, 30, 57, 12, 43, 12, 43, 30, 57, 60, 27, 46, 9, 0, 18, 51, 33, 27, 9, 40, 58, 17, 3, 34, 48, 10, 24, 57, 43, 1, 19, 50, 32, 26, 8, 41, 59, 16, 2, 35, 49, 11, 25, 56, 42, 32, 50, 19, 1, 59, 41, 8, 26, 49, 35, 2, 16, 42, 56, 25, 11, 33, 51, 18, 0, 58, 40, 9, 27, 48, 34, 3, 17, 43, 57, 24, 10, 0, 52, 35, 23, 54, 2, 21, 33, 28, 40, 63, 11, 42, 30, 9, 61, 17, 37, 50, 6, 39, 19, 4, 48, 13, 57, 46, 26, 59, 15, 24, 44, 29, 41, 62, 10, 43, 31, 8, 60, 1, 53, 34, 22, 55, 3, 20, 32, 12, 56, 47, 27, 58, 14, 25, 45, 16, 36, 51, 7, 38, 18, 5, 49, 0, 24, 54, 46, 29, 5, 43, 51, 6, 30, 48, 40, 27, 3, 45, 53, 4, 28, 50, 42, 25, 1, 47, 55, 2, 26, 52, 44, 31, 7, 41, 49, 20, 12, 34, 58, 9, 17, 63, 39, 18, 10, 36, 60, 15, 23, 57, 33, 16, 8, 38, 62, 13, 21, 59, 35, 22, 14, 32, 56, 11, 19, 61, 37, 0, 31, 49, 46, 5, 26, 52, 43, 23, 8, 38, 57, 18, 13, 35, 60, 1, 30, 48, 47, 4, 27, 53, 42, 22, 9, 39, 56, 19, 12, 34, 61, 8, 23, 57, 38, 13, 18, 60, 35, 31, 0, 46, 49, 26, 5, 43, 52, 9, 22, 56, 39, 12, 19, 61, 34, 30, 1, 47, 48, 27, 4, 42, 53, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


byte polyL(byte *pol,int d,byte x)
{
  int i;
  byte r=0, xL[19];

  xL[0]=1; xL[1]=x; 
  xL[2]=multtable(xL[1],xL[1]); xL[3]=multtable(xL[2],xL[1]); xL[4]=multtable(xL[2],xL[2]); 
  xL[5]=multtable(xL[3],xL[3]); xL[6]=multtable(xL[4],xL[3]); xL[7]=multtable(xL[4],xL[4]); 
  xL[8]=multtable(xL[5],xL[5]); xL[9]=multtable(xL[6],xL[6]); xL[10]=multtable(xL[7],xL[7]); 
  xL[11]=multtable(xL[8],xL[8]); xL[12]=multtable(xL[9],xL[9]); xL[13]=multtable(xL[10],xL[10]); 
  xL[16]=multtable(xL[11],xL[11]); xL[14]=multtable(xL[16],xL[16]); xL[18]=multtable(xL[12],xL[12]); 
  xL[17]=multtable(xL[18],xL[18]); xL[15]=multtable(xL[17],xL[17]);

  for(i=0;i<d;i++)
    r^=multtable(pol[i],xL[i]);
  return r;
}

byte polyCRV(byte *pol,byte x)
{
  int i;
  
  byte p1=polyL(pol,19,x);
  byte q1=polyL(pol+19,19,x);
  byte p2=polyL(pol+38,19,x);
  byte q2=polyL(pol+57,19,x);
  byte p3=polyL(pol+76,19,x);

  return multtable(p1,q1)^multtable(p2,q2)^p3;
}

byte polyCRV_tab(byte *pol,byte *lin,byte x)
{
  int i;
  byte r=0, p1=0, q1=0, p2=0, q2=0, p3=0, xL[5];

  xL[0]=x; 
  xL[1]=multtable(xL[0],xL[0]); xL[2]=multtable(xL[1],xL[0]); 
  xL[3]=multtable(xL[1],xL[1]); xL[4]=multtable(xL[2],xL[3]); 

  p1 ^= pol[0] ^ lin[(int)(xL[0])] ^ lin[64+(int)(xL[2])] ^ lin[128+(int)(xL[4])];
  q1 ^= pol[19] ^ lin[192+(int)(xL[0])] ^ lin[192+64+(int)(xL[2])] ^ lin[192+128+(int)(xL[4])];
  p2 ^= pol[19*2] ^ lin[192*2+(int)(xL[0])] ^ lin[192*2+64+(int)(xL[2])] ^ lin[192*2+128+(int)(xL[4])];
  q2 ^= pol[19*3] ^ lin[192*3+(int)(xL[0])] ^ lin[192*3+64+(int)(xL[2])] ^ lin[192*3+128+(int)(xL[4])];
  p3 ^= pol[19*4] ^ lin[192*4+(int)(xL[0])] ^ lin[192*4+64+(int)(xL[2])] ^ lin[192*4+128+(int)(xL[4])];

  return multtable(p1,q1)^multtable(p2,q2)^p3;
}

void testlagrangeCRV()
{
  int i,j;
  byte x;

  for(i=0;i<8;i++)
    for(x=0;x<64;x++)
      if((polyCRV(CRV+95*i,x)&15)!=sbox[(int)(x)+64*i])				/*Output only 4 bits*/
      {
	   fprintf(stderr,"\nIncorrect Lagrange interpolation %d %02x %02x %02x\n",
		   i,x,polyCRV(CRV+95*i,x)&15,sbox[(int)(x)+64*i]);
	   for(j=0;j<5;j++)
	  	printf("%d\n",polyL(CRV+95*i+19*j,19,x));

	   exit(EXIT_FAILURE);
      }
}

void testlagrangeCRV_tab()
{
  int i,j;
  byte x;

  for(i=0;i<8;i++)
    for(x=0;x<64;x++)
      if((polyCRV_tab(CRV+95*i,LinP+960*i,x)&15)!=sbox[(int)(x)+64*i])				/*Output only 4 bits*/
      {
	   fprintf(stderr,"\nIncorrect Lagrange interpolation with tables %d %02x %02x %02x\n",
		   i,x,polyCRV_tab(CRV+95*i,LinP+960*i,x)&15,sbox[(int)(x)+64*i]);

	   exit(EXIT_FAILURE);
      }
}

void polyCRV_share(int ii,byte *x,int n)
{
  int i,j,k,nmult;
  byte zi[5][n];
  byte v[5][n];
  byte temp[n], temp2[n];

  // Memory: 12*n+4

  byte *pol=CRV+95*ii;
  byte *lin = LinP+960*ii;
  
  memcpy(zi[0],x,n);
  nmult=0;
  
  square_share(zi[0],zi[1],n); 
  multshare(zi[0],zi[1],zi[2],n);
  nmult++;
  
  square_share(zi[1],zi[3],n);
  multshare(zi[2],zi[3],zi[4],n);
  nmult++;
 
  /* compute p1, q1, p2, q2, p3 */

  for(i=0;i<5;i++)
  {
    share(pol[i*19],v[i],n);
    for(k=0;k<n;k++)       
      v[i][k] ^= (lin[192*i+(int)(zi[0][k]&63)] ^ lin[192*i+64+(int)(zi[2][k]&63)] ^ lin[192*i+128+(int)(zi[4][k]&63)]); /* Each share <= 63 */
  }
  
  multshare(v[0],v[1],temp,n);
  multshare(v[2],v[3],temp2,n);
  nmult+=2;

  for(k=0;k<n;k++)
    x[k]=temp[k] ^ temp2[k] ^ v[4][k];
}

void testlagrangeCRV_share()
{
  int i,j,n=9;
  byte x,y;
  byte a[9];

  for(i=0;i<8;i++)
    for(x=0;x<64;x++)
	{ 
	  share(x,a,n);
	  refresh(a,n);
	  polyCRV_share(i,a,n);
	  y=decode(a,n) & 15;

      	  if(y!=sbox[(int)(x)+64*i])				/*Output only 4 bits*/
     	  {
	   fprintf(stderr,"\nIncorrect Lagrange interpolation with shares %d %02x %02x %02x\n",
		   i,x,y,sbox[(int)(x)+64*i]);

	   exit(EXIT_FAILURE);
      	  }
	}  
}

/*
int main()
{
 testlagrangeCRV();
 testlagrangeCRV_tab();
 testlagrangeCRV_share();
  
 return 0;
}
*/


