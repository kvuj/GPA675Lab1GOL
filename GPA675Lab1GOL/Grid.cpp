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

Grid::~Grid()
{
}

size_t Grid::width() const
{
	return mWidth;
}

size_t Grid::height() const
{
	return mHeight;
}

size_t Grid::size() const
{
	return mWidth * mHeight;
}

void Grid::resize(size_t width, size_t height, CellType initValue)
{
	mData.reserve(width * height);
	mWidth = width;
	mHeight = height;
}

Grid::CellType Grid::value(int column, int row) const
{
	return mData[(column - 1) * (row - 1)];
}

void Grid::setValue(int column, int row, CellType value)
{
	mData[(column - 1) * (row - 1)] = value;
}

std::optional<Grid::CellType> Grid::at(int column, int row) const
{
	if (column > mWidth || row > mHeight)
		return std::nullopt;

	return mData[(column - 1) * (row - 1)];
}

void Grid::setAt(int column, int row, CellType value)
{
	if (column > mWidth || row > mHeight)
		return;

	mData[(column - 1) * (row - 1)] = value;
}

Grid::DataType const& Grid::data() const
{
	return mData;
}

Grid::DataType& Grid::data()
{
	return mData;
}
