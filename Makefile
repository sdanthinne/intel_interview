.RECIPEPREFIX = >
p.o: p.c
>echo "making p.o..."
>cc -c p.c -o $@

rr: p.o`
>cc -c rr.c -o rr.o
>cc rr.o p.o -o rr

fcfs: p.o
>cc -c fcfs.c -o fcfs.o
>cc fcfs.o p.o -o fcfs

sjf: p.o
>cc -c sjf.c -o sjf.o
>cc sfc.o p.o -o sjf
