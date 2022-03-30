#include <stddef.h>
#include <stdlib.h>

#include "utils.h"
#include "heckecliff.h"
#include "math_list.h"
#include "logs.h"

/*
 *  HeckeClifford algebra functions.
 *
 * */
AlgebraElement * HeckeElement_new(uint32_t index){
	//AlgebraElement * res = (AlgebraElement *)malloc_zero(sizeof(AlgebraElement));
	AlgebraElement * res = AlgebraElement_new();

	res->name[0] = 'T',
	res->type = ALGEBRA_HECKE_E;
	res->infix[0] = '_';
	res->index = index;
	//{
	//	char tmp[16];
	//	MDEBUG("ELE:[%s]", AlgebraElement_toString(tmp, res));
	//	MDEBUG("ELE: %p, [%s][%s][%d]", res, res->name, res->infix, res->index);
	//}
	return res;
}

AlgebraElement * CliffElement_new(uint32_t index){
	//AlgebraElement * res = (AlgebraElement *)malloc_zero(sizeof(AlgebraElement));
	AlgebraElement * res = AlgebraElement_new();

	res->name[0] = 'c',
	res->type = ALGEBRA_CLIFF_E;
	res->infix[0] = '_';
	res->index = index;
	/*
	{
		char tmp[16];
		MDEBUG("ELE:[%s]", AlgebraElement_toString(tmp, res));
		MDEBUG("ELE: %p, [%s][%s][%d]", res, res->name, res->infix, res->index);
	}
	*/
	//MDEBUG("Return : %p", res);
	return res;
}

math_list * Hecke_GetGenerators(uint32_t size){
	math_list * res = NULL;
	
	AlgebraElement * T;
	Monomial * mo;
	uint32_t i;
	for(i=0; i<size-1; i++){
		T = HeckeElement_new(i+1);
		mo = Monomial_from_AlgebraElement(T);
		
		res = math_list_append(res, node_from_Monomial(mo));
		//MDEBUG("append: %p, %p", res, mo);
	}
	//MDEBUG("return : %p, %p", res, mo);
	return res;
}

math_list * Cliff_GetGenerators(uint32_t size){
	math_list * res = NULL;
	
	AlgebraElement * T;
	Monomial * mo;
	uint32_t i;
	for(i=0; i<size; i++){
		T = CliffElement_new(i+1);
		mo = Monomial_from_AlgebraElement(T);

		math_list * node = node_from_Monomial(mo);
		res = math_list_append(res, node);
		//MDEBUG("append: %p, %p", res, mo);
	}
	//MDEBUG("return : %p, %p", res, mo);
	return res;
}

math_list * Hecke_GetAll(uint32_t size){
	return HeckeCliff_GetAll(size, 1);
}
math_list * Cliff_GetAll(uint32_t size){
	return HeckeCliff_GetAll(size, 2);
}

//Type == 1: Hecke
//2: Clifford
//3 HeckeClifford
math_list * HeckeCliff_GetAll(uint32_t size, int type){
	math_list * res, *end;
	math_list * gen_hecke, * gen_cliff, *gen;
	math_list * p1, *p2;
	Monomial * m1, *m2, *m;
	math_list * node;
       	math_list * found;
	int i;
	int updated;
	char tmp[STR_BUFFER];

	gen_hecke = NULL;
	gen_cliff = NULL;
	if(type& 1){
		gen_hecke= Hecke_GetGenerators(size);
	}
	if(type& 2){
		gen_cliff= Cliff_GetGenerators(size);
	}
	
	gen = math_list_cat(gen_hecke, gen_cliff);
	res = NULL;
	end = NULL;
	MDEBUG("Got Gen: [%s]", math_list_to_string(tmp, gen, (math_list_node_string_func)Monomial_toString));

	for(p1 = gen; p1 != NULL; p1 = p1->next){
		node = node_from_Monomial(p1->data);
		res = math_list_append(res, node);
	}

	//int i=0;
	do{
		updated = 0;
		//int j = 0;
		end = math_list_end(res);
		for(p1 = gen; p1 != NULL; p1 = p1->next){
			//j = j + 1;
			m1 = p1->data;
			//len = math_list_length(res);
			i = 0;
			//MDEBUG("LEFT  LIST: [%s]", math_list_to_string(tmp, gen, Monomial_toString));
			//MDEBUG("RIGHT LIST: [%s]", math_list_to_string(tmp, gen, Monomial_toString));
			for(i = 0; i < math_list_length(res) ; i++){
				p2 = math_list_node_at(res, i);
				m2 = p2->data;

				//MDEBUG("LEFT  LIST: [%s] in [%s]", Monomial_toString(tmp, m1), 
				//				math_list_to_string(tmp2, gen, Monomial_toString));
				//MDEBUG("RIGHT LIST: %d [%s] in [%s]", i, Monomial_toString(tmp, m2),
				//				math_list_to_string(tmp2, res, Monomial_toString));
				//MDEBUG("LEFT  LIST: [%s].", Monomial_toString(tmp, m1));
				//MDEBUG("RIGHT LIST: %d [%s]. ", i, Monomial_toString(tmp, m2));

				if(m2 == NULL){
					//MDEBUG("Warning: Monomial pointer null.");
				}
				m = Monomial_mul_Monomial(m1, m2);
				//MDEBUG("Mul result:[%s]", Monomial_toString(tmp, m));
				HeckeCliff_reduce(m);
				//MDEBUG("Mul reduce result:[%s]", Monomial_toString(tmp, m));
				
				//MDEBUG("Length ==%d.", math_list_length(m->elements));
				if(math_list_length(m->elements) == 0){
					//MDEBUG("Length ==%d.", math_list_length(m->elements));
					//continue;
				}
				found = math_list_find_data(res, m, (math_list_compare_func)Monomial_similar);
				if(found == NULL){
					//NOTICE: Just get list, not calculate.
					m->coeff = 1;
					node = node_from_Monomial(m);
					Monomial_delete(m);
					res = math_list_append(res, node);
					//MDEBUG("Got new in result. %s", Monomial_toString(tmp, m));
					updated ++;
					if(updated >=10)break;
				}else{
					//Free node;
					//MDEBUG("Found it in result. %s", Monomial_toString(tmp, m));
				}
				//m = Monomial_mul_Monomial(m2, m1);
				//res = math_list_append_from(res, m);
	
				if(p2 == end)break;
			}
		}
		MDEBUG("Updated: %d", updated);
		//i = i + 1;
		//if(i > 1) break;

	}while(updated);

	math_list_delete_list(gen, (math_list_data_delete_func)Monomial_delete)	;
	//MDEBUG("Sorted.");
	math_list_sort_increase(res, NULL, (math_list_compare_func)Monomial_similar);
	return res;
}

