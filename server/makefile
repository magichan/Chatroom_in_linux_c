SRC = ./src/*.c
SRH = ./headers/*.h
OBJ = server

$(OBJ): $(SRC) $(SRH) 
	clang -g $(SRC) -o $(OBJ) -lpthread  -I ./headers


.PHONY: clean  run 
clean:
	-rm -f *.o



