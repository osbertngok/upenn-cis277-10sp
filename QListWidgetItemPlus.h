#ifndef _QLISTWIDGETITEMPLUS_H_
#define _QLISTWIDGETITEMPLUS_H_
#include <QListWidgetItem>
#include "Stuff.h"
class QLWI_Face : public QListWidgetItem
{
  public:
    QLWI_Face(const QString & text, QListWidget * parent, Face* f);
    Face* _f;
};

class QLWI_Vertex : public QListWidgetItem
{
  public:
    QLWI_Vertex(const QString & text, QListWidget * parent, Vertex* v);
    Vertex* _v;
};

class QLWI_HalfEdge : public QListWidgetItem
{
  public:
    QLWI_HalfEdge(const QString & text, QListWidget * parent, HalfEdge* he);
    HalfEdge* _he;
};
#endif