/*
 *	T_i T_j  = T_j T_i, if |i-j|>1;
 *	T_{i+1} T_i T_{i+1} = T_i T_{i+1} T_{i};
 *
 *
 * */
int HeckeCliff_T_T(Monomial *mo, math_list * begin){
	AlgebraElement * a, *b;
	int updated=0;

	a = begin->data;
	b = begin->next->data;

	if(b->index + 1 <  a->index){
		// a: T_3,  b : T_1
		math_list_exchange(begin, begin->next);
		mo->elements = math_list_begin(begin);
		updated ++;
	}else if(b->index ==  a->index){
		// a: T_i b: T_i
	}
	/*
	}else if( (begin->next->next != NULL)&&(AlgebraElement_node_type(begin->next->next) == ALGEBRA_HECKE_E)  ){
		AlgebraElement *c = begin->next->next->data;
		if( (a->index == b->index + 1) && (c->index == b->index + 1)){
			a->index--;
			c->index--;
			b->index++;
			updated ++;
		}
	}
	*/
	return updated;
}
int HeckeCliff_T_C(Monomial *mo, math_list * begin){
	AlgebraElement * a, *b;
	int updated=0;

	a = begin->data;
	b = begin->next->data;

	// a: T_3,  b : c_2, c_5
	if( (b->index <  a->index) || (a->index + 1 < b->index) ){
		//math_list_exchange(begin, begin->next);
		//mo->elements = math_list_begin(begin);
	}
	return updated;
}
int HeckeCliff_C_T(Monomial *mo, math_list * begin){
	AlgebraElement * a, *b;
	int updated=0;

	a = begin->data;
	b = begin->next->data;

	// a : c_2, c_5, b:T_3
	if( (a->index <  b->index) || (b->index + 1 < a->index) ){
		math_list_exchange(begin, begin->next);
		mo->elements = math_list_begin(begin);
		updated ++;
	}
	else if( a->index ==  b->index + 1){
		a ->index --;
		math_list_exchange(begin, begin->next);
		mo->elements = math_list_begin(begin);
		updated ++;
	}
	return updated;
}
int HeckeCliff_C_C(Monomial *mo, math_list * begin){
	AlgebraElement * a, *b;
	int updated=0;

	a = begin->data;
	b = begin->next->data;

	// a : c_2, b:c_1
	if(a->index > b->index){
		math_list_exchange(begin, begin->next);
		mo->elements = math_list_begin(begin);
		mo->coeff = mo->coeff * (-1);
		updated ++;
	}else if( a->index == b->index){
		updated ++;
		//MDEBUG("Need to remove  it:");
		math_list *next = begin->next->next;
		math_list *pre = begin->pre;
		math_list *tmp1 = begin->next;
		math_list_delete_node(tmp1);
		math_list_delete_node(begin);
		

		mo->coeff = mo->coeff * (-1);
		if(pre == NULL){
			mo->elements = math_list_begin(next);
		}else{
			mo->elements = math_list_begin(pre);
		}
		updated ++;
	
	}
	return updated;
}

