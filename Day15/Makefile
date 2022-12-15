CC = g++

CPPFLAGS = -pthread -fconcepts -I../range-v3/include -I../Utils -std=gnu++2b -ggdb -O0 -Wall -Wextra -Werror
CPPFLAGS_OPT = -pthread -fconcepts -I../range-v3/include -std=gnu++2b -I../Utils -O3 -Wall -Wextra -Werror

p1: 
	$(CC) part1.cpp -o part1.out $(CPPFLAGS) 
p1_alt: 
	$(CC) part1_alt.cpp -o part1_alt.out $(CPPFLAGS) -lz3
p1_fast: 
	$(CC) part1.cpp -o part1.out $(CPPFLAGS_OPT)
p1_alt_fast: 
	$(CC) part1_alt.cpp -o part1_alt.out $(CPPFLAGS_OPT) -lz3

p2:
	$(CC) part2.cpp -o part2.out $(CPPFLAGS) 
p2_alt: 
	$(CC) part2_alt.cpp -o part2_alt.out $(CPPFLAGS) -lz3
p2_fast: 
	$(CC) part2.cpp -o part2.out $(CPPFLAGS_OPT)
p2_alt_fast: 
	$(CC) part2_alt.cpp -o part2_alt.out $(CPPFLAGS_OPT) -lz3

all: p1 p2
alt: p1_alt p2_alt
fast: p1_fast p2_fast
alt_fast: p1_alt_fast p2_alt_fast