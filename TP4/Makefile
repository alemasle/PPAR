
EXEC = seqgol paragol

all: $(EXEC)

seqgol:
	gcc -o seqgol gamelifeSeq.c

paragol:
	mpicc -o paragol gamelifePara.c

clean:
	rm $(EXEC)
