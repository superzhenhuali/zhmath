#include <stddef.h>
#include <string.h>

#include "math_list.h"
#include "testing.h"
#include "logs.h"
#include "equation.h"

void print_formula(const char * s){
	TEXDEBUG("\n" FORMULAR_LEFT "\n");
	TEXDEBUG("%s", s);
	TEXDEBUG("\n" FORMULAR_RIGHT "\n");
}
void print_z(Polynomial * z){
	//char zs[STR_BUFFER];
	//TEXDEBUG("\n\n---%s : %d---\n\n", __FILE__, __LINE__);
	//Polynomial_toString(zs, z);
	//TEXDEBUG("\n\n---%s : %d---\n\n", __FILE__, __LINE__);
	TEXDEBUG("Center: \n\n");
	TEXDEBUG(FORMULAR_LEFT);
	TEXDEBUG("z &=");
	Polynomial_Output(z, stdout);
	Polynomial_Output(z, tex_log);
	TEXDEBUG(" ");
	TEXDEBUG(FORMULAR_RIGHT);
		
	//TEXDEBUG("\n\n---%s : %d---\n\n", __FILE__, __LINE__);
}
void print_ppz(PolyPolynomial * z){
	char zs[STR_BUFFER];
	//TEXDEBUG("\n\n---%s : %d---\n\n", __FILE__, __LINE__);
	//TEXDEBUG("\n\n---%s : %d---\n\n", __FILE__, __LINE__);
	TEXDEBUG("Center: \n\n");
	TEXDEBUG(FORMULAR_LEFT);
	TEXDEBUG("z &=");
	PolyPolynomial_Output(z, stdout);
	PolyPolynomial_Output(z, tex_log);
	TEXDEBUG(FORMULAR_RIGHT);
		
	//TEXDEBUG("\n\n---%s : %d---\n\n", __FILE__, __LINE__);
}
int HeckeCliff_calculate_poly(Polynomial * poly){
	//char str[STR_BUFFER];
	int updated;
	// SINGLE ONE
	//	TEXDEBUG("\n\n------%d,-\n\n", __LINE__);
	//Polynomial_toString(str, poly);
	//MDEBUG("String length %u", strlen(str));
	//TEXDEBUG("&= %s \\\\\n", str);
	int i=0;
	do{
		i++;
		TEXDEBUG("\n\n---%s : %d---%d elements, round %d .\n\n", __FILE__, __LINE__, math_list_length(poly->monomials), i);
		//MDEBUGP();
		//MDEBUG("Polynomial = %s", Polynomial_toString(str, poly));
		Polynomial_cal_sum(poly);
		updated = Polynomial_rearrange_round(poly);
		MDEBUG("Updated: %d", updated);
		//MDEBUG("Polynomial = %s", Polynomial_toString(str, poly));
		//TEXDEBUG("& \\mbox{sort Return updated: %d } \\\\\n", updated);
		if(updated){
			//Polynomial_toString(str, poly);
			//MDEBUG("String length %d", strlen(str));
			////TEXDEBUG("&= %s \\\\\n", str);
			//MDEBUG("Poly %s", str);

			continue;
		}
		//MDEBUGP();
		TEXDEBUG("\n\n---%s : %d---%d elements.\n\n", __FILE__, __LINE__, math_list_length(poly->monomials));
		//updated = HeckeCliff_Polynomial_sort(poly);
		//TEXDEBUG("\n\n---%s : %d---\n\n", __FILE__, __LINE__);
		MDEBUG("hecke poly sort return updated: %d ", updated);
		//TEXDEBUG("& \\mbox{hecke poly sort Return updated: %d } \\\\\n", updated);
		if(updated){
			//Polynomial_toString(str, poly);
			//MDEBUG("String length %d", strlen(str));
			////TEXDEBUG("&= %s \\\\\n", str);
			//MDEBUG("Poly %s", str);
			continue;
		}
		MDEBUGP();
		TEXDEBUG("\n\n---%s : %d---\n\n", __FILE__, __LINE__);
		updated = HeckeCliff_Polynomial_calculate(poly);
		TEXDEBUG("\n\n---%s : %d---\n\n", __FILE__, __LINE__);
		MDEBUG("calculate Return updated: %d ", updated);
		//MDEBUG("Poly %s", Polynomial_toString(str, poly));
		//TEXDEBUG("& \\mbox{hecke calculate Return updated: %d } \\\\\n", updated);
		MDEBUGP();
		if(updated){
			//MDEBUGP();
			//Polynomial_toString(str, poly);
			//MDEBUG("String length %u", strlen(str));
			//MDEBUGP();
			////TEXDEBUG("&= %s \\\\\n", str);
			//MDEBUG("Poly %s", Polynomial_toString(str, poly));
			continue;
		}

		//TEXDEBUG("\n\n---%s : %d---\n\n", __FILE__, __LINE__);
		//break;
	}while(updated);
	return 0;
}

