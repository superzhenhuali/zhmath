#ifndef __BIG_INT_H__
#define __BIG_INT_H__

#include <stdlib.h>
#include <stdint.h>

#define MAX_PINT 100
#define MAX_BUF 128

typedef enum repr_type_t{
	TYPE_HEX=1,
	TYPE_DEC,
}repr_type;

#define SYSTEM_MOD_HEX 0xff
#define SYSTEM_MOD_DEC 100

typedef struct bigint_t{
	uint8_t value[128];
	int nsize;
	repr_type ntype;
	uint8_t minus;
}bigint;
//typedef struct bigint_t bigint;

int bigint_from_str(bigint *bi, const char * str);
int bigint_to_str(char * str, int size, bigint * bi);
int bigint_dump(bigint * bi);
int bigint_cmp_norm(bigint * a, bigint * b);
int bigint_from_hex( bigint * bi, const char * str);
int bigint_from_dec( bigint * bi, const char * str);
int bigint_from_type(bigint * bi, const char * str, repr_type t);
int bigint_add_norm(bigint * bres, bigint * a, bigint * b);
int bigint_sub_norm(bigint * bres, bigint * a, bigint * b);
int bigint_add(bigint * bres, bigint * a, bigint * b);
int bigint_sub(bigint * bres, bigint * a, bigint * b);
int bigint_mul(bigint * bres, bigint * a, bigint * b);
int bigint_div(bigint * bres, bigint * a, bigint * b);
#endif
