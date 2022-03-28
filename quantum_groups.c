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
