#include <QtOpenGL>
#include <iostream>
#include <map>
#include <string>
#include "Stuff.h"

using std::endl;
using std::cout;
using std::map;
using std::pair;
using std::string;

Stuff::Stuff()
:_max_f_num(0),
 _max_v_num(0),
 _max_he_num(0),
 _trash_flag(false)
{
}

Stuff::~Stuff()
{
  for (list<Vertex*>::iterator i = _v_vec.begin();
       i != _v_vec.end();
       ++i)
    delete *i;
  for (list<HalfEdge*>::iterator i = _he_vec.begin();
       i != _he_vec.end();
       ++i)
    delete *i;
  for (list<Face*>::iterator i = _f_vec.begin();
       i != _f_vec.end();
       ++i)
    delete *i;
}

void Stuff::toCube(unsigned int level)
{
  HalfEdge* ahhe;
  map<string,HalfEdge*> smap;
  toQuad(vec3(1,1,0.5),vec3(-1,1,0.5),vec3(-1,-1,0.5),vec3(1,-1,0.5),vec3(1,1,0));

  if (level < 1) return;

  list<Vertex*>::iterator iter = _v_vec.begin();
  Vertex* v1 = *(iter++);
  Vertex* v2 = *(iter++);
  Vertex* v3 = *(iter++);
  Vertex* v4 = *(iter++);
  smap["e12"] = v1->toV(v2);
  splitQuad(smap["e12"]->adjface(),v2);
  Vertex* v5;
  //cout << v4->toV(v2)->getId() << endl;
  addVertexToHalfEdge(v4->toV(v2), v5, smap["e45"], 0.5);
  //cout << smap["e45"]->adjface()->getId() << ' ';
  splitQuad(smap["e45"]->adjface(),v5);
  //cout << smap["e45"]->sym()->adjface()->getId() << ' ';
  splitQuad(smap["e45"]->sym()->adjface(),v5);
  smap["e23"] = v2->toV(v3);
  smap["e34"] = v3->toV(v4);
  smap["e41"] = v4->toV(v1);
  //cout << smap["e12"]->adjface()->getId() << ' ';
  smap["e12"]->adjface()->setColor(vec3(1,1,0));
  //cout << smap["e23"]->adjface()->getId() << ' ';
  smap["e23"]->adjface()->setColor(vec3(1,0,1));
  //cout << smap["e34"]->adjface()->getId() << ' ';
  smap["e34"]->adjface()->setColor(vec3(0,1,1));
  //cout << smap["e41"]->adjface()->getId() << ' ';
  smap["e41"]->adjface()->setColor(vec3(0,1,0));

  if (level < 2) return;

  Vertex* v6;
  //cout << v5->toV(v1)->getId() << ' ';
  addVertexToHalfEdge(v5->toV(v1),v6,ahhe,0.5);
  //cout << smap["e41"]->adjface()->getId() << ' ';
  splitQuad(smap["e41"]->adjface(),v4);
  //cout << smap["e41"]->adjface()->getId() << ' ';
  smap["e41"]->adjface()->setColor(vec3(0,0,1));

  if (level < 3) return;
  //cout << smap["e34"]->adjface()->getId() << ' ';
  //cout << smap["e41"]->adjface()->getId() << ' ';
  splitVertices(smap["e34"]->adjface(),smap["e41"]->adjface(),v4,smap["e47"]);
  Vertex* v7 = smap["e47"]->adjvertex();
  v7->setCoord(vec3(0.5,-0.5,0.5));

  if (level < 4) return;
  //cout << v5->toV(v7)->adjface()->getId() << ' ';
  //cout << v5->toV(v2)->adjface()->getId() << ' ';
  splitVertices(v5->toV(v7)->adjface(),v5->toV(v2)->adjface(),v5,smap["e58"]);
  Vertex* v8 = smap["e58"]->adjvertex();
  v8->setCoord(vec3(-0.5,-0.5,0.5));
  v5->setCoord(vec3(-0.5,0.5,0.5));

  if (level < 5) return;

  v1->setCoord(vec3(0.5,0.5,-0.5));
  v2->setCoord(vec3(-0.5,0.5,-0.5));
  v3->setCoord(vec3(-0.5,-0.5,-0.5));
  v4->setCoord(vec3(0.5,-0.5,-0.5));

  if (level < 6) return;
  //cout << v2->toV(v1)->getId() << ' ';
  fillFace(v2->toV(v1),vec3(1,1,1));

  if (level < 7) return;
  //cout << v8->toV(v3)->adjface()->getId() << ' ';
  splitQuad(v8->toV(v3)->adjface(),v3);
  //cout << v8->toV(v7)->adjface()->getId() << ' ';
  splitQuad(v8->toV(v7)->adjface(),v7);
  //cout << v8->toV(v5)->adjface()->getId() << ' ';
  splitQuad(v8->toV(v5)->adjface(),v5);

  trashV(v6);
  emptyTrash();
  if (level < 8) return;

  trashV(v2);
  emptyTrash();
  if (level < 9) return;

  trashV(v4);
  emptyTrash();

  if (level < 10) return;
  //cout << v3->toV(v5)->getId();
  fillFace(v3->toV(v5),vec3(0.5,0.5,0.5));

  return;
}

