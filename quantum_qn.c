#include "polynomial.h"
#include "quantum_groups.h"
#include "quantum_qn.h"


math_list * Quantum_Groups_Qn_getAll(int n){
	math_list * res = NULL;
	
	AlgebraElement * T;
	Monomial * mo;
	uint32_t i;

	ElementType types[3] = {QUANTUM_F, QUANTUM_E, QUANTUM_K};
	for(i=1; i<= n; i++){
		int j;
		for(j=0; j<3; j++){
			if( (i == n) && (j != 2) ){
				continue;
			}
			T = QuantumElement_new(types[j], i);
			mo = Monomial_from_AlgebraElement(T);
			res = math_list_append(res, node_from_Monomial(mo));
		
			T = QuantumElement_new(types[j], 0-i);
			mo = Monomial_from_AlgebraElement(T);
			res = math_list_append(res, node_from_Monomial(mo));
		}
		//MDEBUG("append: %p, %p", res, mo);
	}
	
	//MDEBUG("return : %p, %p", res, mo);
	return res;
}
