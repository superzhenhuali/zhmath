#ifndef __ZH_DEFINES_H__
#define __ZH_DEFINES_H__ 1
#include <stdint.h>

typedef enum Matrix_entry_t{
	MATRIX_ENTRY_NONE=0,
	MATRIX_ENTRY_CHAR,
	MATRIX_ENTRY_INT,
	MATRIX_ENTRY_REAL,
	MATRIX_ENTRY_COMPLEX,
	MATRIX_ENTRY_ALGEBRA_ELEMENT,
	MATRIX_ENTRY_MONOMIAL,
	MATRIX_ENTRY_POLYNOMIAL,
}Matrix_type;


extern uint8_t MATRIX_SIZE[8];


#define ALGEBRA_ELEMENT_NAME_LENGTH 16
#define ALGEBRA_NAME_DEFAULT "A"
#define ALGEBRA_HECKE_NAME_DEFAULT "T"
#define ALGEBRA_CLIFF_NAME_DEFAULT "c"


typedef enum ElementType_t {
	NONE_TYPE= 0,
	NUMBER_E = 1,
	VARIABLE_E0, 
	VARIABLE_E1, 
	VARIABLE_E2, 
	VARIABLE_E100, 
	VARIABLE_E, 
	ALGEBRA_E,
	ALGEBRA_HECKE_E,
	ALGEBRA_CLIFF_E,
	ALGEBRA_HIGH1, 
	ALGEBRA_HIGH2, 
	ALGEBRA_HIGH3,
	//E, F, K, index may be minus if it is odd in super case.
       	QUANTUM_F,
       	QUANTUM_K,	
       	QUANTUM_E,	
	ALGEBRA_ALL,
}ElementType;

//union Number
typedef int Number;
//Number operator *(Number & other);
//	Monomial operator *(AlgebraElement & ele);
//	Monomial operator *(Monomial & m);
//	~Number(){}

//std::map<ElementType, std::string> typeToName;


struct AlgebraElement_t;
struct Monomial_t;
struct Polynomial_t;
struct PolyMonomial_t;
struct PolyPolynomial_t;
struct Matrix_t;
struct Equation_t;

#define ENTRY_SIZE_POLYNOMIAL (sizeof(struct Polynomial_t))
#define ENTRY_SIZE_DEFAULT (sizeof(void *))

#endif
