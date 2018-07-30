#include "ui/rootwindow.h"
#include <QApplication>


int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    app.setApplicationDisplayName("Mpi3 Media Player");
    app.setApplicationName("mpi3desktop");
    app.setApplicationVersion("1.0");

    Mpi3RootDesktop wnd;
    wnd.initialize();
    wnd.show();

    return app.exec();
}
