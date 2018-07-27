#include "uistyle.h"

#include <QFile>


Mpi3Style::Mpi3Style(){}
Mpi3Style::~Mpi3Style(){}

void Mpi3Style::load(const QString &path){
    if(!path.isNull()){
        m_filepath = path;
    }

    QFile loadFile(m_filepath);
    if(loadFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        QString raw = loadFile.readAll();
        QString qss = removeComments(raw);

        QString themeBegin = "!THEME=BEGIN";
        QString themeEnd = "!THEME=END";

        int i = 0;
        while(i < qss.length()){
            int n = qss.indexOf("\n", i);
            QString line = qss.mid(i, n-i);

            if(line.startsWith(themeBegin)){
                n = qss.indexOf(themeEnd);
                QString rawStyle = qss.mid(i, n);

                int b = themeBegin.length();
                int e = themeEnd.length();

                m_style = rawStyle.mid(b, rawStyle.length()-b-e);

            } else if(line.startsWith("!")) {
                setProperty(line);
            }

            i = n + 1;
        }
    }
}
void Mpi3Style::save(const QString &path){
    if(!path.isNull()){
        m_filepath = path;
    }

    QFile saveFile(m_filepath);
    if(saveFile.open(QFile::ReadWrite)){

    }
}

void Mpi3Style::setProperty(const QString &line){
    int split = line.indexOf("=", 0);

    QString pName = line.mid(1, split-1);
    QString pValue = line.right(line.length() - split-1);

    if(pName == "NAME"){
        m_name = pValue;
    }
}
QString Mpi3Style::removeComments(const QString &text){
    QString parsed;

    int i = 0;
    while(i < text.length()){
        int n = text.indexOf("\n", i);
        QString line = text.mid(i, n-i);

        int s = line.indexOf("//", 0);
        if(s > -1){
            parsed += line.left(s);
        }
        else {
            parsed += line + "\n";
        }

        i = n + 1;
    }

    parsed += "\n";
    return parsed;
}

QString Mpi3Style::qssName() const {
    return m_name;
}
QString Mpi3Style::qssStyle() const {
    return m_style;
}
QString Mpi3Style::qssPath() const {
    return m_filepath;
}
