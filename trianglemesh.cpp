#include "trianglemesh.h"

TriangleMesh::TriangleMesh()
{
    first_vertex = NULL;
    edgeVector = new std::vector<HE_edge>();
    maxVector = new QVector3D(0.0,0.0,0.0);
    minVector = new QVector3D(0.0,0.0,0.0);
}

TriangleMesh::~TriangleMesh(){
    delete edgeVector;
    delete maxVector;
    delete minVector;
}

