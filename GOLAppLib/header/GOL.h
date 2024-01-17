#pragma once
#ifndef GOL_INTERFACE_H
#define GOL_INTERFACE_H


#include <string>
#include <vector>
#include <optional>


//
//  ___ __  __ ____   ___  ____ _____  _    _   _ _____ 
// |_ _|  \/  |  _ \ / _ \|  _ \_   _|/ \  | \ | |_   _|
//  | || |\/| | |_) | | | | |_) || | / _ \ |  \| | | |  
//  | || |  | |  __/| |_| |  _ < | |/ ___ \| |\  | | |  
// |___|_|  |_|_|    \___/|_| \_\|_/_/   \_\_| \_| |_|  
//                                                        
// 
// Attention, vous ne pouvez pas modifier ce fichier.
// 



//! 
//! \brief Moteur de simulation d'un automate cellulaire à 2 dimensions.
//!
//! \details La classe GOL représente une interface standardisée visant 
//! l'implémentation complète et suffisante d'un engin de simulation 
//! d'un automate cellulaire à 2 dimensions.
//! 
//! Cette classe abstraite ne fait rien et ne possède aucun attribut. Elle ne 
//! constitue qu'un modèle standardisé pouvant être facilement utilisé pour 
//! réaliser une simulation.
//! 
//! La classe possède :
//!  - la définition des types utilitaires
//!  - la définition et l'implémentation de :
//!		- le constructeur par défaut
//!     - un constructeur personnalisé (cette définition est absolument inutile
//!		  et en temps normal elle serait absente de ce fichier, sa présence 
//!       n'est qu'à titre d'exemples pédagogiques)
//!		- le destructeur virtuel
//!  - la définition de plusieurs fonctions abstraites (virtuelles pures).
//! 
//! L'approche utilisée est donc un parfait exemple d'un développement 
//! modulaire s'appuyant sur le polymorphisme. Néanmoins, pour rester critique, 
//! cette classe présente un défaut de conception du fait qu'elle réalise 
//! beaucoup trop de tâches à elle seule. Un découpage mieux structuré serait 
//! préférable considérant un projet d'envergure. La forme actuelle de cette 
//! conception vise à mettre l'emphase du développement sur les objetifs du 
//! projet sans être dérangé par d'autres aspects : conception et implémentation 
//! de structures de données et d'algorithmes.
//! 
//! Puisque cette classe est abstraite, elle est destiné à être héritée afin 
//! que vous implémentiez **entièrement** un engin de type automate cellulaire 
//! répondant strictement à l'interface présentée.
//! 
//! Éléments importants :
//!  - Vous devez implémenter entièrement le moteur de simulation.
//!  - La grille est 2d de dimensions finies.
//!  - **En tout temps la classe doit être dans un état 
//!    consistant.** Peu importe l'ordre d'appel des fonctions. De plus, 
//!    il doit être possible d'appeler n'importe quelle fonction et que 
//!    son effet soit immédiat (dans la mesure de la logique).
//!  - À l'initialisation de votre engin, la configuration par défaut est :
//!    - grille de 100 x 100
//!    - toutes les cellules sont mortes
//!    - règle de Conway : `B3/S23`
//!    - stratégie de gestion de bord : BorderManagement::foreverDead
//!    - itération courante : 0
//!	   - les couleurs sont noires pour les cellules mortes et blanches
//!      pour les cellules vivantes
//!  - Vous devez avoir une représentation interne indépendante de la 
//!    représentation à l'écran. 
//!  - Vous devez être en mesure de gérer n'importe quelle règle. Les règles 
//!    sont définies par une chaîne de caractère suivant ce format : 
//!    `B###.../S###...` 
//!  - Vous devez être en mesure de gérer les effets de bord selon 
//!    5 stratégies différentes :								 
//!		1. immuable tel quel
//!		2. mort pour toujours
//!		3. vivant pour toujours
//!		4. téléportation
//!		5. miroir
//!  - Vous devez être en mesure d'initialiser la grille de plusieurs façons :
//!		1. remplir avec une valeur uniforme (mort ou vivant)
//!		2. remplir en forme de damier (en pouvant préciser la valeur de 
//!         la première cellule : mort ou vivant) 
//!		3. remplir aléatoirement avec un pourcentage de probabilité, pour 
//!         chaque cellule, d'être vivant
//!     4. mettre un patron centré sur la grille
//!     5. manuellement, cellule par cellule, avec la fonction setState(...)
//!  - La fonction GOL::processOneStep doit faire un pas de 
//!    simulation. 
//!  - Vous devez utiliser le type GOL::State pour représenter chaque cellule 
//!    de la simulation utilise, .
//!  - Vous devez être capable de produire une représentation visuelle 
//!    de la représentation interne. La fonction GOL::updateImage 
//!    vous passe le _buffer_ où se trouve l'image de sortie. C'est dans 
//!    cette zone mémoire que vous devez dessiner la représentation visuelle 
//!    de la simulation.
//!	 - Plusieurs mutateurs réinitialisent le compteur d'itération à 0.
//!     - Voici ceux qui réinitialisent le compteur d'itération :
//!			- GOL::resize
//!			- GOL::fill
//!			- GOL::fillAlternately
//!			- GOL::setFromPattern
//!			- GOL::randomize
//!			- GOL::setRule
//!			- GOL::setBorderManagement
//!     - Ce dernier laisse inchangé le compteur d'itération :
//!			- GOL::setState
//!  - À tout moment, il doit être possible de questionner la simulation afin 
//!    d'obtenir les informations contenues dans la structure GOL::Statistics. 
//!    On remarque que toutes les informations sont stockées dans un 
//!    std::optional. Ainsi, si vous n'avez pas la réponse, vous pouvez 
//!    simplement laissé la valeur indéterminée.
//!  - Vous devez implémenter une autre classe qui s'occupe de gérer une 
//!    structure de données 2d. Cette classe, nommée Grid, est une 
//!    encapsulation dirigée d'une matrice 2d. Voir plus bas dans cette 
//!    page pour les détails.
//!
class GOL
{
public:
	//    ____    __  __ _       _ _   _                   _             _                         
	//   |  _ \  /_/ / _(_)_ __ (_) |_(_) ___  _ __     __| | ___  ___  | |_ _   _ _ __   ___  ___ 
	//   | | | |/ _ \ |_| | '_ \| | __| |/ _ \| '_ \   / _` |/ _ \/ __| | __| | | | '_ \ / _ \/ __|
	//   | |_| |  __/  _| | | | | | |_| | (_) | | | | | (_| |  __/\__ \ | |_| |_| | |_) |  __/\__ \
	//   |____/ \___|_| |_|_| |_|_|\__|_|\___/|_| |_|  \__,_|\___||___/  \__|\__, | .__/ \___||___/
	//                                                                       |___/|_|              
	// 
	//! \brief Le type représentant l'état d'une cellule.
	//! \details L'état d'une cellule peut être mort ou vivant.
	enum class State : uint8_t {
		dead = 0,  //!< L'état mort.
		alive = 1, //!< L'état vivant.
	};
	//
	//! \brief Le type représentant le nombre d'itération de la simulation.
	//! \details Le type choisi est un entier non signé de 32 bits.
	using IterationType = uint32_t;
	// 
	//! \brief Le type représentant la stratégie de gestion de bord.
	//! \details L'algorithme de simulation doit gérer les effets de bord. 
	//! C'est-à-dire que les calculs de voisinage doivent être adaptés pour 
	//! les cellules situées sur les bords de la grille.
	//! 
	//! 5 modes de gestion sont possibles.
	enum class BorderManagement : uint8_t {
		immutableAsIs = 0,	//!< Les cellules du contour ne sont jamais modifiées. Elles sont laissées dans leur état courant.
		foreverDead,		//!< Les cellules du contour ne sont jamais modifiées. Elles sont toujours mortes.
		foreverAlive,		//!< Les cellules du contour ne sont jamais modifiées. Elles sont toujours vivantes.
		warping,			//!< Les cellules du contour sont évaluées. On utilise les cellules du côté opposé de la grille pour les cellules extérieures.
		mirror,				//!< Les cellules du contour sont évaluées. On utilise les cellules du côté opposé de la cellule pour les cellules extérieures.
	};
	//
	//! \brief Le type représentant un canal de couleur.
	//! \details Le type choisi est un entier non signé de 8 bits.
	using ColorChannelType = uint8_t;
	//
	//! \brief Le type représentant une couleur.
	//! \details Le type choisi est une structure contenant 3 canaux de couleur.
	struct Color {
		ColorChannelType red = 0;	//!< Le canal de couleur rouge.
		ColorChannelType green = 0; //!< Le canal de couleur verte.
		ColorChannelType blue = 0;  //!< Le canal de couleur bleue.
	};
	//
	//! \brief Le type représentant des données statistiques de la simulation.
	//! \details Cette structure encapsule deux groupes d'information sur la simulation en cours :
	//!  - les informations de la configuration de la simulation
	//!		- règle
	//! 	- stratégie de gestion de bord
	//! 	- largeur de la grille
	//! 	- hauteur de la grille
	//! 	- nombre total de cellules
	//!  - les informations sur le déroulement de la simulation
	//! 	- itération courante (combien de pas de simulation)
	//!     - nombre total de cellules mortes en valeur absolue
	//!     - nombre total de cellules vivantes en valeur absolue
	//!     - nombre total de cellules mortes en valeur relative
	//!     - nombre total de cellules vivantes en valeur relative
	//!		- tendance de la variation entre le nombre de cellules mortes et vivantes en valeur absolue
	//!     - tendance de la variation entre le nombre de cellules mortes et vivantes en valeur relative
	struct Statistics {
		std::optional<std::string> rule;					//!< La règle de la simulation exprimé par la chaîne de caractères de format "B###/S###".
		std::optional<BorderManagement> borderManagement;   //!< La stratégie de gestion de bord.
		std::optional<size_t> width;						//!< La largeur de la grille.
		std::optional<size_t> height;			            //!< La hauteur de la grille.	
		std::optional<size_t> totalCells;	                //!< Le nombre total de cellules.
		std::optional<IterationType> iteration;             //!< Le nombre d'itération de la simulation.
		std::optional<size_t> totalDeadAbs;					//!< Le nombre total de cellules mortes en valeur absolue.
		std::optional<size_t> totalAliveAbs;                //!< Le nombre total de cellules vivantes en valeur absolue.
		std::optional<float> totalDeadRel;                  //!< Le nombre total de cellules mortes en valeur relative du nombre total de cellules.
		std::optional<float> totalAliveRel;		            //!< Le nombre total de cellules vivantes en valeur relative du nombre total de cellules.	
		std::optional<int> tendencyAbs;				        //!< La tendance de la variation entre le nombre de cellules mortes et vivantes en valeur absolue. 
		std::optional<float> tendencyRel;		            //!< La tendance de la variation entre le nombre de cellules mortes et vivantes en valeur relative.	
	};
	//
	//! \brief Le type représentant les informations permettant d'identifier les auteurs de l'implémentation.
	//! \details Cette structure encapsule les informations permettant d'identifier les auteurs de l'implémentation.
	struct Author {
		std::string lastName;		//!< Le nom de famille de l'auteur.
		std::string firstName;		//!< Le prénom de l'auteur.
		std::string studentEmail;    //!< L'adresse courriel de l'auteur. On désire l'adresse courriel étudiante.
	};
	//
	//! \brief Le type représentant les informations documentant l'implémentation.
	//! \details Cette structure encapsule les informations documentant 
	//! l'implémentation. Ces informations ne sont pas techniquement nécessaires 
	//! au déroulement du progamme mais servent de mécanisme à l'interface usager 
	//! et de stratégie de remise. 
	//! 
	//! Cette structure doit être rempli adéquatement car elle est un peu 
	//! l'équivalent du rapport. Voir les détails dans la section rapport de 
	//! l'énoncé.
	struct ImplementationInformation {
		std::string title;							//!< Le titre de l'implémentation.
		std::vector<Author> authors;				//!< Les auteurs de l'implémentation.
		std::vector<std::string> answers;			//!< Les réponses aux questions du laboratoire.
		std::vector<std::string> optionnalComments;	//!< Des commentaires optionnels.
	};



