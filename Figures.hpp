#pragma once
#include "MovesTemplate.hpp"

#include <vector>
#include <memory>

using list_moves_t = std::vector<std::pair<int, int>>;
class Board;

class Figure {
public:
	friend class Board;

	enum class Color {
		Black,
		White
	};

	Figure(int i, int j, Color color);

	Color getColor() const;
	std::pair<int,int> getCoord() const;

	virtual Figure* Clone() const = 0; //pattern prototype
	virtual char getCharSymbol() const = 0;
	virtual list_moves_t getAllValibaleMoves(const Board&) = 0;
protected:
	void setCoord(const std::pair<int, int> & coord);

	Color m_color;
	std::unique_ptr <Directory> m_trasing_moves_tool;
	int32_t m_i, m_j;
};

class Pawn : public Figure  {
public:
	Pawn(int i, int j, Color color);

	Figure* Clone() const override;
	char getCharSymbol() const override;
	list_moves_t getAllValibaleMoves(const Board&) override;
};

class Horse : public Figure {
public:
	Horse(int i, int j, Color color);

	Figure* Clone() const override;
	char getCharSymbol() const override;
	list_moves_t getAllValibaleMoves(const Board&) override;
};

class Rook : public Figure {
public:
	Rook(int i, int j, Color color);

	Figure* Clone() const override;
	char getCharSymbol() const override;
	list_moves_t getAllValibaleMoves(const Board&) override;
};

class Elephant : public Figure {
public:
	Elephant(int i, int j, Color color);

	Figure* Clone() const override;
	char getCharSymbol() const override;
	list_moves_t getAllValibaleMoves(const Board&) override;
};

class Queen : public Figure {
public:
	Queen(int i, int j, Color color);

	Figure* Clone() const override;
	char getCharSymbol() const override;
	list_moves_t getAllValibaleMoves(const Board&) override;
};

class King : public Figure {
public:
	King(int i, int j, Color color);

	Figure* Clone() const override;
	char getCharSymbol() const override;
	list_moves_t getAllValibaleMoves(const Board&) override;
};