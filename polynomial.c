#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>

#include "utils.h"
#include "math_list.h"
#include "logs.h"
#include "algebra.h"
#include "polynomial.h"
#include "equation.h"

AlgebraElement * AlgebraElement_new(){
	AlgebraElement * res = (AlgebraElement *)malloc_zero(sizeof(AlgebraElement));
	AlgebraElement_init(res);
	return res;
}

AlgebraElement * Parameter_Variable_new(const char * name){
	AlgebraElement * v_q = AlgebraElement_from_string(name);
	v_q->type = VARIABLE_NOINDEX;
	return v_q;
}
void Monomial_delete(Monomial * mo){
	if(mo == NULL){
		return;
	}
	if(mo->elements != NULL){
		math_list * node;
		math_list *p;
		for(p = mo->elements; p != NULL; ){
			node = p;
			free(p->data);
			p = p->next;
			free(node);
		}
	}
	mo->elements = NULL;
	free(mo);
}
AlgebraElement * AlgebraElement_init(AlgebraElement * res){
	res->type = VARIABLE_E;
	res->infix[0] = '_';
	res->index = 0;
	res->power = 1;
	//res->size = sizeof(AlgebraElement);
	return res;
}
AlgebraElement * AlgebraElement_from_string(const char * str){
	AlgebraElement * res = AlgebraElement_new();

	uint32_t i;

	AlgebraElement_init(res);
	for(i=0; (str[i]!=0) &&( str[i] != '_'); i++ ){
		res->name[i] = str[i];
	}

	if(str[i] == '_'){
		i++;
	}
	if(ZHM_IS_NUMBER(str[i])){
		res->index = atoi((str + i));
	}
	return res;
}

math_list *node_from_Monomial(Monomial * mo){
	math_list * node = math_list_node_new();
	node->data = Monomial_copy(mo);
	return node;
}

/*
AlgebraElement * AlgebraElement_copy(AlgebraElement * ele){
	AlgebraElement * res = AlgebraElement_new();
	if(NULL != res){
		memcpy(res, ele, sizeof(AlgebraElement));
	}
	return res;
}
*/
math_list *node_from_AlgebraElement(AlgebraElement * ele){
	math_list * node = math_list_node_new();
	AlgebraElement * nele = AlgebraElement_copy(ele);
	node->data = nele;
	return node;
}
ElementType AlgebraElement_node_type(math_list *node){
	if(NULL == node) return NONE_TYPE;

	AlgebraElement * e = node->data;
	return e->type;
}
char * AlgebraElement_node_name(math_list *node){
	if(NULL == node) return NULL;

	AlgebraElement * e = node->data;
	return e->name;
}
int AlgebraElement_node_index(math_list *node){
	if(NULL == node) return -1; 

	AlgebraElement * e = node->data;
	return e->index;
}
AlgebraElement * AlgebraElement_node(math_list *node){
	if(NULL == node) return NULL;

	AlgebraElement * e = node->data;
	return e;
}

char *  AlgebraElement_toString(char * res, AlgebraElement * ele){
	res[0] = 0;
	if(ele == NULL){
		return res;
	}
	char indf[128], powf[128];

	if((ele->index  == 0 ) && (ele->type ==VARIABLE_NOINDEX) ){
		indf[0] = 0;
	}else{
		sprintf(indf,"_{%d}",  ele->index);
	}
	//if( ele->index != -1){
	//	sprintf(indf,"_{%d}",  ele->index);
	//}else{
	//	indf[0] = 0;
	//}

	if(ele->power != 1){
		sprintf(powf, "^{%d}",  ele->power);
	}else{
		powf[0] = 0;
	}
	sprintf(res, "%s%s%s", ele->name, indf, powf);
	return res;
}

Monomial * Monomial_copy(Monomial * mo){
	Monomial * nm;

	if(NULL == mo ){
		return NULL;
	}

	nm = Monomial_new();
	nm->coeff = mo->coeff;
	
	if(NULL == mo->elements){
		//MDEBUG("IS NULL ELEMENT");
		return nm;
	}
	

	math_list * p;
	nm->elements = NULL;
	for(p = mo->elements; p != NULL; p = p->next){
		AlgebraElement * nele = AlgebraElement_copy(p->data);
		nm->elements = math_list_append_from_data(nm->elements, nele);
	}
	//nm->elements = math_list_copy(mo->elements);
	/*
	{
		char tmp[128];
		MDEBUG("Copy from : %p:[%s]", mo, Monomial_toString(tmp, mo));
		MDEBUG("Copy to   : %p:[%s]", nm, Monomial_toString(tmp, nm));
	
	}
	*/
	return nm;
}

Monomial* Monomial_new(){
	Monomial * res = (Monomial *)malloc_zero(sizeof(Monomial));
	res->coeff = 1;
	return res;
	//MDEBUG("Construct: %p", this);
}
/*
void Monomial_free(Monomial * mo){
	//Monomial * mo = *mop;
	math_list_free(mo->elements);
	free(mo);
	return ;
	//MDEBUG("Construct: %p", this);
}
*/

Monomial * Monomial_from_AlgebraElement(AlgebraElement * ele){
	if(NULL == ele)
		return NULL;
	
	Monomial * res = Monomial_new();
	res->coeff = 1;
	math_list * node = node_from_AlgebraElement(ele);
	res->elements = node;
	return res;
}
Monomial * Monomial_from_string(const char * str){
	char *s2;
	int len;
	int index;
	Monomial *res;
	AlgebraElement * ele;
	
	//char tmp[256];
	res = Monomial_new();

	if(str == NULL) return NULL;
	len = strlen(str);
	s2 = malloc_zero(len + 2);
	strcpy(s2, str);
	//MDEBUG("string: [%s]", str);
	s2 = strtok(s2, " ");
	//MDEBUG("%p", s2);

	while(s2){
		//MDEBUG("Got %s", s2);
		if(*s2 == '-' ){
			int num = atoi(s2+1);
			res->coeff = num*(-1);
		}else 
		if(ZHM_IS_NUMBER(*s2) ){
			int num = atoi(s2);
			res->coeff = num;
		}else{
			ele = NULL;
			switch(*s2){
				case 'T':
					index = atoi(s2 + 2);
					//MDEBUG("Got Hecke T_%d", index);
					ele = HeckeElement_new(index);
					break;
				case 'c':
					index = atoi(s2 + 2);
					//MDEBUG("Got Cliff c_%d", index);
					ele = CliffElement_new(index);
					break;
				default:
					ele = AlgebraElement_from_string(s2);
					//MDEBUG("Got other [%s][%s], ", s2, AlgebraElement_toString(tmp, ele));
					break;
			}
			if(ele != NULL){
				math_list * node = node_from_AlgebraElement(ele);
				res->elements = math_list_append(res->elements, node);
			}
		
		}


		//{
		//	char tmp[128];
		//	MDEBUG("Result: %s", Monomial_toString(tmp, res));
		//}
		s2 = strtok(NULL, " ");
	}
	return res;
}


