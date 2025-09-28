myprog: main.c pipes.c parser.c prompt.c process.c miprof.c redirections.c struct.c\
        prompt.h parser.h pipes.h process.h miprof.h config.h redirections.h struct.h
	gcc main.c pipes.c parser.c prompt.c process.c miprof.c redirections.c struct.c -o myprog

clean:
	rm -f myprog
