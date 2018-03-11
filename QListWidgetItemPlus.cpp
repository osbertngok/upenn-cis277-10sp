#include "QListWidgetItemPlus.h"

QLWI_Face::QLWI_Face(const QString & text, QListWidget * parent, Face* f)
: QListWidgetItem(text,parent),
  _f(f)
{
}

QLWI_Vertex::QLWI_Vertex(const QString & text, QListWidget * parent, Vertex* v)
: QListWidgetItem(text,parent),
  _v(v)
{
}

QLWI_HalfEdge::QLWI_HalfEdge(const QString & text, QListWidget * parent, HalfEdge* he)
: QListWidgetItem(text,parent),
  _he(he)
{
}
