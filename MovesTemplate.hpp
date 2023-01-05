#pragma once
#include <vector>

class Board;
using list_moves_t = std::vector<std::pair<int, int>>;

class Directory {
public:
	Directory();
	virtual list_moves_t simulateMoves(int directory, const Board&, int i, int j) = 0;
protected:
	std::vector<std::pair<int,int>>  m_directories_kofs;
	std::vector<std::pair<int, int>> m_horses_kofs;
};

class RayDirectory : public Directory {
public:
	list_moves_t simulateMoves(int directory, const Board&, int i, int j) override;
};

class HorseDirectory : public Directory {
public:
	list_moves_t simulateMoves(int directory, const Board&, int i, int j) override;
};

class KingDirectory : public Directory {
public:
	list_moves_t simulateMoves(int directory, const Board&, int i, int j) override;
};