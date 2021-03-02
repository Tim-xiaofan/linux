#include <stdio.h>
#include <stdint.h>

struct ether_addr
{
	uint8_t addr_bytes[6];
};

const char *fmt = "%02x:%02x:%02x:%02x:%02x:%02x";

int main()
{
	struct ether_addr mac1 = {{0x1, 0x2, 0x3, 0x4, 0x5, 0x6}};
	struct ether_addr mac2 = mac1;
	printf(fmt, mac2.addr_bytes[0],
				mac2.addr_bytes[1],
				mac2.addr_bytes[2],
				mac2.addr_bytes[3],
				mac2.addr_bytes[4],
				mac2.addr_bytes[5]);
}
