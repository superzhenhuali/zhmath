#include <stdlib.h>
#include <string.h>

#include "utils.h"

void * malloc_zero(uint32_t size){
	void * res = malloc(size);
	if(res){
		memset(res, 0, size);
	}
	return res;
}

void string_right_trip(char * str){
	if(str == NULL )return ;
	int i;
	i = strlen(str) - 1;
	for(; i>0 ; i--){
		int tag = 0;
		switch(str[i]){
			case '\r':
			case '\n':
			case ' ':
			case '\\':
				str[i] = 0;
				break;
			default:
				tag = 1;
				break;
		}
		if(tag)break;
	}
	
}
void string_right_trip_middle(char * str){
	if(str == NULL )return ;
	int i;
	string_right_trip(str);
	i = strlen(str) - 1;
	for(i = 0; i< strlen(str); i++){
		switch(str[i]){
			case '&':
			case '\r':
			case '\n':
				str[i] = ' ';
				break;
			default:
				break;
		}
		if(str[i] == '\\' && str[i+1] == '\\'){
			str[i] = ' ';
			str[i+1] = ' ';
		}
		if(strstr(str, "\\qquad") == str+i){
			int j=0;
			for(j=0; j<6; j++){
				str[i+j] = ' ';
			}
		}
	}
	
}
