#pragma once

#ifndef MPANELPLAYBACK_H
#define MPANELPLAYBACK_H


#include "mpanel.h"


class MPanelPlayback : public MPanel
{    
    Q_OBJECT

public:
    explicit MPanelPlayback(QWidget *parent = nullptr);

private:
    void initializeLayout();

};


#endif
