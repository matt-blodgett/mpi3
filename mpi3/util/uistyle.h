#ifndef UISTYLE_H
#define UISTYLE_H

#include <QString>


class Mpi3Theme
{

public:
    Mpi3Theme();
    ~Mpi3Theme();

public:
    void load(const QString &path = QString());
    void save(const QString &path = QString());

private:
    void setProperty(const QString &line);
    QString removeComments(const QString &text);

public:
    QString& qssPath();
    QString qssName;
    QString qssStyle;

private:
    QString filepath;

};

#endif
