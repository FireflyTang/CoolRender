//
//  main.cpp
//  CoolRender
//
//  Created by 唐如麟 on 15/7/23.
//  Copyright © 2015年 唐如麟. All rights reserved.
//


//#include "PTRenderCore.hpp"
#include "PT_Advanced.hpp"

int main(int argc, char** argv) {

	time_t start = time(NULL);

#ifdef __DRAW__
	InitScene(argc, argv);
#endif

	Mesh_t Mesh("Resources/GlassBall.obj");
	Materials_t Materials("Resources/Materials.txt");
	Mesh.SetMaterial("pPlane1", Materials["Diffuse"],Color_t(130,219,82));
    Mesh.SetMaterial("pSphere1", Materials["Glass"], Color_t::White);
	//Mesh.SetMaterial("pPlane1", Materials["Rough_Glass"], Color_t(0,0,1));
	//Mesh.SetMaterial("pSphere1", Materials["Glass"], Color_t(1, 1, 1));
	LightSources_t LightSources("Resources/FaceLightSources.txt");
	for (auto& FaceLightSource : LightSources.FaceLightSources) {
		FaceLightSource.Shineness = 40;
	}

	Camera_t Camera(Point_3(0, 10, 20), Point_3(0, 5, 0), Vector_3(0, 2, -1), 5, 800, 600, 10);

	Scene_t Scene(Mesh, LightSources);

	Parameters_t Parameters(Scene, Camera);

	Rays_t Rays;

	PT_Phong_t PT(Parameters, Rays);
	PT.MaxRecursionDeep = 4;
	PT.FaceLightSourceSampleTimes = 5;
	//PT.DiffuseReflectionResampleTimes = 4;
	auto result = PT.PathTracing();
	PostTreatment_t PostTreatment(Camera, result);
	PostTreatment.Downsample().Output_BMP();
	   
	std::cout << std::endl << "Time Used(s):" << time(NULL) - start << std::endl;
	std::system("pause");
	return 0;
};
