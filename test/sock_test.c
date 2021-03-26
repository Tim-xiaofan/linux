#include <sys/socket.h>

#include <stdio.h>

int main(int argc, char* argv[])
{
    int sock1 = socket(AF_INET, SOCK_DGRAM, 0);
    int sock2 = socket(AF_INET, SOCK_DGRAM, 0);
    printf("sock1 = %d, sock2 = %d\n", sock1, sock2);
}
