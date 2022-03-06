#include <stdio.h>
#include <string.h>

#include "zhmath.h"
#include "bigint.h"

#define IS_DEC(x) (((x)<='9') && ((x)>='0'))
#define IS_HEX_A_L(x) (((x)<='f') && ((x)>='a'))
#define IS_HEX_A_U(x) (((x)<='F') && ((x)>='A'))
#define IS_HEX_A(x) (IS_HEX_A_L(x) || IS_HEX_A_U(x))
#define IS_HEX(x) (IS_DEC(x) || IS_HEX_A(x))
#define HEX_V_TO_C(x) ((x) >=10 )?('a' + (x)-10):('0' + (x))


char inline static hex_value(char x){
	if(IS_DEC(x))
		return x - '0';

	if(IS_HEX_A_L(x))
		return x - 'a' + 10;

	if(IS_HEX_A_U(x))
		return x-'A' + 10;

	return 0;
}

int bigint_dump(bigint * bi){
	char vs[128];

	if(!bi) return -1;

	bigint_to_str(vs, 128, bi);
	printf("Size : %d \n", bi->nsize);
	printf("Value: %s \n", vs);
	
	return 0;
}

int bigint_from_str(bigint *bi, const char * str){
	const char * p;
	if(!bi) return -1;
	if(!str) return -1;

	//l = strlen(str);
	MDEBUGP();
	memset(bi, 0, sizeof(bigint));

	MDEBUG("%s", str);
	if( str[0]== '-'){
		MDEBUG("Is minus;");
		bi->minus = 1;
		str++;
	}
	
	if( str[0]== 0){
		bi->value[0] = 0;
		bi->nsize = 0;
	}
	if((str[0] == '0') && ( (str[1] == 'x')||(str[1] == 'X') )){
		str += 2;
		for(p = str; *p != 0; p++){
			if(!IS_HEX(*p)) 
				return -1;
		}
		bi->ntype = TYPE_HEX;
		MDEBUGP();
		return bigint_from_hex(bi, str);
	}else{
		for(p = str; *p != 0; p++){
			if(!IS_DEC(*p)) 
				return -1;
		}
		bi->ntype = TYPE_DEC;
		MDEBUGP();
		return bigint_from_dec(bi, str);
	}
	return 0;
}

int bigint_from_hex(bigint * bi, const char * str){
	MDEBUGP();
	return bigint_from_type(bi, str, TYPE_HEX);
}
int bigint_from_dec( bigint * bi, const char * str){
	MDEBUGP();
	return bigint_from_type(bi, str, TYPE_DEC);
}

int bigint_from_type(bigint * bi, const char * str, repr_type t){
	const char *p, *s;
	uint8_t *value, lastv;
	
	MDEBUG("str:%s , type:%d ", str, t);
	if( (t != TYPE_HEX) && (t != TYPE_DEC))
		return -1;

	for(p=str; *p; p++);
	p--;

	if((p-str)%2 == 1){
		// ab cd 01
		s = str + 1;
		lastv = 0;
	}else{
		// a bc
		s = str + 2;
		lastv = hex_value(str[0]);
	}

	MDEBUG("lastv: %x", lastv);
	value = bi->value;
	for(; p >= s; p-=2){
		switch(t){
			case TYPE_HEX:{
				*value = hex_value(*p) | ( hex_value(*(p - 1)) << 4 );
				MDEBUG("hex value in hex: %x", *value);
				break;
			}
			case TYPE_DEC:{
				*value = hex_value(*p) + hex_value(*(p - 1)) * 10 ;
				MDEBUG("dec value in hex: %x", *value);
				break;
			}
		}
		value ++;
	}

	if(lastv){
		*value = lastv;
		value ++;
	}

	bi->nsize = value - bi->value;
	return bi->nsize;
}


