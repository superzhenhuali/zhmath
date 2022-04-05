#include <string.h>
#include <stdlib.h>

#include "utils.h"
#include "equation.h"
#include "matrix.h"
#include "logs.h"

Equation * Equation_new(){
	Equation * res = malloc_zero(sizeof(Equation));
	return res;
}
PPEquation * PPEquation_new(){
	PPEquation * res = malloc_zero(sizeof(Equation));
	return res;
}

char * Equation_toString(char * res, Equation * eq){
	res[0] = 0;
	if(eq == NULL){
		return res;
	}else{
		char t1[STR_BUFFER], t2[STR_BUFFER];
		Polynomial_toString(t1, eq->left);
		Polynomial_toString(t2, eq->right);
		strcat(res, t1);
		strcat(res, " = ");
		strcat(res, t2);
		return res;
	}
}
void Equation_Output(Equation * eq, FILE * fp){
	if(eq == NULL){
		return ;
	}
	
	Polynomial_Output(eq->left, fp);
	fprintf(fp, " = ");
	Polynomial_Output(eq->right, fp);
	//fflush(fp);
	return ;
}

char * PPEquation_toString(char * res, PPEquation * eq){
	res[0] = 0;
	if(eq == NULL){
		return res;
	}else{
		char t1[STR_BUFFER], t2[STR_BUFFER];
		PolyPolynomial_toString(t1, eq->left);
		PolyPolynomial_toString(t2, eq->right);
		string_right_trip_middle(t1);
		string_right_trip_middle(t2);
		//sprintf(res, "LEFT %s = RIGHT %s ", t1, t2);
		sprintf(res, "%s =%s ", t1, t2);
		//strcat(res, t1);
		//strcat(res, " = ");
		//strcat(res, t2);
		return res;
	}
}

void PPEquation_Output(PPEquation * eq, FILE * fp){
	if(eq == NULL)
		return ;

	PolyPolynomial_Output(eq->left, fp);
	fprintf(fp, " = ");
	PolyPolynomial_Output(eq->right, fp);
	//fflush(fp);
	return ;
}
/*
 *	a = b -->> a-b = 0;
 *
 * */
int  Equation_Process(Equation * eq){
	int updated;
	if(eq == NULL || eq->left == NULL || eq->right == NULL){
		return 0;
	}
	updated = 0;
#if 0
	{
		char t1[STR_BUFFER], t2[STR_BUFFER];
		TEXDEBUG("LEFT : $%s$, ", Polynomial_toString(t1, eq->left));
		//for(p = eq->left->monomials; p!= NULL; p = p->next){
		//	MDEBUG("left: %p", p);
		//}
		MDEBUG("RIGHT: $%s$\r\n\r\n", Polynomial_toString(t1, eq->right));
		//for(p = eq->right->monomials; p!= NULL; p = p->next){
		//	MDEBUG("right: %p", p);
		//}
	}
#endif
	if(math_list_length(eq->right->monomials) > 0){
		math_list * p;
		for(p = eq->right->monomials; p!= NULL; p = p->next){
			Monomial * m = p->data;
			m->num_coeff = m->num_coeff * (-1);
			updated ++;
		}
		eq->left->monomials = math_list_cat(eq->left->monomials, eq->right->monomials);
	}
	math_list * node = math_list_node_new();
	Monomial * m = Monomial_new();
	m->num_coeff = 0;
	node->data = m;
	eq->right->monomials = node;
#if 0
	{
		char t1[STR_BUFFER], t2[STR_BUFFER];
		TEXDEBUG("After LEFT : $%s$, ", Polynomial_toString(t1, eq->left));
		//for(p = eq->left->monomials; p!= NULL; p = p->next){
		//	TEXDEBUG("left: %p", p);
		//}
		TEXDEBUG("After RIGHT: $%s$\r\n\r\n", Polynomial_toString(t1, eq->right));
		//for(p = eq->right->monomials; p!= NULL; p = p->next ){
		//	TEXDEBUG("right: $%p$", p);
		//}
	}
#endif
	//updated += Polynomial_rearrange_round(eq->left);
	updated += math_list_sort_increase(eq->left->monomials, NULL, (math_list_compare_func)Monomial_compare);
	eq->left->monomials = math_list_begin(eq->left->monomials);
	updated += Polynomial_cal_sum(eq->left);
	return updated;
}