int Hecke_IJI(Monomial *mo){
	//int len;
	int i, j;
	int updated;
	if((NULL == mo)||(NULL == mo->elements)){
		return 0;
	}
	math_list * begin, *p, *q;
	math_list *pa, *pb, *pc;
	//AlgebraElement * a, *b, *c;
	//int ia, ib, ic;
	int n_i, n_ip, qi; 
	int i_begin, i_end, j_begin;
	begin = NULL;
	j = -1;
	for(i_begin = 0, p = mo->elements; p!= NULL; p = p->next, i_begin++){
		if(AlgebraElement_node_type(p) == ALGEBRA_HECKE_E){
			begin = p;
			j = 1;
			break;
		}
	}
	if(NULL == begin )return 0;

	for(i_end = i_begin; p!= NULL; p = p->next, i_end++){
		if(AlgebraElement_node_type(p) == ALGEBRA_HECKE_E){
			j ++;
		}else{
			i_end --;
			break;
		}
	}
	if(j <= 2)return 0;

	updated = 0;
	//MDEBUG("From %d to %d ", i_begin, i_end);
	/*
	 * ia = i+1;
	 * 
	 * */
	for(; i_begin != i_end-1; i_begin++){
		p = math_list_node_at(mo->elements, i_begin); 
		//char tmp[256];
		i = AlgebraElement_node_index(p) - 1;
		//MDEBUG("Round Check begin [%d][%s]", i_begin, AlgebraElement_toString(tmp, ALGEBRAELEMENT_NODE(p)));
		if(i <=0)continue;
		//n_small_1 = 0; //count of i-1
		//n_small_2 = 0; //count of i-1
		n_ip = 1;
		n_i = 0;
		pa = p;
		pb = NULL;
		pc = NULL;
		
		//end = math_list_node_at(mo->elements, i_end);
		//MDEBUG("Check begin [%d][%d]", i_begin+1, i_end);
		for(j_begin = i_begin + 1;  j_begin <= i_end; j_begin ++){
			q = math_list_node_at(mo->elements, j_begin); 
			//MDEBUG("Check[%s]", AlgebraElement_toString(tmp, ALGEBRAELEMENT_NODE(q)));
			qi = AlgebraElement_node_index(q);
			if(qi == i+2){
				//MDEBUG("Got i+2, break");
				break;
			}else if( qi == i+1){
				//MDEBUG("Got i+1");
				pc = q;
				n_ip ++;
			}else if(qi == i){
				//MDEBUG("Got i");
				pb = q;
				n_i ++;
			}

			//MDEBUG("n_ip: %d, n_i:%d", n_ip, n_i);
			//	i+1,..., i+1
			if((n_ip >=2) && (n_i == 0) ){
				//MDEBUG("Not OK, break.");
				break;
			}
			//	i,., i,
			if(n_i >=2){
				//MDEBUG("Not OK, break.");
				break;
			}
			
			//i+1, i-1, i, ... ,  i+1,
			if((n_ip == 2) && (n_i == 1)){
				/*
				MDEBUG("%p, %p, %p:(%p, %p, %p) Index : %d, %d, %d", pa, pb, pc,
					ALGEBRAELEMENT_NODE(pa),
					ALGEBRAELEMENT_NODE(pb),
					ALGEBRAELEMENT_NODE(pc), 
					ALGEBRAELEMENT_NODE(pa)->index ,
					ALGEBRAELEMENT_NODE(pb)->index ,
					ALGEBRAELEMENT_NODE(pc)->index 
				);
				*/
				ALGEBRAELEMENT_NODE(pa)->index --;
				/*
				MDEBUG("%p, %p, %p, Index : %d, %d, %d", pa, pb, pc,
					ALGEBRAELEMENT_NODE(pa)->index ,
					ALGEBRAELEMENT_NODE(pb)->index ,
					ALGEBRAELEMENT_NODE(pc)->index 
				);
				*/
				ALGEBRAELEMENT_NODE(pb)->index ++;
				/*
				MDEBUG("%p, %p, %p, Index : %d, %d, %d", pa, pb, pc,
					ALGEBRAELEMENT_NODE(pa)->index ,
					ALGEBRAELEMENT_NODE(pb)->index ,
					ALGEBRAELEMENT_NODE(pc)->index 
				);
				*/
				ALGEBRAELEMENT_NODE(pc)->index --;
				math_list_remove(pa);
				math_list_remove(pc);
				math_list_insert_before(pb, pa);
				math_list_insert_after(pb, pc);
				/*
				MDEBUG("%p, %p, %p, Index : %d, %d, %d", pa, pb, pc,
					ALGEBRAELEMENT_NODE(pa)->index ,
					ALGEBRAELEMENT_NODE(pb)->index ,
					ALGEBRAELEMENT_NODE(pc)->index 
				);
				*/
				//a = AlgebraElement_node(pa);
				//b = AlgebraElement_node(pb);
				//c = AlgebraElement_node(pc);
				//a->index --;
				//b->index ++;
				//c->index --;
				updated ++;
				mo->elements = math_list_begin(pb);
				//MDEBUG("OK, break.");
				break;
			}
			//MDEBUGP();
		}
		//MDEBUG("i_begin: %d", i_begin);
	}

	return updated;
}

/*
 * T_i TI- -->> T_i
 * */
int Hecke_reduce_II(Monomial *mo){
	math_list * p;
	AlgebraElement *a, *b;
	int updated = 0;
	
	//MDEBUGP();
	if((mo == NULL) || (mo->elements == NULL) || (mo->elements->next == NULL)){
		//MDEBUGP();
		return 0;
	}
	//MDEBUGP();
	for(p = mo->elements; p->next != NULL; p = p->next){
		//MDEBUGP();
		a = ALGEBRAELEMENT_NODE(p);
		b = ALGEBRAELEMENT_NODE(p->next);
		if( (a->type == ALGEBRA_HECKE_E)&&(b->type == ALGEBRA_HECKE_E) &&(a->index == b->index)){
			//char tmp1[16], tmp2[16];
			//MDEBUG("Got it, delete: %s, %s", AlgebraElement_toString(tmp1, a), AlgebraElement_toString(tmp2, b));

			math_list * tmp = p->next;
			math_list_delete_node(tmp);
			mo->elements = math_list_begin(p);
			updated++;
			break;
		}
	}
	//MDEBUG("Updated: %d", updated);
	return updated;
	
}

/*
 *
 * c_{i} T_{i} -->> T_{i} c_{i+1}
 * */
