#include "mfileparser.h"
#include <QString>

static const bool showDebug = false;

MFileParser::MFileParser()
{}

int getNumberOfDigits(long number)
{
    int digits = 0;
    if (number < 0) digits = 1;
    while (number) {
        number /= 10;
        digits++;
    }
    return digits;
}

struct MFileFace{
    long faceId;
    long vertex1;
    long vertex2;
    long vertex3;
};

QString debugPrintEdge(TriangleMesh::HE_edge* edge){
    return " : edge: " + QString::number(edge->index) + " , vertex: "
            + QString::number(edge->prev->vert->index)
            + " -> " + QString::number(edge->vert->index);
}

TriangleMesh MFileParser::getTriangleMesh(QString fileName){
    TriangleMesh* mesh = parseFile(fileName);
    return *mesh;
}

TriangleMesh* MFileParser::parseFile(QString fileName){
    if(showDebug)
        qDebug() << "Mesh " << fileName << " to be opened:\n";
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0,"error",file.errorString());
        return NULL;
    }

    if (!file.isReadable()) {
        QMessageBox::information(0,"Unable to read file %s, aborting\n", file.fileName().toStdString().c_str());
        return NULL;
    }

    TriangleMesh* mesh = new TriangleMesh();
    QMap<quint64,TriangleMesh::HE_vert*>* vertMap = new QMap<quint64,TriangleMesh::HE_vert*>();
    QMap<quint64,TriangleMesh::HE_face*>* faceMap = new QMap<quint64,TriangleMesh::HE_face*>();
    QMap<quint64,TriangleMesh::HE_edge*>* edgeMap = new QMap<quint64,TriangleMesh::HE_edge*>();
    QVector3D max = QVector3D(0.0,0.0,0.0);
    QVector3D min = QVector3D(0.0,0.0,0.0);

    QMultiMap<quint64,TriangleMesh::HE_face*>* vert2faceMap = new QMultiMap<quint64,TriangleMesh::HE_face*>();

    QList<MFileFace*>* faceDataList = new QList<MFileFace*>();
    long vertex_count=0;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();

        /// processing
        QString data = line.trimmed();
        if(!data.startsWith("#"))
        {
            QStringList parts = data.split(QRegExp("\\s+"));

            if(((QString)parts.at(0)).toLower()=="vertex")
            {
                TriangleMesh::HE_vert* vert = new TriangleMesh::HE_vert();
                bool ok;
                vert->index = ((QString)parts.at(1)).toLong(&ok,10);
                vert->x = ((QString)parts.at(2)).toFloat(&ok);
                vert->y = ((QString)parts.at(3)).toFloat(&ok);
                vert->z = ((QString)parts.at(4)).toFloat(&ok);

                //max and min vectors
                max.setX(qMax(max.x(),vert->x));
                max.setY(qMax(max.y(),vert->y));
                max.setZ(qMax(max.z(),vert->z));
                min.setX(qMin(min.x(),vert->x));
                min.setY(qMin(min.y(),vert->y));
                min.setZ(qMin(min.z(),vert->z));

                if(ok)
                {
                    vertMap->insert(vert->index,vert);
                    if(mesh->first_vertex==NULL){
                        mesh->first_vertex = vert;
                    }
                    vertex_count++;
                }
                else
                {
                    QMessageBox::information(0,"Unable to read file, error in type conversion %s, aborting\n", file.fileName().toStdString().c_str());
                    return NULL;
                }
            }

            if(((QString)parts.at(0)).toLower()=="face")
            {
                bool ok;
                long index = ((QString)parts.at(1)).toLong(&ok,10);
                long vertex1_id = ((QString)parts.at(2)).toLong(&ok,10);
                long vertex2_id = ((QString)parts.at(3)).toLong(&ok,10);
                long vertex3_id = ((QString)parts.at(4)).toLong(&ok,10);
                if(ok){
                    MFileFace* facedata = new MFileFace();
                    facedata->faceId = index;
                    facedata->vertex1 = vertex1_id;
                    facedata->vertex2 = vertex2_id;
                    facedata->vertex3 = vertex3_id;
                    faceDataList->append(facedata);
                }
            }
        }
    }
    file.close();

    //Find the translation vector
    float mx = (max.x()+min.x())/2;
    float my = (max.y()+min.y())/2;
    float mz = (max.z()+min.z())/2;
    QVector3D tr(-mx,-my,-mz);

    //Find the scale vector
    float lx = (max.x()-min.x());
    float ly = (max.y()-min.y());
    float lz = (max.z()-min.z());
    float ll = sqrt((lx*lx) + (ly*ly) + (lz*lz));
    if(ll==0){ll=1;}//check divide by zero just in case
    float a=1/ll;
    QVector3D sr(a,a,a);

    //Normalize vertexes and move to origin
    QMap<quint64,TriangleMesh::HE_vert*>::iterator i = vertMap->begin();
    while (i != vertMap->end()) {
        TriangleMesh::HE_vert* vert = i.value();
        QVector3D* vertV = new QVector3D(vert->x,vert->y,vert->z);
        scaleAndMoveToOrigin(sr,tr,vertV);
        vert->x = vertV->x();
        vert->y = vertV->y();
        vert->z = vertV->z();
        delete vertV;
        ++i;
    }

    //Normalize and assign the max vector
    mesh->maxVector->setX(max.x());
    mesh->maxVector->setY(max.y());
    mesh->maxVector->setZ(max.z());
    scaleAndMoveToOrigin(sr,tr,mesh->maxVector);

    //Normalize and assign the min vector
    mesh->minVector->setX(min.x());
    mesh->minVector->setY(min.y());
    mesh->minVector->setZ(min.z());
    scaleAndMoveToOrigin(sr,tr,mesh->minVector);

    int power_factor = getNumberOfDigits(vertex_count);
    long units = pow(10,power_factor) * 1000;
    long edgeCount=0;

    QListIterator<MFileFace*> iter(*faceDataList);
    while(iter.hasNext())
    {
        MFileFace* facedata = iter.next();
        long index = facedata->faceId;
        long vertex1_id = facedata->vertex1;
        long vertex2_id = facedata->vertex2;
        long vertex3_id = facedata->vertex3;

        if(showDebug)
            qDebug() <<"\n" << "Face id: " << index ;
        //<< "Vertex IDs - " << vertex1_id << "," << vertex2_id << "," << vertex3_id;

        TriangleMesh::HE_face* face = new TriangleMesh::HE_face();

        //retreive vertexes
        TriangleMesh::HE_vert* vertex_one = vertMap->value(vertex1_id,NULL);
        TriangleMesh::HE_vert* vertex_two = vertMap->value(vertex2_id,NULL);
        TriangleMesh::HE_vert* vertex_three = vertMap->value(vertex3_id,NULL);

        //create all half-edges for the face
        if(vertex_one!=NULL && vertex_two!=NULL && vertex_three!=NULL)
        {
            TriangleMesh::HE_edge* edge1 = new TriangleMesh::HE_edge();
            TriangleMesh::HE_edge* edge2 = new TriangleMesh::HE_edge();
            TriangleMesh::HE_edge* edge3 = new TriangleMesh::HE_edge();

            //first half-edge
            edge1->vert = vertex_two;
            edge1->index = ++edgeCount;
            edge1->prev = edge3;
            edge1->next = edge2;
            vertex_one->edge = edge1;
            quint64 edge_key1 =  vertex1_id * units + vertex2_id;
            edgeMap->insert(edge_key1,edge1);


            //second half-edge
            edge2->vert = vertex_three;
            edge2->index = ++edgeCount;
            edge2->prev = edge1;
            edge2->next = edge3;
            vertex_two->edge = edge2;
            quint64 edge_key2 =  vertex2_id * units + vertex3_id;
            edgeMap->insert(edge_key2,edge2);


            //third half-edge
            edge3->vert = vertex_one;
            edge3->index = ++edgeCount;
            edge3->prev = edge2;
            edge3->next = edge1;
            vertex_three->edge = edge3;
            quint64 edge_key3 =  vertex3_id * units + vertex1_id;
            edgeMap->insert(edge_key3,edge3);

            if(showDebug){
                qDebug() << "Added edge: Key- " << edge_key1 << debugPrintEdge(edge1);
                qDebug() << "Added edge: Key- " << edge_key2 << debugPrintEdge(edge2);
                qDebug() << "Added edge: Key- " << edge_key3 << debugPrintEdge(edge3);
            }

            //pairing edge 1
            quint64 pair_key1 =  vertex2_id * units + vertex1_id;
            TriangleMesh::HE_edge* pair1 = edgeMap->take(pair_key1);
            if(pair1!=NULL){
                TriangleMesh::HE_edge* pairEdge = pair1;
                if(showDebug)
                    qDebug() << "Pair found: Current edge- " << edge1->index <<  " ,Pair edge- " << pairEdge->index;
                edge1->pair = pairEdge;
                pairEdge->pair = edge1;
                edgeMap->insert(pair_key1,pairEdge);
            }
            else{
                if(showDebug)
                    qDebug() << "Pair not found: Pair key- " << pair_key1 << debugPrintEdge(edge1);
            }

            //pairing edge 2
            quint64 pair_key2 =  vertex3_id * units + vertex2_id;
            TriangleMesh::HE_edge* pair2 = edgeMap->take(pair_key2);
            if(pair2!=NULL)
            {
                TriangleMesh::HE_edge* pairEdge = pair2;
                if(showDebug)
                    qDebug() << "Pair found: Current edge- " << edge2->index <<  " ,Pair edge- " << pairEdge->index;
                edge2->pair = pairEdge;
                pairEdge->pair = edge2;
                edgeMap->insert(pair_key2,pairEdge);
            }
            else{
                if(showDebug)
                    qDebug() << "Pair not found: Pair key- " << pair_key2 << debugPrintEdge(edge2);
            }

            //Pairing edge3
            quint64 pair_key3 =  vertex1_id * units + vertex3_id;
            TriangleMesh::HE_edge* pair3 = edgeMap->take(pair_key3);
            if(pair3!=NULL)
            {
                TriangleMesh::HE_edge* pairEdge = pair3;
                if(showDebug)
                    qDebug() << "Pair found: Current edge- " << edge3->index <<  " ,Pair edge- " << pairEdge->index;
                edge3->pair = pairEdge;
                pairEdge->pair = edge3;
                edgeMap->insert(pair_key3,pairEdge);
            }
            else{
                if(showDebug)
                    qDebug() << "Pair not found: Pair key- " << pair_key3 << debugPrintEdge(edge3);
            }

            //Create the face
            face->index = index;
            face->edge = edge3;

            //Assign face to edges
            edge1->face = face;
            edge2->face = face;
            edge3->face = face;

            faceMap->insert(face->index,face);
            vert2faceMap->insertMulti(vertex1_id,face);
            vert2faceMap->insertMulti(vertex2_id,face);
            vert2faceMap->insertMulti(vertex3_id,face);
        }
    }


    //Assign surface normal
    QMap<quint64,TriangleMesh::HE_face*>::iterator iv = faceMap->begin();
    while (iv != faceMap->end()) {
        iv.value()->normal = calculateFaceNormal(iv.value());
        ++iv;
    }


    //Assign vertex normals
    QMap<quint64,TriangleMesh::HE_vert*>::iterator im = vertMap->begin();
    while (im != vertMap->end()) {
        quint64 vertIndex = im.value()->index;
        QList<TriangleMesh::HE_face*> faces = vert2faceMap->values(vertIndex);
        im.value()->normal = calculateVertexNormal(faces);
        ++im;
    }

    QMap<quint64,TriangleMesh::HE_edge*>::iterator ig = edgeMap->begin();
    while (ig != edgeMap->end()) {
        TriangleMesh::HE_edge* edge = ig.value();
        mesh->edgeVector->push_back(*edge);
        ++ig;
    }

    return mesh;
}

