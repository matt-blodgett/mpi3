#ifndef MPANELS_H
#define MPANELS_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QPushButton;
class QLabel;
class QAbstractItemView;
class QSplitter;
QT_END_NAMESPACE

class LibraryTreeview;
class LibraryModel;
class Mpi3Library;


class PanelLibrary : public QWidget
{
    Q_OBJECT

public:
    explicit PanelLibrary(QWidget *parent=nullptr);
    ~PanelLibrary();


private:
    LibraryTreeview *tree_library = nullptr;
    LibraryModel *m_model = nullptr;

    QSplitter *frm_library = nullptr;
    QWidget *frm_views = nullptr;
    QWidget *frm_trees = nullptr;

    QPushButton *btn_songs = nullptr;
    QPushButton *btn_artists = nullptr;
    QPushButton *btn_playlists = nullptr;

    QLabel *lbl_view = nullptr;


    Mpi3Library *m_library = nullptr;

private:
    void paintEvent(QPaintEvent *event);

};


#endif // MPANELS_H
