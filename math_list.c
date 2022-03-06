#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "math_list.h"
#include "utils.h"
#include "logs.h"
#include "polynomial.h"

void math_list_free(math_list *list){
	math_list * p, *tmp;
	if(NULL == list) return ;
	for(p = list; p != NULL; ){
		tmp= p->next;
		free(p);
		p = tmp;
	}
}

math_list * math_list_node_new(){
	math_list * node = malloc_zero(sizeof(math_list));
	return node;
}

math_list * math_list_begin(math_list * node){
	math_list * p;
	if(NULL == node) return NULL;
	for(p = node; p->pre != NULL; p = p->pre){
		//MDEBUG("node:%p, p:%p", node, p);
	}
	return p;
}
math_list * math_list_end(math_list * node){
	math_list * p;
	if(NULL == node) return NULL;
	//MDEBUG("node:%p", node);
	//MDEBUG("node->next:%p", node->next);
	for(p = node; p->next != NULL; p = p->next){
		//MDEBUG("node:%p", node);
		//MDEBUG("node->next:%p", node->next);
	}
	//MDEBUGP();
	return p;
}
int math_list_length(math_list * list){
	int i;
	math_list * p;
	if(NULL == list) return 0;
	for(i=0, p = list; p != NULL; p = p->next, i++);
	return i;
}

void * math_list_data_at(math_list * list, int index){
	math_list * node = math_list_node_at(list, index);
	if(node != NULL)return node->data;

	return NULL;
}
math_list * math_list_node_at(math_list * list, int index){
	int i;
	math_list * p;
	if(NULL == list) return NULL;
	if(index < 0) return NULL;

	for(i=0, p = list; (i!= index) && (p->next != NULL); p = p->next, i++);
	return p;
}

#if 1
/*
 * if size == 0, did not copy data;
 * if size > 0: copy data
 * */
math_list * math_list_node_from(void * data){
	math_list * node;

	node = math_list_node_new();
	if(NULL == node )return NULL;

	node->data = data;
	//TEXDEBUG("\n\n------%d- node from data:node: %p, data arg: %p, res data:%p--\n\n", __LINE__, node,data,  node->data);

	return node;
}
#else
/*
 * if size == 0, did not copy data;
 * if size > 0: copy data
 * */
math_list * math_list_node_from(void * ele, uint32_t size){
	math_list * node;

	node = math_list_node_new();
	if(NULL == node )return NULL;

	if(ele != NULL){
		if(size >0){
			//MDEBUG("Node copy with size");
			node->data = malloc(size);
			node->size = size;
			memcpy(node->data, ele, size);
		}else{
			MDEBUG("Node copy without size");
			node->data = ele;
			node->size = 0;
		}
	}

	return node;
}
#endif

math_list * math_list_append_from_data(math_list * list, void * d){
	math_list * node = math_list_node_from(d);
	//MDEBUGP();
	//TEXDEBUG("\n\n------%d- Append from data:node: %p, data arg: %p, check:%p--\n\n", __LINE__, node,d,  node->data);
	return math_list_append(list, node);
}


/*
 * Return: List begin.
 * */