	//
	//     ____                _                   _                           __  ____            _                   _                  
	//    / ___|___  _ __  ___| |_ _ __ _   _  ___| |_ ___ _   _ _ __ ___     / / |  _ \  ___  ___| |_ _ __ _   _  ___| |_ ___ _   _ _ __ 
	//   | |   / _ \| '_ \/ __| __| '__| | | |/ __| __/ _ \ | | | '__/ __|   / /  | | | |/ _ \/ __| __| '__| | | |/ __| __/ _ \ | | | '__|
	//   | |__| (_) | | | \__ \ |_| |  | |_| | (__| ||  __/ |_| | |  \__ \  / /   | |_| |  __/\__ \ |_| |  | |_| | (__| ||  __/ |_| | |   
	//    \____\___/|_| |_|___/\__|_|   \__,_|\___|\__\___|\__,_|_|  |___/ /_/    |____/ \___||___/\__|_|   \__,_|\___|\__\___|\__,_|_|   
	//                                                                                                                                    
	// 
	// Les classes héritant de GOL doivent réaliser :
	//  - le constructeur par défaut : _class_()
	//  - le constructeur d'initialisation proposé : _class_(size_t width, size_t height, State defaultState = State::dead)
	//  - le destructeur : ~_class_()
	// 
	//! \brief Constructeur par défaut.
	GOL() = default;
	// 
	//! \brief Constructeur d'initialisation personnalisé.
	GOL(size_t width, size_t height, State defaultState = State::dead) {}
	// 
	//! Destructeur.
	virtual ~GOL() = default;
	// 
	// Les 4 autres fonctions spéciales sont laissées à votre discrétion.
	// Toutefois, un simple -delete- est suffisant pour ce premier laboratoire.
	// _class_(_class_ const &) = delete;
	// _class_(_class_ &&) = delete;
	// _class_& operator=(_class_ const &) = delete;
	// _class_& operator=(_class_ &&) = delete;


