#if defined(MPI3_BUILD_DESKTOP)
#include "desktop/mroot.h"
typedef MRootDesktop MRootWindow;
#elif defined(MPI3_BUILD_DEVICE)
#include "device/mroot.h"
typedef MRootDevice MRootWindow;
#endif


#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MRootWindow wnd;   
    wnd.initialize();
    wnd.show();

    return app.exec();
}