Monomial * Monomial_from_Number(Number n){
	Monomial * res = Monomial_new();
	res->coeff = n;
	return res;
}
int Monomial_sortType(Monomial * mo){
	math_list *p;
	int updated;
	if(NULL == mo)return 0;

	updated = 0;
	for(p = mo->elements; p != NULL; p = p->next){
		if(AlgebraElement_node_type(p) > VARIABLE_E){
			//Need to mode variables after it into before.
			math_list * var;
			for(var = p->next; var != NULL; var = var->next){
				if(AlgebraElement_node_type(var) <= VARIABLE_E){
					math_list_remove(var);
					math_list_insert_before(p, var);
					mo->elements = math_list_begin(p);
					updated ++;
				}
			}
		}
	}
	mo->elements = math_list_begin(mo->elements);
	return updated;
}
int Monomial_sortVariable(Monomial *mo){
	math_list *p1;
	math_list *last=NULL;
	int updated;

	if(NULL == mo || NULL == mo->elements )return 0;

	for(p1 = mo->elements; (p1 != NULL) && (AlgebraElement_node_type(p1) <= VARIABLE_E); p1 = p1->next);
	if(p1 == mo->elements) return 0;
	
	//MDEBUG("p1: %p", p1);
	if(p1 == NULL){
		last = math_list_end(mo->elements);
	}else{
		last = p1->pre;
	}
	//math_list_sort_increase(mo->elements, last, AlgebraElement_compare_variable );
	updated = math_list_sort_increase(mo->elements, last, (math_list_compare_func)AlgebraElement_compare_variable );
	mo->elements = math_list_begin(mo->elements);

	AlgebraElement * last_e, *ele;
	last_e = NULL;
	for(p1 = mo->elements; (p1 != NULL) && (AlgebraElement_node_type(p1) <= VARIABLE_E); p1 = p1->next){
		ele = p1->data;
		if(last_e != NULL){
			if(AlgebraElement_compare_variable(ele, last_e) == 0){
				math_list * tmp;
				tmp = p1;
				p1 = p1->pre;
				last_e->power += ele->power;
				math_list_remove(tmp);
				free(tmp->data);
				free(tmp);
			}
		}

		last_e = ele;
	}

	return updated;
}

#if 0
void Monomial_sortVariable1(Monomial *mo){
	math_list *p1, *p2;
	math_list *last=NULL;
	if(NULL == mo)return ;
	for(p1 = mo->elements; (p1 != NULL) && (AlgebraElement_node_type(p1) == VARIABLE_E); p1 = p1->next){
		MDEBUG("p1:%p, last:%p", p1, last);
		MDEBUG("p1:%p(%s_%d)",  p1, AlgebraElement_node(p1)->name, AlgebraElement_node(p1)->index);
	}
	last = p1;
	//MDEBUG("p1:%p, last:%p", p1, last);
	//int j=0;
	for(p1 = mo->elements; (p1 != NULL) && (AlgebraElement_node_type(p1) == VARIABLE_E);  p1 = p1->next){
		math_list *t1;
		
		/*
		int i=0;
		j = j+1;

		if(j>100)break;
		MDEBUG("j=%d, p1:%p(%s_%d)last:%p(%s_%d)", j, 
					p1, AlgebraElement_node(p1)->name, AlgebraElement_node(p1)->index,
					last, AlgebraElement_node(last)->name, AlgebraElement_node(last)->index
				);
		for(math_list *q =  mo->elements; q!= NULL; q = q->next){
			printf("%p, ", q);
		}
		printf("\n");
		*/
		for(p2 = p1->next; (p2 != last); ){
			/*
			i = i +1;
			MDEBUG("i=%d, p1:%p(%s_%d), p2:%p(%s_%d), last:%p(%s_%d)", i, 
					p1, AlgebraElement_node(p1)->name, AlgebraElement_node(p1)->index,
					p2, AlgebraElement_node(p2)->name, AlgebraElement_node(p2)->index,
					last, AlgebraElement_node(last)->name, AlgebraElement_node(last)->index
				);
			if(i> 100)break;
			*/
			if(AlgebraElement_compare_variable(p1->data, p2->data) >  0){
				//MDEBUG("p1 > p2");
				t1 = p2->next;
				math_list_exchange(p1, p2);
				mo->elements = math_list_begin(p1);
				p1 = p2;
				p2 = t1;
			}else{
				//MDEBUG("p1 < p2");
				p2 = p2->next;
			}
			//MDEBUG("After round p1:%p(%s_%d), p2:%p(%s_%d), last:%p(%s_%d)",  
			//		p1, AlgebraElement_node(p1)->name, AlgebraElement_node(p1)->index,
			//		p2, AlgebraElement_node(p2)->name, AlgebraElement_node(p2)->index,
			//		last, AlgebraElement_node(last)->name, AlgebraElement_node(last)->index
			//	);
		}
		//MDEBUG("After j=%d, p1:%p(%s_%d)last:%p(%s_%d)", j, 
		//			p1, AlgebraElement_node(p1)->name, AlgebraElement_node(p1)->index,
		//			last, AlgebraElement_node(last)->name, AlgebraElement_node(last)->index
		//		);

	}
}
#endif
#if 0
int HeckeCliff_relation1(Monomial *mo, math_list  * begin, math_list  * end){
	AlgebraElement * a, *b;
	math_list *p, *q;
	
	int updated = 0;
	int check;

	p = q = NULL;

	for(p = begin; p != end; p = p->next){
		//Sort here.
		//MDEBUG("Round Got %p vs %p", p, q);
		//MDEBUG("Round Got p->next: %p", p->next);
		for(q = p; q != end; q = q->next){
			//MDEBUG("Round Got p %p:[%s_%d]", p, AlgebraElement_node_name(p), AlgebraElement_node_index(p));
			//MDEBUG("Round Got q %p:[%s_%d]", q, AlgebraElement_node_name(q), AlgebraElement_node_index(q));
			//Sort here.
			//MDEBUG("Got %p vs %p", p, q);
			//if((q!= NULL) &&())
			a = q->data;
			b = q->next->data;
			check = 0;
			if((a!= NULL) || (b != NULL)){
				//MDEBUG("Comparing a:[%s%d] b:[%s%d]", a->name, a->index, b->name, b->index);
				if( (a->type == ALGEBRA_HECKE_E) && (b->type == ALGEBRA_HECKE_E) ){
					// a: T_3,  b : T_1
					if(b->index + 1 <  a->index){
						check = 1;
					}
				}else if( (a->type == ALGEBRA_HECKE_E) && (b->type == ALGEBRA_CLIFF_E) ){
					// a: T_3,  b : c_2, c_5
					if( (b->index <  a->index) || (a->index + 1 < b->index) ){
						//check = 1;
					}
				
				}else if( (a->type == ALGEBRA_CLIFF_E) && (b->type == ALGEBRA_HECKE_E) ){
					// a : c_2, c_5, b:T_3
					if( (a->index <  b->index) || (b->index + 1 < a->index) ){
						check = 1;
					}
				}
				/*
				else if( (a->type == ALGEBRA_CLIFF_E) && (b->type == ALGEBRA_CLIFF_E) ){
					// a : c_2, b:c_1
					if(a->index > b->index){
						check = 1;
						mo->coeff = mo->coeff * (-1);
					}
				}
				*/

				if(check == 1){
					char tmp[128];
					updated ++;
					//MDEBUG("Need to change it:");
					math_list_exchange(q, q->next);
					mo->elements = math_list_begin(q);
					MDEBUG("after change : [%s]", Monomial_toString(tmp, mo));
					if(p == q){
						p = p->pre;
					}
					q = q->pre;
				}
			}
		}
	}

	return 0;
}

/*
 *	c_i * c_i = -1;
 *	c_i * c_j = c_j * c_i;
 *
 * */
int HeckeCliff_relation2(Monomial *mo, math_list  * begin, math_list  * end){
	AlgebraElement * a, *b;
	math_list *p, *q;
	char tmp[128];
	
	int updated = 0;
	int check;

	p = q = NULL;

	for(p = begin; p != end; p = p->next){
		for(q = p; q != end; q = q->next){
			a = q->data;
			b = q->next->data;
			check = 0;
			if((a!= NULL) || (b != NULL)){
				if( (a->type == ALGEBRA_CLIFF_E) && (b->type == ALGEBRA_CLIFF_E) ){
					// a : c_2, b:c_1
					if(a->index > b->index){
						check = 1;
						mo->coeff = mo->coeff * (-1);
					}
					else if(a->index == b->index){
						check = 2;
						mo->coeff = mo->coeff * (-1);
					}
				}

				if(check == 1){
					updated ++;
					//MDEBUG("Need to change it:");
					math_list_exchange(q, q->next);
					mo->elements = math_list_begin(q);
					MDEBUG("changed : [%s]", Monomial_toString(tmp, mo));
					if(p == q){
						p = p->pre;
					}
					q = q->pre;
				}else if(check == 2){
					updated ++;
					MDEBUG("Need to remove  it:");
					math_list *tmp1 = q->next;
					math_list *tmp2 = q->next->next;
					
					math_list_remove(q);
					math_list_remove(tmp1);
					q = tmp2;
					if(q != NULL) q = q->pre;
					//free(p);
					//free(q);
					mo->elements = math_list_begin(p);
					MDEBUG("changed: [%s]", Monomial_toString(tmp, mo));
				}
			}
		}
	}

	return 0;
}
/*
 *	c_{i+1} T_i = T_i * c_i;
 *
 * */
