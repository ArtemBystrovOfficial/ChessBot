#include "Simulation.hpp"

int main(int argc, char *argv[]) {
	MovesSimulator simulator(5);
	if (argc == 2) {
		simulator.gameWithMate(simulator.readBoardFromFile(argv[1]));
	}
	return argc==2 ? 0 : -1;
}