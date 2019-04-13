#include "mmediautil.h"
#include "mmedialibrary.h"


#include <QUrl>


namespace Mpi3 {
namespace Core {

bool validMediaFiles(QUrl mediaUrl)
{
    if(mediaUrl.toString().endsWith(".mp3")
        || mediaUrl.toString().endsWith(".wav")) {
        return true;
    }

    return false;
}
bool validMediaFiles(QList<QUrl> mediaUrls)
{
    if(mediaUrls.size() == 0) {
        return false;
    }

    for(int i = 0; i < mediaUrls.size(); i++) {
        if(!validMediaFiles(mediaUrls.at(i))) {
            return false;
        }
    }

    return true;
}

QByteArray songsToBytes(MSongList songlist)
{
    QByteArray pidBytes;
    for(MSong *s : songlist) {
        pidBytes.append(s->pid());
    }

    return pidBytes;
}
QList<QUrl> songsToPaths(MSongList songlist)
{
    QList<QUrl> songUrls;
    for(MSong *s : songlist) {
        songUrls.append(s->path());
    }

    return songUrls;
}

QStringList bytesToSongs(QByteArray pidBytes)
{
    QStringList pidStrings;

    int i = 0;
    int length = MPI3_PID_STRING_LENGTH;
    while(i + length <= pidBytes.size()) {
        pidStrings.append(pidBytes.mid(i, length));
        i += length;
    }

    return pidStrings;
}

};
};
