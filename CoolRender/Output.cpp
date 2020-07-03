//
//  Output.cpp
//  CoolRender
//
//  Created by 唐如麟 on 15/7/27.
//  Copyright © 2015年 唐如麟. All rights reserved.
//

#include "Output.hpp"
#include <string>
#include "SOIL.h"

void
OutputToBMP
(const std::string filename,const unsigned char * result,const int ResolutionX,const int ResolutionY){
    SOIL_save_image(filename.data(), SOIL_SAVE_TYPE_BMP, ResolutionX, ResolutionY, SOIL_LOAD_RGBA, result);
}