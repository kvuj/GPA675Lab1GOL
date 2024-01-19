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
	return mRule.value_or(std::move(std::string()));
}

GOL::BorderManagement GOLTeamH::borderManagement() const
{
	return mBorderManagement.value_or(GOL::BorderManagement::immutableAsIs);
}

GOL::Color GOLTeamH::color(State state) const
{
	return state == GOL::State::alive ? mAliveColor : mDeadColor;
}

// TODO
GOL::Statistics GOLTeamH::statistics() const
{
	return std::move(GOL::Statistics{
		.rule = mRule,
		.borderManagement = mBorderManagement,
		.width = width(),
		.height = height(),
		.totalCells = size(),
		.iteration = mIteration,
		.totalDeadAbs = mData.totalDead(),
		.totalAliveAbs = mData.totalAlive(),
		.totalDeadRel = mData.totalDeadRel(),
		.totalAliveRel = mData.totalAliveRel()
		// .tendencyAbs = ...,
		// .tendencyRel = ...
		});
}

// TODO
GOL::ImplementationInformation GOLTeamH::information() const
{
	return std::move(ImplementationInformation{
		.title{"Laboratoire 1"},
		.authors{{"Timothée Leclaire-Fournier"}, {"Martin Euzenat"} }
		// Réponses aux questions...
		});
}

void GOLTeamH::resize(size_t width, size_t height, State defaultState)
{
	mData.resize(width, height, defaultState);
}

// La performance n'est pas tant recherchée puisque cette fonction est appelée très rarement
bool GOLTeamH::setRule(std::string const& rule)
{
	mRule = rule;
	bool firstPart{ true };
	std::bitset<9> parsedRuleRevive, parsedRuleSurvive;

	// On vérifie que la chaine de charactères contient un B au début.
	// 5 = taille minimale
	if (rule.size() < 5 || !(rule[0] == 'B' || rule[0] == 'b'))
		return false;

	for (size_t i{ 1 }; i < rule.length(); i++) {
		// On utilise std::optional comme levier pour vérifier si un char est un nombre.
		auto opt = convertCharToNumber(rule[i]);

		// Si c'est un chiffre, on continue en enregistrant la valeur.
		if (opt.has_value()) {
			if (firstPart)
				parsedRuleRevive.set(opt.value());
			else
				parsedRuleSurvive.set(opt.value());

			continue;
		}

		// On vérifie qu'il y ait un backslash avec un char après.
		if (firstPart && rule[i] == '/' && rule.size() > i + 1
			&& (rule[i + 1] == 'S' || rule[i + 1] == 's')) {
			i++; // On saute le S
			firstPart = false;
			continue;
		}
		else // Aucun slash + s, alors pas bon.
			return false;
	}

	// Si les les deux règles ont au moins un nombre, alors OK.
	if (parsedRuleRevive.any() && parsedRuleSurvive.any()) {
		mParsedRuleRevive = parsedRuleRevive;
		mParsedRuleSurvive = parsedRuleSurvive;
		return true;
	}
	else
		return false;
}

void GOLTeamH::setBorderManagement(GOL::BorderManagement borderManagement)
{
	mBorderManagement = borderManagement;
	mIteration = 0;
}

void GOLTeamH::setState(int x, int y, State state)
{
	mData.setAt(x, y, state);
	mIteration = 0;
}

void GOLTeamH::fill(State state)
{
	mData.fill(state);
	mIteration = 0;
}

void GOLTeamH::fillAlternately(State firstCell)
{
	mData.fillAternately(firstCell);
	mIteration = 0;
}

void GOLTeamH::randomize(double percentAlive)
{
	mData.randomize(percentAlive);
	mIteration = 0;
}

// TODO
bool GOLTeamH::setFromPattern(std::string const& pattern, int centerX, int centerY)
{
	mIteration = 0;
	return false;
}

// TODO
bool GOLTeamH::setFromPattern(std::string const& pattern)
{
	mIteration = 0;
	return false;
}

void GOLTeamH::setSolidColor(State state, Color const& color)
{
	if (state == State::dead)
		mDeadColor = color;
	else
		mAliveColor = color;
}

// TODO
void GOLTeamH::processOneStep()
{
	
}

void GOLTeamH::updateImage(uint32_t* buffer, size_t buffer_size) const
{
	if (buffer == nullptr)
		return;

	auto s_ptr = buffer;
	auto e_ptr = &buffer[buffer_size];

	for (const auto& i : mData.data()) {
		if (i == GridTeamH::CellType::alive) {
			*s_ptr &= 0;						// Clear
			*s_ptr |= MAX_ALPHA << 24;			// Alpha = 255
			*s_ptr |= mAliveColor.red << 16;
			*s_ptr |= mAliveColor.green << 8;
			*s_ptr |= mAliveColor.blue;
		}
		else {
			*s_ptr &= 0;
			*s_ptr |= MAX_ALPHA << 24;
			*s_ptr |= mDeadColor.red << 16;
			*s_ptr |= mDeadColor.green << 8;
			*s_ptr |= mDeadColor.blue;
		}
		s_ptr++;

		// Sanity check
		if (s_ptr > e_ptr)
			break;
	}
}

std::optional<unsigned char> GOLTeamH::convertCharToNumber(const char c)
{
	if (c < 48 || c > 57)
		return std::nullopt;

	return (c - 48);
}
