
EXEC = seqgol paragol

all: $(EXEC)

seqgol:
	gcc -o seqgol gamelifeSeq.c

paragol:
	mpicc -o paragol gamelifePara.c
	# Run with mpirun -np n ./paragol

clean:
	rm $(EXEC)
