#Urse Andrei 312CB
build:
	gcc battleships.c -o battleships -Wall -Wextra -std=c99 -g -lm

clean:
	rm -f battleships