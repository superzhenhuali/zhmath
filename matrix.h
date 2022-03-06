#ifndef __ZH_MATRIX_H__
#define __ZH_MATRIX_H__  1
#include <stdint.h>
#include "defines.h"

typedef void * PT;

typedef struct Matrix_t 
{
	PT *entries; 
	Matrix_type type;
	uint8_t rows;
	uint8_t cols;
	uint8_t mem_rows;
	uint8_t mem_cols;
	int (*dump)(struct Matrix_t * mat);
	int (*resize)(struct Matrix_t * mat, int rows, int cols);
	int (*setValue)(struct Matrix_t * mat, void *arr);
	int (*setType)(struct Matrix_t * mat, Matrix_type type);
	int (*setEntry)(struct Matrix_t * mat, uint8_t row, uint8_t col, void *arr);
	void * extra1;
	void * extra2;
}Matrix;

Matrix * Matrix_new();
int Matrix_resize(Matrix * mat, int rows, int cols);
Matrix * Matrix_char_new();
void Matrix_char_dump(Matrix * mat);
void Matrix_char_set(Matrix * mat, void *arr);
int Matrix_setType(Matrix * mat, Matrix_type type);

int Matrix_resize(Matrix * mat, int rows, int cols);
int Matrix_Polynomial_dump(struct Matrix_t * mat);
int Matrix_Polynomial_set(struct Matrix_t * mat, void *arr);
int Matrix_Polynomial_set_entry(struct Matrix_t * mat, uint8_t row, uint8_t col, PT data);
void Matrix_Polynomial_to_Tex(Matrix * mat);
PT Matrix_get(Matrix * mat, int row, int col);


#endif
