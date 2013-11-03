all:
	gcc Trabalho_ICC.c Functions.c -o invmat -lm -g  

clean:
	rm -f *~
	rm -f *.o
	rm -f invmat

