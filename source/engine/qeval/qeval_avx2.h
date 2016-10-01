#ifndef __QEVAL_AVX2__
#define __QEVAL_AVX2__

#include "qeval.h"
#include "stdint.h"
#include "vectorclass.h"

/***************************************************************************************************************
 * 
 * QEvalAVX2 - Implementation of QEval using the AVX2 and BMI1 instruction sets.  
 * 
 * For details regarding the public interface, see qeval.h. 
 * 
 ***************************************************************************************************************/


class QEvalAVX2 : public QEval {
	public:
		
		inline uint32_t score(uint64_t) const final;
		inline void score(uint64_t*, uint32_t*, uint64_t) const final;
		
		QEvalAVX2();
		~QEvalAVX2();
		
private:
		//Single processing methods
		uint8_t* _straights; //Pointer to the look-up table for straights
		void generateStraights(); //Generated the table pointed to by _straights
		uint8_t findBestStraight(uint64_t) const; //Finds the best straight amongst 13 cards of different heights
		
		//Batch processing methods
		inline void batch16(uint64_t*, uint32_t*) const ; //Processes 16 hands at a time
		inline Vec16us findStraight(Vec16us) const ; //Finds best straights 16 hands at a time
		Vec8ui _index; //Variable used to permute elements of a vector
};
#endif

