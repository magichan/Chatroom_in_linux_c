CC = clang
server : server.o user.o tool.o 
	clang  -g server.o user.o tool.o -lpthread -o server
server.o: tool.h frame.h user.h server.h debug.h
tool.o: tool.h frame.h user.h server.h debug.h
user.o: tool.h frame.h user.h server.h debug.h