void Stuff::toTriangle(vec3 p1, vec3 p2, vec3 p3, vec3 color)
{
  clear();
  //Vertices
  Vertex* v1 = new Vertex(p1);
  Vertex* v2 = new Vertex(p2);
  Vertex* v3 = new Vertex(p3);

  //Half Edges
  //Outer Loop Only

  HalfEdge* he12 = new HalfEdge();
  HalfEdge* he23 = new HalfEdge();
  HalfEdge* he31 = new HalfEdge();

  //Now Inner Loop

  HalfEdge* he21 = new HalfEdge();
  HalfEdge* he32 = new HalfEdge();
  HalfEdge* he13 = new HalfEdge();

  //Face
  Face* f = new Face();


  //Vertices information
  v1->setHE(he12);
  v2->setHE(he23);
  v3->setHE(he31);

  //Half Edges Information
  he12->setFace(f);
  he12->setVertex(v2);
  he12->setNext(he23);
  he12->setSym(he21);
  
  he23->setFace(f);
  he23->setVertex(v3);
  he23->setNext(he31);
  he23->setSym(he32);

  he31->setFace(f);
  he31->setVertex(v1);
  he31->setNext(he12);
  he31->setSym(he13);

  he21->setFace(NULL);
  he21->setVertex(v1);
  he21->setNext(he13);
  he21->setSym(he12);
  
  he32->setFace(NULL);
  he32->setVertex(v2);
  he32->setNext(he21);
  he32->setSym(he23);

  he13->setFace(NULL);
  he13->setVertex(v3);
  he13->setNext(he32);
  he13->setSym(he31);
  //Face Information
  f->setHE(he12);
  f->setColor(color);

  //Integrate into Stuff

  addV(v1);
  addV(v2);
  addV(v3);

  addF(f);

  addHE(he12);
  addHE(he23);
  addHE(he31);
  addHE(he21);
  addHE(he32);
  addHE(he13);

  return;
}
void Stuff::toQuad(vec3 p1, vec3 p2, vec3 p3, vec3 p4, vec3 color)
{
  toTriangle(p1, p2, p3, color);
  //Pick up the last HalfEdge
  HalfEdge* he = _he_vec.back();
  //Add it!
  Vertex* newVertex;
  HalfEdge* newHalfEdge;
  addVertexToHalfEdge(he,newVertex,newHalfEdge,0.5);
  newVertex->setCoord(p4);
  return;
}

void Stuff::clear()
{
  for (list<Vertex*>::iterator i = _v_vec.begin();
       i != _v_vec.end();
       ++i)
    delete *i;
  for (list<HalfEdge*>::iterator i = _he_vec.begin();
       i != _he_vec.end();
       ++i)
    delete *i;
  for (list<Face*>::iterator i = _f_vec.begin();
       i != _f_vec.end();
       ++i)
    delete *i;
  _f_vec.clear();
  _v_vec.clear();
  _he_vec.clear();
  _max_f_num = 0;
  _max_v_num = 0;
  _max_he_num = 0;
  _trash_flag = false;
  return;
}

void Stuff::addV(Vertex* p)
{
  _max_v_num++;
  p->setId(_max_v_num);
  _v_vec.push_back(p);
  return;
}

void Stuff::addF(Face* p)
{
  _max_f_num++;
  p->setId(_max_f_num);
  _f_vec.push_back(p);
  return;
}

void Stuff::addHE(HalfEdge* p)
{
  _max_he_num++;
  p->setId(_max_he_num);
  _he_vec.push_back(p);
  return;
}