	// 
	//       _                                             
	//      / \   ___ ___ ___  ___ ___  ___ _   _ _ __ ___ 
	//     / _ \ / __/ __/ _ \/ __/ __|/ _ \ | | | '__/ __|
	//    / ___ \ (_| (_|  __/\__ \__ \  __/ |_| | |  \__ \
	//   /_/   \_\___\___\___||___/___/\___|\__,_|_|  |___/
	//                                                                                                           
	//
	//! \brief Accesseur retournant la largeur de la grille de simualtion.
	//! 
	//! \return La largeur de la grille.
	virtual size_t width() const = 0;
	//
	//! \brief Accesseur retournant la hauteur de la grille de simualtion.
	//! 
	//! \return La hauteur de la grille.
	virtual size_t height() const = 0;
	//
	//! \brief Accesseur retournant le nombre total de cellules de la grille 
	//! de simualtion.
	//! 
	//! \return Le nombre total de cellules de la grille.
	virtual size_t size() const = 0;
	//
	//! \brief Accesseur retournant l'état d'une cellule.
	//! 
	//! \details Cette fonction ne valide pas les entrées pour une raison de 
	//! performance.
	//! L'origine est le coin supérieur gauche de la grille.
	//! 
	//! \param x La coordonnée en x de la cellule.
	//! \param y La coordonnée en y de la cellule.
	//! \return L'état de la cellule.
	virtual State state(int x, int y) const = 0;
	//
	//
	//! \brief Accesseur retournant la chaîne de caractères correspondant à 
	//! la règle courante.
	//! 
	//! \details La chaîne de caractères est de la forme "B###/S###".
	//! 
	//! \return La chaîne de caractères correspondant à la règle courante selon 
	//! le format `B###.../S###...`.
	virtual std::string rule() const = 0;
	//
	//
	//! \brief Accesseur retournant la stratégie courante de gestion des bords.
	//! 
	//! \return La stratégie courante de gestion des bords.
	virtual BorderManagement borderManagement() const = 0; 
	//
	//
	//! \brief Accesseur retournant la couleur d'un état.
	//! 
	//! \details Cette fonction retourne la couleur associée à l'état passé
	//! en argument.
	//! 
	//! \param state L'état dont on veut connaître la couleur.
	//! \return La couleur associée à l'état.
	virtual Color color(State state) const = 0;
	//
	//
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
	virtual Statistics statistics() const = 0;
	//
	// 
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
	virtual ImplementationInformation information() const = 0;
	//
	//   
	//    __  __       _        _                      
	//   |  \/  |_   _| |_ __ _| |_ ___ _   _ _ __ ___ 
	//   | |\/| | | | | __/ _` | __/ _ \ | | | '__/ __|
	//   | |  | | |_| | || (_| | ||  __/ |_| | |  \__ \
	//   |_|  |_|\__,_|\__\__,_|\__\___|\__,_|_|  |___/
	// 
	// 
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
	virtual void resize(size_t width, size_t height, State defaultState = State::dead) = 0;
	//
	//
	//! \brief Mutateur modifiant la règle de la simulation.
	//! 
	//! \details Cette fonction s'assure que la chaîne de caractères est valide 
	//! et assigne la nouvelle règle pour que les prochaines évolutions 
	//! l'utilisent. 
	//! 
	//! Si la règle est valide, on assigne la novuelle règle, l'ancienne règle 
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
	virtual bool setRule(std::string const & rule) = 0;
	//
	//
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
	//! 
	//! <pre>
	//!     +---+---+---+---+---+ 
	//!   x | 	|	|	|	| X |
	//!     +---+---+---+---+---+
	//!   y |-*-|   |	|	| Y |
	//!     +---+---+---+---+---+
	//!   z |	|	|	|	| Z |
	//! </pre>
	//! 
	//! BorderManagement::mirror utilise les voisins opposés par rapport à la 
	//! **cellule**.
	//! 
	//! <pre>
	//!     +---+---+---+---+---+ 
	//!   x | 	| X |	|	|   |
	//!     +---+---+---+---+---+
	//!   y |-*-| Y |	|	|   |
	//!     +---+---+---+---+---+
	//!   z |	| Z |	|	|   |
	//! </pre>
	//! 
	//! \param borderManagement La nouvelle stratégie de gestion de bord.
	virtual void setBorderManagement(BorderManagement borderManagement) = 0;
	//
	// 
	//! \brief Mutateur modifiant l'état d'une cellule de la grille.
	//! 
	//! \details Cette fonction ne valide pas les entrées pour une raison de 
	//! performance. L'origine est le coin supérieur gauche de la grille.
	//! 
	//! \param x La coordonnée en x de la cellule.
	//! \param y La coordonnée en y de la cellule.
	//! \param state Le nouvel état de la cellule.
	virtual void setState(int x, int y, State state) = 0;
	//
	//
	//! \brief Mutateur remplissant de façon uniforme toutes les cellules de 
	//! la grille.
	//! 
	//! \details Cette fonction remplit toutes les cellules de la grille avec
	//! l'état passé en argument.
	//! 
	//! L'itération courante est remise à 0.
	//! 
	//! \param state L'état d'initialisation des cellules.
	virtual void fill(State state = State::dead) = 0;
	//
	//
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
	virtual void fillAlternately(State firstCell = State::dead) = 0;
	//
	//
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
	virtual void randomize(double percentAlive = 0.5) = 0;
	//
	//
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
	virtual bool setFromPattern(std::string const & pattern, int centerX, int centerY) = 0;
	//
	//
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
	virtual bool setFromPattern(std::string const & pattern) = 0;
	//
	//
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
	virtual void setSolidColor(State state, Color const & color) = 0;


