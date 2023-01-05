#include "Simulation.hpp"

#include <cassert>
#include <iostream>
#include <limits>
#include <fstream>
#include <algorithm>

std::string MovesSimulator::fromCoordToString(int i, int j) {
	std::string str; //RVO
	str += char(j + 'a');
	str += char( (kSizeMap - i - 1) + '1');
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
	return str;
}

std::pair<int, int> MovesSimulator::fromStringToCoord(std::string str) {
	std::transform(str.begin(), str.end()-1, str.begin(), ::tolower);
	return std::make_pair(int( (kSizeMap - str[1] - 1 ) + '1'), int( str[0] - 'a'));
}

MovesSimulator::boardItem MovesSimulator::parseBoardItem(std::string str) {
	boardItem item;
	item.is_correct = true;

	if (str.size() != 4)
		return { false };
	//Color
	auto col = str[0];
	switch (col) {
	case 'W': item.color = Figure::Color::White;  break;
	case 'B': item.color = Figure::Color::Black; break;
	default: return { false }; break;
	}
	//Type
	auto type = str[1];
	switch (type) {
	case 'K':case 'Q':case 'R':
	case 'N':case 'B':case 'P':case 'p':
		item.Figure_type = type;
		break;
	default: return { false }; break;
	}
	//i - j
	auto [i, j] = fromStringToCoord(std::string(str.begin() + 2, str.end()));
	if (!BasicFunctions::checkArea(i, j))
		return { false };

	item.i = i;
	item.j = j;
	return item;
}

void MovesSimulator::gameWithMate(Board board) {
	board.draw();
	std::cout << "Start position. Working...\n";

	int i, j, ii, jj;
	while (true) {
		//White
		bool is = false;
		BestStep info;

		for (int i = 1; i <= m_absolute_max_depth; i++) {
			m_max_depth = i;
			auto [is_out,info_out] = simulateMate(board);
			is = is_out;
			info = info_out;
			if (is)
				break;
		}

		if (is) {
			board.drawMovePointer(info.i, info.j, info.to_i, info.to_j);
			int steps_before_mate = (info.min_steps - 2) / 2 + 1;

			std::cout << "Best Way for Whites: "<< fromCoordToString(info.i,info.j)<<" -> "<< fromCoordToString(info.to_i,info.to_j)<<"\n";
			std::cout << "Steps before Mate: " << steps_before_mate << "\n";
			system("Pause");
			auto status = board.moveFigure(info.i, info.j, info.to_i, info.to_j);
			if (steps_before_mate == 1) {
				board.draw();
				std::cout << "White win! End\n";
				system("Pause");
				return;
			}			
		}
		else {
			board.draw();
			std::cout << "No mate white's. End\n";
			system("Pause");
			return;
		}
		//Black
		while (true) {
			board.draw();
			std::cout << "Which figure move? Choose black, write: (A-H)(1-8)\n";

			std::string coord; std::cin >> coord;
			auto out_coord = fromStringToCoord(coord);
			i = out_coord.first; j = out_coord.second;

			auto opt = board.isFigure(i, j);
			if (opt.has_value()) {
				if (opt.value() != Figure::Color::Black)
					continue;
			}
			else
				continue;
		break;
		}
		while (true)
		{
			board.drawValiableMoves(i, j);
			std::cout << "Ok, where figure will step: (A-H)(1-8)\n";

			std::string coord; std::cin >> coord;
			auto out_coord = fromStringToCoord(coord);
			ii = out_coord.first; jj = out_coord.second;

			auto status = board.moveFigure(i, j, ii, jj);
			if (status != Board::MoveStatus::Normal) {
				std::cout << "try again\n";
				continue;
			}
			break;
		}
	}
}

