hw4_exe : optimal.o fifo.o lifo.o lru.o lfu.o sc.o esc.o main.o
	gcc -o hw4_exe optimal.o fifo.o lifo.o lru.o lfu.o sc.o esc.o main.o

optimal.o : optimal.c
	gcc -c -o optimal.o optimal.c
fifo.o : fifo.c
	gcc -c -o fifo.o fifo.c
lifo.o : lifo.c
	gcc -c -o lifo.o lifo.c
lru.o : lru.c
	gcc -c -o lru.o lru.c
lfu.o : lfu.c
	gcc -c -o lfu.o lfu.c
sc.o : sc.c
	gcc -c -o sc.o sc.c
esc.o : esc.c
	gcc -c -o esc.o esc.c
main.o : main.c
	gcc -c -o main.o main.c

clean :
	rm *.o hw4_exe
