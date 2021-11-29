#include "iftun.h"
#include "extremite.h"

#define MAX 1500
int main (int argc, char **argv){

	if(argc != 6){
		printf("Usage: %s port ipServeur ipTun ipTun_Sortie LAN_Sortie\n", argv[0]);
		printf("ou\n");
		printf("Usage: %s `cat NOM_VM.txt`\n", argv[0]);
		exit(1);
	}

	char *port = argv[1];
	char *ipServ = argv[2];
	char *ipTun = argv[3];
	char *ipTunSortie = argv[4];
	char *lan = argv[5];
	
	char command[100];
	char dev2[MAX];
	char buf[MAX];
	int fdTun;
	strcpy(dev2,"tun%d");
	fdTun = tun_alloc(dev2);
	
	sprintf(command, "./configure-tun.sh %s %s %s %s", ipTun, lan,ipTunSortie, dev2);
	
	system(command);
	
	int in;
	
	
	in = ext_in(ipServ, port, fdTun);

	return 0;
}
