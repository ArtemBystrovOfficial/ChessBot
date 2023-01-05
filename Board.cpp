#include "Board.hpp"

#include <Windows.h>
#include <iostream>
#include <map>

Board::Board(const Board& board) {
	FOR_MAP
		m_board[i][j] = board.m_board[i][j] ?
						board.m_board[i][j]->Clone() : nullptr;
}

Board::Board(const list_figure_t& start_list) {
	static bool is_first = true;
	if(is_first)
		system("color 8F");
	is_first = false;

	FOR_MAP
		m_board[i][j] = nullptr;

	for (auto *elem : start_list) {
		auto [i, j] = elem->getCoord();
		if (BasicFunctions::checkArea(i, j))
			m_board[i][j] = elem->Clone();
	}
}

void Board::draw() const{
	system("cls");

	HANDLE h;
	h = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout << "------SCREEN MAP-------\n";
	std::cout << "    ";
	for (int i = 0; i < kSizeMap; i++)
		std::cout << char('a' + i) << " ";
	std::cout << std::endl;
	for (int i = 0; i < kSizeMap; i++) {
		SetConsoleTextAttribute(h, 0xF | 0x80);
		std::cout <<" " << kSizeMap - i << "  ";
		for (int j = 0; j < kSizeMap; j++) {
			if (m_board[i][j]) {
				if (m_board[i][j]->getColor() == Figure::Color::Black)
					SetConsoleTextAttribute(h, 0x0 | 0x80);
				else
					SetConsoleTextAttribute(h, 0xF | 0x80);
				std::cout << m_board[i][j]->getCharSymbol();
			}
			else {
				SetConsoleTextAttribute(h, 0x7 | 0x80);
				std::cout << char(28);
			}
			std::cout << ' ';
		}
		std::cout << std::endl;
	}
	SetConsoleTextAttribute(h, 0xF | 0x80);
	std::cout << "----------------------\n";
}

void Board::drawValiableMoves(int ii, int jj) const {
	system("cls");

	auto list_moves = getAllValiableMoves(ii, jj);

	HANDLE h;
	h = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout << "------SCREEN MAP-------\n";
	std::cout << "    ";
	for (int i = 0; i < kSizeMap; i++)
		std::cout << char('a' + i) << " ";
	std::cout << std::endl;
	for (int i = 0; i < kSizeMap; i++) {

		SetConsoleTextAttribute(h, 0xF | 0x80);
		std::cout << " " << kSizeMap - i << "  ";
		for (int j = 0; j < kSizeMap; j++) {
			auto it = std::find(std::begin(list_moves), std::end(list_moves), std::make_pair( i,j)); // find possible move
			int background_color = it == std::end(list_moves) ? kGrayColor : kRedColor;
			if (i == ii && j == jj)
				background_color = kYellowColor;

			if (m_board[i][j]) {
				if (m_board[i][j]->getColor() == Figure::Color::Black)
					SetConsoleTextAttribute(h, 0x0 | background_color);
				else
					SetConsoleTextAttribute(h, 0xF | background_color);
				std::cout << m_board[i][j]->getCharSymbol();
			}
			else {
				SetConsoleTextAttribute(h, 0x7 | background_color);
				std::cout << char(28);
			}
			std::cout << ' ';
		}
		std::cout << std::endl;
	}
	SetConsoleTextAttribute(h, 0xF | 0x80);
	std::cout << "----------------------\n";
}

void Board::drawMovePointer(int ii, int jj, int to_ii, int to_jj) const {
	system("cls");

	HANDLE h;
	h = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout << "------SCREEN MAP-------\n";
	std::cout << "    ";
	for (int i = 0; i < kSizeMap; i++)
		std::cout << char('a' + i) << " ";
	std::cout << std::endl;
	for (int i = 0; i < kSizeMap; i++) {
		SetConsoleTextAttribute(h, 0xF | 0x80);
		std::cout << " " << kSizeMap - i << "  ";
		for (int j = 0; j < kSizeMap; j++) {
			int background_color = kGrayColor;
			if (i == ii && j == jj)
				background_color = kYellowColor;
			if (i == to_ii && j == to_jj)
				background_color = kBlueColor;
			if (m_board[i][j]) {
				if (m_board[i][j]->getColor() == Figure::Color::Black)
					SetConsoleTextAttribute(h, 0x0 | background_color);
				else
					SetConsoleTextAttribute(h, 0xF | background_color);
				std::cout << m_board[i][j]->getCharSymbol();
			}
			else {
				SetConsoleTextAttribute(h, 0x7 | background_color);
				std::cout << char(28);
			}
			std::cout << ' ';
		}
		std::cout << std::endl;
	}
	SetConsoleTextAttribute(h, 0xF | 0x80);
	std::cout << "----------------------\n";
}

Board::MoveStatus Board::moveFigure(int i, int j, int to_i, int to_j) noexcept {
	//Fail
	if (!(BasicFunctions::checkArea(i, j) && BasicFunctions::checkArea(to_i, to_j))) 
		return MoveStatus::Fail;
	if (i == to_i && j == to_j) // Check for pad
		return MoveStatus::Normal;
	if (!m_board[i][j]) 
		return MoveStatus::Fail;
	if (m_board[to_i][to_j]) 
		if (m_board[i][j]->getColor() == m_board[to_i][to_j]->getColor())
			return MoveStatus::Fail;
	
	MoveStatus status = MoveStatus::Normal;

	//Black or White win
	if (m_board[to_i][to_j])
		if (m_board[to_i][to_j]->getCharSymbol() == 'K')
			status = m_board[to_i][to_j]->getColor() == Figure::Color::Black ?
														MoveStatus::WhiteWin :
														MoveStatus::BlackWin ;
	safetyFreeMove(i, j, to_i, to_j);

	if((to_i == 0) && m_board[to_i][to_j]->getCharSymbol() == 'P'
	   && m_board[to_i][to_j]->getColor() == Figure::Color::White ||
	   (to_i == kSizeMap-1) && m_board[to_i][to_j]->getCharSymbol() == 'P'
	   && m_board[to_i][to_j]->getColor() == Figure::Color::Black)
			transformToQueen(to_i, to_j);

	return status;
}

list_figure_t Board::getFigureList(Figure::Color color) const {
	list_figure_t list;// RVO
	FOR_MAP
		if (m_board[i][j])
			if (m_board[i][j]->getColor() == color)
				list.push_back(m_board[i][j]);			
	return list;
}

std::optional<Figure::Color> Board::isFigure(int i, int j) const {
	if (BasicFunctions::checkArea(i, j))
		if (m_board[i][j])
			return m_board[i][j]->getColor();
			
	return std::nullopt;	
}

list_moves_t Board::getAllValiableMoves(int i, int j) const {
	if (BasicFunctions::checkArea(i, j))
		if (m_board[i][j])
			return m_board[i][j]->getAllValibaleMoves(*this);

	return list_moves_t();
}

Board::~Board() {
	FOR_MAP
		if (m_board[i][j])
			delete m_board[i][j];
}

void Board::safetyFreeMove(int i, int j, int to_i, int to_j) {
	if (m_board[to_i][to_j]) 
		delete m_board[to_i][to_j];

	m_board[to_i][to_j] = m_board[i][j];
	m_board[to_i][to_j]->setCoord({ to_i, to_j });
	m_board[i][j] = nullptr;
}

void Board::transformToQueen(int i, int j) {
	if (BasicFunctions::checkArea(i, j)) {
		if (m_board[i][j]) {
			auto color = m_board[i][j]->getColor();
			delete m_board[i][j];
			m_board[i][j] = new Queen(i, j, color);
		}
	}
}

