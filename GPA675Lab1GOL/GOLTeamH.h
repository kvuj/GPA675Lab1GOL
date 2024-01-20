#pragma once

#include <string>
#include <array>
#include <optional>
#include <bitset>
#include <GOL.h>
#include "GridTeamH.h"

constexpr unsigned char MAX_ALPHA = 255;

class GOLTeamH : public GOL
{
public:
	size_t width() const override { return mData.width(); }
	size_t height() const override { return mData.height(); }
	size_t size() const override { return mData.size(); }
	State state(int x, int y) const override { return mData.value(x, y); } //details L'état d'une cellule peut être mort ou vivant.
	std::string rule() const override { return mRule.value_or(std::move(std::string())); } //details La règle est une chaîne de caractères qui contient les règles de survie et de naissance.
	BorderManagement borderManagement() const override { return mBorderManagement.value_or(GOL::BorderManagement::immutableAsIs);} //details La gestion des bords est une énumération qui contient les différents types de gestion des bords.
	Color color(State state) const override { return state == GOL::State::alive ? mAliveColor : mDeadColor; } 
	//Accesseur dans le .cpp
	Statistics statistics() const override;
	ImplementationInformation information() const override;

	void resize(size_t width, size_t height, State defaultState) override;
	bool setRule(std::string const& rule) override;
	void setBorderManagement(BorderManagement borderManagement) override;
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

	// On utilise un bitset qui contient les rÃ¨gles de chaque nombre.
	std::bitset<9> mParsedRuleRevive, mParsedRuleSurvive;
	GridTeamH mData;
	Color mDeadColor, mAliveColor;

	// Fonction utilisÃ©e Ã  l'interne.
	std::optional<unsigned char> convertCharToNumber(const char c);
};