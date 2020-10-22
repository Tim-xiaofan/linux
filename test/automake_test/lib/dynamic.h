/*dynamic.h*/
#ifndef _DYNAMIC_H
#define _DYNAMIC_H
#include <stdio.h>
struct rte_dynamic
{
	char libname[20];
};

void dynamic_test(const struct rte_dynamic *m_static);

#endif//_DYNAMIC_H