void MFileParser::scaleAndMoveToOrigin(
        QVector3D scaleV,
        QVector3D transV,
        QVector3D* vertV)
{
    /*Apply translation and scale to the point*/

    QMatrix4x4 A(1.0,        0.0,        0.0,        transV.x(),
                 0.0,        1.0,        0.0,        transV.y(),
                 0.0,        0.0,        1.0,        transV.z(),
                 0.0,        0.0,        0.0,        1.0);

    QMatrix4x4 B(scaleV.x(), 0.0,        0.0,        0.0,
                 0.0,        scaleV.y(), 0.0,        0.0,
                 0.0,        0.0,        scaleV.z(), 0.0,
                 0.0,        0.0,        0.0,        1.0);

    QVector3D ov(vertV->x(),vertV->y(),vertV->z());
    QVector3D tv = A * ov;
    QVector3D fv = B * tv;

    if(showDebug)
        qDebug() << "original: " << vertV->x() << "," << vertV->y() << ","  << vertV->z();

    vertV->setX(fv.x());
    vertV->setY(fv.y());
    vertV->setZ(fv.z());

    if(showDebug)
        qDebug() << "transformed: " << fv.x() << ","  << fv.y() << ","  << fv.z();
}

/**
 * @brief calculateFaceNormal
 * @param TriangleMesh::HE_face
 * @return TriangleMesh::HE_normal
 */
