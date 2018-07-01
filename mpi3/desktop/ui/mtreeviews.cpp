#include "mtreeviews.h"


// ----------------------------------------------------------------------------------------------------
// * LibraryTreeview *
// ----------------------------------------------------------------------------------------------------
LibraryTreeview::LibraryTreeview(QWidget *parent) : QTreeView(parent)
{
    this->setObjectName("LibraryTreeview");
    this->setAlternatingRowColors(true);

}

LibraryTreeview::~LibraryTreeview()
{

}


