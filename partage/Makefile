#Projet Reseaux

CC = gcc
CFLAGS = -Wall -W -std=c99 -pedantic
OBJ = iftun.o extremite.o


all: tunnel46d test_iftun_serveur test_iftun2

extremite.o : extremite.h extremite.c 
	$(CC) $(CCFLAGS) -c extremite.c 

iftun.o : iftun.h iftun.c 
	$(CC) $(CCFLAGS) -c iftun.c 

tunnel46d :  tunnel46d.c $(OBJ)
	$(CC) $(CCFLAGS) -o tunnel46d tunnel46d.c $(OBJ)

test_iftun2 :  test_iftun2.c $(OBJ)
	$(CC) $(CCFLAGS) -o test_iftun2 test_iftun2.c $(OBJ)

test_iftun_serveur : test_iftun_serveur.c $(OBJ)
	$(CC) $(CCFLAGS) -o test_iftun_serveur test_iftun_serveur.c $(OBJ)


clean:
	rm -f *.o core

