#include "ui/mroot.h"
#include <QApplication>


int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    Mpi3RootDesktop wnd;

    wnd.initialize();
    wnd.show();

    return app.exec();
}
