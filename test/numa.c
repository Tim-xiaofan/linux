#include <numa.h>
#include <stdio.h>

int main()
{
	if (numa_available() < 0) 
	{
		printf("Your system does not support NUMA API\n");
		return -1;
	}
	printf("There are %d node.\n", numa_max_node());
}
