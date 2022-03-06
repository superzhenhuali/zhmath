#ifndef __SYMMETRIC_GROUP_H__
#define __SYMMETRIC_GROUP_H__
#include <stdint.h>

typedef struct {
	int size;
	int *perm;	
}symmetric_group;

sym_grp * sym_grp_init(int size);

#endif

