#ifndef __MWC32AVX2__
#define __MWC32AVX2__
#include "stdint.h"
#include "ranvec1.h"
#include <iostream>
using namespace std;

template<uint32_t pool_size> 

class MWC32AVX2 {
	public:
		uint32_t operator()();
		
		typedef uint32_t result_type;
		uint32_t max() const;
		uint32_t min() const;
		
		MWC32AVX2 ();
		~MWC32AVX2 ();
	private:
		Ranvec1 _rgen;
		uint32_t*_pool;
		uint32_t _i;
		void generate();
		void initialize();
};

template<uint32_t pool_size>
uint32_t MWC32AVX2<pool_size> :: operator()() {
	//cout << "Hi from operator" << endl;
	if( _i == pool_size) {
		//cout << "Regen" << endl;	
		generate();
		_i=0;
	}
	return _pool[_i++];
}

template<uint32_t pool_size>
void MWC32AVX2<pool_size> :: generate() {
	
	uint32_t* pointer = _pool; 
	
	for(uint32_t i= 0; i!= pool_size/8; ++i) {
		
		_rgen.random256b().store(pointer);
		//cout << "Array_info" << pointer[i] << endl;
		//cout << "Array_info" << pointer[i] << endl;
		pointer +=8;
	}
	
}

template<uint32_t pool_size>
uint32_t MWC32AVX2<pool_size> :: min() const {
	return 0;
}

template<uint32_t pool_size>
uint32_t MWC32AVX2<pool_size>  :: max() const {
	return 0xffffffff;
}

template<uint32_t pool_size>
void MWC32AVX2<pool_size> :: initialize() {
	_pool = new uint32_t [pool_size];
}

template<uint32_t pool_size>
MWC32AVX2<pool_size> :: MWC32AVX2 ():_i(0), _rgen(Ranvec1(1)) {
	_rgen.init(0);
	initialize();
	generate();
	//cout << "Hi" << endl;
}

template<uint32_t pool_size>
MWC32AVX2<pool_size> :: ~MWC32AVX2 () {
	delete [] _pool;
	
}

#endif