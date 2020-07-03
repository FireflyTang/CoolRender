//
//  PT_tRenderCore.cpp
//  CoolRender
//
//  Created by 唐如麟 on 15/7/23.
//  Copyright © 2015年 唐如麟. All rights reserved.
//
#include "tbb/task_scheduler_init.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"
#include <CGAL/Origin.h>

#include "PTRenderCore.hpp"

#ifdef __DRAW__
std::vector<Segment_3> SegmentsToDraw;
#endif


Ray_3
Rays_t::
CalcDiffuseReflectedRay
(const Parameters_t& Parameters, const Ray_3& EyeRay, const Intersection_t& Intersection) const {
	double a, b, l;
	a = acos(1 - 2 * ((double)(rand()) / RAND_MAX));//phi
	b = ((double)(rand()) / RAND_MAX) * M_PI * 2;//theta
	l = sin(a);
	double x, y, z;
	x = sin(b) * l;
	y = cos(a);
	z = cos(b) * l;
	Vector_3 temp(x, y, z);
	Vector_3 Normal = Intersection.pFace->Normal;
	if ((EyeRay.to_vector() * Normal) * (temp * Normal) > 0)
		temp += 2 * (temp * Normal) * (-Normal);
	return Ray_3(Intersection.Point, temp);
}

Ray_3
Rays_t::
CalcMirrorReflectedRay
(const Parameters_t& Parameters, const Ray_3& EyeRay, const Intersection_t& Intersection) const {
	const Vector_3& normal = Intersection.pFace->Normal;
	Vector_3 IncidentVector = NORMALIZE(EyeRay.to_vector());
	return Ray_3(Intersection.Point, 2 * (normal * (-IncidentVector)) * normal + IncidentVector);
}

Ray_3
Rays_t::
CalcRefractedRay
(const Parameters_t& Parameters, const Ray_3& EyeRay, const Intersection_t& Intersection) const {
	Vector_3 reference = Intersection.pFace->Normal;
	Vector_3 Incidentvector = EyeRay.to_vector();
	Incidentvector = NORMALIZE(Incidentvector);
	if (Incidentvector * reference < 0) reference = -reference;
	Vector_3 temp = Incidentvector - (Incidentvector * reference) * reference;
	temp = NORMALIZE(temp);
	double angle;
	if (Incidentvector * Intersection.pFace->Normal < 0) {
		angle = asin(sin(acos(reference * Incidentvector)) / Intersection.pFace->Material.Ni);
	}
	else {
		angle = asin(sin(acos(reference * Incidentvector)) * Intersection.pFace->Material.Ni);
	}
	return Ray_3(Intersection.Point, reference + temp * tan(angle));
}

bool
PT_t::
IfTotalReflection
(const Ray_3& EyeRay, const Intersection_t& Intersection) const {
	double Ni = Intersection.pFace->Material.Ni;
	assert(Ni >= 1);
	Vector_3 IncidentVector = EyeRay.to_vector();
	IncidentVector = NORMALIZE(IncidentVector);
	if (EyeRay.to_vector() * Intersection.pFace->Normal > 0) {
		if (Ni * sin(acos(IncidentVector * Intersection.pFace->Normal)) > 1) {
			return true;
		}
	}
	return false;
}

bool
PT_t::
IfAllowRefraction
(const Ray_3& EyeRay, const Intersection_t& Intersection, const int RecursionDeep, bool ShouldTotalReflection) const {
	const Color_t& Tf = Intersection.pFace->Material.Tf;
	if ((Tf.R < EPSILON) && (Tf.G < EPSILON) && (Tf.B < EPSILON)) return false;
	if (ShouldTotalReflection && IfAllowTotalReflection) return false;
	return true;
}

bool
PT_t::
IfAllowMirrorReflection
(const Ray_3& EyeRay, const Intersection_t& Intersection, const int RecursionDeep, bool ShouldTotalReflection) const {
	const Color_t& Ks = Intersection.pFace->Material.Ks;
	const Color_t& Tf = Intersection.pFace->Material.Tf;
	if (((Tf.R > EPSILON) || (Tf.G > EPSILON) || (Tf.B > EPSILON)) && ShouldTotalReflection&& IfAllowTotalReflection) return true;
	if ((Ks.R < EPSILON) && (Ks.G < EPSILON) && (Ks.B < EPSILON)) return false;
	return true;
}