int HeckeCliff_reduce_CT(Monomial *mo){
	math_list * p;
	AlgebraElement *a, *b;
	int updated = 0;
	
	//MDEBUGP();
	if((mo == NULL) || (mo->elements == NULL) || (mo->elements->next == NULL)){
		//MDEBUGP();
		return 0;
	}
	//MDEBUGP();
	for(p = mo->elements; p->next != NULL; p = p->next){
		//char tmp1[256], tmp2[256];
		a = ALGEBRAELEMENT_NODE(p);
		b = ALGEBRAELEMENT_NODE(p->next);
		//MDEBUG("In CT %d:%d, %s, %s", a->type, b->type, AlgebraElement_toString(tmp1, a), AlgebraElement_toString(tmp2, b));
		if( (a->type == ALGEBRA_CLIFF_E)&&(b->type == ALGEBRA_HECKE_E) &&(a->index == b->index)){
			//MDEBUG("Got it.");
			a->index ++;
			math_list_exchange(p, p->next);
			mo->elements = math_list_begin(p);
			updated++;
			break;
		}
	}
	//MDEBUG("Updated: %d", updated);
	return updated;
	
}

int HeckeCliff_sort(Monomial *mo){
	int updated;
       
	do{
		updated = HeckeCliff_sort_round(mo);
		updated += Hecke_IJI(mo);
	}while(updated != 0);

	return 0;
	
}

/*
 * Assume it is sorted by value type;
 * It is now HeckeCliff together.
 *
 * */
int HeckeCliff_sort_round(Monomial *mo){
	math_list  * begin;
	math_list * end;
	AlgebraElement * a, *b;
	math_list *p, *q;
	int i, j;
	int begin_i, end_i;

	int updated = 0;
	//char tmp[256];

	begin = NULL;
	end = NULL;
	p = q = NULL;
	for(i=0, p = mo->elements; p != NULL; p = p->next, i++){
		//MDEBUG("Got list: %p, p %p", list, p);
		if(
			(AlgebraElement_node_type(p) == ALGEBRA_HECKE_E)
			|| (AlgebraElement_node_type(p) == ALGEBRA_CLIFF_E)
		){
			begin = p;
			begin_i = i;
			break;
		}
	}
	if(begin == NULL){
		return 0;
	}
	//MDEBUG("Got begin: %p", begin);
	for(p = begin->next, i++; (p != NULL); p = p->next, i++){
		//MDEBUG("check begin: %p:[%s_%d]", p, AlgebraElement_node_name(p), AlgebraElement_node_index(p));
		if(
			(AlgebraElement_node_type(p) != ALGEBRA_HECKE_E)
			&& (AlgebraElement_node_type(p) != ALGEBRA_CLIFF_E)
		){
			//MDEBUGP();
			end = p;
			end_i = i;
			break;
		}
	}
	if(end == NULL){
		end = p;
		end_i = i;
	}
	//MDEBUG("Got end: %p", end);
	
	
	//MDEBUG("before it: [%s]", Monomial_toString(tmp, mo));
	//for(p = begin; p != end; p = p->next){
	for(i = begin_i; i != end_i; i++){
		//MDEBUGP();
		p = math_list_node_at(mo->elements, i);
		if(NULL == p){
			break;
		}
		//Sort here.
		//MDEBUG("Round Got %p vs %p", p, q);
		//MDEBUG("Round Got p->next: %p", p->next);
		//for(q = p; q != end; q = q->next){
		for(j = i; j != end_i; j++){
			//MDEBUGP();
			q = math_list_node_at(mo->elements, j);
			if((NULL == q) || (NULL == q->next)){
				break;
			}
			//MDEBUGP();
			//MDEBUG("Round Got p %p:[%s_%d]", p, AlgebraElement_node_name(p), AlgebraElement_node_index(p));
			//MDEBUG("Round Got q %p:[%s_%d]", q, AlgebraElement_node_name(q), AlgebraElement_node_index(q));
			//Sort here.
			//if((q!= NULL) &&())
			a = q->data;
			b = q->next->data;
			//check = 0;
			if((a!= NULL) || (b != NULL)){
				//MDEBUG("before it: [%s]", Monomial_toString(tmp, mo));
				//MDEBUG("Comparing a:[%s%d] b:[%s%d]", a->name, a->index, b->name, b->index);
				if( (a->type == ALGEBRA_HECKE_E) && (b->type == ALGEBRA_HECKE_E) ){
					updated += HeckeCliff_T_T(mo, q);
					//MDEBUG("after change :%d  [%s]", updated, Monomial_toString(tmp, mo));
				}else if( (a->type == ALGEBRA_HECKE_E) && (b->type == ALGEBRA_CLIFF_E) ){
					updated += HeckeCliff_T_C(mo, q);
					//MDEBUG("after change :%d  [%s]", updated, Monomial_toString(tmp, mo));
				}else if( (a->type == ALGEBRA_CLIFF_E) && (b->type == ALGEBRA_HECKE_E) ){
					updated += HeckeCliff_C_T(mo, q);
					//MDEBUG("after change :%d  [%s]", updated, Monomial_toString(tmp, mo));
				}else if( (a->type == ALGEBRA_CLIFF_E) && (b->type == ALGEBRA_CLIFF_E) ){
					updated +=HeckeCliff_C_C(mo, q);
					//MDEBUG("after change :%d  [%s]", updated, Monomial_toString(tmp, mo));
				}
				//MDEBUGP();
				/*
				if(check == 1){
					//Exchange
					char tmp[128];
					updated ++;
					//MDEBUG("Need to change it:");
					math_list_exchange(q, q->next);
					mo->elements = math_list_begin(q);
					if(p == q){
						p = p->pre;
					}
					q = q->pre;
				}
				*/
			}
			//MDEBUGP();
		}
		//MDEBUGP();
	}
	
	//MDEBUG("Return : %d", updated);
	return updated;
}

