#include "uistyle.h"

#include <QStyleOption>
#include <QPainter>

#include <QFile>


Mpi3StyleSheet::Mpi3StyleSheet(){}
Mpi3StyleSheet::~Mpi3StyleSheet(){}

void Mpi3StyleSheet::load(const QString &path){
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
void Mpi3StyleSheet::save(const QString &path){
    if(!path.isNull()){
        m_filepath = path;
    }

    QFile saveFile(m_filepath);
    if(saveFile.open(QFile::ReadWrite)){

    }
}

QString Mpi3StyleSheet::qssName() const {
    return m_name;
}
QString Mpi3StyleSheet::qssStyle() const {
    return m_style;
}
QString Mpi3StyleSheet::qssPath() const {
    return m_filepath;
}

void Mpi3StyleSheet::setProperty(const QString &line){
    int split = line.indexOf("=", 0);

    QString pName = line.mid(1, split-1);
    QString pValue = line.right(line.length() - split-1);

    if(pName == "NAME"){
        m_name = pValue;
    }
}
QString Mpi3StyleSheet::removeComments(const QString &text){
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


Mpi3Style::Mpi3Style(QStyle *style) : QProxyStyle(style){}

Mpi3Style::TV_IndicatorStyle Mpi3Style::currentTVIndicatorStyle() const{
    return m_currentTVIndicatorStyle;
}
void Mpi3Style::setTVIndicatorStyle(Mpi3Style::TV_IndicatorStyle tvIndicatorStyle){
    m_currentTVIndicatorStyle = tvIndicatorStyle;
}

void Mpi3Style::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const{
    if(element == QStyle::PE_IndicatorRadioButton){
        return;
    }
    else if(element == QStyle::PE_IndicatorItemViewItemDrop && !option->rect.isNull()){
        if(painter){
            QStyleOption opt(*option);
            opt.rect.setLeft(0);

            if(widget){
                opt.rect.setRight(widget->width());
            }

            switch(m_currentTVIndicatorStyle){

                case Mpi3Style::TV_IndicatorStyleMove: {
                    opt.rect.setHeight(0);
                    break;
                }

                case Mpi3Style::TV_IndicatorStyleDrop: {
                    if(option->rect.y() % 21 > 0){
                        opt.rect.setY(option->rect.y() - 21 + 1);
                    }
                    opt.rect.setHeight(21);
                    break;
                }
            }

            QProxyStyle::drawPrimitive(element, &opt, painter, widget);
        }

        return;
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}
