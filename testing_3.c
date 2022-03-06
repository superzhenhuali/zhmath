#include <stddef.h>

#include "math_list.h"
#include "testing.h"
#include "logs.h"
#include "equation.h"

void print_formula(const char * s){
	TEXDEBUG("\n" FORMULAR_LEFT "\n");
	TEXDEBUG("%s", s);
	TEXDEBUG("\n" FORMULAR_RIGHT "\n");
}

int HeckeCliff_calculate_poly(Polynomial * poly){
	char str[STR_BUFFER];
	int updated;
	// SINGLE ONE
	//	TEXDEBUG("\n\n------%d,-\n\n", __LINE__);
	Polynomial_toString(str, poly);
	MDEBUG("String length %d", strlen(str));
	//TEXDEBUG("&= %s \\\\\n", str);
	do{
		
		//MDEBUGP();
		//MDEBUG("Polynomial = %s", Polynomial_toString(str, poly));
		updated = Polynomial_rearrange_round(poly);
		MDEBUG("Updated: %d", updated);
		//MDEBUG("Polynomial = %s", Polynomial_toString(str, poly));
		//TEXDEBUG("& \\mbox{sort Return updated: %d } \\\\\n", updated);
		if(updated){
			Polynomial_toString(str, poly);
			MDEBUG("String length %d", strlen(str));
			////TEXDEBUG("&= %s \\\\\n", str);
			//MDEBUG("Poly %s", str);

			continue;
		}
		//MDEBUGP();
		//TEXDEBUG("\n\n------%d,-\n\n", __LINE__);
		updated = HeckeCliff_Polynomial_sort(poly);
		MDEBUG("hecke poly sort return updated: %d ", updated);
		//TEXDEBUG("& \\mbox{hecke poly sort Return updated: %d } \\\\\n", updated);
		if(updated){
			Polynomial_toString(str, poly);
			MDEBUG("String length %d", strlen(str));
			////TEXDEBUG("&= %s \\\\\n", str);
			//MDEBUG("Poly %s", str);
			continue;
		}
		MDEBUGP();
		//TEXDEBUG("\n\n------%d,-\n\n", __LINE__);
		updated = HeckeCliff_Polynomial_calculate(poly);
		//TEXDEBUG("\n\n------%d,-\n\n", __LINE__);
		MDEBUG("calculate Return updated: %d ", updated);
		//MDEBUG("Poly %s", Polynomial_toString(str, poly));
		//TEXDEBUG("& \\mbox{hecke calculate Return updated: %d } \\\\\n", updated);
		MDEBUGP();
		if(updated){
			//MDEBUGP();
			Polynomial_toString(str, poly);
			MDEBUG("String length %d", strlen(str));
			//MDEBUGP();
			////TEXDEBUG("&= %s \\\\\n", str);
			//MDEBUG("Poly %s", Polynomial_toString(str, poly));
			continue;
		}

		//break;
	}while(updated);
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
				Polynomial * p = Polynomial_copy(a->coeff);
				MDEBUG("coefficient copy %p: [%s] to %p.", p, Polynomial_toString(tmp, p), eq->left);
				eq->left = p;
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
void tex_output_equations(math_list * eq_list){
	math_list * eq_node;
	TEXDEBUG(FORMULAR_LEFT);
	for(eq_node = eq_list;  eq_node!= NULL; eq_node = eq_node->next){
		Equation * e = eq_node->data;
		char eq_str[STR_BUFFER];
		Equation_toString(eq_str, e);
		TEXDEBUG("\t & \\qquad %s \\\\\r\n", eq_str);
	}
	TEXDEBUG(FORMULAR_RIGHT);
}
void tex_output_ppequations(math_list * ppeq_list){
	math_list * eq_node;
	TEXDEBUG(FORMULAR_LEFT);
	for(eq_node = ppeq_list;  eq_node!= NULL; eq_node = eq_node->next){
		PPEquation * e = eq_node->data;
		char eq_str[STR_BUFFER];
		PPEquation_toString(eq_str, e);
		TEXDEBUG("\t & \\qquad %s \\\\\r\n", eq_str);
	}
	TEXDEBUG(FORMULAR_RIGHT);
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
		int found = 0;
		for(p = poly->monomials; p != NULL; ){
			z_next = p->next;
			Monomial * smd = p->data;
			if(math_list_find_data(smd->elements, ele, (math_list_compare_func)AlgebraElement_compare_variable ) != NULL){
				char smd_str[128];
				Monomial_toString(smd_str, smd);
				//TEXDEBUG("Monomial [$%s$] found [$%s$]\n\n", smd_str, ele_str);
				found = 1;
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
math_list * simple_ppequations(math_list * ppeq_list){
	math_list * na, *nb;
	math_list * res, *p;
	res = NULL;
	char eq_s[STR_BUFFER];
	for(na = ppeq_list;  na!= NULL; na = na->next){
		MDEBUGP();
		PPEquation * a = na->data;
		if(a == NULL || a->left == NULL || a->left->polymonomials == NULL){
			continue;
		}
		math_list * pms = a->left->polymonomials;
		if(math_list_length(pms) == 2){
			PolyMonomial * pm1 = pms->data;
			PolyMonomial * pm2 = pms->next->data;
			PPEquation_toString(eq_s, a);
			if(Polynomial_compare(pm1->coeff, pm2->coeff) == 0){
				TEXDEBUG("Equation {$%s$} satisfies.\n\n", eq_s);
			}else{
				math_list * p1, *p2;
				p1 = pm1->coeff->monomials;
				p2 = pm2->coeff->monomials;
				Monomial* mo1 = Monomial_from_string("-1");
				Polynomial * newp = Polynomial_new();
				newp->monomials = math_list_node_from(mo1);

				if(math_list_length(p1) == math_list_length(p2)){
					TEXDEBUG("Equation {$%s$} Length same. ", eq_s);
					Polynomial_toString(eq_s, newp);
					TEXDEBUG("po: {$%s$}. ", eq_s);
					Polynomial * mulp = Polynomial_mul_Polynomial(newp, pm1->coeff);
					Polynomial_toString(eq_s, mulp);
					TEXDEBUG("Mul result: {$%s$} ", eq_s);
					char s2[128];
					Polynomial_toString(s2, pm2->coeff);

					if(Polynomial_compare(mulp, pm2->coeff) == 0){
					//if(strcmp(s2, eq_s) == 0){
						TEXDEBUG("Equation mul -1.\n\n");
					}
				}
			
			}


		}
	}
	return res;
}
math_list * replace_equations(math_list * *eq_list, math_list * replace){
	math_list * na, *nb;
	math_list * res, *p;
	res = NULL;
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
	math_list * na, *nb;

	for(na = eq_list;  na!= NULL; na = na->next){
		Equation *a = na->data;
		use_solve_zero(solves, a->left);
	}
	return;
}

void HeckeCliff_Cal_Z(int n){
	char tmp[STR_BUFFER];
	char form[STR_BUFFER+ 16];

	math_list * Tlist;
	math_list * p;
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
	i = 0;
	for(p = Tlist; p != NULL; p = p->next){
		mo = p->data;
		MDEBUG("list: %p, p:%p, mo:%p", Tlist, p, mo);
		MDEBUG("Got element %d: %s", i,Monomial_toString(tmp, mo));
		i++;
	}
	MDEBUG("count: %d, %d", n, math_list_length(Tlist));
	
	Polynomial * z = Polynomial_new();
	Monomial * m1, *m2;
	int xi;

	xi = 1;
	char s[16];
	int z_index[] = {43,20,21,44,7,8,37,36,9,4,35,6,5,11,38,10,45,22,40,26,23,24,47,46,42,19, -1};
	int zi, ztag;
	for(p = Tlist; p != NULL; p = p->next, xi++){
		AlgebraElement * ele;
		sprintf(s, "a_%d", xi);

		ztag = 0;
		for(zi=0; z_index[zi]!= -1; zi++){
			if(xi == z_index[zi])ztag = 1;
		}	
		if(ztag){
			continue;
		}

		//xi ++;
		//m1 = Monomial_from_string(s);
		ele = AlgebraElement_from_string(s);
		ele->type = VARIABLE_E2;



		switch(xi){
			case 15:
				ele->index = 3;
				break;
			case 18:
				ele->index = 12;
				break;
			case 28:
				ele->index = 12;
				break;
			case 14:
				ele->index = 12;
				break;
			case 16:
				ele->index = 12;
				break;
			case 34:
				ele->index = 12;
				break;
			case 32:
				ele->index = 12;
				break;
			case 27:
				ele->index = 3;
				break;
			case 33:
				ele->index = 3;
				break;
			case 29:
				ele->index = 3;
				break;
			case 41:
				ele->index = 39;
				break;
			default: break;
		
		}

		//MDEBUG("Got element [%s] from string [%s]", Monomial_toString(tmp, mo), s);
		m2 = p->data;
		//mo = Monomial_mul_Monomial(m1, m2);
		mo = AlgebraElement_mul_Monomial(ele, m2);
		AlgebraElement * _ele = mo->elements->data;
		switch(xi){
			case 31:
			case 13:
			case 17:
				_ele->index = 3;
				mo->coeff = mo->coeff *(-1);
				break;
			case 30:
				_ele->index = 12;
				mo->coeff = mo->coeff *(-1);
				break;
			default: break;
		}
		//MDEBUG("list: %p, p:%p, mo:%p", Tlist, p, mo);
		//MDEBUG("Got element %s", Monomial_toString(tmp, mo));
		z->monomials = math_list_append(z->monomials, node_from_Monomial(mo));
	}

	Polynomial_toString(tmp, z);
	MDEBUG("Center: %s", tmp);
	TEXDEBUG("Assume $z$ is in center, and ");
	//sprintf(form, "z &= %s, ", tmp);
	//print_formula(form);
	TEXDEBUG("\n" FORMULAR_LEFT "\n");
	TEXDEBUG("z &= %s", tmp);
	TEXDEBUG("\n" FORMULAR_RIGHT "\n");

	int j=0;
	math_list * solves = NULL;
	math_list * eq_list = NULL;
	math_list * ppeq_list = NULL;
	for(p = Tlist; p != NULL; p = p->next){
		//if(j >= 2)continue;
		j ++;
		char t1[STR_BUFFER], t2[STR_BUFFER], tp[STR_BUFFER];
		char poly1_s[STR_BUFFER], z_s[STR_BUFFER];
		//, [STR_BUFFER], tp[STR_BUFFER];
		Polynomial * poly1, *poly_left, *poly_right;
		PolyPolynomial * ppleft, *ppright; 
		//int updated;
		m1 = p->data;
		if(m1->elements == NULL)continue;

		//Monomial_toString(t1, m1);
		//MDEBUG("Try $%s$, ", t1);
		poly1 = Monomial_copy_to_Polynomial(m1);
		Polynomial_toString(poly1_s, poly1);
		Polynomial_toString(z_s, z);
		MDEBUG("Polynomial   :%s", poly1_s);
		MDEBUG("Polynomial z :%s", z_s);

		//MDEBUG("pointer [%p] * [%p] = [%p]", poly1, z, poly_left);
		//MDEBUG("pointer [%p] * [%p] = [%p]", poly1->monomials, z->monomials, poly_left->monomials);
		TEXDEBUG("Calculating $%s z = z %s$, we have \n", poly1_s, poly1_s);
		//TEXDEBUG("\n\n------%d,-\n\n", __LINE__);

		poly_left = Polynomial_mul_Polynomial(poly1, z);
		//TEXDEBUG("\n\n------%d,-\n\n", __LINE__);
		////TEXDEBUG(FORMULAR_LEFT);
		////TEXDEBUG("\n");
		//poly1 * z
		////TEXDEBUG("&%s \\times (%s) \\\\\n", poly1_s, z_s);
		HeckeCliff_calculate_poly(poly_left);
		//TEXDEBUG("\n\n------%d,-\n\n", __LINE__);
		//TEXDEBUG(FORMULAR_RIGHT);
		//TEXDEBUG("\n");
		//TEXDEBUG("XXX");
		//MDEBUG("Try to clear left: [%s]", Polynomial_toString(t1, poly_left));
		ppleft = PolyPolynomial_from_Polynomial(poly_left, ALGEBRA_HECKE_E);
		MDEBUGP();
		PolyPolynomial_toString(tp, ppleft);
		//TEXDEBUG(FORMULAR_LEFT);
		////TEXDEBUG("& = ");
		////TEXDEBUG(tp);
		////TEXDEBUG(FORMULAR_RIGHT);
		MDEBUGP();

		//break;
		poly_right = Polynomial_mul_Polynomial(z, poly1);
		////TEXDEBUG(FORMULAR_LEFT);
		////TEXDEBUG("\n");
		//poly1 * z
		////TEXDEBUG("&( %s ) \\times %s \\\\\n", z_s, poly1_s);
		HeckeCliff_calculate_poly(poly_right);
		//MDEBUG("Polynomial = %s", Polynomial_toString(t3, poly_right));
		//TEXDEBUG(FORMULAR_RIGHT);
		//TEXDEBUG("\n");
		//MDEBUG("Try to clear right: [%s]", Polynomial_toString(t1, poly_right));
		ppright = PolyPolynomial_from_Polynomial(poly_right, ALGEBRA_HECKE_E);
		PolyPolynomial_toString(tp, ppright);
		//TEXDEBUG(FORMULAR_LEFT);
		////TEXDEBUG("& = ");
		////TEXDEBUG(tp);
		////TEXDEBUG(FORMULAR_RIGHT);
		TEXDEBUG("Hence we have,");

		math_list * eqs = Equations_from_PolyPolynomials(ppleft, ppright);

		tex_output_equations(eqs);
		{
			math_list *p;
			for(p = eqs; p != NULL; p = p->next){
				Equation *eq = p->data;
				Equation_Process(eq);
			}
		
			TEXDEBUG("Simpling: ");
			tex_output_equations(eqs);
		}
	#if 0
		math_list * eq_node;
		TEXDEBUG(FORMULAR_LEFT);
		for(eq_node = eqs; eq_node!= NULL; eq_node = eq_node->next){
			Equation * e = eq_node->data;
			char eq_str[STR_BUFFER];
			Equation_toString(eq_str, e);
			TEXDEBUG("\t & \\qquad %s \\\\\r\n", eq_str);
		}
		TEXDEBUG(FORMULAR_RIGHT);
	#endif
		eq_list = math_list_cat(eq_list, eqs);

		//TEXDEBUG("Equation numbers: %d, total %d ", math_list_length(eqs), math_list_length(eq_list));
		//break;
		//ppright = PolyPolynomial_from_Polynomial(poly_right);
		//break;
#if 0
		// SINGLE ONE
		Polynomial_toString(t3, poly3);
		TEXDEBUG("&= %s \\\\\n", t3);
		do{
			
			//MDEBUGP();
			MDEBUG("Polynomial = %s", Polynomial_toString(t3, poly3));
			updated = Polynomial_rearrange_round(poly3);
			MDEBUG("Polynomial = %s", Polynomial_toString(t3, poly3));
			TEXDEBUG("& \\mbox{sort Return updated: %d } \\\\\n", updated);
			if(updated){
				Polynomial_toString(t3, poly3);
				TEXDEBUG("&= %s \\\\\n", t3);
				MDEBUG("Poly %s", t3);
				continue;
			}
			//MDEBUGP();
			updated = HeckeCliff_Polynomial_sort(poly3);
			MDEBUG("hecke poly sort return updated: %d ", updated);
			TEXDEBUG("& \\mbox{hecke poly sort Return updated: %d } \\\\\n", updated);
			if(updated){
				Polynomial_toString(t3, poly3);
				TEXDEBUG("&= %s \\\\\n", t3);
				MDEBUG("Poly %s", t3);
				continue;
			}
			updated = HeckeCliff_Polynomial_calculate(poly3);
			MDEBUG("calculate Return updated: %d ", updated);
			MDEBUG("Poly %s", Polynomial_toString(t3, poly3));
			TEXDEBUG("& \\mbox{hecke calculate Return updated: %d } \\\\\n", updated);
			if(updated){
				Polynomial_toString(t3, poly3);
				TEXDEBUG("&= %s \\\\\n", t3);
				MDEBUG("Poly %s", Polynomial_toString(t3, poly3));
				continue;
			}

			//break;
		}while(updated);
#endif
		//sprintf(form, "%s * (%s) = %s, ", t1, t2, t3);
		//print_formula(form);
		//MDEBUG("[%s] * [%s] = [%s]", t1, t2, t3);
		//break;
#if 1
		MDEBUGP();
		Matrix * mat = Equation_to_matrix(eq_list, VARIABLE_E2);
		//Matrix_Polynomial_to_Tex(mat);
		int updated = 0;

		do{
			updated = Equation_solve_matrix(mat);
			if(updated){
				//Matrix_Polynomial_to_Tex(mat);
			}
		}while(updated > 0);
		//TEXDEBUG("Solved : %d .", math_list_length(mat->extra2));
		//tex_output_equations(mat->extra2);
	
#if 0
		solves = math_list_cat(solves, mat->extra2);
		//TEXDEBUG("\n\n------%d,-\n\n", __LINE__);
		solves = simple_equations(solves);
		//TEXDEBUG("\n\n------%d,-\n\n", __LINE__);
		TEXDEBUG("Got solved part: \n\n");
		tex_output_equations(solves);
		TEXDEBUG("Done solved part: \n\n");
		use_solve_zero(solves, z);
#endif
#if 0
		if(solves != NULL){
			math_list * sp;
			for(sp = solves; sp != NULL; sp = sp->next){
				Equation * e = sp->data;
				Monomial * mm = e->left->monomials->data;
				AlgebraElement * ele = mm->elements->data;
				char ele_str[16];
				AlgebraElement_toString(ele_str, ele);
				TEXDEBUG("Got zero element [$%s$]\n\n", ele_str);
				math_list * sp2, *z_next;
				int found = 0;
				for(sp2 = z->monomials; sp2 != NULL; ){
					z_next = sp2->next;
					Monomial * smd = sp2->data;
					if(math_list_find_data(smd->elements, ele, (math_list_compare_func)AlgebraElement_compare_variable ) != NULL){
						char smd_str[128];
						Monomial_toString(smd_str, smd);
						TEXDEBUG("Monomial [$%s$] found [$%s$]\n\n", smd_str, ele_str);
						found = 1;
						Monomial_delete(smd);
						math_list_delete_node(sp2);
					}
					sp2 = z_next;
				}
			}
		}
#endif
		{
			char zs[65536];
			Polynomial_toString(zs, z);
			TEXDEBUG("Changed: \n\n", zs);
			TEXDEBUG(FORMULAR_LEFT);
			TEXDEBUG("z &=%s ", zs);
			TEXDEBUG(FORMULAR_RIGHT);
		
		}

#endif
	}

	TEXDEBUG("Summarizing those equations, we have");
	//	TEXDEBUG("\n\n------%d,-\n\n", __LINE__);
	//simple_equations_solve(eq_list,  solves);
	//	TEXDEBUG("\n\n------%d,-\n\n", __LINE__);
	eq_list = simple_equations(eq_list);
	//	TEXDEBUG("\n\n------%d,-\n\n", __LINE__);
	tex_output_equations(eq_list);

#if 0
	{
		math_list *p;
		for(p = eq_list; p != NULL; p = p->next){
			Equation *eq = p->data;
			Equation_Process(eq);
		}
		
		TEXDEBUG("Trying, ");
		tex_output_equations(eq_list);
	}
#endif
#if 0
	{
		TEXDEBUG("Totally %d equations. ", math_list_length(eq_list));
		eq_list = simple_equations(eq_list);
		TEXDEBUG("Trying again, ");
		tex_output_equations(eq_list);
		TEXDEBUG("Totally %d equations .", math_list_length(eq_list));
		//TEXDEBUG("\n\n------%d---\n\n", __LINE__);
		ppeq_list = PPEquations_from_Equations(eq_list);
		//TEXDEBUG("\n\n------%d---\n\n", __LINE__);
		TEXDEBUG("Totally %d PP equations .", math_list_length(ppeq_list));
		tex_output_ppequations(ppeq_list);
		TEXDEBUG("\n\n------%d---\n\n", __LINE__);
		math_list  * solves = PPEquations_solve(& ppeq_list);
		TEXDEBUG("Solved, Totally %d PP equations .", math_list_length(ppeq_list));
		TEXDEBUG("\n\n------%d---\n\n", __LINE__);
		tex_output_ppequations(ppeq_list);
		TEXDEBUG("Solved, Totally %d PP solutions.", math_list_length(solves));
		tex_output_ppequations(solves);
		TEXDEBUG("\n\n------%d---\n\n", __LINE__);
	}
#endif
#if 1
	
		MDEBUGP();
		Matrix * mat = Equation_to_matrix(eq_list, VARIABLE_E2);
		//Matrix_Polynomial_to_Tex(mat);
		int updated = 0;

		do{
			updated = Equation_solve_matrix(mat);
			if(updated){
				//Matrix_Polynomial_to_Tex(mat);
			}
		}while(updated > 0);
		//tex_output_equations(mat->extra2);
		//tex_output_equations(solves);
		Matrix_Polynomial_to_Tex(mat);
		tex_output_equations(eq_list);
	
	
#endif
#if 1
	
		//solves;
		//eq_list;
		ppeq_list = PPEquations_from_Equations(eq_list);
		//simple_ppequations( ppeq_list);
		//TEXDEBUG("\n\n------%d---\n\n", __LINE__);
		TEXDEBUG("Totally %d PP equations .", math_list_length(ppeq_list));
		tex_output_ppequations(ppeq_list);
		{
			char zs[65536];
			Polynomial_toString(zs, z);
			TEXDEBUG("Center: \n\n", zs);
			TEXDEBUG(FORMULAR_LEFT);
			TEXDEBUG("z &=%s ", zs);
			TEXDEBUG(FORMULAR_RIGHT);
		
		}
	
	
#endif
#if 0
	math_list * eq_node;
	TEXDEBUG(FORMULAR_LEFT);
	for(eq_node = eq_list;  eq_node!= NULL; eq_node = eq_node->next){
		Equation * e = eq_node->data;
		char eq_str[STR_BUFFER];
		Equation_toString(eq_str, e);
		TEXDEBUG("\t & \\qquad %s \\\\\r\n", eq_str);
	}
	TEXDEBUG(FORMULAR_RIGHT);
#endif
}