int HeckeCliff_relation3(Monomial *mo, math_list  * begin, math_list  * end){
	AlgebraElement * a, *b;
	math_list *p, *q;
	char tmp[128];
	
	int updated = 0;
	int check;

	p = q = NULL;

	for(p = begin; p != end; p = p->next){
		for(q = p; q != end; q = q->next){
			a = q->data;
			b = q->next->data;
			check = 0;
			if((a!= NULL) || (b != NULL)){
				if( (a->type == ALGEBRA_CLIFF_E) && (b->type == ALGEBRA_HECKE_E) ){
					// a : c_2, b:T_1
					if(a->index > b->index){
						check = 1;
						mo->coeff = mo->coeff * (-1);
					}
					else if(a->index == b->index){
						check = 2;
						mo->coeff = mo->coeff * (-1);
					}
				}

				if(check == 1){
					updated ++;
					//MDEBUG("Need to change it:");
					math_list_exchange(q, q->next);
					mo->elements = math_list_begin(q);
					MDEBUG("changed : [%s]", Monomial_toString(tmp, mo));
					if(p == q){
						p = p->pre;
					}
					q = q->pre;
				}else if(check == 2){
					updated ++;
					MDEBUG("Need to remove  it:");
					math_list *tmp1 = q->next;
					math_list *tmp2 = q->next->next;
					
					math_list_remove(q);
					math_list_remove(tmp1);
					q = tmp2;
					if(q != NULL) q = q->pre;
					//free(p);
					//free(q);
					mo->elements = math_list_begin(p);
					MDEBUG("changed: [%s]", Monomial_toString(tmp, mo));
				}
			}
		}
	}

	return 0;
}
int HeckeCliff_relation5(Monomial *mo, math_list  * begin, math_list  * end){

	return 0;
}
int HeckeCliff_relation4(Monomial *mo, math_list  * begin, math_list  * end){

	return 0;
}
#endif
int Monomial_sort(Monomial * mo){
	int updated = 0;
	//char tmp[256];

	//MDEBUG("before sort: [%s]", Monomial_toString(tmp, mo));
	do{
		updated = 0;
		updated += Monomial_sortType(mo);
		updated += Monomial_sortVariable(mo);
	}while(updated != 0);
	//MDEBUG("after  sort: [%s]", Monomial_toString(tmp, mo));
	return updated;
}

int PolyMonomial_sort(PolyMonomial * pm){
	int updated = 0;
	//char tmp[256];
	math_list * p;
	Monomial * mo;

	if(pm == NULL) return 0;
	if(pm->coeff != NULL){
		for(p = pm->coeff->monomials; p != NULL; p = p->next){
			mo = p->data;
			updated += Monomial_sort(mo);
		}
	}
	mo = pm->monomial;
	updated += Monomial_sort(mo);

	return updated;
}

void Monomial_Output( Monomial * m, FILE * fp){
	char str[STR_BUFFER];
	Monomial_toString(str, m);
	fprintf(fp, str);
	//fflush(fp);
	return ;
}
char * Monomial_toString(char *res, Monomial * m){
	math_list * p;
	if(NULL == res || NULL  == m)
		return NULL;

	if((m->elements == NULL )||(math_list_length(m->elements) == 0)){
		sprintf(res, "%d", m->coeff);
		return res;
	}
	res[0] = 0;

	//MDEBUGP();
	
	switch(m->coeff ){
		case 1:
			res[0] = 0;
			break;
		case -1:
			strcpy(res, "-");
			break;
		default:
			sprintf(res, "%d", m->coeff);
			break;
	}
	/*
	if(m->coeff != 1){
		sprintf(res, "%d", m->coeff);
	}else{
		res[0] = 0;
	}*/

	//MDEBUG("Result part:[%s]", res);
	for(p = m->elements; p != NULL; p = p ->next){
		char tmp[16];
		AlgebraElement * ele = p->data;
		AlgebraElement_toString(tmp, ele);
		strcat(res, " ");
		strcat(res, tmp);
	}
	if(res[0] == 0){
		res[0] = '1';
		res[1] = 0;
	}
	return res;
}

int  AlgebraElement_compare_variable(AlgebraElement *a, AlgebraElement *b){
	//return AlgebraElement_compare_variable_with_type(a, b, NONE_TYPE);
	return AlgebraElement_compare_variable_with_type(a, b, VARIABLE_E);
}
int  AlgebraElement_compare(AlgebraElement *a, AlgebraElement *b){
	return AlgebraElement_compare_variable_with_type(a, b, ALGEBRA_ALL);
	//return AlgebraElement_compare_variable_with_type(a, b, VARIABLE_E);
}

/*	Assume a->type == b->type == VARIABLE_E
 * 	Return value:
 *	0, equal
 *	1, a > b
 *	-1, a < b 
 * */
int  AlgebraElement_compare_variable_with_type(AlgebraElement *a, AlgebraElement *b, ElementType type){
	int t;

	if(a == NULL){
		if(b == NULL)return 0;
		return -1;
	}
	if(b == NULL)
		return 1;

	//std::cout<<"Comparing:"<<this->toString()<<" with "<<other.toString()<<";"<<std::endl;
	if(type > NONE_TYPE){
		if((a->type <= type) && (b->type <= type)){
			t = a->type - b->type;
			//MDEBUGP();
			if(t != 0){
				return t;
			}
		}
	}
	//MDEBUGP();
#if 0
	{
		char tmp1[128], tmp2[128];
		AlgebraElement_toString(tmp1, a);
		AlgebraElement_toString(tmp2, b);
		MDEBUG("In it[%s] vs [%s]", tmp1, tmp2);
	}
#endif
	//MDEBUG("a:%p(%s_%d), b:%p(%s_%d). ", a, a->name, a->index,b, b->name, b->index	);
	t = strcmp(a->name, b->name);
	//MDEBUG("str compare = %d", t);
	if(t != 0){
		return t;
	}
	//MDEBUGP();
	t = a->index - b->index;
	//MDEBUG("index compare = %d", t);
	if(t != 0){
		return t;
	}
	t = a->power - b->power;
	//MDEBUG("index compare = %d", t);
	if(t != 0){
		return t;
	}

	//MDEBUG("Return 0.");
	
	return 0;
}

