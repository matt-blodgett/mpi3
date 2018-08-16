#ifndef MPANELDEVICE_H
#define MPANELDEVICE_H

#include <QWidget>


class MPanelDevice : public QWidget
{
    Q_OBJECT

public:
    explicit MPanelDevice(QWidget *parent = nullptr);
    ~MPanelDevice();

private:
    void initializeLayout();

private:
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *event);
};

#endif
