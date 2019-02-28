#if defined(MPI3_BUILD_DESKTOP)
#include "desktop/mroot.h"
typedef MRootDesktop MRootWindow;
#elif defined(MPI3_BUILD_DEVICE)
#include "device/mroot.h"
typedef MRootDevice MRootWindow;
#endif


#ifndef MPI3_MESSAGE_PATTERN
//#   define MPI3_MESSAGE_PATTERN "[%{time yyyyMMdd h:mm:ss.zzz}] %{if-debug}DBUG:%{endif}%{if-info}INFO:%{endif}%{if-warning}WARN:%{endif}%{if-critical}CRIT:%{endif}%{if-fatal}FATAL_ERROR:%{endif} <%{function}> %{message}"
#   define MPI3_MESSAGE_PATTERN "%{if-debug}DBUG:%{endif}%{if-info}INFO:%{endif}%{if-warning}WARN:%{endif}%{if-critical}CRIT:%{endif}%{if-fatal}FATAL_ERROR:%{endif} <%{function}> %{message}"
#endif


#include <QApplication>
#include <iostream>


QCoreApplication *initialize(int &argc, char *argv[])
{
    for(int i = 0; i < argc; i++){
        if(!qstrcmp(argv[i], "--no-ui")){
            return new QCoreApplication(argc, argv);
        }
    }

    return new QApplication(argc, argv);
}


int main(int argc, char *argv[])
{
    QScopedPointer<QCoreApplication> app(initialize(argc, argv));
    qSetMessagePattern(MPI3_MESSAGE_PATTERN);

    if(qobject_cast<QApplication*>(app.data())){
        MRootWindow *wnd = new MRootWindow();
        wnd->initialize();
        wnd->show();
    }
    else {
        std::cerr << "windowless mode not yet supported" << std::endl;
        return 1;
    }

    return app->exec();
}
