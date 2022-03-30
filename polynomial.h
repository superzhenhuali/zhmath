#ifndef __ZHMATH_POLYNOMIAL_H__
#define __ZHMATH_POLYNOMIAL_H__
#include <stdint.h>
#include <stdio.h>
#include "defines.h"
#include "math_list.h"



typedef struct AlgebraElement_t{
	ElementType type;
	char name[16];
	char infix[16];
	int16_t index;
	int16_t power;
}AlgebraElement;

typedef struct Monomial_t{
	math_list * elements;
	Number coeff;
}Monomial;

typedef struct Polynomial_t{
	math_list * monomials;
}Polynomial;

typedef struct PolyMonomial_t{
	Polynomial *coeff;
	Monomial *monomial;
}PolyMonomial;


typedef struct PolyPolynomial_t{
	// list of PolyMonomial
	math_list * polymonomials;
}PolyPolynomial;


#define ALGEBRAELEMENT_NODE(node) ((AlgebraElement *)(node->data))
#define MONOMIAL_NODE(node) ((Monomial *)(node->data))


Monomial * Monomial_new();
void Monomial_delete(Monomial * mo);
//void Monomial_free(Monomial * mo);
Monomial * Monomial_copy(Monomial * mo);
AlgebraElement* AlgebraElement_copy(AlgebraElement* a);
Monomial * AlgebraElement_mul_AlgebraElement(AlgebraElement* a, AlgebraElement* b);
Monomial * Monomial_mul_Monomial(Monomial *a, Monomial *b);
Monomial * Monomial_mul_list(int count,...);
Monomial * Monomial_mul_AlgebraElement(Monomial *mo, AlgebraElement * ele);
Monomial * Monomial_from_AlgebraElement(AlgebraElement * ele);
Monomial * Monomial_from_string(const char * str);
AlgebraElement * AlgebraElement_from_string(const char * str);
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
int  Monomial_similar(Monomial *a, Monomial *b);
int  Monomial_compare(Monomial *a, Monomial *b);
int  AlgebraElement_compare_variable(AlgebraElement *a, AlgebraElement *b);
int  AlgebraElement_compare_variable_with_type(AlgebraElement *a, AlgebraElement *b, ElementType type);
ElementType AlgebraElement_node_type(math_list *node);
AlgebraElement * AlgebraElement_node(math_list *node);
char * AlgebraElement_node_name(math_list *node);
int AlgebraElement_node_index(math_list *node);
AlgebraElement * Parameter_Variable_new(const char * name);

int Monomial_sort(Monomial *mo);
int Monomial_sortType(Monomial *mo);
int Monomial_sortVariable(Monomial *mo);

Polynomial * Polynomial_new();
Polynomial * Monomial_copy_to_Polynomial(Monomial * mo);
char * Polynomial_toString(char *res, Polynomial * poly);
Polynomial * Polynomial_mul_Polynomial(Polynomial * p1, Polynomial * p2);
int  Monomial_compare_type(Monomial *a, Monomial *b, ElementType type);

int Polynomial_rearrange_round(Polynomial * poly);

//All elements_type < type is in coeffcients.
math_list * Monomial_from_type(Monomial *mo, ElementType type);
//All elements_type < type is in coeffcients.
PolyPolynomial * PolyPolynomial_from_Polynomial(Polynomial * poly, ElementType type);
char * PolyPolynomial_toString(char * str, PolyPolynomial * pp);

Polynomial * Polynomial_copy(Polynomial * poly);
int Polynomial_compare(Polynomial * pa, Polynomial * pb);
int Polynomial_cal_sum(Polynomial * poly);
int PolyPolynomial_cal_sum(PolyPolynomial * poly);

AlgebraElement * Monomial_find_AlgebraElement_by_type(Monomial * mo, ElementType type);
Monomial * Monomial_get_coeff_by_type(Monomial * mo, ElementType type);
void Polynomial_delete(Polynomial * poly);
void PolyPolynomial_delete(PolyPolynomial * poly);
void PolyMonomial_delete(PolyMonomial * pm);
Monomial * Monomial_get_coeff_by_Ele(Monomial * mo, AlgebraElement * ele);


Polynomial * Monomial_replace(Monomial * mo, AlgebraElement * ele, Polynomial * poly);
void Polynomial_replace(Polynomial * poly, struct Equation_t * eq);
void Polynomial_replace_list(Polynomial * poly, math_list * eqs);
void PolyPolynomial_Output(PolyPolynomial * pp, FILE * fp);
void PolyMonomial_Output(PolyMonomial * pm, FILE * fp);
void Polynomial_Output(Polynomial * poly, FILE * fp);
void Monomial_Output( Monomial * m, FILE * fp);
int PolyPolynomial_rearrange_round(PolyPolynomial * poly);
int PolyMonomial_sort(PolyMonomial * pm);

PolyMonomial * PolyMonomial_new();
char * PolyMonomial_toString(char * str, PolyMonomial * pm);
int  AlgebraElement_compare(AlgebraElement *a, AlgebraElement *b);
#endif
