#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include "vivek.h"


void gotpacket(u_char *args,const struct pcap_pkthdr *header,const u_char *packet)
{
	struct sniff_ethernet *ethernet;
	struct sniff_ip *ip;
	struct sniff_tcp *tcp;
	char *payload;
	int size_ip;
	int size_tcp;
	ethernet = (struct sniff_ethernet *)packet;
	ip = (struct sniff_ip *)(packet+SIZE_ETHERNET);
	size_ip=IP_HL(ip)*4;
	tcp = (struct sniff_tcp *)(packet+SIZE_ETHERNET+size_ip);
	size_tcp=TH_OFF(tcp) * 4;
	if(size_tcp < 20)
	{
		printf("Invalid TCP packet\n");
		return;
	}
	printtcppacket(tcp);
	payload = (packet+SIZE_ETHERNET+size_tcp+size_ip);
	printf("Payload: %s\n",payload);


}

int main(int argc,char **argv)
{
	char dev[]="wlan0";
	char errbuf[PCAP_ERRBUF_SIZE];
	struct bpf_program fp;
	char filter_exp[]="port 6";
	bpf_u_int32 mask;
	bpf_u_int32 net;
	struct pcap_pkthdr header;
	const u_char *packet;

	if(pcap_lookupnet(dev,&net,&mask,errbuf) == -1)
	{
		printf("%s\n",errbuf);
		exit(1);
	}
	if(dev == NULL)
	{
		printf("%s\n",errbuf);
		exit(1);
	}
	printf("Device: %s\n",dev);

	pcap_t *handle;
	handle=pcap_open_live(dev,BUFSIZ,1,1000,errbuf); //device,maximum bytes to be captures,promiscous mode on or off,timeout,errbuf
	if(handle == NULL)
	{
		printf("%s\n",errbuf);
		exit(1);
	}
	// if(pcap_compile(handle,&fp,filter_exp,0,net) == -1) //compile the filter: pcap_t, compiled filter string,expression,optimize,net
	// {
	// 	printf("Error in filter");
	// 	exit(1);
	// }
	// if(pcap_setfilter(handle,&fp) == -1) //attach filter
	// {
	// 	printf("Could not install filter\n");
	// 	exit(1);
	// }
	// packet = pcap_next(handle, &header);
	/* Print its length */
	// printf("Jacked a packet with length of [%d]\n", header.len);
	/* And close the session */
	pcap_loop(handle,-1,gotpacket,NULL);
	pcap_close(handle);
	return 0;
}