int  Equation_compare(Equation * a, Equation *b){
	int ret;
	//MDEBUG("a:%p, b:%p", a, b);
	if(a == NULL){
		if(b == NULL)return 0;
		return -1;
	}
	if(b == NULL){
		return 1;
	}
#if 0
	{
		
		char t1[STR_BUFFER], t2[STR_BUFFER];
		TEXDEBUG("Equation : [$%s$] comparing with [$%s$], \r\n\r\n", Equation_toString(t1, a), Equation_toString(t2, b));
	}
#endif
	ret = Polynomial_compare(a->left, b->left);
	if(ret){
		return ret;
	}

	ret = Polynomial_compare(a->right, b->right);
	if(ret){
		return ret;
	}

	//TEXDEBUG("Comparing result 0.\r\n\r\n");
	return 0;
}


//Equations solve by matrix: Bugs. So abandonned.
//All algebra elements in Monomials of type < type, will be put into the entries;
Matrix * Equation_to_matrix(math_list * eq_list, ElementType type){
	Matrix * mat;
	if(eq_list == NULL)return NULL;
	mat = Matrix_new();
	mat->setType(mat, MATRIX_ENTRY_POLYNOMIAL);
	
	math_list * p, *q;
	math_list * vs;

	vs = NULL;
	for(p = eq_list; p!= NULL; p = p->next){
		Equation * eq = p->data;
		for(q = eq->left->monomials; q!= NULL; q = q->next){
			AlgebraElement * ele = Monomial_find_AlgebraElement_by_type(q->data, type);
			if(ele == NULL){
				continue;
			}
			if(math_list_find_data(vs, ele, (math_list_compare_func)AlgebraElement_compare_variable ) == NULL){
#if 0
				{	
					char tmp[16];
					MDEBUG("Got new element: [%s][%d]", ele->name, ele->index);
				}
#endif
				vs = math_list_append_from_data(vs, ele);
			}
		}
	}

	math_list_sort_increase(vs, NULL, (math_list_compare_func)AlgebraElement_compare_variable );
	vs = math_list_begin(vs);

	int row, col;
	row = math_list_length(eq_list);
	col = math_list_length(vs) + 1;
	
	MDEBUG("Size: %d , %d", row, col);
	Matrix_resize(mat, row, col);
	
	int i, j;

	{
		char str[STR_BUFFER];
		str[0] = 0;
		for(j=0; j<col-1; j++){
			//Get coeffs of a_1, a_2, or a_3
			AlgebraElement * ele = math_list_data_at(vs, j);
			char t1[16];
			AlgebraElement_toString(t1, ele);
			strcat(str, t1);
			if(j < col - 2)strcat(str, ", ");
		}
		MDEBUG("AlgeElements: %s", str);
		TEXDEBUG("\r\n\r\nTo solve the variables: $%s$ in colomns with extra consts col: \r\n\r\n", str);
	}
	for(i = 0; i<row; i++){
		Equation * eq = math_list_data_at(eq_list, i);
		{
			char str[STR_BUFFER];
			Equation_toString(str, eq);
			MDEBUG("Equation Polynomial %s", str);
		}
		// eq : [ a_1 + q a_2 + 4 a_2 + a_3  = 0 ]
		Polynomial * poly = eq->left;
		for(j=0; j<col-1; j++){
			//Get coeffs of a_1, a_2, or a_3
			AlgebraElement * ele = math_list_data_at(vs, j);
			//In this found, ele may be a_2
			
			Polynomial * new_p = Polynomial_new();
			math_list * node;
			for(node = poly->monomials; node != NULL; node = node->next){
				//poly = a_1 + q a_2 + 4 a_2 + a_3 - 2q + 3
				Monomial * mo = node->data;
				math_list * s_end = math_list_find_data(mo->elements, ele, (math_list_compare_func) AlgebraElement_compare_variable );
				if(s_end != NULL){
					//found a_2 once, may be [q a_2 ] [4 a_2 ]
					Monomial * nm = Monomial_get_coeff_by_type(mo, type);
					if(nm != NULL){
						new_p->monomials = math_list_append_from_data(new_p->monomials, nm);
					}
				}
			}
			//new_p = [q] + [4]
			Matrix_Polynomial_set_entry(mat, i, j, new_p);
			{
				char str[STR_BUFFER];
				str[0] = 0;
				Polynomial_toString(str, new_p);
				MDEBUG("Got coeff Polynomial [%s]", str);
			}
		}
		{
			Polynomial * new_p = Polynomial_new();
			math_list * node;
			for(node = poly->monomials; node != NULL; node = node->next){
				Monomial * mo = node->data;
				AlgebraElement * ele = Monomial_find_AlgebraElement_by_type(mo, type);
				if(ele == NULL){
					//found q, 1 
					Monomial * nm = Monomial_copy(mo);
					new_p->monomials = math_list_append_from_data(new_p->monomials, nm);
				}
			}
			//new_p = -2q + 3
			Matrix_Polynomial_set_entry(mat, i, col, new_p);
			{
				char str[STR_BUFFER];
				Polynomial_toString(str, new_p);
				MDEBUG("Got const Polynomial %s", str);
			}
		}
	}
	
	mat->extra1 = vs;
	return mat;
}

