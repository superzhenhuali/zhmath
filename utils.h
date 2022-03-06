#ifndef __ZHMATH_UTILS_H__
#define __ZHMATH_UTILS_H__ 1
#include <stdint.h>

#define ZHM_IS_NUMBER(a) (((a)<= '9')&&((a) >='0'))
#define ZHM_IS_UPPER(a) (((a)<= 'Z')&&((a) >='A'))
#define ZHM_IS_LOWER(a) (((a)<= 'z')&&((a) >='a'))
#define ZHM_IS_ALPHABETA(a) (ZHM_IS_UPPER(a) || ZHM_IS_LOWER(a))

void * malloc_zero(uint32_t size);
void string_right_trip(char * str);
void string_right_trip_middle(char * str);


#endif
