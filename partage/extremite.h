#ifndef __EXTREMITE__
#define __EXTREMITE__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>

#include <linux/ioctl.h>
#include <linux/if.h>
#include <linux/if_tun.h>

#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>

#define MAXLIGNE 80
#define CIAO "Au revoir ...\n"


int ext_out (char* port, int fd);
int ext_in(char* hote, char* port, int fdtun);
void echo(int f, char* hote, char* port);


#endif
