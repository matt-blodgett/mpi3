#include "ui/mroot.h"
#include <QApplication>

#include <QDebug>
//#include "util/medialib.h"


//void load_test()
//{

//    Mpi3Library *mpi3Lib2 = Mpi3Library::load("C:\\Users\\mablodgett\\Desktop\\lib.txt");

//    foreach(Mpi3Song *s, mpi3Lib2->songs.values()){
//        qDebug() << s->pid << " " << s->name;
//    }

//}


int main(int argc, char *argv[])
{

//    srand(time(NULL));

//    Mpi3Library *mpi3Lib = new Mpi3Library(true);
//    mpi3Lib->name = "Main Library";
//    mpi3Lib->added = "03/07/2017";

//    Mpi3Song *song_1 = mpi3Lib->addSong();
//    song_1->name = "Me, Myself and I";
//    song_1->artist = "G-Eazy";

//    Mpi3Song *song_2 = mpi3Lib->addSong();
//    song_2->name = "Been On";
//    song_2->artist = "G-Eazy";

//    Mpi3Folder *fldr_1 = mpi3Lib->addFolder();
//    Mpi3Playlist *plist_1 = mpi3Lib->addPlaylist();

//    fldr_1->name = "electric beat";
//    plist_1->name = "upbeat";

//    Mpi3Playlist *plist_2 = mpi3Lib->addPlaylist(fldr_1);
//    plist_2->name = "dance";

//    mpi3Lib->save("C:\\Users\\mablodgett\\Desktop\\lib.txt");

//    load_test();



    QApplication app(argc, argv);
    Mpi3RootDesktop wnd;

    wnd.initialize();
    wnd.show();

    return app.exec();
}
