#ifndef __ZH_LOG_H__
#define __ZH_LOG_H__ 1
#include <stdio.h>
#define STR_BUFFER 16*1024

#define FORMULAR_LEFT "\n\\begin{align*}"
#define FORMULAR_RIGHT "\\end{align*}\n"

#define TEX 0

#if TEX
#define LOG_DEBUG 0
#define LOG_TEX 1
#else
#define LOG_DEBUG 1
#define LOG_TEX 0
#endif

#define LOG_NOLOG 0

#define ZHDEBUG LOG_TEX

extern int log_count ;

#define Output(fmt, args...)   do{printf(fmt, ##args);}while(0)
#if LOG_DEBUG
	#define MDEBUGC(fmt, args...)  printf(fmt , ##args) 
	#define MDEBUG(fmt, args...)   do{  \
			printf("debug: %s- %s - %d:"fmt "\n",  \
			__FILE__, __FUNCTION__, __LINE__, ##args);  \
		log_count ++; \
		}while(0)
	#define MDEBUGP() MDEBUG("")
#else
		#define MDEBUGC(fmt, args...)  do{}while(0)
		#define MDEBUG(fmt, args...)   do{  }while(0)
		#define MDEBUGP() do{}while(0)
#endif

extern FILE * tex_log;
#if LOG_TEX
	#define TEXDEBUG(fmt, args...)   do{printf(fmt, ##args); \
		if(tex_log){fprintf(tex_log, fmt, ##args); fflush(tex_log);} \
	}while(0)
#else
	#define TEXDEBUG(fmt, args...)   do{}while(0)
#endif
//#endif

void check_tex_log();
#endif
