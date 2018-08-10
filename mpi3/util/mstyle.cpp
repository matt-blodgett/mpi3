#include "mstyle.h"

#include <QStyleOption>
#include <QPainter>

#include <QFile>


MStyleSheet::MStyleSheet(){}

void MStyleSheet::load(const QString &path){
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
void MStyleSheet::save(const QString &path){
    if(!path.isNull()){
        m_filepath = path;
    }

    QFile saveFile(m_filepath);
    if(saveFile.open(QFile::ReadWrite)){

    }
}

QString MStyleSheet::qssName() const {
    return m_name;
}
QString MStyleSheet::qssStyle() const {
    return m_style;
}
QString MStyleSheet::qssPath() const {
    return m_filepath;
}

void MStyleSheet::setProperty(const QString &line){
    int split = line.indexOf("=", 0);

    QString pName = line.mid(1, split-1);
    QString pValue = line.right(line.length() - split-1);

    if(pName == "NAME"){
        m_name = pValue;
    }
}
QString MStyleSheet::removeComments(const QString &text){
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


MStyle::MStyle(QStyle *style) : QProxyStyle(style){}

MStyle::TV_IndicatorStyle MStyle::currentTVIndicatorStyle() const{
    return m_currentTVIndicatorStyle;
}
void MStyle::setTVIndicatorStyle(MStyle::TV_IndicatorStyle tvIndicatorStyle){
    m_currentTVIndicatorStyle = tvIndicatorStyle;
}

void MStyle::drawPrimitive(
        PrimitiveElement element, const QStyleOption *option,
        QPainter *painter, const QWidget *widget) const{

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

                case MStyle::TV_IndicatorStyleMove: {
                    opt.rect.setHeight(0);
                    break;
                }

                case MStyle::TV_IndicatorStyleDrop: {
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
