#ifndef MLIBRARY_H
#define MLIBRARY_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QPushButton;
class QTreeView;
class QLabel;
class QSplitter;
QT_END_NAMESPACE


class PanelLibrary : public QWidget
{
    Q_OBJECT

public:
    explicit PanelLibrary(QWidget *parent=nullptr);
    ~PanelLibrary();


private:
    QTreeView *tree_library = nullptr;

    QSplitter *frm_library = nullptr;
    QWidget *frm_views = nullptr;
    QWidget *frm_trees = nullptr;

    QPushButton *btn_songs = nullptr;
    QPushButton *btn_artists = nullptr;
    QPushButton *btn_playlists = nullptr;

    QLabel *lbl_view = nullptr;

private:
    void paintEvent(QPaintEvent *event);

};


#endif // MLIBRARY_H
