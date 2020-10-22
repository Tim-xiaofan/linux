/*static.h*/
#ifndef _STATIC_H
#define _STATIC_H
#include <stdio.h>
struct rte_static
{
	char libname[20];
};

void static_test(const struct rte_static* m_static);

#endif//_STATIC_H
