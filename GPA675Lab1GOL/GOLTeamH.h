#pragma once

#include <string>
#include <array>
#include <optional>
#include <bitset>
#include <GOL.h>
#include "Grid.h"

class GOLTeamH : public GOL
{
public:
	size_t width() const override;
	size_t height() const override;
	size_t size() const override;
	State state(int x, int y) const override;
	std::string rule() const override;
	BorderManagement borderManagement() const override;
	Color color(State state) const override;
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
	std::optional<GOL::BorderManagement> mBorderManagement;
	std::optional<IterationType> mIteration;

	Grid mData;
	std::bitset<9> mParsedRuleRevive, mParsedRuleSurvive;
	Color mDeadColor, mAliveColor;

	std::optional<unsigned char> convertCharToNumber(const char c);
};