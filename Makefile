all: clean cserver cclient

cserver: csrv.o
	gcc -g -o cserver csrv.o

cclient: ccli.o
	gcc -g -o cclient ccli.o

csrv.o:
	gcc -g -c -o csrv.o server.c

ccli.o:
	gcc -g -c -o ccli.o client.c

clean:
	rm -f *.o
	rm -f cserver cclient


