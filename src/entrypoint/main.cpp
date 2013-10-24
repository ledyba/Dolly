/* coding: utf-8 */
/**
 * Color
 *
 * Copyright 2013, psi
 */

#include "../main/sha256.h"
int main(int argc, char** argv)
{
	using namespace clr;
	Field out[8];
	Field in[1] = { Field(0x6161610a) };
	sha2(in, 1, out);
	for( int i=0;i<8;++i ){
		std::printf("%08x", out[i].value());
		std::puts("");
	}
	return 0;
}
