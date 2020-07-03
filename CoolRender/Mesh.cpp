//
//  Mesh.cpp
//  CoolRender
//
//  Created by 唐如麟 on 15/7/22.
//  Copyright © 2015年 唐如麟. All rights reserved.
//

#include "Mesh.hpp"
const
Face_primitive::Datum&
Face_primitive::
datum() const {
	return m_pt->Triangle;
}

const
Face_primitive::Id&
Face_primitive::
id() const {
	return m_pt;
}

const
Face_primitive::Point&
Face_primitive::
reference_point() const {
	return m_pt->Vertexes[0].Position();
}


Mesh_t::
Mesh_t(const std::string& inObjFilepath) :ObjFilepath(inObjFilepath) {
	std::fstream ObjFile(ObjFilepath, std::ios::in);
	if (!ObjFile) {
		perror("obj file open error!");
		exit(-1);
	};
	std::string Empty;
	std::string Line;
	std::stringstream LineStream;

	//skip two lines
	getline(ObjFile, Empty);
	getline(ObjFile, Empty);
	//read mtllib
	getline(ObjFile, Line);
	LineStream.clear();
	LineStream.str(Line);
	LineStream >> Empty;
	LineStream >> MtlFilepath;
	MtlFilepath = ObjFilepath.substr(0, ObjFilepath.find_last_of('/') + 1) + MtlFilepath;

	std::string Commd;
	std::string GroupName;
	Group_t* CurrentGroup = nullptr;
	while (true) {
		getline(ObjFile, Line);
		if (Line.empty()) break;
		LineStream.clear();
		LineStream.str(Line);
		LineStream >> Commd;
		//group
		if (!strcmp(Commd.data(), "g")) {
			LineStream >> GroupName;
			//only affect faces
			//if(strcmp(GroupName.data(),"default")){
			CurrentGroup = &(Groups.insert(std::pair<std::string, Group_t>(GroupName, Group_t(*this, GroupName))).first->second);
			std::cout << "Group: " << GroupName << std::endl;
			//}
		}
		if (!strcmp(Commd.data(), "v")) {
			double x, y, z;
			LineStream >> x >> y >> z;
			Vertexes.push_back(Point_3(x, y, z));
		}
		if (!strcmp(Commd.data(), "vt")) {
			double x, y;
			LineStream >> x >> y;
			VertexTextures.push_back(Point_2(x, y));
		}
		if (!strcmp(Commd.data(), "vn")) {
			double x, y, z;
			LineStream >> x >> y >> z;
			VertexNormals.push_back(Vector_3(x, y, z));
		}
		if (!strcmp(Commd.data(), "f")) {
			int a, b, c;
			char empty;
			unsigned long faceindex = Faces.size();
			Face_t face(*this, faceindex);
			while (!LineStream.eof()) {
				LineStream >> a >> empty >> b >> empty >> c;
				face.Vertexes.push_back(Vertex_t(*this, a - 1, b - 1, c - 1));
			}
			Vector_3 normal(0, 0, 0);
			for (auto& v : face.Vertexes) {
				normal = normal + v.Normal();
			}
			normal = (normal / face.Vertexes.size());
			face.Normal = NORMALIZE(normal);
			Triangle_3 triangle = Triangle_3(face.Vertexes[0].Position(), face.Vertexes[1].Position(), face.Vertexes[2].Position());
			face.Triangle = triangle;
			Faces.push_back(face);
			CurrentGroup->FacesIndexes.push_back(faceindex);
		}
	}
	_Tree.insert(Faces.begin(), Faces.end());
	_Tree.build();
}

std::vector<Intersection_t>
Mesh_t::
QueryIntersections(const Ray_3& Query) const {
	typedef boost::optional<AABB_Tree::Intersection_and_primitive_id<Segment_3>::Type> intersection_t;
	std::vector<intersection_t> intersections;
	std::vector<Intersection_t> results;
	_Tree.all_intersections(Query, std::back_inserter(intersections));
	for (auto i : intersections) {
		if (Point_3* p = boost::get<Point_3>(&(i->first))) {
			results.push_back(Intersection_t(*p, *i->second));
		}
	}
	return results;
}

void
Mesh_t::
SetMaterial
(const std::string& inGroupName, const Material_t& inMaterial, const Color_t& inColor) {
	std::map<std::string, Group_t>::iterator igroup = Groups.find(inGroupName);
	if (igroup == Groups.end()) {
		perror("no such group");
		exit(-1);
	}
	for (unsigned long i : igroup->second.FacesIndexes) {
		Material_t& material = Faces[i].Material;
		material.Ka = inColor;
		material.Tf = inMaterial.Tf;
		material.Ks = inMaterial.Ks;
		material.Kd = inMaterial.Kd;
		material.Ni = inMaterial.Ni;
	}

}