int HeckeCliff_calculate_polypoly(PolyPolynomial * poly){
	int updated;
	int i=0;
	do{
		i++;
		//TEXDEBUG("\n\n---%s : %d---%d elements, round %d .\n\n", __FILE__, __LINE__, math_list_length(poly->polymonomials), i);
#if 0
                {
                      char s1[16*1024], s2[16*1024];
                      PolyMonomial_toString(s1, poly->polymonomials->data);
                      PolyMonomial_toString(s2, poly->polymonomials->next->data);
                      TEXDEBUG("-----TTT %d -------%p, %p[ %s + %s = ", i, poly->polymonomials, poly->polymonomials->next, s1, s2);
		}
#endif
		PolyPolynomial_cal_sum(poly);
#if 0
                {
                      char s1[16*1024], s2[16*1024];
                      PolyMonomial_toString(s1, poly->polymonomials->data);
                      PolyMonomial_toString(s2, poly->polymonomials->next->data);
                      TEXDEBUG("-----TTT %d -------%p, %p[ %s + %s = ", i, poly->polymonomials, poly->polymonomials->next, s1, s2);
		}
#endif
		//TEXDEBUG("\n\n CEN ---%s : %d---%d elements, round %d .\n\n", __FILE__, __LINE__, math_list_length(poly->polymonomials), i);
		updated = PolyPolynomial_rearrange_round(poly);
#if 0
                {
                      char s1[16*1024], s2[16*1024];
                      PolyMonomial_toString(s1, poly->polymonomials->data);
                      PolyMonomial_toString(s2, poly->polymonomials->next->data);
                      TEXDEBUG("-----TTT %d -------%p, %p[ %s + %s = ", i, poly->polymonomials, poly->polymonomials->next, s1, s2);
		}
#endif
		MDEBUG("Updated: %d", updated);
		//MDEBUG("Polynomial = %s", Polynomial_toString(str, poly));
		//TEXDEBUG("& \\mbox{sort Return updated: %d } \\\\\n", updated);
		if(updated){
			//Polynomial_toString(str, poly);
			//MDEBUG("String length %d", strlen(str));
			////TEXDEBUG("&= %s \\\\\n", str);
			//MDEBUG("Poly %s", str);

			continue;
		}
		//MDEBUGP();
		//TEXDEBUG("\n\n---%s : %d---%d elements.\n\n", __FILE__, __LINE__, math_list_length(poly->polymonomials));
		//updated = HeckeCliff_Polynomial_sort(poly);
		//TEXDEBUG("\n\n---%s : %d---\n\n", __FILE__, __LINE__);
		MDEBUG("hecke poly sort return updated: %d ", updated);
		//TEXDEBUG("& \\mbox{hecke poly sort Return updated: %d } \\\\\n", updated);
		if(updated){
			//Polynomial_toString(str, poly);
			//MDEBUG("String length %d", strlen(str));
			////TEXDEBUG("&= %s \\\\\n", str);
			//MDEBUG("Poly %s", str);
			continue;
		}
		MDEBUGP();
		//TEXDEBUG("\n\n---%s : %d---\n\n", __FILE__, __LINE__);
		//The following line causes bug
		//TEXDEBUG("\n\n---%s : %d---\n\n", __FILE__, __LINE__ );
		//int ll = 0;
		//int ll = math_list_length(poly->polymonomials);
		//TEXDEBUG("\n\n---%s : %d---\n\n", __FILE__, __LINE__ );
		//TEXDEBUG("\n\n---%s : %d---%d elements, round %d .\n\n", __FILE__, __LINE__, ll, i);
		updated = HeckeCliff_PolyPolynomial_calculate(poly);
		//TEXDEBUG("\n\nCEN ---%s : %d---%d elements, round %d .\n\n", __FILE__, __LINE__, math_list_length(poly->polymonomials), i);
		PolyPolynomial_cal_sum(poly);
		//TEXDEBUG("\n\nCEN ---%s : %d---%d elements, round %d .\n\n", __FILE__, __LINE__, math_list_length(poly->polymonomials), i);
#if 0
                {
                      char s1[16*1024], s2[16*1024];
                      PolyMonomial_toString(s1, poly->polymonomials->data);
                      PolyMonomial_toString(s2, poly->polymonomials->next->data);
                      TEXDEBUG("-----TTT %d -------%p, %p[ %s + %s = \n", i, poly->polymonomials, poly->polymonomials->next, s1, s2);
		}
#endif
		//TEXDEBUG("\n\n---%s : %d---\n\n", __FILE__, __LINE__);
		MDEBUG("calculate Return updated: %d ", updated);
		//MDEBUG("Poly %s", Polynomial_toString(str, poly));
		//TEXDEBUG("& \\mbox{hecke calculate Return updated: %d } \\\\\n", updated);
		MDEBUGP();
		if(updated){
			//MDEBUGP();
			//Polynomial_toString(str, poly);
			//MDEBUG("String length %u", strlen(str));
			//MDEBUGP();
			////TEXDEBUG("&= %s \\\\\n", str);
			//MDEBUG("Poly %s", Polynomial_toString(str, poly));
			continue;
		}

		//break;
	}while(updated);
	//TEXDEBUG("\n\n---%s : %d---\n\n", __FILE__, __LINE__);
	return 0;
}