int HeckeCliff_reduce(Monomial *mo){
	int updated;
	updated = 0;
	
	//char tmp[256];
	//MDEBUG("Before reduce: [%s]", Monomial_toString(tmp, mo));
	do{
		updated = HeckeCliff_sort(mo);
		//MDEBUG("after sort : %d, result [%s]", updated, Monomial_toString(tmp, mo));
		//updated += Hecke_IJI(mo);
		//MDEBUG("after IJI: %d, result [%s]", updated, Monomial_toString(tmp, mo));
		updated += Hecke_reduce_II(mo);
		//MDEBUG("after II: %d, result [%s]", updated, Monomial_toString(tmp, mo));
		updated += HeckeCliff_reduce_CT(mo);
		//MDEBUG("after CT: %d, result [%s]", updated, Monomial_toString(tmp, mo));
	}while(updated != 0);
	//MDEBUG("After  reduce: [%s]", Monomial_toString(tmp, mo));
	return updated;
}

//Return: NULL: mo is still mo.
//Not NULL: a list of monomials.
math_list * HeckeCliff_calculate(Monomial *mo){
	int updated;
	updated = 0;
	
	char tmp[256];
	math_list * res;
	MDEBUG("Before calculate: [%s]", Monomial_toString(tmp, mo));
	updated = HeckeCliff_sort(mo);
	MDEBUG("after sort : %d, result [%s]", updated, Monomial_toString(tmp, mo));
	//updated += Hecke_IJI(mo);
	//MDEBUG("after IJI: %d, result [%s]", updated, Monomial_toString(tmp, mo));
	res = Hecke_calculate_II(mo);
	MDEBUG("after II: %p, result [%s]", res, Monomial_toString(tmp, mo));
	if(res == NULL){
		MDEBUG("To calculate CT");
		res= HeckeCliff_calculate_CT(mo);
	}

	if(res != NULL){
		math_list * p;
		for(p = res; p != NULL; p = p->next){
			Monomial_sort(p->data);
			updated = HeckeCliff_sort(p->data);
			MDEBUG("after sort : %d, result [%s]", updated, Monomial_toString(tmp, p->data));
		}
	}
	return res;
}

