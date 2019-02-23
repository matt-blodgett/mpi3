#pragma once

#ifndef MPANELMEDIA_H
#define MPANELMEDIA_H


#include "mpanel.h"


class MPanelMedia : public MPanel
{
    Q_OBJECT

public:
    explicit MPanelMedia(QWidget *parent = nullptr);

private:
    void initializeLayout();

};


#endif
