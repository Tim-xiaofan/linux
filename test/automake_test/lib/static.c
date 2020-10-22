/*static.c*/
#include "static.h"
void static_test(const struct rte_static* m_static)
{
	printf("libname = %s\n", m_static->libname);
}
