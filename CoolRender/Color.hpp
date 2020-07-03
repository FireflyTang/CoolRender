//
//  Color.hpp
//  CoolRender
//
//  Created by 唐如麟 on 15/7/24.
//  Copyright © 2015年 唐如麟. All rights reserved.
//

#pragma once
#include <cmath>

class Color_t{
public:
    Color_t(){};
    Color_t(const double inR,const double inG,const double inB):R(inR),G(inG),B(inB){};
	Color_t(const int inR, const int inG, const int inB) ;
	Color_t& Cut255();
public:
    double R=0,G=0,B=0;
    static Color_t White;
    static Color_t Black;
    static Color_t Red;
    static Color_t Green;
    static Color_t Blue;
};

Color_t
operator+(const Color_t & l,const Color_t & r);
Color_t
operator*(const Color_t & l,const Color_t & r);
Color_t
operator*(const Color_t & l,double r);
Color_t
operator*(double l,const Color_t &  r);
Color_t
operator/(const Color_t & l,double r);
Color_t
operator/(double l,const Color_t &  r);
Color_t &
operator+=(Color_t & l,const Color_t & r);
Color_t &
operator*=(Color_t & l,const Color_t & r);
Color_t &
operator*=(Color_t &  l,double r);
Color_t &
operator/=(Color_t & l,double r);
Color_t &			
Cut255();

