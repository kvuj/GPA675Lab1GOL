#pragma once

#include <algorithm>
#include <vector>
#include <random>

#include "GOL.h"

class GridTeamH
{
public:
	// Définition des types
	using CellType = GOL::State;
	using DataType = std::vector<CellType>;

	// Définition des constructeurs / destructeur
	GridTeamH();
	GridTeamH(size_t width, size_t height, CellType initValue = CellType{});
	GridTeamH(GridTeamH const&) = delete;
	GridTeamH(GridTeamH&&) = delete;
	GridTeamH& operator=(GridTeamH const&) = delete;
	GridTeamH& operator=(GridTeamH&&) = delete;
	~GridTeamH();

	// Accesseurs et mutateurs de la grille
	size_t width() const;
	size_t height() const;
	size_t size() const;

	void resize(size_t width, size_t height, CellType initValue = CellType{});

	// Accesseurs et mutateurs des cellules
	CellType value(int column, int row) const;
	void setValue(int column, int row, CellType value);

	std::optional<CellType> at(int column, int row) const;
	void setAt(int column, int row, CellType value);

	// Accesseurs du "buffer" de la grille
	DataType const& data() const;
	DataType& data();

	size_t totalDead() const;
	float totalDeadRel() const;

	size_t totalAlive() const;
	float totalAliveRel() const;

	void fill(CellType value);
	void fillAternately(CellType initValue);
	void randomize(double percentAlive);

private:
	DataType mData;
	size_t mWidth, mHeight;
	
	// Pour la génération de nombres aléatoires
	std::random_device mRandomDevice;
	std::mt19937 mEngine;
	std::uniform_real_distribution<> mDistribution;
};