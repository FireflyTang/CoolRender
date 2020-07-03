//
//  LightSource.cpp
//  CoolRender
//
//  Created by 唐如麟 on 15/7/24.
//  Copyright © 2015年 唐如麟. All rights reserved.
//

#include "LightSource.hpp"

FaceLightSource_t &
LightSources_t::
Insert
(const FaceLightSource_t & Input){
    //Input.pLightSources=this;
    FaceLightSources.push_back(Input);
    return FaceLightSources.back();
}

unsigned long
LightSources_t::
Build
(){
    _Tree.clear();
    _Tree.insert(FaceLightSources.begin(),FaceLightSources.end());
    _Tree.build();
    return _Tree.size();
}



LightSources_t::
LightSources_t(const std::string & inFilename){
    std::fstream LightSourcesFile(inFilename);
    if(!LightSourcesFile){
        perror("open light sources file fail");
        exit(-1);
    }
    std::string Line;
    std::stringstream LineStream;
    std::string Command;
    FaceLightSource_t * pFaceLightSource = nullptr;
    int vertexindex=0;
    double xyz[3][3];
    while(!LightSourcesFile.eof()){
        getline(LightSourcesFile, Line);
        if(Line.empty()) continue;
        LineStream.clear();
        LineStream.str(Line);
        LineStream >> Command;
        if(!strcmp(Command.data(), "v")){
            LineStream >>xyz[vertexindex][0]>>xyz[vertexindex][1]>>xyz[vertexindex][2];
            if(vertexindex==2){
                Triangle_3 temp = Triangle_3(Point_3(xyz[0][0],xyz[0][1],xyz[0][2]),Point_3(xyz[1][0],xyz[1][1],xyz[1][2]),Point_3(xyz[2][0],xyz[2][1],xyz[2][2]));
                pFaceLightSource->Triangle=temp;
            }
            vertexindex = (vertexindex+1)%3;
            continue;
        }
        if(!strcmp(Command.data(), "vn")){
            double x,y,z;
            LineStream >> x >> y >> z;
            Vector_3 temp=Vector_3(x,y,z);
            pFaceLightSource->Normal=temp;
            continue;
        }
        if(!strcmp(Command.data(), "c")){
            double R,G,B;
            LineStream >> R >> G >> B;
            pFaceLightSource->Color.R=R;
            pFaceLightSource->Color.G=G;
            pFaceLightSource->Color.B=B;
            continue;
        }
        if(!strcmp(Command.data(), "l")){
            LineStream >> pFaceLightSource->Lightness;
            continue;
        }
        if(!strcmp(Command.data(), "FaceLightSource")){
            std::cout<<"FaceLightSource"<<std::endl;
            pFaceLightSource=&Insert(FaceLightSource_t(*this));
            continue;
        }
		if (!strcmp(Command.data(), "EnvironmentLightSource")) {
			getline(LightSourcesFile, Line);
			LineStream.clear();
			LineStream.str(Line);
			LineStream >> Command;
			double R, G, B;
			LineStream >> R >> G >> B;
			EnvironmentLightSource = Color_t(R, G, B);
			std::cout << "EnvironmentLightSource:" << " "<<R<<" "<<G << " " <<B << std::endl;
		}
    }
    Build();
}

 bool
LightSources_t::
IfHitAnyLightSource
(const Ray_3& EyeRay) const {
	return _Tree.do_intersect(EyeRay);
}

 std::vector<LightSourceIntersection_t>
LightSources_t::
QueryIntersections(const Ray_3& Query) const {
	typedef boost::optional<AABB_Tree::Intersection_and_primitive_id<Segment_3>::Type> intersection_t;
	std::vector<intersection_t> intersections;
	std::vector<LightSourceIntersection_t> results;
	_Tree.all_intersections(Query, std::back_inserter(intersections));
	for (auto i : intersections) {
		if (Point_3* p = boost::get<Point_3>(&(i->first))) {
			results.push_back(LightSourceIntersection_t(*p, *i->second));
		}
	}
	return results;
}

 Color_t
LightSources_t::
HitEnvironmentLightSource(const Ray_3 EyeRay) const {
	return EnvironmentLightSource;
}

 std::vector<Point_3> 
	 FaceLightSource_t::
	 Sample(const int SampleNum) const
 {
	 std::vector<Point_3> Samples;
	 const Triangle_3& Trig = Triangle;
	 Vector_3 AB = Trig[1] - Trig[0];
	 Vector_3 BC = Trig[2] - Trig[1];
	 for (int i = 0; i < SampleNum; i++) {
		 double s = ((double)rand()) / RAND_MAX;
		 double t = sqrt(((double)rand()) / RAND_MAX);
		 Samples.push_back((Trig[0]+(t*AB+s*t*BC)));
	 }
	 return Samples;
 }
