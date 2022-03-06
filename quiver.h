#ifndef __ZH_QUIVER_H__
#define __ZH_QUIVER_H__
#include <stdint.h>
#include "defines.h"
#include "matrix.h"

typedef uint8_t dim_ele ;
#define DIM_SIZE 128 ;
typedef struct dimv_t{
	dim_ele *vec;
	int size;
}dimv;

void dimv_free(dimv * v);
dimv * dimv_new(int size);

typedef Matrix quiver;
//typedef dim_ele dimv[];
void dimv_dump(dimv *v);

void quiver_simple_module(quiver * quiv, int pos, dimv * res);


#endif

