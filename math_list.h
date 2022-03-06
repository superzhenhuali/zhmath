#ifndef __ZHMATH_LIST_H__
#define __ZHMATH_LIST_H__ 1
#include <stdint.h>

typedef struct math_list_t{
	void* data;
	//int size;
	struct math_list_t* pre;
	struct math_list_t* next;
}math_list;
typedef int (*math_list_compare_func)(void *a, void *b);
typedef int (*math_list_node_string_func)(char * res, void *list);
typedef void (*math_list_data_delete_func)(void *);

int math_list_length(math_list * list);
math_list * math_list_node_at(math_list * list, int index);
void * math_list_data_at(math_list * list, int index);

math_list * math_list_node_new();
math_list * math_list_node_from(void * ele);
math_list * math_list_append(math_list * list, math_list * node);
math_list * math_list_append_from_data(math_list * list, void * d);
void math_list_free(math_list *list);
math_list * math_list_begin(math_list * node);
math_list * math_list_end(math_list * node);
void math_list_exchange(math_list * a, math_list * b);
//math_list * math_list_copy(math_list * list);
math_list * math_list_insert_before(math_list * node, math_list * new_one);
math_list * math_list_insert_after(math_list * node, math_list * new_one);
math_list * math_list_remove(math_list * node);
void math_list_delete_node(math_list * node);
void math_list_delete_list(math_list * list, math_list_data_delete_func delete_func);
math_list * math_list_cat(math_list * a, math_list * b);

//[begin, end)
math_list * math_list_sublist(math_list * list, math_list * begin, math_list * end);

int math_list_sort_increase(math_list * list_begin, math_list * list_end,  math_list_compare_func compare);

math_list * math_list_find_node(math_list * list, math_list * node);
math_list *  math_list_find_data(math_list * list, void *data,  math_list_compare_func compare);
int math_list_find_data_index(math_list * list, void *data,  math_list_compare_func compare);
char * math_list_to_string(char * res, math_list * list, math_list_node_string_func to_string);
int  math_list_compare(math_list *a, math_list *b, math_list_compare_func compare);
math_list * math_list_find_node_pointer(math_list * list, math_list * node);

#endif
