#include <QtWidgets/QApplication>

#include "GOLApp.h"
#include "GOLTeamH.h"


//TEST DE COMMIT

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);
    
    GOLApp window;
    window.addEngine(new GOLTeamH());




    window.show();
    return application.exec();
}