math_list * Equations_from_PolyPolynomials(PolyPolynomial * left, PolyPolynomial *right){

	math_list * res= NULL, *p =NULL, *q= NULL;

	res = NULL;
	for(p = left->polymonomials; p != NULL; p = p->next){
		PolyMonomial * a = p->data;
		for(q = right->polymonomials; q != NULL; q = q->next){
			PolyMonomial * b = q->data;
			if(Monomial_compare(a->monomial, b->monomial) == 0){
				char tmp[1024];
				Equation * eq = Equation_new();

				MDEBUG("Got equations for [%s].", Monomial_toString(tmp, a->monomial));
				MDEBUG("coefficient a: [%s].", Polynomial_toString(tmp, a->coeff));
				MDEBUG("coefficient b: [%s].", Polynomial_toString(tmp, b->coeff));
				MDEBUG("eq:%p, left: %p", eq, eq->left);
				//Polynomial * p = Polynomial_copy(a->coeff);
				//MDEBUG("coefficient copy %p: [%s] to %p.", p, Polynomial_toString(tmp, p), eq->left);
				//eq->left = p;
				MDEBUGP();
				eq->left = Polynomial_copy(a->coeff);
				eq->right = Polynomial_copy(b->coeff);

				math_list *node = math_list_node_new();
				node->data = eq;
				res = math_list_append(res, node);
				break;
			}
		}
	}

	return res;
}
math_list * PPEquations_from_Equations(math_list * eq_list){
	math_list * pp_list = NULL;
	math_list * p;
	for(p = eq_list; p != NULL; p = p->next){
		Equation * eq = p->data;
		PPEquation * ppeq = PPEquation_from_Equation(eq, VARIABLE_E2);
		pp_list = math_list_append_from_data(pp_list, ppeq);
	}

	return pp_list;

}
PolyPolynomial * rearrage_z(Polynomial * z){
	PolyPolynomial * ppz;
	if(z == NULL)return NULL;
	math_list * p;
	for(p = z->monomials; p!= NULL; p = p->next){
		Monomial * mo = p->data;
		if(mo != NULL){
			math_list * q = mo->elements;
			AlgebraElement * ele=NULL;
			for(q = mo->elements; q!= NULL; q = q->next){
				ele = q->data;
				if(strcmp(ele->name, "a") == 0){
					ele->type = ALGEBRA_HIGH2;
				}
			}
		}
	}
	ppz = PolyPolynomial_from_Polynomial(z, ALGEBRA_HIGH1);
	return ppz;
}
void tex_output_equations(math_list * eq_list){
	math_list * eq_node;
	TEXDEBUG(FORMULAR_LEFT"\n");
	for(eq_node = eq_list;  eq_node!= NULL; eq_node = eq_node->next){
		Equation * e = eq_node->data;
		TEXDEBUG("\t & \\qquad ");
		Equation_Output(e, stdout);
		Equation_Output(e, tex_log);
		TEXDEBUG("\\\\\n");
	}
	TEXDEBUG(FORMULAR_RIGHT"\n");
	//fflush(stdout);
}
void tex_output_ppequations(math_list * ppeq_list){
	math_list * eq_node;
	TEXDEBUG(FORMULAR_LEFT"\n");
	for(eq_node = ppeq_list;  eq_node!= NULL; eq_node = eq_node->next){
		PPEquation * e = eq_node->data;
		TEXDEBUG("\t & \\qquad ");
		PPEquation_Output(e, stdout);
		PPEquation_Output(e, tex_log);
		TEXDEBUG("\\\\\n");
	}
	TEXDEBUG(FORMULAR_RIGHT"\n");
	//fflush(stdout);
}

