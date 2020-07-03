//
//  Material.cpp
//  CoolRender
//
//  Created by 唐如麟 on 15/8/2.
//  Copyright © 2015年 唐如麟. All rights reserved.
//
#include <fstream>
#include <sstream>
#include <iostream>
#include "Material.hpp"

Materials_t::
Materials_t(const std::string & inFilename):_Filename(inFilename){
    std::ifstream MaterialsFile(_Filename);
    if(!MaterialsFile){
        perror("open materials file fail");
        exit(-1);
    }
    std::string Line;
    std::stringstream LineStream;
    std::string Command;
    Material_t * pMaterial = nullptr;
    while(!MaterialsFile.eof()){
        getline(MaterialsFile, Line);
        if(Line.empty()) continue;
        LineStream.clear();
        LineStream.str(Line);
        LineStream >> Command;
        if(!strcmp(Command.data(), "Kd")){
            LineStream >> pMaterial->Kd.R >> pMaterial->Kd.G >> pMaterial->Kd.B;
            continue;
        }
        if(!strcmp(Command.data(), "Ks")){
            LineStream >> pMaterial->Ks.R >> pMaterial->Ks.G >> pMaterial->Ks.B;
            continue;
        }
        if(!strcmp(Command.data(), "Tf")){
            LineStream >> pMaterial->Tf.R >> pMaterial->Tf.G >> pMaterial->Tf.B;
            continue;

        }
        if(!strcmp(Command.data(), "Ni")){
            LineStream >> pMaterial->Ni;
            continue;

        }
        
        pMaterial=&(_MaterialMap.insert(std::pair<std::string,Material_t>(Command,Material_t())).first->second);
        std::cout<<"Macterial: "<<Command<<std::endl;
    }
}

 Material_t&
Materials_t::
operator[](const std::string & inMaterialName){
    std::map<std::string, Material_t>::iterator iMaterial;
    iMaterial = _MaterialMap.find(inMaterialName);
    if(iMaterial==_MaterialMap.end()){
        perror("wrong material name");
        exit(-1);
    }
    return iMaterial->second;
}

