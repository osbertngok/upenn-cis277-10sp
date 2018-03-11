HW7 By Yang Yue

Check Procedure:

1.Again, this program has been tested in Ubuntu 9.10. Probably works in Mac by executing run.sh but I'm not sure.
2.Once compiled, run ./homework6.
3.Feel free to use Key_Left / Key_Right / Key_Up / Key_Down to adjust the Camera with 10 degree incremental. Use Key_PageUp / Key_PageDown to zoom in / out.
4.Choose Preset->Cube: Level 6.
5.Rotate and select any face you wish. Change the view to check the existence of the ray. Notice only the front-most face is selected and highlighted by flashing. Other faces should not be flashing even the ray goes through the face.
6.Click a point where the ray points to none of the faces. No face should be now selected.

Design Decisions:
Nothing special in this assignment. The flashing effect is done in this way not because it is flashy but because it is easy to implement. It is, however, not so user-friendly.
It is designed to be error-free no matter which angle you look at, even boundary conditions. All special cases have been carefully handled.

HW6 By Yang Yue

Check Procedure:

1.Again, this program has been tested in Ubuntu 9.10. Probably works in Mac by executing run.sh but I'm not sure.
2.Once compiled, run ./homework6.
3.Feel free to use Key_Left / Key_Right / Key_Up / Key_Down to adjust the Camera with 10 degree incremental. Use Key_PageUp / Key_PageDown to zoom in / out.

4. The following checking procedures are very tricky and complicated. Any tiny mistake could lead to failure to complete the procedure. However, the program itself has provided some checkpoints for you to rest, so relax :)

5. Abbrieviations adopted as follows:

L0 : Preset -> Cube: Level 0. Notice you can load any level at any time you want.
SQ(1,2) : Others -> Split Quad into Two, Choose Face 1 and Vertex 2.
HE(10)->AV : HalfEdges -> HalfEdge 10 -> Add Vertex.
F(1)->(1,1,0) : Faces -> Face 1 -> Change Color. Set Red as full, Green as full and Blue as empty.
V(7)->(5,-5,5) : Vertices -> Vertex 7 -> Change Coordinate, set X as 5, Y as -5, Z as 5.
SV(2,5,4) : Others -> Split Vertex into Two, Choose Face 2, Face 5 and Vertex 4.
HE(4)->FF : HalfEdge -> HalfEdge 4 -> Fill Face.
V(6)->D : Vertices -> Vertex 6 -> Delete

Ready? Go!

L0
SQ(1,2)
HE(10)->AV
SQ(2,5)
SQ(1,5)
F(1)->(1,1,0)
F(2)->(0,1,1)
F(3)->(1,0,1)
F(4)->(0,1,0)
//The instructions above is equivalent to L1
HE(15)->AV
SQ(4,4)
F(5)->(0,0,1)
//The instructions above is equivalent to L2
SV(2,5,4)
V(7)->(5,-5,5)
//The instructions above is equivalent to L3
SV(4,3,5)
V(8)->(-5,-5,5)
V(5)->(-5,5,5)
//The instructions above is equivalent to L4
V(1)->(5,5,-5)
V(2)->(-5,5,-5)
V(3)->(-5,-5,-5)
V(4)->(5,-5,-5)
//The instructions above is equivalent to L5
HE(4)->FF
//The instructions above is equivalent to L6
SQ(2,3)
SQ(4,7)
SQ(3,5)
V(6)->D
//The instructions above is equivalent to L7
V(2)->D
//The instructions above is equivalent to L8
V(4)->D
//The instructions above is equivalent to L9
HE(30)->FF
//The instructions above is equivalent to L10

L6->L10 demonstrates how to transform a cube into a tetrahedron.

Design Decisions:
1. New Dialog is a much more user-friendly approach, yet it is extremely time consuming.
2. I decide to represent boundary edges as "edges whose face is NULL" instead of having symmetric NULL halfedges. This makes traversing halfedges around a vertex possible.
3. Still need some mechanism to indicate a face or an edge on the map. Maybe next time.


------The Following is HW5-------

HW5 By Yang Yue

Check Procedure:

1.Again, this program has been tested in Ubuntu 9.10. Probably works in Mac by executing run.sh but I'm not sure.
2.Once compiled, run ./homework5.
3.Feel free to use Key_Left / Key_Right / Key_Up / Key_Down to adjust the Camera with 10 degree incremental. Use Key_PageUp / Key_PageDown to zoom in / out.

Design Decisions:

1. I have spent around half an hour to modify algebra because it's lacking of const support and the critical function, rotation3D does not take care of all situations (maybe it assumes zero vector won't be passed as parameters).

2. There are two versions of Cameras in this homework. The first one (default) uses traditional approach -- translate and rotate twice. The second one uses the orignal implementation of gluLookat, which is more efficient but probably not up to the standard of this assignment, and I left it as a backup file (Camera.cpp.bak2)

3. Up Vector should be always keep unitized, otherwise it could grow as you try to change your eye vector and become extremely large and finally exceed the size of a double.
