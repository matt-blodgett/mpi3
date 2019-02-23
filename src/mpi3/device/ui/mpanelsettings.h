#pragma once

#ifndef MPANELSETTINGS_H
#define MPANELSETTINGS_H


#include "mpanel.h"


class MPanelSettings : public MPanel
{
    Q_OBJECT

public:
    explicit MPanelSettings(QWidget *parent = nullptr);

private:
    void initializeLayout();

};


#endif