//Return: NULL: mo is still mo.
//Not NULL: a list of monomials.
int HeckeCliff_PolyPolynomial_calculate(PolyPolynomial * poly){
	math_list * node;
	int updated;
	updated = 0;
	if(poly == NULL || poly->polymonomials == NULL){
		return 0;
	}

	//char tmp[STR_BUFFER];
	
	updated = 0;

	//TEXDEBUG("\n\n---%s : %d---%d elements \n\n", __FILE__, __LINE__,  math_list_length(poly->polymonomials));
	MDEBUG("updated, %d", updated);
	//MDEBUG("updated, %d, Polynomial = [%s]", updated, Polynomial_toString(tmp, poly));
	for(node = poly->polymonomials; node != NULL; node = node->next){
		PolyMonomial * pm = node->data;
		//TEXDEBUG("\n\n---%s : %d---node : %p \n\n", __FILE__, __LINE__,  node);
		//updated += HeckeCliff_sort(mo);
		//TEXDEBUG("\n\n---%s : %d---%p \n\n", __FILE__, __LINE__,  pm);
#if 0
		{
			char str[1024*16];
			PolyMonomial_toString(str, pm);
			TEXDEBUG("\n\n---%s : %d---%p [%s] \n\n", __FILE__, __LINE__,  pm, str);
		}
#endif
		do{
			updated = 0;
			updated += HeckeCliff_sort_round(pm->monomial);
			updated += Hecke_IJI(pm->monomial);
		}while(updated);
	}

	//TEXDEBUG("\n\n---%s : %d---%d elements \n\n", __FILE__, __LINE__,  math_list_length(poly->polymonomials));
	MDEBUG("updated, %d", updated);
	//MDEBUG("updated, %d, Polynomial = [%s]", updated, Polynomial_toString(tmp, poly));
	if(updated != 0){
		//MDEBUG("sorted: [%s]", Polynomial_toString(tmp, poly));
		//MDEBUG("Hecke Cliff sorted.");
		//return updated;
	}
	//MDEBUG("updated, %d, Polynomial = [%s]", updated, Polynomial_toString(tmp, poly));
	MDEBUG("updated, %d", updated);
	//updated += Hecke_IJI(mo);
	//MDEBUG("after IJI: %d, result [%s]", updated, Monomial_toString(tmp, mo));
	
	//TEXDEBUG("\n\n---%s : %d---%d elements \n\n", __FILE__, __LINE__,  math_list_length(poly->polymonomials));
	Hecke_calculate_monomial_func calculates[2] = {Hecke_calculate_II, HeckeCliff_calculate_CT};
	int i;
	for(i = 0; i<2; i++){
		//TEXDEBUG("\n\n---%s : %d---%d elements, %d \n\n", __FILE__, __LINE__,  math_list_length(poly->polymonomials), i);
		int j=0;
		for(node = poly->polymonomials; node != NULL; node = node->next){
			PolyMonomial * pm;
			Monomial * mo;
			math_list * res, *last;
			//TEXDEBUG("\n\n---%s : %d---node : %p \n\n", __FILE__, __LINE__,  node);
		       	
			pm = node->data;
		       	mo= pm->monomial;
			res = (calculates[i])(mo);
			
			//TEXDEBUG("\n\n---%s : %d---%d elements, %d, round %d \n\n", __FILE__, __LINE__,  math_list_length(poly->polymonomials), i, j++);
			if(res != NULL){
				//TEXDEBUG("\n\n---%s : %d---%d elements, %d \n\n", __FILE__, __LINE__,  math_list_length(poly->monomials), i);
				updated ++;
				Polynomial * p = Polynomial_new();
				p->monomials = res;
				Polynomial * tmp1 = Polynomial_mul_Polynomial(pm->coeff, p);
				PolyPolynomial * npp = PolyPolynomial_from_Polynomial(tmp1, ALGEBRA_HECKE_E);
#if 0
				TEXDEBUG("\n\n---PM becomes \n\n", __FILE__, __LINE__);
				TEXDEBUG("\n" FORMULAR_LEFT "\n&");
				PolyMonomial_Output(pm, stdout);
				TEXDEBUG(FORMULAR_RIGHT "\n");
				TEXDEBUG("\n" FORMULAR_LEFT "\n&");
				PolyPolynomial_Output(npp, stdout);
				TEXDEBUG(FORMULAR_RIGHT "\n");
				TEXDEBUG("\n\n---End of PM \n\n", __FILE__, __LINE__);
				//TEXDEBUG("\n\n---%s : %d---%d elements, %d \n\n", __FILE__, __LINE__,  math_list_length(poly->polymonomials), i);
#endif

#if 0
				{
					char s1[STR_BUFFER],  s2[STR_BUFFER];
					TEXDEBUG("TT: coeff: %p , %p [%s]", pm->coeff, pm->coeff->monomials, Polynomial_toString(s1, pm->coeff));
					int i;
					math_list * px;
					for(px = pm->coeff->monomials; px != NULL; px = px->next){
						Monomial * tmm = px->data;
						TEXDEBUG("TT: coeff Monomial: [ $%s$ ], length %d \n", Monomial_toString(s1, tmm), math_list_length(tmm->elements));
					}
					TEXDEBUG("TT: Monomial: [ $%s$ ], coeff: [$%s$]\n", Monomial_toString(s1, mo), Polynomial_toString(s2, pm->coeff));
					TEXDEBUG("TT: Mul list Monomial result: : [ $%s$ ],\n", Polynomial_toString(s1, tmp1));
					TEXDEBUG("TT: PM [ $%s$ ], PM: [$%s$]\n", 
							PolyMonomial_toString(s1, npp->polymonomials->data), 
							PolyMonomial_toString(s2, npp->polymonomials->next->data));

				}
#endif	
				//TEXDEBUG("\n\n---%s : %d---%d elements, %d \n\n", __FILE__, __LINE__,  math_list_length(poly->polymonomials), i);
				math_list * t;
				for(t = npp->polymonomials; t != NULL; t = t->next){
					math_list * nt;
					nt = math_list_node_from(t->data);
					math_list_insert_after(node, nt);
					//TEXDEBUG("\n\n---%s : %d--- new node %p \n\n", __FILE__, __LINE__,  nt);
					last = nt;
				}
				math_list_delete_node(node);
				if(node == poly->polymonomials){
					poly->polymonomials = math_list_begin(last);
				}
				node = last;
				//TEXDEBUG("\n\n---%s : %d---%d elements, %d \n\n", __FILE__, __LINE__,  math_list_length(poly->polymonomials), i);
				//MDEBUG("met II: [%s]", Polynomial_toString(tmp, poly));
			}
		}

	}
	//TEXDEBUG("\n\n---%s : %d---%d elements \n\n", __FILE__, __LINE__,  math_list_length(poly->polymonomials));
	if(updated != 0){
		return updated;
	}
	return updated;
}
//Return: NULL: mo is still mo.
//Not NULL: a list of monomials.
int HeckeCliff_Polynomial_calculate(Polynomial * poly){
	math_list * node;
	int updated;
	updated = 0;
	if(poly == NULL || poly->monomials == NULL){
		return 0;
	}

	//char tmp[STR_BUFFER];
	
	updated = 0;

	TEXDEBUG("\n\n---%s : %d---%d elements \n\n", __FILE__, __LINE__,  math_list_length(poly->monomials));
	MDEBUG("updated, %d", updated);
	//MDEBUG("updated, %d, Polynomial = [%s]", updated, Polynomial_toString(tmp, poly));
	for(node = poly->monomials; node != NULL; node = node->next){
		Monomial * mo = node->data;
		//updated += HeckeCliff_sort(mo);
		updated += HeckeCliff_sort_round(mo);
		updated += Hecke_IJI(mo);
	}

	//TEXDEBUG("\n\n---%s : %d---%d elements \n\n", __FILE__, __LINE__,  math_list_length(poly->monomials));
	MDEBUG("updated, %d", updated);
	//MDEBUG("updated, %d, Polynomial = [%s]", updated, Polynomial_toString(tmp, poly));
	if(updated != 0){
		//MDEBUG("sorted: [%s]", Polynomial_toString(tmp, poly));
		//MDEBUG("Hecke Cliff sorted.");
		return updated;
	}
	//MDEBUG("updated, %d, Polynomial = [%s]", updated, Polynomial_toString(tmp, poly));
	MDEBUG("updated, %d", updated);
	//updated += Hecke_IJI(mo);
	//MDEBUG("after IJI: %d, result [%s]", updated, Monomial_toString(tmp, mo));
	
	TEXDEBUG("\n\n---%s : %d---%d elements \n\n", __FILE__, __LINE__,  math_list_length(poly->monomials));
	Hecke_calculate_monomial_func calculates[2] = {Hecke_calculate_II, HeckeCliff_calculate_CT};
	int i;
	for(i = 0; i<2; i++){
		//TEXDEBUG("\n\n---%s : %d---%d elements, %d \n\n", __FILE__, __LINE__,  math_list_length(poly->monomials), i);
		int j=0;
		for(node = poly->monomials; node != NULL; node = node->next){
			Monomial * mo;
			math_list * res, *t;
		       	
			mo = node->data;
			//res = Hecke_calculate_II(mo);
			res = (calculates[i])(mo);
			
			//TEXDEBUG("\n\n---%s : %d---%d elements, %d, round %d \n\n", __FILE__, __LINE__,  math_list_length(poly->monomials), i, j++);
			if(res != NULL){
				//TEXDEBUG("\n\n---%s : %d---%d elements, %d \n\n", __FILE__, __LINE__,  math_list_length(poly->monomials), i);
				//MDEBUG("Hecke Cliff II found");
				updated ++;
				math_list * last;
				last = node;
				for(t = res; t != NULL; t = t->next){
					math_list * nt;
					nt = math_list_node_from(t->data);
					//MDEBUGP();
					math_list_insert_after(last, nt);
					last = nt;
				}
				//TEXDEBUG("\n\n---%s : %d---%d elements, %d \n\n", __FILE__, __LINE__,  math_list_length(poly->monomials), i);
				math_list_delete_node(node);
				if(node == poly->monomials){
					poly->monomials = math_list_begin(last);
				}
				node = last;
				//TEXDEBUG("\n\n---%s : %d---%d elements, %d \n\n", __FILE__, __LINE__,  math_list_length(poly->monomials), i);
				//MDEBUG("met II: [%s]", Polynomial_toString(tmp, poly));
			}
		}

	}
	TEXDEBUG("\n\n---%s : %d---%d elements \n\n", __FILE__, __LINE__,  math_list_length(poly->monomials));
	if(updated != 0){
		return updated;
	}
	return updated;
	/*
	//MDEBUG("after II: %p, result [%s]", res, Monomial_toString(tmp, mo));
	if(res == NULL){
		Monomial * mo;
		MDEBUG("To calculate CT");
		res= HeckeCliff_calculate_CT(mo);
	}

	if(res != NULL){
		math_list * p;
		for(p = res; p != NULL; p = p->next){
			Monomial_sort(p->data);
			updated = HeckeCliff_sort(p->data);
			MDEBUG("after sort : %d, result [%s]", updated, Monomial_toString(tmp, p->data));
		}
	}
	return res;
	*/
}
/*
 * T_i T_i =  q T_i - T_i + q
 * */