bool Stuff::addVertexToHalfEdge(HalfEdge* he, Vertex* & nv, HalfEdge* & nhe, double ratio)
{
  /*
    //Real Half Edge Edition

  //Error Checking
  if (he == NULL) return false;

  //Find out HalfEdges, Vertices and Faces that could be affected
  
  Vertex* fromVertex = NULL;  //HE0->V
  Vertex* toVertex = he->adjvertex(); //HE1->V

  HalfEdge* newHalfEdge1 = new HalfEdge(); //HENL
  addHE(newHalfEdge1);

  HalfEdge* previousHalfEdge1 = NULL;  //HE0
  Face* leftFace1 = he->adjface(); //HE1->FACE

  //Now cooking time for parameters!
  //First we need to find out fromVertex and previous HalfEdge
  HalfEdge* heIterator = he;
  while (heIterator->next() != he)
  {
    heIterator = heIterator->next();
    if (heIterator == NULL) return false;
  }
  //We got it...
  previousHalfEdge1 = heIterator;
  fromVertex = previousHalfEdge1->adjvertex();

  Vertex* newVertex = new Vertex(fromVertex->getCoord() * (1 - ratio) + toVertex->getCoord() * ratio); //NV
  addV(newVertex);

  //previousHalfEdge1
  previousHalfEdge1->setNext(newHalfEdge1);

  //fromVertex
  fromVertex->setHE(newHalfEdge1); // Just in case

  //newHalfEdge1
  newHalfEdge1->setFace(leftFace1);
  newHalfEdge1->setVertex(newVertex);
  newHalfEdge1->setNext(he);

  //newVertex
  newVertex->setHE(he);

  //Next, any symmetric HalfEdges?
  HalfEdge* she = he->sym();
  if (she != NULL)
  {

    //First we need to find out fromVertex and previous HalfEdge
    heIterator = she;
    while (heIterator->next() != she)
    {
      heIterator = heIterator->next();
      if (heIterator == NULL) return false;
    }
    //We got it...
    HalfEdge* previousHalfEdge2 = heIterator;

    HalfEdge* newHalfEdge2 = new HalfEdge(); //HENR, actually it is not
    addHE(newHalfEdge2);

    //previousHalfEdge2
    previousHalfEdge2->setNext(newHalfEdge2);

    //toVertex
    toVertex->setHE(newHalfEdge2); // Just in case

    //newHalfEdge2
    newHalfEdge2->setFace(she->adjface());
    newHalfEdge2->setVertex(newVertex);
    newHalfEdge2->setNext(she);

    //Need to change the symmetric values
    he->setSym(newHalfEdge2);
    newHalfEdge1->setSym(she);
    she->setSym(newHalfEdge1);
    newHalfEdge2->setSym(he);

  }
  nv = newVertex;
  nhe = newHalfEdge1;
  return true;
*/

  //Error Checking
  if (he == NULL) return false;

  //Find out HalfEdges, Vertices and Faces that could be affected
  
  Vertex* fromVertex = NULL;  //HE0->V
  Vertex* toVertex = he->adjvertex(); //HE1->V

  HalfEdge* newHalfEdge1 = new HalfEdge(); //HENL
  addHE(newHalfEdge1);

  HalfEdge* previousHalfEdge1 = NULL;  //HE0
  Face* leftFace1 = he->adjface(); //HE1->FACE

  //Now cooking time for parameters!
  //First we need to find out fromVertex and previous HalfEdge
  HalfEdge* heIterator = he;
  while (heIterator->next() != he)
  {
    heIterator = heIterator->next();
    if (heIterator == NULL) return false;
  }
  //We got it...
  previousHalfEdge1 = heIterator;
  fromVertex = previousHalfEdge1->adjvertex();

  Vertex* newVertex = new Vertex(fromVertex->getCoord() * (1 - ratio) + toVertex->getCoord() * ratio); //NV
  addV(newVertex);

  //previousHalfEdge1
  previousHalfEdge1->setNext(newHalfEdge1);

  //fromVertex
  fromVertex->setHE(newHalfEdge1); // Just in case

  //newHalfEdge1
  newHalfEdge1->setFace(leftFace1);
  newHalfEdge1->setVertex(newVertex);
  newHalfEdge1->setNext(he);

  //newVertex
  newVertex->setHE(he);

  //Next, any symmetric HalfEdges?
  HalfEdge* she = he->sym();

  //First we need to find out fromVertex and previous HalfEdge
  heIterator = she;
  while (heIterator->next() != she)
  {
    heIterator = heIterator->next();
    if (heIterator == NULL) return false;
  }
  //We got it...
  HalfEdge* previousHalfEdge2 = heIterator;

  HalfEdge* newHalfEdge2 = new HalfEdge(); //HENR, actually it is not
  addHE(newHalfEdge2);

  //previousHalfEdge2
  previousHalfEdge2->setNext(newHalfEdge2);

  //toVertex
  toVertex->setHE(newHalfEdge2); // Just in case

  //newHalfEdge2
  newHalfEdge2->setFace(she->adjface());
  newHalfEdge2->setVertex(newVertex);
  newHalfEdge2->setNext(she);

  //Need to change the symmetric values
  he->setSym(newHalfEdge2);
  newHalfEdge1->setSym(she);
  she->setSym(newHalfEdge1);
  newHalfEdge2->setSym(he);

  nv = newVertex;
  nhe = newHalfEdge1;
  return true;
}

bool Stuff::addVertexToHalfEdge(HalfEdge* he, double ratio)
{
  Vertex* adhoc1;
  HalfEdge* adhoc2;
  return addVertexToHalfEdge(he,adhoc1,adhoc2,ratio);
}

