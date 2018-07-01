#ifndef MTREEVIEWS_H
#define MTREEVIEWS_H

#include <QTreeView>
#include <QLabel>


// ----------------------------------------------------------------------------------------------------
// * LibraryTreeview *
// ----------------------------------------------------------------------------------------------------
class LibraryTreeview : public QTreeView
{
    Q_OBJECT

public:
    explicit LibraryTreeview(QWidget *parent=nullptr);
    ~LibraryTreeview();

};


#endif // MTREEVIEWS_H
