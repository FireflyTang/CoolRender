//
//  DrawScene.hpp
//  CoolRender
//
//  Created by 唐如麟 on 15/7/22.
//  Copyright © 2015年 唐如麟. All rights reserved.
//

#ifndef DrawScene_cpp
#define DrawScene_cpp


#include "Mesh.hpp"
#include "Camera.hpp"
#include "Scene.hpp"

void
DrawSence
(const Mesh_t & Mesh,Scene_t * inpScene=nullptr,Camera_t * inpCamera= nullptr,std::vector<Segment_3> * inpSegmentsToDraw=nullptr);

void InitScene(int argc, char ** argv);

#endif /* DrawScene_cpp */