bool Stuff::splitVertices(Face* f1, Face* f2, Vertex* v, HalfEdge* & nhe)
{
  //safety check
  if (!f1) return false;
  if (!f2) return false;
  if (f1 == f2) return false;
  if (!v) return false;

  //Three things new:
  Vertex* newVertex = new Vertex(v->getCoord());
  HalfEdge* old2New = new HalfEdge();
  HalfEdge* new2Old = new HalfEdge();

  //Critical HalfEdges

  HalfEdge* f1InHE = f1->HEtoVertex(v);
  if (f1InHE == NULL) return false;
  HalfEdge* f1OutHE = f1InHE->next();

  HalfEdge* f2InHE = f2->HEtoVertex(v);
  if (f2InHE == NULL) return false;
  HalfEdge* f2OutHE = f2InHE->next();

  
  //Affected HalfEdges;
  vector<HalfEdge*> vhe;
  HalfEdge* iter = f1OutHE;
  while (iter->sym() != f2InHE)
  {
    vhe.push_back(iter->sym());
    iter = iter->sym()->next();
  }
  vhe.push_back(f2InHE);

  for (unsigned int i = 0, size = vhe.size(); i < size; ++i)
    vhe[i]->setVertex(newVertex);

  //Now deal with two new HEs
  old2New->setFace(f1);
  old2New->setVertex(newVertex);
  old2New->setNext(f1OutHE);
  old2New->setSym(new2Old);

  new2Old->setFace(f2);
  new2Old->setVertex(v);
  new2Old->setNext(f2OutHE);
  new2Old->setSym(old2New);

  //Something to do with InHEs
  f1InHE->setNext(old2New);
  f2InHE->setNext(new2Old);

  //newVertex
  newVertex->setHE(new2Old);
  v->setHE(old2New);

  //push back to stack
  addV(newVertex);
  addHE(old2New);
  addHE(new2Old);

  nhe = old2New;
  return true;
}

bool Stuff::splitVertices(Face* f1, Face* f2, Vertex* v)
{
  HalfEdge* adhoc;
  return splitVertices(f1,f2,v,adhoc);
}

bool Stuff::splitQuad(Face* f, Vertex* v, HalfEdge* & Ov)
{
  // Safety check
  if (!f) return false;
  if (!v) return false;
  
  unsigned int _ID = 4;

  HalfEdge* tempHE[4];
  Vertex* tempV[4];
  tempHE[0] = f->adjHE();
  tempV[0] = tempHE[0]->adjvertex();
  if (tempV[0] == v) _ID = 0;

  for (unsigned int i = 1; i < 4; ++i)
  {
    tempHE[i] = tempHE[i - 1]->next();
    tempV[i] = tempHE[i]->adjvertex();
    if (tempV[i] == v) _ID = i;
  }
  if (_ID == 4) return false;
  if (tempHE[3]->next() != tempHE[0]) return false;

  HalfEdge* OutEdge = new HalfEdge();
  HalfEdge* InEdge = new HalfEdge();
  Face* LeftFace = f;
  Face* RightFace = new Face();
  RightFace->setColor(LeftFace->getColor());

  //Left Circle
  OutEdge->setFace(f);
  OutEdge->setVertex(tempV[(_ID+2)%4]);
  OutEdge->setNext(tempHE[(_ID+3)%4]);
  OutEdge->setSym(InEdge);

  tempHE[_ID]->setNext(OutEdge);

  LeftFace->setHE(OutEdge);

  //Right Circle
  InEdge->setFace(RightFace);
  InEdge->setVertex(tempV[_ID]);
  InEdge->setNext(tempHE[(_ID+1)%4]);
  InEdge->setSym(OutEdge);

  tempHE[(_ID+1)%4]->setFace(RightFace);
  tempHE[(_ID+2)%4]->setFace(RightFace);
  tempHE[(_ID+2)%4]->setNext(InEdge);

  RightFace->setHE(InEdge);

  addHE(OutEdge);
  addHE(InEdge);
  addF(RightFace);

  Ov = OutEdge;
  return true;
}

bool Stuff::splitQuad(Face* f, Vertex* v)
{
  HalfEdge* adhoc;
  return splitQuad(f,v,adhoc);
}

bool Stuff::fillFace(HalfEdge* p, vec3 color)
{
  if (!p) return false;
  if (p->adjface() != NULL) return false;
  Face* nF = new Face();
  nF->setColor(color);
  nF->setHE(p);
  HalfEdge* iter = p;
  do
  {
    iter->setFace(nF);
    iter = iter->next();
  } while (iter != p);
  addF(nF);
  return true;
}


bool Stuff::isDirty() const
{
  return _trash_flag;
}

void Stuff::dirty()
{
  _trash_flag = true;
  return;
}

void Stuff::clean()
{
  _trash_flag = false;
  return;
}

list<Face*>& Stuff::Faces(void)
{
  return _f_vec;
}

list<Vertex*>& Stuff::Vertices(void)
{
  return _v_vec;
}

list<HalfEdge*>& Stuff::HalfEdges(void)
{
  return _he_vec;
}

void Stuff::trashV(Vertex* p)
{
  if (!p) return;
  if (!p->isDirty())
  {
    dirty();
    p->dirty();
    HalfEdge* iter = p->adjHE();
    do
    {
      trashHE(iter);
      iter = iter->sym()->next();
    } while (!iter->isDirty());
  }
  return;
}