bool
PT_t::
IfAllowDiffuseReflection
(const Ray_3& EyeRay, const Intersection_t& Intersection, const int RecursionDeep) const {
	const Color_t& Kd = Intersection.pFace->Material.Kd;
	if ((Kd.R < EPSILON) && (Kd.G < EPSILON) && (Kd.B < EPSILON)) return false;
	return true;
};

Light_t
PT_t::
HitLightSource
(const Ray_3& EyeRay, const LightSourceIntersection_t& Intersection) const {
	Vector_3 vector = -EyeRay.to_vector();
	vector = NORMALIZE(vector);
	Light_t result(vector, Intersection.pFaceLightSource->Color * Intersection.pFaceLightSource->Lightness);
#ifdef __DRAW__
	result.isHitLightSource = true;
	result.LightSourceIntersection = Intersection;
#endif
	return result;
}


Color_t
PT_t::
HitEnvironmernt(const Ray_3& EyeRay) const {
	return Parameters.pScene->pLightSources->HitEnvironmentLightSource(EyeRay);
}


int
PT_t::
GetDiffuseReflectionResampleTimes
(const Ray_3& EyeRay, const Intersection_t& Intersection, const int RecursionDeep) const {
	return DiffuseReflectionResampleTimes;
}

Light_t
PT_t::
CalcReturnLight
(const Ray_3& EyeRay, const Intersection_t& Intersection, const Light_t& RefractionLight, const std::vector<Light_t>& DiffuseReflectLights, const Light_t& MirrorReflectedLight, bool ShouldTotalReflection) const {
	Light_t result;
	result.Vector = -NORMALIZE(EyeRay.to_vector());
	const Color_t& Kd = Intersection.pFace->Material.Kd;
	const Color_t& Ks = Intersection.pFace->Material.Ks;
	const Color_t& Tf = Intersection.pFace->Material.Tf;

	if (!ShouldTotalReflection) {
		result.Color = RefractionLight.Color * Tf + MirrorReflectedLight.Color * Ks;
	}
	else {
		result.Color = MirrorReflectedLight.Color * (Ks + Tf);
	}

	if (DiffuseReflectLights.size()) {
		Color_t color;
		for (auto& i : DiffuseReflectLights) {
			color += i.Color;
		}
		color /= static_cast<double>(DiffuseReflectLights.size());
		result.Color += color * Kd;
	}
	result.Color *= Intersection.pFace->Material.Ka;

	return result;
}

boost::optional<Intersection_t>
PT_t::
SelectIntersection
(const Ray_3& EyeRay, const std::vector<Intersection_t>& Intersections) const {
	double mindistance = DBL_MAX;
	boost::optional<Intersection_t> result;
	for (auto& i : Intersections) {
		double mindistance0 = CGAL::squared_distance((i).Point, EyeRay.source());
		if ((mindistance0 < mindistance) && (mindistance0 > EPSILON)) {
			result = i;
			mindistance = mindistance0;
		}
	}
	return result;
}

boost::optional<LightSourceIntersection_t>
PT_t::
SelectLightSourceIntersection
(const Ray_3& EyeRay, const std::vector<LightSourceIntersection_t>& Intersections)const {
	double mindistance = DBL_MAX;
	boost::optional<LightSourceIntersection_t> result;
	for (auto& i : Intersections) {
		double mindistance0 = CGAL::squared_distance(i.Point, EyeRay.source());
		if ((mindistance0 < mindistance) && (mindistance0 > EPSILON)) {
			if (EyeRay.to_vector() * i.pFaceLightSource->Normal < 0) result = i;
			mindistance = mindistance0;
		}
	}
	return result;
}

//--------------------------------------------------

