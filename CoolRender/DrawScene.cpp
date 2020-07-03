//
//  DrawScene.cpp
//  CoolRender
//
//  Created by 唐如麟 on 15/7/22.
//  Copyright © 2015年 唐如麟. All rights reserved.
//

#include "Camera.hpp"

#include "CoolCG.h"
#include "CoolCGDelegate.h"

#include "Mesh.hpp"
#include "Camera.hpp"
#include "Scene.hpp"



const Mesh_t * pMesh;
std::vector<Segment_3> * pSegmentsToDraw;
Camera_t * pCamera;
const LightSources_t * pLightSources;

void displayfun(){
    
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    glEnable(GL_DEPTH_TEST);
    
    glViewport(0, 0, 1000, 500);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30, 2, 5, 1000);
    //------------------------------------
    glMatrixMode(GL_MODELVIEW);
    
    glLoadIdentity();
    classControl.setLookAt();
    glBegin(GL_TRIANGLES);
    for(auto & face : pMesh->Faces){
        glColor3d(face.Material.Ka.R,face.Material.Ka.G,face.Material.Ka.B);
        for(int i =0;i<3;i++){
            const Point_3 & Point = face.Vertexes[i].Position();
            glVertex3d(Point[0], Point[1], Point[2]);
        }
    }
    glEnd();
    glColor3d(1, 0, 0);
    glBegin(GL_LINES);
    if(pSegmentsToDraw){
        for(auto l:(*pSegmentsToDraw)){
            glVertex3d(l.source().x(), l.source().y(), l.source().z());
            glVertex3d(l.target().x(), l.target().y(), l.target().z());
        }
    }
    if(pCamera){
        for(int i=0;i<3;i++){
            glVertex3d(pCamera->ProjectionPlane[i].x(), pCamera->ProjectionPlane[i].y(), pCamera->ProjectionPlane[i].z());
            glVertex3d(pCamera->ProjectionPlane[i+1].x(), pCamera->ProjectionPlane[i+1].y(), pCamera->ProjectionPlane[i+1].z());
        }
        glVertex3d(pCamera->ProjectionPlane[3].x(), pCamera->ProjectionPlane[3].y(), pCamera->ProjectionPlane[3].z());
        glVertex3d(pCamera->ProjectionPlane[0].x(), pCamera->ProjectionPlane[0].y(), pCamera->ProjectionPlane[0].z());
        glVertex3d(pCamera->Center.x(), pCamera->Center.y(), pCamera->Center.z());
        glVertex3d((pCamera->Center+pCamera->Vector*5).x(), (pCamera->Center+pCamera->Vector*5).y(), (pCamera->Center+pCamera->Vector*5).z());
    }
    glEnd();
    glBegin(GL_TRIANGLES);
    if(pLightSources){
        for(auto & i:pLightSources->FaceLightSources){
            glColor3d(i.Color.R,i.Color.G,i.Color.B);
            const Triangle_3 & triangle = i.Triangle;
            glVertex3d(triangle[0].x(), triangle[0].y(),triangle[0].z());
            glVertex3d(triangle[1].x(), triangle[1].y(),triangle[1].z());
            glVertex3d(triangle[2].x(), triangle[2].y(),triangle[2].z());
        }
    }
    glEnd();
    glutSwapBuffers();
}
void InitScene(int argc, char ** argv){
    glutInit(&argc,argv);
};


void DrawSence
(const Mesh_t & Mesh,Scene_t * inpScene=nullptr,Camera_t * inpCamera= nullptr,std::vector<Segment_3> * inpSegmentsToDraw=nullptr){
    pMesh = & Mesh;
    pSegmentsToDraw=inpSegmentsToDraw;
    pCamera=inpCamera;
    pLightSources=inpScene->pLightSources;
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
    classControl.InitWindowSize(1000, 500);
    
    glutInitWindowPosition(0,0);
    glutCreateWindow("Scene");
    
    classControl.setPos(50,50,50);
    classControl.setUp(-1,2,-1);
    classControl.setCenter(0,0,0);
    classControl.setMoveStepCoefficient(0.2);
    classControl.setRotationStepCoefficient(1.5);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    glutSpecialFunc(DelagatekeyRotateByCenter);
    glutKeyboardFunc(DelegatekeyMove);
    glutDisplayFunc(displayfun);
    glutMouseFunc(DelegateMousePress);
    glutMotionFunc(DelegateMouseMoveByCenter);
    
    glutMainLoop();
}



