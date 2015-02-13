
SRC = ./src/*.c
SRH = ./headers/*.h
OBJ = client

$(OBJ): $(SRC) $(SRH) 
	clang  -g $(SRC) -o $(OBJ) -lpthread  -I ./headers



.PHONY: clean  run 
clean:
	-rm -f *.o
run:
	./client -p 4507 -a 127.0.0.1
