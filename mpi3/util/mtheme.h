#ifndef MTHEME_H
#define MTHEME_H

#include <QString>


// ----------------------------------------------------------------------------------------------------
// * Mpi3Theme *
// ----------------------------------------------------------------------------------------------------
class Mpi3Theme
{

public:
    Mpi3Theme();

public:
    void load(const QString &path = NULL);
    void save(const QString &path = NULL);

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

#endif // MTHEME_H