void Stuff::trashF(Face* p)
{
  if (!p) return;
  if (!p->isDirty())
  {
    dirty();
    p->dirty();
    HalfEdge* iter = p->adjHE();
    do
    {
      iter->setFace(NULL);
      iter = iter->next();
    } while (iter != p->adjHE());
  }
  return;
}

void Stuff::trashHE(HalfEdge* p)
{
  if (!p) return;
  if (!p->isDirty())
  {
    dirty();
    p->dirty();
    p->sym()->dirty();
    if (p->adjface() != NULL) trashF(p->adjface());
    if (p->sym()->adjface() != NULL) trashF(p->sym()->adjface());
    
    p->previous()->setNext(p->sym()->next());
    p->sym()->previous()->setNext(p->next());

    //take care of vertices

    //Still lack Vertex validation checking!
    HalfEdge* testHE1 = p->adjvertex()->adjHE();
    if (testHE1 == p->sym()) 
    {
      p->adjvertex()->setHE(p->next());
    }
    testHE1 = p->adjvertex()->adjHE();
    if (testHE1->sym()->next() == testHE1) trashV(p->adjvertex());

    HalfEdge* testHE2 = p->sym()->adjvertex()->adjHE();
    if (testHE2 == p) p->sym()->adjvertex()->setHE(p->sym()->next());
    testHE2 = p->sym()->adjvertex()->adjHE();
    if (testHE2->sym()->next() == testHE2) trashV(p->sym()->adjvertex());
  }
  return;
}

void Stuff::emptyTrash(void)
{
  for (list<Vertex*>::iterator i = _v_vec.begin(); i !=  _v_vec.end();)
    if ((*i)->isDirty())
      i = _v_vec.erase(i);
    else
      ++i;
  for (list<HalfEdge*>::iterator i = _he_vec.begin(); i !=  _he_vec.end();)
    if ((*i)->isDirty())
      i = _he_vec.erase(i);
    else
      ++i;
  for (list<Face*>::iterator i = _f_vec.begin(); i !=  _f_vec.end();)
    if ((*i)->isDirty())
      i = _f_vec.erase(i);
    else
      ++i;
  clean();
  return;
}

void Stuff::paintA(const mat4& m)
{
  for (list<Face*>::const_iterator i = _f_vec.begin(); i !=  _f_vec.end(); ++i)
    if (!(*i)->isDirty())
      (*i)->paintA(m);
  return;
}

void Stuff::printStatus(void) const
{
  cout << "Faces: " << _f_vec.size() << endl;
  for (list<Face*>::const_iterator i = _f_vec.begin(); i !=  _f_vec.end(); ++i)
    (*i)->printStatus();
  cout << "Vertices: " << _v_vec.size() << endl;
  for (list<Vertex*>::const_iterator i = _v_vec.begin(); i !=  _v_vec.end(); ++i)
    (*i)->printStatus();
  cout << "HalfEdges: " << _he_vec.size() << endl;
  for (list<HalfEdge*>::const_iterator i = _he_vec.begin(); i !=  _he_vec.end(); ++i)
    (*i)->printStatus();
  cout << endl;
  return;
}

Face::Face()
:  _he_p(NULL),
  _color(vec3(0,0,0)),
  _id(0),
  _dirty(false),
  _selected(false),
  _flash(false)
{
}

Face::~Face()
{
}

void Face::setColor(double R, double G, double B)
{
  setColor(vec3(R,G,B));
  return;
}

void Face::setColor(vec3 color)
{
  _color = color;
  return;
}

void Face::setId(unsigned int id)
{
  _id = id;
  return;
}

void Face::setHE(HalfEdge* he)
{
  _he_p = he;
  return;
}

void Face::dirty(void)
{
  _dirty = true;
  return;
}

void Face::select(void)
{
  _selected = true;
  return;
}

void Face::unselect(void)
{
  _selected = false;
  return;
}

HalfEdge* Face::adjHE(void) const
{
  return _he_p;
}

HalfEdge* Face::HEtoVertex(Vertex* v) const
{
  HalfEdge* iter = _he_p;
  do
  {
    if (iter->adjvertex() == v) return iter;
    iter = iter->next();
  }
  while (iter != _he_p);
  return NULL;
}

unsigned int Face::getId(void) const
{
  return _id;
}

vec3 Face::getColor(void) const
{
  return _color;
}
bool Face::isTriangle(void) const
{
  return (numOfVertices() == 3);
}

bool Face::isQuad(void) const
{
  return (numOfVertices() == 4);
}

unsigned int Face::numOfVertices(void) const
{
  if (_he_p == NULL) return 0;
  HalfEdge* iter = _he_p->next();
  unsigned int counter = 1;
  while (iter != _he_p)
  {
    iter = iter->next();
    ++counter;
    if (iter == NULL) return 0;
  }
  return counter;

}

