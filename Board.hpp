#pragma once

#include "Figures.hpp"
#include "Constants.hpp"
#include <optional>
#include <vector>

using list_figure_t = std::vector <const Figure*>;

class Board {
public:
	enum class MoveStatus {
		Normal,
		BlackWin,
		WhiteWin,
		Fail
	};

	Board(const Board&);
	Board() = delete;
	Board(const list_figure_t& start_list);

	void draw() const;
	void drawValiableMoves(int i, int j) const;
	void drawMovePointer(int i, int j, int to_i, int to_j) const;

	MoveStatus moveFigure(int i,int j,int to_i,int to_j) noexcept;

	list_figure_t getFigureList(Figure::Color color) const;
	std::optional<Figure::Color> isFigure(int i,int j) const;
	
	std::vector< std::pair<int, int> > getAllValiableMoves(int i, int j) const;

	~Board();
private:

	 //move with free memory *only replace* use moveFigure
	 void safetyFreeMove(int i, int j, int to_i, int to_j); 
	 void transformToQueen(int i, int j);

	 Figure* m_board[kSizeMap][kSizeMap];
};