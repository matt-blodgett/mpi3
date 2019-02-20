#pragma once

#ifndef MPANELHOME_H
#define MPANELHOME_H


#include "mpanel.h"


class MPanelHome : public MPanel
{
    Q_OBJECT

public:
    explicit MPanelHome(QWidget *parent = nullptr);

private:
    void initializeLayout();

};

#endif



















