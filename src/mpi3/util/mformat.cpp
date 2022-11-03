#include "mpi3/util/mformat.h"

#include <QString>
#include <QList>


namespace Mpi3 {

    namespace Util {

        QString timeToString(double time)
        {
            time /= 1000;
            int sctime = static_cast<int>(time);
            int seconds = sctime % 60;
            int minutes = (sctime - seconds) / 60;

            QString secs = QString::number(seconds);
            QString mins = QString::number(minutes);

            if(secs.size() == 1) {
                secs.prepend("0");
            }

            return QString(mins + ":" + secs);
        }
        QString sizeToString(double size, int prec)
        {
            const QList<QString> fileSizeSuffixes = {"KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};

            QString unit("B");
            QStringListIterator i(fileSizeSuffixes);
            while(size >= 1024.0 && i.hasNext()) {
                unit = i.next();
                size /= 1024.0;
            }

            return QString().setNum(size,'f', prec) + " " + unit;
        }
        QString percentToString(double percent, int prec)
        {
            return QString().setNum(percent * 100.0,'f', prec) + " %";
        }

    };
};