	//
	//
	//    __  __           _ _  __ _           _                      
	//   |  \/  | ___   __| (_)/ _(_) ___ __ _| |_ ___ _   _ _ __ ___ 
	//   | |\/| |/ _ \ / _` | | |_| |/ __/ _` | __/ _ \ | | | '__/ __|
	//   | |  | | (_) | (_| | |  _| | (_| (_| | ||  __/ |_| | |  \__ \
	//   |_|  |_|\___/ \__,_|_|_| |_|\___\__,_|\__\___|\__,_|_|  |___/
	//                                                                
	//! \brief Fonction effectuant une itération de la simulation.
	//! 
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
	virtual void processOneStep() = 0;
	//
	// 
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
	virtual void updateImage(uint32_t * buffer, size_t buffer_size) const = 0;

};







//
//    _                            _              _   
//   (_)_ __ ___  _ __   ___  _ __| |_ __ _ _ __ | |_ 
//   | | '_ ` _ \| '_ \ / _ \| '__| __/ _` | '_ \| __|
//   | | | | | | | |_) | (_) | |  | || (_| | | | | |_ 
//   |_|_| |_| |_| .__/ \___/|_|   \__\__,_|_| |_|\__|
//               |_|                                  
// 
// Attention, vous devez implémenter et utiliser une classe Grid.
// 
// L'ébauche sous forme de commentaire plus bas est un guide 
// semi-contraignant.
// 
// Toutefois :
//  - Vous DEVEZ implémenter l'interface présentée.
//  - Vous POUVEZ ajouter des éléments comme vous le voulez.
//
//  > Autrement dit, l'interface proposé est minimum. 
//  > NÉANMOINS, vous devez garder en tête l'intention générique de 
//    cette classe et ne pas la spécialiser pour le contexte du projet 
//    de GOL. Par exemple, si on faisait un copier/coller du code et 
//    qu'on modifiait le CellType pour autre chose (un Vect2D par exemple), 
//    alors, tout fonctionnerait et la classe serait encore pertinente.
//    D'ailleurs, vous remarquez que c'est le seul endroit où il existe un 
//    lien entre cette classe et le projet courant.
//
// De plus, vous devez gérer manuellement l'allocation dynamique de la mémoire.
//
//
//	class Grid																		// Classe facilitant la gestion d'un tableau dynamique 2d.
//	{																				// 
//	public:																			// 
//		// Définition des types														//
//		using CellType = GOL::State;												// Correspond au type fondamental de chaque cellule de la grille. 
//		using DataType = ...;														// Correspond à la grille, au conteneur de cellules. À vous de déterminer la structure de données à utiliser.
//																					//
//		// Définition des cosntructeurs / destructeur								//
//		Grid();																		// 
//		Grid(size_t width, size_t height, CellType initValue = CellType{});			// 
//		Grid(Grid const&) = delete;													// 
//		Grid(Grid&&) = delete;														// 
//		Grid& operator=(Grid const&) = delete;										// 
//		Grid& operator=(Grid&&) = delete;											// 
//		~Grid();																	// 
//																					//
//		// Accesseurs et mutateurs de la grille										// 
//		size_t width() const;														// Accesseur retournant la largeur de la grille.
//		size_t height() const;														// Accesseur retournant la hauteur de la grille.
//		size_t size() const;														// Accesseur retournant le nombre de cellule de la grille.
//																					// 
//		void resize(size_t width, size_t height, CellType initValue = CellType{});	// Mutateur modifiant la taille de la grille et initialise le contenu par la valeur spécifiée.
//																					//
//		// Accesseurs et mutateurs des cellules										//
//		CellType value(int column, int row) const;									// Accesseur retournant la valeur d'une cellule à une certaine coordonnée. Pour une raison de performance, cette fonction NE VALIDE PAS ses entrées. Autrement dit, c'est la responsabilité du programmeur utilisateur de faire ses validations, au risque de 'crasher' le programme.
//		void setValue(int column, int row, CellType value);							// Mutateur modifiant la valeur d'une cellule à une certaine coordonnée. Pour une raison de performance, cette fonction NE VALIDE PAS ses entrées. Autrement dit, c'est la responsabilité du programmeur utilisateur de faire ses validations, au risque de 'crasher' le programme.
//																					// 
//		std::optional<CellType> at(int column, int row) const;						// Accesseur retournant la valeur d'une cellule à une certaine coordonnée. Cette fonction VALIDE ses entrées et retourne un optional nul si la coordonnée est invalide. 
//		void setAt(int column, int row, CellType value);							// Mutateur modifiant la valeur d'une cellule à une certaine coordonnée. Cette fonction VALIDE ses entrées et ne fait rien si la coordonnée est invalide. 
//																					// 
//		// Accesseurs du "buffer" de la grille.										//
//		DataType const & data() const;												// Accesseur en lecture seule sur le "buffer" de la grille.
//		DataType & data();															// Accesseur en lecture/écriture sur le "buffer" de la grille.
//																					// 
//	private:																		// 
//		// ...																		// Il y a des attributs essentiels au fonctionnement de cette classe.
//		// ...																		// À vous de les déterminer. 
//		// ...																		// 
//	};																				//  
//																					// 
//																					// 
//																					// 



#endif // GOL_INTERFACE_H