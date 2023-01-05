#include "Figures.hpp"
#include "Constants.hpp"

#include <algorithm>
#include <vector>
#include "Board.hpp"

Figure::Figure(int i, int j, Color color) 
: m_i(i)
, m_j(j)
, m_color(color){}

Figure::Color Figure::getColor() const {
	return m_color;
}

std::pair<int, int> Figure::getCoord() const {
	return { m_i,m_j };
}

void Figure::setCoord(const std::pair<int, int>& coord) {
	m_i = coord.first;
	m_j = coord.second;
}

Pawn::Pawn		  (int i, int j, Color color) : Figure(i, j, color) { m_trasing_moves_tool = std::make_unique<KingDirectory>(); }
Horse::Horse      (int i, int j, Color color) : Figure(i, j, color) { m_trasing_moves_tool = std::make_unique<HorseDirectory>(); }
Rook::Rook        (int i, int j, Color color) : Figure(i, j, color) { m_trasing_moves_tool = std::make_unique<RayDirectory>(); }
Elephant::Elephant(int i, int j, Color color) : Figure(i, j, color) { m_trasing_moves_tool = std::make_unique<RayDirectory>(); }
Queen::Queen      (int i, int j, Color color) : Figure(i, j, color) { m_trasing_moves_tool = std::make_unique<RayDirectory>(); }
King::King        (int i, int j, Color color) : Figure(i, j, color) { m_trasing_moves_tool = std::make_unique<KingDirectory>(); }

Figure* Pawn::Clone() const		{return new Pawn    (m_i, m_j, m_color);}
Figure* Horse::Clone() const	{return new Horse   (m_i, m_j, m_color);}
Figure* Rook::Clone() const		{return new Rook    (m_i, m_j, m_color);}
Figure* Elephant::Clone() const	{return new Elephant(m_i, m_j, m_color);}
Figure* Queen::Clone() const	{return new Queen   (m_i, m_j, m_color);}
Figure* King::Clone() const		{return new King    (m_i, m_j, m_color);}

char Pawn::getCharSymbol() const	 {return 'P';}
char Horse::getCharSymbol() const	 {return 'N';}
char Rook::getCharSymbol() const	 {return 'R';}
char Elephant::getCharSymbol() const {return 'B';}
char Queen::getCharSymbol() const	 {return 'Q';}
char King::getCharSymbol() const	 {return 'K';}

list_moves_t Pawn::getAllValibaleMoves(const Board& board) {
	list_moves_t list; //RVO
	// Step
	// 1|  black (reverse)
	//  x 
	//  |5 white 
	if (m_color == Color::Black) {
		auto col_opt = board.isFigure(m_i + 1, m_j);
		if (!col_opt.has_value()) { // no figures front 
			auto out = m_trasing_moves_tool->simulateMoves(1, board, m_i, m_j);
			list.insert(std::end(list), std::begin(out), std::end(out));
		}
	}
	else {
		auto col_opt = board.isFigure(m_i - 1, m_j);
		if (!col_opt.has_value()) {// no figures front
			auto out = m_trasing_moves_tool->simulateMoves(5, board, m_i, m_j);
			list.insert(std::end(list), std::begin(out), std::end(out));
		}
	}
	// Attack
		// Step
	//0\   /2 -> black (reverse)
	//   x
	//6/   \4 -> white
	if (m_color == Color::Black) {
		auto col_opt = board.isFigure(m_i + 1, m_j + 1);
		if (col_opt.has_value()) { // no figures front 
			auto out = m_trasing_moves_tool->simulateMoves(0, board, m_i, m_j);
			list.insert(std::end(list), std::begin(out), std::end(out));
		}
		col_opt = board.isFigure(m_i + 1, m_j - 1);
		if (col_opt.has_value()) { // no figures front 
			auto out = m_trasing_moves_tool->simulateMoves(2, board, m_i, m_j);
			list.insert(std::end(list), std::begin(out), std::end(out));
		}
	}
	else {
		auto col_opt = board.isFigure(m_i - 1, m_j + 1);
		if (col_opt.has_value()) { // no figures front 
			auto out = m_trasing_moves_tool->simulateMoves(6, board, m_i, m_j);
			list.insert(std::end(list), std::begin(out), std::end(out));
		}
		col_opt = board.isFigure(m_i - 1, m_j - 1);
		if (col_opt.has_value()) { // no figures front 
			auto out = m_trasing_moves_tool->simulateMoves(4, board, m_i, m_j);
			list.insert(std::end(list), std::begin(out), std::end(out));
		}
	}

	return list;
}

list_moves_t Horse::getAllValibaleMoves(const Board& board) {
	list_moves_t list; //RVO
	for (int i = 0; i < kCountDirectories; i++) {
		auto out = m_trasing_moves_tool->simulateMoves(i, board, m_i, m_j);
		list.insert(std::end(list), std::begin(out), std::end(out));
	}
	return list;
}

list_moves_t Rook::getAllValibaleMoves(const Board& board) {
	list_moves_t list; //RVO
	//     |1  
	// 7 - x - 3 
	//	   |5  -> i=1; i+=2
	for (int i = 1; i < kCountDirectories; i += 2) {
		auto out = m_trasing_moves_tool->simulateMoves(i, board, m_i, m_j);
		list.insert(std::end(list), std::begin(out), std::end(out));
	}
	return list;
}

list_moves_t Elephant::getAllValibaleMoves(const Board& board) {
	list_moves_t list; //RVO
	//0\   /2
	//   x
	//6/   \4 -> i+=2
	for (int i = 0; i < kCountDirectories; i += 2) {
		auto out = m_trasing_moves_tool->simulateMoves(i, board, m_i, m_j); 
		list.insert(std::end(list), std::begin(out), std::end(out));
	}
	return list;
}

list_moves_t Queen::getAllValibaleMoves(const Board& board) {
	list_moves_t list; //RVO
	//0\1| /2
	//7- x -3
	//6/ |5\4 -> i++
	for (int i = 0; i < kCountDirectories; i++) {
		auto out = m_trasing_moves_tool->simulateMoves(i, board, m_i, m_j);
		list.insert(std::end(list), std::begin(out), std::end(out));
	}
	return list;
}

list_moves_t King::getAllValibaleMoves(const Board& board) {
	list_moves_t list; //RVO
	//0\1| /2
	//7- x -3
	//6/ |5\4 -> i++
	for (int i = 0; i < kCountDirectories; i++) {
		auto out = m_trasing_moves_tool->simulateMoves(i, board, m_i, m_j);
		list.insert(std::end(list), std::begin(out), std::end(out));
	}
	return list;
}
