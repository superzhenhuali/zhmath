#ifndef __ZHMATH_HECKE_CLIFF_H__
#define __ZHMATH_HECKE_CLIFF_H__ 1
#include "math_list.h"
#include "algebra.h"

AlgebraElement * HeckeElement_new(uint32_t index);
AlgebraElement * CliffElement_new(uint32_t index);

math_list * Hecke_GetGenerators(uint32_t size);
math_list * Cliff_GetGenerators(uint32_t size);
math_list * Hecke_GetAll(uint32_t size);
math_list * Cliff_GetAll(uint32_t size);
math_list * HeckeCliff_GetAll(uint32_t size, int type);
//int HeckeCliff_relation1(Monomial *mo, math_list  * begin, math_list  * end);
//int HeckeCliff_relation2(Monomial *mo, math_list  * begin, math_list  * end);
//int HeckeCliff_relation3(Monomial *mo, math_list  * begin, math_list  * end);
//int HeckeCliff_relation4(Monomial *mo, math_list  * begin, math_list  * end);
int HeckeCliff_T_T(Monomial *mo, math_list * begin);
int HeckeCliff_T_C(Monomial *mo, math_list * begin);
int HeckeCliff_C_T(Monomial *mo, math_list * begin);
int HeckeCliff_C_C(Monomial *mo, math_list * begin);
int Hecke_IJI(Monomial *mo);
int Hecke_reduce_II(Monomial *mo);
int HeckeCliff_reduce_CT(Monomial *mo);

int HeckeCliff_sort(Monomial *mo);
int HeckeCliff_sort_round(Monomial *mo);
int HeckeCliff_reduce(Monomial *mo);

math_list * HeckeCliff_calculate(Monomial *mo);
math_list * HeckeCliff_calculate_CT(Monomial *mo);
math_list * Hecke_calculate_II(Monomial *mo);
int HeckeCliff_Polynomial_calculate(Polynomial * poly);
int  HeckeCliff_compare(Monomial *a, Monomial *b);
int  HeckeCliff_Polynomial_sort(Polynomial *poly);
typedef math_list* (*Hecke_calculate_monomial_func )(Monomial *mo);
int HeckeCliff_PolyPolynomial_calculate(PolyPolynomial * poly);
#endif
