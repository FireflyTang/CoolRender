//
//  Mesh.hpp
//  CoolRender
//
//  Created by 唐如麟 on 15/7/22.
//  Copyright © 2015年 唐如麟. All rights reserved.
//

#pragma once

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Point_3.h>
#include <CGAL/Point_2.h>
#include <CGAL/Vector_3.h>
#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits.h>
#include <CGAL/AABB_triangle_primitive.h>
#include <CGAL/boost/graph/graph_traits_Polyhedron_3.h>
#include "Macro.hpp"
#include "Material.hpp"

#include "Color.hpp"

class Group_t;
class Face_t;
class Intersection_t;
class Mesh_t;

struct Face_primitive {
public:
    typedef Face_t * Id;
    typedef Kernel::Point_3    Point;
    typedef Kernel::Triangle_3 Datum;
private:
    typedef std::vector<Face_t>::iterator Face_Iterator;
public:
    Face_primitive(Face_Iterator it): m_pt(&(*it)) {}
    const  Id & id() const;
    const  Datum & datum() const;
    const  Point & reference_point() const;
private:
    Id m_pt;
};

class Mesh_t{
public:
    Mesh_t(const std::string & inObjFilepath);
    std::vector<Intersection_t>  QueryIntersections(const Ray_3 & Query) const;
	void  SetMaterial(const std::string & inGroupName,const Material_t & inMaterial,const Color_t & inColor);
public:
    std::string ObjFilepath;
    std::string MtlFilepath;
    std::map<std::string,Group_t> Groups;
    std::vector<Point_3> Vertexes;
    std::vector<Point_2> VertexTextures;
    std::vector<Vector_3> VertexNormals;
    std::vector<Face_t> Faces;
private:
    typedef CGAL::AABB_traits<Kernel, Face_primitive> AABB_face_traits;
    typedef CGAL::AABB_tree<AABB_face_traits> AABB_Tree;
    AABB_Tree _Tree;
};

class Vertex_t{
public:
    Vertex_t(const Mesh_t & inMesh,const int inVertexPositionIndex,const int inVertexTextureIndex,const int inVertexNormalIndex):
    _VertexPositionIndex(inVertexPositionIndex), _VertexTextureIndex(inVertexTextureIndex), _VertexNormalIndex(inVertexNormalIndex),_pMesh(&inMesh){};
    
    const  Point_3 & Position() const {return _pMesh->Vertexes[_VertexPositionIndex];};
    const  Point_2 & Texture() const {return _pMesh->VertexTextures[_VertexTextureIndex];};
    const  Vector_3 & Normal() const {return _pMesh->VertexNormals[_VertexNormalIndex];};
    int  PositionIndex() {return _VertexPositionIndex;};
    int  TextureIndex() {return _VertexTextureIndex;};
    int  NormalIndex() {return _VertexNormalIndex;};
private:
    int _VertexPositionIndex;
    int _VertexTextureIndex;
    int _VertexNormalIndex;
    const Mesh_t * _pMesh;
};


class Face_t{
public:
    Face_t(const Mesh_t & inMesh,const unsigned long inFaceIndex):_Faceindex(inFaceIndex), _pMesh(&inMesh){};
    std::vector<Vertex_t> Vertexes;
    Vector_3 Normal;
    Triangle_3 Triangle;
    Material_t Material;
private:
    unsigned long _Faceindex;
    const Mesh_t * _pMesh = nullptr;
};

class Group_t{
public:
    Group_t(const Mesh_t & inMesh,const std::string & inGroupName):GroupName(inGroupName),_pMesh(&inMesh){};
    
    std::string GroupName;
    std::vector<unsigned long> FacesIndexes;
private:
    const Mesh_t * _pMesh = nullptr;
};

class Intersection_t{
public:
#ifdef __DRAW__
    Intersection_t(){};
#endif
    Intersection_t(const Point_3 & inPoint,const Face_t & inFace): Point(inPoint),pFace(&inFace){};
    Point_3 Point;
    const Face_t * pFace = nullptr;
};

