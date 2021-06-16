/** 20210616 15:45, zyj ,GuangDong*/
// pcap_offline.cpp -- Getting Started: The format of a offline pcap application
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pcap.h>

static void packet_handle(const u_char * pkt_str, int len);

typedef void (*packet_handle_t)(const u_char *, int);

static void offline_loop(packet_handle_t ph, const char * filename);

int main(int argc, char *argv[])
{
	offline_loop(packet_handle, argv[1]);
	return(0);
}

static void offline_loop(packet_handle_t ph, const char * filename)
{
	pcap_t *handle;			/* Session handle */
	char errbuf[PCAP_ERRBUF_SIZE];	/* Error string */
	struct pcap_pkthdr pkthdr;

	handle = pcap_open_offline(filename, errbuf);
	if(!handle)
	{
		fprintf(stderr, "pcap_open_offline failed:%s\n", errbuf);
		exit(EXIT_FAILURE);
	}
	while (1){
		const u_char *pkt_str = pcap_next(handle, &pkthdr);
		if (!pkt_str){
			fprintf(stderr, "Pcap file parse over !\n");
			exit(EXIT_FAILURE);
		}
		ph(pkt_str, pkthdr.len);
	}
	/* And close the session */
	pcap_close(handle);
}

static void packet_handle(const u_char * pkt_str, int len)
{
	printf("length : %d\n", len);
}
