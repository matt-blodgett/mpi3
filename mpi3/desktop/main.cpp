#include "mroot.h"
#include <QApplication>

#include "util/mglobal.h"

int main(int argc, char *argv[]){

    qRegisterMetaType<Mpi3::MediaState>();
    qRegisterMetaType<Mpi3::EngineState>();
    qRegisterMetaType<Mpi3::ErrorState>();

    QApplication app(argc, argv);
    app.setApplicationDisplayName("Mpi3 Media Player");
    app.setApplicationName("mpi3desktop");
    app.setApplicationVersion("1.0");

    Mpi3RootDesktop wnd;
    wnd.initialize();
    wnd.show();

    return app.exec();
}
