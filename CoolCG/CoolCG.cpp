//
//  CoolLib.cpp
//  Lession4
//
//  Created by 唐如麟 on 15/4/8.
//  Copyright (c) 2015年 唐如麟. All rights reserved.
//

#include "CoolCG.h"
#include <iostream>
using namespace CM;
namespace CC{
	using namespace CM;
	classControl::
	classControl(){
		setPos(1,1,1);
		setUp(-1,1,-1);
		setCenter(0,0,0);
		//	m3dLoadIdentity44(TotalRotation44);
		//	m3dLoadIdentity33(TotalRotation33);
	};
	
	classControl::
	~classControl(){
	};
	
	void
	classControl::
	setLookAt(){
		gluLookAt(Postion[0],Postion[1],Postion[2], Center[0], Center[1], Center[2], Up[0], Up[1], Up[2]);
	}
	
	void
	classControl::
	keyRotateByCenter(int key,int,int){
		CM::Vector     right;
		CM::Vector     view;
		CM::Matrix     RotationMartix33;
		//CM::Matrix     RotationMartix44;
		CM::Vector     axis;
		
		static GLfloat	angle,angle0;
		static GLint last_key;
		
		if (!((key==GLUT_KEY_UP)
			  ||(key==GLUT_KEY_DOWN)
			  ||(key==GLUT_KEY_LEFT)
			  ||(key==GLUT_KEY_RIGHT)))return;
		
		view=Center-Postion;
		right=view.cross(Up);
		
		//绕right旋转
		if(key==GLUT_KEY_UP || key==GLUT_KEY_DOWN){
			if(key==GLUT_KEY_UP)   {
				if(last_key==GLUT_KEY_UP)
					angle0 = fmin(RotateMaxStep,angle0*RotateStepDelta);
				else
					angle0 = RotateInitStep;
			}
			if(key==GLUT_KEY_DOWN)  {
				if(last_key==GLUT_KEY_DOWN)
					angle0 = fmax(-RotateMaxStep,angle0*RotateStepDelta);
				else
					angle0 = -RotateInitStep;
			}
			axis=right;
		}
		
		//绕up旋转
		if(key==GLUT_KEY_LEFT || key==GLUT_KEY_RIGHT){
			if(key==GLUT_KEY_LEFT)    {
				if(last_key==GLUT_KEY_LEFT)
					angle0 = fmin(RotateMaxStep,angle0*RotateStepDelta);
				else
					angle0 = RotateInitStep;
			}
			if(key==GLUT_KEY_RIGHT)  {
				if(last_key==GLUT_KEY_RIGHT)
					angle0 = fmax(-RotateMaxStep,angle0*RotateStepDelta);
				else
					angle0 = -RotateInitStep;
			}
			axis=Up;
		}
		angle=angle0*RotationStepCoefficient;
		
		RotationMartix33 = CM::GetRotationMatrix(angle,axis[0], axis[1], axis[2]);
		view=RotationMartix33*view;
		Up=RotationMartix33*Up;
		Postion=Center-view;
		
		last_key = key;
		//	m3dCopyMatrix33(LastRotationMartix33,RotationMartix33);
		//
		//	M3DMatrix33f TotalRotation0;
		//	m3dCopyMatrix33(TotalRotation0,TotalRotation33);
		//	m3dMatrixMultiply33(TotalRotation33, TotalRotation0, RotationMartix33);
		//
		//	m3dRotationMatrix44(RotationMartix44,angle/180*M_PI,axis[0], axis[1], axis[2]);
		//	m3dCopyMatrix44(LastRotationMartix44,RotationMartix44);
		//	M3DMatrix44f TotalRotation1;
		//	m3dCopyMatrix44(TotalRotation1,TotalRotation44);
		//	m3dMatrixMultiply44(TotalRotation44, TotalRotation1, RotationMartix44);
		
		glutPostRedisplay();
		return;
	}
	
