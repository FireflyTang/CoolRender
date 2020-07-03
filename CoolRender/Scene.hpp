//
//  Scene.hpp
//  CoolRender
//
//  Created by 唐如麟 on 15/7/24.
//  Copyright © 2015年 唐如麟. All rights reserved.
//

#pragma once

#include "Mesh.hpp"
#include "LightSource.hpp"

class Scene_t{
public:
    Scene_t(const Mesh_t & inMesh,const LightSources_t & inLightSources):pMesh(&inMesh),pLightSources(&inLightSources){};
public:
    const Mesh_t * pMesh=nullptr;
    const LightSources_t * pLightSources=nullptr;
};


