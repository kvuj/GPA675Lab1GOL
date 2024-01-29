#include "GridTeamH.h"
#include "GOL.h"

#include <optional>
#include <utility>

// Constructeur Grid par défaut
GridTeamH::GridTeamH()
	: GridTeamH(100, 100, CellType::dead)
{
}

GridTeamH::GridTeamH(size_t width, size_t height, CellType initValue)
	:mWidth{ width }, mHeight{ height }, mEngine(mRandomDevice()), mDistribution(0.0, 1.0)
	, mAliveCount{}, mLastGenAliveCount{}
	, mData{}, mIntermediateData{}
{
	resize(width, height, initValue);
}

GridTeamH::GridTeamH(GridTeamH const& cpy)
	: GridTeamH(cpy.width(), cpy.height(), CellType::alive)
{
	mAliveCount = cpy.mAliveCount;
	memcpy(mData, cpy.mData, cpy.size() * sizeof(CellType));
	memcpy(mIntermediateData, cpy.mIntermediateData, cpy.size() * sizeof(CellType));
}

// https://learn.microsoft.com/en-us/cpp/cpp/move-constructors-and-move-assignment-operators-cpp
GridTeamH::GridTeamH(GridTeamH&& mv) noexcept
{
	*this = std::move(mv);
}

GridTeamH& GridTeamH::operator=(GridTeamH const& cpy)
{
	// Il ne faut pas se copier soi même.
	if (this != &cpy) {
		dealloc();

		mWidth = cpy.mWidth;
		mHeight = cpy.mHeight;
		mAliveCount = cpy.mAliveCount;

		mData = new CellType[mWidth * mHeight];
		mIntermediateData = new CellType[mWidth * mHeight];

		memcpy(mData, cpy.mData, cpy.size() * sizeof(CellType));
		memcpy(mIntermediateData, cpy.mIntermediateData, cpy.size() * sizeof(CellType));
	}

	return *this;
}

// https://learn.microsoft.com/en-us/cpp/cpp/move-constructors-and-move-assignment-operators-cpp
GridTeamH& GridTeamH::operator=(GridTeamH&& mv) noexcept
{
	// Il ne faut pas se copier soi même.
	if (this != &mv) {
		dealloc();

		mAliveCount = mv.mAliveCount;
		mWidth = mv.mWidth;
		mHeight = mv.mHeight;
		mData = mv.mData;
		mIntermediateData = mv.mIntermediateData;

		// Il faut que le destructeur de l'ancien objet soit valide.
		mv.mData = nullptr;
		mv.mIntermediateData = nullptr;
	}

	return *this;
}

// Destructeur Grid
GridTeamH::~GridTeamH()
{
	dealloc();
}

// Mutateur modifiant la taille de la grille et initialise le contenu par la valeur spécifiée.
void GridTeamH::resize(size_t width, size_t height, CellType initValue)
{
	dealloc();

	mWidth = width;
	mHeight = height;

	mData = new CellType[width * height];
	mIntermediateData = new CellType[width * height];

	fill(initValue, true);
}


void GridTeamH::dealloc()
{
	if (mData && mIntermediateData) {
		delete[] mData;
		delete[] mIntermediateData;
	}
	mData = nullptr;
	mIntermediateData = nullptr;
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
	mData[(static_cast<unsigned long long>(row) - 1) * mWidth + (static_cast<unsigned long long>(column) - 1)] = value;
}

void GridTeamH::setAliveCount(size_t aliveCount)
{
	mLastGenAliveCount = mAliveCount;
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
	return (mWidth * mHeight) - mAliveCount;
}

float GridTeamH::totalDeadRel() const
{
	return static_cast<float>(totalDead()) / static_cast<float>(size());
}

size_t GridTeamH::totalAlive() const
{
	return mAliveCount;
}

float GridTeamH::totalAliveRel() const
{
	return static_cast<float>(totalAlive()) / static_cast<float>(size());
}

size_t GridTeamH::lastGenDead() const
{
	return (mWidth * mHeight) - mLastGenAliveCount;
}

float GridTeamH::lastGenDeadRel() const
{
	return static_cast<float>(lastGenDead()) / static_cast<float>(size());
}

size_t GridTeamH::lastGenAlive() const
{
	return mLastGenAliveCount;
}

float GridTeamH::lastGenAliveRel() const
{
	return static_cast<float>(lastGenAlive()) / static_cast<float>(size());
}

int GridTeamH::tendencyAbs() const
{
	return lastGenAlive() - totalAlive();
}

float GridTeamH::tendencyRel() const
{
	return static_cast<float>(tendencyAbs()) / static_cast<float>(size());
}

void GridTeamH::fill(CellType value, bool fillBorder)
{
	for (size_t i{ static_cast<size_t>(1) - fillBorder }; i < mWidth - (static_cast<size_t>(1) - fillBorder); i++)
		for (size_t j{ static_cast<size_t>(1) - fillBorder }; j < mHeight - (static_cast<size_t>(1) - fillBorder); ++j)
			mData[i + (j * mWidth)] = value;
}

void GridTeamH::fillAlternately(CellType initValue, bool fillBorder)
{
	auto otherValue = (initValue == CellType::alive) ? CellType::dead : CellType::alive;

	for (size_t i{ static_cast<size_t>(1) - fillBorder }; i < mWidth - (static_cast<size_t>(1) - fillBorder); i++)
		for (size_t j{ static_cast<size_t>(1) - fillBorder }; j < mHeight - (static_cast<size_t>(1) - fillBorder); ++j)
			mData[i + (j * mWidth)] = !((i + j) % 2) ? initValue : otherValue;
}

void GridTeamH::randomize(double percentAlive, bool fillBorder)
{
	for (size_t i{ static_cast<size_t>(1) - fillBorder }; i < mWidth - (static_cast<size_t>(1) - fillBorder); i++)
		for (size_t j{ static_cast<size_t>(1) - fillBorder }; j < mHeight - (static_cast<size_t>(1) - fillBorder); ++j)
			mData[i + (j * mWidth)] = static_cast<GridTeamH::CellType>(mDistribution(mEngine) < percentAlive);
}

void GridTeamH::fillBorder(CellType value)
{
	fillBorderOperation(mData, value);
	fillBorderOperation(mIntermediateData, value);
}

void GridTeamH::fillBorderOperation(DataType ptr, CellType value) const
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

void GridTeamH::switchToIntermediate()
{
	// Swap pour la performance.
	auto* temp{ mData };
	mData = mIntermediateData;
	mIntermediateData = temp;
}
