#include "GOLTeamH.h"

size_t GOLTeamH::width() const
{
	return mData.width();
}

size_t GOLTeamH::height() const
{
	return mData.height();
}

size_t GOLTeamH::size() const
{
	return mData.size();
}

GOL::State GOLTeamH::state(int x, int y) const
{
	return mData.value(x, y);
}

std::string GOLTeamH::rule() const
{
	return mRule;
}

GOL::BorderManagement GOLTeamH::borderManagement() const
{
	return BorderManagement();
}

GOL::Color GOLTeamH::color(State state) const
{
	return Color();
}

GOL::Statistics GOLTeamH::statistics() const
{
	return Statistics();
}

GOL::ImplementationInformation GOLTeamH::information() const
{
	return ImplementationInformation();
}

void GOLTeamH::resize(size_t width, size_t height, State defaultState)
{
	mData.resize(width, height, defaultState);
}

bool GOLTeamH::setRule(std::string const& rule)
{
	mRule = rule;
	return true;
	// TODO: return false si parsing error;
}

void GOLTeamH::setBorderManagement(BorderManagement borderManagement)
{
}

void GOLTeamH::setState(int x, int y, State state)
{
	mData.setAt(x, y, state);
}

void GOLTeamH::fill(State state)
{
	mData.resize(mData.width(), mData.height(), state);
}

void GOLTeamH::fillAlternately(State firstCell)
{
}

void GOLTeamH::randomize(double percentAlive)
{
}

bool GOLTeamH::setFromPattern(std::string const& pattern, int centerX, int centerY)
{
	return false;
}

bool GOLTeamH::setFromPattern(std::string const& pattern)
{
	return false;
}

void GOLTeamH::setSolidColor(State state, Color const& color)
{
}

void GOLTeamH::processOneStep()
{
}

void GOLTeamH::updateImage(uint32_t* buffer, size_t buffer_size) const
{
}
