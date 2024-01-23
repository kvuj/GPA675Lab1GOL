#include "GridTeamH.h"
#include "GOL.h"

#include <optional>

// Constructeur Grid par défaut
GridTeamH::GridTeamH()
	: GridTeamH(100, 100, CellType::alive)
{
}

GridTeamH::GridTeamH(size_t width, size_t height, CellType initValue)
	:mWidth{ width }, mHeight{ height }, mEngine(mRandomDevice()), mDistribution(0.0, 1.0), mAliveCount{}
	, mData{}, mIntermediateData{}
{
	resize(width, height, initValue);
}

// Destructeur Grid
GridTeamH::~GridTeamH()
{
	dealloc();
}

// Mutateur modifiant la taille de la grille et initialise le contenu par la valeur spécifiée.
void GridTeamH::resize(size_t width, size_t height, CellType initValue)
{
	// On exige que si mData soit null, mIntermediateData le soi aussi.
	if (mData)
		dealloc();

	mWidth = width;
	mHeight = height;

	mData = new CellType[width * height];
	mIntermediateData = new CellType[width * height];

	fill(initValue, true);
}

void GridTeamH::dealloc()
{
	delete[] mData;
	delete[] mIntermediateData;
}

// Accesseur retournant la valeur d'une cellule à une certaine coordonnée.
GridTeamH::CellType GridTeamH::value(int column, int row) const
{
	return mData[(static_cast<unsigned long long>(row) - 1) * mWidth + (static_cast<unsigned long long>(column) - 1)];
}

// Mutateur modifiant la valeur d'une cellule à une certaine coordonnée.
void GridTeamH::setValue(int column, int row, CellType value)
{
	mData[(static_cast<unsigned long long>(row) - 1) * mWidth + (static_cast<unsigned long long>(column) - 1)] = value;
}

// Accesseur retournant la valeur d'une cellule à une certaine coordonnée. 
std::optional<GridTeamH::CellType> GridTeamH::at(int column, int row) const
{
	if (column >= mWidth || row >= mHeight)
		return std::nullopt;

	return mData[(row - 1) * mWidth + (column - 1)];
}


// Mutateur modifiant la valeur d'une cellule à une certaine coordonnée.
void GridTeamH::setAt(int column, int row, CellType value)
{
	if (column > mWidth || row > mHeight)
		return;

	mData[(row - 1) * mWidth + (column - 1)] = value;
}

void GridTeamH::setAliveCount(size_t aliveCount)
{
	mAliveCount = aliveCount;
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

size_t GridTeamH::totalDead() const
{
	return mAliveCount;
}

float GridTeamH::totalDeadRel() const
{
	return static_cast<float>(totalDead()) / static_cast<float>(size());
}

size_t GridTeamH::totalAlive() const
{
	return (mWidth * mHeight) - mAliveCount;
}

float GridTeamH::totalAliveRel() const
{
	return static_cast<float>(totalAlive()) / static_cast<float>(size());
}

void GridTeamH::fill(CellType value, bool fillBorder)
{
	for (size_t i{ static_cast<size_t>(1) - fillBorder }; i < mWidth - (static_cast<size_t>(1) - fillBorder); i++)
		for (size_t j{ static_cast<size_t>(1) - fillBorder }; j < mHeight - (static_cast<size_t>(1) - fillBorder); ++j)
			mData[i + (j * mWidth)] = value;
}

// TODO: FIX LES COLONNES EN FAISANT ATTENTION AU BORDER
// TIMOTHEE: Je m'en occupe.
void GridTeamH::fillAternately(CellType initValue, bool fillBorder)
{
	auto otherValue = (initValue == CellType::alive) ? CellType::dead : CellType::alive;

	for (size_t i{ static_cast<size_t>(1) - fillBorder }; i < mWidth - (static_cast<size_t>(1) - fillBorder); i++)
		for (size_t j{ static_cast<size_t>(1) - fillBorder }; j < mHeight - (static_cast<size_t>(1) - fillBorder); ++j)
			mData[i + (j * mWidth)] = !(i % 2) ? initValue : otherValue;
}

void GridTeamH::randomize(double percentAlive, bool fillBorder)
{
	for (size_t i{ static_cast<size_t>(1) - fillBorder }; i < mWidth - (static_cast<size_t>(1) - fillBorder); i++)
		for (size_t j{ static_cast<size_t>(1) - fillBorder }; j < mHeight - (static_cast<size_t>(1) - fillBorder); ++j)
			mData[i + (j * mWidth)] = static_cast<GridTeamH::CellType>(mDistribution(mEngine) < percentAlive);
}

void GridTeamH::fillBorder(CellType value)
{
	fillBorderManipulations(mData, value);
	fillBorderManipulations(mIntermediateData, value);
}

void GridTeamH::fillBorderManipulations(DataType ptr, CellType value) const
{
	auto* e_ptr = ptr + (mWidth - 1);

	// TOP
	while (ptr < e_ptr) {
		*ptr = value;
		ptr++;
	}

	// DROITE
	e_ptr += mWidth * (mHeight - 1);
	while (ptr < e_ptr) {
		*ptr = value;
		ptr += mWidth;
	}

	// DESSOUS
	e_ptr -= (mWidth - 1);
	while (ptr > e_ptr) {
		*ptr = value;
		ptr--;
	}

	// GAUCHE
	e_ptr -= mWidth * (mHeight - 1);
	while (ptr > e_ptr) {
		*ptr = value;
		ptr -= mWidth;
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
	auto* temp{ mData };
	mData = mIntermediateData;
	mIntermediateData = temp;
}


