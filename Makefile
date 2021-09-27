CC=g++
CFLAGS=-Wall -Wextra -O3

.PHONY: clean

csim: main.cpp bloat_simulator.cpp weapons.cpp duel_simulator.cpp simulations.cpp
	@$(CC) -o csim $(CFLAGS) $^ -fopenmp

clean:
	@rm -f csim
