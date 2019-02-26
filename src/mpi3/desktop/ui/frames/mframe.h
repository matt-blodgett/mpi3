#pragma once

#ifndef MFRAME_H
#define MFRAME_H


#include <QWidget>


class MFrame : public QWidget
{
    Q_OBJECT

public:
    explicit MFrame(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event);
};


#endif
