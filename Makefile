myprog: main.c exec_with_pipes.c parser.c prompt.c process.c Comando_miprof.c
	gcc main.c exec_with_pipes.c parser.c prompt.c process.c Comando_miprof.c -o myprog
clean: 
	rm -f myprog
debugear:
	gcc -g main.c exec_with_pipes.c parser.c prompt.c process.c Comando_miprof.c -o test