int  Monomial_similar(Monomial *a, Monomial *b){
	if(a == NULL) {
		if(b != NULL)
			return -1;
		return 0;
	}
	if(b == NULL) return 1;
	//math_list * pa, *pb;
	int r;
#if 0
	{
		char tmp1[128], tmp2[128];
		Monomial_toString(tmp1, a);
		Monomial_toString(tmp2, b);
		MDEBUG("[%s] vs [%s]", tmp1, tmp2);
	}
#endif	
	r = math_list_compare(a->elements, b->elements, (math_list_compare_func)AlgebraElement_compare_variable);
	return r;
#if 0
	for(pa = a->elements, pb = b->elements; ; pa = pa->next, pb = pb->next){
		if((pa == NULL) && (pb != NULL)){
			//MDEBUGP();
			return -1;
		}else if((pb == NULL) && (pa != NULL)){
			//MDEBUGP();
			return 1;
		}else if((pb == NULL) && (pa == NULL)){
			//MDEBUGP();
			return 0;
		}
		r = AlgebraElement_compare_variable(pa->data, pb->data);
		if(r != 0){
#if 0
			{
				char tmp1[128], tmp2[128];
				AlgebraElement_toString(tmp1, pa->data);
				AlgebraElement_toString(tmp2, pb->data);
				MDEBUG("In it[%s] vs [%s]", tmp1, tmp2);
			}
#endif
			MDEBUG("Return %d", r);
			return r;
		}
	}
#endif
}
int  Monomial_compare(Monomial *a, Monomial *b){
	if(a == NULL) {
		if(b != NULL)
			return -1;
		return 0;
	}
	if(b == NULL) return 1;
	int r;

	r = math_list_compare(a->elements, b->elements, (math_list_compare_func)AlgebraElement_compare);
	if(r != 0)return r;

	r = a->coeff - b->coeff;
	if(r != 0)return r;
	//math_list * pa, *pb;
	return r;

}

/*
 *
 *	begin at the position with its_type >= type;
 * */
int  Monomial_compare_type(Monomial *a, Monomial *b, ElementType type){
	if((a == NULL) || (b == NULL)) return -1;
	math_list * pa, *pb;
	int r;
#if 0
	{
		char tmp1[128], tmp2[128];
		Monomial_toString(tmp1, a);
		Monomial_toString(tmp2, b);
		MDEBUG("[%s] vs [%s]", tmp1, tmp2);
	}
#endif
	for(pa = a->elements; (pa != NULL) && ((ALGEBRAELEMENT_NODE(pa)->type) < type); pa = pa->next){
	}
	for(pb = b->elements; (pb != NULL) && ((ALGEBRAELEMENT_NODE(pb)->type) < type); pb = pb->next){
	}

#if 0
	{
		char tmp1[128], tmp2[128];
		AlgebraElement *ea, *eb;
		if(pa != NULL){
			ea = pa->data;
			AlgebraElement_toString(tmp1, ea);
		}else{
			tmp1[0] = 0;
		}
		if(pb != NULL){
			eb = pb->data;
			AlgebraElement_toString(tmp2, eb);
		}else{
			tmp2[0] = 0;
		}
		MDEBUG("compare : begin at [%s] vs [%s]", tmp1, tmp2);
	}
#endif

	//MDEBUG("compare : begin at [%p] vs [%p]", pa, pb);
	r = math_list_compare(pa, pb, (math_list_compare_func)AlgebraElement_compare_variable);
	return r;

}
Monomial * Number_mul_AlgebraElement(Number n, AlgebraElement * ele){
	if(NULL == ele)
		return NULL;
	
	Monomial * res = Monomial_new();
	res->coeff = n;
	math_list * node = node_from_AlgebraElement(ele);
	res->elements = node;
	return res;
	/*
	MDEBUG("Multiply : number : %d", this->value);
	Monomial nm;
	nm.mulBy(*this);
	nm.mul(ele);
	MDEBUG("Result: %s", nm.toString().c_str());
	return nm;
	*/
}

Monomial * Number_mul_Monomial(Number a, Monomial * m){
	Monomial * nm = Monomial_copy(m);
	nm->coeff = a * nm->coeff;

	return nm;
}
Monomial * Monomial_mul_AlgebraElement(Monomial *mo, AlgebraElement * ele){
	Monomial * res = Monomial_copy(mo);
	//Monomial * res = mo;
	
	math_list * node = node_from_AlgebraElement(ele);
	//math_list * node = node_from_AlgebraElement(ele);
	{
		char tmp[16];
		MDEBUG("ELE:[%s]", AlgebraElement_toString(tmp, ele));
	}
	res->elements = math_list_append(res->elements, node);
	Monomial_sort(res);
	return res;
}

Monomial * AlgebraElement_mul_AlgebraElement(AlgebraElement* a, AlgebraElement* b){
	Monomial * res = Monomial_new();
	math_list * node_a = node_from_AlgebraElement(a);
	math_list * node_b = node_from_AlgebraElement(b);
	res->elements = math_list_append(res->elements, node_a);
	res->elements = math_list_append(res->elements, node_b);
	Monomial_sort(res);
	return res;
}

Monomial*  AlgebraElement_mul_Monomial(AlgebraElement* ele, Monomial * mo){
	Monomial * res = Monomial_copy(mo);
	//Monomial * res = mo;
	//AlgebraElement * ele = AlgebraElement_copy(e);
	math_list * node = node_from_AlgebraElement(ele);
	res->elements = math_list_insert_before(res->elements, node);
        Monomial_sort(res);
	return res;
}

Monomial *  Monomial_mul_Monomial( Monomial *a, Monomial *b){
	if(a == NULL || b == NULL) return NULL;

	Monomial * na;
	Monomial * nb;
	if( a->coeff * b->coeff == 0){
		na = Monomial_new();
		na->coeff = 0;
		na->elements = NULL;
		return na;
	}

	//TEXDEBUG("\n\n---%s : %d ,- \n\n", __FILE__, __LINE__ );
	na = Monomial_copy(a);
	nb = Monomial_copy(b);
	//TEXDEBUG("\n\n---%s : %d ,- \n\n", __FILE__, __LINE__ );
#if 0
	{
		char t1[128], t2[128];
		Monomial_toString(t1, na);
		Monomial_toString(t2, nb);
		//MDEBUG("Mul try : [%s]*[%s] ", t1, t2);
		TEXDEBUG("na->elements: %s, nb->elements: %s\n\n", t1, t2);
	}
#endif	
	na->coeff = na->coeff * nb->coeff;
	na->elements = math_list_cat(na->elements, nb->elements);
	//Monomial_sortType(na);
        //Monomial_sortVariable(na);
	//free(nb);
	//TEXDEBUG("\n\n---%s : %d ,- \n\n", __FILE__, __LINE__ );
	Monomial_sort(na);
	//{
	//	char t1[128], t2[128];
	//	//MDEBUG("Mul try : [%s]*[%s] ", t1, t2);
	//	TEXDEBUG("Result: %s \n\n", t1, t2);
	//}
	//TEXDEBUG("\n\n---%s : %d ,- \n\n", __FILE__, __LINE__ );
	//HeckeCliff_sort(na);
	/*
	{
		char t1[128], t2[128], t3[128];
		Monomial_toString(t1, a);
		Monomial_toString(t2, b);
		Monomial_toString(t3, na);
		MDEBUG("In : [%s]*[%s] = [%s]", t1, t2, t3);
	
	}
	*/
	return na;
}

Monomial * Monomial_mul_list(int count,...){
        //将要用来存放需要的某个可变参数的指针的信息
        va_list ap;
	Monomial * mo, *marg, *res;
        int i;

        //开始的初始化，其中ap含有指向可变参数的指针的信息，count是当前函数中最后一个非可变的参数(这样才能定位).
        va_start(ap, count);
	


	res = Monomial_new();

        for(i = 0; i < count; i++){
                marg = va_arg(ap, Monomial *);
		if(marg == NULL){
			continue;		
		}
		mo = Monomial_copy(marg);
		res->coeff = res->coeff * mo->coeff;
		res->elements = math_list_cat(res->elements, mo->elements);
		Monomial_sort(res);
        }

        //使用完可变参数之后要用这个来释放资源
        va_end(ap);

	return res;
}
Polynomial * Polynomial_new(){
	Polynomial * res = malloc_zero(sizeof(Polynomial));
	return res;
}
Polynomial * Monomial_copy_to_Polynomial(Monomial * mo){
	Polynomial * p = Polynomial_new();
	//char tmp[256];
	//MDEBUG("Monomial [%s]", Monomial_toString(tmp, mo));
	if(NULL != mo){
		//Monomial * mo2 = Monomial_copy(mo);
		//MDEBUG("Monomial-> [%s]", Monomial_toString(tmp, mo2));
		p->monomials = node_from_Monomial(mo);
	}
	//MDEBUG("Polynomial [%s]", Polynomial_toString(tmp, p));
	return p;
}