TriangleMesh::Normal* MFileParser::calculateFaceNormal(TriangleMesh::HE_face* face){
    TriangleMesh::HE_vert* p1 = face->edge->vert;
    TriangleMesh::HE_vert* p2 = face->edge->next->vert;
    TriangleMesh::HE_vert* p3 = face->edge->prev->vert;

    TriangleMesh::HE_vert U;
    U.x = p2->x - p1->x;
    U.y = p2->y - p1->y;
    U.z = p2->z - p1->z;

    TriangleMesh::HE_vert V;
    V.x = p3->x - p1->x;
    V.y = p3->y - p1->y;
    V.z = p3->z - p1->z;

    TriangleMesh::Normal* normal = new TriangleMesh::Normal();
    normal->x = (U.y*V.z) - (U.z*V.y);
    normal->y = (U.z*V.x) - (U.x*V.z);
    normal->z = (U.x*V.y) - (U.y*V.x);

    return normal;
}

/**
 * @brief calculateVertexNormal
 * @param QList<TriangleMesh::HE_face*>
 * @return TriangleMesh::HE_normal
 */
TriangleMesh::Normal* MFileParser::calculateVertexNormal(QList<TriangleMesh::HE_face*> faces){
    TriangleMesh::Normal* normal = new TriangleMesh::Normal();
    normal->x = 0.0f;
    normal->y = 0.0f;
    normal->z = 0.0f;

    float numOfFaces =  (float)faces.size();
    QListIterator<TriangleMesh::HE_face*> iter(faces);
    while(iter.hasNext())
    {
        TriangleMesh::Normal* faceNormal = iter.next()->normal;
        normal->x += faceNormal->x;
        normal->y += faceNormal->y;
        normal->z += faceNormal->z;
    }

    normal->x = normal->x/numOfFaces;
    normal->y = normal->y/numOfFaces;
    normal->z = normal->z/numOfFaces;

    return normal;
}

