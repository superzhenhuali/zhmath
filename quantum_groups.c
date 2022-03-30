#include "logs.h"
#include "polynomial.h"
#include "quantum_groups.h"

AlgebraElement * QuantumElement_new(ElementType type, uint32_t index){
	//AlgebraElement * res = (AlgebraElement *)malloc_zero(sizeof(AlgebraElement));
	AlgebraElement * res = AlgebraElement_new();
	
	switch(type){
		case QUANTUM_F:
			res->name[0] = 'F';
			break;
		case QUANTUM_K:
			res->name[0] = 'K';
			break;
		case QUANTUM_E:
			res->name[0] = 'E';
			break;
		default:
			return NULL;
			break;
	}
	res->type = type;
	res->infix[0] = '_';
	res->index = index;
	//{
	//	char tmp[16];
	//	MDEBUG("ELE:[%s]", AlgebraElement_toString(tmp, res));
	//	MDEBUG("ELE: %p, [%s][%s][%d]", res, res->name, res->infix, res->index);
	//}
	return res;
}

math_list * Quantum_Groups_Get_List(ElementType type, int begin_index, int end_index){
	math_list * res = NULL;
	
	AlgebraElement * T;
	Monomial * mo;
	int i;

	MDEBUG("Type: %d, index : %d to %d  ", type, begin_index, end_index);
	//ElementType types[3] = {QUANTUM_F, QUANTUM_E, QUANTUM_K};
	for(i=begin_index; i<= end_index; i++){
		T = QuantumElement_new(type, i);
		mo = Monomial_from_AlgebraElement(T);
		res = math_list_append(res, node_from_Monomial(mo));
		MDEBUG("Type: %d, index : %d ", type, i);
	}
	
	//MDEBUG("return : %p, %p", res, mo);
	return res;
}
