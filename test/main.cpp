/* coding: utf-8 */
/**
 * TestEntryPoint
 *
 * Copyright 2013, psi
 */

#include <gtest/gtest.h>
#include <SDL2/SDL.h>
int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

