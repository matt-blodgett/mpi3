#pragma once

#ifndef MPANEL_H
#define MPANEL_H

#include <QWidget>


class MPanel : public QWidget
{
    Q_OBJECT

public:
    explicit MPanel(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event);
};


#endif








