math_list * math_list_append(math_list * list, math_list * ne){
	math_list * p;

	//MDEBUGP();
	if(NULL == ne )return list;

	//MDEBUGP();
	ne->pre = NULL;
	ne->next = NULL;
	if(NULL == list ){
		return ne;
	}
	//MDEBUGP();
	for(p = list; p->next != NULL; p = p->next){
		if(p == ne) return list;
	}
	//MDEBUGP();
	if(p == ne) return list;
	//math_list_insert_after(p, ne);
	p->next = ne;
	ne->pre = p;

	//MDEBUGP();
	return list;
}
math_list * math_list_insert_after(math_list * node, math_list * new_one){
	math_list * p;

	if(NULL== new_one){
		return node;
	}
	if(NULL == node){
		return new_one;
	}
	if(math_list_find_node(node, new_one)!= NULL) return node;
	p = node->next;

	new_one->next = p;
	new_one->pre = node;
	node->next = new_one;
	if(p != NULL){
		p->pre = new_one;
	}
	return new_one;
}
math_list * math_list_remove(math_list * node){
	math_list * p, *n;
	if(node == NULL) return NULL;
	p = node->pre;
	n = node->next;
	//MDEBUG("pre:%p, node:%p, next:%p", p, node, n);
	if(p != NULL){
		//MDEBUG("pre:%p, p->pre:%p", p, p->pre);
	}
	node->pre = NULL;
	node->next = NULL;
	if(NULL != p){
		p->next = n;
	}
	if(NULL != n){
		n->pre = p;
	}
	if(p != NULL) {
		//MDEBUG("pre:%p, p->pre:%p", p, p->pre);
		return math_list_begin(p);
	}
	if(n != NULL) return math_list_begin(n);
	
	//if(rec == NULL) return NULL;
	//MDEBUGP();
	//p = math_list_begin(rec);
	//MDEBUGP();
	return NULL;
}
void math_list_delete_node(math_list * node){
	math_list_remove(node);
	//free(node->data);
	free(node);
	return ;
}
void math_list_delete_list(math_list * list, math_list_data_delete_func delete_func){
	math_list * p, *node;
	for(p = list; p != NULL; ){
		node = p;
		p = p->next;
		delete_func(node->data);
		free(node);
	}
	return ;
}
math_list * math_list_insert_before(math_list * node, math_list * new_one){
	math_list * p;

	if(NULL == node){
		return new_one;
	}
	if(NULL== new_one){
		return node;
	}
	if(math_list_find_node(new_one, node)!= NULL) return node;
	
	p = node->pre;

	new_one->next = node;
	new_one->pre = p;
	node->pre = new_one;
	if(p != NULL){
		p->next = new_one;
	}
	return new_one;
}
void math_list_exchange( math_list * a, math_list * b){
	if((NULL == a )||(NULL == b)){
		return ;
	}

	math_list * anext, *apre;
	math_list * bnext, *bpre;

	anext = a->next;
	bnext = b->next;
	apre = a->pre;
	bpre = b->pre;
	if(anext == b){
		//MDEBUGP();
		math_list_remove(a);
		math_list_insert_after(b, a);
		return ;
	}
	if(bnext == a){
		//MDEBUGP();
		math_list_remove(b);
		math_list_insert_after(a, b);
		return ;
	}

	math_list_remove(a);
	math_list_remove(b);

	if( apre!= NULL){
		math_list_insert_after(apre, b);
	}else{
		math_list_insert_before(anext, b);
	}
	if( bpre!= NULL){
		math_list_insert_after(bpre, a);
	}else{
		math_list_insert_before(bnext, a);
	}
	
}

/* Does not copy data.*/
math_list * math_list_node_copy(math_list * node){
	math_list * n = math_list_node_new();
	if(NULL == n )return NULL;

	n->data = node->data;
	return n;

	//n->next = node->next;
	//n->pre = node->pre;
	//return math_list_node_from(node->data, node->size);
}
/*
math_list * math_list_copy(math_list * list){
	math_list * p, *np;
	math_list * nlist;

	if(NULL == list) return NULL;

	//nlist = malloc_zero(sizeof(math_list));
	//if(NULL == nlist )return NULL;
	
	nlist = math_list_node_copy(list);

	np = nlist;
	for(p = list->next; p != NULL; p = p->next){
		math_list *tmp;
		tmp = math_list_node_copy(p);
		math_list_insert_after(np, tmp);
		np = tmp;
	}
	return nlist;
}
*/
math_list * math_list_cat(math_list * a, math_list * b){
	//TEXDEBUG("Cat list, a%p, b%p\r\n\r\n", a, b);
	if(NULL == a){
		return b;
	}
	if(NULL == b)return a;
	
	math_list *end =  math_list_end(a);
	end->next = b;
	b->pre = end;
	return a;
}

/*
 *	Sort members [list_begin, list_end];
 *	return : updated nodes count;
 * */
//void math_list_sort_increase(math_list * list_begin, math_list * list_end,  int (*compare)(void *a, void *b)){
int math_list_sort_increase(math_list * list_begin, math_list * list_end,  math_list_compare_func compare){
	math_list *p1, *p2, *end;
	int ret, updated;
	if(NULL == list_begin) return 0;
	end = list_end;
	if(end != NULL){
		end = end->next;
	}
	updated = 0;
	for(p1 = list_begin; p1 != end;  p1 = p1->next){
		math_list *t1;
		
		for(p2 = p1->next; p2 !=end; ){
			ret = (*compare)(p1->data, p2->data);
			//MDEBUG("Compare result: %d", ret);
			//if((*compare)(p1->data, p2->data) >  0){
			if( ret >  0){
				t1 = p2->next;
				math_list_exchange(p1, p2);
				updated ++;
				p1 = p2;
				p2 = t1;
			}else{
				p2 = p2->next;
			}
		}

	}
	return updated;
}