bool Face::isDirty(void) const
{
  return _dirty;
}

bool Face::isSelected(void) const
{
  return _selected;
}

vec3 Face::Normal(void) const
{
  double a = 0,b = 0,c = 0;
  double lastx, lasty, lastz;
  double currx, curry, currz;
  HalfEdge* iter = adjHE();
  lastx = iter->adjvertex()->getX();
  lasty = iter->adjvertex()->getY();
  lastz = iter->adjvertex()->getZ();
  do
  {
    iter = iter->next();
    currx = iter->adjvertex()->getX();
    curry = iter->adjvertex()->getY();
    currz = iter->adjvertex()->getZ();
    a += (lasty - curry) * (lastz + currz);
    b += (lastz - currz) * (lastx + currx);
    c += (lastx - currx) * (lasty + curry);
    lastx = currx;
    lasty = curry;
    lastz = currz;
  } while (iter != adjHE());
  return (vec3(a,b,c)).normalize();
  /*
  HalfEdge* iter = adjHE();
  vec3 p[4];
  for (unsigned int i = 0; i < 4; ++i)
    p[i] = iter->adjvertex()->getCoord();
  vec3 result = normal(p[0],p[1],p[2]) + normal(p[1],p[2],p[3]);
  return result.normalize();
  */

}

bool Face::isPlanar(double epsilon) const
{
  vec3 normal = Normal();
  double maxdotproduct = 0;
  double lastx, lasty, lastz;
  double currx, curry, currz;
  HalfEdge* iter = adjHE();
  lastx = iter->adjvertex()->getX();
  lasty = iter->adjvertex()->getY();
  lastz = iter->adjvertex()->getZ();
  do
  {
    iter = iter->next();
    currx = iter->adjvertex()->getX();
    curry = iter->adjvertex()->getY();
    currz = iter->adjvertex()->getZ();
    double adhocproduct = abs(normal[0] * (currx - lastx) + normal[1] * (curry - lasty) + normal[2] * (currz - lastz));
    if (adhocproduct > maxdotproduct) maxdotproduct = adhocproduct;
    lastx = currx;
    lasty = curry;
    lastz = currz;
  } while (iter != adjHE());
  if (maxdotproduct > epsilon) return false;
  else return true;
}

double Face::rayIntersection(vec3 _start, vec3 _end) const
{
  double a = 0,b = 0,c = 0;
  double last[3];
  double curr[3];
  double culx = 0, culy = 0, culz = 0;
  unsigned counter = 0;

  //flatten
  unsigned int _in = 0;

  double ax,ay,bx,by,cx,cy;

  HalfEdge* iter = adjHE();
  last[0] = iter->adjvertex()->getX();
  last[1] = iter->adjvertex()->getY();
  last[2] = iter->adjvertex()->getZ();
  do
  {
    iter = iter->next();
    curr[0] = iter->adjvertex()->getX();
    curr[1] = iter->adjvertex()->getY();
    curr[2] = iter->adjvertex()->getZ();

    culx += curr[0];
    culy += curr[1];
    culz += curr[2];

    a += (last[1] - curr[1]) * (last[2] + curr[2]);
    b += (last[2] - curr[2]) * (last[0] + curr[0]);
    c += (last[0] - curr[0]) * (last[1] + curr[1]);

    last[0] = curr[0];
    last[1] = curr[1];
    last[2] = curr[2];
    ++counter;
  } while (iter != adjHE());
  culx /= counter;
  culy /= counter;
  culz /= counter;
  double d = - a * culx - b * culy - c * culz;
  double x1 = _start[0];
  double y1 = _start[1];
  double z1 = _start[2];

  double x2 = _end[0];
  double y2 = _end[1];
  double z2 = _end[2];

  double t = (a * x1 + b * y1 + c * z1 + d)/(a * (x1 - x2) + b * (y1 - y2) + c * (z1 - z2));
  //test
  /*
  cout << "start: " << _start[0]<<' '<< _start[1]<<' '<< _start[2] << endl;
  cout << "end: " << _end[0]<<' '<< _end[1]<<' '<< _end[2] << endl;
  cout << "t: " << t << endl;
  */
  //test
  //cout << getId() << "  " << t << endl;

  if (!(t >= 0) && !(t < 0)) return NAN;
  double newt[3];
  newt[0] = (1 - t) * x1 + t * x2;
  newt[1] = (1 - t) * y1 + t * y2;
  newt[2] = (1 - t) * z1 + t * z2;
  //cout << "t: " << newt[0] << " " << newt[1] << " " << newt[2] << endl;
  iter = adjHE();
  last[0] = iter->adjvertex()->getX();
  last[1] = iter->adjvertex()->getY();
  last[2] = iter->adjvertex()->getZ();
  do
  {
    iter = iter->next();
    curr[0] = iter->adjvertex()->getX();
    curr[1] = iter->adjvertex()->getY();
    curr[2] = iter->adjvertex()->getZ();
    /*
    cout << "lastx " << last[0] << "; lasty " << last[1] << "; lastz " << last[2]  << endl;
    cout << "currx " << curr[0] << "; curry " << curr[1] << "; currz " << curr[2]  << endl;
    */
    //Check If Inside Strip
    /*
    for (unsigned int i = 0; i < 3; ++i)
    {
    */
    //Use which i?
    //By Default we use i = 2, which means collapse Z. You can't use it if Z * (a,b,c) = 0
    unsigned int i = 2;
    if (abs(c) < 0.001)
    {
      i = 0;
      // We use i = 1, which means collapse X. You can't use it if X * (a,b,c) = 0
      if (abs(a) < 0.001)
        i = 1;
    }
    //cout << "i = " << i << endl;
    //ignore i, i+1 == infinity
    //sorted by [i+2]
    if (last[(i+2)%3] >= curr[(i+2)%3])
    {
      bx = last[(i+1)%3];
      by = last[(i+2)%3];
      ax = curr[(i+1)%3];
      ay = curr[(i+2)%3];
    }
    else
    {
      ax = last[(i+1)%3];
      ay = last[(i+2)%3];
      bx = curr[(i+1)%3];
      by = curr[(i+2)%3];
    }
    cx = newt[(i+1)%3];
    cy = newt[(i+2)%3];
    //test
    /*
    cout << "ax " << ax << "; bx " << bx << "; cx " << cx  << endl;
    cout << "ay " << ay << "; by " << by << "; cy " << cy  << endl;
    */
    if (ay < cy)
    {
      if (by >= cy)
      {
      //cout << "haha ";
      //cout << "right:" << (cx-ax)*(by-ay)-(bx-ax)*(cy-ay) << endl;
      //on the right?
        if ((cx-ax)*(by-ay)-(bx-ax)*(cy-ay) > 0)
          ++_in;
      }
    }
    /*
    }
    */

    last[0] = curr[0];
    last[1] = curr[1];
    last[2] = curr[2];
    //cout << "rlastx " << last[0] << "; lasty " << last[1] << "; lastz " << last[2]  << endl;
    //cout << "rcurrx " << curr[0] << "; curry " << curr[1] << "; currz " << curr[2]  << endl;
  } while (iter != adjHE());
  //test
  //cout << "_in: " << _in << endl;

  if (_in % 2 == 1)
    return t;
  else
    return NAN;
}

