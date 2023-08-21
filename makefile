CC = clang++ -std=gnu++17


all:	genAudio

genAudio:	main.o genAudio.o
	${CC} main.o genAudio.o -o genAudio

main.o:	genAudio.h main.cpp
	${CC} -c main.cpp

genAudio.o:	genAudio.h genAudio.cpp
	${CC} -c genAudio.cpp

clean:
	rm *.o genAudio