void use_solve_zero(math_list * solves, Polynomial * poly){
	if(solves == NULL){
		return ;
	}
	math_list * sp;
	for(sp = solves; sp != NULL; sp = sp->next){
		Equation * e = sp->data;
		Monomial * mm = e->left->monomials->data;
		AlgebraElement * ele = mm->elements->data;
		char ele_str[16];
		AlgebraElement_toString(ele_str, ele);
		//TEXDEBUG("Got zero element [$%s$]\n\n", ele_str);
		math_list * p, *z_next;
		for(p = poly->monomials; p != NULL; ){
			z_next = p->next;
			Monomial * smd = p->data;
			if(math_list_find_data(smd->elements, ele, (math_list_compare_func)AlgebraElement_compare_variable ) != NULL){
				char smd_str[128];
				Monomial_toString(smd_str, smd);
				//TEXDEBUG("Monomial [$%s$] found [$%s$]\n\n", smd_str, ele_str);
				if(z_next != NULL){
					poly->monomials = math_list_begin(z_next);
				}else if(p->pre == NULL){
					poly->monomials = NULL;
				}
				//Monomial_delete(smd);
				math_list_remove(p);
				//free(p);
			}
			p = z_next;
		}
	}

}
math_list * simple_equations(math_list * eq_list){
	math_list * na, *nb;
	math_list * res, *p;
	res = NULL;

        {
                math_list * px;
                int ix=0;
                for(px = eq_list; px!=NULL && ix < 100; px = px->next, ix++){
                        MDEBUG("check round: %p, pre:%p, next:%p", px, px->pre, px->next);
                }

        }

	for(na = eq_list;  na!= NULL; ){
		MDEBUGP();
		nb = na->next;
		Equation * a = na->data;
		if(Polynomial_compare(a->left, a->right) == 0){
			math_list_remove(na);
			na = nb;
			continue;
		}
		p = math_list_find_data(res, a, (math_list_compare_func) Equation_compare);
		MDEBUGP();
		if(p == NULL){
			MDEBUGP();
			math_list_remove(na);
			MDEBUGP();
			res = math_list_append(res, na);
			MDEBUGP();
		}

		MDEBUGP();
		na = nb;
	}
	return res;
}
void resolve_self(math_list * eq_list){

	math_list * na, *nb;

	for(na = eq_list;  na!= NULL; na = na->next){
		MDEBUGP();
		Equation * ea = na->data;
		for(nb = na->next; nb != NULL; nb = nb->next){
			Equation * eb = nb->data;
			Polynomial_replace(ea->right, eb);
		}
	}
}
math_list * replace_equations(math_list * *eq_list_p, math_list * replace){
	math_list * na, *nb;
	math_list * res, *p;
	res = NULL;
	math_list * eq_list = *eq_list_p;
	for(na = eq_list;  na!= NULL; ){
		MDEBUGP();
		nb = na->next;
		Equation * a = na->data;
		p = math_list_find_data(res, a, (math_list_compare_func) Equation_compare);
		if(p == NULL){
			math_list_remove(na);
			res = math_list_append(res, na);
		}

		na = nb;
	}
	return res;
}
void simple_equations_solve(math_list * eq_list, math_list * solves){
	math_list * na;

	for(na = eq_list;  na!= NULL; na = na->next){
		Equation *a = na->data;
		use_solve_zero(solves, a->left);
	}
	return;
}

