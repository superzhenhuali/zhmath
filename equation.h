#ifndef __ZHMATH_EQUATION_H__
#define __ZHMATH_EQUATION_H__ 1
#include <stdio.h>

#include "defines.h"
#include "matrix.h"
#include "polynomial.h"

typedef struct Equation_t{
	Polynomial * left;
	Polynomial * right;
}Equation;
typedef struct PPEquation_t{
	PolyPolynomial * left;
	PolyPolynomial * right;
}PPEquation;

Equation * Equation_new();
PPEquation * PPEquation_new();
char * Equation_toString(char * res, Equation * eq);
int  Equation_Process(Equation * eq);
int  Equation_compare(Equation * a, Equation *b);
Matrix * Equation_to_matrix(math_list * eq_list, ElementType type);
int Equation_solve_matrix(Matrix * mat);
PPEquation * PPEquation_from_Equation(Equation * eq,  ElementType type);
char * PPEquation_toString(char * res, PPEquation * eq);
math_list * PPEquations_solve(math_list ** res_p, math_list ** ppeqs_p);
math_list * PPEquations_solve_1(math_list ** res_p, math_list ** ppeqs_p);
math_list * PPEquations_solve_2(math_list ** res_p, math_list ** ppeqs_p);
void Equation_Output(Equation * eq, FILE * fp);
void PPEquation_Output(PPEquation * eq, FILE * fp);

void Equation_delete(Equation *eq);
void PPEquation_delete(PPEquation *eq);
#endif
