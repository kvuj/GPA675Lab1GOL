#pragma once
#ifndef GOLTEAMH_H
#define GOLTEAMH_H


#include <iostream>
#include <regex>
#include <string>
#include <optional>
#include < algorithm >

#include <GOL.h>
#include "GridTeamH.h"

// Fichier : GridTeam.h
// GPA675 – Laboratoire 1 
// Création :
// - Timothée Leclaire-Fournier et Martin Euzenat
// - 2024/01/17
// - - - - - - - - - - - - - - - - - - - - - - -
// Classe GOLTeamH
// 
// Cette classe nous permet de créer et de manipuler un jeu de la vie.
// Elle se base sur l'utilisation de la classe GridTeamH afin de gérer les mouvement dans la grid
// - - - - - - - - - - - - - - - - - - - - - - -

constexpr uint32_t MAX_ALPHA = 255 << 24;

class GOLTeamH : public GOL
{
public:
	struct sizeQueried {
		size_t width, height;
		std::string pos;
	};

	GOLTeamH();
	GOLTeamH(GOLTeamH const&) = delete;
	GOLTeamH(GOLTeamH&&) = delete;
	GOLTeamH& operator =(GOLTeamH const&) = delete;
	GOLTeamH& operator =(GOLTeamH&&) = delete;

	virtual ~GOLTeamH() = default;

	// inline puisque trivial.
	size_t width() const override { return mData.width(); }
	size_t height() const override { return mData.height(); }
	size_t size() const override { return mData.size(); }
	State state(int x, int y) const override { return mData.value(x, y); }
	std::string rule() const override { return mRule.value_or(std::string()); }
	BorderManagement borderManagement() const override { return mBorderManagement.value_or(GOL::BorderManagement::immutableAsIs); }
	Color color(State state) const override { return state == GOL::State::alive ? mAliveColor : mDeadColor; }

	Statistics statistics() const override;
	ImplementationInformation information() const override;

	void resize(size_t width, size_t height, State defaultState) override;
	bool setRule(std::string const& rule) override;
	void setBorderManagement(BorderManagement borderManagement) override;
	void setBorder();
	void setState(int x, int y, State state) override;
	void fill(State state) override;
	void fillAlternately(State firstCell) override;
	void randomize(double percentAlive) override;
	bool setFromPattern(std::string const& pattern, int centerX, int centerY) override;
	bool setFromPattern(std::string const& pattern) override;
	void setSolidColor(State state, Color const& color) override;
	void processOneStep() override;
	void updateImage(uint32_t* buffer, size_t buffer_size) const override;

private:
	std::optional<std::string> mRule;
	std::optional<BorderManagement> mBorderManagement;
	std::optional<IterationType> mIteration;

	// On utilise un bitset qui contient les règles de chaque nombre.
	// On n'utilise pas std::bitset pour des raisons de performance.
	//
	// Le premier 16 bits (à gauche) est celui de la règle de survie alors
	// qu'à droite nous avons la règle de réanimation.
	// 
	// Exemple:
	// 
	// 0000000111000111 0000000110011001
	// ^^^^^^^          ^^^^^^^
	//     Bits non utilisés
	//
	uint32_t mParsedRule;

	GridTeamH mData;
	Color mDeadColor, mAliveColor;
	uint64_t mColorEncoded;

	// Fonctions utilisées à l'interne.
	unsigned char convertCharToNumber(const char c);
	std::optional<sizeQueried> parsePattern(std::string const& pattern);
	void fillDataFromPattern(sizeQueried& sq, int centerX, int centerY);
	void countLifeStatusCells();

	// Fonction qui modifie le border selon la règle
	void modifyBorderIfNecessary();
	size_t countNeighbors(const uint8_t* ptrGrid) const;
};

#endif GOLTEAMH_H