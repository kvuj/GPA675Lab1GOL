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

		// S'il n'y a pas de chiffre, on vérifie qu'il y ait 
		// un backslash avec un S après.
		if (firstPart && rule[i] == '/' && rule.size() > i + 1
			&& (rule[i + 1] == 'S' || rule[i + 1] == 's')) {
			i++;				// On saute le S
			firstPart = false;	// Deuxième partie
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

	switch (borderManagement) {
	case GOL::BorderManagement::foreverDead:
		mData.fillBorder(GridTeamH::CellType::dead);
		break;
	case GOL::BorderManagement::foreverAlive:
		mData.fillBorder(GridTeamH::CellType::alive);
		break;
	case GOL::BorderManagement::warping:
		mData.fillBorderWarped();
		break;
	case GOL::BorderManagement::mirror:
		mData.fillBorderWarped();
		break;
	}
}

void GOLTeamH::setState(int x, int y, State state)
{
	mData.setAt(x, y, state);
	mIteration = 0;
}

void GOLTeamH::fill(State state)
{
	if (mBorderManagement == GOL::BorderManagement::immutableAsIs)
		mData.fill(state, true);
	else
		mData.fill(state, false);

	mIteration = 0;
}

void GOLTeamH::fillAlternately(State firstCell)
{
	if (mBorderManagement == GOL::BorderManagement::immutableAsIs)
		mData.fillAternately(firstCell, true);
	else
		mData.fillAternately(firstCell, false);

	mIteration = 0;
}

void GOLTeamH::randomize(double percentAlive)
{
	if (mBorderManagement == GOL::BorderManagement::immutableAsIs)
		mData.randomize(percentAlive, true);
	else
		mData.randomize(percentAlive, false);

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

void GOLTeamH::processOneStep()
{
	auto& grid{ mData.data() };
	auto& intGrid{ mData.intData() };

	if (mBorderManagement == GOL::BorderManagement::foreverDead) {
		size_t aliveCount{};
		size_t offset{ width() + 2 };
		auto* ptr{ &grid[0]};

		for (size_t i{}; i < mData.realSize() - offset - 1; ++i) {

			if (mData.isInBorder(i)) {
				ptr++;
				continue;
			}

			aliveCount = 0;

			// Top
			ptr -= offset + 1;
			if (*ptr == GOL::State::alive)
				aliveCount++;
			ptr++;
			if (*ptr == GOL::State::alive)
				aliveCount++;
			ptr++;
			if (*ptr == GOL::State::alive)
				aliveCount++;

			// Milieu
			ptr += offset - 2;
			if (*ptr == GOL::State::alive)
				aliveCount++;
			ptr += 2;
			if (*ptr == GOL::State::alive)
				aliveCount++;


			// Dessous
			ptr += offset - 2;
			if (*ptr == GOL::State::alive)
				aliveCount++;
			ptr++;
			if (*ptr == GOL::State::alive)
				aliveCount++;
			ptr++;
			if (*ptr == GOL::State::alive)
				aliveCount++;

			if (grid[i] == GOL::State::alive)
				intGrid[i] = (mParsedRuleSurvive.test(aliveCount)) ? GOL::State::alive : GOL::State::dead;
			else
				intGrid[i] = (mParsedRuleRevive.test(aliveCount)) ? GOL::State::alive : GOL::State::dead;

			// On retourne à une place plus loin qu'à l'origine.
			ptr -= offset;
		}
		ptr = nullptr;

		mData.switchToIntermediate();
		mIteration.value()++;
	}
}

void GOLTeamH::updateImage(uint32_t* buffer, size_t buffer_size) const
{
	if (buffer == nullptr)
		return;

	auto s_ptr = buffer;
	auto e_ptr = &buffer[buffer_size];
	auto& grid = mData.data();

	// On itère sur chaque éléments du tableau et on associe la couleur.
	for (size_t index{ width() + 2 };
		index < (width() + 2) * (height() + 1);
		index++) {

		if (mData.isInBorder(index))
			continue;

		auto var = static_cast<uint8_t>(grid[index]);

		*s_ptr &= 0;						// Clear
		*s_ptr |= MAX_ALPHA << 24;			// Alpha = 255

		*s_ptr |= mAliveColor.red * var << 16;
		*s_ptr |= mAliveColor.green * var << 8;
		*s_ptr |= mAliveColor.blue * var;

		*s_ptr |= mDeadColor.red * (1 - var) << 16;
		*s_ptr |= mDeadColor.green * (1 - var) << 8;
		*s_ptr |= mDeadColor.blue * (1 - var);

		s_ptr++;
	}
}

std::optional<unsigned char> GOLTeamH::convertCharToNumber(const char c)
{
	if (c < 48 || c > 57)
		return std::nullopt;

	return (c - 48);
}