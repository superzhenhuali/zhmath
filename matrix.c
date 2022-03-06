#include <stdlib.h>
#include <string.h>

#include "zhmath.h"
#include "utils.h"
#include "matrix.h"
#include "polynomial.h"
#include "logs.h"
#include "testing.h"

uint8_t MATRIX_SIZE[8] = {
	1, 4, sizeof(double), 16
};


void Matrix_array_dump(char *array, int size){
	int i,j;
	char *pos;
	
	pos = array;
	for(i=0; i<size; i++){
		for(j=0; j<size; j++){
			MDEBUGC("%d \t", *pos);
		}
		MDEBUGC("\n");
	}
}

/*
Matrix * Matrix_new(Matrix_type type){
	switch(type){
		case MATRIX_ENTRY_CHAR:
			return Matrix_char_new();
		default:
			return NULL;
	}
}
*/

Matrix * Matrix_new(){
	Matrix * mat;

	mat = (Matrix *)malloc_zero(sizeof(Matrix));
	if(NULL == mat) return NULL;

	mat->type = MATRIX_ENTRY_NONE;

	//mat->entries = NULL;
	mat->resize = Matrix_resize;
	//mat->dump = Matrix_char_dump;
	//mat->setValue = Matrix_char_set;
	mat->setType = Matrix_setType;

	return mat;
}

int Matrix_setType(Matrix * mat, Matrix_type type){
	mat->type = type;
	switch(type){ 
		case MATRIX_ENTRY_POLYNOMIAL:
			mat->dump = Matrix_Polynomial_dump;
			mat->setValue = Matrix_Polynomial_set;
			break;
		default:
			break;
	}
	return 0;
}

int Matrix_resize(Matrix * mat, int rows, int cols){
	if(NULL == mat )return -1;
	if(NULL != mat->entries ) 
		free(mat->entries);

	mat->rows = rows;
	mat->cols = cols;
	mat->mem_rows = rows;
	mat->mem_cols = cols;
	
	mat->entries = malloc_zero(mat->rows * mat->cols * sizeof(PT));
	if(NULL == mat->entries )return -1;
	
	
	return 0;
}

void Matrix_char_dump(Matrix * mat){
	char * ent;
	int row,col;

	if(NULL == mat || NULL == mat->entries )return;

	ent = (char *)mat->entries;
	MDEBUG("Matrix: %p", mat);
	for(row = 0; row < mat->rows; row ++){
		//MDEBUG("Matrix: row");
		for(col = 0; col < mat->cols; col ++){
			MDEBUGC("%d\t", *ent);
			ent ++;
		}
		MDEBUGC("\n");
	}
}

void Matrix_char_set(Matrix * mat, void *arr){
	char * ent;
	char * nv;
	int row,col;

	//MDEBUG("Matrix: %p, mat->entries:%p, arr:%p, ", mat, mat->entries, arr);
	if(	NULL == mat || NULL == mat->entries || NULL == arr )
		return;

	ent = (char *)mat->entries;
	nv = (char *)arr;
	for(row = 0; row < mat->rows; row ++){
		//MDEBUG("Matrix: row, ent: %p", ent);
		for(col = 0; col < mat->cols; col ++){
			//MDEBUG("Matrix: row %d, col %d ", row, col);
			*ent = *nv;
			ent ++;
			nv ++;
		}
	}
}

int Matrix_Polynomial_dump(Matrix * mat){
	return 0;
}
        //int (*resize)(Matrix * mat, int rows, int cols);
int Matrix_Polynomial_set(Matrix * mat, void *arr){
	return 0;
}
int Matrix_Polynomial_set_entry(Matrix * mat, uint8_t row, uint8_t col, PT data){
	if(mat == NULL){
		MDEBUG("Matrix pointer error.");
		return -1;
	}
	if((row >= mat->rows)||(col >= mat->cols)){
		MDEBUG("size error.");
		return -1;
	}

	*(mat->entries + row*(mat->mem_cols) + col) = data;
	return 0;
}

PT Matrix_get(Matrix * mat, int row, int col){
	PT res;

	res = *(mat->entries + row*(mat->mem_cols) + col);
	//MDEBUG("Return %p", res);
	return res;
}
void Matrix_Polynomial_to_Tex(Matrix * mat){
	int i, j;
	Polynomial * poly;
	
	/*
	 
	 \left (
	\begin{matrix}
	         0        &      a_{12}^0   & a_{13}^0\\
	         0 &      0 & a_{23}^0 \\
	          a^0_{31}& a^0_{32}& 0
	\end{matrix}     
	\right) .
	 
	 * */

	char tmp[STR_BUFFER];
	math_list *p;
	TEXDEBUG("\n" FORMULAR_LEFT "\n");
	//TEXDEBUG("\\left (\n");
	//TEXDEBUG("\\begin{matrix}\n");
	//TEXDEBUG("Variables: [");
        for(p = mat->extra1; p!= NULL; p = p->next){

                AlgebraElement * e = p->data;
                AlgebraElement_toString(tmp, e);
                TEXDEBUG("%s, ", tmp);
			if(p->next != NULL){
				TEXDEBUG(" & ");
			}else{
				TEXDEBUG(" \\\\\n");
			}
        }
        //TEXDEBUG("]. \n\n");
	for(i=0; i<mat->rows; i++){
		for(j=0; j < mat->cols; j++){
			//poly = *(mat->entries + i*(mat->cols) + j);
			poly = Matrix_get(mat, i, j);
			Polynomial_toString(tmp, poly);
			TEXDEBUG(" %s ", tmp);
			if(j != mat->cols - 1){
				TEXDEBUG(" & ");
			}else{
				TEXDEBUG(" \\\\\n");
			}
		}
	}
	//TEXDEBUG("\\end{matrix}\n");
	//TEXDEBUG("\\right).");
	TEXDEBUG(FORMULAR_RIGHT "\n");
}