char * Polynomial_toString(char *res, Polynomial * poly){
	math_list * p;
	Monomial * mo;
	MDEBUGP();
	if(NULL == res )
		return NULL;
	MDEBUGP();
	if( (NULL  == poly)||(poly->monomials == NULL) || (math_list_length(poly->monomials) == 0)){
		res[0] = '0';
		res[1] = 0;
		return res;
	}

	MDEBUGP();
	res[0] = 0;
	//MDEBUG("to string: [%p].%p", poly, poly->monomials);
	
	int ilen = 0;
	for(p = poly->monomials; p != NULL; p = p ->next){
		char tmp[128];
		mo = p->data;
		if(ilen >= 120){
			strcat(res, "\\\\\n\t&\\qquad \\qquad");
			ilen = 0;
		}
		Monomial_toString(tmp, mo);
		//MDEBUG("%p->%p: [%s]", poly, p, tmp);
		if(p != poly->monomials){
			if(mo->coeff >= 0 ){
				strcat(res, " + ");
			}else if(mo->coeff < 0 ){
				strcat(res, " ");
			}
		}
		strcat(res, tmp);
		ilen += strlen(tmp);
		//MDEBUG("%s, %d, %d ", tmp, strlen(tmp), ilen);
	}
	MDEBUG("%p: %s", poly, res);
	return res;
}
void Polynomial_Output(Polynomial * poly, FILE * fp){
	math_list * p;
	Monomial * mo;
	MDEBUGP();
	if( (NULL  == poly)||(poly->monomials == NULL) || (math_list_length(poly->monomials) == 0)){
		fprintf(fp, "0");
		//fflush(fp);
		return ;
	}

	MDEBUGP();
	//MDEBUG("to string: [%p].%p", poly, poly->monomials);
	
	int ilen = 0;
	for(p = poly->monomials; p != NULL; p = p ->next){
		char tmp[1024];
		mo = p->data;
		if(ilen >= 120){
			fprintf(fp, "\\\\\n\t&\\qquad \\qquad");
			ilen = 0;
		}
		Monomial_toString(tmp, mo);
		//MDEBUG("%p->%p: [%s]", poly, p, tmp);
		if(p != poly->monomials){
			if(mo->coeff >= 0 ){
				fprintf(fp, " + ");
			}else if(mo->coeff < 0 ){
				fprintf(fp, " ");
			}
		}
		fprintf(fp, tmp);
		ilen += strlen(tmp);
		//MDEBUG("%s, %d, %d ", tmp, strlen(tmp), ilen);
	}
	//MDEBUG("%p: %s", poly, res);
	//fflush(fp);
	return ;
}
Polynomial * Polynomial_mul_Polynomial(Polynomial * p1, Polynomial * p2){
	Polynomial *res;
	Monomial *m1, *m2, *m;
	math_list *node1, *node2;

	res = Polynomial_new();
	if(NULL == p1 || NULL == p1->monomials){
		MDEBUGP();
		return res;
	}
	if(NULL == p2 || NULL == p2->monomials){
		MDEBUGP();
		return res;
	}
	//int i = 0;
	//int t1, t2;
	//t1 = math_list_length(p1->monomials);
	//t2 = math_list_length(p2->monomials);
	//TEXDEBUG("\n\n---%s : %d , %d x %d,-\n\n", __FILE__, __LINE__, t1, t2);

	//last = NULL;	
	//MDEBUG("%p->%p", res, res->monomials);
	for(node1 = p1->monomials; node1 != NULL; node1 = node1->next){
		//i++;
		m1 = node1->data;
		for(node2 = p2->monomials; node2!= NULL; node2 = node2->next){
			m2 = node2->data;
#if 0
			{
				char s1[1024], s2[1024];
				Monomial_toString(s1, m1);
				Monomial_toString(s2, m2);
				MDEBUG("Monomial [%s]*[%s] ", s1, s2);
				TEXDEBUG("\n\n---%s : %d ,- %d, [$%s * %s $], %d x %d \n\n", __FILE__, __LINE__, i, s1, s2, t1, t2);
				//fflush(stdout);
			}
#endif			
			//TEXDEBUG("\n\n---%s : %d ,- %d, \n\n", __FILE__, __LINE__, i );
			////fflush(stdout);
			m = Monomial_mul_Monomial(m1, m2);
			/*
			{
				char t1[128], t2[128], t3[128];
				Monomial_toString(t1, m1);
				Monomial_toString(t2, m2);
				MDEBUG("Monomial [%s]*[%s] ", t1, t2);
			}
			*/
			//node = node_from_Monomial(m);
			//Monomial_delete(m);
			//free(m);
			//TEXDEBUG("\n\n---%s : %d ,- %d, \n\n", __FILE__, __LINE__, i );
			res->monomials = math_list_append_from_data(res->monomials, m);
			//TEXDEBUG("\n\n---%s : %d ,- %d, \n\n", __FILE__, __LINE__, i );
			//res->monomials = math_list_begin(node);
			/*
			{
				char t1[128], t2[128], t3[128];
				Monomial_toString(t1, m1);
				Monomial_toString(t2, m2);
				Monomial_toString(t3, m);
				MDEBUG("Monomial [%s]*[%s] = [%s]", t1, t2, t3);
			}
			*/
			//MDEBUG("%p->%p ... ->%p", res, res->monomials, math_list_end(res->monomials));
			//free(m);
		}
	}

	return res;
}

int Polynomial_rearrange_round(Polynomial * poly){
	math_list * node;
	int updated = 0;
	//char tmp[STR_BUFFER];

	if(NULL == poly || NULL == poly->monomials)
		return 0;
	//MDEBUG("Polynomial = [%s]", Polynomial_toString(tmp, poly));
	//updated = math_list_sort_increase(poly->monomials, NULL, (math_list_compare_func)Monomial_similar);
	//poly->monomials = math_list_begin(poly->monomials);
	//MDEBUG("Polynomial = [%s]", Polynomial_toString(tmp, poly));
	if(updated ){
		MDEBUG("& \\mbox{sort in list %d } \\\\\n", updated);
		return updated;
	}

	for(node = poly->monomials; node != NULL; node = node->next){
		Monomial * mo;
		mo = node->data;
		updated += Monomial_sort(mo);
		//updated += Monomial_sortType(mo);
		//updated += Monomial_sortVariable(mo);
	}
	return updated;
}

int PolyPolynomial_rearrange_round(PolyPolynomial * poly){
	math_list * node;
	int updated = 0;
	//char tmp[STR_BUFFER];

	if(NULL == poly || NULL == poly->polymonomials)
		return 0;
	//MDEBUG("Polynomial = [%s]", Polynomial_toString(tmp, poly));
	//updated = math_list_sort_increase(poly->monomials, NULL, (math_list_compare_func)Monomial_similar);
	//poly->monomials = math_list_begin(poly->monomials);
	//MDEBUG("Polynomial = [%s]", Polynomial_toString(tmp, poly));
	if(updated ){
		MDEBUG("& \\mbox{sort in list %d } \\\\\n", updated);
		return updated;
	}

	for(node = poly->polymonomials; node != NULL; node = node->next){
		PolyMonomial * pm;
		pm = node->data;
		updated += PolyMonomial_sort(pm);
		//updated += Monomial_sortType(mo);
		//updated += Monomial_sortVariable(mo);
	}
	return updated;
}

