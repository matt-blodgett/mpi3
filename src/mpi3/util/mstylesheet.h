#pragma once

#ifndef MSTYLESHEET_H
#define MSTYLESHEET_H


#include <QString>


#include "mpi3/util/mutil.h"
#ifdef MPI3_BUILD_SHARED
class MPI3_EXPORT_UTIL MStyleSheet;
#endif


class MStyleSheet
{

public:
    explicit MStyleSheet();

public:
    void load(const QString &path = QString());
    void save(const QString &path = QString());

public:
    QString name() const;
    QString style() const;
    QString path() const;

private:
    void setProperty(const QString &line);
    QString removeComments(const QString &text);

private:
    QString m_name;
    QString m_style;
    QString m_path;
};


#endif
