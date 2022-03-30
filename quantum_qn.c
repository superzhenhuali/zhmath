#include "polynomial.h"
#include "quantum_groups.h"
#include "quantum_qn.h"


math_list * Quantum_Groups_Qn_getAll(int n){
	math_list * res = NULL, *t = NULL;
	
	AlgebraElement * T;
	Monomial * mo;
	uint32_t j;

	ElementType types[3] = {QUANTUM_F, QUANTUM_E, QUANTUM_K};
	int lens[3] = {n-1, n-1, n};
	
	for(j=0; j<3; j++){
		t = Quantum_Groups_Get_List(types[j], 1, lens[j]);
		res = math_list_cat(res, t);
		t = Quantum_Groups_Get_List(types[j], 0-lens[j], 0-1);
		res = math_list_cat(res, t);
	}
	
	//MDEBUG("return : %p, %p", res, mo);
	return res;
}