Light_t
PT_t::
PathTracingRecursion_Refracted(const Parameters_t& Parameters, const Ray_3& EyeRay, const Intersection_t& Intersection, int RecursionDeep) const {
	Light_t RefractedLight;
	Ray_3 RefractedRay = Rays.CalcRefractedRay(Parameters, EyeRay, Intersection);
	RefractedLight = PathTracingRecursion(RefractedRay, RecursionDeep + 1);
	return RefractedLight;
}

Light_t
PT_t::
PathTracingRecursion_Mirror(const Parameters_t& Parameters, const Ray_3& EyeRay, const Intersection_t& Intersection, int RecursionDeep) const {
	Light_t MirrorReflectedLight;
	Ray_3 MirrorReflectedRay = Rays.CalcMirrorReflectedRay(Parameters, EyeRay, Intersection);
	MirrorReflectedLight = PathTracingRecursion(MirrorReflectedRay, RecursionDeep + 1);
	return MirrorReflectedLight;
}

std::vector<Light_t>
PT_t::
PathTracingRecursion_Diffuse(const Parameters_t& Parameters, const Ray_3& EyeRay, const Intersection_t& Intersection, int RecursionDeep) const {
	std::vector<Light_t> DiffuseReflectedLights;
	for (int i = 0; i < GetDiffuseReflectionResampleTimes(EyeRay, Intersection, RecursionDeep); i++) {
		Ray_3 DiffuseReflectedRay = Rays.CalcDiffuseReflectedRay(Parameters, EyeRay, Intersection);
		DiffuseReflectedLights.push_back(PathTracingRecursion(DiffuseReflectedRay, RecursionDeep + 1));
	}
	return DiffuseReflectedLights;
}

Light_t
PT_t::
PathTracingRecursion
(const Ray_3& EyeRay, const int RecursionDeep) const {
	const LightSources_t& LightSources = *(Parameters.pScene->pLightSources);
	std::vector<LightSourceIntersection_t> LightSourceIntersections = LightSources.QueryIntersections(EyeRay);
	boost::optional<LightSourceIntersection_t> pSelectedLightSourceIntersection = SelectLightSourceIntersection(EyeRay, LightSourceIntersections);

	const Mesh_t& Mesh = *(Parameters.pScene->pMesh);
	std::vector<Intersection_t> Intersections = Mesh.QueryIntersections(EyeRay);
	boost::optional<Intersection_t> pSelectedIntersection = SelectIntersection(EyeRay, Intersections);

	double MinLightSourceIntersectionDistance = DBL_MAX;
	double MinInersectionDistance = DBL_MAX;

	if (pSelectedLightSourceIntersection) {
		MinLightSourceIntersectionDistance = CGAL::squared_distance(EyeRay.source(), pSelectedLightSourceIntersection->Point);
	}
	if (pSelectedIntersection) {
		MinInersectionDistance = CGAL::squared_distance(EyeRay.source(), pSelectedIntersection->Point);
	}

	if (MinLightSourceIntersectionDistance < MinInersectionDistance) {
		return HitLightSource(EyeRay, *pSelectedLightSourceIntersection);
	}
	if (MinInersectionDistance < MinLightSourceIntersectionDistance) {
		if (RecursionDeep < MaxRecursionDeep) {
			Light_t RefractedLight;
			Light_t MirrorReflectedLight;
			std::vector<Light_t> DiffuseReflectedLights;
			bool ShouldTotalReflection = IfTotalReflection(EyeRay, *pSelectedIntersection);
			if (IfAllowRefraction(EyeRay, *pSelectedIntersection, RecursionDeep, ShouldTotalReflection))
				RefractedLight = PathTracingRecursion_Refracted(Parameters, EyeRay, *pSelectedIntersection, RecursionDeep);
#ifdef __DRAW__
			SegmentsToDraw.push_back(Segment_3(RefractedRay.source(), RefractedRay.source() + 15 * NORMALIZE(RefractedRay.to_vector())));
#endif
			if (IfAllowMirrorReflection(EyeRay, *pSelectedIntersection, RecursionDeep, ShouldTotalReflection))
				MirrorReflectedLight = PathTracingRecursion_Mirror(Parameters, EyeRay, *pSelectedIntersection, RecursionDeep);
			if (IfAllowDiffuseReflection(EyeRay, *pSelectedIntersection, RecursionDeep))
				DiffuseReflectedLights = PathTracingRecursion_Diffuse(Parameters, EyeRay, *pSelectedIntersection, RecursionDeep);
#ifdef __DRAW__
			if (MirrorReflectedLight.LastIntersection.pFace) { //上一次击中了某个表面
				SegmentsToDraw.push_back(Segment_3(MirrorReflectedLight.LastIntersection.Point, pSelectedIntersection->Point));
			}
			else {
				if (MirrorReflectedLight.isHitLightSource) {
					SegmentsToDraw.push_back(Segment_3(MirrorReflectedLight.LightSourceIntersection.Point, pSelectedIntersection->Point));
				}
			}
#endif
			Light_t result = CalcReturnLight(EyeRay, *pSelectedIntersection, RefractedLight, DiffuseReflectedLights, MirrorReflectedLight, ShouldTotalReflection);
#ifdef __DRAW__
			MirrorReflectedLight.LastIntersection = *pSelectedIntersection;
#endif
			return result;
		}
		else {
			Light_t(NORMALIZE(-EyeRay.to_vector()), Color_t::Black);
		}
	}

	return Light_t(NORMALIZE(-EyeRay.to_vector()), HitEnvironmernt(EyeRay));
}

