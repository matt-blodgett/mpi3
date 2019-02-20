#include "mroot.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationDisplayName("Mpi3 Media Player");
    app.setApplicationName("mpi3device");
    app.setApplicationVersion("1.0");

    MRootDevice wnd;
    wnd.initialize();
    wnd.show();

    return app.exec();
}
