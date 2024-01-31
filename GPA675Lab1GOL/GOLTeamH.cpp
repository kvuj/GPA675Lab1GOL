#include "GOLTeamH.h"

GOLTeamH::GOLTeamH()
	: mParsedRule{}, mColorEncoded{}
{
}

//! \brief Accesseurs retournant des informations générales sur la 
	//! simulation en cours.
	//! 
	//! \details Retourne plusieurs informations sur la simulation. Chaque 
	//! valeur est optionnelle et peut être indéterminée.
	//! 
	//! Voir la documentation liée à la structure Statistics pour plus de détails.
	//! 
	//! Toutefois, les tendances sont définies ainsi :
	//! - La tendance indique la variation entre le nombre de cellules mortes 
	//!   et vivantes en valeur absolue et relative.
	//! - Une tendance est à la hausse s'il y a plus de cellules naissantes 
	//!   que mourantes.
	//! - Un premier caractères indique si la tendance est à la hausse (+) ou
	//!   à la baisse (-).
	//! - La tendance doit se faire sur un certain nombre d'itérations. Ce
	//!   nombre est défini par l'implémentation mais doit être supérieur à 10 
	//!   itérations. L'objectif est d'avoir une lecture relativement stable de 
	//!   la tendance afin qu'elle soit agréable à lire dans l'interface usager.
	//! - Optionnellement, vous pouvez ajouter un second caractère indiquant 
	//!   la stabilité de la tendance. Voici la légende :
	//!   - '-' : stable
	//!   - '~' : légèrement instable
	//!   - 'w' : instable
	//!   - 'W' : très instable
	//! 
	//! \return Une structure contenant les informations sur la simulation.
GOL::Statistics GOLTeamH::statistics() const
{
	return GOL::Statistics{
		.rule = mRule,
		.borderManagement = mBorderManagement,
		.width = width(),
		.height = height(),
		.totalCells = size(),
		.iteration = mIteration,
		.totalDeadAbs = mData.totalDead(),
		.totalAliveAbs = mData.totalAlive(),
		.totalDeadRel = mData.totalDeadRel(),
		.totalAliveRel = mData.totalAliveRel(),
		.tendencyAbs = mData.tendencyAbs(),
		.tendencyRel = mData.tendencyRel()
	};
}

//! \brief Accesseurs retournant les informations sur la réalisation 
	//! de l'implémentation. 
	//! 
	//! \details Retourne plusieurs informations sur la réalisation de
	//! l'implémentation. Chaque valeur est optionnelle et peut être
	//! indéterminée.
	//! 
	//! En quelque sorte, c'est l'approche utilisée pour produire un mini 
	//! rapport utilitaire.
	//! 
	//! Voir la documentation liée à la structure ImplementationInformation
	//! pour plus de détails.
	//! 
	//! Pour les réponses 'answers', l'objectif est d'utiliser clairement le 
	//! vocabulaire technique approprié tout en étant très concis et très précis.
	//! 
	//! \return Une structure contenant les informations sur la réalisation
	//! 
GOL::ImplementationInformation GOLTeamH::information() const
{
	return ImplementationInformation{
		.title{"Laboratoire 1"},
		.authors{{"Timothée Leclaire-Fournier timothee.leclaire-fournier.1@ens.etsmtl.ca"},
				 {"Martin Euzenat martin.euzenat.1@ens.etsmtl.ca"}},
		.answers{},
		.optionnalComments{}
		// Réponses aux questions...
	};
}


//! \brief Mutateur modifiant la taille de la grille de simulation.
//! 
//! \details Cette fonction réinitialise la taille et le contenu de la 
//! grille. La taille est spécifié peut être 0 x 0. Le contenu de la 
//! grille est entièrement mis à l'état passé en argument.
//! 
//! Cette fonction s'assure que si l'une des dimensions est 0, alors les 
//! deux sont mises à 0.
//! 
//! L'ancient motif de la grille est perdu. L'itération courante est remise 
//! à 0.
//! 
//! \param width La nouvelle largeur de la grille.
//! \param height La nouvelle hauteur de la grille.
//! \param defaultState L'état d'initialisation des cellules.
//! 
void GOLTeamH::resize(size_t width, size_t height, State defaultState)
{
	mData.resize(width, height, defaultState);
	setBorder();
	countLifeStatusCells();
}

