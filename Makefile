myprog: main.c crear_archivo.c exec_with_pipes.c parser.c prompt.c process.c 
	gcc main.c crear_archivo.c exec_with_pipes.c parser.c prompt.c process.c  -o myprog
clean: 
	rm -f myprog
debugear:
	gcc -g main.c crear_archivo.c exec_with_pipes.c parser.c prompt.c process.c -o test