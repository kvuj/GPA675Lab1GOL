#include "Grid.h"
#include "GOL.h"

Grid::Grid()
	: Grid(100, 100, CellType::alive)
{
}

Grid::Grid(size_t width, size_t height, CellType initValue)
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
	mData.reserve(width * height);
	mWidth = width;
	mHeight = height;
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

// TODO
Grid::DataType Grid::totalDead()
{
	return DataType();

}