#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

struct ether_addr
{
	uint8_t addr_bytes[6];
};

#define FMT "%02x:%02x:%02x:%02x:%02x:%02x\n"
#define VALUE {0x1, 0x2, 0x3, 0x4, 0x5, 0x6}

void list_bytes(uint8_t bytes[])
{
	printf(FMT, bytes[0],
				bytes[1],
				bytes[2],
				bytes[3],
				bytes[4],
				bytes[5]);
}

int main()
{
	struct ether_addr *mac1 = 
		(struct ether_addr* ) malloc(sizeof(struct ether_addr));
	mac1->addr_bytes[0] = 0x1;
	mac1->addr_bytes[1] = 0x2;
	mac1->addr_bytes[2] = 0x3;
	mac1->addr_bytes[3] = 0x4;
	mac1->addr_bytes[4] = 0x5;
	mac1->addr_bytes[5] = 0x6;
	struct ether_addr mac2;
	mac2 = *mac1;
	printf("%p, %p\n", mac1->addr_bytes, mac2.addr_bytes);
	list_bytes(mac2.addr_bytes);
	free(mac1);
	list_bytes(mac2.addr_bytes);
	uint8_t bytes[6]= VALUE;
	uint8_t bytes1[6];
	//bytes1 = bytes; /** invalid array assign*/
	printf("%p, %p\n", bytes, bytes1);
	list_bytes(bytes1);
}
