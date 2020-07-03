//
//  LightSource.hpp
//  CoolRender
//
//  Created by 唐如麟 on 15/7/24.
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

#include "Color.hpp"

class LightSources_t;
class FaceLightSource_t{
public:
    FaceLightSource_t(const LightSources_t & inLightSources):pLightSources(&inLightSources){};
    //FaceLightSource_t(const Triangle_3 & inTriangle,Vector_3 inNormal = Vector_3(0,-1,0)):Triangle(inTriangle),Normal(inNormal){};
	std::vector<Point_3> Sample(const int SampleNum) const;
public:
    Vector_3 Normal;
    double Lightness=1;
	int Shineness = 30;
    Triangle_3 Triangle;
    const LightSources_t * pLightSources = nullptr;
    Color_t Color = Color_t::White;
};

struct FaceLightSource_primitive {
public:
    typedef FaceLightSource_t * Id;
    typedef Kernel::Point_3    Point;
    typedef Kernel::Triangle_3 Datum;
private:
    typedef std::vector<FaceLightSource_t>::iterator FaceLightSource_Iterator;
public:
    //FaceLightSource_primitive() {}
    FaceLightSource_primitive(FaceLightSource_Iterator it): m_pt(&(*it)) {}
    const Id & id() const { return m_pt; }
    const Datum & datum() const {
        return m_pt->Triangle;
    }
    const Point reference_point() const {
        return m_pt->Triangle[0];
    }
private:
    Id m_pt;
};

class LightSourceIntersection_t{
public:
    LightSourceIntersection_t(){};
    LightSourceIntersection_t(const Point_3 & inPoint,const FaceLightSource_t & inFaceLightSource): Point(inPoint),pFaceLightSource(&inFaceLightSource){};
public:
    Point_3 Point;
    const FaceLightSource_t * pFaceLightSource = nullptr;
};


class LightSources_t{
private:
    typedef CGAL::AABB_traits<Kernel, FaceLightSource_primitive> AABB_FaceLightSource_traits;
    typedef CGAL::AABB_tree<AABB_FaceLightSource_traits> AABB_Tree;
public:
    LightSources_t(const std::string & inFilename);
    unsigned long Build();
    bool IfHitAnyLightSource(const Ray_3 & EyeRay) const;
    std::vector<LightSourceIntersection_t> QueryIntersections(const Ray_3 & Query) const;
    FaceLightSource_t & Insert(const FaceLightSource_t & Input);
	Color_t EnvironmentLightSource;
	Color_t HitEnvironmentLightSource (const Ray_3 EyeRay) const;
public:
    std::vector<FaceLightSource_t> FaceLightSources;
private:
    AABB_Tree _Tree;
};