//All elements_type < type is in coeffcients.
math_list * Monomial_from_type(Monomial *mo, ElementType type){
	if(NULL == mo || NULL == mo->elements)return NULL;
	math_list * node;
	AlgebraElement * a;
	//(res[0]).coeff = 1;
	//(res[1]).coeff = 1;
	Monomial * res1, * res2;
	res1 = Monomial_new();
	res2 = Monomial_new();
	
	for(node = mo->elements; node != NULL ; node = node->next){
		a = node->data;
		if(a->type < type ){
			math_list * node = node_from_AlgebraElement(a);
			res1->elements = math_list_append(res1->elements, node);
		}else{
			math_list * node = node_from_AlgebraElement(a);
			res2->elements = math_list_append(res2->elements, node);
		}
	}
	res1->coeff = mo->coeff;
	res2->coeff = 1;
	math_list * res = NULL;
	res = math_list_append_from_data(res, res1);
	res = math_list_append_from_data(res, res2);
	//res = math_list_node_new();
	//res->data = res1;
	//res->next = math_list_node_new();
	//res->next->data = res2;
	//res->next->pre = res;
	//TEXDEBUG("\n\n------%d, Mo: coeff--%d-\n\n", __LINE__, mo->coeff);
	//TEXDEBUG("\n\n------%d--return %p, Get [%p], [%p]-\n\n", __LINE__, res, res1, res2);
	//TEXDEBUG("\n\n------%d--return %p, check [%p], [%p]-\n\n", __LINE__, res, res->data, res->next->data);

	return res;
}

int Polynomial_cal_sum(Polynomial * poly){
	int updated;
	if(NULL == poly || NULL == poly->monomials)
		return 0;

	math_list * p, *q;
	updated = 0;
	math_list * tmp_head = math_list_node_new();
	poly->monomials = math_list_insert_before(poly->monomials, tmp_head);

	for(p = poly->monomials->next; p!= NULL; ){
		Monomial * m1 = p->data;
		for(q = p->next;  q!= NULL; ){
			Monomial * m2;
			math_list * node = q->next;
			m2 = q->data;
			if(Monomial_similar(m1, m2) == 0){
				m1->coeff += m2->coeff;
				Monomial_delete(m2);
				math_list_delete_node(q);
			}
			q = node;
		}
		p = p->next;
	}
	for(p = poly->monomials->next; p!= NULL; ){
		Monomial * m1 = p->data;
		math_list * node = p->next;
		if(m1->coeff == 0){
			Monomial_delete(m1);
			math_list_delete_node(p);
		}
		p = node;
	}
	if(math_list_length(poly->monomials) > 1){
		poly->monomials = poly->monomials->next;
		poly->monomials->pre = NULL;
		free(tmp_head);
	}else{
		Monomial * m = Monomial_new();
		m->coeff = 0;
		tmp_head->data = m;
	}
	return updated;
}
Polynomial * Polynomial_sum_Polynomial(Polynomial* a, Polynomial *b){
	Polynomial * na, *nb, *res;
	res = Polynomial_new();

	math_list *node, *p; 
	if(a!= NULL ){
		for(p = a->monomials; p!=NULL; p = p->next){
			MDEBUGP();
			node = math_list_node_new();
			node->data = Monomial_copy(p->data);
			res->monomials = math_list_append(res->monomials, node);
		}
	}
	if(b!= NULL ){
		for(p = b->monomials; p!=NULL; p = p->next){
			MDEBUGP();
			node = math_list_node_new();
			node->data = Monomial_copy(p->data);
			res->monomials = math_list_append(res->monomials, node);
		}
	}

	//Polynomial_cal_sum(res);
	return res;

}
int PolyPolynomial_cal_sum(PolyPolynomial * poly){
	int updated;
	if(NULL == poly || NULL == poly->polymonomials)
		return 0;

	math_list * p, *q;
	updated = 0;

	//TEXDEBUG("\n\n---%s : %d---%d elements: \n\n", __FILE__, __LINE__, math_list_length(poly->polymonomials));
        Monomial* mo1 = Monomial_from_string("-1");
        Polynomial * p_m1 = Monomial_copy_to_Polynomial(mo1);

	for(p = poly->polymonomials; p!= NULL; p = p->next){
		PolyMonomial * pm1 = p->data;
		Monomial * mo = pm1->monomial;
		if(mo->coeff == -1){
			mo->coeff = 1;
			math_list * n;
			Polynomial * px = pm1->coeff;

			for(n = px->monomials; n != NULL; n = n->next){
				Monomial * mx = n->data;
				mx->coeff = mx->coeff * (-1);
			}
		}

	}
	for(p = poly->polymonomials; p!= NULL; ){
		PolyMonomial * pm1 = p->data;
#if 0
				{
					char ss1[16*1024], ss2[16*1024];
					Monomial_toString(ss1, pm1->monomial);
					TEXDEBUG("---- Monomial coeff [ %s ] \n", ss1);
				}
#endif
		for(q = p->next;  q!= NULL; ){
			PolyMonomial * pm2;
			math_list * node = q->next;
			pm2 = q->data;
			int comp = Monomial_compare(pm1->monomial, pm2->monomial);
#if 0
				{
					char ss1[16*1024], ss2[16*1024];
					char s1[16*1024], s2[16*1024];
					Monomial_toString(ss1, pm1->monomial);
					Monomial_toString(ss2, pm2->monomial);
					Monomial * m1, *m2;
					m1 = Monomial_copy(pm1->monomial);
					m2 = Monomial_copy(pm2->monomial);
					m1->coeff = 1;
					m2->coeff = 1;
					Monomial_toString(s1, m1);
					Monomial_toString(s2, m2);
					if((strcmp(s1, s2) == 0) && (comp != 0)){
						TEXDEBUG("------compare: Monomial [ %s , %s ], %d \n", ss1, ss2, comp);
					}
				}
#endif
			//if(Monomial_compare(pm1->monomial, pm2->monomial) == 0){
			if(comp == 0){
#if 0
				{
					char s1[16*1024], s2[16*1024];
					PolyMonomial_toString(s1, pm1);
					PolyMonomial_toString(s2, pm2);
					TEXDEBUG("------------[ %s + %s = ", s1, s2);
				}
#endif
				pm1->coeff = Polynomial_sum_Polynomial(pm1->coeff, pm2->coeff);
#if 0
				{
					char s1[16*1024];
					PolyMonomial_toString(s1, pm1);
					TEXDEBUG("%s", s1);
				}
#endif
				math_list_delete_node(q);
			}
			q = node;
		}
		p = p->next;
	}
	//TEXDEBUG("\n\n---%s : %d---\n\n", __FILE__, __LINE__);
	for(p = poly->polymonomials; p!= NULL; ){
		PolyMonomial * m1 = p->data;
		math_list * node = p->next;
		if(m1->coeff == NULL ){
			//Monomial_delete(m1);
			if(p ==  poly->polymonomials){
				poly->polymonomials = poly->polymonomials->next;
			}
			math_list_delete_node(p);
		}
		p = node;
	}
	//TEXDEBUG("\n\n---%s : %d---\n\n", __FILE__, __LINE__);
	return updated;
}
PolyMonomial * PolyMonomial_new(){
	PolyMonomial * res = malloc_zero(sizeof(PolyMonomial));
	return res;
}
//All elements_type < type is in coeffcients.
PolyPolynomial * PolyPolynomial_from_Polynomial(Polynomial * poly, ElementType type){
	PolyPolynomial * res;
	math_list *p, *q, *tag;
	//TEXDEBUG("\n\n------%d---\n\n", __LINE__);

	if(poly == NULL || poly->monomials == NULL){
		return NULL;
	}	
	Polynomial_cal_sum(poly);
	//char tmp1[STR_BUFFER], tmp2[STR_BUFFER];
	//MDEBUG("type:%d, Polynomial: [%s]", type, Polynomial_toString(tmp1, poly));
	
	res = malloc_zero(sizeof(PolyPolynomial));
	//TEXDEBUG("\n\n------%d---\n\n", __LINE__);
	for(p = poly->monomials; p != NULL; p = p->next){
		
		//TEXDEBUG("\n\n------%d---\n\n", __LINE__);
		Monomial * mo = p->data;
		math_list  * larr = Monomial_from_type(mo, type);
		if(larr == NULL) continue;
		Monomial * arr0 = math_list_data_at(larr, 0);
		Monomial * arr1 = math_list_data_at(larr, 1);
		//TEXDEBUG("\n\n------%d-list %p, [%p][%p]--\n\n", __LINE__, larr, larr->data, arr1);

		//MDEBUG("Got monomial: [%s]", Monomial_toString(tmp1, mo));
		//MDEBUG("Got monomial: coeff:[%s], mono:[%s]", Monomial_toString(tmp1, arr), Monomial_toString(tmp2, arr+1) );
		tag = NULL;
		for(q = res->polymonomials; q != NULL; q = q->next){
			PolyMonomial * pm = q->data;
			//pm->coeff;
			//pm->monomial not pointer;
			//if(Monomial_similar(pm->monomial, arr1) == 0){
			if(Monomial_compare(pm->monomial, arr1) == 0){
				//found in list.
				tag = q;
				break;
			}
		}
		//TEXDEBUG("\n\n------%d---\n\n", __LINE__);
		if(tag == NULL){
			//Not found.
			//MDEBUG("Did not Find it");
			PolyMonomial * pm = PolyMonomial_new();
			//TEXDEBUG("\n\n------%d---arr1: %p\n\n", __LINE__, arr1);
			//TEXDEBUG("\n\n------%d---arr1: %d\n\n", __LINE__, arr1->coeff);
			pm->monomial = Monomial_copy(arr1);
			//TEXDEBUG("\n\n------%d---\n\n", __LINE__);
			pm->coeff = malloc_zero(sizeof(Polynomial));
			pm->coeff->monomials = math_list_append(pm->coeff->monomials, node_from_Monomial(arr0));
			math_list * node = math_list_node_new();
			node->data = pm;
			res->polymonomials = math_list_append(res->polymonomials, node);
		}else{
			//MDEBUG("Found it");
			PolyMonomial * pm = tag->data;
			pm->coeff->monomials = math_list_append(pm->coeff->monomials, node_from_Monomial(arr0));
		}
		//TEXDEBUG("\n\n------%d---\n\n", __LINE__);
		Monomial_delete(arr0);
		Monomial_delete(arr1);
		math_list_delete_node(larr->next);
		math_list_delete_node(larr);
	}
	//TEXDEBUG("\n\n------%d---\n\n", __LINE__);
	for(q = res->polymonomials; q != NULL; ){
		math_list * node = q->next;
		PolyMonomial * pm = q->data;
		math_list_sort_increase(pm->coeff->monomials, NULL, (math_list_compare_func)Monomial_similar);
		pm->coeff->monomials = math_list_begin(pm->coeff->monomials);
		Polynomial_cal_sum(pm->coeff);
		
		if(math_list_length(pm->coeff->monomials) == 1){
			
			Monomial * mx = pm->coeff->monomials->data;
			if(mx->coeff == 0){
				math_list_remove(q);
			}
		}
		q = node;
	}
	//TEXDEBUG("\n\n------%d---\n\n", __LINE__);
	//MDEBUGP();
	return res;
}

