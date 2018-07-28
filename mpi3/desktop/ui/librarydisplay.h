#ifndef LIBRARYDISPLAY_H
#define LIBRARYDISPLAY_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QRadioButton;
class QSplitter;
class QLabel;
QT_END_NAMESPACE

class Mpi3TreeView;


class PanelLibrary : public QWidget
{
    Q_OBJECT

public:
    explicit PanelLibrary(QWidget *parent = nullptr);
    ~PanelLibrary();

public:
    enum View {
        ViewAllSongs,
        ViewArtists,
        ViewAlbums,
        ViewContainer
    };

    PanelLibrary::View currentView() const;
    void changeView(PanelLibrary::View view);
    void setDisplay(const QString &title);

private:
    PanelLibrary::View m_currentView;

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

private slots:
    void containerClicked(const QModelIndex &index);

signals:
    void viewChanged();

private:
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent *event);

};


#endif
