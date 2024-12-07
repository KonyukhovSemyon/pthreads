all: prog

prog: main.o matrix.o gauss.o
	g++ main.o matrix.o gauss.o -o prog -pthread



main.o: main.cpp matrix.hpp gauss.hpp
	g++ -g -Wall -Wextra -Werror -pedantic -std=gnu++0x -O3 -c main.cpp

matrix.o: matrix.cpp matrix.hpp
	g++ -g -Wall -Wextra -Werror -pedantic -std=gnu++0x -O3 -c matrix.cpp

gauss.o: gauss.cpp gauss.hpp
	g++ -g -Wall -Wextra -Werror -pedantic -std=gnu++0x -O3 -c gauss.cpp


clean:
	rm -f *.o prog
