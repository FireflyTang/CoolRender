//
//  Material.hpp
//  CoolRender
//
//  Created by 唐如麟 on 15/8/2.
//  Copyright © 2015年 唐如麟. All rights reserved.
//
#pragma once
#include <map>
#include <string>
#include "Color.hpp"


class Material_t{
public:
    Color_t Kd;
    Color_t Ka;
    Color_t Tf;
    Color_t Ks;
    double Ni;
};

class Materials_t{
public:
    Materials_t(const std::string & inFilename);
	Material_t & operator[](const std::string & inMaterialName);
private:
    std::string _Filename;
    std::map<std::string,Material_t> _MaterialMap;
};


