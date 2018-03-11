#ifndef _STUFF_H_
#define _STUFF_H_

#include <vector>
#include <list>
#include <map>
#include "algebra3.h"

using std::vector;
using std::list;

class Stuff;
class HalfEdge;
class Vertex;
class Face;

class Stuff  // a.k.a Mesh
{
  public:
    Stuff();
    ~Stuff();
    
    //Predefined functions to change the Mesh to a specific Geometry
    void toTriangle(vec3 p1, vec3 p2, vec3 p3, vec3 color);
    void toQuad(vec3 p1, vec3 p2, vec3 p3, vec3 p4, vec3 color);
    void toCube(unsigned int level = 100);

    //Make it empty
    void clear();

    //Primitive Functions:
    void addV(Vertex* p);
    void addF(Face* p);
    void addHE(HalfEdge* p);

    //Secondary Functions: (Leave them to Geometer)
    bool addVertexToHalfEdge(HalfEdge* he, Vertex* & nv, HalfEdge* & nhe, double ratio = 0.5);
    bool addVertexToHalfEdge(HalfEdge* he, double ratio = 0.5);
    bool splitVertices(Face* f1, Face* f2, Vertex* v, HalfEdge* & nhe);
    bool splitVertices(Face* f1, Face* f2, Vertex* v);
    bool splitQuad(Face* f, Vertex* v, HalfEdge* & Ov);
    bool splitQuad(Face* f, Vertex* v);
    bool fillFace(HalfEdge* p, vec3 color);

    // The following functions are for deleting vertices. Leave them alone.
    bool isDirty() const; 
    void dirty();
    void clean();

    // Return the Faces, Vertices and HalfEdges in case you need to use them in MainWindow
    list<Face*>& Faces(void);
    list<Vertex*>& Vertices(void);
    list<HalfEdge*>& HalfEdges(void);
    
    //Trash functions. Leave them alone.
    void trashV(Vertex* p);
    void trashF(Face* p);
    void trashHE(HalfEdge* p);
    
    void emptyTrash(void);

    // Paint Function. Notice we don't need the Matrix m any more. Change it accordingly.
    void paintA(const mat4& m);

    // For Test only.
    void printStatus(void) const;

  private:
    list<Face*> _f_vec;  //list of Faces
    list<Vertex*> _v_vec; // list of Vertices
    list<HalfEdge*> _he_vec; // list of HalfEdges

    int _max_f_num; // The Maximum ID of Faces
    int _max_v_num; // The Maximum ID of Vertices
    int _max_he_num; // The Maximum ID of HalfEdges

    bool _trash_flag; // Whether all trashable F,V,HEs are trashed. Ignore it.
};

class Face
{
  public:
    Face();
    ~Face();
    //Seting Functions. Might be useful for Visualizer
    void setColor(double R, double G, double B);
    void setColor(vec3 color);
    void setId(unsigned int id);
    void setHE(HalfEdge* he);

    //Ignore the following functions
    void dirty(void);
    void select(void);
    void unselect(void);

    HalfEdge* adjHE(void) const;
    HalfEdge* HEtoVertex(Vertex* v) const;

    //Get Functions. Might be helpful
    unsigned int getId(void) const;
    vec3 getColor(void) const;
    bool isTriangle(void) const;
    bool isQuad(void) const;
    unsigned int numOfVertices(void) const;
    bool isDirty(void) const;
    bool isSelected(void) const;
 
    //Return Normal
    vec3 Normal(void) const;

    //Return isPlannar
    bool isPlanar(double epsilon = 0.01) const;

    //Ignore this
    double rayIntersection(vec3 _start, vec3 _end) const;
 
    //need to change it to no-parameter version, i.e. gluLookat Version
    bool paintA(const mat4& m);

    //test
    void printStatus(void) const;

  private:
    HalfEdge* _he_p;
    vec3 _color;
    unsigned int _id;

    //ignore the rest
    bool _dirty;
    bool _selected;
    bool _flash;
};

class Vertex
{
  public:
    Vertex();
    Vertex(double x, double y, double z);
    Vertex(vec3 p);
    ~Vertex();

    // Set Functions
    void setId(unsigned int id);
    void setHE(HalfEdge* he);
    void setCoord(const vec3& coord);
    void dirty();

    // Get Functions
    unsigned int getId() const;
    HalfEdge* adjHE(void) const;
    vec3 getCoord(void) const;
    bool isDirty(void) const;
    HalfEdge* toV(Vertex* v) const;
    double getX() const;
    double getY() const;
    double getZ() const;

    //Test Only
    void printStatus(void) const;
  private:
    HalfEdge* _he_p;
    vec3 _coord; //Coordinate
    unsigned int _id;

    //Ignore this
    bool _dirty;
};

class HalfEdge
{
  public:
    HalfEdge();
    ~HalfEdge();

    //Set Functions
    void setId(unsigned int id);
    void setFace(Face* f);
    void setVertex(Vertex* v);
    void setNext(HalfEdge* he);
    void setSym(HalfEdge* he);
    void dirty(void);

    //Get Functions
    HalfEdge* next(void) const;
    HalfEdge* previous(void) const;
    HalfEdge* sym(void) const;
    Face* adjface(void) const;
    Vertex* adjvertex(void) const;
    bool isDirty(void) const;
    bool isBoundary(void) const;
    unsigned int getId() const;

    //Test
    void printStatus(void) const;
  private:
    Face* _f_p;
    Vertex* _v_p;
    HalfEdge* _next;
    HalfEdge* _sym;
    unsigned int _id;

    //Ignore
    bool _dirty;

};

#endif
