//#include <stdint.h>
//#include <stdio.h>
#include <math.h>
#include <string.h>

#include "bigint.h"
#include "matrix.h"
#include "zhmath.h"
#include "quiver.h"
#include "algebra.h"
#include "quantum_qn.h"
//#include "heckecliff.h"
#include "testing.h"

int test_bigint(int argc, char * argv[]){
	bigint b1, b2;
	bigint br;

	if(argc < 3)return 0;

	bigint_from_str(&b1, argv[1]);
	bigint_from_str(&b2, argv[2]);
	bigint_add(&br, &b1, &b2);

	bigint_dump(&b1);
	bigint_dump(&b2);
	MDEBUG("Result");
	bigint_dump(&br);
	return 0;

}

void test_matrix(){
	Matrix *mat;
	char arr[3][3] = {{1,2,3}, {2,5,7}, {6,9,12} };

	mat = Matrix_new(MATRIX_ENTRY_CHAR);
	MDEBUG("matrix: %p, arr:%p ", mat, arr[0]);
	mat->resize(mat, 3, 3);
	mat->dump(mat);
	mat->setValue(mat, (void ** )arr);
	mat->dump(mat);
}

void test_quiver(){
	char quivarr[3][3] = {{1,0, 0}, {1,1,1}, {0,0,1} };
	quiver * quiv;
	dimv * v;

	quiv = Matrix_new(MATRIX_ENTRY_CHAR);
	MDEBUG("matrix: %p, arr:%p ", quiv, quivarr[0]);
	quiv->resize(quiv, 3, 3);
	quiv->dump(quiv);

	v = dimv_new(3);

	quiver_simple_module(quiv, 3, v);
	
	MDEBUG("Simple module");
	dimv_dump(v);
}
void test_algebra_1(){
	Number n1 = 4;
	AlgebraElement e1;
	AlgebraElement e2;
	AlgebraElement *t1;
	AlgebraElement *c1;
	Monomial * m1;
	Monomial * m2;
	Monomial * m3;
	Monomial * m4;
	Monomial * tm1;
	Monomial * tm2;
	Monomial * tm3;
	Monomial * tm4;
	Monomial * tm5;
	char tmp[256];

	t1 = HeckeElement_new(3);
	c1 = CliffElement_new(2);

	AlgebraElement_init(&e1);
	e1.type = VARIABLE_E;
	strcpy(e1.name,"q");
	e1.index = -1;

	MDEBUGP();
	MDEBUG("Result: e2 = [%s]", AlgebraElement_toString(tmp, &e2));
	AlgebraElement_init(&e2);
	e2.type = VARIABLE_E;
	strcpy(e2.name,"q");
	e2.index = 2;
	MDEBUG("Result: e2 = [%s]", AlgebraElement_toString(tmp, &e2));
	
	AlgebraElement_toString(tmp, &e1);
	MDEBUGP();
	MDEBUG("e1: [%s]", tmp);
	MDEBUGP();
	m1 = Number_mul_AlgebraElement(n1, &e1);
	MDEBUGP();
	tm1 = Monomial_mul_AlgebraElement(m1, t1);

	MDEBUG("Result: m1 = [%s]", Monomial_toString(tmp, m1));

	MDEBUG("Result: e2 = [%s]", AlgebraElement_toString(tmp, &e2));
	m2 = Monomial_mul_AlgebraElement(m1, &e2);
	tm2 = Monomial_mul_AlgebraElement(m2, c1);
	//Monomial_free(m1);
	MDEBUG("Result: m2 = [%s]", Monomial_toString(tmp, m2));
	m3 = Monomial_mul_AlgebraElement(m2, &e1);
	MDEBUG("Result: m3 = [%s]", Monomial_toString(tmp, m3));
	m4 = Monomial_mul_Monomial(m2, m3);
	MDEBUG("Result: m4 = [%s]", Monomial_toString(tmp, m4));
	MDEBUG("Result: tm1 = [%s]", Monomial_toString(tmp, tm1));
	MDEBUG("Result: tm2 = [%s]", Monomial_toString(tmp, tm2));

	tm3 = Monomial_mul_Monomial(tm1, tm2);
	MDEBUG("Result: tm3 = [%s]", Monomial_toString(tmp, tm3));
	tm4 = Monomial_mul_Monomial(tm3, tm1);
	MDEBUG("Result: tm4 = [%s]", Monomial_toString(tmp, tm4));
	tm5 = Monomial_mul_Monomial(tm4, m3);
	MDEBUG("Result: tm5 = [%s]", Monomial_toString(tmp, tm5));
	Monomial_sortType(tm5);
	MDEBUG("Result: tm5 = [%s]", Monomial_toString(tmp, tm5));
	Monomial_sortVariable(tm5);
	MDEBUG("Result: tm5 = [%s]", Monomial_toString(tmp, tm5));
}


