//
//  Camera.hpp
//  CoolRender
//
//  Created by 唐如麟 on 15/7/23.
//  Copyright © 2015年 唐如麟. All rights reserved.
//

#pragma once

#include <vector>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Point_3.h>
#include <CGAL/Vector_3.h>
#include "Macro.hpp"

struct View_t {
	Point_3 Center;
	Vector_3 Vector;
	Vector_3 Head;
};

class Camera_t {
public:
	Camera_t(Point_3 inCenter,
		Point_3 inLookat,
		Vector_3 inHead,
		double inProjectionDistance,
		int inResolutionX = 800,
		int inResolutionY = 600,
		int inResamplePixels = 5
	);
	Point_3 Center;
	Point_3 Lookat;
	Vector_3 Head;
	Vector_3 Right;
	double ProjectDistance;
	double AngleX = 60;
	double AngleY;
	double Ratio;
	int ResolutionX, ResolutionY;
	double PixelDistanceX, PixelDistanceY;
	std::vector<Point_3> ProjectionPlane;
	double Aperture = 1;
	int ResamplePixels;
	int ResampleResolutionX, ResampleResolutionY;
};