Board MovesSimulator::readBoardFromFile(const std::string& path) {
	std::fstream in(path);
	if (!in.is_open())
		return Board(list_figure_t());
	list_figure_t list_figures;
	std::string str;
	while (std::getline(in, str)) {
		auto item = parseBoardItem(str);
		if (item.is_correct) {
			Figure* figure;

			switch (item.Figure_type) {
			case 'K': figure = new King(item.i, item.j, item.color); break;
			case 'Q': figure = new Queen(item.i, item.j, item.color); break;
			case 'R': figure = new Rook(item.i, item.j, item.color); break;
			case 'N': figure = new Horse(item.i, item.j, item.color); break;
			case 'B': figure = new Elephant(item.i, item.j, item.color); break;
			case 'P': [[fallthrough]];
			case 'p': figure = new Pawn(item.i, item.j, item.color); break;
			}
			if (figure)
				list_figures.push_back(figure);
		}
	}
	Board out(list_figures);
	for (auto elem : list_figures) // safety remove
		delete elem;

	return out;
}

MovesSimulator::MovesSimulator(size_t max_depth)
: m_absolute_max_depth(max_depth + 2)
, m_max_depth(max_depth)
{} // +2 mat with recursive

std::pair<bool, MovesSimulator::BestStep> MovesSimulator::simulateMate(const Board& board) {
	CurrentMove current_move = CurrentMove::White;

	auto list_figures = board.getFigureList(current_move);
	for (auto it = std::rbegin(list_figures);it< std::rend(list_figures);it++) {
		const auto *elem = *it;
		auto [i, j] = elem->getCoord();
		auto list_moves = board.getAllValiableMoves(i, j);

		for (auto [to_i, to_j] : list_moves) {
			auto [is, min_depth] = recusiveFindDepth(board, current_move, 1, i, j, to_i, to_j);

			if (is) {
				return { true, {min_depth,i,j,to_i,to_j} };
			}
		}
	}

	return { false,{} };
}

std::pair<bool,int> MovesSimulator::recusiveFindDepth(Board current_board, CurrentMove current_move,int depth, int i, int j, int to_i, int to_j) {
	if (depth > m_max_depth)
		return { false,depth };

	using MoveStatus = Board::MoveStatus;
	int count_good_depth = 0;
	size_t count_all_depth = 0;
	int current_min_depth = std::numeric_limits<int>::max();
	int min_depth_potential_shah = std::numeric_limits<int>::max();
	int count_pad_situations = 0;

	auto status = current_board.moveFigure(i, j, to_i, to_j);
	current_move = current_move == CurrentMove::White ? CurrentMove::Black : CurrentMove::White; // other switch

	switch (status) {
	case MoveStatus::Normal: {
		auto list_figures = current_board.getFigureList(current_move);
		for (const auto* elem : list_figures) {
			auto [ii, jj] = elem->getCoord();

			auto [is_potential_shah, depth_potential_shah] = recusiveFindDepth(current_board, current_move, depth + 1, ii, jj, ii, jj); // check shah position
			min_depth_potential_shah = std::min(min_depth_potential_shah, depth_potential_shah);

			auto list_moves = current_board.getAllValiableMoves(ii, jj);
			count_all_depth += list_moves.size();
			
			for (auto [to_ii, to_jj] : list_moves) {
				auto [is, min_depth] = recusiveFindDepth(current_board, current_move, depth + 1, ii, jj, to_ii, to_jj);

				if (min_depth - depth == 2)
					count_pad_situations++;

				if(is) {
					if(min_depth - depth != 2)
						current_min_depth = std::min(min_depth, current_min_depth);
					count_good_depth++;
					if (current_move == CurrentMove::White)
						break;
				}
			}
		}
		}break;
	case MoveStatus::Fail: 
		assert(false);
		break;
	case MoveStatus::BlackWin: {
		return { false,depth }; // inverting result for white simulation
		}break;
	case MoveStatus::WhiteWin: {
		return { true,depth };
		}break;
	}

	if (!count_all_depth) // pad 1 lvl
		return { false,depth };

	if(count_all_depth == count_good_depth && min_depth_potential_shah - depth == 2) // mat
		return { true,min_depth_potential_shah };

	if(count_pad_situations == count_all_depth) //pad 2 lvl
		return { false,current_min_depth };

	if (current_move == CurrentMove::Black)
		return  count_all_depth == count_good_depth ? std::make_pair(true, current_min_depth) 
													: std::make_pair(false, current_min_depth); // all moves
	else
		return  count_good_depth != 0 ? std::make_pair(true, current_min_depth) 
									  : std::make_pair(false, current_min_depth);				// any moves
}
