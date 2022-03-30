#ifndef __ZHMATH_QUANTUM_GROUPS_H__
#define __ZHMATH_QUANTUM_GROUPS_H__ 1

#include "defines.h"
#include "math_list.h"

AlgebraElement * QuantumElement_new(ElementType type, uint32_t index);
math_list * Quantum_Groups_Get_List(ElementType type, int begin_index, int end_index);

#endif

