#include "logs.h"

int log_count = 0;
FILE * tex_log = NULL;
char * tex_file_name;

void check_tex_log(){
	if(tex_file_name == NULL){
		tex_file_name = "./log.tex";
	}
	if(tex_log == NULL){
		tex_log = fopen(tex_file_name, "w");
	}
}
