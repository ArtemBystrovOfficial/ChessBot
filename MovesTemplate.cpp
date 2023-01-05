#include "MovesTemplate.hpp"
#include "Constants.hpp"
#include "Board.hpp"

Directory::Directory()  
: m_directories_kofs({ { 1, 1}, { 1, 0},
					   { 1,-1}, { 0,-1},
					   {-1,-1}, {-1, 0},
					   {-1, 1}, { 0, 1} }) 
, m_horses_kofs     ({ { 1, 2}, { 1,-2},
				       {-1, 2}, {-1,-2},
				       { 2, 1}, { 2,-1},
				       {-2, 1}, {-2,-1} }) {}
						

list_moves_t RayDirectory::simulateMoves(int directory, const Board& board, int i, int j) {
	if (!BasicFunctions::checkDirectory(directory))
		return {};
	
	auto [i_kof, j_kof] = m_directories_kofs[directory];
	int current_i = i + i_kof, current_j = j + j_kof;
	auto opt_active = board.isFigure(i, j);
	Figure::Color active_color;

	if (opt_active.has_value())
		active_color = opt_active.value();
	else
		return {};

	list_moves_t list; //RVO

	while (BasicFunctions::checkArea(current_i, current_j)) {
		auto opt = board.isFigure(current_i, current_j);
		if (opt.has_value()) {
			if (opt.value() != active_color) {
				list.push_back({ current_i, current_j });
			}
			break;
		}
		list.push_back({ current_i, current_j });
		current_i += i_kof;
		current_j += j_kof;
	}

	return list;
}

list_moves_t HorseDirectory::simulateMoves(int directory, const Board& board, int i, int j) {
	if (!BasicFunctions::checkDirectory(directory))
		return {};

	auto [i_kof, j_kof] = m_horses_kofs[directory];
	int current_i = i + i_kof, current_j = j + j_kof;
	auto opt_active = board.isFigure(i, j);
	Figure::Color active_color;

	if (opt_active.has_value())
		active_color = opt_active.value();
	else
		return {};

	list_moves_t list; //RVO
	
	if (BasicFunctions::checkArea(current_i, current_j)) {
		auto opt = board.isFigure(current_i, current_j);
		if (opt.has_value()) {
			if (opt.value() != active_color) {
				list.push_back({ current_i, current_j });
			}
		}
		else
			list.push_back({ current_i, current_j });
	}

	return list;
}

list_moves_t KingDirectory::simulateMoves(int directory, const Board& board, int i, int j) {
	if (!BasicFunctions::checkDirectory(directory))
		return {};

	auto [i_kof, j_kof] = m_directories_kofs[directory];
	int current_i = i + i_kof, current_j = j + j_kof;
	auto opt_active = board.isFigure(i, j);
	Figure::Color active_color;

	if (opt_active.has_value())
		active_color = opt_active.value();
	else
		return {};

	list_moves_t list; //RVO

	if (BasicFunctions::checkArea(current_i, current_j)) {
		auto opt = board.isFigure(current_i, current_j);
		if (opt.has_value()) {
			if (opt.value() != active_color) {
				list.push_back({ current_i, current_j });
			}
		}
		else
			list.push_back({ current_i, current_j });
	}

	return list;
}
