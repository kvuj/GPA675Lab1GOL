#include "Grid.h"
#include "GOL.h"

Grid::Grid()
	: Grid(100, 100, CellType::alive)
{
}

Grid::Grid(size_t width, size_t height, CellType initValue)
	:mWidth{ width }, mHeight{ height }, mEngine(mRandomDevice), mDistribution(0.0, 1.0)
{
	resize(width, height, initValue);
}

// Destructeur Grid
Grid::~Grid()
{

}

// Accesseur retournant la largeur de la grille.
size_t Grid::width() const
{
	return mWidth;
}
// Accesseur retournant la hauteur de la grille.
size_t Grid::height() const
{
	return mHeight;
}

// Accesseur retournant le nombre de cellule de la grille (la taille de la grille).
size_t Grid::size() const
{
	return mWidth * mHeight;
}

// Mutateur modifiant la taille de la grille et initialise le contenu par la valeur spécifiée.
void Grid::resize(size_t width, size_t height, CellType initValue)
{
	// TODO: Performance de resize avec beaucoup d'appel?
	// Investiguer reserve + resize
	mData.resize(width * height);
	mWidth = width;
	mHeight = height;

	fill(initValue);
}

// Accesseur retournant la valeur d'une cellule à une certaine coordonnée.
Grid::CellType Grid::value(int column, int row) const
{
	return mData[(column - 1) * (row - 1)];
}

// Mutateur modifiant la valeur d'une cellule à une certaine coordonnée.
void Grid::setValue(int column, int row, CellType value)
{
	mData[(column - 1) * (row - 1)] = value;
}

// Accesseur retournant la valeur d'une cellule à une certaine coordonnée. 
std::optional<Grid::CellType> Grid::at(int column, int row) const
{
	if (column > mWidth || row > mHeight)
		return std::nullopt;

	return mData[(column - 1) * (row - 1)];
}

// Mutateur modifiant la valeur d'une cellule à une certaine coordonn�e.
void Grid::setAt(int column, int row, CellType value)
{
	if (column > mWidth || row > mHeight)
		return;

	mData[(column - 1) * (row - 1)] = value;
}
// Accesseur en lecture seule sur le "buffer" de la grille.
Grid::DataType const& Grid::data() const
{
	return mData;
}
// Accesseur en lecture/écriture sur le "buffer" de la grille.
Grid::DataType& Grid::data()
{
	return mData;
}

// https://en.cppreference.com/w/cpp/algorithm/count
size_t Grid::totalDead() const
{
	return std::count_if(mData.begin(), mData.end(), [](auto& i) { return i == CellType::dead; });
}

float Grid::totalDeadRel() const
{
	return static_cast<float>(totalDead()) / static_cast<float>(size());
}

size_t Grid::totalAlive() const
{
	return std::count_if(mData.begin(), mData.end(), [](auto& i) { return i == CellType::alive; });
}

float Grid::totalAliveRel() const
{
	return static_cast<float>(totalAlive()) / static_cast<float>(size());
}

void Grid::fill(CellType value)
{
	for (auto& i : mData)
		i = value;
}

void Grid::fillAternately(CellType initValue)
{
	auto otherValue = (initValue == CellType::alive) ? CellType::dead : CellType::alive;

	for (size_t i{}; i < mData.size(); i++)
		mData[i] = !(i % 2) ? initValue : otherValue;
}

void Grid::randomize(double percentAlive)
{
	for (auto& i : mData) {
		if (mDistribution(mEngine) < percentAlive)
			i = CellType::alive;
	}
}
