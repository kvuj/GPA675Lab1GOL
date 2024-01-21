#include "GridTeamH.h"
#include "GOL.h"
#include <iostream>

// Constructeur Grid par défaut
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

// Mutateur modifiant la taille de la grille et initialise le contenu par la valeur spécifiée.
void GridTeamH::resize(size_t width, size_t height, CellType initValue)
{
	mData.resize((width + 2) * (height + 2));
	mIntermediateData.resize((width + 2) * (width + 2));
	mWidth = width;
	mHeight = height;

	fill(initValue, false);
}

// Accesseur retournant la valeur d'une cellule à une certaine coordonnée.
GridTeamH::CellType GridTeamH::value(int column, int row) const
{
	size_t offset{ mWidth + 2 };
	return mData[offset * row + (column - 1)];
}

// Mutateur modifiant la valeur d'une cellule à une certaine coordonnée.
void GridTeamH::setValue(int column, int row, CellType value)
{
	size_t offset{ mWidth + 2 };
	mData[offset * row + (column - 1)] = value;
}

// Accesseur retournant la valeur d'une cellule à une certaine coordonnée. 
std::optional<GridTeamH::CellType> GridTeamH::at(int column, int row) const
{
	if (column >= mWidth || row >= mHeight)
		return std::nullopt;

	size_t offset{ mWidth + 2 };
	return mData[offset * row + (column - 1)];
}


// Mutateur modifiant la valeur d'une cellule à une certaine coordonn e.
void GridTeamH::setAt(int column, int row, CellType value)
{
	if (column > mWidth || row > mHeight)
		return;

	size_t offset{ mWidth + 2 };
	mData[offset * row + (column - 1)] = value;
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

GridTeamH::DataType const& GridTeamH::intData() const
{
	return mIntermediateData;
}

GridTeamH::DataType& GridTeamH::intData()
{
	return mIntermediateData;
}

// TODO: FIX
// https://en.cppreference.com/w/cpp/algorithm/count
size_t GridTeamH::totalDead() const
{
	return std::count_if(mData.begin(), mData.end(), [](auto& i) { return i == CellType::dead; });
}

float GridTeamH::totalDeadRel() const
{
	return static_cast<float>(totalDead()) / static_cast<float>(size());
}

// TODO: FIX
size_t GridTeamH::totalAlive() const
{
	return std::count_if(mData.begin(), mData.end(), [](auto& i) { return i == CellType::alive; });
}

float GridTeamH::totalAliveRel() const
{
	return static_cast<float>(totalAlive()) / static_cast<float>(size());
}

void GridTeamH::fill(CellType value, bool fillBorder)
{
	if (fillBorder) {
		for (auto& i : mData)
			i = value;
	}
	else {
		for (size_t index{}; index < mData.size(); index++) {
			if (isInBorder(index))
				continue;

			mData[index] = value;
		}
	}
}

// TODO: FIX LES COLONNES EN FAISANT ATTENTION AU BORDER
// TIMOTHEE: Je m'en occupe.
void GridTeamH::fillAternately(CellType initValue, bool fillBorder)
{
	auto otherValue = (initValue == CellType::alive) ? CellType::dead : CellType::alive;

	if (fillBorder) {
		for (size_t i{}; i < mData.size(); i++)
			mData[i] = !(i % 2) ? initValue : otherValue;
	}
	else {
		for (size_t index{}; index < mData.size(); index++) {
			if (isInBorder(index))
				continue;

			mData[index] = !(index % 2) ? initValue : otherValue;
		}
	}
}

void GridTeamH::randomize(double percentAlive, bool fillBorder)
{
	if (fillBorder) {
		for (auto& i : mData) {
			if (mDistribution(mEngine) < percentAlive)
				i = CellType::alive;
			else
				i = CellType::dead;
		}
	}
	else {
		for (size_t index{}; index < mData.size() - 1; index++) {
			if (isInBorder(index))
				continue;

			if (mDistribution(mEngine) < percentAlive)
				mData[index] = CellType::alive;
			else
				mData[index] = CellType::dead;
		}
	}
}


// Performance non nécessaire.
void GridTeamH::fillBorder(CellType value)
{
	for (size_t index{}; index < mData.size(); index++) {
		if (isInBorder(index))
			mData[index] = value;
	}
}

// TODO
void GridTeamH::fillBorderWarped()
{

}

// TODO
void GridTeamH::fillBorderMirror()
{

}

void GridTeamH::switchToIntermediate()
{
	// Swap pour la performance.
	mData.swap(mIntermediateData);
}

