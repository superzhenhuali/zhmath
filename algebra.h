#ifndef __ZHMATH_ALGEBRA__
#define __ZHMATH_ALGEBRA__ 1
#include <stdint.h>

#include "defines.h"
#include "math_list.h"
#include "polynomial.h"
#include "heckecliff.h"

/*
#define ALGEBRA_ELEMENT_NAME_LENGTH 16
#define ALGEBRA_NAME_DEFAULT "A"
#define ALGEBRA_HECKE_NAME_DEFAULT "T"
#define ALGEBRA_CLIFF_NAME_DEFAULT "c"


typedef enum ElementType_t {
	NONE_TYPE= 0,
	NUMBER_E = 1,
	VARIABLE_E, 
	ALGEBRA_E,
	ALGEBRA_HECKE_E,
	ALGEBRA_CLIFF_E,
}ElementType;

//union Number
typedef int Number;
//Number operator *(Number & other);
//	Monomial operator *(AlgebraElement & ele);
//	Monomial operator *(Monomial & m);
//	~Number(){}

//std::map<ElementType, std::string> typeToName;

typedef struct AlgebraElement_t{
	ElementType type;
	char name[16];
	char infix[16];
	int16_t index;
}AlgebraElement;

typedef struct Monomial_t{
	math_list * members;
	Number coeff;
}Monomial;

#define ALGEBRAELEMENT_NODE(node) ((AlgebraElement *)(node->data))
Monomial * Monomial_new();
void Monomial_free(Monomial * mo);
Monomial * Monomial_copy(Monomial * mo);
Monomial * Monomial_mul_Monomial(Monomial *a, Monomial *b);
Monomial * Monomial_mul_AlgebraElement(Monomial *mo, AlgebraElement * ele);
Monomial * Monomial_from_AlgebraElement(AlgebraElement * ele);
Monomial * Monomial_from_string(const char * str);
Monomial * Monomial_from_Number(Number n);
void Monomial_mulBy_Element(AlgebraElement * ele);
void Monomial_mulByNumber(Number num);
Monomial * Number_mul_AlgebraElement(Number n, AlgebraElement * ele);
Monomial*  AlgebraElement_mul_Monomial(AlgebraElement* ele, Monomial * mo);
//math_list *node_from_AlgebraElement(AlgebraElement * ele);
math_list *node_from_Monomial(Monomial * mo);
AlgebraElement * AlgebraElement_new();
AlgebraElement * AlgebraElement_init(AlgebraElement * res);
void AlgebraElement_delete(AlgebraElement * alg);
char * AlgebraElement_toString(char * res, AlgebraElement *ele);
char * Monomial_toString(char * res, Monomial * m);
void AlgebraElement_mul(AlgebraElement * ele);
int  Monomial_compare(Monomial *a, Monomial *b);
int  AlgebraElement_compare_variable(AlgebraElement *a, AlgebraElement *b);
ElementType AlgebraElement_node_type(math_list *node);
AlgebraElement * AlgebraElement_node(math_list *node);
char * AlgebraElement_node_name(math_list *node);
int AlgebraElement_node_index(math_list *node);

int Monomial_sort(Monomial *mo);
int Monomial_sortType(Monomial *mo);
int Monomial_sortVariable(Monomial *mo);
*/
#endif
