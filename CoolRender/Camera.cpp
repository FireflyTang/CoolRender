//
//  Camera.cpp
//  CoolRender
//
//  Created by 唐如麟 on 15/7/23.
//  Copyright © 2015年 唐如麟. All rights reserved.
//

#include "Camera.hpp"

#define M_PI 3.1415926
#include <math.h>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Point_3.h>
#include <CGAL/Vector_3.h>
#include "Macro.hpp"

Camera_t::
Camera_t(Point_3 inCenter,
	Point_3 inLookat,
	Vector_3 inHead,
	double inProjectionDistance,
	int inResolutionX,
	int inResolutionY,
	int inResamplePixels
) :
	Center(inCenter),
	Lookat(inLookat),
	Head(inHead),
	ProjectDistance(inProjectionDistance),
	ResolutionX(inResolutionX),
	ResolutionY(inResolutionY),
	ResamplePixels(inResamplePixels) {
	Vector_3 Vector = NORMALIZE(Lookat-Center);
	Head = NORMALIZE(Head);
	Right = CGAL::cross_product(Vector, Head);
	Right = NORMALIZE(Right);
	Ratio = ((double)ResolutionX) / ResolutionY;
	Point_3 ProjectionCenter = Center + Vector * ProjectDistance;
	double halflengthx, halflengthy;
	halflengthx = ProjectDistance * tan(AngleX / 2 / 180 * M_PI);
	halflengthy = halflengthx / Ratio;
	ResampleResolutionX = ResolutionX * (ResamplePixels * 2 + 1);
	ResampleResolutionY = ResolutionY * (ResamplePixels * 2 + 1);
	PixelDistanceX = 2 * halflengthx / (ResampleResolutionX - 1);
	PixelDistanceY = 2 * halflengthy / (ResampleResolutionY - 1);
	ProjectionPlane.push_back(ProjectionCenter + halflengthy * Head - halflengthx * Right);
	ProjectionPlane.push_back(ProjectionCenter + halflengthy * Head + halflengthx * Right);
	ProjectionPlane.push_back(ProjectionCenter - halflengthy * Head + halflengthx * Right);
	ProjectionPlane.push_back(ProjectionCenter - halflengthy * Head - halflengthx * Right);
};