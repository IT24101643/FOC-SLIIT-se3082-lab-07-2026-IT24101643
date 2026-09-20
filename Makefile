CC = mpicc
MPIRUN = mpirun --oversubscribe
NP = 4

PROGRAMS = \
	Exercise01/sum_bcast \
	Exercise02/sum_scatter \
	Exercise03/sum_gather \
	Exercise04/sum_reduce \
	Exercise05/sum_allreduce \
	Exercise06/sum_scan

all: $(PROGRAMS)

Exercise01/sum_bcast: Exercise01/sum_bcast.c
	$(CC) -o $@ $<

Exercise02/sum_scatter: Exercise02/sum_scatter.c
	$(CC) -o $@ $<

Exercise03/sum_gather: Exercise03/sum_gather.c
	$(CC) -o $@ $<

Exercise04/sum_reduce: Exercise04/sum_reduce.c
	$(CC) -o $@ $<

Exercise05/sum_allreduce: Exercise05/sum_allreduce.c
	$(CC) -o $@ $<

Exercise06/sum_scan: Exercise06/sum_scan.c
	$(CC) -o $@ $<

run: all
	$(MPIRUN) -np $(NP) ./Exercise01/sum_bcast
	$(MPIRUN) -np $(NP) ./Exercise02/sum_scatter
	$(MPIRUN) -np $(NP) ./Exercise03/sum_gather
	$(MPIRUN) -np $(NP) ./Exercise04/sum_reduce
	$(MPIRUN) -np $(NP) ./Exercise05/sum_allreduce
	$(MPIRUN) -np $(NP) ./Exercise06/sum_scan

clean:
	rm -f $(PROGRAMS)