//! \brief Mutateur modifiant la règle de la simulation.
	//! 
	//! \details Cette fonction s'assure que la chaîne de caractères est valide 
	//! et assigne la nouvelle règle pour que les prochaines évolutions 
	//! l'utilisent. 
	//! 
	//! Si la règle est valide, on assigne la nouvelle règle, l'ancienne règle 
	//! est perdue et l'itération courante est remise à 0. Si la règle est 
	//! invalide, on ne fait rien.
	//! 
	//! La simulation doit être en mesure de supporté les règles valides.
	//! 
	//! Les règles sont définies par une chaîne de caractère suivant ce format:
	//! `B###.../S###...`, par exemple : `B3/S23`.
	//! - La lettre `B` (minuscule ou majuscule) débute la section 'B'orn 
	//!   concernant les états morts qui naissent
	//! - Suit une série de _n_ caractères pouvant contenir une instance de 
	//!   chacun de ces caractères : '0', '1', '2', '3', '4', '5', '6', '7', '8'.
	//!   La valeur de _n_ peut varier de 0 à 9. Cette séquence indique toutes 
	//!   les combinaisons du nombre de cellules voisines vivantes qui font 
	//!   naître une cellule morte.
	//! - Le caractère barre oblique `/` (_slash_) qui indique la transition 
	//!   vers la section suivante.
	//! - La lettre `S` (minuscule ou majuscule) débute la section 'S'urvive
	//!   concernant les états vivants qui survivent (qui restent vivantes).
	//! - Suit une série de _n_ caractères pouvant contenir une instance de 
	//!   chacun de ces caractères : '0', '1', '2', '3', '4', '5', '6', '7', '8'.
	//!   La valeur de _n_ peut varier de 0 à 9. Cette séquence indique toutes 
	//!   les combinaisons du nombre de cellules voisines vivantes qui font 
	//!   naître une cellule morte.
	//!	  
	//! Par exemple : `B02/S1357` indique que :
	//! - si une cellule est morte :
	//!		- elle naît se elle possède 0 ou 2 voisins vivants
	//!	- si la cellule est vivante :
	//!		- elle survie si elle possède 1, 3, 5 ou 7 voisins vivants
	//! 
	//! La règle de Conway `B3/S23` est celle par défaut.

bool GOLTeamH::setRule(std::string const& rule)
{
	mParsedRule = 0;
	std::regex regexp(R"(B(\d+)/S(\d+))", std::regex_constants::icase);
	std::smatch m;

	if (std::regex_search(rule, m, regexp)) {
		for (auto& i : m[1].str())
			mParsedRule |= 1u << convertCharToNumber(i);
		
		for (auto& i : m[2].str())
			mParsedRule |= 1u << (convertCharToNumber(i) + 16);

		return true;
	}
	return false;
}

