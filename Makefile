.RECIPEPREFIX = >
ARGS=-ggdb
p:
>echo "making p.o..."
>for number in 0 1 2 3 4 ; do \
	gcc $(ARGS) p.c -o p$$number ; \
 done

rr: p
>gcc $(ARGS) -c rr.c -o rr.o
>gcc rr.o -o rr

fcfs: p
>gcc -c fcfs.c -o fcfs.o
>gcc fcfs.o p.o -o fcfs

sjf: p
>gcc -c sjf.c -o sjf.o
>gcc sfc.o p.o -o sjf

clean:
>rm -f p0 p1 p2 p3 p4 rr sjf fcfs test

test: clean p
>gcc test.c -o test
>./test

run: rr
>./rr 10 p0 p1
