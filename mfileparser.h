#ifndef MFILEPARSER_H
#define MFILEPARSER_H

#include "trianglemesh.h"
#include "viewportwidget.h"
#include <QtWidgets>
#include <map>
#include <QMatrix4x4>

class MFileParser
{
public:
    MFileParser();
    TriangleMesh getTriangleMesh(QString fileName);
    TriangleMesh* parseFile(QString fileName);
    void scaleAndMoveToOrigin(QVector3D scaleV,
                              QVector3D transV,
                              QVector3D* vertV);
    TriangleMesh::Normal* calculateFaceNormal(TriangleMesh::HE_face* face);
    TriangleMesh::Normal* calculateVertexNormal(QList<TriangleMesh::HE_face*> faces);
};

#endif // MFILEPARSER_H