bool Face::paintA(const mat4& m)
{
  if (isDirty()) return false;
  if (isTriangle())
  {
    HalfEdge* p1 = adjHE();
    HalfEdge* p2 = p1->next();
    HalfEdge* p3 = p2->next();
    vec4 v1 = vec4(p1->adjvertex()->getCoord(),1);
    vec4 v2 = vec4(p2->adjvertex()->getCoord(),1);
    vec4 v3 = vec4(p3->adjvertex()->getCoord(),1);
    v1 = m * v1;
    v2 = m * v2;
    v3 = m * v3;

    //Draw!
    if (isSelected())
    {
      if (_flash)
        glColor3f(_color[0]*0.5,_color[1]*0.5,_color[2]*0.5);
      else
       glColor3f(_color[0],_color[1],_color[2]);
      _flash = !_flash;
    }
    else
      glColor3f(_color[0],_color[1],_color[2]);
    glBegin(GL_TRIANGLES);
      glVertex3f(v1[0],v1[1],v1[2]);
      glVertex3f(v2[0],v2[1],v2[2]);
      glVertex3f(v3[0],v3[1],v3[2]);
    glEnd();

    return true;
  }
  if (isQuad())
  {
    HalfEdge* p1 = adjHE();
    HalfEdge* p2 = p1->next();
    HalfEdge* p3 = p2->next();
    HalfEdge* p4 = p3->next();
    vec4 v1 = vec4(p1->adjvertex()->getCoord(),1);
    vec4 v2 = vec4(p2->adjvertex()->getCoord(),1);
    vec4 v3 = vec4(p3->adjvertex()->getCoord(),1);
    vec4 v4 = vec4(p4->adjvertex()->getCoord(),1);
    v1 = m * v1;
    v2 = m * v2;
    v3 = m * v3;
    v4 = m * v4;

    //Draw!

    if (isSelected())
    {
      if (_flash)
        glColor3f(_color[0]*0.5,_color[1]*0.5,_color[2]*0.5);
      else
       glColor3f(_color[0],_color[1],_color[2]);
      _flash = !_flash;
    }
    else
      glColor3f(_color[0],_color[1],_color[2]);
    glBegin(GL_QUADS);
      glVertex3f(v1[0],v1[1],v1[2]);
      glVertex3f(v2[0],v2[1],v2[2]);
      glVertex3f(v3[0],v3[1],v3[2]);
      glVertex3f(v4[0],v4[1],v4[2]);
    glEnd();
    return true;
  }
  if (numOfVertices() > 4)
  {
    HalfEdge* iter = adjHE();
    if (isSelected())
    {
      if (_flash)
        glColor3f(_color[0]*0.5,_color[1]*0.5,_color[2]*0.5);
      else
       glColor3f(_color[0],_color[1],_color[2]);
      _flash = !_flash;
    }
    else
      glColor3f(_color[0],_color[1],_color[2]);
    glBegin(GL_POLYGON);
    do
    {
      vec3 t = m * (iter->adjvertex()->getCoord());
      glVertex3f(t[0],t[1],t[2]);
      iter = iter->next();
    } while (iter != adjHE());
    glEnd();
    return true;
  }
  return false;
}