//! \brief Mutateur modifiant la stratégie de gestion de bord.
	//! 
	//! \details Cette fonction assigne la nouvelle stratégie pour que les
	//! prochaines évolutions l'utilisent.
	//! 
	//! L'ancienne stratégie est perdue. L'itération courante est remise à 0.
	//! 
	//! Vous devez être en mesure de gérer les effets de bord selon 5 
	//! stratégies différentes :								 
	//!	- BorderManagement::immutableAsIs : immuable tel quel. Les cellules 
	//!   du contour ne sont jamais modifiées.
	//! - BorderManagement::foreverDead : mort pour toujours. Les cellules 
	//!   du contour ne sont jamais modifiées, elles sont toujours mortes.
	//! - BorderManagement::foreverAlive : vivant pour toujours. Les cellules
	//!   du contour ne sont jamais modifiées, elles sont toujours vivantes.
	//! - BorderManagement::warping : téléportation. Les cellules du contour
	//!   sont évaluées. On utilise les cellules du côté opposé de la grille
	//!   pour les cellules extérieures.
	//! - BorderManagement::mirror : miroir. Les cellules du contour sont
	//!   évaluées. On utilise les cellules du côté opposé de la cellule pour
	//!   les cellules extérieures.
	//!																
	//! Pour les 2 exemples suivants, la cellule analysée `-*-` possède 3 
	//! voisins situés à l'extérieur de la grille 'x', 'y' et 'z'. Ces cellules 
	//! sont substituées par les voisins opposés 'X', 'Y' et 'Z'. 
	//!     	
	//! BorderManagement::warping utilise les voisins opposés par rapport à la 
	//! **grille**.


void GOLTeamH::setBorderManagement(BorderManagement borderManagement)
{
	mBorderManagement = borderManagement;
	mIteration = 0;
	setBorder();
	countLifeStatusCells();
}

void GOLTeamH::setBorder()
{
	switch (mBorderManagement.value_or(GOL::BorderManagement::foreverDead)) {
	case GOL::BorderManagement::foreverDead:
		mData.fillBorder(GridTeamH::CellType::dead);
		break;
	case GOL::BorderManagement::foreverAlive:
		mData.fillBorder(GridTeamH::CellType::alive);
		break;
	}
}


//! \brief Mutateur modifiant l'état d'une cellule de la grille.
	//! 
	//! \details Cette fonction ne valide pas les entrées pour une raison de 
	//! performance. L'origine est le coin supérieur gauche de la grille.
	//! 
	//! \param x La coordonnée en x de la cellule.
	//! \param y La coordonnée en y de la cellule.
	//! \param state Le nouvel état de la cellule.
void GOLTeamH::setState(int x, int y, State state)
{
	mData.setAt(x, y, state);
	mIteration = 0;
	countLifeStatusCells();
}

//! \brief Mutateur remplissant de façon uniforme toutes les cellules de 
	//! la grille.
	//! 
	//! \details Cette fonction remplit toutes les cellules de la grille avec
	//! l'état passé en argument.
	//! 
	//! L'itération courante est remise à 0.
	//! 
	//! \param state L'état d'initialisation des cellules.
void GOLTeamH::fill(State state)
{
	mData.fill(state, mBorderManagement == GOL::BorderManagement::immutableAsIs);
	modifyBorderIfNecessary(reinterpret_cast<uint8_t*>(mData.data()),
		reinterpret_cast<uint8_t*>(mData.intData()));
	mIteration = 0;
	countLifeStatusCells();
}

//! \brief Mutateur remplissant de façon alternée toutes les cellules de
	//! la grille.
	//! 
	//! \details Cette fonction remplit toutes les cellules de la grille avec
	//! un motif en damier. La première cellule, en haut à gauche, est de 
	//! l'état passée en argument.
	//! 
	//! L'itération courante est remise à 0.
	//! 
	//! \param firstCell L'état de la première cellule.
void GOLTeamH::fillAlternately(State firstCell)
{
	mData.fillAlternately(firstCell, mBorderManagement == GOL::BorderManagement::immutableAsIs);
	modifyBorderIfNecessary(reinterpret_cast<uint8_t*>(mData.data()),
		reinterpret_cast<uint8_t*>(mData.intData()));
	mIteration = 0;
	countLifeStatusCells();
}

//! \brief Mutateur remplissant de façon aléatoire toutes les cellules de
	//! la grille.
	//! 
	//! \details Cette fonction remplit toutes les cellules de la grille avec
	//! un motif aléatoire. Le pourcentage de probabilité d'une cellule d'être
	//! vivante est passé en argument.
	//! 
	//! L'itération courante est remise à 0.
	//! 
	//! \param percentAlive Le pourcentage de probabilité d'une cellule d'être
	//! vivante. La valeur doit être comprise entre 0.0 et 1.0 inclusivement.
