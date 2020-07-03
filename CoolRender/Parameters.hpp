#pragma once

#include "Mesh.hpp"
#include "Scene.cpp"
#include "DrawScene.hpp"
#include "LightSource.hpp"
#include "PostTreatment.hpp"
#include "Material.hpp"

class Parameters_t {
public:
	Parameters_t(const Scene_t& inScene, const Camera_t& inCamera) :pScene(&inScene), pCamera(&inCamera) {}
public:
	const Scene_t* pScene;
	const Camera_t* pCamera;
};