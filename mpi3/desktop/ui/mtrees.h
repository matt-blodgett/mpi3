#ifndef MTREES_H
#define MTREES_H

#include <QLabel>
#include <QTreeView>


class PanelTrees : public QWidget
{
    Q_OBJECT

public:
    explicit PanelTrees(QWidget *parent=nullptr);
    ~PanelTrees();

private:
    QLabel *lblTreeview = nullptr;
    QTreeView *treeLibrary = nullptr;

private:
    void paintEvent(QPaintEvent *event);

};


#endif // MTREES_H
