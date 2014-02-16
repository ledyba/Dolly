/* coding: utf-8 */
/**
 * Color
 *
 * Copyright 2013, psi
 */

#include "../main/sha256.h"
#include "../main/dolly/Dolly.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <cstdio>

int main(int argc, char** argv)
{
	{
		using namespace dolly;
		RecorderBuilder b(640,480,"test.mp4");
		Recorder r = b.build();
		for( int i=0;i<1000;++i ) {
			r.shot();
		}
	}
	using namespace clr;
	Field out[8];
	Field in[100*3] = {
	};
	int idx=0;
	for( auto& i : in ) {
		i = Field(0x00000000,
				idx %3 == 0 ? Color(1,0,0) :
				idx %3 == 1 ? Color(0,1,0) : Color(0,0,1) );
		++idx;
	}
	std::printf("length: %lu\n", sizeof(in)/sizeof(Field));
	sha2(in, out);
	for( int i=0;i<8;++i ){
		Color c = out[i].color();
		std::printf("%08x -> %.2f,%.2f,%.2f",
				out[i].value(), c.red(), c.green(), c.blue());
		std::puts("");
	}

	return 0;
}
