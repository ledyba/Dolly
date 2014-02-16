/* coding: utf-8 */
/**
 * Dolly
 *
 * Copyright 2014, PSI
 */
#include "Util.h"
#include <stdio.h>
#include <cstdarg>
#include <cstring>
#include <stdexcept>

namespace dolly
{

std::string format(std::string const& fmt, ...)
{
	va_list lst;
	va_start(lst, fmt);
	std::string res(formatv(fmt, lst));
	va_end(lst);
	return res;
}
std::string formatv(std::string const& fmt, va_list args)
{
	char buff[8192];
	size_t len = vsnprintf(buff, 8192, fmt.c_str(), args);
	if(len < 0){
		throw std::runtime_error("Format string too long!!");
	}
	return buff;
}


}
