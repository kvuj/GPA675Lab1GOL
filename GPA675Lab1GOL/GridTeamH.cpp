#include "GridTeamH.h"
#include "GOL.h"
#include <iostream>

//constructeur Grid par défaut


GridTeamH::GridTeamH()
	: GridTeamH(100, 100, CellType::alive)
{
}

GridTeamH::GridTeamH(size_t width, size_t height, CellType initValue)
	:mWidth{ width }, mHeight{ height }, mEngine(mRandomDevice()), mDistribution(0.0, 1.0)
{
	resize(width, height, initValue);
}

// Destructeur Grid
GridTeamH::~GridTeamH()
{

}

// Accesseur retournant la largeur de la grille.
size_t GridTeamH::width() const
{
	return mWidth;
}


// Accesseur retournant la hauteur de la grille.
size_t GridTeamH::height() const
{
	return mHeight;
}

// Accesseur retournant le nombre de cellule de la grille (la taille de la grille).
size_t GridTeamH::size() const
{
	return mWidth * mHeight;
}


// Mutateur modifiant la taille de la grille et initialise le contenu par la valeur spécifiée.
void GridTeamH::resize(size_t width, size_t height, CellType initValue)
{
	// TODO: Performance de resize avec beaucoup d'appel?
	// Investiguer reserve + resize
	mData.resize(width * height);
	mWidth = width;
	mHeight = height;

	fill(initValue);
}

// Accesseur retournant la valeur d'une cellule à une certaine coordonnée.
GridTeamH::CellType GridTeamH::value(int column, int row) const

{
	return mData[(column - 1) * (row - 1)];
}

// Mutateur modifiant la valeur d'une cellule à une certaine coordonnée.
void GridTeamH::setValue(int column, int row, CellType value)
{
	mData[(column - 1) * (row - 1)] = value;
}

// Accesseur retournant la valeur d'une cellule à une certaine coordonnée. 
std::optional<GridTeamH::CellType> GridTeamH::at(int column, int row) const
{
	if (column > mWidth || row > mHeight)
		return std::nullopt;

	return mData[(column - 1) * (row - 1)];
}


// Mutateur modifiant la valeur d'une cellule à une certaine coordonn�e.
void GridTeamH::setAt(int column, int row, CellType value)
{
	if (column > mWidth || row > mHeight)
		return;

	mData[(column - 1) * (row - 1)] = value;
}


// Accesseur en lecture seule sur le "buffer" de la grille.
GridTeamH::DataType const& GridTeamH::data() const
{
	return mData;
}
// Accesseur en lecture/écriture sur le "buffer" de la grille.
GridTeamH::DataType& GridTeamH::data()

{
	return mData;
}

// https://en.cppreference.com/w/cpp/algorithm/count
size_t GridTeamH::totalDead() const
{
	return std::count_if(mData.begin(), mData.end(), [](auto& i) { return i == CellType::dead; });
}

float GridTeamH::totalDeadRel() const
{
	return static_cast<float>(totalDead()) / static_cast<float>(size());
}

size_t GridTeamH::totalAlive() const
{
	return std::count_if(mData.begin(), mData.end(), [](auto& i) { return i == CellType::alive; });
}

float GridTeamH::totalAliveRel() const
{
	return static_cast<float>(totalAlive()) / static_cast<float>(size());
}

void GridTeamH::fill(CellType value)
{
	for (auto& i : mData)
		i = value;
}

void GridTeamH::fillAternately(CellType initValue)
{
	auto otherValue = (initValue == CellType::alive) ? CellType::dead : CellType::alive;

	for (size_t i{}; i < mData.size(); i++)
		mData[i] = !(i % 2) ? initValue : otherValue;
}

void GridTeamH::randomize(double percentAlive)
{
	for (auto& i : mData) {
		if (mDistribution(mEngine) < percentAlive)
			i = CellType::alive;
		else
			i = CellType::dead;
	}
}