void Face::printStatus(void) const
{
  cout << "Face " << getId();
  cout << " Corr. HE: " << adjHE()->getId();
  cout << " Color: ";
  cout << _color[0] << ' ';
  cout << _color[1] << ' ';
  cout << _color[2] << ' ';
  cout << endl;
  HalfEdge* iter = _he_p;
  do{
    cout << "-" << iter->getId();
    cout << " -> (";
    cout << iter->adjvertex()->getId() << ") ";
    iter = iter->next();
  } while (iter != _he_p);
  cout << endl;

  return;
}

Vertex::Vertex()
:_he_p(NULL),
 _coord(vec3(0,0,0)),
 _id(0),
 _dirty(false)
{
}

Vertex::Vertex(double x, double y, double z)
:_he_p(NULL),
 _coord(vec3(x,y,z)),
 _id(0),
 _dirty(false)
{
}

Vertex::Vertex(vec3 p)
:_he_p(NULL),
 _coord(p),
 _id(0),
 _dirty(false)
{
}

Vertex::~Vertex()
{
}

void Vertex::setId(unsigned int id)
{
  _id = id;
  return;
}

void Vertex::setHE(HalfEdge* he)
{
  _he_p = he;
  return;
}

void Vertex::setCoord(const vec3& coord)
{
  _coord = coord;
  return;
}

void Vertex::dirty(void)
{
  _dirty = true;
  return;
}

unsigned int Vertex::getId(void) const
{
  return _id;
}

HalfEdge* Vertex::adjHE(void) const
{
  return _he_p;
}

vec3 Vertex::getCoord(void) const
{
  return _coord;
}

bool Vertex::isDirty(void) const
{
  return _dirty;
}

HalfEdge* Vertex::toV(Vertex* v) const
{
  HalfEdge* iter = _he_p;
  do
  {
    if (iter->adjvertex() == v) return iter;
    iter = iter->sym()->next();
  } while (iter != _he_p);
  return NULL;
}


double Vertex::getX(void) const
{
  return _coord[0];
}

double Vertex::getY(void) const
{
  return _coord[1];
}

double Vertex::getZ(void) const
{
  return _coord[2];
}

void Vertex::printStatus(void) const
{
  cout << "Vertex " << getId();
  cout << " Corr. HE: " << adjHE()->getId();
  cout << " " << getX();
  cout << " " << getY();
  cout << " " << getZ();
  cout << endl;
}

HalfEdge::HalfEdge()
:_f_p(NULL),
 _v_p(NULL),
 _next(NULL),
 _sym(NULL),
 _id(0),
 _dirty(false)
{
}

HalfEdge::~HalfEdge()
{
}

void HalfEdge::setId(unsigned int id)
{
  _id = id;
  return;
}

void HalfEdge::setFace(Face* f)
{
  _f_p = f;
  return;
}

void HalfEdge::setVertex(Vertex* v)
{
  _v_p = v;
  return;
}

void HalfEdge::setNext(HalfEdge* he)
{
  _next = he;
  return;
}

void HalfEdge::setSym(HalfEdge* he)
{
  _sym = he;
  return;
}

void HalfEdge::dirty(void)
{
  _dirty = true;
  return;
}

HalfEdge* HalfEdge::next(void) const
{
  return _next;
}

HalfEdge* HalfEdge::previous(void) const
{
  HalfEdge* iter = sym();
  while(true)
  {
    if (iter->next() == this) return iter;
    iter = iter->next()->sym();
  }
}

HalfEdge* HalfEdge::sym(void) const
{
  return _sym;
}

Face* HalfEdge::adjface(void) const
{
  return _f_p;
}

Vertex* HalfEdge::adjvertex(void) const
{
  return _v_p;
}

bool HalfEdge::isDirty(void) const
{
  return _dirty;
}

bool HalfEdge::isBoundary(void) const
{
  return (adjface() == NULL);
}

unsigned int HalfEdge::getId(void) const
{
  return _id;
}

void HalfEdge::printStatus(void) const
{
  cout << "HE " << getId();
  if (!adjface())
    cout << " Corr. Face: NULL"; 
  else
    cout << " Corr. Face " << adjface()->getId();
  cout << " Corr. Ver. " << adjvertex()->getId();
  cout << " Corr. Next " << next()->getId();
  cout << " Corr. Sym " << sym()->getId();
  cout << endl;
}

