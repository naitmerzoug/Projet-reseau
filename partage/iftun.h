#ifndef __IFTUN__
#define __IFTUN__

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define MAX 1500


int tun_alloc(char *dev);
void tun_copy (int src, int dest, char * buf);
void tun_copy_inv(int src, int dest, char* buf);


#endif