math_list * Hecke_calculate_II(Monomial *mo){
	math_list * res, *p;
	AlgebraElement *a, *b;
	Monomial * left, *right;

	//MDEBUGP();
	if((mo == NULL) || (mo->elements == NULL) || (mo->elements->next == NULL)){
		//MDEBUGP();
		return NULL;
	}
	
	//MDEBUGP();
	res = NULL;
	for(p = mo->elements; p->next != NULL; p = p->next){
		//MDEBUGP();
		a = ALGEBRAELEMENT_NODE(p);
		b = ALGEBRAELEMENT_NODE(p->next);
		if( (a->type == ALGEBRA_HECKE_E)&&(b->type == ALGEBRA_HECKE_E) &&(a->index == b->index)){
			char tmp1[16], tmp2[16];
			MDEBUG("Got it, process: %p:%p, %s, %s", p, p->next, 
					AlgebraElement_toString(tmp1, a), AlgebraElement_toString(tmp2, b));
			left = Monomial_new();
			right = Monomial_new();
			//X X ... X       T_i T_i X X ....X
			//mo->elements, ->begin   end ->
			MDEBUG("Got sublist: %p:%p, [%s, %s)", mo->elements, p, 
					AlgebraElement_toString(tmp1, mo->elements->data), 
					AlgebraElement_toString(tmp2, p->data));
			left->elements = math_list_sublist(mo->elements, mo->elements, p);
			left->coeff = mo->coeff;
			right->elements = math_list_sublist(mo->elements, p->next->next, NULL);
			MDEBUG("Left  length %d", math_list_length(left->elements));
			MDEBUG("right length %d", math_list_length(right->elements));
			
			{
				char tmp[256];
				MDEBUG("sub left  : [%s]", Monomial_toString(tmp, left));
				MDEBUG("sub right : [%s]", Monomial_toString(tmp, right));
			}
			
			AlgebraElement * T_i = HeckeElement_new(a->index);
			//AlgebraElement * v_q = AlgebraElement_from_string("q");
			//v_q->type = VARIABLE_NOINDEX;
			AlgebraElement * v_q = Parameter_Variable_new("q");
			Monomial * m, *m_right;
			
			//char tmp[256];
			//MDEBUGP();
			//m_left = Monomial_mul_Monomial(left, m1);
			//m_right = Monomial_mul_Monomial(m_left, right);
			m = AlgebraElement_mul_AlgebraElement(v_q, T_i);
			m_right = Monomial_mul_list(3, left, m, right);
			//MDEBUG("Result       :[%s]", Monomial_toString(tmp, m_right));
			res = math_list_append_from_data(res, m_right);
			//MDEBUGP();

			//m_left = Monomial_mul_Monomial(left, m2);
			//m_right = Monomial_mul_Monomial(m_left, right);
			m = Number_mul_AlgebraElement(-1, T_i);
			m_right = Monomial_mul_list(3, left, m, right);
			//MDEBUG("Result       :[%s]", Monomial_toString(tmp, m_right));
			res = math_list_append_from_data(res, m_right);

			//m_left = Monomial_mul_Monomial(left, m3);
			//m_right = Monomial_mul_Monomial(m_left, right);
			m = Monomial_from_AlgebraElement(v_q);
			m_right = Monomial_mul_list(3, left, m, right);
			//MDEBUG("Result       :[%s]", Monomial_toString(tmp, m_right));
			res = math_list_append_from_data(res, m_right);
			return res;
		}
	}
	//MDEBUG("Updated: %d", updated);
	return NULL;
}
/*
 *
 * c_{i} T_{i} = T_{i} c_{i+1} 
 * */