int dump_Equation(Matrix * mat){
	math_list * p;
	if(NULL == mat)return 0;
	char tmp[128];

	TEXDEBUG("Variables: [");
	for(p = mat->extra1; p!= NULL; p = p->next){
		AlgebraElement * e = p->data;
		AlgebraElement_toString(tmp, e);
		TEXDEBUG("$%s$, ", tmp);
	}
	TEXDEBUG("]. \n\n");

#if 0
	TEXDEBUG("Zero Variables: [");
	for(p = mat->extra2; p!= NULL; p = p->next){
		Equation * e = p->data;
		Equation_toString(tmp, e);
		TEXDEBUG("$%s$, ", tmp);
	}
	TEXDEBUG("]. \n\n");
#endif
	return 0;
}
int Equation_solve_matrix(Matrix * mat){
	int updated;
	int i, j, tag;

	updated = 0;
	//math_list * vs = mat->extra1; 
	//variables: a2, a3, .....
	//math_list * solves;
	
	dump_Equation(mat);

	for(i = 0; i < mat->rows; i++){
		tag = 0;
		for(j = 0; j < mat->cols-1; j++){
			Polynomial * poly;
			poly = Matrix_get(mat, i, j);
			if(poly != NULL && poly->monomials != NULL && math_list_length(poly->monomials) >0){
				tag++; 
				break;
			}
		}
		if(tag == 0){
			int ii, jj;
			//Move i+1 to i;
			for(ii = i; ii < mat->rows; ii++){
				for(jj = 0; jj < mat->cols-1; jj++){
					//Polynomial * polyt;
					Polynomial *polytp;
					//polyt = Matrix_get(mat, ii, jj);
					polytp = Matrix_get(mat, ii+1, jj);
					//Polynomial_free(polyt);
					Matrix_Polynomial_set_entry(mat, ii, jj, polytp);
					Matrix_Polynomial_set_entry(mat, ii+1, jj, NULL);
				}
			}
			mat->rows --;
			//updated ++;
			//TEXDEBUG("\r\n\r\nDeleted %d rows.\r\n\r\n", updated);
			//return updated;
		}
	}

	if(updated > 0){
		TEXDEBUG("\r\n\r\nDeleted %d rows.\r\n\r\n", updated);
		return updated;
	}

	for(i = 0; i < mat->rows; i++){
		int ii, jj;
		int ti, tj;
		jj = -1;
		tag = 0;
		//TEXDEBUG("\n\n------%d---\n\n", __LINE__);
		//dump_Equation(mat);
		
		//try to find a single element variable in this row, then it is 0
		for(j = 0; j < mat->cols-1; j++){
			Polynomial * poly;
			poly = Matrix_get(mat, i, j);
			if(poly != NULL && poly->monomials != NULL && math_list_length(poly->monomials) >0){
				tag ++; 
				tj = j;
				ti = i;
				//break;
			}
		}
		//TEXDEBUG("\n\n------%d---\n\n", __LINE__);
		if(tag == 1){
			//Got it.
			//Matrix_Polynomial_to_Tex(mat);
			MDEBUG("In %d-th row, Got %d-th col to be zero.", ti, tj);
			TEXDEBUG("\n\nIn %d-th row, Got %d-th col to be zero.\n\n", ti, tj);
#if 0
			{
				char tmp[STR_BUFFER];
				for(jj = 0; jj < mat->cols; jj++){
					Polynomial * poly;
					poly = Matrix_get(mat, ti, jj);
					Polynomial_toString(tmp, poly);
					TEXDEBUG("$%s$, ", tmp);
				}
				TEXDEBUG("\n\n");
			}
#endif
			//Matrix_Polynomial_to_Tex(mat);
			//Set all col tj to 0;
			for(ii = 0; ii < mat->rows; ii++){
				//TEXDEBUG("\n\n------%d---\n\n", __LINE__);
				if(ii == ti) continue;
				Polynomial * poly;
				poly = Matrix_get(mat, ii, tj);
				//TEXDEBUG("\n\n------%d---\n\n", __LINE__);
				if(poly == NULL)continue;
				{
					//char tmp[128];
					MDEBUG("Set polynomial %d, %d %p", ii, j, poly);
					//MDEBUG("Got polynomial->monomials %p", poly->monomials);
					//Polynomial_toString(tmp, poly);
					//MDEBUG("Got polynomial %s", tmp);
				}
				//TEXDEBUG("\n\n------%d---\n\n", __LINE__);
				//Polynomial_free(poly);
				//TEXDEBUG("\n\n------%d---\n\n", __LINE__);
				Matrix_Polynomial_set_entry(mat, ii, tj, NULL);
				//TEXDEBUG("\n\n------%d---\n\n", __LINE__);
			}
			
			//Matrix_Polynomial_to_Tex(mat);
			
			//TEXDEBUG("\n\n------%d---\n\n", __LINE__);
			{
			//remove from variable list;
			math_list * node = math_list_node_at(mat->extra1, tj);
			if(tj == 0){
				mat->extra1 = node->next;
			}
			AlgebraElement * ele = AlgebraElement_copy(node->data);
			//math_list_data_at(mat->extra1, tj);
			char tmp[128];
			AlgebraElement_toString(tmp, ele);
			TEXDEBUG("\n\nGot zero: $%s$.\n\n", tmp);
			//TEXDEBUG("Before remove it, head = %p", mat->extra1);
			math_list_remove(node);
			//TEXDEBUG("After  remove it, head = %p", vs);

			//TEXDEBUG("\n\n------%d---\n\n", __LINE__);
			//Solves
			Monomial * mo = Monomial_new();
			mo->elements = math_list_node_from(ele);
			Polynomial * left, *right;
			left = Polynomial_new();
			left->monomials = math_list_node_from(mo);
			right = Polynomial_new();

			//TEXDEBUG("\n\n------%d---\n\n", __LINE__);
			Equation * s1 = Equation_new();
			s1->left = left;
			s1->right = right;

			//solves = math_list_append_from_data(solves, s1);
			//TEXDEBUG("\n\n------%d---%p\n\n", __LINE__, mat->extra2);
			dump_Equation(mat);
			mat->extra2 = math_list_append_from_data(mat->extra2, s1);
			//TEXDEBUG("\n\n------%d---%p\n\n", __LINE__, mat->extra2);
			dump_Equation(mat);

			//TEXDEBUG("\n\n------%d---\n\n", __LINE__);
			//Matrix_Polynomial_to_Tex(mat);

			for(jj = tj; jj < mat->cols-1; jj++){
				for(ii = 0; ii < mat->rows; ii++){
					//Polynomial * poly_t;
					Polynomial *poly_tp;
					//poly_t = Matrix_get(mat, ii, jj);
					poly_tp = Matrix_get(mat, ii, jj+1);
					Matrix_Polynomial_set_entry(mat, ii, jj, poly_tp);
					Matrix_Polynomial_set_entry(mat, ii, jj+1, NULL);
#if 0
					{
						char t1[128], t2[128];
						Polynomial_toString(t1, poly_t);
						Polynomial_toString(t2, poly_tp);
						TEXDEBUG("\n\n------%d---(%d,%d):[%s]\n\n", __LINE__, ii, jj, t1);
						TEXDEBUG("\n\n------%d---(%d,%d):[%s]\n\n", __LINE__, ii, jj+1, t2);
					}
#endif
					//Polynomial_free(poly_t);
					
				}

			}
			mat->cols--;
			//TEXDEBUG("\n\n------%d---\n\n", __LINE__);
			//Matrix_Polynomial_to_Tex(mat);
			//TEXDEBUG("\n\n------%d---\n\n", __LINE__);

			}//end of remove col

			updated ++;

		} // end of tag == 1
	} //for(i = 0; i < mat->rows; i++)

	//TEXDEBUG("\n\n------%d---\n\n", __LINE__);
	/*
	if(solves != NULL){
		math_list * stored_solves = mat->extra2;
		if(stored_solves == NULL){
			mat->extra2 = solves;
		}else{
			math_list_cat(stored_solves, solves);
		}
	}
	*/
	if(updated > 0){
		//TEXDEBUG("\r\n\r\nGot 0 variables: %d. \r\n\r\n", updated);
		return updated;
	}

	return updated;
}

