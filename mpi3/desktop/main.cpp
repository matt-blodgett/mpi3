#include "mroot.h"
#include "mglobal.h"
#include <QApplication>


int main(int argc, char *argv[]){

    Mpi3::register_global_metatypes();

    QApplication app(argc, argv);
    app.setApplicationDisplayName("Mpi3 Media Player");
    app.setApplicationName("mpi3desktop");
    app.setApplicationVersion("1.0");

    MRootDesktop wnd;
    wnd.initialize();
    wnd.show();

    return app.exec();
}
