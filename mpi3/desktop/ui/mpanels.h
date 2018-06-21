#ifndef MPANELS_H
#define MPANELS_H

#include <QPushButton>


class PanelViews : public QWidget
{
    Q_OBJECT

public:
    explicit PanelViews(QWidget *parent=nullptr);
    ~PanelViews();

private:
    QPushButton *btnSongs = nullptr;
    QPushButton *btnArtists = nullptr;
    QPushButton *btnPlaylists = nullptr;

private:
    void paintEvent(QPaintEvent *event);

};


#endif // MPANELS_H