void GOLTeamH::randomize(double percentAlive)
{
	mData.randomize(percentAlive, mBorderManagement == GOL::BorderManagement::immutableAsIs);
	modifyBorderIfNecessary(reinterpret_cast<uint8_t*>(mData.data()),
		reinterpret_cast<uint8_t*>(mData.intData()));
	mIteration = 0;
	countLifeStatusCells();
}

//! \brief Mutateur remplissant la grille par le patron passé en argument.
	//! 
	//! \details Cette fonction remplit la grille avec le patron donné. Le 
	//! patron est centré sur la coordonnées (centerX, centerY).
	//! 
	//! Le patron respecte un format précis. Se référer à l'énoncé pour voir 
	//! tous les détails.
	//! 
	//! Si le patron n'est pas valide, la grille n'est pas modifiée et on 
	//! retourne false. Sinon, on retourne vrai.
	//! 
	//! L'itération courante est remise à 0.
	//! 
	//! \param pattern Le patron à appliquer.
	//! \param centerX La coordonnée en x de la grille où se trouve centré le patron.
	//! \param centerY La coordonnée en y de la grille où se trouve centré le patron.
	//! \return true si le patron est valide, false sinon.
	//!

bool GOLTeamH::setFromPattern(std::string const& pattern, int centerX, int centerY)
{
	auto sq = parsePattern(pattern);

	if (!sq.has_value())
		return false;

	fillDataFromPattern(pattern, sq.value(), centerX, centerY);

	mIteration = 0;
	countLifeStatusCells();
	return true;
}

//! \brief Mutateur remplissant la grille par le patron passé en argument.
	//! 
	//! \details Cette fonction est une surcharge utilitaire de la fonction 
	//! setFromPattern(std::string const & pattern, int centerX, int centerY).
	//! Cette version utilise le centre de la grille comme centre du patron.
	//! 
	//! L'itération courante est remise à 0.
	//! 
	//! \param pattern Le patron à appliquer.
	//! \return true si le patron est valide, false sinon.

bool GOLTeamH::setFromPattern(std::string const& pattern)
{
	auto sq = parsePattern(pattern);

	if (!sq.has_value())
		return false;

	size_t centerX = mData.width() / 2 - (sq.value().width / 2);
	size_t centerY = mData.height() / 2 - (sq.value().height / 2);

	fillDataFromPattern(pattern, sq.value(), centerX + 1, centerY + 1);

	mIteration = 0;
	countLifeStatusCells();
	return true;
}

//! \brief Mutateur modifiant la couleur d'un état.
	//! 
	//! \details Cette fonction modifie la couleur d'un état. 
	//! 
	//! Formellement, cette fonction ne modifie rien en soit mais plutôt 
	//! permet de stocker quelle couleur utiliser pour représenter un état 
	//! lors de l'affichage de la grille sur une image.
	//! 
	//! \param state L'état dont on veut modifier la couleur.
	//! \param color La nouvelle couleur de l'état.
void GOLTeamH::setSolidColor(State state, Color const& color)
{
	if (state == State::alive)
		mAliveColor = color;
	else
		mDeadColor = color;

	mColorEncoded &= 0; // Clear
	mColorEncoded |= ((static_cast<uint64_t>(mAliveColor.red) << 16) << 32 * static_cast<uint8_t>(state));
	mColorEncoded |= ((static_cast<uint64_t>(mAliveColor.green) << 8) << 32 * static_cast<uint8_t>(state));
	mColorEncoded |= (static_cast<uint64_t>(mAliveColor.blue) << 32 * static_cast<uint8_t>(state));

	mColorEncoded |= ((static_cast<uint64_t>(mDeadColor.red) << 16) * static_cast<uint8_t>(state));
	mColorEncoded |= ((static_cast<uint64_t>(mDeadColor.green) << 8) * static_cast<uint8_t>(state));
	mColorEncoded |= (static_cast<uint64_t>(mDeadColor.blue) * static_cast<uint8_t>(state));
}


