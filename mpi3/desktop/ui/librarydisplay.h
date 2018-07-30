#ifndef LIBRARYDISPLAY_H
#define LIBRARYDISPLAY_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QRadioButton;
class QSplitter;
class QLabel;
QT_END_NAMESPACE

class Mpi3TreeView;


class Mpi3PanelLibrary : public QWidget
{
    Q_OBJECT

public:
    explicit Mpi3PanelLibrary(QWidget *parent = nullptr);
    ~Mpi3PanelLibrary();

public:
    enum View {
        ViewAllSongs,
        ViewArtists,
        ViewAlbums,
        ViewContainer
    };

    Mpi3PanelLibrary::View currentView() const;
    void setDisplay(const QString &title);
    void changeView(Mpi3PanelLibrary::View view);

private:
    Mpi3PanelLibrary::View m_currentView;

    Mpi3TreeView *tree_songlist = nullptr;
    Mpi3TreeView *tree_containers = nullptr;

    QSplitter *frm_library = nullptr;
    QWidget *frm_views = nullptr;
    QWidget *frm_trees = nullptr;

    QRadioButton *btn_songs = nullptr;
    QRadioButton *btn_artists = nullptr;
    QRadioButton *btn_albums = nullptr;

    QLabel *lbl_playlist = nullptr;
    QLabel *lbl_view = nullptr;

private:
    void containerClicked(const QModelIndex &index);

signals:
    void viewChanged();

private:
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent *event);

};


#endif