PPEquation * PPEquation_from_Equation(Equation * eq,  ElementType type){
	if(eq == NULL) return NULL;
	PPEquation * res = PPEquation_new();
	res->left = PolyPolynomial_from_Polynomial(eq->left, type);
	res->right = PolyPolynomial_from_Polynomial(eq->right, type);

	return res;
}

math_list * PPEquations_solve(math_list ** res_p, math_list ** ppeqs_p){

	PPEquations_solve_1(res_p, ppeqs_p);
	PPEquations_solve_2(res_p, ppeqs_p);

	return *res_p;
}

math_list * PPEquations_solve_1(math_list ** res_p, math_list ** ppeqs_p){
	if(NULL == ppeqs_p)return NULL;
	math_list *ppeqs = *ppeqs_p;
	if(NULL == ppeqs)return NULL;
	math_list * p, *q;
	math_list * res = *res_p;
	for(p = ppeqs; p!= NULL;){
		q = p->next;

		PPEquation * pe = p->data;
		if(pe== NULL || pe->left == NULL || pe->left->polymonomials == NULL){
			if(p->pre == NULL){
				ppeqs = math_list_begin(q);
			}else{
				ppeqs = math_list_begin(p->pre);
			}
			math_list_remove(p);
			p = q;
			continue;
		}
		PolyPolynomial * pp = pe->left;

		//if(pp->polymonomials == NULL){
		//	math_list_remove(p);
		//	p = q;
		//	continue;
		//}
		if(math_list_length(pp->polymonomials) == 1){
			////////////////
			PolyMonomial * pm1 = pp->polymonomials->data;

			Equation * eq= Equation_new();
			Monomial * mleft = Monomial_copy(pm1->monomial);
			Polynomial * eleft = Polynomial_new();
			eleft->monomials = math_list_node_from(mleft);
			eq->left = eleft;
			Polynomial * eright = Polynomial_new();
			Monomial * mright = Monomial_new();
			mright->num_coeff = 0;
			eq->right = eright;

			math_list * node = math_list_node_from(eq);
			MDEBUG("new node: %p", node);
			res = math_list_append(res, node);
			MDEBUG("res:%p, new node: %p, pre:%p, next:%p", res, node, node->pre, node->next);
#if 0
			{
				math_list * px;
				int ix=0;
				for(px = res; px!=NULL && ix < 100; px = px->next, ix++){
					MDEBUG("check round: %p, pre:%p, next:%p", px, px->pre, px->next);
				}
			
			}
			char eq_s[STR_BUFFER];
			PPEquation_toString(eq_s, pe);
			TEXDEBUG("Equation {$%s$} ", eq_s);
			Equation_toString(eq_s, eq);
			TEXDEBUG("Become Equation {$%s$}.", eq_s);
			TEXDEBUG("\n\n");
#endif
			//////////////////


			if(p->pre == NULL){
				ppeqs = math_list_begin(q);
			}else{
				ppeqs = math_list_begin(p->pre);
			}
			math_list_remove(p);
			PolyMonomial * pm = pp->polymonomials->data;
			math_list * mlist =pm->coeff->monomials->next;
			Monomial * mo = pm->coeff->monomials->data;
			for(; mlist != NULL; ){
				math_list * n = mlist->next;
				Monomial * nm = mlist->data;
				Monomial_delete(nm);
				math_list_delete_node(mlist);
				mlist = n;
			}
			Monomial_delete(mo);
			mo->num_coeff = 1;
			p = q;

			continue;
		}
		p = q;

	}
#if 0
	{
		math_list * px;
		int ix=0;
		for(px = res; px!=NULL && ix < 100; px = px->next, ix++){
			MDEBUG("check round: %p, pre:%p, next:%p", px, px->pre, px->next);
		}
	
	}
#endif
	*ppeqs_p = ppeqs;
	*res_p = res;
	return res;
}

