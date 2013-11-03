all: Trabalho_ICC.c
	gcc Trabalho_ICC.c -o invmat -lm -g
clean:
	rm invmat
