CC=g++
CCFLAGS=-Wall -Werror -g3 -pedantic -std=c++14 -Wno-sign-compare
LD=g++
LDFLAGS=-pthread

slows: slows.o server.o request.o response.o
	$(LD) $(LDFLAGS) -o slows slows.o server.o request.o response.o
slows.o: slows.cpp server.h server.cpp request.h request.cpp response.h response.cpp
	$(CC) $(CCFLAGS) -o slows.o -c slows.cpp
server.o: server.cpp server.h
	$(CC) $(CCFLAGS) -o server.o -c server.cpp
request.o: request.cpp request.h
	$(CC) $(CCFLAGS) -o request.o -c request.cpp
response.o: response.cpp response.h
	$(CC) $(CCFLAGS) -o response.o -c response.cpp
clean:
	rm -f *.o slows
