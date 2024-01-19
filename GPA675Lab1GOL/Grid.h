#pragma once

#include <algorithm>
#include <vector>
#include <random>

#include "GOL.h"

class Grid																		// Classe facilitant la gestion d'un tableau dynamique 2d.
{																				// 
public:																			// 
	// Définition des types														//
	using CellType = GOL::State;												// Correspond au type fondamental de chaque cellule de la grille. 
	using DataType = std::vector<CellType>;										// Correspond à la grille, au conteneur de cellules. À vous de déterminer la structure de données à utiliser.
																				//
	// Définition des constructeurs / destructeur								//
	Grid();																		// 
	Grid(size_t width, size_t height, CellType initValue = CellType{});			// 
	Grid(Grid const&) = delete;													// 
	Grid(Grid&&) = delete;														// 
	Grid& operator=(Grid const&) = delete;										// 
	Grid& operator=(Grid&&) = delete;											// 
	~Grid();																	// 
																				//
	// Accesseurs et mutateurs de la grille										// 
	size_t width() const;														// Accesseur retournant la largeur de la grille.
	size_t height() const;														// Accesseur retournant la hauteur de la grille.
	size_t size() const;														// Accesseur retournant le nombre de cellule de la grille.
																				// 
	void resize(size_t width, size_t height, CellType initValue = CellType{});	// Mutateur modifiant la taille de la grille et initialise le contenu par la valeur spécifiée.
																				//
	// Accesseurs et mutateurs des cellules										//
	CellType value(int column, int row) const;									// Accesseur retournant la valeur d'une cellule à une certaine coordonnée. Pour une raison de performance, cette fonction NE VALIDE PAS ses entrées. Autrement dit, c'est la responsabilité du programmeur utilisateur de faire ses validations, au risque de 'crasher' le programme.
	void setValue(int column, int row, CellType value);							// Mutateur modifiant la valeur d'une cellule à une certaine coordonnée. Pour une raison de performance, cette fonction NE VALIDE PAS ses entrées. Autrement dit, c'est la responsabilité du programmeur utilisateur de faire ses validations, au risque de 'crasher' le programme.
																				// 
	std::optional<CellType> at(int column, int row) const;						// Accesseur retournant la valeur d'une cellule à une certaine coordonnée. Cette fonction VALIDE ses entrées et retourne un optional nul si la coordonnée est invalide. 
	void setAt(int column, int row, CellType value);							// Mutateur modifiant la valeur d'une cellule à une certaine coordonnée. Cette fonction VALIDE ses entrées et ne fait rien si la coordonnée est invalide. 
																				// 
	// Accesseurs du "buffer" de la grille.										//
	DataType const& data() const;												// Accesseur en lecture seule sur le "buffer" de la grille.
	DataType& data();															// Accesseur en lecture/écriture sur le "buffer" de la grille.

	size_t totalDead() const;
	float totalDeadRel() const;

	size_t totalAlive() const;
	float totalAliveRel() const;

	void fill(CellType value);
	void fillAternately(CellType initValue);
	void randomize(double percentAlive);

private:																		// 
	DataType mData;																// Il y a des attributs essentiels au fonctionnement de cette classe.
	size_t mWidth, mHeight;														// À vous de les déterminer. 
	
	// Random
	std::random_device mRandomDevice;
	std::mt19937 mEngine;
	std::uniform_real_distribution<> mDistribution;
};