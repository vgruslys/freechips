#ifndef __VECTOR_ALGORITHMS__
#define __VECTOR_ALGORITHMS__

#include "vectorclass.h"

/************************************************************************************************************
 * 
 * Vector Algorithms - set of bit manipulation functions acting on vectors of 
 * 16 16bit integers.
 * 
 * Vec16us popcount(Vec16us v, const Vec32uc & table)
 * 		Computes the number of set bits by using a lookup table
 * Vec32uc popcountTable()
 * 		Generates the table needed to compute popcounts
 * Vec32uc msbix(Vec16us v, const Vec32uc & table_lo, const Vec32uc & table_hi, const Vec32uc & table_hi_hi)
 *  	Computes the index of the most significant bit by using three lookup tables
 * 		Remark: this number is 0 if there is no set bit, and between 1 to 16 otherwise (i.e. the first possible
 * 		index is 1 not 0)
 * Vec32uc msbixTableLo();
 * Vec32uc msbixTableHi();
 * Vec32uc msbixTableHiHi();
 * 		Generate the tables required to compute the index of the most significant bit
 * Vec16us lsb(Vec16us v);
 * 		Returns the least set bit of v
 * Vec16us rlsb(Vec16us v);
 * 		Removes the least set bit of v
 * Vec16us rlsbc(Vec16us v);
 * 		Returns v minus its least set bit if v has at lest two set bits, returns v otherwise
 * Vec16us rlsbc2(Vec16us v);
 * 		Returns v minus its least set bit if v has at least three set bits, returns v otherwise
 * 
 * ***********************************************************************************************************/
Vec16us popcount(Vec16us, const Vec32uc &);
Vec32uc popcountTable();
Vec16us msbix(Vec16us, const Vec32uc &, const Vec32uc &, const Vec32uc &);
Vec32uc msbixTableLo();
Vec32uc msbixTableHi();
Vec32uc msbixTableHiHi();
Vec16us rlsbc(Vec16us);
Vec16us rlsbc2(Vec16us);
Vec16us rlsb(Vec16us);
Vec16us lsb(Vec16us);
#endif
