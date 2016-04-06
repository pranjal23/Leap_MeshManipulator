#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

#include <QList>
#include <QMap>
#include <vector>
#include <QVector3D>
#ifdef _WIN32
    #include <Windows.h>
    #include <GL/glu.h>
#elif __APPLE__
    #include <OpenGL/glu.h>
#endif

class TriangleMesh
{
public:
    explicit TriangleMesh();
    ~TriangleMesh();
    struct HE_edge;
    struct HE_vert;
    struct HE_face;
    struct Normal;
    struct Normal {
        float x, y, z; // the vertex coordinates
    };
    struct HE_edge {
        long index; //for quick access while parsing only
        HE_vert* vert; // vertex at the end of the half-edge
        HE_edge* pair; // oppositely oriented half-edge
        HE_face* face; // the incident face
        HE_edge* prev; // previous half-edge around the face
        HE_edge* next; // next half-edge around the face
    };
    struct HE_vert {
        long index;
        float x, y, z; // the vertex coordinates
        HE_edge* edge; // one of the half-edges emanating from the vertex
        Normal* normal; //normal for the vertex
    };
    struct HE_face {
        long index;
        HE_edge* edge; // one of the half-edges bordering the face
        Normal* normal; //normal for the face
    };
    HE_vert* first_vertex;
    std::vector<HE_edge>* edgeVector;
    void copyEdge(HE_edge* in, HE_edge* out);

    //Max and Min X,Y,Z positions to draw bounding box
    QVector3D* maxVector;
    QVector3D* minVector;
};

#endif // TRIANGLEMESH_H
