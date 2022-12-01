CC = g++

CPPFLAGS = -pthread -fconcepts -I../range-v3/include -I../Utils -std=gnu++2b -ggdb -O0 -Wall -Wextra -Werror
CPPFLAGS_OPT = -pthread -fconcepts -I../range-v3/include -std=gnu++2b -I../Utils -O3 -Wall -Wextra -Werror

p1: 
	$(CC) part1.cpp -o part1 $(CPPFLAGS) 
p1_fast: 
	$(CC) part1.cpp -o part1 $(CPPFLAGS_OPT)

p2:
	$(CC) part2.cpp -o part2 $(CPPFLAGS) 
p2_fast: 
	$(CC) part2.cpp -o part2 $(CPPFLAGS_OPT)

all: p1 p2
fast: p1_fast p2_fast