/*
 *	Sort members [list_begin, list_end];
 *	return : updated nodes count;
 * */
//void math_list_sort_increase(math_list * list_begin, math_list * list_end,  int (*compare)(void *a, void *b)){
math_list * math_list_find_node(math_list * list, math_list * node){
	if(list == NULL || node == NULL){
		return NULL;
	}
	math_list * p;
	for(p = list; p!= NULL; p = p->next){
		if(p == node)return p;
	}
	return NULL;

}
math_list * math_list_find_node_pointer(math_list * list, math_list * node){
	if(list == NULL || node == NULL){
		return NULL;
	}
	math_list *p;
	for(p = list; p != NULL;  p = p->next){
		if(p == node){
			return p;
		}	
	}
	return NULL;
}
math_list *  math_list_find_data(math_list * list, void *data,  math_list_compare_func compare){
	math_list *p;
	if(list == NULL || data == NULL){
		return NULL;
	}

	for(p = list; p != NULL;  p = p->next){
		if(p->data == data) return p;
		if(compare(p->data, data) == 0){
			return p;
		}	
	}
	return NULL;
}
int  math_list_find_data_index(math_list * list, void *data,  math_list_compare_func compare){
	math_list *p;
	if(list == NULL || data == NULL){
		return -1;
	}
	int i = 0;
	for(p = list; p != NULL;  p = p->next, i++){
		if(compare(p->data, data) == 0){
			return i;
		}	
	}
	return -1;
}
char * math_list_to_string(char * res, math_list * list, math_list_node_string_func to_string){
	char tmp[256];
	math_list * p;
	if(NULL == res || NULL == list )return NULL;

	res[0] = 0;
	for(p = list; p != NULL; p = p->next){
		to_string(tmp, p->data);
		strcat(res, "  ");
		strcat(res, tmp);
	}
	return res;
}
//[begin, end)
math_list * math_list_sublist(math_list * list, math_list * begin, math_list * end){
	if(NULL == list || NULL == begin)return NULL;
	if(end == begin)return NULL;
	//MDEBUG("list %p, begin:%p, end:%p", list, begin, end);
	math_list * res = NULL;
	math_list * p, *q;

	if( math_list_find_node_pointer(list, begin) == NULL){
		//MDEBUG("List sublist: begin not found.");
		return NULL;
	}
	if( (end != NULL) && math_list_find_node_pointer(list, end) == NULL){
		//MDEBUG("List sublist: end not found.");
		return NULL;
	}
	//MDEBUG("Found begin.");
	res = math_list_node_copy(begin);
	//if(list == begin){
	//	return res;
	//}
	//MDEBUG("sublist :[%p, %p)", begin, end);
	q = res;
	for(p= begin->next; (p != end) && (p != NULL); p = p->next ){
		//MDEBUG("last %p, new to copy:%p", q, p);
		math_list *node = math_list_node_copy(p);
		//MDEBUG("last %p, new node:%p", q, node);
		//MDEBUGP();
		math_list_insert_after(q, node);
		q = node;
		//MDEBUGP();
	}
	//MDEBUGP();
	return res;
}
int  math_list_compare(math_list *a, math_list *b, math_list_compare_func compare){
	math_list * pa, *pb;
	int r;

#if 0	
	{
		char tmp1[128], tmp2[128];
		Monomial *ma, *mb;
		ma = Monomial_new();
		mb = Monomial_new();
		ma->elements = a;
		mb->elements = b;
		Monomial_toString(tmp1, ma);
		Monomial_toString(tmp2, mb);
		MDEBUG("[%s] vs [%s]", tmp1, tmp2);
	}
#endif

	for(pa = a, pb = b; ; pa = pa->next, pb = pb->next){
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
		r = compare(pa->data, pb->data);
#if 0
			{
				char tmp1[128], tmp2[128];
				AlgebraElement_toString(tmp1, pa->data);
				AlgebraElement_toString(tmp2, pb->data);
				MDEBUG("In it[%s] vs [%s]", tmp1, tmp2);
			}
#endif
		if(r != 0){
			//MDEBUG("Return %d", r);
			return r;
		}
	}
}