Equation * PPEquation_to_solve_Equation(PPEquation * ppeq){
	MDEBUGP();
	
	if(ppeq == NULL || ppeq->left == NULL || ppeq->left->polymonomials == NULL){
		return NULL;
	}
	math_list * pms = ppeq->left->polymonomials;
	if(math_list_length(pms) != 2){
		return NULL;
	}

#if 0
	{
		char eq_s[STR_BUFFER];
		PPEquation_toString(eq_s, ppeq);
		TEXDEBUG("\n\nEQUATION:  {$%s$}, ", eq_s);
	}
#endif
	Equation * res = NULL;
	//Polynomial [-1]
	Monomial* mo1 = Monomial_from_string("-1");
	Polynomial * p_m1 = Monomial_copy_to_Polynomial(mo1);

	Monomial* mo2 = Monomial_from_string("1");
	Polynomial * p_1 = Monomial_copy_to_Polynomial(mo2);

	// (f+h) a_1 + g a_2 = 0;
	// pm1 = (f+h)a_1
	// pm2 = g a_2
	PolyMonomial * pm1 = pms->data;
	PolyMonomial * pm2 = pms->next->data;
	PolyMonomial * pm_left = NULL, *pm_right = NULL;
	Monomial *mleft = NULL;
	Monomial * mright = NULL;
		
	Polynomial * poly_coeff1, *poly_coeff2;
	Polynomial * poly_left=NULL, *poly_right=NULL;
	Polynomial * mulp;
	poly_coeff1 = pm1->coeff;
	poly_coeff2 = pm2->coeff;
	mulp = Polynomial_mul_Polynomial(p_m1, poly_coeff1);
		
#if 0
	{
		char eq_s[STR_BUFFER];
		Polynomial_toString(eq_s, pm1->coeff);
		TEXDEBUG("Coeff:  %p {$%s$}, ", pm1->coeff,  eq_s);
		Polynomial_toString(eq_s, pm2->coeff);
		TEXDEBUG("Coeff:  %p {$%s$}, ",  pm2->coeff, eq_s);
	}
#endif
	//TEXDEBUG("\n\n------\n\n");
	//Get coeff length 1:
	if( (poly_coeff1 == NULL) || (poly_coeff2 == NULL)){
		//TEXDEBUG("No coefficient, return .\n\n");
		return NULL;
	}

	int tag = 0;
	if(Polynomial_compare(poly_coeff1, poly_coeff2) == 0){
		//TEXDEBUG("satisfies -1 equal.");
		res= Equation_new();
		res->left = Monomial_copy_to_Polynomial(pm1->monomial);
		res->right = Polynomial_mul_Polynomial(p_m1, Monomial_copy_to_Polynomial(pm2->monomial));
	}else if(Polynomial_compare(mulp, poly_coeff2) == 0){
		//TEXDEBUG("equation equals.");
		res= Equation_new();
		res->left = Monomial_copy_to_Polynomial(pm1->monomial);
		res->right = Monomial_copy_to_Polynomial(pm2->monomial);
	}
	if(res){
#if 0
		{
		char eq_s[STR_BUFFER];
		PPEquation_toString(eq_s, ppeq);
		TEXDEBUG("\n\nEquation:  {$%s$}. ", eq_s);
		Equation_toString(eq_s, res);
		TEXDEBUG("Solution:{$%s$}. \n\n", eq_s);
		}
#endif
		TEXDEBUG("\n\n");
		return res;
	}
	
#if 0
	{
		char eq_s[1024];
		Monomial_toString(eq_s, pm1->monomial);
		TEXDEBUG("Monomial ({$%s$}), ", eq_s);
		Monomial_toString(eq_s, pm2->monomial);
		TEXDEBUG("Monomial ({$%s$}), ", eq_s);
	}
#endif
	if( (math_list_length(pm1->coeff->monomials)==1) && (math_list_length(pm2->coeff->monomials) == 1 )){
		Monomial * m1, *m2;
		m1 = pm1->coeff->monomials->data;
		m2 = pm2->coeff->monomials->data;
		if(math_list_length(m2->elements)==0){
			pm_left = pm2;
			pm_right = pm1;
		}else if(math_list_length(m1->elements)==0){
			pm_left = pm1;
			pm_right = pm2;
		}else{
			// q a_1 + q a_2;
			return NULL;
		}
	}else{
		// a_1 + f a_2
		if(math_list_length(pm1->coeff->monomials) == 1 ){
			pm_left = pm1;
			pm_right = pm2;
		}else if(math_list_length(pm2->coeff->monomials) == 1 ){
			pm_left = pm2;
			pm_right = pm1;
		}else{
			return NULL;
		}
		Monomial * m1;
		m1 = pm_left->coeff->monomials->data;
		if(math_list_length(m1->elements)>0){
			//q a_1;
			return NULL;
		}
	}
	pm_right->coeff = Polynomial_mul_Polynomial(pm_right->coeff, p_m1);
	if(Polynomial_compare(pm_left->coeff, p_m1) == 0){
		pm_right->coeff = Polynomial_mul_Polynomial(pm_right->coeff, p_m1);
		poly_left = Monomial_copy_to_Polynomial(pm_left->monomial);
		poly_right = Polynomial_mul_Polynomial(pm_right->coeff, Monomial_copy_to_Polynomial(pm_right->monomial));
	}else if(Polynomial_compare(pm_left->coeff, p_1) == 0){
		poly_left = Monomial_copy_to_Polynomial(pm_left->monomial);
		poly_right = Polynomial_mul_Polynomial(pm_right->coeff, Monomial_copy_to_Polynomial(pm_right->monomial));
	}else{
		return NULL;
	}

	res= Equation_new();
	res->left = poly_left;
	res->right = poly_right;
	
#if 0
	{
		char eq_s[1024];
		Polynomial_toString(eq_s, pm_left->coeff);
		TEXDEBUG("Left coeff: ({$%s$}), ", eq_s);
		Polynomial_toString(eq_s, pm_right->coeff);
		TEXDEBUG("Right coeff: ({$%s$}), ", eq_s);
	}
#endif
#if 0
	{
		char eq_s[1024];
		Polynomial_toString(eq_s, poly_left);
		TEXDEBUG("Left : ({$%s$}), ", eq_s);
		Polynomial_toString(eq_s, poly_right);
		TEXDEBUG("Right: ({$%s$}), ", eq_s);
	}
#endif
#if 0
	{
		char eq_s[1024];
		PPEquation_toString(eq_s, ppeq);
		TEXDEBUG("\n\nEquation:  {$%s$}. ", eq_s);
		Equation_toString(eq_s, res);
		TEXDEBUG("Solution: ({$%s$}). ", eq_s);
	}
#endif
	
	if(res){
		return res;
	}
#if 0
		////////////////
		Equation * eq= Equation_new();
		
		//Monomial * mleft = Monomial_copy(pm1->monomial);
		//Polynomial * eleft = Polynomial_new();
		//eleft->monomials = math_list_node_from(mleft);
		Polynomial * eleft = Monomial_copy_to_Polynomial(pm1->monomial);
		eq->left = Polynomial_mul_Polynomial(eleft, pleft);

		//Monomial * mright = Monomial_copy(pm2->monomial);
		//Polynomial * eright = Polynomial_new();
		//eright->monomials = math_list_node_from(mright);
		Polynomial * eright = Monomial_copy_to_Polynomial(pm2->monomial);
		eq->right = Polynomial_mul_Polynomial(eright, pright);

		//math_list_sort_increase(eq->left, NULL, (math_list_compare_func)AlgebraElement_compare_variable );
	        //eq->left->monomials = math_list_begin(eq->left->monomials);
		//math_list_sort_increase(eq->right->monomials, NULL, (math_list_compare_func)AlgebraElement_compare_variable );
	        //eq->right->monomials = math_list_begin(eq->right->monomials);

		////
		Equation_toString(eq_s, eq);
		//TEXDEBUG("Become Equation {$%s$}.", eq_s);
		////
		//mright->num_coeff = mright->num_coeff * (-1);
		tag = 0;
		mleft = eq->left->monomials->data;
		mright = eq->right->monomials->data;
		if(math_list_length(eq->right->monomials) == 1){
			//TEXDEBUG("RIGHT=1;");
			if(math_list_length(eq->left->monomials) > 1){
				//TEXDEBUG("LEFT > 1;");
				tag = 1;
			}else{
				if(math_list_length(mright->elements) == 1){
					//TEXDEBUG("LEFT = 1 but RIGHT no q");
					tag = 1;
				}
			}
			
		}
		if(tag == 1){
			Polynomial * tmp;
			tmp = eq->left;
			eq->left = eq->right;
			eq->right = tmp;
		}
#if 1
		mleft = eq->left->monomials->data;
		if( (math_list_length(eq->left->monomials) == 1 )
			&& (math_list_length(mleft->elements) == 1) ){
			if(mleft->coeff == -1){
				mleft->coeff = 1;
				eq->right = Polynomial_mul_Polynomial(newp, eq->right);
			}
			if(mleft->coeff == 1){
				math_list * node = math_list_node_from(eq);
				MDEBUG("new node: %p", node);
				res = math_list_append(res, node);
				MDEBUG("new node: %p, pre:%p, next:%p", node, node->pre, node->next);
#if 0
				{
					math_list * px;
					int ix=0;
					for(px = res; px!=NULL && ix < 100; px = px->next, ix++){
						MDEBUG("check round: %p, pre:%p, next:%p", px, px->pre, px->next);
					}
				
				}
				Equation_toString(eq_s, eq);
				TEXDEBUG("Got solution {$%s$}.", eq_s);
				TEXDEBUG("\n\n");
				TEXDEBUG("\n\n");
#endif
				math_list_remove(na);
			}
		}
#endif
#endif
		//////////////////
	return res;
}
math_list * PPEquations_solve_2(math_list ** res_p, math_list ** ppeqs_p){
	if(NULL == ppeqs_p)return NULL;
	math_list *ppeq_list = *ppeqs_p;

	if(NULL == ppeq_list)return NULL;

	math_list * res = NULL;
	math_list * na, *nb;
	res = *res_p;


	//Polynomial [-1]
	Monomial* mo1 = Monomial_from_string("-1");
	Polynomial * newp = Monomial_copy_to_Polynomial(mo1);

	Monomial* mo2 = Monomial_from_string("1");
	Polynomial * new1 = Monomial_copy_to_Polynomial(mo2);

	for(na = ppeq_list;  na!= NULL; ){
		nb = na->next;
		Equation *eq = PPEquation_to_solve_Equation(na->data);
		if(eq!= NULL){
			res = math_list_append_from_data(res, eq);
			math_list_remove(na);
		}
		na = nb;
	}
	return res;
}

void Equation_delete(Equation *eq){
	if(eq == NULL) return ;
	Polynomial_delete(eq->left);
	Polynomial_delete(eq->right);
	free(eq);
}
void PPEquation_delete(PPEquation *eq){
	if(eq == NULL) return ;
	PolyPolynomial_delete(eq->left);
	PolyPolynomial_delete(eq->right);
	free(eq);
}
