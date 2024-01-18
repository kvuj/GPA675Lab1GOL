#include "GOLTeamH.h"

size_t GOLTeamH::width() const
{
	return size_t();
}

size_t GOLTeamH::height() const
{
	return size_t();
}

size_t GOLTeamH::size() const
{
	return size_t();
}

GOL::State GOLTeamH::state(int x, int y) const
{
	return State();
}

std::string GOLTeamH::rule() const
{
	return std::string();
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
	return false;
}

void GOLTeamH::setBorderManagement(BorderManagement borderManagement)
{
}

void GOLTeamH::setState(int x, int y, State state)
{
}

void GOLTeamH::fill(State state)
{
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
