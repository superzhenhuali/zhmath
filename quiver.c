#include <stdlib.h>
#include <string.h>

#include "logs.h"
#include "quiver.h"

dimv * dimv_new(int size){
	dimv * n = (dimv *)malloc(sizeof(dimv));
	if(NULL == n)return n;

	memset(n, 0, sizeof(dimv));
	n->size = size;

	n->vec = (dim_ele *)malloc(size * sizeof(dim_ele));
	if(NULL == n->vec){
		free(n);
		return NULL;
	}

	memset(n->vec, 0, size * sizeof(dim_ele));
	return n;
}

void dimv_free(dimv * v){
	if(NULL != v){
		if(NULL != v->vec){
			free(v->vec);
		}
		free(v);
	}
}

void dimv_dump(dimv * v){
	int i;
	MDEBUGP();
	for(i=0; i< v->size; i++){
		MDEBUGC("%d\t", v->vec[i]);
	}
	MDEBUGC("\n");
}

void quiver_simple_module(quiver * quiv, int pos, dimv * res){
	int i=0;
	for(i=0; i<quiv->rows; i++){
		res->vec[i] = 0;
	}
	res->vec[pos-1] = 1;
}
void quiver_projective_module(quiver * quiv, int pos, dimv * res){
	int i=0;

	for(i=0; i<quiv->rows; i++){
		res->vec[i] = 0;
	}
	res->vec[pos-1] = 1;
}
