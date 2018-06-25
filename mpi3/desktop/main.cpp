#include "ui/mroot.h"
#include <QApplication>

#include <QDebug>
#include "util/medialib.h"


int main(int argc, char *argv[])
{
    QString a = QString::fromStdString(generatePID());
    qDebug() << a;


//    QApplication app(argc, argv);
//    Mpi3RootDesktop wnd;

//    wnd.initialize();
//    wnd.show();

//    return app.exec();
}
