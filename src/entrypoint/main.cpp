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
	Field in[] = {
			Field(0x61616161, Color(255,0,0)),
			Field(0x61616161, Color(0,255,0)),
			Field(0x61616161, Color(0,0,255)),
			Field(0x61616161, Color(255,0,0)),
			Field(0x61616161, Color(0,255,0)),
			Field(0x61616161, Color(0,0,255)),
			Field(0x61616161, Color(255,0,0)),
			Field(0x61616161, Color(0,255,0)),
			Field(0x61616161, Color(0,0,255)),
			Field(0x61616161, Color(255,0,0)),
			Field(0x61616161, Color(0,255,0)),
			Field(0x61616161, Color(0,0,255)),
			Field(0x61616161, Color(255,0,0)),
			Field(0x61616161, Color(0,255,0)),
			Field(0x61616161, Color(0,0,255)),
			Field(0x61616161, Color(255,0,0)),
			Field(0x61616161, Color(0,255,0)),
			Field(0x61616161, Color(0,0,255)),
			Field(0x61616161, Color(255,0,0)),
			Field(0x61616161, Color(0,255,0)),
			Field(0x61616161, Color(0,0,255)),
			Field(0x61616161, Color(255,0,0)),
			Field(0x61616161, Color(0,255,0)),
			Field(0x6161610a, Color(0,0,255))
	};
	sha2(in, out);
	for( int i=0;i<8;++i ){
		Color c = out[i].color();
		std::printf("%08x -> %02d,%02d,%02d", out[i].value(), c.red(), c.green(), c.blue());
		std::puts("");
	}
	return 0;
}