void
PT_t::
PathTracingPixel
(std::shared_ptr<std::vector<Color_t>> result, const int PixelPositionX, const int PixelPositionY) const {
	const Camera_t& Camera = *(Parameters.pCamera);
	Point_3 PixelPosisiton = Camera.ProjectionPlane[0] + Camera.PixelDistanceX * PixelPositionX * Camera.Right - Camera.PixelDistanceY * PixelPositionY * Camera.Head;
	Ray_3 EyeRay(Camera.Center, PixelPosisiton);
	Light_t resultlight = PathTracingRecursion(EyeRay, 0);
#ifdef __DRAW__
	DrawSence(*(Parameters.pScene->pMesh), Parameters.pScene, &Camera, &SegmentsToDraw);
#endif
	(*result)[PixelPositionY * Parameters.pCamera->ResampleResolutionX + PixelPositionX] = resultlight.Color;
}

void
PT_t::
PathTracingThread
(std::shared_ptr<std::vector<Color_t>> result, const int PixelPositionY) const {
	static int finished = 0;
	printf("finished = %d / %d\n", finished++, Parameters.pCamera->ResampleResolutionY);
	for (int PixelPositionX = 0; PixelPositionX < Parameters.pCamera->ResampleResolutionX; PixelPositionX++)
		PathTracingPixel(result, PixelPositionX, PixelPositionY);
}

class
	PathTracingThreadClass_t {
public:
	PathTracingThreadClass_t( const PT_t & inPT, std::shared_ptr<std::vector<Color_t>> inresult) :PT(inPT), Parameters(inPT.Parameters), result(inresult) {}
	void operator()(const tbb::blocked_range<int>& r) const {
		for (auto PixelPositionY = r.begin(); PixelPositionY != r.end(); PixelPositionY++)
			PT.PathTracingThread(result, PixelPositionY);
	}
public:
	const PT_t& PT;
	const Parameters_t& Parameters;
	std::shared_ptr<std::vector<Color_t>> result;
};

std::shared_ptr<std::vector<Color_t>>
PT_t::
PathTracing
() const {
	tbb::task_scheduler_init init;
	std::shared_ptr<std::vector<Color_t>>  result(new std::vector<Color_t>(Parameters.pCamera->ResampleResolutionX * Parameters.pCamera->ResampleResolutionY));
	const Camera_t& Camera = *(Parameters.pCamera);
#ifdef __DRAW__
	PathTracingPixel(Parameters, result, 350 * 5, 200 * 5);
#endif

	//        for(int y=0;y<Camera.ResampleResolutionY;y++){
	//            for(int x=0;x<Camera.ResampleResolutionX;x++){
	//                PathTracingPixel(Parameters, result, x,y);
	//            }
	//        }

	tbb::parallel_for(tbb::blocked_range<int>(0, Camera.ResampleResolutionY), PathTracingThreadClass_t(*this, result));
	return result;
}



