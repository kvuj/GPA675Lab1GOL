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
	size_t realSize() const;

	void resize(size_t width, size_t height, CellType initValue = CellType{});

	// Accesseurs et mutateurs des cellules
	CellType value(int column, int row) const;
	void setValue(int column, int row, CellType value);

	std::optional<CellType> at(int column, int row) const;
	void setAt(int column, int row, CellType value);

	// Accesseurs du "buffer" de la grille
	DataType const& data() const;
	DataType& data();
	
	DataType const& intData() const;
	DataType& intData();

	size_t totalDead() const;
	float totalDeadRel() const;

	size_t totalAlive() const;
	float totalAliveRel() const;

	void fill(CellType value, bool fillBorder);
	void fillAternately(CellType initValue, bool fillBorder);
	void randomize(double percentAlive, bool fillBorder);

	void fillBorder(CellType value);
	void fillBorderWarped();
	void fillBorderMirror();

	void switchToIntermediate();
	bool isInBorder(size_t index) const;

private:
	DataType mData, mIntermediateData;
	size_t mWidth, mHeight;
	
	// Pour la génération de nombres aléatoires
	std::random_device mRandomDevice;
	std::mt19937 mEngine;
	std::uniform_real_distribution<> mDistribution;
};

inline bool GridTeamH::isInBorder(size_t index) const
{
	return(index % (mWidth + 2) < 1
		|| index % (mWidth + 2) > mWidth
		|| index < mWidth + 2
		|| index > (mWidth + 2) * (mHeight + 1));
}