	void
	classControl::
	keyRotateBySelf(int key,int,int){
		CM::Vector      right;
		CM::Vector      view;
		CM::Matrix      RotationMartix33;
		//CM::Matrix      RotationMartix44;
		CM::Vector      axis;
		
		static GLfloat	angle,angle0;
		static GLint last_key;
		
		if (!((key==GLUT_KEY_UP)
			  ||(key==GLUT_KEY_DOWN)
			  ||(key==GLUT_KEY_LEFT)
			  ||(key==GLUT_KEY_RIGHT)))return;
		
		view=Center-Postion;
		right=view.cross(Up);
		
		//绕right旋转
		if(key==GLUT_KEY_UP || key==GLUT_KEY_DOWN){
			if(key==GLUT_KEY_UP)    {
				if(last_key==GLUT_KEY_UP)
					angle0 = fmin(RotateMaxStep,angle0*RotateStepDelta);
				else
					angle0 = RotateInitStep;
			}
			if(key==GLUT_KEY_DOWN)  {
				if(last_key==GLUT_KEY_DOWN)
					angle0 = fmax(-RotateMaxStep,angle0*RotateStepDelta);
				else
					angle0 = -RotateInitStep;
			}
			axis=right;
		}
		
		//绕up旋转
		if(key==GLUT_KEY_LEFT || key==GLUT_KEY_RIGHT){
			if(key==GLUT_KEY_LEFT)    {
				if(last_key==GLUT_KEY_LEFT)
					angle0 = fmin(RotateMaxStep,angle0*RotateStepDelta);
				else
					angle0 = RotateInitStep;
			}
			if(key==GLUT_KEY_RIGHT)  {
				if(last_key==GLUT_KEY_RIGHT)
					angle0 = fmax(-RotateMaxStep,angle0*RotateStepDelta);
				else
					angle0 = -RotateInitStep;
			}
			axis.set(0,1,0);
		}
		angle=angle0*RotationStepCoefficient;
		
		RotationMartix33 = CM::GetRotationMatrix(angle,axis[0], axis[1], axis[2]);
		view=RotationMartix33*view;
		Up=RotationMartix33*Up;
		Center=Postion+view;
		
		last_key = key;
		//	m3dCopyMatrix33(LastRotationMartix33,RotationMartix33);
		//	M3DMatrix33f TotalRotation0;
		//	m3dCopyMatrix33(TotalRotation0,TotalRotation33);
		//	m3dMatrixMultiply33(TotalRotation33, TotalRotation0, RotationMartix44);
		//
		//	m3dRotationMatrix44(RotationMartix44,angle/180*M_PI,axis[0], axis[1], axis[2]);
		//	m3dCopyMatrix44(LastRotationMartix44,RotationMartix44);
		//	M3DMatrix44f TotalRotation1;
		//	m3dCopyMatrix44(TotalRotation1,TotalRotation44);
		//	m3dMatrixMultiply44(TotalRotation44,  RotationMartix44,TotalRotation1);
		
		glutPostRedisplay();
		return;
	}
	
	void
	classTex::
	setAnisotropy(GLenum target){
		GLfloat fLargest;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
		glTexParameterf(target,GL_TEXTURE_MAX_ANISOTROPY_EXT,fLargest);
	}
	
