#include "vectorclass.h"
#include "immintrin.h"
#include "stdint.h"

/*Function to compute the number of set bits of 16  16-bit unsigned integers in parallel*/

Vec16us popcount(Vec16us v, const Vec32uc & table) {
	Vec32uc count_lo = _mm256_shuffle_epi8 (table, v & Vec32uc(0xf));  //Mask needed  due to _mm256_shuffle_epi8 behaviour
	Vec32uc y = ((Vec32uc) (__m256i) v) >> 4; 
	Vec32uc count_hi = _mm256_shuffle_epi8 (table, y);
	Vec32uc count_half = (count_hi + count_lo); 
	return (((Vec16us) count_half) >> 8) + (Vec16us) (count_half & Vec16us(0xff));  //Mask needed to remove upper half counts}
}

/*Function to generate the lookup table used by the above function*/
Vec32uc popcountTable() {
	uint8_t array_table [32] {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4};
	Vec32uc table;
	table.load(array_table);
	return table;
}

/*Function to compute the position of the largest set bit of 16 16-bit unsigned integers in parallel*/

Vec16us msbix(Vec16us v, const Vec32uc & table_lo, const Vec32uc & table_hi, const Vec32uc & table_hi_hi) {  
	Vec32uc index_lo = _mm256_shuffle_epi8 (table_lo, v & Vec32uc(0xf)); //Mask needed  due to _mm256_shuffle_epi8 behaviour
	v = v >> 4; 
	Vec32uc index_hi = _mm256_shuffle_epi8 (table_hi, v);
	Vec32uc index_half = max(index_lo, index_hi); 
	Vec32uc index_half_hi = (__m256i) (((Vec16us) index_half) >> 8); 
	index_half_hi = _mm256_shuffle_epi8(table_hi_hi, index_half_hi);
	return ((Vec16us) ((__m256i) max(index_half, index_half_hi))) & Vec16us(0xff); //2op 
}

/*Functions to generate the three tables used by the above function*/
Vec32uc msbixTableLo() {
	uint8_t array_table [32] {0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4};
	Vec32uc table;
	table.load(array_table);
	return table;
}

Vec32uc msbixTableHi() {
	uint8_t array_table [32] {0,5,6,6,7,7,7,7,8,8,8,8,8,8,8,8,0,5,6,6,7,7,7,7,8,8,8,8,8,8,8,8};
	Vec32uc table;
	table.load(array_table);
	return table;
}

Vec32uc msbixTableHiHi() {
	uint8_t array_table [32] {0,9,10,11,12,13,14,15,16,0,0,0,0,0,0,0,0,9,10,11,12,13,14,15,16,0,0,0,0,0,0,0};
	Vec32uc table;
	table.load(array_table);
	return table;
}

/*Function to remove the lowest set bit of 16 16-bit unsigned integers in parallel if at least two bits are set*/
Vec16us rlsbc(Vec16us v) { //7ops
	Vec16us z = v & (Vec16us(0) - v);
	Vec16us y = v-z;
	return v- (Vec16us) andnot(z, y==0);
}

/*Function to remove the lowest set bit of 16 16-bit unsigned integers in parallel if at least three bits are set*/
	Vec16us rlsbc2(Vec16us v) { // 10ops
	Vec16us z = v & (Vec16us(0) - v);
	Vec16us y = v-z;
	Vec16us w = y & (y-Vec16us(1));
	return v - (Vec16us) andnot(z,w==0);
}
/*Function to remove the lowest set bit of 16 16-bit unsigned integers in parallel */
Vec16us rlsb(Vec16us v) { 
	return v & (v-1);
}

/*Function to get the lowest set bit of 16 16-bit integers in parallel */

Vec16us lsb(Vec16us v) {
	return v & (0-v);
}
