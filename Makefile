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
>gcc $(ARGS) -c sjf.c -o sjf.o
>gcc sjf.o -o sjf

clean:
>rm -f p0 p1 p2 p3 p4 rr sjf fcfs test

test: clean p
>gcc test.c -o test
>./test

runrr: clean rr
>./rr 50 p0 p1 p2 p3 p4

runfcfs: clean fcfs
>./fcfs p0 p1 p2 p3 p4

runsjf-user: clean sjf
>./sjf p0 10 p1 9 p2 8 p3 2 p4 4

runsjf-file: clean sjf
>./sjf p0 10 p2 3 p4 5 p1 2 -f
