#ifndef UISTYLE_H
#define UISTYLE_H

#include <QString>


class Mpi3Style
{

public:
    Mpi3Style();
    ~Mpi3Style();

public:
    void load(const QString &path = QString());
    void save(const QString &path = QString());

private:
    void setProperty(const QString &line);
    QString removeComments(const QString &text);

public:
    QString qssName() const;
    QString qssStyle() const;
    QString qssPath() const;

private:
    QString m_name;
    QString m_style;
    QString m_filepath;
};

#endif
