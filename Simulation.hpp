#pragma once

#include "Board.hpp"
#include <string>

using CurrentMove = Figure::Color;

class MovesSimulator {
private:
	struct BestStep {
		int min_steps;
		int i, j, to_i, to_j;
	};

	struct boardItem {
		bool is_correct;
		Figure::Color color;
		char Figure_type;
		int i, j;
	};
public:
	MovesSimulator(size_t m_absolute_max_depth = 5);

	Board readBoardFromFile(const std::string& path);
	void gameWithMate(Board);
	std::pair<bool, BestStep> simulateMate(const Board&);
private:

	std::string fromCoordToString(int i, int j);
	std::pair<int, int> fromStringToCoord(std::string);
	boardItem parseBoardItem(std::string);

	std::pair<bool, int> recusiveFindDepth(Board current_board, CurrentMove current_move,int depth, int i, int j, int to_i, int to_j);

	size_t m_absolute_max_depth;
	size_t m_max_depth;
};