math_list * HeckeCliff_calculate_CT(Monomial *mo){
	math_list * res, *p;
	AlgebraElement *a, *b;
	Monomial * left, *right;
	
	if((mo == NULL) || (mo->elements == NULL) || (mo->elements->next == NULL)){
		//MDEBUGP();
		return NULL;
	}
	//MDEBUGP();
	res = NULL;
	for(p = mo->elements; p->next != NULL; p = p->next){
		//char tmp1[256], tmp2[256];
		a = ALGEBRAELEMENT_NODE(p);
		b = ALGEBRAELEMENT_NODE(p->next);
		//MDEBUG("In CT %d:%d, %s, %s", a->type, b->type, AlgebraElement_toString(tmp1, a), AlgebraElement_toString(tmp2, b));
		if( (a->type == ALGEBRA_CLIFF_E)&&(b->type == ALGEBRA_HECKE_E) &&(a->index == b->index)){
			//MDEBUG("Got it, process: %s, %s", AlgebraElement_toString(tmp1, a), AlgebraElement_toString(tmp2, b));
			left = Monomial_new();
			right = Monomial_new();
			//X X ... X       T_i T_i X X ....X
			//mo->elements, ->begin   end ->
			left->elements = math_list_sublist(mo->elements, mo->elements, p);
			left->coeff = mo->coeff;
			right->elements = math_list_sublist(mo->elements, p->next->next, NULL);
			
			{
				char tmp[256];
				MDEBUG("sub left  : [%s]", Monomial_toString(tmp, left));
				MDEBUG("sub right : [%s]", Monomial_toString(tmp, right));
			}
			//b: T_i, a: c_i
			AlgebraElement * T_i = HeckeElement_new(b->index);
			AlgebraElement * c_ip = CliffElement_new(a->index + 1);
			AlgebraElement * c_i = CliffElement_new(a->index );
			//AlgebraElement * v_q = AlgebraElement_from_string("q");
			//v_q->type = VARIABLE_NOINDEX;
			AlgebraElement * v_q = Parameter_Variable_new("q");

			Monomial * m, *m_res;
			/*
			{
				char tmp[256];
				MDEBUG("Mul: [%s]", Monomial_toString(tmp, m1));
				MDEBUG("Mul: [%s]", Monomial_toString(tmp, m2));
				MDEBUG("Mul: [%s]", Monomial_toString(tmp, m3));		
			}
			*/
			//char tmp[256];
			//MDEBUGP();
			m = AlgebraElement_mul_AlgebraElement(T_i, c_ip);
			m_res = Monomial_mul_list(3, left, m, right);
			//MDEBUG("Result       :[%s]", Monomial_toString(tmp, m_res));
			res = math_list_append_from_data(res, m_res);

			m = AlgebraElement_mul_AlgebraElement(v_q, c_i);
			m_res = Monomial_mul_list(3, left, m, right);
			//MDEBUG("Result       :[%s]", Monomial_toString(tmp, m_res));
			res = math_list_append_from_data(res, m_res);

			m = Number_mul_AlgebraElement(-1, c_i);
			m_res = Monomial_mul_list(3, left, m, right);
			//MDEBUG("Result       :[%s]", Monomial_toString(tmp, m_res));
			res = math_list_append_from_data(res, m_res);

			m = AlgebraElement_mul_AlgebraElement(v_q, c_ip);
			m->coeff = -1;
			m_res = Monomial_mul_list(3, left, m, right);
			//MDEBUG("Result       :[%s]", Monomial_toString(tmp, m_res));
			res = math_list_append_from_data(res, m_res);

			m = Number_mul_AlgebraElement(1, c_ip);
			m_res = Monomial_mul_list(3, left, m, right);
			//MDEBUG("Result       :[%s]", Monomial_toString(tmp, m_res));
			res = math_list_append_from_data(res, m_res);

			//MDEBUGP();

			return res;
		}
	}
	//MDEBUG("Updated: %d", updated);
	return NULL;
	
}

int  HeckeCliff_compare(Monomial *a, Monomial *b){
	int r;
	r = Monomial_compare_type(a, b, ALGEBRA_HECKE_E);

#if 0
	{
		char t1[128], t2[128];
		Monomial_toString(t1, a);
		Monomial_toString(t2, b);
		MDEBUG("%d: %s -->> %s ", r, t1, t2);
	}
#endif
	return r;
}
int  HeckeCliff_Polynomial_sort(Polynomial *poly){
	int updated;
	if(NULL == poly || NULL == poly->monomials)
		return 0;

	updated = math_list_sort_increase(poly->monomials, NULL, (math_list_compare_func)HeckeCliff_compare);
	poly->monomials = math_list_begin(poly->monomials);
	return updated;
}