//! \details Cette fonction constitue la partie centrale de la simulation.
	//! Elle est responsable de faire évoluer la grille d'une itération.
	//! 
	//! Cette fonction doit appliquer la logique de l'automate cellulaire en 
	//! tenant compte pour chaque cellule :
	//! - de la grille
	//! - de l'état de chacune des cellules voisines
	//! - de la règle de la simulation
	//! - de la stratégie de gestion de bord
	//! 
	//! Après l'appel de cette fonction, la grille est mise à jour avec le 
	//! nouvel état de chaque cellule suivant l'état précédent. Les statistiques 
	//! doivent tenir compte de cette évolution.

void GOLTeamH::processOneStep()
{
	// Pour des raisons de performance, on accède à la grille interne.
	//
	// Les variables suivantes sont utilisées afin d'éviter des appels de fonctions
	// qui peuvent prendre beaucoup de temps.
	auto const widthNoBorder{ mData.width() - 2 }, heightNoBorder{ mData.height() - 2 };
	auto const offset{ mData.width() };

	size_t neighborsAliveCount{}, aliveCount{};

	// On commence à la première case qui n'est pas dans le border pour sauver une opération
	// par cycle.
	// Pointeur du tableau intermédiaire.
	auto* ptrGridInt{ reinterpret_cast<uint8_t*>(mData.intData()) + (offset + 1) };

	// Pointeur qui se promène en mémoire.
	auto* ptrGrid{ reinterpret_cast<uint8_t*>(mData.data()) };

	for (size_t j{ 1 }; j < heightNoBorder + 1; ++j) {
		for (size_t i{ 1 }; i < widthNoBorder + 1; ++i) {
			neighborsAliveCount = 0;

			// Top
			neighborsAliveCount += *ptrGrid;
			ptrGrid++;
			neighborsAliveCount += *ptrGrid;
			ptrGrid++;
			neighborsAliveCount += *ptrGrid;

			// Milieu
			ptrGrid += offset - 2;
			neighborsAliveCount += *ptrGrid;
			ptrGrid += 2;
			neighborsAliveCount += *ptrGrid;


			// Dessous
			ptrGrid += offset - 2;
			neighborsAliveCount += *ptrGrid;
			ptrGrid++;
			neighborsAliveCount += *ptrGrid;
			ptrGrid++;
			neighborsAliveCount += *ptrGrid;

			// On retourne à une place plus loin qu'à l'origine.
			ptrGrid -= (2 * offset) + 1;
			ptrGridInt++;

			// On prend avantage du fait que GOL::State::alive = 1.
			// 
			// On accède à la bonne partie des bits et on compare si le bit de survie/réanimation est
			// présent. Voir GOLTeamH.cpp pour plus de détails.
			*(ptrGridInt - 1) = static_cast<bool>(
				(mParsedRule >> *(ptrGrid + offset) * 16) & (1u << neighborsAliveCount));

			if (*(ptrGridInt - 1))
				aliveCount++;
		}

		// On saute le border
		ptrGrid += 2;
		ptrGridInt += 2;
	}

	ptrGrid = nullptr;
	ptrGridInt = nullptr;

	modifyBorderIfNecessary(reinterpret_cast<uint8_t*>(mData.data()), reinterpret_cast<uint8_t*>(mData.intData()));
	mData.switchToIntermediate(); // Mise à jour de la grille
	mIteration.value()++;
	mData.setAliveCount(aliveCount);
}


