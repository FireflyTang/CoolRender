//
//  CoolLib.h
//  Lession4
//
//  Created by 唐如麟 on 15/4/8.
//  Copyright (c) 2015年 唐如麟. All rights reserved.
//

#ifndef __CoolLib__
#define __CoolLib__
#include "CoolMath.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <glut/glut.h>
#include "OpenGL/gl.h"
#include <SOIL.h>

namespace CC{
	
	class classTex{
	public:
		void setAnisotropy(GLenum targe = GL_TEXTURE_2D);
		void setMipmap(GLenum target = GL_TEXTURE_2D);
		GLuint LoadMipmapTex(const char * filename);
		GLuint LoadCubeMapMipmapTex(const char *x_pos_file,	const char *x_neg_file,	const char *y_pos_file, 	const char *y_neg_file, const char *z_pos_file,	const char *z_neg_file);
	};
	
	class classControl{
	public:
		classControl();
		~classControl();
		virtual void keyMove(unsigned char key,int,int);
		virtual void keyPressOtherKey(unsigned char key,int,int);
		void keyRotateByCenter(int key,int,int);
		void keyRotateBySelf(int key,int,int);
		void setLookAt();
		void setPos(CM::Vector _Postion);
		void setUp(CM::Vector _Up);
		void setCenter(CM::Vector _Center);
		void setPos(GLfloat x,GLfloat y,GLfloat z);
		void setUp(GLfloat x,GLfloat y,GLfloat z);
		void setCenter(GLfloat x,GLfloat y,GLfloat z);
		void setRotationStepCoefficient(GLfloat c);
		void setMoveStepCoefficient(GLfloat c);
		virtual void MousePress(int button,int state,int x,int y);
		virtual void MouseMoveByCenter(int x,int y);
		virtual void MouseMoveBySelf(int x,int y);
		
		void InitWindowSize(GLint _WindowSizeX,GLint _WindowSizeY);
		
//		M3DMatrix33f	LastRotationMartix33;
//		M3DMatrix44f	LastRotationMartix44;
//		M3DMatrix44f	TotalRotation44;
//		M3DMatrix33f	TotalRotation33;

		CM::Vector Postion;
		CM::Vector Up;
		CM::Vector Center;
		CM::Vector Postion0;
		CM::Vector Up0;
		CM::Vector Center0;
		
		GLint WindowSizeX=500,WindowSizeY=500;
	private:
		GLfloat RotationStepCoefficient=1;
		GLfloat MoveStepCoefficient=1;
		
		GLfloat RotateInitStep = 0.2;
		GLfloat RotateMaxStep = RotateInitStep * 20;
		GLfloat RotateStepDelta = 1.4;
		
		GLfloat MoveInitStep = 5;
		GLfloat MoveMaxStep = MoveInitStep * 3;
		GLfloat MoveStepDelta = 1.1;
		
		GLint MousePressPostionX,MousePressPostionY;
	};
}

#endif /* defined(__CoolLib__) */
