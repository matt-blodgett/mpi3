//#include "ui/mroot.h"
//#include <QApplication>
#include <QDebug>

#include "util/medialib.h"

void test(Mpi3Library *m, QString pid)
{
    Mpi3Song *s = m->songs[pid];
    qDebug() << s->name;
}

int main()
{
    Mpi3Library *mpi3Lib = new Mpi3Library;

    Mpi3Song *song_1 = mpi3Lib->addSong();
    song_1->name = "Me, Myself and I";
    song_1->artist = "G-Eazy";

    Mpi3Song *song_2 = mpi3Lib->addSong();
    song_2->name = "Been On";
    song_2->artist = "G-Eazy";

    qDebug() << mpi3Lib->songs.values().length();

    test(mpi3Lib, song_1->pid());

    mpi3Lib->save("C:\\Users\\mablodgett\\Desktop\\lib.txt");
    return 0;

}

//int main(int argc, char *argv[])
//{
//    QApplication app(argc, argv);
//    Mpi3RootDesktop wnd;

//    wnd.initialize();
//    wnd.show();

//    return app.exec();
//}
