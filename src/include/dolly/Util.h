/* coding: utf-8 */
/**
 * Dolly
 *
 * Copyright 2014, PSI
 */
#pragma once

#include <cstdarg>
#include <string>

namespace dolly
{

std::string format(std::string const& fmt, ...);
std::string formatv(std::string const& fmt, va_list args);

}
