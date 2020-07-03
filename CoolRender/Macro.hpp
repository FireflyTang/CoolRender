//
//  Macro.hpp
//  CoolRender
//
//  Created by 唐如麟 on 15/7/22.
//  Copyright © 2015年 唐如麟. All rights reserved.
//
#pragma once
typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_3 Point_3;
typedef Kernel::Point_2 Point_2;
typedef Kernel::Ray_3 Ray_3;
typedef Kernel::Vector_3 Vector_3;
typedef Kernel::Segment_3 Segment_3;
typedef Kernel::Direction_3 Direction_3;
typedef Kernel::Triangle_3 Triangle_3;

constexpr double EPSILON=2e-10;
inline Vector_3 NORMALIZE(Vector_3 x) {
	return ((x) / (sqrt((x).squared_length())));
};
constexpr double M_PI = 3.1415926;


