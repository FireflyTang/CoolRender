//
//  PTRenderCore.hpp
//  CoolRender
//
//  Created by 唐如麟 on 15/7/23.
//  Copyright © 2015年 唐如麟. All rights reserved.
//
#pragma once
#include <memory>

#include "Scene.hpp"
#include "Parameters.hpp"


class Light_t {
public:
	Light_t(const Vector_3& inVector = CGAL::NULL_VECTOR, const Color_t& inColor = Color_t::Black) :Color(inColor), Vector(inVector) {};
public:
	Vector_3 Vector;
	Color_t Color;
#ifdef __DRAW__
	Intersection_t LastIntersection;
	bool isHitLightSource = false;
	LightSourceIntersection_t LightSourceIntersection;
#endif
};



class Rays_t {
public:
	virtual 
		Ray_3
		CalcRefractedRay(const Parameters_t&, const Ray_3&, const Intersection_t&) const;
	virtual 
		Ray_3
		CalcMirrorReflectedRay(const Parameters_t&, const Ray_3&, const Intersection_t&) const ;
	virtual 
		Ray_3
		CalcDiffuseReflectedRay(const Parameters_t&, const Ray_3&, const Intersection_t&) const;
};

class PT_t {
public:
	Parameters_t& Parameters;
	Rays_t& Rays;
	int MaxRecursionDeep = 10;
	int DiffuseReflectionResampleTimes = 5;
	bool IfAllowTotalReflection = true;

	PT_t(Parameters_t& inParameters, Rays_t& inRays) :Parameters(inParameters), Rays(inRays) {};
	virtual 
		bool
		IfTotalReflection(const Ray_3&, const Intersection_t&) const;
	virtual 
		bool
		IfAllowRefraction(const Ray_3&, const Intersection_t&, const int, bool) const;
	virtual 
		bool
		IfAllowMirrorReflection(const Ray_3&, const Intersection_t&, const int, bool) const;

	virtual 
		bool
		IfAllowDiffuseReflection(const Ray_3&, const Intersection_t&, const int) const;
	virtual 
		Light_t
		HitLightSource(const Ray_3&, const LightSourceIntersection_t&) const;
	virtual 
		Color_t
		HitEnvironmernt(const Ray_3&) const;

	virtual 
		int
		GetDiffuseReflectionResampleTimes(const Ray_3&, const Intersection_t&, const int) const;
	virtual 
		Light_t
		CalcReturnLight(const Ray_3&, const Intersection_t&, const Light_t&, const std::vector<Light_t>&, const Light_t&, bool) const;
	virtual 
		boost::optional<Intersection_t>
		SelectIntersection
		(const Ray_3& EyeRay, const std::vector<Intersection_t>& Intersections) const;
	virtual 
		boost::optional<LightSourceIntersection_t>
		SelectLightSourceIntersection
		(const Ray_3& EyeRay, const std::vector<LightSourceIntersection_t>& Intersections) const;

	virtual 
		Light_t
		PathTracingRecursion_Refracted
		(const Parameters_t& Parameters, const Ray_3& EyeRay, const Intersection_t& Intersection, int RecursionDeep) const;
	virtual 
	Light_t
		PathTracingRecursion_Mirror(const Parameters_t& Parameters, const Ray_3& EyeRay, const Intersection_t& Intersection, int RecursionDeep) const;
	virtual 
		std::vector<Light_t>
		PathTracingRecursion_Diffuse(const Parameters_t& Parameters, const Ray_3& EyeRay, const Intersection_t& Intersection, int RecursionDeep) const;

	virtual 
		Light_t
		PathTracingRecursion
		(const Ray_3& EyeRay, const int RecursionDeep) const;
	virtual 
		void
		PathTracingPixel
		(std::shared_ptr<std::vector<Color_t>> result, const int PixelPositionX, const int PixelPositionY) const;
	virtual 
		void
		PathTracingThread
		(std::shared_ptr<std::vector<Color_t>> result, const int PixelPositionY) const;
	virtual 
		std::shared_ptr<std::vector<Color_t>>
		PathTracing
		() const;
};