#pragma once

#ifndef MSTATUSBAR_H
#define MSTATUSBAR_H


#include "mpanel.h"


class MStatusBar : public MPanel
{
    Q_OBJECT

public:
    explicit MStatusBar(QWidget *parent = nullptr);

private:
    void initializeLayout();

};

#endif








































