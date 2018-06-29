#include "mtheme.h"

#include <QFile>

// ----------------------------------------------------------------------------------------------------
// * Mpi3Theme *
// ----------------------------------------------------------------------------------------------------
Mpi3Theme::Mpi3Theme()
{

}

void Mpi3Theme::load(const QString &path)
{
    if(path != NULL){
        this->filepath = path;
    }

    QFile loadFile(this->filepath);
    if(loadFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString raw = loadFile.readAll();
        QString qss = this->removeComments(raw);

        QString themeBegin = "!THEME=BEGIN";
        QString themeEnd = "!THEME=END";

        int i = 0;
        while(i < qss.length())
        {
            int n = qss.indexOf("\n", i);
            QString line = qss.mid(i, n-i);

            if(line.startsWith(themeBegin))
            {
                n = qss.indexOf(themeEnd);
                QString rawStyle = qss.mid(i, n);

                int b = themeBegin.length();
                int e = themeEnd.length();

                this->qssStyle = rawStyle.mid(b, rawStyle.length()-b-e);
            }
            else if(line.startsWith("!"))
            {
                this->setProperty(line);
            }

            i = n + 1;
        }
    }
}

QString Mpi3Theme::removeComments(const QString &text)
{
    QString parsed;

    int i = 0;
    while(i < text.length())
    {
        int n = text.indexOf("\n", i);
        QString line = text.mid(i, n-i);

        int s = line.indexOf("//", 0);
        if(s > -1){parsed += line.left(s);}
        else{parsed += line + "\n";}

        i = n + 1;
    }

    parsed += "\n";
    return parsed;
}

void Mpi3Theme::setProperty(const QString &line)
{
    int split = line.indexOf("=", 0);

    QString pName = line.mid(1, split-1);
    QString pValue = line.right(line.length() - split-1);

    if(pName == "NAME")
    {
        this->qssName = pValue;
    }
}


void Mpi3Theme::save(const QString &path)
{
    if(path != NULL){
        this->filepath = path;
    }

    QFile saveFile(this->filepath);

    if(saveFile.open(QFile::ReadWrite))
    {


    }
}

QString& Mpi3Theme::qssPath()
{
    return this->filepath;
}