//! \brief Fonction dessinant l'état de la simulation sur une image passée 
	//! en paramètre. 
	//! 
	//! \details Cette fonction dessine l'état courant de chaque cellule sur 
	//! l'image donnée. Chaque pixel de l'image correspond à une cellule de 
	//! la grille de simulation.
	//! 
	//! La couleur de chaque pixel est déterminée par la couleur associée à
	//! l'état de la cellule.
	//! 
	//! L'organisation en mémoire est ce qu'on appel 'Packed Pixel' suivant 
	//! l'ordre ARGB. Ainsi, toutes les données d'un pixel est contiguë en 
	//! mémoire. Les 4 canaux de couleurs sont stockés dans un entier non
	//! signé de 32 bits. Chaque canal est codé sur 8 bits. Le canal alpha est
	//! le plus significatif et le canal bleu est le moins significatif.
	//! 
	//! La variable buffer pointe vers un tableau de pixels de taille 
	//! buffer_size. La taille de l'image est donc buffer_size / sizeof(uint32_t).
	//! 
	//! <pre>
	//! [           32 bits            ]
	//! [8 bits][8 bits][8 bits][8 bits]
	//! [alpha ][red   ][green ][blue  ]
	//! [alpha ][rouge ][vert  ][bleu  ]
	//! </pre>
	//! 
	//! Vous pouvez considérer que l'image a été créée avec le format de 
	//! Qt QImage::Format_ARGB32.
	//! 
	//! Il est attendu que vous fassiez une validation minimum des intrants.
	//! 
	//! \param buffer Le tableau de pixels de l'image.
	//! \param buffer_size La taille du tableau de pixels.

void GOLTeamH::updateImage(uint32_t* buffer, size_t buffer_size) const
{
	if (buffer == nullptr)
		return;

	auto* s_ptr{ buffer }, * e_ptr{ buffer + buffer_size };

	// Pointeur qui se promène en mémoire.
	auto* ptrGrid{ reinterpret_cast<uint8_t*>(mData.data()) };

	// On itère sur chaque éléments du tableau et on associe la couleur.
	while (s_ptr < e_ptr) {
		*s_ptr &= 0;						// Clear
		*s_ptr |= MAX_ALPHA << 24;			// Alpha = 255
		*s_ptr |= mColorEncoded >> (32 * (*ptrGrid));

		s_ptr++;
		ptrGrid++;
	}

	s_ptr = nullptr;
	ptrGrid = nullptr;
}

unsigned char GOLTeamH::convertCharToNumber(const char c)
{
	return (c - 48);
}

std::optional<GOLTeamH::sizeQueried> GOLTeamH::parsePattern(std::string const& pattern)
{
	// \[ -> on match le caractère [
	// (\d+) -> on match plusieurs caractères de 0-9
	std::regex regexp(R"(\[(\d+)x(\d+)\](\d+))", std::regex_constants::icase);
	std::smatch m;

	if (std::regex_search(pattern, m, regexp))
		return GOLTeamH::sizeQueried{ .width = std::stoull(m[1]), .height = std::stoull(m[2]), .pos = m[3] };
	else
		return std::nullopt;
}

void GOLTeamH::fillDataFromPattern(std::string const& pattern, sizeQueried& sq,
	int centerX, int centerY)
{
	// Remplissage de la grille aux positions spécifiées par le patron
	for (size_t y = 0; y < sq.height; ++y) {
		for (size_t x = 0; x < sq.width; ++x) {
			// TODO: Check si in bounds et vérifie que ça répare le problème de quand on ferme, erreur.
			if (centerX - sq.width/2 + x <0 
				|| centerX - sq.width / 2 + x > mData.width() 
				|| centerY - sq.height / 2 + x <0 
				|| centerY - sq.height / 2 + x > mData.height()) {
				continue;
			}
			
			State cellState = (sq.pos[(y * sq.width) + x] == '0') ? State::dead : State::alive;
			mData.setAt(centerX + x, centerY + y, cellState);
		}
	}
}

void GOLTeamH::countLifeStatusCells()
{
	size_t aliveCount{};
	auto* s_ptr{ mData.data() }, * e_ptr{ mData.data() + mData.size() };

	while (s_ptr < e_ptr) {
		if (*s_ptr == State::alive) {
			aliveCount++;
		}
		s_ptr++;
	}
	mData.setAliveCount(aliveCount);
}


