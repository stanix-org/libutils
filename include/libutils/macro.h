#ifndef LIBUTILS_MACRO_H
#define LIBUTILS_MACRO_H

#include <stddef.h>

#define utils_loop_var_value(type, var, value) for ( \
		type var = value, _utils_tmp = (type)1; \
		_utils_tmp; \
		_utils_tmp = (type)0)
	
#define utils_loop_var(type, var) for ( \
		type var, _utils_tmp = (type)1; \
		_utils_tmp; \
		_utils_tmp = (type)0)

#endif
