//
//  Color.cpp
//  CoolRender
//
//  Created by 唐如麟 on 15/7/24.
//  Copyright © 2015年 唐如麟. All rights reserved.
//

#include "Color.hpp"

Color_t Color_t::White(1,1,1);
Color_t Color_t::Black(0,0,0);
Color_t Color_t::Red(1,0,0);
Color_t Color_t::Green(0,1,0);
Color_t Color_t::Blue(0,0,1);

Color_t::
Color_t(const int inR, const int inG, const int inB) {
	R = (double)inR / 255;
	G = (double)inG / 255;
	B = (double)inB / 255;
};

Color_t
operator+(const Color_t & l,const Color_t & r){
    return Color_t(l.R+r.R,l.G+r.G,l.B+r.B);
}

Color_t
operator*(const Color_t & l,const Color_t & r){
    return Color_t(l.R*r.R,l.G*r.G,l.B*r.B);
}

Color_t
operator*(const Color_t & l,double r){
    return Color_t(l.R*r,l.G*r,l.B*r);
}

Color_t
operator*(double l,const Color_t &  r){
    return Color_t(r.R*l,r.G*l,r.B*l);
}

Color_t
operator/(const Color_t & l,double r){
    return Color_t(l.R/r,l.G/r,l.B/r);
}

Color_t
operator/(double l,const Color_t &  r){
    return Color_t(r.R/l,r.G/l,r.B/l);
}

Color_t &
operator+=(Color_t & l,const Color_t & r){
    l.R+=r.R;
    l.G+=r.G;
    l.B+=r.B;
    return l;
}

Color_t &
operator*=(Color_t & l,const Color_t & r){
    l.R*=r.R;
    l.G*=r.G;
    l.B*=r.B;
    return l;
}

Color_t &
operator*=(Color_t & l,double r){
    l.R*=r;
    l.G*=r;
    l.B*=r;
    return l;
}

Color_t &
operator/=(Color_t & l,double r){
    l.R/=r;
    l.G/=r;
    l.B/=r;
    return l;
}

Color_t& 
Color_t::
Cut255() {
	R = round(R > 255 ? 255 : R);
	G = round(G > 255 ? 255 : G);
	B = round(B > 255 ? 255 : B);
	return *this;
}