void eqlist_replace_list(math_list * eq_list, math_list* solves){
	math_list * na;
	for(na = eq_list;  na!= NULL; na = na->next){
		Equation *a = na->data;
		Polynomial_replace_list(a->left, solves);
	}
		
}
int compare_length(Monomial * m1, Monomial *m2){
	if(m1 == NULL){
		if(m2 == NULL)return 0;
		return -1;
	}
	if(m2 == NULL)return 1;
	int x1, x2, r;
	x1 = math_list_length(m1->elements);
	x2 = math_list_length(m2->elements);
	if(x1 < x2)return -1;
	if(x1 > x2) return 1;
	math_list * p, *q;

	for(p = m1->elements, q=m2->elements; p!= NULL && q!= NULL; p = p->next, q=q->next){
		r=AlgebraElement_compare(p->data, q->data);
		if(r=0)return r;
	}
	return 0;
}
void HeckeCliff_Cal_Z(int n){
	//char tmp[10240];
	//char form[STR_BUFFER+ 16];

	math_list * Tlist;
	math_list * p, *p_sub;
	Monomial * mo;
	int i;

	MDEBUGP();
	//(n !) * (2^n)
       	//Tlist = Cliff_GetAll(n);
       	Tlist = HeckeCliff_GetAll(n, 3);
	//math_list_sort_increase(Tlist, NULL, Monomial_compare);
	Tlist = math_list_begin(Tlist);
	MDEBUG("Tlist %p", Tlist);

	MDEBUGP();
	MDEBUG("count: %d, %d", n, math_list_length(Tlist));
	TEXDEBUG("\n\nGot %d basis. \n\n", math_list_length(Tlist));
	math_list_sort_increase(Tlist, NULL, (math_list_compare_func)compare_length );
	Tlist = math_list_begin(Tlist);
	i = 0;
	for(p = Tlist; p != NULL; p = p->next){
		mo = p->data;
#if 1
		{
		char tmp[STR_BUFFER];
		MDEBUG("list: %p, p:%p, mo:%p", Tlist, p, mo);
		MDEBUG("Got element %d: %s", i,Monomial_toString(tmp, mo));
		TEXDEBUG("Got basis $%d$: $%s$, \n\n", i+1, Monomial_toString(tmp, mo));
		}
#endif
		i++;
	}

	
	Polynomial * z = Polynomial_new();
	Monomial * m1, *m2;
	int xi;

	xi = 1;
	char s[16];
	z->monomials = NULL;
	for(p = Tlist; p != NULL; p = p->next, xi++){
		AlgebraElement * ele = AlgebraElement_new();

		sprintf(s, "a_%d", xi);

		//xi ++;
		//m1 = Monomial_from_string(s);
		//ele = AlgebraElement_from_string(s);
		strcpy(ele->name, "a");
		ele->index = xi;
		ele->type = VARIABLE_E2;


		//MDEBUG("Got element [%s] from string [%s]", Monomial_toString(tmp, mo), s);
		m2 = p->data;
		//mo = Monomial_mul_Monomial(m1, m2);
		mo = AlgebraElement_mul_Monomial(ele, m2);
		//AlgebraElement * _ele = mo->elements->data;
		//MDEBUG("list: %p, p:%p, mo:%p", Tlist, p, mo);
		//MDEBUG("Got element %s", Monomial_toString(tmp, mo));
		z->monomials = math_list_append_from_data(z->monomials, mo);
	}

	//Polynomial_toString(tmp, z);
	//MDEBUG("Center: %s", tmp);
	TEXDEBUG("Assume $z$ is in center, and ");
	//sprintf(form, "z &= %s, ", tmp);
	//print_formula(form);
	TEXDEBUG("\n" FORMULAR_LEFT "\n");
	TEXDEBUG("z &= ");
	Polynomial_Output(z, stdout);
	Polynomial_Output(z, tex_log);
	TEXDEBUG(FORMULAR_RIGHT "\n");

	int j=1;
	math_list * solves = NULL;
	math_list * eq_list = NULL;
	math_list * ppeq_list = NULL;
	int num_sol, num_sol_last, dup_count;
	num_sol = 0;
	dup_count = 0;
	num_sol_last = 0;
	for(p = Tlist; p != NULL; p = p->next, j++){
		//if(j >= 2)continue;
		//char t1[STR_BUFFER], t2[STR_BUFFER], tp[STR_BUFFER];
		//char poly1_s[STR_BUFFER], z_s[STR_BUFFER];
		//, [STR_BUFFER], tp[STR_BUFFER];
		Polynomial * poly1, *poly_left, *poly_right;
		PolyPolynomial * ppleft, *ppright; 
		//math_list * tmp_solves = NULL;
		//math_list * tmp_eq_list = NULL;
		//math_list * tmp_ppeq_list = NULL;
		//int updated;
		m1 = p->data;
		if(m1->elements == NULL)continue;

		//Monomial_toString(t1, m1);
		//MDEBUG("Try $%s$, ", t1);
		poly1 = Monomial_copy_to_Polynomial(m1);
		//Polynomial_toString(poly1_s, poly1);
		//Polynomial_toString(z_s, z);
		//MDEBUG("Polynomial   :%s", poly1_s);
		//MDEBUG("Polynomial z :%s", z_s);

		//MDEBUG("pointer [%p] * [%p] = [%p]", poly1, z, poly_left);
		//MDEBUG("pointer [%p] * [%p] = [%p]", poly1->monomials, z->monomials, poly_left->monomials);
		//TEXDEBUG("\n\nRound %d, Calculating $%s z = z %s$, we have \n", j, poly1_s, poly1_s);
		TEXDEBUG("\n\nRound %d, Calculating $", j);
		Polynomial_Output(poly1, stdout);
		Polynomial_Output(poly1, tex_log);
		TEXDEBUG(" z = z ");
		Polynomial_Output(poly1, stdout);
		Polynomial_Output(poly1, tex_log);
		TEXDEBUG("$, \n");

		//TEXDEBUG("\n\n------%d,-Polynomial mul %d and %d\n\n", __LINE__, 
		//		math_list_length(poly1->monomials),
		//		math_list_length(z->monomials)
		//		);
		//fflush(stdout);
		
		poly_left = Polynomial_mul_Polynomial(poly1, z);
		//TEXDEBUG("\n\n------%d,-\n\n", __LINE__);
		////TEXDEBUG(FORMULAR_LEFT);
		////TEXDEBUG("\n");
		//poly1 * z
		////TEXDEBUG("&%s \\times (%s) \\\\\n", poly1_s, z_s);
		//TEXDEBUG("\n\n---%s : %d---: %d elements\n\n", __FILE__, __LINE__, math_list_length(poly_left->monomials));
		Polynomial_cal_sum(poly_left);
		ppleft = PolyPolynomial_from_Polynomial(poly_left, ALGEBRA_HECKE_E);
		HeckeCliff_calculate_polypoly(ppleft);
		//TEXDEBUG("\n\n---%s : %d---\n\n", __FILE__, __LINE__);
		//TEXDEBUG("\n\n---%s : %d---\n\n", __FILE__, __LINE__);
		//TEXDEBUG(FORMULAR_RIGHT);
		//TEXDEBUG("\n");
		//TEXDEBUG("XXX");
		//MDEBUG("Try to clear left: [%s]", Polynomial_toString(t1, poly_left));
		//TEXDEBUG("\n\n---%s : %d---\n\n", __FILE__, __LINE__);
		MDEBUGP();
		//PolyPolynomial_toString(tp, ppleft);
		//TEXDEBUG(FORMULAR_LEFT);
		////TEXDEBUG("& = ");
		////TEXDEBUG(tp);
		////TEXDEBUG(FORMULAR_RIGHT);
		MDEBUGP();

		//break;
		poly_right = Polynomial_mul_Polynomial(z, poly1);
		//TEXDEBUG("\n\n---%s : %d-- %d elements-\n\n", __FILE__, __LINE__, math_list_length(poly_right->monomials));
		////TEXDEBUG(FORMULAR_LEFT);
		////TEXDEBUG("\n");
		//poly1 * z
		////TEXDEBUG("&( %s ) \\times %s \\\\\n", z_s, poly1_s);
		Polynomial_cal_sum(poly_right);
		ppright = PolyPolynomial_from_Polynomial(poly_right, ALGEBRA_HECKE_E);
		HeckeCliff_calculate_polypoly(ppright);
		//MDEBUG("Polynomial = %s", Polynomial_toString(t3, poly_right));
		//TEXDEBUG(FORMULAR_RIGHT);
		//TEXDEBUG("\n");
		//MDEBUG("Try to clear right: [%s]", Polynomial_toString(t1, poly_right));
		//TEXDEBUG("\n\n---%s : %d---\n\n", __FILE__, __LINE__);
		//PolyPolynomial_toString(tp, ppright);
		//TEXDEBUG(FORMULAR_LEFT);
		////TEXDEBUG("& = ");
		////TEXDEBUG(tp);
		////TEXDEBUG(FORMULAR_RIGHT);
		//TEXDEBUG("Hence we have,");
		//fflush(stdout);
		math_list * eqs = Equations_from_PolyPolynomials(ppleft, ppright);


		//tex_output_equations(eqs);
		//TODO
		TEXDEBUG("We got %d equations .\n\n", math_list_length(eqs));
		for(p_sub = eqs; p_sub != NULL; p_sub = p_sub->next){
			Equation *eq = p_sub->data;
			Equation_Process(eq);
		}
	
		//TEXDEBUG("Simpling: ");
		//tex_output_equations(eqs);

		eq_list = eqs;
		//eq_list = math_list_cat(eq_list, eqs);
		//TEXDEBUG("Summarizing those equations, we have");
		//	TEXDEBUG("\n\n------%d,-\n\n", __LINE__);
		//simple_equations_solve(eq_list,  solves);
		//	TEXDEBUG("\n\n------%d,-\n\n", __LINE__);
		MDEBUGP();
		eq_list = simple_equations(eq_list);
		//	TEXDEBUG("\n\n------%d,-\n\n", __LINE__);
		//tex_output_equations(eq_list);
		//TEXDEBUG("\n\n------%d,-\n\n", __LINE__);

	


		//TEXDEBUG("\n\nTotally %d equations .\n\n", math_list_length(eq_list));
		//tex_output_equations(eq_list);
		//TEXDEBUG("\n\n------%d---\n\n", __LINE__);
		ppeq_list = PPEquations_from_Equations(eq_list);
		//TEXDEBUG("\n\n------%d---\n\n", __LINE__);
		//TEXDEBUG("Totally %d PP equations .\n\n", math_list_length(ppeq_list));
		//tex_output_ppequations(ppeq_list);
		//TEXDEBUG("\n\n------%d---\n\n", __LINE__);
		//math_list  * solves;
	       	PPEquations_solve(&solves, & ppeq_list);
		//TEXDEBUG("\n\n------%d---\n\n", __LINE__);
		//tex_output_ppequations(ppeq_list);
		//tex_output_equations(solves);

		resolve_self(solves);
		solves = simple_equations(solves);
		num_sol = math_list_length(solves);
		if(num_sol_last != num_sol){
			num_sol_last = num_sol;
			dup_count = 0;
		}else{
			dup_count ++;
		}
		TEXDEBUG("Totally %d solutions, dup count: %d.\n\n", math_list_length(solves), dup_count);
		tex_output_equations(solves);
		//TEXDEBUG("\n\n---%s : %d---\n\n", __FILE__, __LINE__);
	
		//tex_output_equations(eq_list);
		//TEXDEBUG("\n\n---%s : %d---\n\n", __FILE__, __LINE__);
		//eqlist_replace_list(eq_list, solves);
		//tex_output_equations(eq_list);
		//TEXDEBUG("\n\n---%s : %d---\n\n", __FILE__, __LINE__);
		//eq_list = simple_equations(eq_list);
		//tex_output_equations(eq_list);
		//TEXDEBUG("\n\n---%s : %d---\n\n", __FILE__, __LINE__);
		//solves;
		//eq_list;
	
		//print_z(z);
		//TEXDEBUG("\n\n---%s : %d---\n\n", __FILE__, __LINE__);
		Polynomial_replace_list(z, solves);
		//TEXDEBUG("\n\n---%s : %d---\n\n", __FILE__, __LINE__);
		print_z(z);
		//TEXDEBUG("\n\n---%s : %d---Round: %d \n\n", __FILE__, __LINE__, j);
		//if(j > 3)break;
		//break;
		//
		//
		//Polynomial_delete(poly_left);
		//Polynomial_delete(poly_right);
		//PolyPolynomial_delete(ppleft);
		//PolyPolynomial_delete(ppright);
		//math_list_delete_list(ppeq_list, PPEquation_delete);
		//math_list_delete_list(eq_list, Equation_delete);
		if(dup_count >= 30)break;
	}
	//TEXDEBUG("\n\nTotally %d PP equations .", math_list_length(ppeq_list));
	//TEXDEBUG("\n\n------%d---\n\n", __LINE__);
	tex_output_ppequations(ppeq_list);
	TEXDEBUG("Solved, Totally %d solutions.", math_list_length(solves));
	tex_output_equations(solves);
	print_z(z);
	PolyPolynomial * ppz = rearrage_z(z);
	print_ppz(ppz);
}
