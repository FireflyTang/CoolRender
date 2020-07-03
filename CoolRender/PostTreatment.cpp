//
//  PostTreatment.cpp
//  CoolRender
//
//  Created by 唐如麟 on 15/7/27.
//  Copyright © 2015年 唐如麟. All rights reserved.
//

#include <memory.h>

#include "PostTreatment.hpp"
#include "boost/format.hpp"
#include "time.h"
#include "sys/timeb.h"
#include "SOIL.h"

PostTreatment_t&
PostTreatment_t::
Downsample() {
	auto dresult = std::make_shared<std::vector<Color_t>>(std::vector<Color_t>(Camera.ResolutionX * Camera.ResolutionY));

	for (int i = 0; i < Screen->size(); i++) {
		int c = Camera.ResamplePixels * 2 + 1;
		int y = i / (Camera.ResolutionX * c * c);
		int x = (i % (Camera.ResolutionX * c)) / c;
		(*dresult)[y * Camera.ResolutionX + x] += (*Screen)[i];
	}

	for (auto& i : *dresult) i /= (Camera.ResamplePixels * 2 + 1) * (Camera.ResamplePixels * 2 + 1);

	Screen = dresult;
	return *this;
};
PostTreatment_t&
PostTreatment_t::
Output_BMP(std::string OutputName) {
	auto iresult = std::make_shared<std::vector<unsigned char>>(std::vector<unsigned char>(Camera.ResolutionX * Camera.ResolutionY * 3));
	for (int y = 0; y < Camera.ResolutionY; y++) {
		for (int x = 0; x < Camera.ResolutionX; x++) {
			Color_t temp = ((*Screen)[y * Camera.ResolutionX + x] * 255).Cut255();
			(*iresult)[3 * (y * Camera.ResolutionX + x)] = static_cast<unsigned char>(round(temp.R));
			(*iresult)[3 * (y * Camera.ResolutionX + x) + 1] = static_cast<unsigned char>(round(temp.G));
			(*iresult)[3 * (y * Camera.ResolutionX + x) + 2] = static_cast<unsigned char>(round(temp.B));
		}
	}
	struct timeb tp;
	struct tm ltime;
	ftime(&tp);
	localtime_s(&ltime, &tp.time);
	if (OutputName.empty())
		OutputName = (boost::format("Output-%1%-%2%--%3%-%4%.bmp") % (ltime.tm_mon + 1) % ltime.tm_mday % ltime.tm_hour % ltime.tm_min).str();
	SOIL_save_image(OutputName.data(), SOIL_SAVE_TYPE_BMP, Camera.ResolutionX, Camera.ResolutionY, SOIL_LOAD_RGB, iresult->data());
	return *this;
};

PostTreatment_t&
PostTreatment_t::
Aperture() {
	const double& scale = Camera.Aperture;
	for (auto& i : *Screen) i /= scale;
	return *this;
}

PostTreatment_t&
PostTreatment_t::
Scale(double scale) {
	for (auto& i : *Screen) i *= scale;
	return *this;
}

