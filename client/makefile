#注释
# client.o  frame.h interface.h tool.h
# interface.o interface.h
# tool.o  tool.h
#vpath %.c src
#vpath %.h headers
CC = clang
client : client.o tool.o interface.o
	clang -g client.o tool.o interface.o -o client
client.o: frame.h interface.h tool.h debug.h
tool.o: tool.h debug.h
interface.o: interface.h debug.h 

.PHONY: clean  run 
clean:
	-rm -f *.o
run:
	./client -p 4507 -a 127.0.0.1
