#include "extremite.h"
#include "iftun.h"

/* echo des messages recus (le tout via le descripteur f) */
void echo(int f, char* ipServ, char* port)
{
	ssize_t lu; 			/* nb d'octets reÃ§us */
	char msg[MAXLIGNE+1]; 	/* tampons pour les communications */
	char tampon[MAXLIGNE+1];
	int pid = getpid(); 	/* pid du processus */
	int compteur=0;


	do { 		/* Faire echo et logguer */
		lu = recv(f,tampon,MAXLIGNE,0);
		if (lu > 0 )
		{
			compteur++;
			tampon[lu] = '\0';
			/* log */
			fprintf(stderr,"[%s:%s](%i): %3i :%s",ipServ,port,pid,compteur,tampon);
			snprintf(msg,MAXLIGNE,"> %s",tampon);
			/* echo vers la sortie standard */
			fprintf(stdout,"%s", msg);
		} 
		else {
			break;
		}
	} 
	while ( 1 );

	/* le correspondant a quittÃ© */
	send(f,CIAO,strlen(CIAO),0);
	close(f);
	fprintf(stderr,"[%s:%s](%i): Terminé.\n",ipServ,port,pid);
}

/* Fait office de serveur écoutant perpétuellement des sockets entrantes puis les écrit dans tun0 local */ 
int ext_out (char* port, int fd){

	char buf[MAX]; 			/* tampons pour les communications */
	int s,n; 						/* descripteurs de socket */
	int len,on; 					/* utilitaires divers */
	struct addrinfo * resol; 		/* resolution */
	struct addrinfo indic = {AI_PASSIVE, PF_INET6, SOCK_STREAM, 0, 0, NULL, NULL, NULL};
	struct sockaddr_in6 client; 	/* adresse de socket du client */
	int err; 						/* code d'erreur */

	fprintf(stderr,"Ecoute sur le port %s\n",port);
	err = getaddrinfo(NULL,port,&indic,&resol);
	if (err < 0){
		fprintf(stderr,"Résolution: %s\n",gai_strerror(err));
		exit(2);
	}

	/* Creation de la socket, de type TCP / IP */
	if ((s = socket(resol->ai_family, resol->ai_socktype, resol->ai_protocol)) < 0) {
		perror("allocation de socket");
		exit(3);
	}
	fprintf(stderr,"le nÂ° de la socket est : %i\n",s);

	/* On rend le port reutilisable rapidement /!\ */
	on = 1;
	if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on,sizeof(on)) < 0) {
		perror("option socket");
		exit(4);
	}
	fprintf(stderr,"Option(s) OK!\n");

	/* Association de la socket s a l'adresse obtenue par resolution */
	if (bind(s, resol->ai_addr, sizeof(struct sockaddr_in6)) < 0) {
		perror("bind");
		exit(5);
	}
	freeaddrinfo(resol); 		/* /!\ Liberation memoire */
	fprintf(stderr,"bind!\n");

	/* la socket est prete a recevoir */
	if (listen(s,SOMAXCONN)<0) {
		perror("listen");
		exit(6);
	}

	fprintf(stderr,"listen!\n");
	printf("sortie listen\n");
	while(1) {
		
		/* attendre et gerer indefiniment les connexions entrantes */
		len = sizeof(struct sockaddr_in6);
		n = accept(s, (struct sockaddr *)&client, (socklen_t*)&len);
		if(n < 0 ) {
			perror("accept");
			exit(7);
		}
		/* Nom reseau du client */
		char hotec[NI_MAXHOST]; 
		char portc[NI_MAXSERV];
		err = getnameinfo((struct sockaddr*)&client, len, hotec, NI_MAXHOST, portc, NI_MAXSERV, 0);
		if (err < 0){
			fprintf(stderr,"résolution client (%i): %s\n", n, gai_strerror(err));
		}
		else{
			fprintf(stderr,"accept! (%i) ip=%s port=%s\n", n, hotec, portc);
		}

		/* Recopie perpétuellement les packets des sockets dans le tunel */
		while(1){
			tun_copy_inv(n, fd, buf);
		}
	}
	close(s);
	return 0;
}


/* Envoie des sockets au serveur depuis tun0 */
int ext_in(char* ipServ, char* port, int fdTun)
{
	
	char buf[MAX];
	char ip[INET6_ADDRSTRLEN]; 			/* adresse IPv6 en notation pointée */
	struct addrinfo *resol; 			/* struct pour la résolution de nom */
	int soc, k; 							/* descripteur de socket */

	/* Résolution de l'hôte */
	if (getaddrinfo(ipServ, port, NULL, &resol) < 0 ){
		perror("résolution adresse");
		exit(2);
	}

	/* On extrait l'addresse IP */
	inet_ntop(resol->ai_family, resol, ip, INET6_ADDRSTRLEN);

	/* Création de la socket, de type TCP / IP */
	/* On ne considere que la première adresse renvoyee par getaddrinfo */
	soc = socket(resol->ai_family, resol->ai_socktype, resol->ai_protocol);
	if (soc < 0) {
		perror("allocation de socket");
		exit(3);
	}
	fprintf(stderr,"le n° de la socket est : %i\n",soc);

	/* Connexion */
	fprintf(stderr,"Essai de connexion à %s (%s) sur le port %s \n\n", ipServ, ip, port);
	int c = connect(soc, resol->ai_addr, sizeof(struct sockaddr_in6));
	if (c < 0) {
		perror("connection");
		exit(4);
	}
	printf("Connecté\n");

	freeaddrinfo(resol); 		/* /!\ Libération mémoire */

	/* Recopie perpétuellement les packets du tunel dans la socket */
	while (1) {
		tun_copy(fdTun, soc, buf);
	}

	close(soc);

	return 0;

}