char * PolyMonomial_toString(char * str, PolyMonomial * pm){
	char t1[STR_BUFFER], t2[STR_BUFFER+16];
	//char res[STR_BUFFER+16];

	if(pm == NULL ){
		str[0] = '0';
		str[1] = 0;
		return str;
	}
	Polynomial_toString(t1, pm->coeff);
	Monomial_toString(t2, pm->monomial);

	if(strcmp(t1, "1") == 0){
		strcpy(str, t2);
		return str;
	}else{
		if((math_list_length(pm->coeff->monomials) == 1) && ((MONOMIAL_NODE(pm->coeff->monomials))->coeff  > 0)){
			sprintf(str, "%s \\cdot %s", t1, t2);
		}else{
			sprintf(str, "(%s) \\cdot %s", t1, t2);
		}
	}
	
	MDEBUG("PP Coeff: %s", t1);
	MDEBUG("PP Monomial: %s", t2);
	return str;
}
void PolyMonomial_Output(PolyMonomial * pm, FILE * fp){
	//char t1[STR_BUFFER], t2[STR_BUFFER+16];
	//char res[STR_BUFFER+16];

	if(pm == NULL ){
		fprintf(fp, "0");
		//fflush(fp);
		return ;
	}
	//Polynomial_toString(t1, pm->coeff);
	//Monomial_toString(t2, pm->monomial);

	//if(strcmp(t1, "1") == 0){
	//	//strcpy(str, t2);
	//	Monomial_Output(pm->monomial, fp);
	//	return str;
	//}else{
		if((math_list_length(pm->coeff->monomials) == 1) && ((MONOMIAL_NODE(pm->coeff->monomials))->coeff  > 0)){
			Polynomial_Output(pm->coeff, fp);
			fprintf(fp," \\cdot ");
			Monomial_Output(pm->monomial, fp);
		}else{
			fprintf(fp,"(");
			Polynomial_Output(pm->coeff, fp);
			fprintf(fp,") \\cdot ");
			Monomial_Output(pm->monomial, fp);
		}
	//}
	//fflush(fp);
	return ;
}
char * PolyPolynomial_toString(char * str, PolyPolynomial * pp){
	math_list * p;
	char tmp[STR_BUFFER];
	if(pp == NULL || pp->polymonomials == NULL){
		str[0] = '0';
		str[1] = 0;
		return str;
	}

	int i = 0;
	tmp[0] = 0;
	str[0] = 0;
	for(p = pp->polymonomials; p != NULL; p = p->next){
		PolyMonomial *pm = p->data;
		PolyMonomial_toString(tmp, pm);
		if(i == 0){
			//strcat(str, " &=");
		}else{
			//if(tmp[0] == '-'){
			//	strcat(str, " &\\qquad ");
			//}else{
				strcat(str, " &\\qquad +");
			//}
		}
		strcat(str, tmp);
		strcat(str, "\\\\\n");
		i++;
	}
	return str;
}

void PolyPolynomial_Output(PolyPolynomial * pp, FILE * fp){
	math_list * p;
	if(pp == NULL || pp->polymonomials == NULL){
		fprintf(fp, "0");
		//fflush(fp);
		return ;
	}

	int i = 0;
	for(p = pp->polymonomials; p != NULL; p = p->next){
		PolyMonomial *pm = p->data;
		if(i == 0){
			//strcat(str, " &=");
		}else{
			//if(tmp[0] == '-'){
			//	strcat(str, " &\\qquad ");
			//}else{
				fprintf(fp, " &\\qquad +");
			//}
		}
		//strcat(str, tmp);
		//strcat(str, "\\\\\n");
		PolyMonomial_Output(pm, fp);
		fprintf(fp, "\\\\\n");
		i++;
	}
	//fflush(fp);
	return ;
}

Polynomial * Polynomial_copy(Polynomial * poly){
	if(NULL == poly)return NULL;
	
	MDEBUGP();
	Polynomial * res = Polynomial_new();

	math_list *node, *p; 
	for(p = poly->monomials; p!=NULL; p = p->next){
		MDEBUGP();
		node = math_list_node_new();
		node->data = Monomial_copy(p->data);
		res->monomials = math_list_append(res->monomials, node);
	}
	MDEBUGP();
	return res;
}
int Polynomial_compare(Polynomial * pa, Polynomial * pb){
	int r;
	if(pa == NULL ){
		if(pb == NULL){
			return 0;
		}
		return -1;
	}
	if(pb == NULL){
		return 1;
	}

	r = math_list_compare(pa->monomials, pb->monomials, (math_list_compare_func)Monomial_compare);
	return r;
}

