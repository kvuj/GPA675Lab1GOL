
#include <QtWidgets/QApplication>

#include "GOLApp.h"




int main(int argc, char* argv[])
{
    QApplication application(argc, argv);
    
    GOLApp window;
    
    // 
    // Ajouter votre implémentation ici. 
    // Évidemment, n'oubliez pas de faire l'inclusion du fichier contenant 
    // votre classe.
    // 
    // Par exemple :
	// 
	// window.addEngine(new MonImplementationGOL);
    // 

    window.show();
    return application.exec();
}
