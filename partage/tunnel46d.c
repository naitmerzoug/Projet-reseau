#include "iftun.h"
#include "extremite.h"


Pour lancer les tunnels:

./tunnel46d `cat VM1-6.txt` sur VM1-6
./tunnel46d `cat VM3-6.txt` sur VM3-6
*/

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
		
	int in, out;
	
	int f = fork();
	if(f < 0){
		perror("Fork\n");
		exit(1);
	}
	else if(f == 0){
		sleep(5);
		in = ext_in(ipServ, port, fdTun);
	}
	else
		out = ext_out(port, fdTun);
	return 0;
}