int bigint_to_str(char * str, int size, bigint * bi){
	char *pv;
	uint8_t * value;

	if(!bi) return -1;
	if(!str) return -1;
	if(size < bi->nsize * 2 + 4 ) return -1;
	
	MDEBUG("size: %d", bi->nsize);
	pv = str;
	if(bi->minus){
		pv[0] = '-';
		pv ++;
	}
	if(bi->ntype == TYPE_HEX){
		pv[0] = '0';
		pv[1] = 'x';
		pv += 2;
	}
	
	if(bi->ntype == TYPE_HEX){
		MDEBUG("Hex");
		for(value = bi->value + bi->nsize - 1; value >= bi->value; value--){
			MDEBUG("buffer: 0x%x", *value);
			*pv = HEX_V_TO_C((*value)>>4);
			pv ++;
			*pv = HEX_V_TO_C((*value)&0xf);
			pv ++;
		}
	}else if(bi->ntype == TYPE_DEC){
		MDEBUG("Dec");
		for(value = bi->value + bi->nsize - 1; value >= bi->value; value--){
			//MDEBUG("buffer: %d", *value);
			*pv = HEX_V_TO_C((*value)/10);
			pv ++;
			*pv = HEX_V_TO_C((*value)%10);
			pv ++;
		}
	}else{
		MDEBUG("Unknown type.");
	}
	*pv = 0;
	return 0;
}

//Ignore minus, only compare their absolutely value.
int bigint_cmp_norm(bigint * a, bigint * b){
	int i;
	uint8_t * pa, *pb;

	if(a->nsize > b->nsize){
		return 1;
	}
	if(b->nsize > a->nsize){
		return -1;
	}
	
	for(i = a->nsize -1, pa = a->value, pb = b->value; 
			i>=0; i--, pa++, pb++){
		if(*pa > *pb )return 1;
		if(*pa < *pb )return -1;
	}
	return 0;
}
int bigint_add_norm(bigint * bres, bigint * a, bigint * b){
	uint8_t *vbig, *vsmall, *vr, next;
	bigint *big, *small;
	uint16_t tv;
	int i;
	
	MDEBUGP();
	if(!bres || !a || !b ) return -1;
	memset(bres, 0, sizeof(bigint));
	if(a->ntype != b->ntype) {
		MDEBUG("Invalid type");
		return -1;
	}

	MDEBUG("Now beginning add");

	MDEBUGP();
	memset(bres,0, sizeof(bigint));
	
	bres->ntype = a->ntype;
	if(a->nsize > b->nsize){
		big = a;
		small = b;
	}else{
		big = b;
		small = a;
	}
	
	vbig = big->value;
	vsmall = small->value;
	vr = bres->value;

	bres->nsize = big->nsize;

	next = 0;
	for(i=1; i <= small->nsize; i++, vbig++, vsmall++, vr++){
		tv = *vbig + *vsmall + next;
		*vr = tv &0xff;
		next = tv >>8;
	}

	for(; i <= big->nsize; i++, vbig++, vr++){
		tv = *vbig + next;
		*vr = tv &0xff;
		next = tv >>8;
	}
	if(next){
		*vr = next;
		bres->nsize++;
	}

	return 0;
}

int bigint_add(bigint * bres, bigint * a, bigint * b){
	if(a->ntype == b->ntype){
		bres->ntype = a->ntype;
		return bigint_add_norm(bres, a, b);
	}

	return 0;
}

int bigint_sub_norm(bigint * bres, bigint * a, bigint * b){
	if(!bres || !a || !b ) return -1;
	if(a->ntype != b->ntype) return -1;

	memcpy(bres,a, sizeof(bigint));

	

	return 0;
}

int bigint_sub(bigint * bres, bigint * a, bigint * b){
	return 0;

}
int bigint_mul(bigint * bres, bigint * a, bigint * b){
	if(!bres || !a || !b ) return -1;
	if(a->ntype != b->ntype) return -1;

	memset(bres,0, sizeof(bigint));

	return 0;
}

int bigint_div(bigint * bres, bigint * a, bigint * b){
	if(!bres || !a || !b ) return -1;
	if(a->ntype != b->ntype) return -1;

	memset(bres,0, sizeof(bigint));

	return 0;
}