AlgebraElement * Monomial_find_AlgebraElement_by_type(Monomial * mo, ElementType type){
	math_list * q;
	for(q = mo->elements; q != NULL; q = q->next){
		AlgebraElement * el = q->data;
		if(el->type == type){
			return el;
		}
	}
	return NULL;
}
AlgebraElement* AlgebraElement_copy(AlgebraElement* a){
	AlgebraElement* b =  AlgebraElement_new();
	memcpy(b, a, sizeof(AlgebraElement));

	return b;
}
Monomial * Monomial_get_coeff_by_type(Monomial * mo, ElementType type){
	math_list * q, *found;
	found = NULL;
	for(q = mo->elements; q != NULL; q = q->next){
		AlgebraElement * el = q->data;
		if(el->type == type){
			found = q;
			break;
		}
	}

	//if((found != NULL) && (found !=  mo->elements)){
	if( (found != NULL) && (mo->coeff != 0)){
		Monomial * nm = Monomial_new();
		nm->coeff = mo->coeff;
		for(q = mo->elements; q != found; q = q->next){
			AlgebraElement * nele = AlgebraElement_copy(q->data);
			nm->elements = math_list_append_from_data(nm->elements , nele);
		}
		return nm;

	}
	return NULL;
}
Monomial * Monomial_get_coeff_by_Ele(Monomial * mo, AlgebraElement * ele){
	math_list * q, *found;
	found = NULL;
	for(q = mo->elements; q != NULL; q = q->next){
		AlgebraElement * el = q->data;
		if(AlgebraElement_compare_variable(el, ele) == 0){
			found = q;
			break;
		}
	}

	//if((found != NULL) && (found !=  mo->elements)){
	if( (found != NULL) && (mo->coeff != 0)){
		Monomial * nm = Monomial_new();
		nm->coeff = mo->coeff;
		for(q = mo->elements; q != NULL; q = q->next){
			if(q == found) continue;
			AlgebraElement * nele = AlgebraElement_copy(q->data);
			nm->elements = math_list_append_from_data(nm->elements , nele);
		}
		return nm;

	}
	return NULL;
}

void Polynomial_delete(Polynomial * poly){
	//TEXDEBUG("\n\nPolynomial free: %p\n\n", poly);
	if(NULL == poly) return ;
	math_list * p, *q;
	for(p = poly->monomials; p != NULL; ){
		q = p->next;
		Monomial * mo = p->data;
		Monomial_delete(mo);
		math_list_delete_node(p);
		p = q;
	}
	poly->monomials = NULL;
	free(poly);
}
void PolyMonomial_delete(PolyMonomial * pm){
	if(NULL == pm) return ;
	Polynomial * p = pm->coeff;
	Monomial * m = pm->monomial;
	Polynomial_delete(p);
	Monomial_delete(m);
	free(pm);
}
void PolyPolynomial_delete(PolyPolynomial * poly){
	//TEXDEBUG("\n\nPolyPolynomial free: %p\n\n", poly);
	if(NULL == poly) return ;
	math_list * p, *q;
	for(p = poly->polymonomials; p != NULL; ){
		q = p->next;
		PolyMonomial * mo = p->data;
		PolyMonomial_delete(mo);
		math_list_delete_node(p);
		p = q;
	}
	poly->polymonomials = NULL;
	free(poly);
}
// Find ele in mo, replace with poly
Polynomial * Monomial_replace(Monomial * mo, AlgebraElement * ele, Polynomial * poly){
	if(NULL == poly || NULL == ele || NULL == mo)return NULL;
	//math_list * p;
	Polynomial * res=NULL;
	Monomial * co;

	co = Monomial_get_coeff_by_Ele(mo, ele);
#if 0
	{
		char s1[1024], s2[1024];
		Monomial_toString(s1, co);
		Polynomial_toString(s2, poly);
		TEXDEBUG("(Got coeff: [$x>%s$], will mul[$x>%s$])", s1, s2);
		//TEXDEBUG("(Got coeff: [$x-%s$]", s1);
	}
#endif
	if(co != NULL){
		Polynomial * pco = Polynomial_new();
		pco->monomials = math_list_node_from(co);
		//tmp = Monomial_copy_to_Polynomial(co);

		res = Polynomial_mul_Polynomial(poly, pco);
#if 0
		{
			char s1[1024];
			Polynomial_toString(s1, res);
			TEXDEBUG("mul result [$x>%s$]: %p", s1, res);
			//TEXDEBUG("(Got coeff: [$x-%s$]", s1);
		}
#endif
		return res;
	}
	return NULL;
}
void Polynomial_replace(Polynomial * poly, Equation * eq){
	if(NULL == poly || NULL == eq)return ;

	Polynomial * eq_left = eq->left;
	//Polynomial * eq_right = eq->right;
	Polynomial *tmp;
	{
		char t1[STR_BUFFER],  t2[STR_BUFFER];
		Polynomial_toString(t1, poly);
		Equation_toString(t2, eq);
		//TEXDEBUG("\n\n-------Poly:[$%s$] \n\n-------- eq:[$%s$]\n\n", t1, t2);
	
	}
	if(math_list_length(eq_left->monomials) > 1){
		return;
	}
	Monomial * mo = eq_left->monomials->data;
	AlgebraElement * ele = mo->elements->data;

	math_list * p, *q, *res;
	res = NULL;
	for(p = poly->monomials; p!= NULL; ){
		//TEXDEBUG("\n\n---%s : %d---\n\n", __FILE__, __LINE__);
		q = p->next;
		math_list_remove(p);

		//Monomial * tm = p->data;
       		tmp = Monomial_replace(p->data, ele, eq->right);
#if 0
		{
			char t1[STR_BUFFER], t2[STR_BUFFER], t0[STR_BUFFER], t4[STR_BUFFER];
			AlgebraElement_toString(t0, ele);
			Monomial_toString(t1, p->data);
			Polynomial_toString(t2, tmp);
			Equation_toString(t4, eq);
			TEXDEBUG("\n\nPoly replace: Monomial [$%s$], by str[$%s$](from equation [$%s$]), result:[$%s$], \n\n", t1, t0, t4, t2);
		
		}
#endif
		if(tmp == NULL ){
			//TEXDEBUG("Got NULL.\n\n");
			res = math_list_append(res, p);
		}else{
			//TEXDEBUG("Got list, replace it .");
			res = math_list_cat(res, tmp->monomials);
		}
		p = q;
		//TEXDEBUG("\n\n---%s : %d---\n\n", __FILE__, __LINE__);
	}
#if 0
		{
			char t1[STR_BUFFER];
			TEXDEBUG(FORLUMAR_LEFT"\n");
			Polynomial_toString(t1, poly);
			TEXDEBUG("z=&%s\\\\\n", t1);
			TEXDEBUG(FORLUMAR_RIGHT"\n");
		}
#endif
	poly->monomials = res;
#if 0
		{
			char t1[STR_BUFFER];
			TEXDEBUG(FORLUMAR_LEFT"\n");
			Polynomial_toString(t1, poly);
			TEXDEBUG("z=&%s\\\\\n", t1);
			TEXDEBUG(FORLUMAR_RIGHT"\n");
		}
#endif
	//TEXDEBUG("\n\n---%s : %d---\n\n", __FILE__, __LINE__);
}
void Polynomial_replace_list(Polynomial * poly, math_list * eqs){
	math_list * eq_l;
	Equation * eq;
	//TEXDEBUG("\n\n--%s : %d---\n\n", __FILE__, __LINE__);

	if(poly == NULL || eqs == NULL){
		return ;
	}
	//TEXDEBUG("\n\n--%s : %d---\n\n", __FILE__, __LINE__);
	//TEXDEBUG("eqs:%p, ", eqs);
	//TEXDEBUG("Poly:%p, ", poly);
	for(eq_l = eqs; eq_l!= NULL; eq_l = eq_l->next){
		eq = eq_l->data;
		//TEXDEBUG("\n\n---%s : %d---\n\n", __FILE__, __LINE__);
		Polynomial_replace(poly, eq);
		//TEXDEBUG("\n\n---%s : %d---\n\n", __FILE__, __LINE__);
	}
	
}

