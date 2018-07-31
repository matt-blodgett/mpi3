#ifndef UISTYLESHEET_H
#define UISTYLESHEET_H

#include <QString>


class Mpi3StyleSheet
{

public:
    Mpi3StyleSheet();
    ~Mpi3StyleSheet();

public:
    void load(const QString &path = QString());
    void save(const QString &path = QString());

public:
    QString qssName() const;
    QString qssStyle() const;
    QString qssPath() const;

private:
    void setProperty(const QString &line);
    QString removeComments(const QString &text);

private:
    QString m_name;
    QString m_style;
    QString m_filepath;
};

#endif
