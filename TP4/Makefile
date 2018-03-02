
EXEC = paragol seqgol

all: $(EXEC)

paragol:
	mpicc -o paragol gamelifePara.c

seqgol:
	gcc -o seqgol gamelifeSeq.c

clean:
	rm $(EXEC)
