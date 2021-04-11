.RECIPEPREFIX = >
ARGS=-ggdb
p:
>for number in 0 1 2 3 4 ; do \
	gcc $(ARGS) p.c -o p$$number ; \
 done

rr: p
>gcc $(ARGS) -c rr.c -o rr.o
>gcc rr.o -o rr

fcfs: p
>gcc $(ARGS) -c fcfs.c -o fcfs.o
>gcc fcfs.o -o fcfs

sjf: p
>gcc -c sjf.c -o sjf.o
>gcc sfc.o -o sjf

clean:
>rm -f p0 p1 p2 p3 p4 rr sjf fcfs test

test: clean p
>gcc test.c -o test
>./test

runrr: clean rr
>./rr 50 p0 p1 p2 p3 p4
runfcfs: clean fcfs
>./fcfs p0 p1 p2 p3 p4
