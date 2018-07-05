#ifndef MLIBRARY_H
#define MLIBRARY_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QRadioButton;
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

public:
    enum View {
        Library,
        Artists,
        Containers,
        Playlist
    };

public:
    View currentView();

private:
    QTreeView *tree_library = nullptr;
    QTreeView *tree_playlists = nullptr;

    QSplitter *frm_library = nullptr;
    QWidget *frm_views = nullptr;
    QWidget *frm_trees = nullptr;

    QRadioButton *btn_songs = nullptr;
    QRadioButton *btn_artists = nullptr;
    QRadioButton *btn_playlists = nullptr;

    QLabel *lbl_view = nullptr;

    View m_currentView;

private slots:
    void changeView(PanelLibrary::View view);

signals:
    void viewChanged();

private:
    void paintEvent(QPaintEvent *event);

};


#endif // MLIBRARY_H