// TODO: combiner avec fillBorder
void GOLTeamH::modifyBorderIfNecessary(uint8_t* ptrGrid, uint8_t* ptrGridInt)
{
	auto bm = mBorderManagement.value_or(BorderManagement::immutableAsIs);
	auto width{ mData.width() }, height{ mData.height() };	// Pour éviter des appels de fonctions trop souvent.
	auto rule{ mParsedRule };								// Pour la capture du lambda.

	if (mBorderManagement.value() == GOL::BorderManagement::immutableAsIs ||
		bm == GOL::BorderManagement::foreverAlive ||
		bm == GOL::BorderManagement::foreverDead)
		return;

	auto* e_ptr = ptrGrid + (width - 1);

	// Lambda pour une opération courante.
	auto getFutureStatus = [rule](size_t count, uint8_t* ptrGrid) {
		return static_cast<bool>((rule >> *(ptrGrid) * 16) & (1u << count));
		};

	// TOP
	while (ptrGrid < e_ptr) {
		*ptrGridInt = getFutureStatus(countNeighbors(ptrGrid, bm), ptrGrid);

		ptrGrid++;
		ptrGridInt++;
	}

	// DROITE
	e_ptr += width * (height - 1);
	while (ptrGrid < e_ptr) {
		*ptrGridInt = getFutureStatus(countNeighbors(ptrGrid, bm), ptrGrid);

		ptrGrid += width;
		ptrGridInt += width;
	}

	// DESSOUS
	e_ptr -= (width - 1);
	while (ptrGrid > e_ptr) {
		*ptrGridInt = getFutureStatus(countNeighbors(ptrGrid, bm), ptrGrid);

		ptrGrid--;
		ptrGridInt--;
	}

	// GAUCHE
	e_ptr -= width * (height - 1);
	while (ptrGrid > e_ptr) {
		*ptrGridInt = getFutureStatus(countNeighbors(ptrGrid, bm), ptrGrid);

		ptrGrid -= width;
		ptrGridInt -= width;
	}
}

size_t GOLTeamH::countNeighbors(uint8_t* ptrGrid, BorderManagement bm) const
{
	auto width{ mData.width() }, height{ mData.height() };	// Pour éviter des appels de fonctions trop souvent.
	size_t neighborsAliveCount{};
	auto* tempPtr{ ptrGrid - (width + 1) };

	// Petit lambda pour simplifier les opérations.
	auto putInBounds = [ptrGrid, width, height, bm](uint8_t* ptr, uint8_t const* cellPtr) {
		if (ptr < ptrGrid)
			ptr += width * (bm == GOL::BorderManagement::mirror) ? 2 : (height - 1);
		else if (ptr > ptrGrid + (width * height))
			ptr -= width * (bm == GOL::BorderManagement::mirror) ? 2 : (height - 1);

		if ((cellPtr - ptrGrid) % width == 0)
			ptr += (bm == GOL::BorderManagement::mirror) ? 2 : (width - 1);
		else if ((cellPtr - ptrGrid) % width == width - 1)
			ptr -= (bm == GOL::BorderManagement::mirror) ? 2 : (width - 1);
		return ptr;
		};

	neighborsAliveCount += *(putInBounds(tempPtr, ptrGrid));
	tempPtr++;
	neighborsAliveCount += *(putInBounds(tempPtr, ptrGrid));
	tempPtr++;
	neighborsAliveCount += *(putInBounds(tempPtr, ptrGrid));

	tempPtr += (width - 2);
	neighborsAliveCount += *(putInBounds(tempPtr, ptrGrid));
	tempPtr += 2;
	neighborsAliveCount += *(putInBounds(tempPtr, ptrGrid));


	tempPtr += (width - 2);
	neighborsAliveCount += *(putInBounds(tempPtr, ptrGrid));
	tempPtr++;
	neighborsAliveCount += *(putInBounds(tempPtr, ptrGrid));
	tempPtr++;
	neighborsAliveCount += *(putInBounds(tempPtr, ptrGrid));

	return neighborsAliveCount;
}
