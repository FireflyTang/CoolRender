//
//  PostTreatment.hpp
//  CoolRender
//
//  Created by 唐如麟 on 15/7/27.
//  Copyright © 2015年 唐如麟. All rights reserved.
//

#pragma once

#include <memory>

#include "Camera.hpp"
#include "Color.hpp"
#include <string>

class PostTreatment_t{
public:
	PostTreatment_t(const Camera_t& inCamera, std::shared_ptr<std::vector<Color_t>> inScreen) :	Camera(inCamera), Screen(inScreen) {};

	const Camera_t& Camera;
	std::shared_ptr<std::vector<Color_t>> Screen;
	virtual
		PostTreatment_t& 
		Downsample();
	virtual
		PostTreatment_t & 
		Output_BMP(std::string OutputName="");
	virtual
		PostTreatment_t&
		Aperture();
	virtual
		PostTreatment_t&
		Scale(double scale);
};

