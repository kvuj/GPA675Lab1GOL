#pragma once

#include <algorithm>
#include <vector>
#include <random>
#include "GOL.h"

/*
	Cette classe fonctionne en ayant deux std::vector qui contiennent les statuts
	des cellules. Un vecteur intermédiaire est échangé avec .swap() avec le vecteur
	réel lors des calculs.

	On utilise une grandeur de width + 2 par height + 2. Cela nous permet d'avoir un
	contour qui sera modifié de façon dynamique par la règle du border. Cela est
	complètement transparent aux fonctions publiques autres que certaines fonctions
	spécialisées.
*/

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
	// inline puisque trivial.
	size_t width() const { return mWidth; }
	size_t height() const { return mHeight; }
	size_t size() const { return mHeight * mWidth; }
	size_t realSize() const { return (mHeight + 2) * (mWidth + 2); }

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

// Attention: performance terrible si utilisation. Seulement lorsque vitesse
// n'est pas demandée.
inline bool GridTeamH::isInBorder(size_t index) const
{
	return(index % (mWidth + 2) < 1
		|| index % (mWidth + 2) > mWidth
		|| index < mWidth + 2
		|| index >(mWidth + 2) * (mHeight + 1));
}