	void
	classTex::
	setMipmap(GLenum target){
		glTexParameteri(target,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(target,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	}
	
	GLuint
	classTex::
	LoadMipmapTex(const char * filename)
	{
		return SOIL_load_OGL_texture(filename,SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	}
	GLuint
	classTex::
	LoadCubeMapMipmapTex(const char *x_pos_file, const char *x_neg_file, const char *y_pos_file, const char *y_neg_file, const char *z_pos_file,	const char *z_neg_file){
		return SOIL_load_OGL_cubemap(x_pos_file, x_neg_file, y_pos_file, y_neg_file, z_pos_file, z_neg_file, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	}
	
	void
	classControl::
	keyMove(unsigned char key,int,int){
		CM::Vector move0;
		CM::Vector view;
		CM::Vector right;
		
		static GLfloat CurrntStep,CurrntStep0;
		static GLint last_key;
		
		view=Center-Postion;
	
		if(last_key==key)
			CurrntStep0 = fmin(MoveMaxStep, CurrntStep0*MoveStepDelta);
		else
			CurrntStep0 = MoveInitStep;
		
		CurrntStep=CurrntStep0*MoveStepCoefficient;
		switch (key) {
			case 'w':{
				move0=CurrntStep*view.normalized();
				Postion+=move0;
				Center+=move0;
				break;
			}
			case 's':{
				move0=CurrntStep*view.normalized();
				Postion-=move0;
				Center-=move0;
				break;
			}
			case 'a':{
				right=view.cross(Up);
				move0=CurrntStep*right.normalized();
				Postion-=move0;
				Center-=move0;
				break;
			}
			case 'd':{
				right=view.cross(Up);
				move0=CurrntStep*right.normalized();
				Postion+=move0;
				Center+=move0;
				break;
			}
			case 'h':{
				CM::Vector yaxis(0,CurrntStep,0);
				Postion+=yaxis;
				Center+=yaxis;
				break;
			}
			case 'l':{
				CM::Vector yaxis(0,CurrntStep,0);
				Postion-=yaxis;
				Center-=yaxis;
				break;
			}
			default:{
				keyPressOtherKey(key, NULL, NULL);
			}
		}
		last_key = key;
		glutPostRedisplay();
	}
	
	void
	classControl::
	keyPressOtherKey(unsigned char key, int, int){
	};
	
	void
	classControl::
	setPos(CM::Vector _Postion){
		Postion=_Postion;
	}
	void
	classControl::
	setUp(CM::Vector _Up){
		Up=_Up;
	}
	void
	classControl::
	setCenter(CM::Vector _Center){
		Center=_Center;
	}
	void
	classControl::
	setPos(GLfloat x,GLfloat y,GLfloat z){
		Postion.set(x,y,z);
	}
	void
	classControl::
	setUp(GLfloat x,GLfloat y,GLfloat z){
		Up.set(x, y, z);
	}
	void
	classControl::
	setCenter(GLfloat x,GLfloat y,GLfloat z){
		Center.set(x,y,z);
	}
	void
	classControl::
	setRotationStepCoefficient(GLfloat c){
		RotationStepCoefficient=c;
	}
	void
	classControl::
	setMoveStepCoefficient(GLfloat c){
		MoveStepCoefficient=c;
	}
	
	void
	classControl::
	MousePress(int button,int state,int x,int y){
		if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
			MousePressPostionX=x;
			MousePressPostionY=y;
			Postion0=Postion;
			Up0=Up;
			Center0=Center;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
		}
	}
	
	void
	classControl::
	MouseMoveByCenter(int x,int y){
		GLfloat AngleX,AngleY;
		AngleX=rad2deg(atanf(((float)(MousePressPostionX-x))/250));
		AngleY=rad2deg(atanf(((float)(MousePressPostionY-y))/250));
		Vector view0=Center0-Postion0;
		Vector right0=view0.cross(Up0);
		Matrix RotationXMatrix=GetRotationMatrix(AngleX, Up0[0], Up0[1], Up0[2]);
		Matrix RotationYMatrix=GetRotationMatrix(AngleY, right0[0], right0[1], right0[2]);
		view0=RotationXMatrix*RotationYMatrix*view0;
		Up=RotationXMatrix*RotationYMatrix*Up0;
		Postion=Center0-view0;
		glutPostRedisplay();
	}
	void
	classControl::
	MouseMoveBySelf(int x,int y){
		GLfloat AngleX,AngleY;
		AngleX=rad2deg(atanf(((float)(MousePressPostionX-x))/WindowSizeX));
		AngleY=rad2deg(atanf(((float)(MousePressPostionY-y))/WindowSizeY));
		Vector view0=Center0-Postion0;
		Vector right0=view0.cross(Up0);
		Matrix RotationXMatrix=GetRotationMatrix(-AngleX, 0, 1, 0);
		Matrix RotationYMatrix=GetRotationMatrix(-AngleY, right0[0], right0[1], right0[2]);
		view0=RotationXMatrix*RotationYMatrix*view0;
		Up=RotationXMatrix*RotationYMatrix*Up0;
		Center=Postion+view0;
		glutPostRedisplay();
	}
	
	void
	classControl::
	InitWindowSize(GLint _WindowSizeX,GLint _WindowSizeY){
		WindowSizeX=_WindowSizeX;
		WindowSizeY=_WindowSizeY;
		glutInitWindowSize(WindowSizeX, WindowSizeY);
	}
}
