#include "iftun.h"
#include "extremite.h"

/* Alloue un tunel et retourne son descripteur de fichier */
int tun_alloc(char *dev)
{
    struct ifreq ifr;
    int fd, err; 

    if( (fd = open("/dev/net/tun", O_RDWR)) < 0 )
    {
		perror("création tun");
        exit(1);
	}
  
	memset(&ifr, 0, sizeof(ifr));
  
        /* Flags: IFF_TUN   - TUN device (no Ethernet headers) 
         *        IFF_TAP   - TAP device  
         *        IFF_NO_PI - Do not provide packet information  
         */
         
	ifr.ifr_flags = IFF_TUN | IFF_NO_PI; 

	if( *dev )
		strncpy(ifr.ifr_name, dev, IFNAMSIZ);

	if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ){
	   close(fd);
	   return err;
	}
	strcpy(dev, ifr.ifr_name);
	return fd;
}


/* Recopie perpétuellement toutes les données lisibles sur src dans le fichier décrit par dst*/
void tun_copy (int src, int dest, char* buf){
	
	int r = read(src, buf, MAX);
	
	if(r < 0){
		perror("Read");

	}
	
	if(write (dest, buf, r) < 0){
		perror("Write");

	}
}

void tun_copy_inv(int src, int dest, char* buf){
	
	int r = recv(src, buf, MAX, 0);
	
	if(r < 0){
		perror("Read");

	}
	
	if(write (dest, buf, r) < 0){
		perror("Write");

	}
}

