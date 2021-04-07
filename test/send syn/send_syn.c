#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <errno.h>

int main()
{
	int sockfd;

	if(sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP) < 0)
	{
		printf("sokcet functin failed : \n");
		exit (-1);
	}

	char packet[512];

	struct sockaddr_in remote; // remote address

	//struct iphdr *ip = (struct iphdr *) packet;
	struct tcphdr *tcp = (struct tcphdr *) packet;;
	
	int *myRwnd = 0, int opt_size;
	opt_size=sizeof(*myRwnd);
	if (getsockopt (sockfd, SOL_SOCKET, SO_RCVBUF, (void*)myRwnd, &opt_size) < 0) {
		fprintf(stderr, "getsockopt: SO_RCVBUF failed !\n");
		return -1
	}
	fprintf(stderr, "receive buffer size initially is : %d\n", *myRwnd);

	//#if defined (LINUX)
	//adl_setReceiveBufferSize(sfd, 10*0xFFFF);
	int ch = 10 * 0xFFFF;
    if (setsockopt (sfd, SOL_SOCKET, SO_RCVBUF, (void*)&ch, sizeof(ch)) < 0) {
        fprintf(stderr, "setsockopt: SO_RCVBUF failed !\n");
        return -1;
    }
    fprintf(stderr, "set receive buffer size to : %d bytes",ch);

	ch = IP_PMTUDISC_DO;
	if (setsockopt(sockfd, IPPROTO_IP, IP_MTU_DISCOVER, (char *) &ch, sizeof(ch)) < 0) {
		error_log(ERROR_FATAL, "setsockopt: IP_PMTU_DISCOVER failed !");
	}
	opt_size=sizeof(*myRwnd);
	if (getsockopt (sfd, SOL_SOCKET, SO_RCVBUF, (void*)myRwnd, &opt_size) < 0) {
		error_log(ERROR_FATAL, "getsockopt: SO_RCVBUF failed !");
		*myRwnd = -1;
	}
	event_logi(INTERNAL_EVENT_0, "receive buffer size finally is : %d", *myRwnd);
	//#endif


	/*
	   struct tcphdr *tcp = (struct tcphdr *) packet; // tcp header
	 */
	remote.sin_family = AF_INET; // family
	remote.sin_addr.s_addr = inet_addr("10.10.0.62"); // destination ip
	remote.sin_port = htons(atoi("1100")); // destination port

	memset(packet, 0, 512); // set packet to 0

	tcp->source = htons(atoi("1100")); // source port
	tcp->dest = htons(atoi("1100")); // destination port
	tcp->seq = htons(random()); // inital sequence number
	tcp->ack_seq = htons(0); // acknowledgement number
	tcp->ack = 0; // acknowledgement flag
	tcp->syn = 1; // synchronize flag
	tcp->rst = 0; // reset flag
	tcp->psh = 0; // push flag
	tcp->fin = 0; // finish flag
	tcp->urg = 0; // urgent flag
	tcp->check = 0; // tcp checksum
	tcp->doff = 5; // data offset

	
	int err;
	if((err = sendto(sockfd, packet, 512, 0, (struct sockaddr *)&remote, sizeof(struct sockaddr))) < 0)
	{ // send packet
		printf("Error: Can't send packet : %d %d %s !\n", err, errno, strerror(errno));
		return -1;
	}
}
