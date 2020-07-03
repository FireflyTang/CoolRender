#pragma once
#include "PTRenderCore.hpp"
#include <cmath>
#include <cassert>

class PT_Phong_t : public PT_t {
public:
	using PT_t::PT_t;
	int FaceLightSourceSampleTimes = 5;
	std::vector<Light_t>
		PathTracingRecursion_Diffuse(const Parameters_t& Parameters, const Ray_3& EyeRay, \
			const Intersection_t& Intersection, int RecursionDeep) const override {

		auto MirrorReflectedLight = Rays.CalcMirrorReflectedRay(Parameters, EyeRay, Intersection);
		//Diffuse
		Color_t HighLightColor = Color_t::Black;
		Color_t DiffuseColor = Color_t::Black;
		const Color_t& Ka = Intersection.pFace->Material.Ka;
		const Color_t& Kd = Intersection.pFace->Material.Kd;
		for (auto& FaceLightSource : Parameters.pScene->pLightSources->FaceLightSources) {
			auto LightSourceSamples = FaceLightSource.Sample(FaceLightSourceSampleTimes);
			for (auto& LightSourceSample : LightSourceSamples) {
				Ray_3 DiffuseEyeRay(Intersection.Point, LightSourceSample);
				if (DiffuseEyeRay.to_vector() * Intersection.pFace->Normal > 0) {
					auto DiffuseIntersections = Parameters.pScene->pMesh->QueryIntersections(DiffuseEyeRay);
					auto DiffuseIntersection = SelectIntersection(DiffuseEyeRay, DiffuseIntersections);
					if (!DiffuseIntersection) {
						if (MirrorReflectedLight.to_vector() * DiffuseEyeRay.to_vector() > 0) {
							HighLightColor += FaceLightSource.Color * FaceLightSource.Lightness * pow(NORMALIZE(MirrorReflectedLight.to_vector()) * NORMALIZE(DiffuseEyeRay.to_vector()), FaceLightSource.Shineness) / FaceLightSourceSampleTimes;
						};
							DiffuseColor += Ka * Kd * (NORMALIZE(DiffuseEyeRay.to_vector()) * Intersection.pFace->Normal)/ FaceLightSourceSampleTimes;
					}
				}
			}
		}
		std::vector<Light_t> Lights;
		Lights.push_back(Light_t(-EyeRay.to_vector(), HighLightColor+ DiffuseColor));
		return Lights;
	};

	Light_t
		CalcReturnLight
		(const Ray_3& EyeRay, const Intersection_t& Intersection, const Light_t& RefractionLight, const std::vector<Light_t>& DiffuseReflectLights, const Light_t& MirrorReflectedLight, bool ShouldTotalReflection) const override {
		Light_t result;
		result.Vector = -NORMALIZE(EyeRay.to_vector());
		const Color_t& Kd = Intersection.pFace->Material.Kd;
		const Color_t& Ks = Intersection.pFace->Material.Ks;
		const Color_t& Tf = Intersection.pFace->Material.Tf;
		const Color_t& Ka = Intersection.pFace->Material.Ka;
		if (!ShouldTotalReflection) {
			result.Color = RefractionLight.Color * Tf + MirrorReflectedLight.Color * Ks;
		}
		else {
			result.Color = MirrorReflectedLight.Color * (Ks + Tf);
		}
		if (!DiffuseReflectLights.empty())
			result.Color += DiffuseReflectLights[0].Color;
		return result;
	};
};