void test_algebra_2(){
	char tmp[256];

	math_list * Tlist, *Clist;
	math_list * p;
	Monomial * mo;

       	Tlist = Hecke_GetGenerators(5);
       	Clist = Cliff_GetGenerators(5);
	MDEBUG("Tlist %p", Tlist);
	MDEBUG("Clist %p", Clist);

	for(p = Tlist; p != NULL; p = p->next){
		mo = p->data;
		MDEBUG("list: %p, p:%p, mo:%p", Tlist, p, mo);
		MDEBUG("Got generator %s", Monomial_toString(tmp, mo));
	}

	for(p = Clist; p != NULL; p = p->next){
		mo = p->data;
		MDEBUG("list: %p, p:%p, mo:%p", Clist, p, mo);
		MDEBUG("Got generator %s", Monomial_toString(tmp, mo));
	}

}
void test_algebra_3(){
	char tmp[256];

	math_list * Tlist;
	math_list * p;
	Monomial * mo;
	int i, n;

	n = 2;
       	Tlist = Hecke_GetAll(n);
	MDEBUG("Tlist %p", Tlist);

	i = 0;
	for(p = Tlist; p != NULL; p = p->next){
		mo = p->data;
		//MDEBUG("list: %p, p:%p, mo:%p", Tlist, p, mo);
		MDEBUG("Got element %s", Monomial_toString(tmp, mo));
		i++;
	}
	i ++;
	MDEBUG("Hecke(%d) cound: %d", n, i);
}
void test_algebra_4(){
	//char str[] = " 2 T_3 T_1 c_2 T_4 c_1 T_1";
	//char *str = " 2 q x_2 a_2 q T_3 T_2 T_1 c_2 q c_1  T_3 T_4 x_1 c_1 T_1";
	//char *str = "T_3 T_2 T_3 T_3 T_5 T_6";
	//char *str = "T_3 T_2 T_3 T_3 ";
	//char *str = "T_4 T_3 T_4 ";
	char * str = "1 T_3 T_2 T_1 T_3 T_2";
	Monomial *mo;
	int updated = 0 ;
	mo = Monomial_from_string(str);
	
	char tmp[256];

	MDEBUG("str:[%s]", str);
	MDEBUG("%d: Got element %s", updated, Monomial_toString(tmp, mo));
        updated = Monomial_sortType(mo);
	MDEBUG("%d: After sort Type, %s", updated, Monomial_toString(tmp, mo));
        updated = Monomial_sortVariable(mo); //TODO
	MDEBUG("%d: After sortVariable, %s", updated, Monomial_toString(tmp, mo));
	updated = HeckeCliff_sort(mo);
	MDEBUG("%d: After sort HeckeCliff, %s", updated, Monomial_toString(tmp, mo));
	updated = Hecke_IJI(mo);
	MDEBUG("%d: After sort IJI %s", updated, Monomial_toString(tmp, mo));
	updated = Hecke_reduce_II(mo);
	MDEBUG("%d: After Reduced: %s", updated, Monomial_toString(tmp, mo));
	updated = Hecke_IJI(mo);
	MDEBUG("%d: After sort IJI %s", updated, Monomial_toString(tmp, mo));
	updated = Hecke_reduce_II(mo);
	MDEBUG("%d: After Reduced: %s", updated, Monomial_toString(tmp, mo));
}
void test_algebra_5(){

	math_list * Tlist;
	math_list * p;
	int i, n;

	n = 4;
	//(n !) * (2^n)
       	//Tlist = Cliff_GetAll(n);
       	Tlist = HeckeCliff_GetAll(n, 3);
	MDEBUG("Tlist %p", Tlist);

	i = 0;
	for(p = Tlist; p != NULL; p = p->next){
		char tmp[256];
		Monomial * mo;
		mo = p->data;
		//MDEBUG("list: %p, p:%p, mo:%p", Tlist, p, mo);
		MDEBUG("Got element %s", Monomial_toString(tmp, mo));
		i++;
	}
	i ++;
	MDEBUG("Cliff (%d) cound: %d", n, i);
}
void test_algebra_6(){
	char tmp[256];

	math_list * Tlist;
	math_list * p;
	Monomial * mo;
	int i, n;

	n = 2;
	//(n !) * (2^n)
       	//Tlist = Cliff_GetAll(n);
       	Tlist = HeckeCliff_GetAll(n, 3);
	//math_list_sort_increase(Tlist, NULL, Monomial_compare);
	Tlist = math_list_begin(Tlist);
	MDEBUG("Tlist %p", Tlist);

	i = 0;
	for(p = Tlist; p != NULL; p = p->next){
		mo = p->data;
		//MDEBUG("list: %p, p:%p, mo:%p", Tlist, p, mo);
		MDEBUG("Got element %s", Monomial_toString(tmp, mo));
		i++;
	}
	
	Polynomial * pl = Polynomial_new();
	Monomial * m1, *m2;
	int xi;

	xi = 1;
	char s[16];
	for(p = Tlist; p != NULL; p = p->next){
		sprintf(s, "x_%d", xi);
		xi ++;
		m1 = Monomial_from_string(s);
		MDEBUG("Got element [%s] from string [%s]", Monomial_toString(tmp, mo), s);
		m2 = p->data;
		mo = Monomial_mul_Monomial(m1, m2);
		//MDEBUG("list: %p, p:%p, mo:%p", Tlist, p, mo);
		MDEBUG("Got element %s", Monomial_toString(tmp, mo));
		pl->monomials = math_list_append(pl->monomials, node_from_Monomial(mo));
	}

	Polynomial_toString(tmp, pl);
	MDEBUG("Result : pl = [%s]", tmp);
	MDEBUG("Cliff (%d) count: %d", n, i);

	for(p = Tlist; p != NULL; p = p->next){
		char t1[256], t2[256], t3[256];
		Polynomial * poly1, *poly3;

		m1 = p->data;
		poly1 = Monomial_copy_to_Polynomial(m1);
		poly3 = Polynomial_mul_Polynomial(poly1, pl);

		//MDEBUG("pointer [%p] * [%p] = [%p]", poly1, pl, poly3);
		//MDEBUG("pointer [%p] * [%p] = [%p]", poly1->monomials, pl->monomials, poly3->monomials);
		Polynomial_toString(t1, poly1);
		Polynomial_toString(t2, pl);
		Polynomial_toString(t3, poly3);
		MDEBUG("[%s] * [%s] = [%s]", t1, t2, t3);
	}

}
void test_algebra_7(){
	//char str[] = " 2 T_3 T_1 c_2 T_4 c_1 T_1";
	//char *str = " 2 q x_2 a_2 q T_3 T_2 T_1 c_2 q c_1  T_3 T_4 x_1 c_1 T_1";
	//char *str = "T_3 T_2 T_3 T_3 T_5 T_6";
	//char *str = "T_3 T_2 T_3 T_3 ";
	//char *str = "q T_4 x_1 q T_3 x_2 T_4 x_2 T_3 T_5 T_6";
	//char *str = "q x_1 q T_3 x_2 T_4 x_2 c_3 T_3 T_5 T_6";
	//char * str = "1 T_3 T_2 q T_1 T_3 x_3 T_2";
	//char *str = "x_1 T_4 T_4 ";
	//char *str = " T_4 T_4 T_5 ";
	//char *str = " T_4 T_4 ";
	//char *str = " x_1 T_4 T_4 T_5 ";
	//char *str = "x_1 c_4 T_4 ";
	char * str = "1 q T_1 q T_2 q T_3 x_3 T_4 c_1 x_3 q T_5 y_2 a_1 c_7 b";
	Monomial *mo;
	int updated = 0; 
	mo = Monomial_from_string(str);
	Polynomial * poly;
	char tmp[256];

	MDEBUG("str:[%s]", str);
	MDEBUG("%d: Got element %s", updated, Monomial_toString(tmp, mo));
        updated = Monomial_sortType(mo);
	MDEBUG("%d: After sort Type, %s", updated, Monomial_toString(tmp, mo));
        updated = Monomial_sortVariable(mo); //TODO
	MDEBUG("%d: After sortVariable, %s", updated, Monomial_toString(tmp, mo));
        updated = Monomial_sort(mo); //TODO
	MDEBUG("After sort,%d,  %s", updated, Monomial_toString(tmp, mo));
	poly = Polynomial_new();
	poly->monomials = HeckeCliff_calculate(mo);
	MDEBUG("%d: After HeckeCliff calculate, %s", updated, Polynomial_toString(tmp, poly));
}
void test_algebra_8(int n){
	//printf("Got n:%d\n", n);
	HeckeCliff_Cal_Z(n);
}
void test_algebra_9(){
	//char str[] = " 2 T_3 T_1 c_2 T_4 c_1 T_1";
	//char *str = " 2 q x_2 a_2 q T_3 T_2 T_1 c_2 q c_1  T_3 T_4 x_1 c_1 T_1";
	//char *str = "T_3 T_2 T_3 T_3 T_5 T_6";
	//char *str = "T_3 T_2 T_3 T_3 ";
	//char *str = "q T_4 x_1 q T_3 x_2 T_4 x_2 T_3 T_5 T_6";
	//char *str = "q x_1 q T_3 x_2 T_4 x_2 c_3 T_3 T_5 T_6";
	//char * str = "1 T_3 T_2 q T_1 T_3 x_3 T_2";
	//char *str = "x_1 T_4 T_4 ";
	//char *str = " T_4 T_4 T_5 ";
	//char *str = " T_4 T_4 ";
	//char *str = " x_1 T_4 T_4 T_5 ";
	char *str1 = "x_2 c_4 T_4 ";
	char *str2 = " 2 x_1 c_4 T_4 ";

	Monomial *mo1, *mo2;
	//int updated = 0; 
	mo1 = Monomial_from_string(str1);
	mo2 = Monomial_from_string(str2);
	char tmp[256];
	
	int r = Monomial_compare(mo1, mo2);
	MDEBUG("Monomial:[%s]", Monomial_toString(tmp, mo1));
	MDEBUG("Monomial:[%s]", Monomial_toString(tmp, mo2));
	MDEBUG("Compare  [%d]", r);
}
void test_algebra_10(){
	char *str1 = "- a_3 ";

	Monomial *mo1;
	//int updated = 0; 
	mo1 = Monomial_from_string(str1);
	char tmp[256];
	
	MDEBUG("Monomial:[%s], %d", Monomial_toString(tmp, mo1), mo1->coeff);
	TEXDEBUG("Monomial:[%s], %d", Monomial_toString(tmp, mo1), mo1->coeff);
}

void test_quantum_qn(int n){
	math_list * Tlist;
	math_list * p;
	Monomial * mo;
	char tmp[128];
	int i=0;

	Tlist = Quantum_Groups_Qn_getAll(n);
	i = 0;
	for(p = Tlist; p != NULL; p = p->next){
		mo = p->data;
		//MDEBUG("list: %p, p:%p, mo:%p", Tlist, p, mo);
		MDEBUG("Got element %s", Monomial_toString(tmp, mo));
		i++;
	}
	i ++;
}

int main(int argc, char * argv[]){
	int n=2;
	log_count = 0;
	if(argc >=2){
		n = atoi(argv[1]);
	}
	//test_matrix();
	//test_quiver();
	//test_bigint(argc, argv);
	//test_algebra_1();
	//test_algebra_2();
	//test_algebra_3();
	//test_algebra_4();
	//test_algebra_5();
	//test_algebra_6();
	//test_algebra_7();
	check_tex_log();
	//test_algebra_8(n);
	test_quantum_qn(2);
	if(tex_log == NULL){
		fclose(tex_log);
	}
	//test_algebra_9();
	//test_algebra_10();
}
